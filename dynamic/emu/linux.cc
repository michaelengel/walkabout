/*
 * Copyright (C) 2001, Sun Microsystems, Inc
 *
 * See the file "LICENSE.TERMS" for information on usage and
 * redistribution of this file, and for a DISCLAIMER OF ALL
 * WARRANTIES.
 *
 */

/*==============================================================================
 * File:       linux.cc
 * OVERVIEW:   Implmentation of linux personality for emulation.
 *             (ie the syscall handler)
 *============================================================================*/

/*
 * $Revision: 1.2 $
 * 18 Jun 01 - Nathan: File added to cvs
 * 30 Jul 01 - David:  Changed _FILE_OFFSET_BITS to 32 so that 
 *			/usr/include/libelf.h won't complain.
 */


/* #define _FILE_OFFSET_BITS 64 */
#define _FILE_OFFSET_BITS 32
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/signal.h>
#include <sys/utsname.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/wait.h>
#include <sys/termios.h>
#include <netinet/in.h>
#include <ucontext.h>
#include <utime.h>
#include <time.h>
#include <dirent.h>
#include <assert.h>
#include "syself.h"
#include "personality.h"

#define LINUX_TCGETS          0x5401
#define LINUX_TCSETS          0x5402
#define LINUX_TCSETSW         0x5403
#define LINUX_TCSETSF         0x5404
#define LINUX_TCGETA          0x5405
#define LINUX_TCSETA          0x5406
#define LINUX_TCSETAW         0x5407
#define LINUX_TCSETAF         0x5408
#define LINUX_TCSBRK          0x5409
#define LINUX_TCXONC          0x540A
#define LINUX_TCFLSH          0x540B
#define LINUX_TIOCEXCL        0x540C
#define LINUX_TIOCNXCL        0x540D
#define LINUX_TIOCSCTTY       0x540E
#define LINUX_TIOCGPGRP       0x540F
#define LINUX_TIOCSPGRP       0x5410
#define LINUX_TIOCOUTQ        0x5411
#define LINUX_TIOCSTI         0x5412
#define LINUX_TIOCGWINSZ      0x5413
#define LINUX_TIOCSWINSZ      0x5414

#define o0 parms[0]
#define o1 parms[1]
#define o2 parms[2]
#define o3 parms[3]
#define o4 parms[4]
#define o5 parms[5]

struct linux_utsname {
    char sysname[65];
    char nodename[65];
    char release[65];
    char version[65];
    char machine[65];
};

static int handleUname( struct linux_utsname *lun )
{
    /* Probably should really insert fake uname here */
    struct utsname un;
    int err = uname( &un );
    if( err == -1 ) return -1;
    strncpy( lun->sysname, un.sysname, 65 );
    strncpy( lun->nodename, un.nodename, 65 );
    strncpy( lun->release, un.release, 65 );
    strncpy( lun->version, un.version, 65 );
    strncpy( lun->machine, un.machine, 65 );
    return err;
}

int LinuxPersonality::handleWritev( int fd, int addr, int count )
{
    struct iovec *cnv = (struct iovec *)malloc( sizeof(struct iovec)*count );
    for( int i=0; i<count; i++ ) {
        cnv[i].iov_base = (caddr_t)(ADDR(getUser32( addr+(i*8) )));
        cnv[i].iov_len = getUser32( addr+(i*8)+4 );
    }
    int err = writev( fd, cnv, count );
    free(cnv);
    return err;
}

#ifdef LINUX
struct linux_stat64 {
    __dev_t st_dev;                     /* Device.  */
    uint32_t __pad1;

    __ino_t __st_ino;                   /* 32bit file serial number.    */
    __mode_t st_mode;                   /* File mode.  */
    __nlink_t st_nlink;                 /* Link count.  */
    __uid_t st_uid;                     /* User ID of the file's owner. */
    __gid_t st_gid;                     /* Group ID of the file's group.*/
    __dev_t st_rdev;                    /* Device number, if device.  */
    unsigned int __pad2;
    __off64_t st_size;                  /* Size of file, in bytes.  */
    __blksize_t st_blksize;             /* Optimal block size for I/O.  */

    __blkcnt64_t st_blocks;             /* Number 512-byte blocks allocated. */
    __time_t st_atime;                  /* Time of last access.  */
    unsigned long int __unused1;
    __time_t st_mtime;                  /* Time of last modification.  */
    unsigned long int __unused2;
    __time_t st_ctime;                  /* Time of last status change.  */
    unsigned long int __unused3;
    __ino64_t st_ino;                   /* File serial number.          */
};
#endif

int LinuxPersonality::handleSyscall( int callno, int *parms )
{
    int err, flags;
    struct stat st;

    dprintf( "Syscall %d [%x,%x,%x,%x,%x,%x]",
             callno, o0, o1, o2, o3, o4, o5 );
    
    switch( callno ) {
        case 1: /* exit */ stop(o0); err = 0; break;
        case 2: /* fork */ err = fork(); break;
        case 3: /* read */ err = read( o0, (char *)ADDR(o1), o2 ); break;
        case 4: /* write */ err = write( o0, (char *)ADDR(o1), o2 ); break;
        case 5: /* open */
            dprintf( "\nopen( '%s', %x, %x )", (char *)ADDR(o0), o1, o2 );
            err = open( (char *)ADDR(o0), o1, o2 );
            break;
        case 6: /* close */ err = close( o0 ); break;
        case 7: /* wait */ err = wait( (int *)ADDR(o0) ); break;
        case 8: /* creat */ err = creat( (char *)ADDR(o0), o1 ); break;
        case 9: /* link */ err = link( (char *)ADDR(o0), (char *)ADDR(o1) ); break;
        case 10: /* unlink */ err = unlink( (char *)ADDR(o0) ); break;
//        case 11: /* exec */
        case 12: /* chdir */ err = chdir( (char *)ADDR(o0) ); break;
        case 13: /* time */ err = time( (time_t *)ADDR(o0) ); break;
        case 14: /* mknod */ err = mknod( (char *)ADDR(o0), o1, o2 ); break;
        case 15: /* chmod */ err = chmod( (char *)ADDR(o0), o1 ); break;
        case 16: /* chown */ err = chown( (char *)ADDR(o0), o1, o2 ); break;
        case 19: /* lseek */ err = lseek( o0, o1, o2 ); break;
        case 20: /* getpid */ err = getpid(); break;
        case 45: /* brk */
            /* The documented brk(2) isn't even half of it... */
            if( o0 != 0 ) {
                /* Map in some anonymous pages from oldbrk to newbrk */
                int oldbrk = PAGE_ALIGN(brklimit);
                int newbrk = PAGE_ALIGN(o0);
                if( newbrk > oldbrk ) {
                    void *res = mmap( vm + oldbrk, newbrk - oldbrk,
                                      PROT_READ|PROT_WRITE|PROT_EXEC,
                                      MAP_ANON|MAP_FIXED|MAP_PRIVATE, -1, 0 );
                    if( res == vm + oldbrk )
                        brklimit = o0;
                } else if( newbrk < oldbrk ) {
                    munmap( vm + newbrk, newbrk-oldbrk );
                    brklimit = o0;
                } else
                    brklimit = o0;
            }
            err = brklimit;
            break;
        case 55: /* fcntl */
        case 221: /* fcntl64 */
            switch( o1 ) { /* command */
                case F_GETLK:
                case F_SETLK:
//                case F_SETLK64:
                case F_SETLKW:
//                case F_SETLKW64:
                    err = fcntl( o0, o1, ADDR(o2) );
                    break;
                default:
                    err = fcntl( o0, o1, o2 );
            }
            break;
        case 64: err = getppid(); break;
        case 65: err = getpgrp(); break;
        case 90: /* mmap */
            /* It's not my fault... honest... */
            o1 = getUser32(o0+4);
            o2 = getUser32(o0+8);
            o3 = getUser32(o0+12);
            o4 = getUser32(o0+16);
            o5 = getUser32(o0+20);
            o0 = getUser32(o0);
            /* NB: We're assuming that the stub won't write these back to the
             * registers, ie only o0 will be returned (and it gets overwritten
             * in a moment anyway.
             */
            /* fallthrough */
        case 192: /* mmap2 */
            dprintf( "\nmmap(%x,%x,%x,%x,%d,%x)", o0,o1,o2,o3,o4,o5 );
            flags = o3&0x1F;
            if( o3 & 0x20 ) flags |= MAP_ANON;
            
            if( o0 == 0 )
                err = (int)mmap( NULL, o1, o2, flags, o4, o5 );
            else
                err = (int)mmap( (caddr_t)ADDR(o0), o1, o2, flags, o4, o5 );
            if( err != -1 ) err = err - (int)vm;
            break;
        case 91: /* munmap */ err = munmap( (caddr_t)ADDR(o0), o1 ); break;
        case 122: /* uname */ err = handleUname( (struct linux_utsname *)ADDR(o0) ); break;
        case 125: /* mprotect */ err = mprotect( (char *)(ADDR(o0)), (size_t)o1, (int)o2 ); break;
        case 146: /* writev */ err = handleWritev( o0, o1, o2 ); break;
        case 195: /* stat64 */ err = stat( (char *)ADDR(o0), (struct stat *)ADDR(o1) );
        case 197: /* fstat64 */ err = fstat( o0, (struct stat *)ADDR(o1) ); break;
        case 199: /* getuid32 */ err = getuid(); break;
        case 200: /* getgid32 */ err = getgid(); break;
        case 201: /* geteuid32 */ err = geteuid(); break;
        case 202: /* getegid32 */ err = getegid(); break;
#if 0
        case 220: /* getdents64 */ err = getdents64( ADDR(o0), o1, (struct dirent *)ADDR(o2), o3 ); break;
#endif
        case 54: /* ioctl */ /* oh fsck...*/
            switch( o1 ) {
                case LINUX_TCGETA:
                    err = ioctl( o0, TCGETA, ADDR(o2) ); break;
                case LINUX_TCGETS:
                case LINUX_TIOCGWINSZ: case LINUX_TIOCGPGRP:
                    err = ioctl( o0, o1, ADDR(o2) ); break;
                    
                default:
                    dprintf( "Warning: unimplemented ioctl( %d, 0x%x (%c%c), ... )\n",
                             o0, o1, (o1>>8)&0xFF, o1&0xFF );
                    err = ioctl( o0, o1, ADDR(o2) ); break; // assume
            }
            break;
        default:
#ifdef DEBUG_EMU
            dprintf( "Warning: unimplemented syscall: %d\n", callno );
            if(debug||debugsys) dumpMainRegisters( stderr );
#endif
            err = -1;
            errno = ENOSYS;
            break;

    }
    
    if( err == -1 ) {
        setReg_CF( 1 );
        o0 = -errno;
        dprintf( " = ERR %d\n", errno );
    } else {
        setReg_CF( 0 );
        o0 = err;
        dprintf( " = %d\n", o0 );
    }
    return o0;    
}

int LinuxPersonality::handleAuxv( AUXV_T *auxv )
{
#define AUX_ENT( n, t, v ) auxv[n].a_type = t; auxv[n].a_un.a_val = v;
    AUX_ENT(0, AT_LNX_UID, getuid() );
    AUX_ENT(1, AT_LNX_EUID, geteuid() );
    AUX_ENT(2, AT_LNX_GID, getgid() );
    AUX_ENT(3, AT_LNX_EGID, getegid() );
#undef AUX_ENT
    return 4;
}
