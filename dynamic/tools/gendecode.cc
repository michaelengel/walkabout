/*
 * Copyright (C) 2001, Sun Microsystems, Inc
 *
 * See the file "LICENSE.TERMS" for information on usage and
 * redistribution of this file, and for a DISCLAIMER OF ALL
 * WARRANTIES.
 *
 */

/*=============================================================================
 * FILE:       gendecode.cc
 * OVERVIEW:   routines to generate matching statements suitable for processing
 *             by the New Jersey toolkit, for instructions and operands.
 *===========================================================================*/

/* $Revision: 1.4 $
 * 07 Feb 01 - Nathan: Initial version
 * 31 Jul 01 - Brian: New class HRTL replaces RTlist. Renamed LRTL to HRTLList.
 * 29 Aug 01 - Bernard: Added inline to generated execute and decode functions 
 */

#include "codegenemu.h"

static CodeGenEmu *gen; /* This is kinda dodgy really */

string mangleMatchParam( const string &s )
{
    return gen->mangleParam(s);
}

string CodeGenEmu::sprintMatchStmt( const string &body, const string &other,
                                    bool returnNextPC )
{
    string s;
    if( returnNextPC ) s = lang->sprintVariableDefn(Type(),"nextPC") +
                           "match [nextPC] pc to\n";
    else s = "match pc to\n";
    return s + body + (other.length()>0 ? "else " + other : "") +
        ";\nendmatch\n" + (returnNextPC ? "return nextPC;\n" : "");
}

string CodeGenEmu::sprintDecodeTypedParam( const string &name,
                                           const list<int> &opts, bool lhs )
{
    string lines;
    int size = 0;
    /* First run through the variants and take the overall parameter
     * type as the largest integer needed to hold all possible options
     * (We don't really have a way of dealing with floating point
     * at the moment)
     */
    string typeName;
    for( list<int>::const_iterator i = opts.begin(); i != opts.end(); i++ ) {
        if( typeName == "" && decodeParamType.find(*i) != decodeParamType.end() ) {
            typeName = decodeParamType[*i];
        }
        if( ssl.DetParamMap.find(*i) != ssl.DetParamMap.end() ) {
            ParamEntry &ent = ssl.DetParamMap[*i];
            if( ent.rta != NULL ) {
                int sz = ent.rta->getType().getSize();
                if( sz > size ) size = sz;
            }
        }
    }
    Type type = paramType; /* Default */
    if( size != 0 )
        type = SIGNED(size);
    ssl.DetParamMap[ssl.ParamMap[name]].type = type;
    
    for( list<int>::const_iterator i = opts.begin(); i != opts.end(); i++ ) {
        string expr = "return 0;\n"; /* Default */
        if( ssl.DetParamMap.find(*i) != ssl.DetParamMap.end() ) {
            ParamEntry &ent = ssl.DetParamMap[*i];
            if( ent.kind == PARAM_LAMBDA ) {
                expr = decodeParamExpr[*i];
            } else if( ent.rta != NULL ) {
                expr = sprintSemStr( *ent.rta->getRHS(), type.getSize() );
            } else if( ent.params.size() > 0 ) {
                expr = mangleParam(theSemTable[*ent.params.begin()].sName);
            }
            if( lhs ) {
	    	/* Bernard: I added an explicit cast to the appropriate 
		 * type on return, otherwise gcc complains. Only needed
		 * for casting from one type of memory pointer to another
		 * it seems.
		 */		
                lines += sprintMatchLine (theSemTable[*i].sName, ent.params,
                                          string("return (") + typeName + " *) "
                                          + lang->sprintAddressOf(expr) + ";\n");
            } else {
                lines += sprintMatchLine (theSemTable[*i].sName, ent.params,
                                          string("return ")
                                          + expr + ";\n");
            }
        } else {
            /* _bad_ user :) */
            lines += sprintMatchLine( theSemTable[*i].sName, list<int>(),
                                      "return 0;\n" );
        }
    }
    if( typeName == "" )
        typeName = lang->sprintType(type);
    decodeParamType[ssl.ParamMap[name]] = typeName;

    // Don't generate "inline" for Java    
    if (lang->getLanguageID() == LANG_JAVA) {
	return lang->sprintFunctionDefn( typeName +
                        	   (lhs ? " *" : ""), prefix + "decode" + name,
                        	   params_t( lang->sprintType(paramType),"pc"),
                        	   sprintMatchStmt(lines,"return 0",false) );    
    }
    
    return lang->sprintFunctionDefn( typeName +
                               (lhs ? " *" : ""), "inline " + prefix + 
			       "decode" + name,
                               params_t( lang->sprintType(paramType),"pc"),
                               sprintMatchStmt(lines,"return 0",false) );
}

void CodeGenEmu::computeDecodeParam( )
{
    decodeParamFuncs = "";
    gen = this;
    findLHSParams();
    
    map<string, int, less<string> >::iterator i;
    for( i = ssl.ParamMap.begin(); i != ssl.ParamMap.end(); i++ ) {
        decodeParamExpr[i->second] = lang->sprintTypeChange(Type(), ssl.DetParamMap[i->second].type, mangleParam(i->first) );
        decodeParamType[i->second] = lang->sprintType(ssl.DetParamMap[i->second].type);
        if( ssl.DetParamMap.find(i->second) != ssl.DetParamMap.end() ) {
            ParamEntry &ent = ssl.DetParamMap[i->second];
            if( ent.kind == PARAM_LAMBDA ) {
                string body = lang->sprintReturn(
                    sprintSemStr( *ent.rta->getRHS(), ent.type.getSize() ));
                params_t p, fp;
                for( list<int>::iterator it = ent.params.begin();
                     it != ent.params.end(); it++ ) {
                    p.push_back( lang->sprintType(ssl.DetParamMap[*it].type),
                                 mangleParam(theSemTable[*it].sName) );
                }
                for( list<int>::iterator it = ent.funcParams.begin();
                     it != ent.funcParams.end(); it++ ) {
                    fp.push_back( lang->sprintType(ssl.DetParamMap[*it].type),
                                  mangleParam(theSemTable[*it].sName) );
                }
                decodeParamFuncs +=
                    lang->sprintClosureDefn( lang->sprintType(ent.type),
                                             i->first, p, fp, body );
                decodeParamExpr[i->second] =
                    lang->sprintClosureCreate( i->first, p );
                decodeParamType[i->second] =
                    lang->sprintClosureType( lang->sprintType(ent.type),
                                             i->first, fp );
            }
        }
    }

    /* Do the typed params last, as they may depend on the earlier bits */
    for( i = ssl.ParamMap.begin(); i != ssl.ParamMap.end(); i++ ) {
        if( ssl.DetParamMap.find(i->second) != ssl.DetParamMap.end() ) {
            ParamEntry &ent = ssl.DetParamMap[i->second];
            if( ent.kind == PARAM_VARIANT ){ /* param list only */
                decodeParamFuncs +=
                    sprintDecodeTypedParam( i->first, ent.params, ent.lhs );
                decodeParamExpr[i->second] =
                    lang->sprintFunctionCall( prefix + "decode" + i->first,
                                              params_t("",mangleParam(i->first)) );
            }
        }
    }
}

/*
 * This routine is part of an attempt to deal with rtls which assign to
 * the instruction parameters - which is fine in a substitution
 * based framework but needs special handling when translating to
 * an imperative langauge.
 *
 * Here we just identify any parameters that are used in this way
 * by scanning through all the instructions for an idParam on the LHS.
 */
void CodeGenEmu::findLHSParams( )
{
    map<string, TableEntry, less<string> >::iterator it;
    for( it = ssl.idict.begin(); it != ssl.idict.end(); it++ ) {
        HRTL &rtl = it->second.hrtl;
        for( int i = 0; i < rtl.getNumRT(); i++ ) {
            RTAssgn *rta = (RTAssgn *)rtl.elementAt(i);
            if( rta->getKind() == RTASSGN &&
                rta->getLHS() != NULL ) {
                /* Don't worry about flag calls for the moment */
                SemStr *lhs = rta->getLHS();
                if( lhs->getFirstIdx() == idParam ) {
                    int lparam = lhs->getSecondIdx();
#ifdef DEBUG                    
                    if( ! ssl.DetParamMap[lparam].lhs )
                        fprintf( stderr, "LHS Param detected: %s\n",
                                 theSemTable[lparam].sName.c_str() );
#endif DEBUG
                    ssl.DetParamMap[lparam].lhs = true;
                }
            }
        }
    }
}

/*
 * Generate a .m input file suitable for NJMC
 */
string CodeGenEmu::sprintExecuteFunc( )
{
    string lines;
    map<string,string> code;
    
    /* Generate the matching statements */
    map<string, TableEntry, less<string> >::iterator it;
    for( it = ssl.idict.begin(); it != ssl.idict.end(); it++ ) {
        if( slednamemap.find(it->first) == slednamemap.end() )
            continue;
        params_t params;
        for( list<int>::iterator i = it->second.params.begin();
             i != it->second.params.end(); i++ )
            params.push_back(lang->sprintType(Type()), decodeParamExpr[*i]);
        if( it->second.flags & TEF_NEXTPC )
            params.push_back( lang->sprintType(paramType), "nextPC" );
        
        code[slednamemap[it->first]] = sprintMatchLine( slednamemap[it->first],
                                                        it->second.params,
            lang->sprintFunctionCall( prefix + "execute" + it->first,
                                      params ) + ";\n" );
    }

    /* Now sort them by precedence */
    list<InstIdent *> insts = sled.orderInstructions();
    list<InstIdent *>::iterator lit;
    for( lit = insts.begin(); lit != insts.end(); lit++ ) {
        if( code.find( (*lit)->name ) != code.end() )
            lines += code[ (*lit)->name ];
    }

    // Don't generate "inline" for Java    
    if (lang->getLanguageID() == LANG_JAVA) {    
	return lang->sprintFunctionDefn(Type(), prefix + "execute",
                               params_t(lang->sprintType(Type()),"pc"),
                               sprintMatchStmt(lines,"unimplemented(pc)",true));
    }					

    return lang->sprintFunctionDefn(Type(), "inline " + prefix + "execute",
                               params_t(lang->sprintType(Type()),"pc"),
                               sprintMatchStmt(lines,"unimplemented(pc)",true));
}

