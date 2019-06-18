/*
 * Copyright (C) 2001, Sun Microsystems, Inc
 *
 * See the file "LICENSE.TERMS" for information on usage and
 * redistribution of this file, and for a DISCLAIMER OF ALL
 * WARRANTIES.
 *
 */


#include "sledtree.h"

extern int yydebug;

int main(int argc, char *argv[])
{
    SledDict dict;

    if( argc < 2 ) {
        fprintf( stderr, "Usage: sledtest <sledfile>\n" );
    }
    for( int i=1; i<argc; i++ ) {
        dict.readSLEDFile( argv[i] );
    }
    dict.printOn(stdout);
    int numInsts = 0, numModes = 0;
    for( SledDict::inst_iterator i = dict.insts.begin(); i != dict.insts.end(); i++ ) {
        if( i->second->type )
            numInsts++;
        else
            numModes++;
    }
    printf( "Total instructions: %d\nTotal modes: %d\nTotal \"types\": %d\n",
             numInsts, numModes, dict.types.size() );
}    
