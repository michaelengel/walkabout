/*
 * Copyright (C) 2001, Sun Microsystems, Inc
 *
 * See the file "LICENSE.TERMS" for information on usage and
 * redistribution of this file, and for a DISCLAIMER OF ALL
 * WARRANTIES.
 *
 */

/*==============================================================================
 * File:       sysv.cc
 * OVERVIEW:   Implementation of methods common to sysv implementations
 *             (It's probably more general than that, but we don't yet support
 *             anything else with which to test it. Primarily this is
 *             the elf binary loader and process initializer.
 *============================================================================*/

/*
 * $Revision: 1.2 $
 * 18 Jun 01 - Nathan: File added to cvs
 * 23 Jun 01 - David:  Create physical stack such that the memory is available
 *				       physically at top of the virtual address.
 * 09 Jul 01 - David:  Eliminated the VM.
*/


#include <sys/mman.h>
#include <stdio.h>
#include "personality.h"
#include "syself.h"

/*
 * This undoubtedly belongs somewhere else. Such as in the main uqbt tree
 */
static char *archNames[] = { "0","Sparc","80x86","Mc68k","Win32","HP PA-RISC", "80286" };

/* Labels taken directly from elf.h. Blame them. NB: I'm really not
 * terribly keen on entering the entire set in, so be my guest...
 */
static char *elfArchNames[] = {
    "None", "AT&T WE 32100", "SUN SPARC", "Intel 80386", "Motorola m68k family",
    "Motorola m88k family", "#UNDEF6", "Intel 80860", "MIPS R3000 big-endian",
    "IBM System/370", "MIPS R3000 little-endian", "#UNDEF11", "#UNDEF12",
    "#UNDEF13", "#UNDEF14", "HPPA", "#UNDEF16", "Fujitsu VPP500",
    "Sun's \"v8plus\"", "Intel 80960", "PowerPC", "PowerPC 64-bit", "IBM S390" };
#define MAX_KNOWN_ELF_ARCH 22

static char *getElfArchName(unsigned id)
{
    static char buf[16]; /* Usual non-MT-safe disclaimers apply, yes */
    if( id > MAX_KNOWN_ELF_ARCH ) {
        sprintf( buf, "%d", id );
        return buf;
    } else {
        return elfArchNames[id];
    }
}
           
unsigned progtop = 0;

int SysVPersonality::mapElfBinary( Elf *elf, int fd, int base, unsigned *top )
{
    int loadaddr=0;
    bool haveLoadaddr = false;
    Elf32_Phdr *phdr = elf32_getphdr(elf);
    Elf32_Ehdr *ehdr = elf32_getehdr(elf);
//    unsigned progtop = 0;

    for( int i=0; i< ehdr->e_phnum; i++ ) {
        if( phdr[i].p_type == PT_LOAD ) {
            if( !haveLoadaddr ) {
                loadaddr = phdr[i].p_vaddr - phdr[i].p_offset + base;
                haveLoadaddr = true;
            }

            /* Need to make sure we align mmaps on page boundaries */
            int bias = PAGE_OFFSET( phdr[i].p_vaddr );
            int flags = 0;
            if( phdr[i].p_flags & PF_R ) flags |= PROT_READ;
            if( phdr[i].p_flags & PF_W ) flags |= PROT_WRITE;
            if( phdr[i].p_flags & PF_X ) flags |= PROT_EXEC;
            void *addr = vm + phdr[i].p_vaddr-bias+base;
            void *res = mmap( (char*)addr, phdr[i].p_filesz+bias, flags,
                              MAP_FIXED | MAP_PRIVATE, fd, phdr[i].p_offset-bias );
            if( res != addr ) {
                fprintf( stderr, "Mapping executable segment at %08X (%08X) failed (%s)\n",
                         phdr[i].p_vaddr, (int)addr,
                         strerror(errno));
                return -1;
            }
            if( phdr[i].p_memsz > phdr[i].p_filesz ) {
                int mapfrom = PAGE_ALIGN(phdr[i].p_vaddr + phdr[i].p_filesz);
                int mapto = PAGE_ALIGN(phdr[i].p_vaddr + phdr[i].p_memsz);
                if( mapfrom < mapto ) {
                    addr = vm + mapfrom + base;
                    //addr = vm + mapfrom;
                    int len = mapto - mapfrom;
                    res = mmap( (char*)addr, len, flags,
                                MAP_FIXED | MAP_ANON | MAP_PRIVATE, -1, 0 );
                    if( res != addr ) {
                        fprintf( stderr, "Mapping anonymous memory at end of segment at %08X (%08X) failed (%s)\n",
                                 phdr[i].p_vaddr + mapfrom, (int)addr,
                                 strerror(errno) );
                        return -1;
                    }
                }
                /* Map in additional pages if needed, and Zero out */
                memset( vm+phdr[i].p_vaddr + phdr[i].p_filesz + base, 0,
                        phdr[i].p_memsz - phdr[i].p_filesz );
            }
            if( phdr[i].p_vaddr + phdr[i].p_memsz > progtop )
                progtop = phdr[i].p_vaddr + phdr[i].p_memsz;
        }
    }
    if( top != NULL ) *top = progtop;
    
    return loadaddr;
}

/*
 * It turns out that we should never need to do this - ld.so is
 * supposed to be able to relocate itself. Nevertheless we keep it
 * around just in case it should ever be needed...
 */
int SysVPersonality::processRelocations( Elf *elf, int loadaddr )
{
    Elf32_Ehdr *ehdr = elf32_getehdr(elf);
    Elf_Scn *scn = NULL;
    Elf32_Shdr *sect;
    while( (scn = elf_nextscn(elf, scn)) != NULL ) {
        sect = elf32_getshdr( scn );
        if( sect->sh_type == SHT_RELA ) {
            dprintf("Found relocations in section %s\n",
                    elf_strptr( elf,  ehdr->e_shstrndx, sect->sh_name ) );
            unsigned addr = sect->sh_addr;
            while( addr < sect->sh_addr+sect->sh_size ) {
                Elf32_Rela rel;
                rel.r_offset = getUser32(addr);
                rel.r_info = getUser32(addr+4);
                rel.r_addend = getUser32(addr+8);
                switch ELF32_R_TYPE(rel.r_info) {
                    case R_SPARC_NONE:
                        break;
                    case R_SPARC_RELATIVE:
                        putUser32( rel.r_offset+loadaddr, loadaddr + rel.r_addend );
                        break;
//                    case R_386_RELATIVE: /* ? */
//                    case R_386_GLOB_DAT:
//                   case R_386_JMP_SLOT:
//                        break;
                    default:
                        dprintf("Unknown relocation type: %x\n", rel.r_info );
                        break;
                        
                }
                addr += sizeof(rel);
            }
            break;
        }
    }
    return 0;
}

/*
 * This is a fairly general purpose execve which should be able to handle
 * any ELF file. For other file types, well, I guess you're out of luck.
 * We can't really use the uqbt BinaryFile interface because we need quite
 * a few internal details that it doesn't supply (and uqbt doesn't need)
 *
 * Yes, I know it leaks file descriptors...
 */
bool SysVPersonality::execve( const char *filename, const char **argv,
                              const char **envp )
{
    int loadaddr = 0;
    int interp_loadaddr = 0;
    int entry = 0;
    
    int fd = open( filename, O_RDONLY );
    if( fd == -1 ) {
        fprintf( stderr, "Unable to execute file: %s (%s)\n", filename,
                 strerror(errno) );
        return false;
    }
    /* We could probably do this without libelf, but this does make some
     * things marginally simpler (ie dealing with byteswapping...)
     */
    if( elf_version(EV_CURRENT) == EV_NONE ) {
        fprintf( stderr, "libelf version is incompatible. You probably need to recompile this program\n" );
        return false;
    }
    Elf *elf = elf_begin(fd, ELF_C_READ, NULL);
    if( elf == NULL ) {
        fprintf( stderr, "elf_begin failed on %s. This is probably not an elf file\n", filename );
        return false;
    }
    Elf32_Ehdr *ehdr = elf32_getehdr(elf);
    if( ehdr->e_type != ET_DYN && ehdr->e_type != ET_EXEC ) {
        fprintf( stderr, "Not an executable elf file: %s\n", filename );
        return false;
    }
    bool archOk = false;
    switch(arch) {
        case SPARC: archOk = (ehdr->e_machine == EM_SPARC); break;
        case PENT:  archOk = (ehdr->e_machine == EM_386); break;
        case MC68K: archOk = (ehdr->e_machine == EM_68K); break;
        case HPPA:  archOk = (ehdr->e_machine == EM_PARISC); break;
        default:
            fprintf( stderr, "I don't actually recognize the architecture that I seem to be compiled with.\nYou'll probably need to update personality.cc to explain to mean what\narchitecture code %d means\n", arch );
            return false;
    }
    if( !archOk ) {
        fprintf( stderr, "I'm expecting to emulate a %s core, but this is a %s ELF file. If you're really\nsure that this should work, please update personality.cc\n",
                 archNames[arch], getElfArchName(ehdr->e_machine) );
        return false;
    }
    /* Checks out okay so far, okay now map in the program segments */
    unsigned topofexec;
    loadaddr = mapElfBinary(elf, fd, 0, &topofexec);
    if( loadaddr == -1 )
        return false;
    brklimit = topofexec;
    entry = ehdr->e_entry;

	// Create physical stack at top of the virtual address.
	// this address range also holds the runtime linker
	void *st = mmap((char*)(DEFAULT_VM_SIZE-STACK_SIZE), STACK_SIZE, 
				PROT_READ|PROT_WRITE|PROT_EXEC, MAP_FIXED | MAP_ANON | MAP_PRIVATE, -1, 0 );
	if( (unsigned)st != DEFAULT_VM_SIZE-STACK_SIZE) {
		fprintf( stderr, "Mapping stack at %08X failed (%s)\n",
                         DEFAULT_VM_SIZE-STACK_SIZE, strerror(errno));
        return -1;
	}
    
    /* Now scan the section table for an interpreter */
    Elf_Scn *scn = NULL;
    Elf32_Shdr *sect;
    char *interp = NULL;
    while( (scn = elf_nextscn(elf, scn)) != NULL ) {
        sect = elf32_getshdr( scn );
        char *name = elf_strptr( elf, ehdr->e_shstrndx, sect->sh_name );
        if( strcmp( name, ".interp" ) == 0 ) {
            Elf_Data *data = elf_getdata( scn, NULL );
            interp = strdup( (char *)data->d_buf );
            break;
        }
    }
    if( interp ) {
        dprintf( "Loading interpreter from: %s\n", interp );
        int ldfd = open( interp, O_RDONLY );
        if( ldfd == -1 ) {
            fprintf( stderr, "Unable to open interpreter: %s\n", interp );
            return false;
        }
        Elf *ldElf = elf_begin(ldfd, ELF_C_READ, NULL);
        if( ldElf == NULL ) {
            fprintf( stderr, "elf_begin failed on %s. This is probably not an elf file\n", interp );
            return false;
        }
        Elf32_Ehdr *ldEhdr = elf32_getehdr(ldElf);
        if( ldEhdr->e_type != ET_DYN && ldEhdr->e_type != ET_EXEC ) {
            fprintf( stderr, "Interpreter can't be executed: %s\n", interp );
            return false;
        }
        if( ldEhdr->e_machine != ehdr->e_machine ) {
            fprintf( stderr, "Interpreter architecture (%s)",
                     getElfArchName(ldEhdr->e_machine) );
            fprintf( stderr, " doesn't match executable (%s)\n",
                     getElfArchName(ehdr->e_machine) );
            return false;
        }
        /*
         * Note on the interpreter loadaddr - this is really only needed
         * on solaris, because for some strange reason other the binary
         * and ld.so seem to like to share the same address mapping...
         */
        interp_loadaddr = mapElfBinary(ldElf, ldfd,
                                       DEFAULT_VM_SIZE - STACK_SIZE, NULL);
        entry = interp_loadaddr + ldEhdr->e_entry;
        if( interp_loadaddr == -1 )
            return false;
        elf_end(ldElf);
        close(ldfd);
    }
    
    
    /* Construct the auxilliary vector
     * Note: all addresses must be vm relative
     */
#define AUX_ENT( n, t, v ) auxv[n].a_type = t; auxv[n].a_un.a_val = v;
    AUXV_T auxv[12];
    AUX_ENT(0, AT_PHDR, loadaddr + ehdr->e_phoff );
    AUX_ENT(1, AT_PHENT, sizeof(Elf32_Phdr) );
    AUX_ENT(2, AT_PHNUM, ehdr->e_phnum );
    AUX_ENT(3, AT_FLAGS, 0 );
    AUX_ENT(4, AT_ENTRY, ehdr->e_entry );
    AUX_ENT(5, AT_BASE, interp_loadaddr);
    int j = handleAuxv(auxv+6);
	AUX_ENT(j+6, AT_NULL, 0);
#undef AUX_ENT
    
    elf_end(elf);
    close(fd);

    /* Get ready to rumble! :) */
    initElfProcess( entry, argv, envp, auxv );
    return true;
}

/*
 * Setup the basic VM parameters as needed.
 */
int SysVPersonality::createVM( )
{
	#ifndef DIRECT_MAP
    	vm = (char *)mmap( NULL, DEFAULT_VM_SIZE, PROT_READ|PROT_WRITE,
        	              MAP_ANON|MAP_PRIVATE, -1, 0 );
	    if( vm == MAP_FAILED ) {
    	    fprintf( stderr, "Unable to allocate memory for virtual machine\n" );
        	abort();
	    }
	#else
		// VM is no longer created, just mmap the sections to where they are suppose to go.
		vm = 0;
	#endif
    return 0;
}

int SysVPersonality::initElfProcess( int entry, const char *argv[],
                                     const char *env[], const AUXV_T *auxv )
{
    int align, ptrsz, stackgap;
    switch( arch ) {
        case SPARC:
            align = 8;
            ptrsz = 4;
            stackgap = 64;
            break;
        case PENT:
        default:
            align = 4;
            ptrsz = 4;
            stackgap = 0;
    }
    
#define ALIGN(x) (((x+align-1)/align)*align)
#define ALIGNPAGE(x) (((x)>>12)<<12)

    int env_count = 0, env_size = 0, i;
    int arg_size = 0, aux_count = 0;
    int argc;

    for( argc=0; argv[argc] != NULL; argc++ )
        arg_size += ALIGN(strlen(argv[argc])+1);
    
    while( env[env_count] != NULL ) {
        env_size += ALIGN(strlen(env[env_count])+1);
        env_count++;
    }
    while( auxv[aux_count].a_type != AT_NULL ) aux_count++;
    aux_count++; /* make sure we include the AT_NULL entry */

//#ifdef DEBUG_EMU
//    env_count = 0;
//    env_size = 0;
//#endif
    
    /* ref Sparc compliance Definition */
    int ptr_size = (argc+1)*ptrsz + (env_count+1)*ptrsz;
    int sp = DEFAULT_VM_SIZE -
        stackgap - /* register space for sparc */
        4 -  /* argc */
        ptr_size - aux_count*sizeof(AUXV_T) - arg_size - env_size ;
    sp = ALIGN(sp-7);
    int arg_base = sp + stackgap + 4 + ptr_size + aux_count*sizeof(AUXV_T);
    int env_base = arg_base + arg_size;
    int argv0_addr = arg_base;

    setReg_pc( entry );
    setReg_sp( sp );
    dprintf( "Initial sp: %08x\n", sp );
    sp+=stackgap;
    putUser32(sp, argc);
    dprintf( "%08x: argc => %08x\n", sp, argc );
    sp+=4;
    for( i=0; i<argc; i++, sp+=ptrsz ) {
        putUser32(sp,arg_base);
        dprintf( "%08x: &argv[%d] => %08x\n", sp, i, arg_base );
        strcpy( &vm[arg_base], argv[i] );
        arg_base += ALIGN(strlen(argv[i])+1);
    }
    putUser32(sp,0);
    dprintf( "%08x: &argv[%d] => %08x\n", sp, i, 0 );
    sp+=ptrsz;
    assert(arg_base <= env_base);
    for( i=0; i<env_count; i++, sp+=ptrsz ) {
        putUser32(sp, env_base);
        dprintf( "%08x: &env[%d] => %08x\n", sp, i, env_base );
        strcpy( &vm[env_base], env[i] );
        env_base += ALIGN(strlen(env[i])+1);
    }
    putUser32(sp, 0);
    dprintf( "%08x: &env[%d] => %08x\n", sp, i, 0 );
    sp+=ptrsz;
    assert( sp+aux_count*sizeof(AUXV_T) <= (unsigned)arg_base );
    dprintf( "%08x: auxv[%d] (%d bytes)\n", sp, aux_count,
             aux_count*sizeof(AUXV_T) );
    for( i=0; i < aux_count; i++ ) {
        putUser32( sp, auxv[i].a_type ); sp+=4;
        if( auxv[i].a_type == AT_SUN_EXECNAME ) {
            putUser32( sp, argv0_addr );
        } else {
            putUser32( sp, auxv[i].a_un.a_val );
        }
        dprintf( "  %08x: auxv[%d] = { %d, %08x }\n", sp, i,
                 auxv[i].a_type, getUser32(sp) );
        sp+=4;
    }

    setMem( vm );
    return 0;
}


/************* A few "generic" posix/sysv handlers **************/

/* TBA */
