/*
 * Copyright (C) 2001, Sun Microsystems, Inc
 *
 * See the file "LICENSE.TERMS" for information on usage and
 * redistribution of this file, and for a DISCLAIMER OF ALL
 * WARRANTIES.
 *
 */


#include "global.h"
#include "ss.h"
#include "codegenemu.h"

/*
 * General notes: This is used by the register access routines to
 * determine which actual blocks of registers a particular expression
 * can access. It's not breathtakingly elegant (just runs through all
 * possible values), but it does have the advantage of being at least
 * nominally correct :) (Doing this in general with sets is somewhat
 * complicated by the ternary expressions).
 *
 * This does depend on the fact that the input sets are generally
 * pretty small - in fact the evaluator will currently reject
 * expressions representing a large range (such as %g1).
 *
 * Generalizing this to deal (correctly) with any kind of expression
 * is left as an exercise for the reader...
 *
 * Nathan: Initial version
 */

int CodeGenEmu::evaluateSemStr( SSCIT &it, SSCIT &end, map<int,int> &params,
                                map<int,int> &regs )
{
    int idx = *it++;
    const SemItem &si = theSemTable[idx];
    
    switch( si.cat ) {
        case cCONSTANT:
            switch(idx) {
                case idIntConst:
                    return *it++;
            }
            break;
        case cOPERATOR:
            int a,b,c;
            a = evaluateSemStr( it, end, params, regs );
            if( si.iNumVarArgs > 1 )
                b = evaluateSemStr( it, end, params, regs );
            if( si.iNumVarArgs > 2 )
                c = evaluateSemStr( it, end, params, regs );
            switch(idx) {
                case idPlus: return a+b;
                case idMinus: return a-b;
                case idMult: return (unsigned)a*(unsigned)b;
                case idDiv: return b==0?0:(unsigned)a/(unsigned)b;
                case idMults: return a*b;
                case idDivs: return b==0?0:a/b;
                case idMod: return (unsigned)a%(unsigned)b;
                case idMods: return a%b;
                case idNeg: return ~a;
                case idAnd: return a&&b;
                case idOr: return a||b;
                case idEquals: return a==b;
                case idNotEqual: return a!=b;
                case idLess: return a<b;
                case idGtr: return a>b;
                case idLessEq: return a<=b;
                case idGtrEq: return a>=b;
                case idLessUns: return (unsigned)a<(unsigned)b;
                case idGtrUns: return (unsigned)a>(unsigned)b;
                case idLessEqUns: return (unsigned)a<=(unsigned)b;
                case idGtrEqUns: return (unsigned)a>=(unsigned)b;
                case idNot: return !a;
                case idSignExt: return a;
                case idBitAnd: return a&b;
                case idBitOr: return a|b;
                case idBitXor: return a^b;
                case idShiftL: return a<<b;
                case idShiftR: return (unsigned)a>>b;
                case idShiftRA: return a>>b;
                case idTern: return a?b:c;
                default: break;
            }
            break;
        case cOPERATION:
            switch( idx ) {
                case idRegOf:
                    assert( *it == idIntConst );
                    it++;
                    idx = *it++;
                    if( regs.find(idx) != regs.end() )
                        return regs[idx];
                    fprintf( stderr, "Failed to match regof %d\n", idx );
                    abort();
                    break;
                case idParam:
                    idx = *it++;
                case idPC: case idAFP: case idAGP:
                case idZF: case idCF: case idNF: case idOF:
                case idFZF: case idFLF: case idFGF: case idCTI:
                    if( params.find(idx) != params.end() )
                        return params[idx];
                    fprintf( stderr, "Failed to match param %d\n", idx );
                    abort();
                    break;
                case idSize:
                    it++;
                    return evaluateSemStr(it,end,params, regs);
            }
            break;
    }
    return 0;
}

int CodeGenEmu::evaluateSemStr( const SemStr &ss, map<int,int> &params,
                                map<int,int> &regs )
{
    SSCIT in = ((SemStr &)ss).getIndices()->begin();
    SSCIT end = ((SemStr &)ss).getIndices()->end();
    return evaluateSemStr( in, end, params, regs );
}

typedef map<int,set<int> > valsets;

set<int> CodeGenEmu::evaluateSemStr( const SemStr &ss )
{
    map<int,int> params;
    map<int,int> regs;
    valsets pvalues, rvalues;
    set<int> results;


    /* Build param list */
    SemStr match(2,idParam,WILD);
    list<SemStr *> result;
    ((SemStr &)ss).searchAll(match,result);
    for( list<SemStr *>::iterator it = result.begin();
         it != result.end(); it++ ) {
        int parm = (*it)->getSecondIdx();
        if( ssl.DetParamMap.find(parm) != ssl.DetParamMap.end() ) {
            if( ssl.DetParamMap[parm].regIdx.size() > 0 ) {
                pvalues[parm] = ssl.DetParamMap[parm].regIdx;
                params[parm] = *pvalues[parm].begin();
            } else {
                fprintf( stderr, "Non-register parameter in r[] expression not supported\n" );
                abort();
            }
        }
    }
    result.clear();
    match = SemStr(3,idRegOf,WILD,WILD);
    ((SemStr &)ss).searchAll(match,result);
    for( list<SemStr *>::iterator it = result.begin();
         it != result.end(); it++ ) {
        if( (*it)->getSecondIdx() != idIntConst ) {
            fprintf( stderr, "Complex register subexpressions not supported: %s\n", ss.sprint().c_str() );
            abort();
        }
        int reg = (*it)->getThirdIdx();
        int sz = ssl.DetRegMap[reg].g_size();
        if( sz > 8 ) {
            fprintf( stderr, "Large register subexpressions not supported: %s\n", ss.sprint().c_str() );
            abort();
        }
//        for( int i= -(1<<(sz-1)); i < 1<<(sz-1); i++ )
        for( int i=0; i<(1<<sz); i++ )
            rvalues[reg].insert(i);
        regs[reg] = *rvalues[reg].begin();
    }

    /* Run through all values */
    bool done = false;
    do {
        results.insert(evaluateSemStr( ss, params, regs ));
        
        done = true;
        for( map<int,int>::iterator it = params.begin();
             it != params.end(); it++ ) {
            set<int>::iterator vsit = pvalues[it->first].find(it->second);
            vsit++;
            if( vsit == pvalues[it->first].end() ) {
                vsit = pvalues[it->first].begin();
                it->second = *vsit;
            } else {
                it->second = *vsit;
                done = false;
                break;
            }
        }
        if( done ) {
            for( map<int,int>::iterator it = regs.begin();
                 it != regs.end(); it++ ) {
                set<int>::iterator vsit = rvalues[it->first].find(it->second);
                vsit++;
                if( vsit == rvalues[it->first].end() ) {
                    vsit = rvalues[it->first].begin();
                    it->second = *vsit;
                } else {
                    it->second = *vsit;
                    done = false;
                    break;
                }
            }
        }
    } while( !done );
    
    return results;
}


int CodeGenEmu::matchRegIdx( const set<int> &regs )
{
    for( map<int,ParamEntry>::iterator parm = ssl.DetParamMap.begin();
         parm != ssl.DetParamMap.end(); parm++ ) {
        unsigned count = 0;
        if( parm->second.regIdx.size() == 0 )
            continue;
        for( set<int>::iterator it = regs.begin(); it != regs.end(); it++ ) {
            if( parm->second.regIdx.find(*it) != parm->second.regIdx.end() ) {
                count++;
            }
        }
        if( count == regs.size() )
            return parm->first;
    }

    fprintf( stderr, "Register expression not matched against any register block\n" );
    fprintf( stderr, "Map was: { " );
    for( set<int>::iterator it = regs.begin(); it != regs.end(); it++ ) {
        fprintf( stderr, "%d, ", *it );
    }
    fprintf( stderr, "}\n" );
             
    return -1;
}
