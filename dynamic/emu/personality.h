/*
 * Copyright (C) 2001, Sun Microsystems, Inc
 *
 * See the file "LICENSE.TERMS" for information on usage and
 * redistribution of this file, and for a DISCLAIMER OF ALL
 * WARRANTIES.
 *
 */

/*==============================================================================
 * File:       personality.h
 * OVERVIEW:   Base class for the OS personality to be emulated.
 *============================================================================*/

/*
 * $Revision: 1.2 $
 * 18 Jun 01 - Nathan: File added to cvs
 */


#ifndef uqbt_personality_h
#define uqbt_personality_h

#include "global.h"
#include "syself.h"

#ifdef HAVE_BYTESWAP_H
#include <byteswap.h>
#else
#define bswap_16(x) ((((x) >> 8) & 0xff) | (((x) & 0xff) << 8))
     
#define bswap_32(x) ((((x) & 0xff000000) >> 24) | (((x) & 0x00ff0000) >>  8) |\
                     (((x) & 0x0000ff00) <<  8) | (((x) & 0x000000ff) << 24))
#endif

//#define DEFAULT_VM_SIZE (64 * 1024 * 1024)
#define DEFAULT_VM_SIZE (256 * 1024 * 1024)
#define STACK_SIZE (2048 * 1024)
//#define PAGE_SIZE 8192
#define PAGE_SIZE pageSize
#define PAGE_ALIGN(x)  ((((x)+PAGE_SIZE-1)/PAGE_SIZE)*PAGE_SIZE)
#define PAGE_OFFSET(x) ((x)%PAGE_SIZE)
#define PAGE_FLOOR(x)  (((x)/PAGE_SIZE)*PAGE_SIZE)

typedef enum { PER_SOLARIS, PER_LINUX } personality_t;

extern int debug, debugsys;

#ifdef DEBUG_EMU
#define dprintf(arg...) if(debug||debugsys) fprintf( stderr , ## arg )
#else
#define dprintf(arg...)
#endif

/* A few external functions which need to be supplied */

void setReg_pc( int );
void setReg_sp( int );
void setReg_CF( int );
void initCore();
void setMem( char * );
void run( );
void stop( int );
int getArchitecture();
personality_t getDefaultPersona();
void dumpMainRegisters( FILE *out );

/* Common macro */
#define ADDR(r) (void *)((r)==0 ? NULL : &vm[r])

/*
 * Base OS personality class. This serves to encapsulate all OS-specifc
 * behaviour so that the emulator core doesn't have to worry about any
 * of it. This is probably a bit *nix-centric at the moment, but we don't
 * support any non-*nix os's yet either (palm will be... interesting...
 * though). Also generally assuming we're running on something reasonably
 * posix compliant.
 */

class Personality {
  public:
    Personality() { /* Provided to make gcc stop complaining */ }
    Personality( int arch );
    static Personality *getInstanceFor( personality_t id, int arch );
    static const char *getNameFor( personality_t id );
    virtual bool execve( const char *filename, const char **argv,
                        const char **envp ) = 0;
    virtual int handleSyscall( int callno, int *parms ) = 0;
    virtual char *getVMBase() { return vm; }
	int pageSize; /* Host page size */

  protected:
    int arch; /* The cpu architecture */
    char *vm;
    bool byteswap;
    int brklimit;

    void putUser32( int addr, uint32_t val );
    void putUser16( int addr, uint16_t val );
    uint32_t getUser32( int addr );
    uint16_t getUser16( int addr );
};

/*
 * For want of something better to call it, this is a superclass for
 * Solaris and Linux (both SysV based) to handle the (fairly significant)
 * amount of commonality. In truth the various BSDs may be able to use
 * this as well, which may warrant a renaming in the future.
 */

class SysVPersonality : public Personality {
  public:
    SysVPersonality( int arch ) : Personality( arch ) { createVM(); }
    virtual bool execve( const char *filename, const char **argv, const char **envp );
    
  protected:
    int createVM();
    int mapElfBinary( Elf *elf, int fd, int base, unsigned *top );
    int processRelocations( Elf *elf, int loadaddr );
    int initElfProcess( int entry, const char **argv, const char **envp, const AUXV_T *auxv );
    virtual int handleAuxv( AUXV_T *auxv ) = 0;
};

/*
 * Solaris personality, obviously. Reports itself as solaris 8 (SunOs 5.8)
 */

class SolarisPersonality : public SysVPersonality {
  public:
    SolarisPersonality( int arch ) : SysVPersonality( arch ) { }
    
    virtual int handleSyscall( int callno, int *parms );

  protected:
    virtual int handleAuxv( AUXV_T *auxv );
};   

/*
 * Linux personality. Reports itself as (and attempts to be compatible
 * with) 2.4.5
 */
class LinuxPersonality : public SysVPersonality {
  public:
    LinuxPersonality( int arch ) : SysVPersonality( arch ) { }
    
    int handleSyscall( int callno, int *parms );
  private:
    int handleWritev( int from, int addr, int count );
    virtual int handleAuxv( AUXV_T *auxv );
};

#endif /* !uqbt_personality_h */
