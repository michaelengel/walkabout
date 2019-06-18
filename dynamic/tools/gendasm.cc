/*
 * Copyright (C) 2001, Sun Microsystems, Inc
 *
 * See the file "LICENSE.TERMS" for information on usage and
 * redistribution of this file, and for a DISCLAIMER OF ALL
 * WARRANTIES.
 *
 */

/*=============================================================================
 * FILE:       gendasm.cc
 * OVERVIEW:   Generate code for instruction disassembly
 *===========================================================================*/

/* $Revision: 1.4 $
 * 23 Apr 01 - Nathan: Initial version
 */


#include "codegenemu.h"
#include <stdio.h>

#define MAXINTWIDTH 15

string CodeGenEmu::sprintDisassembleInst( const InstIdent &inst, bool callName=false )
{
    vector<OperandIdent *>::const_iterator i;
    string s, line;    
    int j = 1;

    /* Unfortunately this gets fairly language specific. */
    if( lang->getLanguageID() == LANG_JAVA ) {
        /* Avoids a lot of stuffing around (And req. for Java) */
        i = inst.params.begin();
        for( const char *p = inst.disasm.begin(); p != inst.disasm.end(); p++ ) {
            if( *p == '%' ) {
                p++;
                if( *p == 's' ) {
                    if( s.length() > 0 ) {
                        line += "out.write( \"" + s + "\" );\n";
                        s = "";
                    }
                    string expr = "\"\"";
                    if( (*i)->expr && (*i)->expr->getKind() == PATTERN ) {
                        expr = prefix + "disassemble" +
                            ((PatternIdent *)(*i)->expr)->name +
                            "( " + (*i)->name + ", out )";
                    } else if( (*i)->isImmed && !callName ) {
                        expr = string("out.write( IMMEDTOSTRING( ") +
                            (*i)->name + "))";			    
                    } else if( (*i)->isImmed ) {
                        expr = string("out.write( callName( ") +
                            (*i)->name + "))";
                    } else if( (*i)->expr ) {
                        expr = string("out.write(") +
                            ((FieldIdent *)(*i)->expr)->name + "Names[" +
                            (*i)->name + "] )";
                    }
                    line += expr + ";\n";
                    i++;
                } else {
                    s += *p;
                }
            } else {
                s += *p;
            }
        }
        if( s.length() > 0 )
            line += "out.write( \"" + s + "\" );\n";
        return "{ " + line + " opname = \"" + inst.name + "\"; }\n";
    } else {
        /* use snprintf. If adding a language with neither strings, nor
         * printf, you'll probably need another case
         */
        s = "{\n";
        for( i = inst.params.begin(); i != inst.params.end(); i++, j++ ) {
            if( (*i)->expr && (*i)->expr->getKind() == PATTERN ) {
                s += string("char tmp") + j + "[" +
                    (((PatternIdent *)(*i)->expr)->maxDisasmLen+1) + "];\n";
            } else if( (*i)->isImmed ) {
                s += string("char tmp") + j + "[" + MAXINTWIDTH + "];\n";
            }
        }
    
        j = 1;
        for( i = inst.params.begin(); i != inst.params.end(); i++, j++ ) {
            if( (*i)->expr && (*i)->expr->getKind() == PATTERN ) {
                s += prefix + "disassemble" + ((PatternIdent *)(*i)->expr)->name +
                    "( " + (*i)->name + ", tmp" + j + ", sizeof(tmp" + j +
                    "), NULL );\n";
            } else if( (*i)->isImmed ) {
                s += string("IMMEDTOSTRING( tmp") + j + ", " + (*i)->name + ", " +
                    MAXINTWIDTH + " );\n";
            }
        }
        if( inst.disasm.length() == 0 ) {
            s += "buf[0] = '\\0';\n";
        } else {
            s += "snprintf( buf, buflen, \"" + inst.disasm + "\"";
            j = 1;
            for( i = inst.params.begin(); i != inst.params.end(); i++, j++ ) {
                s += ", ";
                if( (*i)->expr && (*i)->expr->getKind() == FIELD && !(*i)->isImmed) {
                    s += ((FieldIdent *)(*i)->expr)->name + "Names[" + (*i)->name + "]";
                } else {
                    s += string("tmp") + j;
                }
            }
            s += ");\n";
        }
        s += "if( opname ) *opname = \"" + inst.name + "\";\n}\n";
        return s;
    }
}

string CodeGenEmu::sprintDisassembleTypedParam( const string &name,
                                                const ListExpr &list )
{
    string lines;
    params_t params( "int", "pc" );
    if( lang->getLanguageID() == LANG_JAVA ) {
        params.push_back( "Writer", "out" );
    } else {
        params.push_back( "char *", "buf" );
        params.push_back( "size_t", "buflen" );
        params.push_back( "char **", "opname" );
    }

    for( ListExpr::const_iterator it = list.exprs.begin();
         it != list.exprs.end(); it++ ) {
        assert( (*it)->getKind() == INSTRUCTION );
        InstIdent *inst = (InstIdent *)(*it);
        lines += sprintMatchLine( inst->name, inst->params,
                                  sprintDisassembleInst( *inst ) );
    }

    return lang->sprintFunctionDefn( Type(), prefix + "disassemble" + name, params,
                                     sprintMatchStmt( lines, "{ BADOPCODE(pc); }", true ) );
}

string CodeGenEmu::sprintDisassembleTypedParams( const string &type,
                                                 map<string,bool> &types ) {
    string lines;
    if( types[type] ) return ""; /* already seen */
    types[type] = true;
    ListExpr *list = (ListExpr *)sled.types[type]->expr;
    assert( list->getKind() == LIST );
    for( ListExpr::const_iterator lit = list->exprs.begin();
         lit != list->exprs.end(); lit++ ) {
        InstIdent *inst = (InstIdent *)*lit;
        assert( inst->getKind() == INSTRUCTION );
        for( vector<OperandIdent *>::iterator param = inst->params.begin();
             param != inst->params.end(); param++ ) {
            
            if( types.find( (*param)->name ) != types.end() )
                lines += sprintDisassembleTypedParams(
                    (*param)->name, types );
        }
    }

    lines += sprintDisassembleTypedParam( type, *list );
    return lines;
}
        
string CodeGenEmu::sprintDisassembleParams( )
{
    string lines;
    for( SledDict::field_iterator it = sled.fields.begin();
         it != sled.fields.end(); it++ ) {
        if( it->second->hasNames() ) {
            list<string> names;
            FieldIdent *fi = (FieldIdent *)it->second;
            for( unsigned i=0; i<fi->names->size(); i++ ) {
                if( (*fi->names)[i] == NULL ) names.push_back("ERR");
                else names.push_back( *(*fi->names)[i] );
            }
            lines += lang->sprintConstStringList( fi->name + "Names", names );
        }
    }

    /* Get a list of typed parameters */
    map<string,bool> types;
    for( SledDict::type_iterator it = sled.types.begin();
         it != sled.types.end(); it++ ) {
        if( it->second->expr->type == TYPE_LIST ) {
            ListExpr *list = (ListExpr *)it->second->expr;
            if( !list->exprs.empty() &&
                list->exprs[0]->getKind() == INSTRUCTION ) {
                /* finally... this is a typed-constructor pattern */
                types[it->first] = false;
            }
        }
    }

    for( map<string,bool>::iterator it = types.begin();
         it != types.end(); it++ ) {
        lines += sprintDisassembleTypedParams( it->first, types );
    }

    return lines;
}

/*
 * Generate a .m input file suitable for NJMC
 */

string CodeGenEmu::sprintDisassembleFunc( )
{
    
    string lines;

    // I want to put a limit on the number of lines of match
    // allowed in one method (or else Java will fail since
    // Java allows a maximum method size of 64K)

    if (disInsts.empty()) 
    	disInsts = sled.orderInstructions();

    if (disIt == NULL) {
       disIt = disInsts.begin();
    }
    else if (disIt == disInsts.end()) {
	return "return 0;"; 
    }
    else {
	disIt++;
	if (disIt == disInsts.end()) {
	   return "return 0;";
	}
    }

    if( lang->getLanguageID() == LANG_JAVA ) { 
	int tempCount = 0;
        for (; disIt != disInsts.end(); disIt++ ) {
            if( (*disIt)->type == NULL ) {
                bool callName = false;
		
		string paramString;		    
		string::iterator stringIt;
		int charCounter = 0;
		for (stringIt = (*disIt)->name.begin();
		     stringIt != (*disIt)->name.end();
		     ++stringIt){
		     paramString.append(1, (char)tolower(*stringIt));
		     if (++charCounter >= 4)
		         break;
		     		     
		}
	    	if (paramString == "call")
		    callName = true;			
	    
                lines += sprintMatchLine( (*disIt)->name, (*disIt)->params,
                                      sprintDisassembleInst( **disIt, callName) );
            }
            if (tempCount++ > 80)
	        break;
	}
    }
    else {
        for (; disIt != disInsts.end(); disIt++ ) {
            if( (*disIt)->type == NULL ) {
                lines += sprintMatchLine( (*disIt)->name, (*disIt)->params,
                                      sprintDisassembleInst( **disIt ) );
            }
        }
    }

    if (disIt != disInsts.end()) {
	// This only works up to 9!
	char tempC = ++disIteration + 48;
	string tempString(1, tempC);
        return sprintMatchStmt( lines, "{nextPC = disassembleInstruction" 
					+ tempString + "(pc, out); }", true); 
    }
    return sprintMatchStmt( lines, "{ BADOPCODE(pc); }", true );
}
