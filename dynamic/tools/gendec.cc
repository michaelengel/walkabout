/*
 * Copyright (C) 2001, Sun Microsystems, Inc
 *
 * See the file "LICENSE.TERMS" for information on usage and
 * redistribution of this file, and for a DISCLAIMER OF ALL
 * WARRANTIES.
 *
 */

/*=============================================================================
 * FILE:       gendec.cc
 * OVERVIEW:   
 *===========================================================================*/

/* $Revision: 1.2 $
 * 18 May 01 - Nathan: Initial version
 * 31 Jul 01 - Brian: New class HRTL replaces RTlist. Renamed LRTL to HRTLList.
 */

#include <fstream.h>
#include <ctype.h>
#include <map>
#include "global.h"
#include "options.h"
#include "rtl.h"
#include "sledtree.h"
#include "codegen.h"

SemTable theSemTable;
options progOptions;
options::options() {}

RTLInstDict ssl;
SledDict sled;

typedef struct {
    string str;
    int length; /* In SemItems */
} sequence;

map<int,string> decodeParams;
map<int,sequence> innerParams;

string mangleMatchParam( const string &s ) { return s; }

string sprintInstantiation( const InstIdent &inst, const string &name,
                            const HRTL &rtl, const list<int> &params )
{
    string s = string("RTs = instantiate(pc, \"") + name + "\"";
    for( list<int>::const_iterator it = params.begin(); it != params.end(); it++ ) {
        s += ", " + decodeParams[*it];
    }
    return s + ");";
}

string sprintSemStrConstruction( SSCIT &it, SSCIT &end, int &length )
{
    int idx = *it++;
    int args = theSemTable[idx].iNumIntArgs + theSemTable[idx].iNumFixedArgs;
    if( idx == idParam ) {
        int parm = *it++;
        length += innerParams[parm].length;
        return innerParams[parm].str;
    }
    length += 1 + args;
    string s = string("") + idx;
    for( int i = 0; i < args; i++ ) {
        s += "," + *it++;
    }
    for( int i = 0; i < theSemTable[idx].iNumVarArgs; i++ ) {
        s += "," + sprintSemStrConstruction( it, end, length );
    }
    return s;
}
string sprintSemStrConstruction( SemStr *ss, int &length )
{
    SSCIT it = ss->getIndices()->begin();
    SSCIT end = ss->getIndices()->end();
    return sprintSemStrConstruction( it, end, length );
}

int main(int argc, char *argv[])
{
    if( argc < 4 ) {
        fprintf( stderr, "Usage: %s <sled-file> <ssl-file> <output-file>\n", argv[0] );
        exit(1);
    }

    if( !sled.readSLEDFile(argv[1]) ) {
        exit(2);
    }
    if( !ssl.readSSLFile(argv[2]) ) {
        exit(3);
    }
    ofstream out(argv[3]);
    if( !out.is_open() ) {
        fprintf( stderr, "Unable to open output file %s\n", argv[3] );
        exit(3);
    }

    string decoderClass = "NJMCDecoder";

    out << "#include \"global.h\"\n";
    out << "#include \"ss.h\"\n";
    out << "#include \"decoder.h\"\n";

    for( map<string,int>::iterator it = ssl.ParamMap.begin();
         it != ssl.ParamMap.end(); it++ ) {
        if( ssl.DetParamMap.find(it->second) != ssl.DetParamMap.end() &&
            ssl.DetParamMap[it->second].kind == PARAM_VARIANT ) {
        } else if( sled.fields.find(it->first) != sled.fields.end() ) {
            FieldIdent *field = sled.fields[it->first];
            if( field->hasNames() ) { /* Registers */
                out << "int reg_" + it->first + "[] = { ";
                for( vector<string *>::iterator name = field->names->begin();
                     name != field->names->end(); name++ ) {
                    if( *name != NULL &&
                        ssl.RegMap.find(**name) != ssl.RegMap.end() ) {
                        out << ssl.RegMap[**name];
                    } else {
                        out << "-1";
                    }
                    out << ",";
                }
                out << " };\n";
                decodeParams[it->second] = "new SemStr( 2, idIntConst, reg_" + it->first + "[" + it->first + "] )";
                innerParams[it->second].str = "idIntConst, reg_" + it->first + "[" + it->first + "]";
                innerParams[it->second].length = 2;
            } else {
                decodeParams[it->second] = "new SemStr( 2, idIntConst, " + it->first + " )";
                innerParams[it->second].str = "idIntConst, " + it->first;
                innerParams[it->second].length = 2;
            }
        } else {
            decodeParams[it->second] = "new SemStr( 2, idIntConst, " + it->first + " )";
            innerParams[it->second].str = "idIntConst, " + it->first;
            innerParams[it->second].length = 2;
        }
    }
    for( map<string,int>::iterator it = ssl.ParamMap.begin();
         it != ssl.ParamMap.end(); it++ ) {
        if( ssl.DetParamMap.find(it->second) != ssl.DetParamMap.end() &&
            ssl.DetParamMap[it->second].kind == PARAM_VARIANT ) {
            list<int> &opts = ssl.DetParamMap[it->second].params;
            out << "SemStr *" << decoderClass << "::decode_" << it->first << "( ADDRESS pc )\n{\n";
            out << "    SemStr *expr;\n";
            out << "    match pc to\n";
            for( list<int>::iterator opt = opts.begin(); opt != opts.end(); opt++ ) {
                ParamEntry &ent = ssl.DetParamMap[*opt];
                int length = 0;
                string cons;
                if( ent.rta ) {
                    cons = sprintSemStrConstruction( ent.rta->getRHS(), length );
                } else if( ent.params.size() > 0 ) {
                    length = 2;
                    cons = "idIntConst, " + theSemTable[ent.params.front()].sName;
                }
                out << "        " << sprintMatchLine(
                    theSemTable[*opt].sName, ent.params,
                    string("return new SemStr( ") + length + ", " + cons + " );\n" );
            }
            out << "    endmatch\n";
            out << "    return expr;\n";
            out << "}\n\n";
            decodeParams[it->second] = "decode_" + it->first + "(" + it->first + ")" ;
        }
    }

    /* Main decode procedure */
    out << "list<RT *>* " << decoderClass << "::decodeLowLevelInstruction (ADDRESS hostPC, ADDRESS pc,\n";
    out << "                DecodeResult& result)\n{";
    out << "    list<RT*>* RTs = NULL;\n";
    out << "    ADDRESS nextPC;\n\n";
    
    out << "    match [nextPC] hostPC to \n";
    for( SledDict::inst_iterator it = sled.insts.begin();
         it != sled.insts.end(); it++ ) {
        /* SSLify the instruction name */
        string sslname;
        for( const char *s = it->first.begin(); s != it->first.end(); s++ ) 
            if( isalnum(*s) ) sslname += toupper(*s);
        
        if( ssl.idict.find(sslname) != ssl.idict.end() ) {
            TableEntry &ent = ssl.idict[sslname];
            out << "        " <<
                sprintMatchLine( it->first, ent.params,
                                 sprintInstantiation(*it->second, sslname, ent.rtl, ent.params) );
            out << "\n";
        }
    }
    out << "        else {\n";
    out << "            RTs = NULL;\n";
    out << "            result.valid = false;\n";
    out << "            result.numBytes = 0;\n";
    out << "        }\n";
    out << "    endmatch\n\n";
    out << "    result.numBytes = (nextPC - hostPC);\n";
    out << "    return RTs;\n";
    out << "}\n";
}
