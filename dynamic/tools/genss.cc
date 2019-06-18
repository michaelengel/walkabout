/*
 * Copyright (C) 2001, Sun Microsystems, Inc
 *
 * See the file "LICENSE.TERMS" for information on usage and
 * redistribution of this file, and for a DISCLAIMER OF ALL
 * WARRANTIES.
 *
 */

/*=============================================================================
 * FILE:       genss.cc
 * OVERVIEW:   Generate emulator code from semantic strings. Also contains the
 *             top level routines for generating instructions and flag calls.
 *===========================================================================*/

/* $Revision: 1.4 $
 * 07 Feb 01 - Nathan: Initial version
 * 24 Apr 01 - Nathan: Fixed some casting problems
 * 01 May 01 - Nathan: Changed flag calls to substitute directly rather than
 *                relying on macros
 * 21 May 01 - David: CodeGenEmu::sprintBodyCode added to allow addition
 *              generation of instrumented code.
 * 22 May 01 - David: Added check to output misc instrm code in
 *              CodeGenEmu::sprintInstructions
 * 23 May 01 - David: Added check to output instrm code in 
 *              CodeGenEmu::sprintExecuteOneFunc
 * 06 Jun 01 - David: Modifed CodeGenEmu::sprintBodyCode to handle new syntax 
 *              for instrumentation
 * 20 Jun 01 - David: make generation of PARAM with a prefix "op_"
 * 31 Jul 01 - Brian: New class HRTL replaces RTlist. Renamed LRTL to HRTLList.
 */

#include <ctype.h>
#include <stdlib.h>
#include "codegenemu.h"

typedef list<SemStr *>::const_iterator SSPCIT;

double ints2dbl(int,int);

string CodeGenEmu::sprintRegisterAssign( SSCIT &in, SSCIT &end,
                                         const SemStr &rhs, type_it &type )
{
    type++;
    if( *in == idTemp ) {
        in++; type++;
        return lang->sprintAssign( theSemTable[*in++].sName,
                                   sprintSemStr(rhs,type) );
    } else if( *in == idIntConst ) {
        in++; type++;
        return searchAndReplace( setreg[ssl.DetRegMap[*in++].g_name()], "%s",
                                 sprintSemStr(rhs,type) );
    } else if( *in == idParam ) {
        in++; type++;
        const string &param = theSemTable[*in++].sName;
        return searchAndReplace(
            searchAndReplace( setidxreg[param], "%s", sprintSemStr(rhs, type) ),
            "%r", mangleParam(param) );
    } else {
        int block = ssl.matchRegIdx(SemStr(in,end));
        string reg = sprintSemStr(in,end,type) + "- " +
            *ssl.DetParamMap[block].regIdx.begin();
        return searchAndReplace(
            searchAndReplace( setidxreg[theSemTable[block].sName],
                              "%s", sprintSemStr(rhs,type) ),
            "%r", reg );
    }
}

string CodeGenEmu::sprintRegisterRead( SSCIT &in, SSCIT &end, type_it &type )
{
    type++;
    if( *in == idTemp ) {
        in++; type++;
        return theSemTable[*in++].sName;
    } else if( *in == idIntConst ) {
        in++; type++;
        int id = *in++;
        return getreg[ssl.DetRegMap[id].g_name()];
    } else if( *in == idParam ) {
        in++; type++;
        const string &param = theSemTable[*in++].sName;
        return searchAndReplace( getidxreg[param], "%r", mangleParam(param) );
    } else {
        int block = ssl.matchRegIdx(SemStr(in,end));
        assert( block != -1 );
        string reg = sprintSemStr(in,end,type) + " - " +
            *ssl.DetParamMap[block].regIdx.begin();
        return searchAndReplace( getidxreg[theSemTable[block].sName],
                                 "%r", reg );
    }
}
string CodeGenEmu::sprintMemoryAssign( SSCIT &in, SSCIT &end,
                                       const SemStr &rhs, type_it &type )
{
    const Type &memtype = *type++;
    string index = sprintSemStr(in, end, type);
    const Type &exprtype = *type;
    return lang->sprintMacroCall( "setMem" + lang->sprintType(memtype),
                                  params_t( lang->sprintType(addressType),
                                            index, lang->sprintType(exprtype),
                                            sprintSemStr(rhs,type) ) ) +
        ";\n";
}
string CodeGenEmu::sprintMemoryRead( SSCIT &in, SSCIT &end, type_it &type )
{
    const Type &memtype = *type++;
    return lang->sprintMacroCall( "getMem" + lang->sprintType(memtype),
                                  params_t(lang->sprintType(addressType),
                                           sprintSemStr(in,end,type)));
}

string CodeGenEmu::sprintOperationCall( const Type &type, int id,
                                       const string &lhs, const string &rhs )
{
    string name = string(type.getType() == FLOATP ? "float" : "int") +
        type.getSize();
    const string *l = &lhs, *r = &rhs;
    switch(id) {
        case idEquals: name += "Equals"; break;
        case idNotEqual: name = "!" + name + "Equals"; break;
        case idGtr:  l = &rhs; r = &lhs; /* fallthrough */
        case idLess: name += "Less"; break;
        case idLessEq: l = &rhs; r = &lhs; /* fallthrough */
        case idGtrEq: name = "!" + name + "Less"; break;

        case idPlus: case idFPlus: name += "Add"; break;
        case idMinus: case idFMinus: name += "Sub"; break;
        case idMult: case idFMult: name += "Mul"; break;
        case idDiv: case idFDiv: name += "Div"; break;
        case idMod: name += "Mod"; break;
    }
    return name + "( (" + *l + "), (" + *r + ") )";
}

/* This is mostly similar to doSprintInfix in ss.c, but we need to handle
 * register & memory accesses differently, so...
 *
 * type may only be changed if the existing value is UNKNOWN_TYPE
 */
string CodeGenEmu::sprintSemStr( SSCIT &it, SSCIT &end, type_it &type )
{
    if( it == end ) return "";

    int idx = *it;
    const char *opname;
    const SemItem &si = theSemTable[*it++];
    Type optype, childType;

    switch( si.cat ) {
        case cCONSTANT:
            switch( idx ) {
                case idFltConst: {
                    type++;
                    // There are two ints that represent the double
                    int i1 = *it++;
                    int i2 = *it++;
                    double f = ints2dbl(i1, i2);
                    return lang->sprintLiteral(f);
                }
                case idIntConst:
                    if( type->getSize() > 32 ) {
                        long long iConst = *it++;
                        type++;
                        return lang->sprintLiteral(iConst);
                    } else {
                        type++;
                        int iConst = *it++;
                        return lang->sprintLiteral(iConst);
                    }
                    
            }
            assert(0);
        case cOPERATOR:
            opname = si.sName.c_str();
            switch( si.iNumVarArgs ) {
                case 1:
                    type++;
                    if( opname[0] == '~' ) {
                        opname = "!";
                        /* Special case because java for some bizarre reason
                         * refuses to permit !x where x is anything but a
                         * boolean...
                         */
                        if( lang->getLanguageID() == LANG_JAVA &&
                            type->getType() != BOOLEAN ) {
                            return string("(") + sprintSemStr( it, end, type ) +
                                " == 0 ? 1 : 0)";
                        }
                    }
                    return string(" ") + opname + " " + sprintSemStr( it, end, type );
                    break;
                case 2: {
                    if( idx == idEquals && *it == idNot ) {
                        it++;
                        opname = "!=";
                    } else if( idx == idAnd ) opname = "&&";
                    else if( idx == idOr ) opname = "||";
                    else if( idx == idEquals ) opname = "==";
                    else if( idx == idNotEqual ) opname = "!=";
                    else if( idx == idShiftRA ) opname = ">>";
                    else if( idx == idShiftR && 
                             lang->getLanguageID() == LANG_JAVA )
                        opname = ">>>";
                    else if( opname[0] == '+' ) opname = "+";
                    else if( opname[0] == '-' ) opname = "-";
                    else if( opname[0] == '*' ) opname = "*";
                    else if( opname[0] == '/' ) opname = "/";

                    optype = *type++;
                    childType = *type;

                    string l = sprintSemStr(it,end,type);
                    string r = sprintSemStr(it,end,type);
                    string expr;
                    if( lang->isTypeSupported( childType ) )
                        expr = string("(%l ") + opname + " %r)";
                    else
                        expr = sprintOperationCall( childType, idx,
                                                    "%l", "%r" );
                    switch(idx) {
                        case idMult: case idDiv: case idMod:
                        case idLessUns: case idGtrUns: case idLessEqUns:
                        case idGtrEqUns: case idShiftR:
                            /* unsigned operations */
                            if( childType.getSigned() == true ) {
                                Type uType = childType;
                                uType.setSigned(false);
                                l = lang->sprintTypeCast(childType, uType, l);
                                r = lang->sprintTypeCast(childType, uType, r);
                                expr = lang->sprintTypeCast( uType, childType,
                                                             expr );
                            }
                            break;
                        case idMults: case idDivs: case idMods: case idShiftRA:
                        case idLess: case idGtr: case idLessEq: case idGtrEq:
                            /* Signed operations */
                            if( childType.getSigned() == false ) {
                                Type sType = childType;
                                sType.setSigned(true);
                                l = lang->sprintTypeCast(childType, sType, l);
                                r = lang->sprintTypeCast(childType, sType, r);
                                expr = lang->sprintTypeCast( sType, childType,
                                                             expr );
                            }
                            break;
                        case idRotateL:
                            expr = string("ROTATELEFT( %l, %r, ") +
                                childType.getSize() + " )";
                            break;
                        case idRotateR:
                            expr = string("ROTATERIGHT( %l, %r, ") +
                                childType.getSize() + " )";
                            break;
                    }

                    return searchAndReplace( searchAndReplace( expr, "%l", l ),
                                             "%r", r );
                    
                }
                case 3: {
                    if( idx == idTern) {
                        type++;
                        string a = sprintSemStr(it,end,type);
                        string b = sprintSemStr(it,end,type);
                        string c = sprintSemStr(it,end,type);
                        return "(" + a + " ? " + b + " : " + c + ")";
                    } else if( idx == idAt ) {
                        optype = *type++;
                        childType = *type;
                        string a = sprintSemStr(it,end,type);
                        string b;
                        if( *it == idIntConst ) {
                            it++; type++;
                            int lo = *it++;
                            if( *it == idIntConst) {
                                it++; type++;
                                int hi = *it++;
                                /* constant bitslice */
                                return lang->sprintTypeCast( childType, optype,
                                    lang->sprintGetBitslice( childType, a,
                                                             lo, hi ));
                            } else {
                                b = lang->sprintLiteral(lo);
                            }
                        } else {
                            b = sprintSemStr(it,end,type);
                        }
                        string c = sprintSemStr(it,end,type);
                        string expr = string("BITSLICE( ") + a + ", " + b +
                            ", " + c + " )";
                        return lang->sprintTypeCast( childType, optype,
                                                 expr );
                    }
                }
            }
            assert(0);
        case cFUNCTION:
        case cSPECIAL:
        case cOPERATION: /* Reg, mem, parm */
            switch( idx ) {
                case idParam: {
                    int param = *it++;
                    string expr = mangleParam(theSemTable[param].sName);
                    type++;
                    if( ssl.DetParamMap[param].lhs )
                        expr = lang->sprintDereference( expr );

                    /* This bit is to transform register operands into
                     * something which gives the ssl-equivalent number.
                     * Note the assumption that registers map linearly...
                     * (If anyone ever finds a sufficiently insane machine
                     * for which this doesn't hold, you'll need to do proper
                     * mapping tables I expect. Enjoy...)
                     */
                    if( ssl.DetParamMap[param].regIdx.size() > 0 )
                        expr = "( " + expr + " + " +
                            *ssl.DetParamMap[param].regIdx.begin() + " )";
                    return expr;
                }
                case idTemp:
                    assert(0); /* shouldn't happen */
                case idRegOf:
                    return sprintRegisterRead( it, end, type );
                case idMemOf:
                    return sprintMemoryRead( it, end, type );
                case idAddrOf:
                    return "ADDROF(" + sprintSemStr( it, end, type ) + ")";
                case idSize:
                    type++;
                    it++;
                    return sprintSemStr(it, end, type);
                case idZfill: case idTruncu: {
                    optype = *type++;
                    childType = *type;
                    it++; it++;
                    string expr = sprintSemStr( it, end, type );
                    if( idx == idZfill && !lang->hasExactType(childType) ) {
                        /* If we're dealing with, say, a 3 bit value,
                         * we need to mask it off to guarantee we
                         * don't accidentally get any extra bits
                         */
                        Type tempType = lang->getNearestType(childType);
                        expr = lang->sprintGetBitslice( tempType, expr, 0,
                            childType.getSize()-1);
                        childType = tempType;
                    }
                    if( childType != optype ) {
                        Type midtype1 = childType;
                        Type midtype2 = optype;
                        midtype1.setSigned(false);
                        midtype2.setSigned(false);
                        /* To ensure a properly z-filled number, we need to
                         * do a 3-stage cast. fun :)
                         */
                        expr = lang->sprintTypeCast( midtype2, optype,
                            lang->sprintTypeCast( midtype1, midtype2,
                            lang->sprintTypeCast( childType, midtype1,
                                                  expr )));
                    }
                    if( idx == idTruncu && !lang->hasExactType(optype) ) {
                        /* Similarly for truncation... */
                        expr = lang->sprintGetBitslice(
                            lang->getNearestType(optype), expr, 0,
                            optype.getSize()-1 );
                        
                    }
                    return expr;
                }
                case idSgnEx: case idTruncs: {
                    optype = *type++;
                    childType = *type;
                    it++; it++;
                    string expr = sprintSemStr( it, end, type );
                    if( idx == idSgnEx && !lang->hasExactType(childType) ) {
                        /* Similarly to zFill, we need to manually sign
                         * extend the value up to the next supported type
                         * (although this one is a leetle more complex)
                         * We _REALLY_ better hope expr doesn't have any
                         * side-effects...
                         */
                        expr = "( ~(" +
                            lang->sprintGetBitslice(
                                lang->getNearestType(childType), expr,
                                childType.getSize()-1,
                                childType.getSize()-1 ) +
                            " - 1) << " + childType.getSize() +
                            ") | " + expr;
                    }
                    Type midtype1 = childType;
                    Type midtype2 = optype;
                    midtype1.setSigned(true);
                    midtype2.setSigned(true);
                    /* In the truncation case this time though, we _want_
                     * the resultant containing type to be sign-filled
                     * anyway, so the compiler will do the right thing
                     * by default
                     */
                    expr = lang->sprintTypeCast( midtype2, optype,
                        lang->sprintTypeCast( midtype1, midtype2,
                        lang->sprintTypeCast( childType, midtype1,
                                              expr )));
                    return expr;
                }
                case idItof:  case idFtoi: case idFsize:
                    optype = *type++;
                    childType = *type;
                    it++; it++;
                    
                    return lang->sprintTypeCast( childType, optype,
                                          sprintSemStr( it, end, type ));
                case idForceInt: case idForceFlt:
                    optype = *type++;
                    childType = *type;
                    return lang->sprintTypeChange( childType, optype,
                                          sprintSemStr( it, end, type ));
                case idNEXT:
                    type++;
                    needNextPC = true;
                    return "nextPC";
                default:
                    type++;
                    if( getreg.find( si.sName ) != getreg.end() ) {
                        /* Special register */
                        return getreg[si.sName];
                    }
                    if( ssl.DetParamMap.find( idx ) != ssl.DetParamMap.end() ) {
                        ParamEntry &param = ssl.DetParamMap[idx];
                        assert( param.funcParams.size() != 0 );
                        /* Lambda invocation */
                        params_t p;
                        for( unsigned i=0; i < param.funcParams.size(); i++ ) {
                            p.push_back( lang->sprintType(*type),
                                         sprintSemStr( it, end, type ) );
                        }
                        return lang->sprintClosureCall( si.sName, p );
                    }
                    string s = si.sName;
                    for( int i=0; i < si.iNumIntArgs; i++ )
                        s = s + *it++ + ",";
                    for( int i=0; i < si.iNumFixedArgs; i++ )
                        s += theSemTable[*it++].sName + ",";
                   for( int i=0; i< si.iNumVarArgs; i++ )
                        s += sprintSemStr( it, end, type );
                    if( si.cat == cFUNCTION)
                        s += ")";
                    return s;
            }
        default:
            assert( 0 ); /* ce n'est possible? */
    }
    assert( 0 );
    return "";
}

string CodeGenEmu::sprintSemStr( const SemStr &ss )
{
    SemStr expr = ss;
    vector<Type> types = ssl.computeLHSTypes( expr );
    if( types.size() != 0 ) {
        type_it type = types.begin();
        return sprintSemStr( expr, type );
    }
    return "/* Type checking failed for RTAssgn */\n";
}    

string CodeGenEmu::sprintSemStr( const SemStr &ss, int size )
{
    SemStr expr = ss;
    vector<Type> types = ssl.computeRHSTypes( expr, size );
    if( types.size() != 0 ) {
        type_it type = types.begin();
        return sprintSemStr( ss, type );
    }
    return "/* Type checking failed for RTAssgn */\n";
}    

string CodeGenEmu::sprintSemStr( const SemStr &ss, const vector<Type> &types )
{
    type_it type = types.begin();
    return sprintSemStr( ss, type );
}

string CodeGenEmu::sprintSemStr( const SemStr &ss, type_it &type )
{
    SSCIT in, end;
    in = ((SemStr &)ss).getIndices()->begin(); /* Cast needed as getIndices */
    end = ((SemStr &)ss).getIndices()->end();  /* isn't const */
    return sprintSemStr( in, end, type );
}

string CodeGenEmu::sprintAssignRT( RTAssgn *rta )
{
    vector<Type> types = ssl.computeRTAssgnTypes(*rta);
    if( types.size() != 0 ) {
        rta->updateType(types[0]);
        type_it type = types.begin();
        return sprintAssignRT( *rta->getLHS(), *rta->getRHS(),
                               type );
    }
    return "/* Type checking failed for RTAssgn */\n";
}

string CodeGenEmu::sprintAssignRT( const SemStr &lhs, const SemStr &rhs,
                                   type_it &type )
{
    int idx = lhs.getFirstIdx();
    SSCIT in = ((SemStr &)lhs).getIndices()->begin();
    SSCIT end = ((SemStr &)lhs).getIndices()->end();
    if( idx == idAt ) {
        /* I _think_ this is the only case we have to handle specially */
        assert( *in == idAt );
        in++; type++;
        string a = sprintSemStr( in, end, type );
        string b = sprintSemStr( in, end, type );
        string c = sprintSemStr( in, end, type );
        string d = sprintSemStr( rhs, type );
        return string("BITSLICEASSN( ") + a + ", " + b + ", " + c + ", " + d +
            ");\n";
        assert( in == end );
    } else if( idx == idRegOf ) { /* Register assign */
        assert( *in == idRegOf );
        in++;
        return sprintRegisterAssign( in, end, rhs, type );
    } else if( idx == idMemOf ) { /* memory assign */
        assert( *in == idMemOf );
        in++; 
        return sprintMemoryAssign( in, end, rhs, type );
    } else { /* what else is there? :) */
        if( lhs.len() != 0 ) {
            string l = sprintSemStr( lhs, type );
            string r = sprintSemStr( rhs, type );
            if( theSemTable[idx].sName == "%SKIP" )  /* Yayness */
                return string("if( (") + r + ") == 1 ) return;\n";
            return lang->sprintAssign( l, r );
        } else {
            return sprintSemStr(rhs, type) + ";\n";
        }
    }
}

string CodeGenEmu::sprintBodyCode(string inst_name, const HRTL &rtl) {
    if (instrm_parser) {
        string instrm_code = "";
        
        // check for instructions to see if it needs instrumenting.
        map<string, table_code*>::iterator i =
            instrm_parser->inst2icode.find(inst_name);        
        
        if (i != instrm_parser->inst2icode.end()) {
            // do instrmentation code.
            
            list< pair<int, void*> >::iterator li;
            for (li=(*i).second->instrm_code.begin();
                 li!=(*i).second->instrm_code.end();  li++) {
                switch ((*li).first) {
                case T_CODE:
                    instrm_code += (char*)((*li).second);
                    break;
                case T_SSL:
                    instrm_code += (char*)((*li).second);
                    break;
                case T_SEMANTICS:
                    instrm_code += sprintRTLCode(rtl);
                    break;
                case T_PARAM: {
                    // look up the table and get the name of the variable         
                    map<string, TableEntry, less<string> >::iterator mi;
                    if ((mi = ssl.idict.find(inst_name)) == ssl.idict.end()) {
                        abort();
                    }
                    list<int>::const_iterator pi = mi->second.params.begin();
                    int parm = (unsigned)((*li).second);
                    while (parm) {
                        pi++;
                        parm --;
                    }
                    instrm_code += "op_" + theSemTable[*pi].sName;                    
                     break;
                }
                default:
                    abort();
                }
            }

            return instrm_code;            
            // return (*i).second->before + sprintRTLCode(rtl) + (*i).second->after;            
        } 
    } 
    return sprintRTLCode(rtl);
}


string CodeGenEmu::sprintRTLCode( const HRTL &rtl )
{
    RTAssgn *rta;
    RTFlagCall *rtf;
    string rtlcode = "";

    list<RT *>* trtl = new list<RT*>();
    rtl.deepCopyList(*trtl);
    ssl.transformPostVars(trtl, false); // Note: DON'T try to optimize
    set<int> declList;
    
    /* find temporaries */
    for( list<RT*>::iterator it = trtl->begin(); it != trtl->end(); it++ ) {
        if( (*it)->getKind() == RTASSGN ) {
            rta = (RTAssgn *)(*it);
            int tmpDefn[3] = { idRegOf, idTemp, WILD };
            int tmpidx = -1;
            if( rta->getLHS()->findSubExpr( 3, tmpDefn, tmpidx ) &&
                declList.find(tmpidx) == declList.end() ) {
                /* tmpidx references temporary identity */
                const string &tmpName = theSemTable[tmpidx].sName;
                rtlcode += lang->sprintVariableDefn( Type::getTempType(tmpName),
                                                     tmpName);
                declList.insert(tmpidx);
            }
        }
    }
    for( list<RT*>::iterator it = trtl->begin(); it != trtl->end(); it++ ) {
        RT *rt = (*it);
        switch( rt->getKind() ) {
            case RTASSGN:
                rtlcode += sprintAssignRT( (RTAssgn *)rt );
                break;
            case RTFLAGCALL: {
                rtf = (RTFlagCall *)rt;
                if( rtf->func_name == "TRAP" ) {
                    /* The old macro code is now only used to handle traps */
                    params_t params;
                    for( SSPCIT it = rtf->actuals.begin(); it != rtf->actuals.end(); it++ )
                        params.push_back( "", sprintSemStr(**it) );
                    rtlcode += lang->sprintMacroCall( string("doFlag") + rtf->func_name,
                                                      params ) + ";\n";
                } else {
                    vector<SemStr *> v(rtf->actuals.begin(), rtf->actuals.end());
                    RTFlagDef *def = ssl.FlagFuncs[rtf->func_name];
                    HRTL rts(0, ssl.instantiateRTL( *def->flag_rtl,
                                                  def->params, v ) );
                    rtlcode += "{" + sprintRTLCode(rts) + "}\n";
                }}
                break;
            
            default:
                fprintf( stderr, "Unknown RT in RTList" );
                assert(0);
        }
    }
    
    return rtlcode;
}

string CodeGenEmu::sprintInstructions( )
{
    string insts;
    // output misc code to the .m file if there are extra code at the end of the
    // instrm spec file.
    if (instrm_parser) {
        if (!instrm_parser->support_code.empty()) {
            insts += instrm_parser->support_code;
        } 
    } 
    map<string, TableEntry, less<string> >::iterator it;
    for( it = ssl.idict.begin(); it != ssl.idict.end(); it++ ) {
        params_t funcParams;
        for( list<int>::const_iterator i = it->second.params.begin();
             i != it->second.params.end(); i++ )
            funcParams.push_back( decodeParamType[*i] +
                                  (ssl.DetParamMap[*i].lhs ? " *" : "" ),
                                  mangleParam(theSemTable[*i].sName) );

        fprintf(stderr, "Generating execute%s...\n", it->first.c_str() );

        currentInst = &it->first;
        needNextPC = false;
        // string rtlCode = sprintRTLCode( it->second.rtl );
        string rtlCode = sprintBodyCode(it->first, it->second.hrtl );
        if( needNextPC ) {
            it->second.flags |= TEF_NEXTPC;
            funcParams.push_back( lang->sprintType(paramType), "nextPC" );
        }

        // Don't generate "inline" for Java    
        if (lang->getLanguageID() == LANG_JAVA) {	
            insts += lang->sprintFunctionDefn( VOID_TYPE, prefix + 
	                                      "execute" + it->first,
                                  	      funcParams, rtlCode );	
        }
	else {
	    insts += lang->sprintFunctionDefn( VOID_TYPE, "inline " + prefix + 
	                                      "execute" + it->first,
                                	      funcParams, rtlCode );
        }
	
        insts += '\n';
    }
    return insts;
}

string CodeGenEmu::sprintFlagCalls( )
{
    string flags;
    
    map<string, RTFlagDef*, less<string> >::iterator it;
    for( it = ssl.FlagFuncs.begin(); it != ssl.FlagFuncs.end(); it++ ){
        if( it->second->func_name != string("TRAP") ) {
            params_t funcParams;
            for( list<int>::const_iterator i = it->second->params.begin();
                 i != it->second->params.end(); i++ )
                funcParams.push_back( lang->sprintType(paramType),
                                      mangleParam(theSemTable[*i].sName) );
            flags += lang->sprintMacroDefn( string("doFlag") + it->first, funcParams,
                                      "{" + sprintRTLCode( *it->second->flag_rtl ) + "}" );
            flags += '\n';
        }
    }
    return flags;
}

string CodeGenEmu::sprintExecuteOneFunc( )
{
    if( ssl.fetchExecCycle ) {
        /*
        // check for instrm code for FetchExecute
        if (instrm_parser) {
            map<string, table_code*>::iterator i = 
                instrm_parser->inst2icode.find("$FETCHEXECUTE");        
            if (i != instrm_parser->inst2icode.end()) {
                return (*i).second->before + sprintRTLCode(*ssl.fetchExecCycle) 
                    + (*i).second->after;
            }
        } 
        return sprintRTLCode(*ssl.fetchExecCycle);
        */
        return sprintBodyCode("FETCHEXECUTE", *ssl.fetchExecCycle);
   } else
        return "";
}

SemStr CodeGenEmu::getSynthParamExpr( const string &name, InvokeExpr *invoke,
                                      int i, list<int> &sslparams )
{
    int n;
    Expr *e = invoke->params[i];
    if( e->type == TYPE_STRING ) {
        string &pname = invoke->inst->params[i]->name;
        if( sled.fields.find(pname) != sled.fields.end() &&
            (sled.fields[pname])->findName(
                ((StringExpr *)e)->value, &n ) ) {
            return SemStr(2, idIntConst, n );
        } else {
            fprintf( stderr, "Error: unrecognized string parameter to invocation '%s' in %s\n", ((StringExpr *)e)->value.c_str(), name.c_str() );
            return SemStr();
        }
    } else if( e->type == TYPE_INT ) {
        return SemStr(2, idIntConst, ((IntegerExpr *)e)->value );
    } else if( e->getKind() == OPERAND ) {
        string &pname = ((OperandIdent *)e)->name;
        unsigned j;
        list<int>::iterator pit = sslparams.begin();
        for( j=0; j < sslparams.size(); j++, pit++ ) {
            if( invoke->inst->params[j]->name == pname ) {
                return SemStr(2, idParam, *pit);
            }
        }
        if( j == sslparams.size() ) {
            if( ssl.ParamMap.find(pname) != ssl.ParamMap.end() ) {
                return SemStr(2, idParam, ssl.ParamMap[pname]);
            } else {
                fprintf( stderr, "Error: unrecognized operand parameter to invocation '%s' in %s\n", pname.c_str(), name.c_str() );
                return SemStr();
            }
        }
    } else if( e->getKind() == INVOKER ) {
        /* Here be dragons... this had better be a param */
        string &pname = ((InvokeExpr *)e)->inst->name;
        if( ssl.ParamMap.find(pname) != ssl.ParamMap.end() ) {
            ParamEntry pent = ssl.DetParamMap[ssl.ParamMap[pname]];
            if( pent.kind == PARAM_EXPR ) {
                SemStr pbase = *pent.rta->getRHS();
                list<int>::iterator pit = pent.params.begin();
                for( unsigned j = 0; j < pent.params.size(); j++, pit++ ) {
                    SemStr find(2,idParam, *pit);
                    SemStr replace = getSynthParamExpr( pname,
                                                        ((InvokeExpr *)e),
                                                        j, pent.params );
                    if( replace == SemStr() )
                        return replace;
                    pbase.searchReplaceAll(find,replace);
                }
                return pbase;
            }
        }
        fprintf( stderr, "Error: Can't handle invoked expression %s in %s\n",
                 pname.c_str(), name.c_str() );
    } else {
        fprintf( stderr, "Error: Don't know what to do with invocation expr " );
        e->printOn(stderr);
        fprintf( stderr, " in %s\n", name.c_str() );
    }
    return SemStr();
}

void CodeGenEmu::computeSynths( )
{
    for( SledDict::inst_iterator it = sled.insts.begin(); it != sled.insts.end(); it++ ) {
        bool err = false;
        if( it->second->isSynth() ) {
            string sslname = getSSLName( it->first );
            if( ssl.idict.find(sslname) != ssl.idict.end() )
                continue; /* Already has semantics */
            InvokeExpr *invoke = (InvokeExpr *)it->second->expr;
            assert( invoke->getKind() == INVOKER );
            InstIdent *parent = invoke->inst;
            string parentname = getSSLName( parent->name );
            if( ssl.idict.find(parentname) == ssl.idict.end() ) {
                fprintf( stderr, "Warning: Synthetic instruction %s specializes %s but %s has no semantics either\n", sslname.c_str(), parentname.c_str(),
                         parentname.c_str() );
                continue; /* Parent has no semantics either. *shrug* */
            }
            TableEntry &ent = ssl.idict[parentname];
            if( ent.params.size() != invoke->params.size() ) {
                fprintf( stderr, "Error: Synthetic instruction %s invokes %s with incorrect number of parameters\n", sslname.c_str(), parentname.c_str() );
                continue;
            }

            /* Having got all that out of the way, we basically want to get the
             * synth's parameters in ssl form, substitute them into a copy of
             * the parent's rtl, and make a new instruction with that. Okay? :)
             */
            HRTL newrtl(ent.hrtl);
            list<int>::iterator pit = ent.params.begin();
            for( unsigned i=0; i < ent.params.size(); i++, pit++) {
                SemStr find(2, idParam, *pit);
                SemStr replace = getSynthParamExpr( sslname, invoke, i,
                                                    ent.params );
                if( replace == SemStr() )
                    err = true;
                else
                    newrtl.searchAndReplace(find,replace);
            }

            /* Figure out the parameter numbers, by cross-referencing
            * against the parent instruction */
            list<int> newparms;
            for( unsigned i=0; i < it->second->params.size(); i++ ) {
                unsigned j;
                pit = ent.params.begin();
                for( j=0; j < ent.params.size(); j++, pit++ ) {
                    if( parent->params[j]->name == it->second->params[i]->name ) {
                        newparms.push_back(*pit);
                        break;
                    }
                }
                if( j == ent.params.size() ) {
                    if( ssl.ParamMap.find(it->second->params[i]->name) != ssl.ParamMap.end() ) {
                        newparms.push_back(ssl.ParamMap[it->second->params[i]->name]);
                    } else {
                        fprintf( stderr, "Error: Don't know what to do with parameter %s in synthetic %s\n", it->second->params[i]->name.c_str(), sslname.c_str() );
                        err = true;
                    }
                }
            }
            if( !err )
                ssl.idict[sslname] = TableEntry(newparms, newrtl);
        }
    }
}
