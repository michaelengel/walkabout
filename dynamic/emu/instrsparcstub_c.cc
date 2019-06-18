/*
 * Copyright (C) 2001, Sun Microsystems, Inc
 *
 * See the file "LICENSE.TERMS" for information on usage and
 * redistribution of this file, and for a DISCLAIMER OF ALL
 * WARRANTIES.
 *
 */

/*==============================================================================
 * File:       instrsparcstub_c.cc
 * OVERVIEW:   Sparc C language stub methods
 *============================================================================*/

/*
 * $Revision: 1.2 $
 * 18 Jun 01 - Nathan: File added to cvs
 * 13 Sep 01 - Cristina: Made a copy of sparcstub_c.cc for instrumentation 
 *		purposes (different include file spathfinder.h)
 */


#include <stdio.h>
#include "personality.h"
#include "spathfinder.h"

extern Personality *persona;

int getArchitecture()
{
    return SPARC;
}

personality_t getDefaultPersona()
{
    return PER_SOLARIS;
}

void setReg_rd( int num, int val )
{
    regs.rd[num] = val;
}

void setReg_sp( int val )
{
    regs.rd[14] = val;
}

void setReg_pc( int val )
{
    regs.r_pc = val;
    regs.r_npc = val+4;
}

void setReg_CF( int val )
{
    regs.r_CF = val ? 1 : 0;
}

void setMem( char *vm )
{
    mem = vm;
}

void stop( int o0 )
{
    exit(o0);
}

void doTrap( int trapno )
{
    if( trapno == 136 ) {
        persona->handleSyscall( regs.rd[1], &regs.rd[8] );
    }
}

void initCore( )
{
}

void printInstCount()
{
    fprintf( stderr, "Interpreted %ld instructions\n", icount );
}
