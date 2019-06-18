/*
 * Copyright (C) 2001, Sun Microsystems, Inc
 *
 * See the file "LICENSE.TERMS" for information on usage and
 * redistribution of this file, and for a DISCLAIMER OF ALL
 * WARRANTIES.
 *
 */

/*==============================================================================
 * File:       personality.cc
 * OVERVIEW:   Implmentation of base personality class - factory and a few
 *             common support methods
 *============================================================================*/

/*
 * $Revision: 1.2 $
 * Nathan Keynes - created.
 *
 */


#include <stdio.h>
#include "personality.h"

int debugsys;

Personality *Personality::getInstanceFor( personality_t id, int arch )
{
    switch( id ) {
        case PER_SOLARIS: return new SolarisPersonality(arch);
//        case PER_LINUX: return new LinuxPersonality(arch);
    }
    return NULL; /* No such persona */
}

const char *Personality::getNameFor( personality_t id )
{
    switch( id ) {
        case PER_SOLARIS: return "Solaris";
        case PER_LINUX: return "Linux";
    }
    return "IHaveNoIdeaWhatYoureTalkingAbout";
}

Personality::Personality( int a )
{
    union {
        short i;
        char b[sizeof(short)];
    } un;
    
    arch = a;
	pageSize = getpagesize();

    /* detect whether we need to do byteswapping */
    memset( &un, 0, sizeof(un) );
    un.b[0] = 0x12; un.b[1] = 0x34;
    
    if( arch == SPARC || arch == MC68K ) {
        byteswap = (un.i != 0x1234);
    } else {
        byteswap = (un.i == 0x1234);
    }
}

void Personality::putUser32( int addr, uint32_t value )
{
    (*((uint32_t *)(&vm[addr]))) = (byteswap ? bswap_32(value) : value );
}

void Personality::putUser16( int addr, uint16_t value )
{
    (*((uint16_t *)(&vm[addr]))) = (byteswap ? bswap_16(value) : value );
}

uint32_t Personality::getUser32( int addr )
{
    return (byteswap ? bswap_32(*((uint32_t *)(&vm[addr])))
            : (*((uint32_t *)(&vm[addr]))) );
}

uint16_t Personality::getUser16( int addr )
{
    return (byteswap ? bswap_16(*((uint16_t *)(&vm[addr])))
            : (*((uint16_t *)(&vm[addr]))) );
}

