/*
 * Copyright (C) 2001, Sun Microsystems, Inc
 *
 * See the file "LICENSE.TERMS" for information on usage and
 * redistribution of this file, and for a DISCLAIMER OF ALL
 * WARRANTIES.
 *
 */

/*=============================================================================
 * FILE:       codegen.cc
 * OVERVIEW:   Generic code generator methods from CodeGenLanguage and 
 *             CodeGenApp
 *===========================================================================*/

/* $Revision: 1.2 $
 * 23 Apr 01 - Nathan: Initial version
 */


#include "codegen.h"
#include "codegen_c.h"
#include "codegen_java.h"
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>

CodeGenLanguage *CodeGenLanguage::getInstanceFor( codegenlang_t lang )
{
    switch(lang) {
        case LANG_C: return new CodeGenC();
        case LANG_JAVA: return new CodeGenJava();
        default: return NULL;
    }
}

bool CodeGenLanguage::isTypeSupported( const Type &type )
{
    if( type.getType() == FLOATP )
        return type.getSize() <= getMaxFloatType().getSize();
    else if( type.getType() == INTEGER )
        return type.getSize() <= getMaxIntType().getSize();
    return true; /* Assume so */
}

bool CodeGenApp::processSkeleton( ostream &out, istream &in )
{
    char buf[512];
    while( !in.eof() ) {
        in.getline(buf,sizeof(buf));
        
        if( buf[0] == '@' ) { /* Conditional blocks */
            for( char *p = buf; *p; p++ ) {
                if( isspace(*p) ) {
                    *p = '\0';
                    break;
                }
            }
            string s(buf+1);
            if( (buf[1] == '!' && isActive(buf+2)) ||
                (buf[1] != '!' && !isActive(buf+1)) ) {
                /* Skip until we find the same tag */
                while( !in.eof() ) {
                    in.getline(buf,sizeof(buf));
                    for( char *p = buf; *p; p++ ) {
                        if( isspace(*p) ) {
                            *p = '\0';
                            break;
                        }
                    }
                    if( buf[0] == '@' && s == string(buf+1) )
                        break;
                }
                if( in.eof() )
                    break;
            }
            continue; /* either way skip this line */
        }
        for( char *p = buf; *p; p++ ) {
            if( *p == '$' ) {
                char *q = ++p;
                string subst;
                if( *p == '{' ) {
                    q++;
                    while( *p && *p != '}' ) p++;
                    subst = string(q,p);
                } else {
                    while( *p && (isalnum(*p) || *p == '_') ) p++;
                    subst = string(q,p);
                }
                out << getSubstitutionFor( subst );
            } else out << *p;
        }
        out << '\n';
    }
    return true;
}

bool CodeGenApp::generateImplFile( )
{
    string skelname = findSkeletonFile(getSkelBasename() + lang->getImplExtension());
    if( skelname == "" ) return false;

    ofstream out(implFilename.c_str());
    if( !out.is_open() ) {
        cerr << "Unable to open output implementation file (" << implFilename
             << ") for writing\n";
        return false;
    }
    ifstream in(skelname.c_str());
    if( !in.is_open() ) {
        cerr << "Unable to open skeleton implementation file (" << skelname
             << ")\n";
        return false;
    }
    out << getHeader( implFilename, true );
    return processSkeleton( out, in );
}

bool CodeGenApp::generateIfaceFile( )
{
    if( !lang->hasInterface() )
        return true; /* Silently do nothing */
        
    string skelname = findSkeletonFile(getSkelBasename() + lang->getIfaceExtension());
    if( skelname == "" ) return false;
    
    ofstream out(ifaceFilename.c_str());
    if( !out.is_open() ) {
        cerr << "Unable to open output interface file (" << ifaceFilename 
             << ") for writing\n";
        return false;
    }
    ifstream in(skelname.c_str());
    if( !in.is_open() ) {
        cerr << "Unable to open skeleton interface file (" << skelname
             << ")\n";
        return false;
    }
    out << getHeader( ifaceFilename, false );
    return processSkeleton( out, in );
}

string CodeGenApp::getFilename( const string &path )
{
    int n = path.find_last_of( '/', path.length() );
    if( n != -1 )
        return  path.substr( n+1 );
    else return path;
}

string CodeGenApp::getBasename( const string &path )
{
    int n = path.find_last_of( '.', path.length() );
    int m = path.find_last_of( '/', ( n==-1 ? path.length() : n) );
    return path.substr( m+1, ( n==-1 ? path.length() : n )-m-1 );
}

/*
 * Temporary total kludge until I find the correct way to do this on 
 * non-solaris platforms. (argv[0] is not always helpful, funnily enough)
 */
#ifndef HAVE_GETEXECNAME
char *getexecname()
{
    return "./tools/";
}
#endif

string CodeGenApp::getBinaryPath( )
{
    string fname = getexecname();
    int n = fname.find_last_of( '/', fname.length() );
    if( n == -1 ) {
        return ".";
    } else {
        return fname.substr(0,n);
    }
}

void CodeGenApp::addSkeletonSearchPath( const string &pathlist )
{
    string binPath = getBinaryPath();
    
    char *s = strdup(pathlist.c_str());
    char *p = strtok(s, ":;");
    while( p ) {
        string a(p);
        searchPaths.push_back( searchAndReplace(p,"$BINDIR",binPath) );
        p = strtok(NULL, ":;" );
    }
    free(s);
}

void CodeGenApp::setSkeletonSearchPath( const string &pathlist )
{
    searchPaths.clear();
    addSkeletonSearchPath( pathlist );
}

string CodeGenApp::findSkeletonFile( const string &fname )
{
    for( list<string>::iterator i = searchPaths.begin();
         i != searchPaths.end(); i++ ) {
        string s = *i + "/" + fname;
        if( access(s.c_str(), R_OK ) == 0 )
            return s;
    }
    cerr << "Unable to find skeleton file: " << fname << "\n";
    return "";
}
