/*
 * Copyright (C) 2001, Sun Microsystems, Inc
 *
 * See the file "LICENSE.TERMS" for information on usage and
 * redistribution of this file, and for a DISCLAIMER OF ALL
 * WARRANTIES.
 *
 */

/*=============================================================================
 * FILE:       genemu.cc
 * OVERVIEW:   emulator generator main program, and some miscellaneous support
 *             routines.
 *===========================================================================*/

/* $Revision: 1.4 $
 * 07 Feb 01 - Nathan: Initial version
 *  1 Aug 01 - Brian: Added stub code to resolve linking against hrtl.o.
 * 30 Aug 01 - Bernard: Remove the need for SSL files if generating disasm
 */

#define APPVERSION "0.61"

#include "global.h"
#include "proc.h"
#include "prog.h"
#include "rtl.h"
#include "options.h"
#include "type.h"
#include "codegenemu.h"
#include "codegen_c.h"
#include <sys/time.h>
#include <unistd.h>
#include <ctype.h>
#include <values.h>
#include <sys/stat.h>
#include <map>

options progOptions;
SemTable theSemTable;
options::options() {}

bool CodeGenEmu::sanityCheck( )
{
    bool rval = true;
    int cnt = 0;
    
    map<string, TableEntry, less<string> >::iterator it;
    for( it = ssl.idict.begin(); it != ssl.idict.end(); it++ ) {
        if( slednamemap.find(it->first) == slednamemap.end() ) {
            fprintf( stderr, "Operation in SSL only: %s\n", it->first.c_str());
            rval = false;
            cnt++;
        }
    }
    if( cnt > 0 )
        fprintf( stderr, "Total: %d\n\n", cnt );
    cnt = 0;
    map<string,string>::iterator ssit;
    for( ssit = slednamemap.begin(); ssit != slednamemap.end(); ssit++ ) {
        if( ssl.idict.find(ssit->first) == ssl.idict.end() &&
            sled.insts[ssit->second]->type == NULL ) {
            fprintf( stderr, "Operation in SLED only: %s\n", ssit->first.c_str() );
            rval = false;
            cnt++;
        }
    }
    if( cnt > 0 )
        fprintf( stderr, "Total: %d\n\n", cnt );
    return rval;
}

typedef enum { BASENAME = 1, PREFIX, IFACE_FILE, IMPL_FILE, IFACE_PATH,
               IMPL_PATH, USER_CODE_TOP, USER_CODE_BOTTOM, USER_CODE_IFACE,
               REGS_STRUCT, REGS_DECL, REGS_DEFN, REGS_ACCESS, PARAM_FUNCS,
               FLAG_CALL_FUNCS, OPERATION_FUNCS, EXECUTE_FUNC,
               EXECUTE_ONE_FUNC, DISASM_PARAMS, DISASM_FUNC, REG_DUMP_FUNCS,
               REGISTER_PC } subst_id_t;


string CodeGenEmu::getSSLName( const string &sledname )
{
    string s;
    for( string::const_iterator p = sledname.begin();
         p != sledname.end(); p++ ){
        if( isalnum(*p) || *p=='_' ) s += toupper(*p);
    }
    return s;
}
bool CodeGenEmu::init( int argc, char **argv, bool disasmOnly )
{
    for( int i = 0; i < argc-1; i++ ) {
        if( !sled.readSLEDFile( argv[i] ) )
            return false;
    }
    
    if (!disasmOnly) {
        if( !ssl.readSSLFile( argv[argc-1], false ) )
            return false;
    }

    sledFilename = argv[0];
    
    if (!disasmOnly) {
        sslFilename = argv[argc-1];
        needByteSwap = (TGTENDIAN == BIGE) ^ ssl.bigEndian;
    }

    /* Convert sled names into ssl versions (uppercase and strip out
     * non-alphanumberic characters )
     */
    for( SledDict::inst_iterator i = sled.insts.begin();
         i != sled.insts.end(); i++ ) {
        string s;
        for( string::const_iterator p = i->first.begin();
             p != i->first.end(); p++ ){
            if( isalnum(*p) || *p == '_' ) s += toupper(*p);
        }
        slednamemap[s] = i->first;
    }

    if (!disasmOnly){
        
	computeSynths(); /* Generate rtls for any synthesized instructions
                	  * that don't have their own already */

	sanityCheck(); /* We don't actually care about this right now - it's
                	* just informational */
    }
    /* Prepare substitution lookup */
    substHash["BASENAME"] = BASENAME;
    substHash["INTERFACE_FILE"] = IFACE_FILE;
    substHash["IMPLEMENTATION_FILE"] = IMPL_FILE;
    substHash["INTERFACE_PATH"] = IFACE_PATH;
    substHash["IMPLEMENTATION_PATH"] = IMPL_PATH;
    substHash["USER_CODE_TOP"] = USER_CODE_TOP;
    substHash["USER_CODE_BOTTOM"] = USER_CODE_BOTTOM;
    substHash["USER_CODE_IFACE"] = USER_CODE_IFACE;    
    substHash["PREFIX"] = PREFIX;
    substHash["REGISTER_STRUCT_TYPE"] = REGS_STRUCT;
    substHash["REGISTER_STRUCT_DECL"] = REGS_DECL;
    substHash["REGISTER_STRUCT_DEFN"] = REGS_DEFN;
    substHash["REGISTER_ACCESSORS"] = REGS_ACCESS;
    substHash["REGISTER_DUMP_FUNCS"] = REG_DUMP_FUNCS;
    substHash["PARAM_FUNCS"] = PARAM_FUNCS;
    substHash["FLAG_CALL_FUNCS"] = FLAG_CALL_FUNCS;
    substHash["OPERATION_FUNCS"] = OPERATION_FUNCS;
    substHash["EXECUTE_FUNC"] = EXECUTE_FUNC;
    substHash["EXECUTE_ONE_FUNC"] = EXECUTE_ONE_FUNC;
    substHash["DISASM_PARAMS"] = DISASM_PARAMS;
    substHash["DISASM_FUNC"] = DISASM_FUNC;
    substHash["PC"] = REGISTER_PC;

    /* Computations */
    registerStructTypeName = "RegisterFile";
    registerStructName = "regs";
    
    if (!disasmOnly)
        computeRegisterStructure();
    computeDecodeParam();
    return true;
}

string CodeGenEmu::getSubstitutionFor( const string &subst )
{
    switch( substHash[subst] ) {
        /* Fundamentals */
        case BASENAME:   return getBasename(ifaceFilename);
        case IFACE_FILE: return getFilename(ifaceFilename);
        case IMPL_FILE:  return getFilename(implFilename);
        case IFACE_PATH: return ifaceFilename;
        case IMPL_PATH:  return implFilename;
        case PREFIX:     return prefix;
        case USER_CODE_TOP:
        case USER_CODE_BOTTOM:
        case USER_CODE_IFACE:  return ""; /* Not yet supported */
        /* Precomputed values */
        case REGS_STRUCT: return registerStruct;
        case REGS_ACCESS: return registerAccessFuncs;
        case PARAM_FUNCS: return decodeParamFuncs;
        /* The rest */
        case REGS_DECL: return lang->sprintVariableDecl( registerStructTypeName,
                                                   registerStructName );
        case REGS_DEFN: return lang->sprintVariableDefn( registerStructTypeName,
                                                   registerStructName );
        case REG_DUMP_FUNCS: return sprintDumpRegisters();
        case FLAG_CALL_FUNCS: return string(); //sprintFlagCalls( );
        case OPERATION_FUNCS: return sprintInstructions( );
        case EXECUTE_FUNC:  return sprintExecuteFunc( );
        case EXECUTE_ONE_FUNC: return sprintExecuteOneFunc( );
        case DISASM_FUNC: return sprintDisassembleFunc( );
        case DISASM_PARAMS: return sprintDisassembleParams( );
        case REGISTER_PC: return getreg["%pc"];
        default: return "INVALID_TEMPLATE_VARIABLE(" + subst + ")";

    }
}

bool CodeGenEmu::isActive( const string &str )
{
    if( str == "DISASM" ) return doDisasm;
    if( str == "EMULATOR" ) return doEmu;
    if( str == "BYTESWAP" ) return needByteSwap;
    fprintf( stderr, "GACK! Unknown section '%s' in skeleton file - wtf...\n",
             str.c_str() );
    return true;
}

string CodeGenEmu::getHeader( const string &fname, bool isImpl )
{
    time_t t = time(NULL);
    struct stat st1, st2;
    char buf[512];

    stat(sledFilename.c_str(), &st1);
    stat(sslFilename.c_str(), &st2);
    string t1 = ctime(&t);
    string t2 = ctime(&st1.st_mtime);
    string t3 = ctime(&st2.st_mtime);
    t1.erase(t1.length()-1,1);
    t2.erase(t2.length()-1,1);
    t3.erase(t3.length()-1,1);
    snprintf( buf, sizeof(buf),
              "%s - %s %s file\n\nAutomatically generated by genemu %s on %s from:\n  %-40s (%s)\n  %-40s (%s)",
              fname.c_str(), (doEmu?"Emulator":"Disassembler"),
              (isImpl?"implementation":"interface"), APPVERSION, t1.c_str(),
              sledFilename.c_str(), t2.c_str(), sslFilename.c_str(), t3.c_str() );
    
    return lang->sprintComment( buf );
}

/****************************************************************************/

void usage()
{
    fprintf( stderr, "Usage: genemu [options] <sled-filename(s)> <ssl-filename>\n" );
    fprintf( stderr, "Recognized options:\n" );
    fprintf( stderr, "   -c  output C code [default]\n" );
    fprintf( stderr, "   -d  disassembler only (do not generate emulator core)\n");
    fprintf( stderr, "       Note: ssl-filename not required with -d option\n");
    fprintf( stderr, "   -j  output Java code\n" );
    fprintf( stderr, "   -i  inst-filename: use inst-filename to instrument code.\n" );
    fprintf( stderr, "   -t  test only, no code output\n" );
    fprintf( stderr, "   -m  additionally generate a Makefile to go with the core\n" );
    fprintf( stderr, "   -o <file>  write output to the given file\n" );
              
    exit(1);
}

int main(int argc, char *argv[])
{
    CodeGenEmu *gen = NULL;
    CodeGenLanguage *lang = NULL;
    string oname;
    bool disasmOnly = false, testOnly = false;
    bool genMake = false;
    codegenlang_t langid = LANG_C;
	extern char *optarg;
    char *inst_file = NULL;

    while(1) {
        int c = getopt( argc, argv, "cCdtjo:i:" );
        if( c == -1 ) break;
        switch(c) {
            case 'c': langid = LANG_C; break;
            case 'C': langid = LANG_CPP; break;
            case 'd': disasmOnly = true; break;
            case 'j': langid = LANG_JAVA; break;
            case 'm': genMake = true; break;
	    	case 'i': inst_file = optarg;
    		          break;
            case 't': testOnly = true; break;
            case 'o': oname = optarg; break;
            case '?': fprintf( stderr, "Unrecognized option\n" );
        }
    }
    if( !disasmOnly && optind + 2 > argc ) {
        usage();
    }
    else if(disasmOnly && optind + 1 > argc ) {
        usage();
    }    
    
    lang = CodeGenLanguage::getInstanceFor( langid );
    if( !lang ) {
        fprintf( stderr, "Specified language not yet supported\n" );
        exit(2);
    }
    gen = new CodeGenEmu(lang, DEFAULT_SKEL_SEARCH_PATH );
cerr << "default skeleton search path: " << DEFAULT_SKEL_SEARCH_PATH << "\n"; 
    
    int getSLEDParams = optind;    
    if (disasmOnly)
        --getSLEDParams; // Because we have no SSL, the last file
                         // on the command line is still a SLED file

    if( !gen->init( argc - getSLEDParams, argv+optind, disasmOnly ) )
        abort();

	if (inst_file) {
        cerr << "instrument file = " << inst_file << endl;
		gen->instrm_parser = new InstrmParser(inst_file, 0);
		gen->instrm_parser->yyparse();
	}
	
    if( testOnly ) {
        gen->sprintInstructions(); /* Throw result away */
    } else {
        if( oname == "" ) {
            oname = changeExt( argv[optind+1], lang->getImplExtension().c_str() );
        }
        gen->implFilename = oname;
        gen->ifaceFilename = changeExt( oname, lang->getIfaceExtension().c_str() );
        gen->doEmu = !disasmOnly;
        gen->doDisasm = true;
        
        gen->generateImplFile();
        gen->generateIfaceFile();
        printf( "Output written to %s\n", oname.c_str() );
    }

	
    return 0;
}


/*
 * Stub code needed to link genemu: these are no-op definitions and method
 * implementations referenced but not called from code linked against genemu.
 */

const SemStr* Proc::getReturnLoc() const  { return NULL; }
const char* Proc::getName() const         { return ""; }
Prog::Prog()                              { }
Proc* Prog::findProc(ADDRESS uAddr) const { return NULL; }
Prog prog;
CSR::CSR()                                { }
CSR::~CSR()                               { }
LogueDict::LogueDict()                    { }
Parameters::~Parameters()                 { }
