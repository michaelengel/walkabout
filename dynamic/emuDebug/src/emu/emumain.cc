/*
 * Copyright (C) 2001, Sun Microsystems, Inc
 *
 * See the file "LICENSE.TERMS" for information on usage and
 * redistribution of this file, and for a DISCLAIMER OF ALL
 * WARRANTIES.
 *
 */

/*==============================================================================
 * File:       emumain.cc
 * OVERVIEW:   Main procedure for the emulator
 *============================================================================*/

/*
 * $Revision: 1.2 $
 * 18 Jun 01 - Nathan: File added to cvs
 */


#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <string.h>
#include "personality.h"

void printInstCount();

Personality *persona;

char *mem;

int secondMain (int argc, char **argv, char **env)
{
    int arch = getArchitecture();
    personality_t platform = getDefaultPersona();
    int breakpt = 0;
    unsigned tracefrom = 0;
    unsigned tracept = 0;
    
    while(1) {
        int c = getopt( argc, argv, "+dDp:" );
        if( c == -1 ) break;
        switch(c) {
            case 'd':
                debug = true;
                break;
            case 'D':
                debugsys = true;
                break;
            case 'p':
                if( strcasecmp(optarg, "solaris") == 0 )
                    platform = PER_SOLARIS;
                else if( strcasecmp(optarg, "linux") == 0 )
                    platform = PER_LINUX;
                else {
                    fprintf( stderr, "Unrecognized platform: '%s'\n", optarg );
                    exit(2);
                }
                break;
            case '?':
                fprintf( stderr, "Unrecognized option\n" );
                exit(2);
        }
    }
    if( optind >= argc ) {
        fprintf( stderr, "Usage: emulate [options] <filename> [application-options]\n" );
        fprintf( stderr, "Recognized options:\n" );
        fprintf( stderr, "    -d  Emit debugging trace\n" );
        fprintf( stderr, "    -D  Emuit debugging for syscalls\n" );
        fprintf( stderr, "    -p <solaris|linux> Use the given OS personality [%s]\n",
                 Personality::getNameFor(getDefaultPersona()) );
        exit(1);
    }
    
    initCore();
    persona = Personality::getInstanceFor(platform, arch);
    if( persona->execve( argv[optind], (const char **)argv+optind,
                         (const char **)env ) == false )
        return 1;

#ifndef NDEBUG
//    atexit( printInstCount );
#endif
//    run();
}
