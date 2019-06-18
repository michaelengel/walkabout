/*
 * Copyright (C) 2001, Sun Microsystems, Inc
 *
 * See the file "LICENSE.TERMS" for information on usage and
 * redistribution of this file, and for a DISCLAIMER OF ALL
 * WARRANTIES.
 *
 */

/*=============================================================================
 * FILE:       genregs.cc
 * OVERVIEW:   Register structure computation and code generation routines
 *===========================================================================*/

/* $Revision: 1.2 $
 * 23 Apr 01 - Nathan: Initial version
 * 24 Apr 01 - Nathan: Added code to setup register types in DetParamMap
 * 24 Apr 01 - Nathan: Added get/set routine generation
 */

#include <set>
#include <ctype.h>
#include <string.h>
#include "codegenemu.h"
#include "reg.h"

bool operator < (const RegisterPart &a, const RegisterPart &b)
{
    /* Sort by position in the top-level register, with contained registers
     * before their containers */
    return a.offset < b.offset ||
        ( a.offset == b.offset && a.width > b.width );
}

string CodeGenEmu::sprintRegisterType( int num )
{
    return lang->sprintType( ssl.DetRegMap[num].g_type() );
}

/*
 * Set the parent (containing) register (ie in a covers/shares relationship)
 */
void CodeGenEmu::setRegisterParent( int child, int offset, int size, int parent,
                                    RegTree &treedn, vector<int> &treeup )
{
    if( child == parent ) return; /* nonsensical */
    while( treeup[parent] != -1 ) parent = treeup[parent];
    if( treeup[child] == -1 ) {
        /* No existing parent - good */
        treeup[child] = parent;
        treedn[parent].insert( RegisterPart(child, offset, size) );
        if( treedn[child].size() != 0 ) {
            /* reparent any existing children */
            for( SRP_IT it = treedn[child].begin();
                 it != treedn[child].end(); it++ ) {
                treeup[it->num] = parent;
                treedn[parent].insert( RegisterPart(it->num, it->offset + offset, it->width) );
            }
            treedn[child].clear();
        }   
    } else {
        /* child already has a parent - merge into the larger one */
        int oldparent = treeup[child];
        int oldoffset = -1;
        for( SRP_IT it = treedn[oldparent].begin();
             it != treedn[oldparent].end(); it++ ) {
            if( it->num == child ) {
                oldoffset = it->offset;
                break;
            }
        }
        assert(oldoffset != -1);
        
        if( ssl.DetRegMap[oldparent].g_size() > ssl.DetRegMap[parent].g_size() ) {
            setRegisterParent( parent, oldoffset - offset,
                               ssl.DetRegMap[parent].g_size(),
                               oldparent, treedn, treeup );
        } else {
            setRegisterParent( oldparent, offset - oldoffset,
                               ssl.DetRegMap[oldparent].g_size(),
                               parent, treedn, treeup );
        }
    }
}

string CodeGenEmu::mangleName( const string &str, const char *prefix )
{
    char *res = (char *)malloc(str.length()+strlen(prefix) + 2);
    char *p = res;
    while( *prefix ) *p++ = *prefix++;
    *p++ = '_';
    for( string::const_iterator s = str.begin(); s != str.end(); s++ ) {
        if( isalnum(*s) ) *p++ = *s;
    }
    *p = '\0';
    return res;
}

param_t CodeGenEmu::consSimpleRegister( const Type &type, const string &sslname,
                                         const string &prefix )
{
    string name = mangleReg(sslname);
    getreg[sslname] = prefix + "." + name;
    setreg[sslname] = lang->sprintAssign( prefix + "." + name, "%s" );
    return param_t(lang->sprintType(type), name);
}

params_t CodeGenEmu::consRegisterTree( SRP_IT &it, const SRP_IT &end,
                                       const Type &type, const string &prefix,
                                       int offset, int border )
{
    params_t elems;
    int padnum = 1;
    
    while( 1 ) {
        param_t newElem;
        RegisterPart reg = *it;
        if( offset < it->offset ) { /* need padding */
            newElem = param_t( lang->sprintType( UNSIGNED(it->offset-offset) ),
                               string("_pad")+ (padnum++));
            offset = it->offset;
        } else if( it->offset + it->width > border || it->num == -1) {
            break;
        } else {
            offset = it->offset+it->width;
            string sslname = ssl.DetRegMap[it->num].g_name();
            string name = mangleReg(sslname);
            it++;
            if( it->offset + it->width < offset ) { /* recursive case */
                string structName = name + "_str";
                string unionName = name + "_un";
                string childPrefix = prefix + "." + unionName + "." +
                    structName;
                string subStruct = lang->sprintStructDefn( "", 
                      consRegisterTree( it, end, type, childPrefix, reg.offset,
                                        offset ) );
                params_t uelems;
                uelems.push_back( subStruct, structName );
                uelems.push_back( sprintRegisterType(reg.num), name );
                newElem = param_t(lang->sprintUnionDefn("",uelems), unionName);
                getreg[sslname] = prefix + "." + unionName + "." + name;
                setreg[sslname] = lang->sprintAssign(prefix + "." + unionName +
                                                     "." + name, "%s");
            } else { /* base case */
                newElem = param_t(sprintRegisterType(reg.num), name);
                getreg[sslname] = prefix + "." + name;
                setreg[sslname] = lang->sprintAssign(prefix + "." + name, "%s");
            }
        }
        
        if( needByteSwap )
            elems.push_front( newElem );
        else
            elems.push_back( newElem );
    }
        
    return elems;
}

class RegisterBlock {
public:
    RegisterBlock(FieldIdent *el) {
        elem = el;
        aliases.push_back(el->name);
        isArray = true;
        type = NULL;
    }
    FieldIdent *elem;
    list<string> aliases;
    vector<int> regs;
    bool isArray;
    Type *type;
};

/*
 * compute the register structure (along with accessor expressions). This is fairly
 * long, because we want to do all the register computations in one go (saves
 * having temporary structures hanging around).
 *
 * Note that this can't cope with everything, so if you're trying to support a
 * particularly strange architecture, you may need to add some code here.
 * Specifically Union mode can't deal with partially overlapping registers or
 * lack of large enough types (the latter should be fixed, the first is more of
 * a limitation of the technique).
 */
void CodeGenEmu::computeRegisterStructure( )
{
    RegTree treedn(ssl.DetRegMap.rbegin()->first+1);
    vector<int> treeup(ssl.DetRegMap.rbegin()->first+1);
    list<RegisterBlock> blocks;

    for( unsigned i=0; i<treeup.size(); i++ ) treeup[i] = -1;

    /* Build the register sets from the covers/shares values */
    for( regmap_it it = ssl.DetRegMap.begin(); it != ssl.DetRegMap.end(); it++ ) {
        int j = it->second.g_mappedIndex();
        if( j != -1 ) {
            if( it->second.g_mappedOffset() != 0 ||
                it->second.g_size() <= ssl.DetRegMap[j].g_size() ) {
                /* i->first shares j */
                setRegisterParent( it->first, it->second.g_mappedOffset(),
                                   it->second.g_size(), j, treedn, treeup );
            } else {
                /* j shares i->first / i->first covers j.. */
                for( int sz = 0; sz < it->second.g_size();
                     sz += ssl.DetRegMap[j].g_size(), j++) {
                    setRegisterParent( j, sz, ssl.DetRegMap[j].g_size(), it->first,
                                       treedn, treeup );
                }
            }
        }
    }

#ifdef DEBUG_EMU
    /* Dump the register "table" */
    printf( "Register table:\n" );
    for( regmap_it it = ssl.DetRegMap.begin(); it != ssl.DetRegMap.end(); it++ ) {
        if( treeup[it->first] == -1 ) {
            printf( "  %s %d\n", it->second.g_name(), it->second.g_size() );
            for( SRP_IT r = treedn[it->first].begin();
                 r != treedn[it->first].end(); r++ ) {
                printf( "    %d %s %d\n", r->width, ssl.DetRegMap[r->num].g_name(),
                        r->offset );
                assert( r->width == ssl.DetRegMap[r->num].g_size() );
            }
        }
    }
#endif

    /* split register fields into equivalence classes ( = access same regs ) */
    for( SledDict::field_iterator it = sled.fields.begin();
         it != sled.fields.end(); it++ ) {
        if( it->second->hasNames() ) {
            bool hasMatch = false;
            for( list<RegisterBlock>::iterator rbit = blocks.begin();
                 rbit != blocks.end(); rbit++ ) {
                if( rbit->elem->isEquivalent(*it->second) ) {
                    rbit->aliases.push_back(it->first);
                    hasMatch = true;
                    break;
                }
                if( rbit->elem->overlaps(*it->second) ) {
                    fprintf( stderr, "WARNING: Overlapping register operands - not handled. Your emulator will probably not work\n" );
                }
            }
            if( !hasMatch ) { /* New equivalence class */
                blocks.push_back( RegisterBlock(it->second) );
                RegisterBlock &rb = blocks.back();
                /* Match reg names against ssl */
                for( vector<string *>::iterator nit = it->second->names->begin();
                     nit != it->second->names->end(); nit++ ) {
                    if( *nit == NULL ||
                        ssl.RegMap.find(**nit) == ssl.RegMap.end() ) {
                        rb.regs.push_back(-1);
                    } else {
                        int num = ssl.RegMap[**nit];
                        rb.regs.push_back(num);
                        /* Check types are all the same */
                        Register &reg = ssl.DetRegMap[num];
                        if( rb.type == NULL )
                            rb.type = new Type(reg.g_type());
                        else if( *rb.type != reg.g_type() ) {
                            fprintf( stderr, "WARNING: Mismatching types in register operand %s\n",
                                     it->first.c_str() );
                        }
                        if( !treedn[num].empty() || treeup[num] != -1 )
                            rb.isArray = false; /* complex member, so no */
                    }
                }
                if( rb.type == NULL ) {
                    /* no recognized registers - drop the operand */
                    blocks.pop_back();
                }
            }
        }
    }

#ifdef DEBUG_EMU
    printf( "Register blocks:\n" );
    for( list<RegisterBlock>::iterator rbit = blocks.begin();
         rbit != blocks.end(); rbit++ ) {
        assert( rbit->type );
        printf( " %s [", lang->sprintType(*rbit->type).c_str() );
        for( list<string>::iterator fit = rbit->aliases.begin();
             fit != rbit->aliases.end(); fit++ ) {
            if( fit != rbit->aliases.begin() )
                printf( ", " );
            printf( "%s", fit->c_str() );
        }
        printf( "] : " );
        for( vector<int>::iterator rit = rbit->regs.begin();
             rit != rbit->regs.end(); rit++ ) {
            if( rit != rbit->regs.begin() )
                printf( ", " );
            if( *rit != -1 )
                printf( "%s", ssl.DetRegMap[*rit].g_name() );
        }
        printf( "\n" );
    }
#endif

    /* set the regTypes & regIdx in DetParamMap */
    for( list<RegisterBlock>::iterator rbit = blocks.begin();
         rbit != blocks.end(); rbit++ ) {
        assert( rbit->type );
        for( list<string>::iterator fit = rbit->aliases.begin();
             fit != rbit->aliases.end(); fit++ ) {
            if( ssl.ParamMap.find(*fit) != ssl.ParamMap.end() ) {
                int id = ssl.ParamMap[*fit];
                ssl.DetParamMap[id].regType = *rbit->type;

                for( vector<int>::iterator rit = rbit->regs.begin();
                     rit != rbit->regs.end(); rit++ ) {
                    ssl.DetParamMap[id].regIdx.insert(*rit);
                }
            }
        }
    }
    /* Set ordinary field types in DetParamMap */
    for( SledDict::field_iterator fit = sled.fields.begin();
         fit != sled.fields.end(); fit++ ) {
        if( ssl.ParamMap.find(fit->first) != ssl.ParamMap.end() ) {
            ssl.DetParamMap[ssl.ParamMap[fit->first]].type =
                SIGNED( (fit->second->hi - fit->second->lo + 1) );
        }
    }

/* Construct structures & set/get expressions for individual registers */
    params_t elems;

    /* Take care of array members first */
    for( list<RegisterBlock>::iterator rbit = blocks.begin();
         rbit != blocks.end(); rbit++ ) {
        if( rbit->isArray ) {
            param_t elem(lang->sprintType(*rbit->type), rbit->elem->name);
            elem.count = rbit->regs.size();
            elems.push_back(elem);
            for( int i=0; i < rbit->regs.size(); i++ ) {
                if( rbit->regs[i] != -1 ) {
                    string sslname = ssl.DetRegMap[rbit->regs[i]].g_name();
                    getreg[sslname] = registerStructName + "." +
                        rbit->elem->name + "[" + i + "]";
                    setreg[sslname] = lang->sprintAssign(getreg[sslname], "%s");
                    treeup[rbit->regs[i]] = -2;
                        /* mark so next bit doesn't bite */
                }
            }
        }
    }

    Type maxIntType = lang->getMaxIntType();
    Type maxFloatType = lang->getMaxFloatType();
    
    if( lang->hasUnions() ) {
        /* Okay, great, build the structures top down */
        for( regmap_it it = ssl.DetRegMap.begin();
             it != ssl.DetRegMap.end(); it++ ) {
            if( treeup[it->first] == -1 ) { /* Topmost node */
                if( treedn[it->first].size() == 0 ) {
                    elems.push_back( consSimpleRegister( it->second.g_type(),
                                                         it->second.g_name(),
                                                         registerStructName ) );
                                      
                } else {
                    treedn[it->first].insert(
                        RegisterPart(it->first, 0, it->second.g_size()) );
                    treedn[it->first].insert(
                        RegisterPart(-1, it->second.g_size(), 0 ) );// sentinel
                    SRP_IT rit = treedn[it->first].begin();
                    elems.push_back( consRegisterTree( rit, treedn[it->first].end(),
                                                       it->second.g_type(),
                                                       registerStructName, 0, it->second.g_size() ));
                }
            }
        }
    } else {
        /* Oh well... guess we get to do lots of bit twiddling */
        for( regmap_it it = ssl.DetRegMap.begin(); it != ssl.DetRegMap.end(); it++ ) {
            if( treeup[it->first] == -1 ) {
                string reg = mangleReg(it->second.g_name());
                Type baseType = it->second.g_type();
                Type maxType;
                if( baseType.getType() == FLOATP )
                    maxType = maxFloatType;
                else maxType = maxIntType;
                if( baseType.getSize() > maxType.getSize() ) {
                    /* Can't represent directly, use an array of maxType */
                    param_t elem = consSimpleRegister( maxType,
                                                        it->second.g_name(),
                                                        registerStructName );
                    elem.count = (baseType.getSize() + maxType.getSize() - 1) /
                        maxType.getSize();
                    elems.push_back( elem );
                    int n = maxType.getSize();
                    
                    for( SRP_IT r = treedn[it->first].begin();
                         r != treedn[it->first].end(); r++ ) {
                        const Type &rtype = ssl.DetRegMap[r->num].g_type();                        
                        string sub = ssl.DetRegMap[r->num].g_name();
                        string base = registerStructName + "." + reg + "[" + r->offset/n + "]";
                        getreg[sub] = lang->sprintTypeChange( maxType, rtype,
                            lang->sprintGetBitslice( maxType, base,
                                                     r->offset%n,
                                                     (r->offset + r->width -1) %n ) );
                        setreg[sub] = lang->sprintSetBitslice( maxType, base,
                            lang->sprintTypeChange( rtype, maxType, "%s" ),
                                                               r->offset%n,
                                                               (r->offset + r->width-1)%n );
                    }
                } else {
                    string base = registerStructName + "." + reg;
                    elems.push_back( consSimpleRegister( it->second.g_type(),
                                                         it->second.g_name(),
                                                         registerStructName ) );
                    for( SRP_IT r = treedn[it->first].begin();
                         r != treedn[it->first].end(); r++ ) {
                        const Type &rtype = ssl.DetRegMap[r->num].g_type();
                        string sub = ssl.DetRegMap[r->num].g_name();
                        getreg[sub] = lang->sprintTypeChange( baseType, rtype,
                            lang->sprintGetBitslice( baseType, base,
                                                     r->offset,
                                                     r->offset + r->width -1 ) );
                        setreg[sub] = lang->sprintSetBitslice( baseType, base,
                            lang->sprintTypeChange( rtype, baseType, "%s" ),
                                                               r->offset,
                                                               r->offset + r->width-1 );
                    }
                }
            }
        }
    }

    /* Add in expressions for the "Special" (non-addressable) registers */
    for( sregmap_it it = ssl.SpecialRegMap.begin();
         it != ssl.SpecialRegMap.end(); it++ ) {
        elems.push_back( consSimpleRegister( it->second.g_type(), it->first,
                                             registerStructName ) );
    }
        
    registerStruct = lang->sprintStructDefn( registerStructTypeName, elems ) + ";\n";


/* Construct set/get expressions for register blocks (eg for r[rd]) */

    /*
     * We (need to) support 3 basic schemes for block access - in descending order
     * of preference, these are:
     *  a) direct arrays of registers
     *  b) arrays of pointers to registers
     *  c) get/set methods (ie switch/case)
     *
     * (a) requires that all the members be simple (ie have no sub-registers). We
     * could try to be cleverer with things like sparc floating point registers,
     * but this is left as an exercise for someone else :)
     * (b) requires pointers (obviously)
     * (c) should work under any circumstances
     */
    for( list<RegisterBlock>::iterator rbit = blocks.begin();
         rbit != blocks.end(); rbit++ ) {
        string get, set;
        string access="";
        if( rbit->isArray ) {
            get = registerStructName + "." + rbit->elem->name + "[%r]";
            set = lang->sprintAssign(get,"%s");
        } else if( lang->hasPointers() ) {
            string rn = registerStructName + "_" + rbit->elem->name;
            get = "(" + lang->sprintDereference( rn + "[%r]") + ")";
            set = lang->sprintAssign(get,"%s");
            list<string> arr;
            for( vector<int>::iterator it = rbit->regs.begin();
                 it != rbit->regs.end(); it++ ) {
                if( *it == -1 )
                    arr.push_back( "NULL" );
                else
                    arr.push_back( lang->sprintAddressOf(
                                       getreg[ssl.DetRegMap[*it].g_name()]) );
            }
            access = lang->sprintArrayVarDefn( lang->sprintType(*rbit->type) + " *",
                                               rn, rbit->regs.size(),
                                               lang->sprintNames(arr) );
        } else {
            string func = registerStructName + "_" + rbit->elem->name;
            string getCases, setCases;
            get = "get" + func + "(%r)";
            set = "set" + func + "(%r,%s);\n";

            for( unsigned i = 0; i < rbit->regs.size(); i++ ) {
                if( rbit->regs[i] != -1 ) {
                    string name = ssl.DetRegMap[rbit->regs[i]].g_name();
                    
                    getCases += lang->sprintSwitchBranch( rbit->regs[i],
                                    lang->sprintReturn(getreg[name]), false );
                    setCases += lang->sprintSwitchBranch( rbit->regs[i],
                                                          setreg[name] );
                }
            }
            setCases = searchAndReplace( setCases, "%s", "val" );
                                                          
            access = lang->sprintFunctionDefn( *rbit->type, "get" + func,
                params_t( lang->sprintType(registerIndexType), "num" ),
                lang->sprintSwitch("num",getCases,
                                   lang->sprintReturn( "default" +
                                       lang->sprintType(*rbit->type)) ) );
                               
            access +=lang->sprintFunctionDefn( VOID_TYPE, "set" + func,
                params_t( lang->sprintType(registerIndexType), "num",
                          lang->sprintType(*rbit->type), "val" ),
                lang->sprintSwitch("num",setCases, "") );
                                               
        }

        for( list<string>::iterator it = rbit->aliases.begin();
             it != rbit->aliases.end(); it++ ) {
            getidxreg[*it] = get;
            setidxreg[*it] = set;
        }
        registerAccessFuncs += access;
    }
    
}

#define TERMWIDTH 120

class RegisterFormatter {
    string code, fmt, flagfmt;
    int col;
    list<string> params, flagparams;
    CodeGenEmu *gen;
    bool flagsMode;
public:
    RegisterFormatter( CodeGenEmu *e ) { gen = e; col = 0; flagsMode = false; }

    void addFlag( Register &reg, char letter );
    void addRegister( Register &reg );
    void finishFlags();
    void breakLine();
    string &sprintCode();
};


void RegisterFormatter::addFlag( Register &reg, char letter )
{
    flagsMode = true;
    flagfmt += "%c";
    flagparams.push_back(
        gen->lang->sprintTypeCast( reg.g_type(), BOOL_TYPE,
            gen->getreg[reg.g_name()]) + "? \'" + letter +"\' : '-'" );
}

void RegisterFormatter::finishFlags()
{
    if( flagsMode ) {
        int width = flagparams.size() + 8;
        if( col + width > TERMWIDTH )
            breakLine();
        fmt += "Flags: " + flagfmt + " ";
        for( list<string>::iterator it = flagparams.begin();
             it != flagparams.end(); it++ ) {
            params.push_back(*it);
        }
        col += width;
        flagsMode = false;
    }
}

void RegisterFormatter::addRegister( Register &reg )
{
    string regfmt = searchAndReplace(reg.g_name(),"%","%%");
    string regparm;
    int width = strlen(reg.g_name());
    if( reg.g_size() == 1 ) { /* flag */
        width += 8;
        regfmt += ": %s ";
        regparm = gen->lang->sprintTypeCast( reg.g_type(), BOOL_TYPE,
            gen->getreg[reg.g_name()]) + "? \"true \" : \"false\"";
    } else if( reg.isFloat() ) {
        width += 9 + (reg.g_size()/4);
        regfmt = regfmt + ": %+." + (reg.g_size()/4)  + "e ";
        regparm = gen->getreg[reg.g_name()];
    } else {
        width += 3 + (reg.g_size()/4);
        regfmt = regfmt + ": %0" + (reg.g_size()/4) + "x ";
        regparm = gen->getreg[reg.g_name()];
    }
    if( col + width > TERMWIDTH ) {
        breakLine();
    }
    fmt += regfmt;
    params.push_back(regparm);
    col += width;
}

void RegisterFormatter::breakLine( )
{
    if( params.size() != 0 ) {
        fmt += "\\n";
        code += gen->lang->sprintFormattedPrint( fmt, params, "out" );
        fmt = "";
        params.clear();
        col = 0;
    }
}

string &RegisterFormatter::sprintCode()
{
    finishFlags();
    breakLine();
    return code;
}

string CodeGenEmu::sprintDumpRegisters( )
{
    /* We try to be a little clever here to get reasonably decent
     * output. No guarantees that this will always be optimal, of
     * course, so may need to be tweaked a little for different
     * architectures. Assume output is 132 columns fixed-width.
     *
     * Generate the following routinges:
     *    dumpControlRegisters()
     *        - dump control (special) regs only
     *    dumpMainRegisters()
     *        - dump control & integer regs
     *    dumpFloatRegisters()
     *        - dump floating point regs only
     *    dumpAllRegisters()
     *        - dump all real (not partial/merged) registers
     *
     * All dump routines take a stream parameter which they write on,
     * and return void.
     *
     * Someone with too much free time might want to make the routines
     * adjust to the actual terminal width at runtime...
     */
    
    /* Standard flags are magical :) */
    RegisterFormatter controlRegs(this);
    RegisterFormatter intRegs(this);
    RegisterFormatter floatRegs(this);
    for( map<string,Register>::iterator rit = ssl.SpecialRegMap.begin();
         rit != ssl.SpecialRegMap.end(); rit++ ) {
        if( rit->first == "%CTI" || rit->first == "%NEXT" ) continue;
        if( rit->second.g_size() == 1 ) { /* Flag */
            if( rit->first == "%NF" ) controlRegs.addFlag(rit->second,'N');
            else if( rit->first == "%AF" ) controlRegs.addFlag(rit->second,'A');
            else if( rit->first == "%CF" ) controlRegs.addFlag(rit->second,'C');
            else if( rit->first == "%OF" ) controlRegs.addFlag(rit->second,'O');
            else if( rit->first == "%ZF" ) controlRegs.addFlag(rit->second,'Z');
            else if( rit->first == "%PF" ) controlRegs.addFlag(rit->second,'P');
            else if( rit->first == "%DF" ) controlRegs.addFlag(rit->second,'D');
            else controlRegs.addRegister(rit->second);
        } else controlRegs.addRegister(rit->second);
    }
    for( map<int, Register>::iterator rit = ssl.DetRegMap.begin();
         rit != ssl.DetRegMap.end(); rit++ ) {
        if( rit->second.g_mappedIndex() == -1 ) {
            if( rit->second.isFloat() )
                floatRegs.addRegister(rit->second);
            else
                intRegs.addRegister(rit->second);
        }
    }
    params_t dumpParams( lang->getStreamType(), "out" );

    return
        lang->sprintFunctionDefn( VOID_TYPE, prefix + "dumpControlRegisters",
                                  dumpParams, controlRegs.sprintCode() ) +
        lang->sprintFunctionDefn( VOID_TYPE, prefix + "dumpMainRegisters",
                                  dumpParams, intRegs.sprintCode() +
                                  controlRegs.sprintCode() ) +
        lang->sprintFunctionDefn( VOID_TYPE, prefix + "dumpFloatRegisters",
                                  dumpParams, floatRegs.sprintCode() ) +
        lang->sprintFunctionDefn( VOID_TYPE, prefix + "dumpAllRegisters",
                                  dumpParams, intRegs.sprintCode() +
                                  controlRegs.sprintCode() +
                                  floatRegs.sprintCode() );
}
