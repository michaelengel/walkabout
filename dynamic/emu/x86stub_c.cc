/*
 * Copyright (C) 2001, Sun Microsystems, Inc
 *
 * See the file "LICENSE.TERMS" for information on usage and
 * redistribution of this file, and for a DISCLAIMER OF ALL
 * WARRANTIES.
 *
 */

/*==============================================================================
 * File:       x86stub_c.cc
 * OVERVIEW:   x86 C language stub methods
 *============================================================================*/

/*
 * $Revision: 1.2 $
 * 18 Jun 01 - Nathan: File added to cvs
 */


#include <stdio.h>
#include "personality.h"
#include "x86emu.h"
#include "global.h"

extern Personality *persona;

int getArchitecture()
{
    return PENT;
}

personality_t getDefaultPersona()
{
    return PER_LINUX;
}

void setReg_sp( int val )
{
    regs.r_esp_un.r_esp = val;
}

void setReg_pc( int val )
{
    regs.r_pc = val;
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
    if( (trapno & 0xFF) == 0x80 ) { 
        /* Syscall under linux - need to check what solaris/x86 does */
        int args[6];
        /* NB: Passing &regs.r_ebx_un.r_ebx would almost do the right thing
         * if regs was correctly sorted, but as it stands...
         */
        args[0] = regs.r_ebx_un.r_ebx;
        args[1] = regs.r_ecx_un.r_ecx;
        args[2] = regs.r_edx_un.r_edx;
        args[3] = regs.r_esi_un.r_esi;
        args[4] = regs.r_edi_un.r_edi;
        args[5] = regs.r_ebp_un.r_ebp;
        persona->handleSyscall( regs.r_eax_un.r_eax, args );
        regs.r_eax_un.r_eax = args[0];
        
    }
}

void initCore( )
{
}

void printInstCount()
{
    fprintf( stderr, "Interpreted %ld instructions\n", icount );
}
