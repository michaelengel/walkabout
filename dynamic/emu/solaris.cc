/*
 * Copyright (C) 2001, Sun Microsystems, Inc
 *
 * See the file "LICENSE.TERMS" for information on usage and
 * redistribution of this file, and for a DISCLAIMER OF ALL
 * WARRANTIES.
 *
 */

/*==============================================================================
 * File:       solaris.cc
 * OVERVIEW:   Solaris personality support (syscall handler)
 *============================================================================*/

/*
 * $Revision: 1.2 $
 * 18 Jun 01 - Nathan: File added to cvs
 */


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

#ifndef __linux__
#include <sys/systeminfo.h>
#include <sys/sysconfig.h>
#else
#define getpgid __getpgid
extern "C" {
pid_t getsid(pid_t pid);
}

#endif 

#define KILL(signum)
#define R_SP 14

#define instruction_access_exception  0x01
#define instruction_access_MMU_miss   0x3C
#define instruction_access_error      0x21
#define illegal_instruction           0x02
#define privileged_instruction        0x03
#define fp_disabled                   0x04
#define fp_exception                  0x08
#define tag_overflow                  0x0A
#define division_by_zero              0x2A
#define data_access_exception         0x09
#define data_access_MMU_miss          0x2C
#define data_access_error             0x29

#define SOLARIS_TCGETA 0x5401
#define SOLARIS_TCGETS 0x540D
#define SOLARIS_TIOCGWINSZ 0x5468
#define SOLARIS_TIOCGPGRP 0x7414
#define SOLARIS_MNTIOC_NMNTS 0x6d01
#define SOLARIS_MNTIOC_GETDEVLIST 0x6d02

extern unsigned brklimit, memlimit;

void dumpMainRegisters( FILE * );
void setReg_pc( int val );
void setReg_sp( int val );
void setReg_CF( int val );
void setReg_rd( int n, int val );
void stop( int val );

static int handlers[NSIG];

static void signalHandler(int signum)
{
#ifdef C_ONLY
    struct RegisterFile oldregs = regs; /* save cpu context */
    setReg_pc( handlers[signum] );
    setReg_rd(15, -1 - 8); /* 8 byte displacement on call return... */; 
    setReg_rd(8, signum);
#ifdef DEBUG
    fprintf( stderr, "Entering signal handler(%d) at 0x%x\n", signum, regs.r_pc );
#endif
    /* FIXME */
    while( regs.r_pc != -1 ) {
        executeOneInstruction();
    }
    regs = oldregs; /* restore context */
#endif
}

#define ISSPECIALACT(x) (((int)(x))<4 && ((int)(x))>=-1)
typedef void sigaction_func_t(int,siginfo_t *, void *);
typedef void sighandler_func_t(int);

int handleSigaction( int sig, const struct sigaction *pact, struct sigaction *oact )
{
    int oldh = handlers[sig];
    
    if( pact != NULL ) {
        struct sigaction act = *pact;
        if( act.sa_flags & SA_SIGINFO ) {
            if( !ISSPECIALACT(act.sa_sigaction) ) {
                handlers[sig] = (int)act.sa_sigaction;
                act.sa_sigaction = (sigaction_func_t *)signalHandler;
            }
        } else {
            if( !ISSPECIALACT(act.sa_handler) ) {
                handlers[sig] = (int)act.sa_handler;
                act.sa_handler = (sighandler_func_t *)signalHandler;
            }
        }
        pact = &act;
    }

    int result = sigaction( sig, pact, oact );
    if( oact != NULL && result != -1 ) {
        /* fixup returned signal handler if necessary */
        if( oact->sa_flags & SA_SIGINFO ) {
            if( !ISSPECIALACT(oact->sa_sigaction) ) {
                oact->sa_sigaction = (sigaction_func_t *)oldh;
            }
        } else {
            if( !ISSPECIALACT(oact->sa_handler) ) {
                oact->sa_handler = (sighandler_func_t *)oldh;
            }
        }
    }
    return result;
}

static int handleSysconfig( int name )
{
    switch(name) {
        /* Note: Taken from solaris 8. YMMV */
        case 3: return sysconf(_SC_CHILD_MAX);
        case 4: return sysconf(_SC_OPEN_MAX);
        case 5: return sysconf(_SC_VERSION);
        case 6: return sysconf(_SC_PAGESIZE);
        case 7: return sysconf(_SC_CLK_TCK);
        case 8: return 3; /* _CONFIG_XOPEN_VERSION */
        default:
            fprintf(stderr, "Unsupported call to _sysconfig(%d)\n", name );
            return -1;
    }
    
}

static int handleSysinfo( int cmd, char *buf, long count )
{
    /* Values here basically taken from the sparc on my desk, ie
     * an ultra 5 running solaris 8 */
    switch( cmd ) {
        case 0x0001: /* SI_SYSNAME */ strncpy( buf, "SunOS", count ); break;
        case 0x0002: /* SI_HOSTNAME */
            struct utsname un;
            uname( &un );
            strncpy( buf, un.nodename, count );
            break;
        case 0x0003: /* SI_RELEASE */ strncpy( buf, "5.8", count ); break;
        case 0x0004: /* SI_VERSION */ strncpy( buf, "Generic_108528-07", count ); break;
        case 0x0005: /* SI_MACHINE */ strncpy( buf, "sun4u", count ); break;
        case 0x0006: /* SI_ARCHITECTURE */ strncpy( buf, "sparc", count ); break;
        case 0x0201: strncpy( buf, "SUNW,Ultra-5_10", count ); break;
        default: fprintf( stderr, "Unsupported call to sysinfo(%d)\n", cmd );
    }
    return 0;
}

struct solaris_utsname {
    char sysname[257];
    char nodename[257];
    char release[257];
    char version[257];
    char machine[257];
};

static int handleUname( struct solaris_utsname *sunp )
{
    /* Probably should really insert fake uname here */
    struct utsname un;
    int err = uname( &un );
    if( err == -1 ) return -1;
    strncpy( sunp->sysname, un.sysname, 257 );
    strncpy( sunp->nodename, un.nodename, 257 );
    strncpy( sunp->release, un.release, 257 );
    strncpy( sunp->version, un.version, 257 );
    strncpy( sunp->machine, un.machine, 257 );    
    return err;
}

struct solaris_stat {
    int st_dev;
    int st_pad1[3];
    int st_ino;
    int st_mode;
    int st_nlink;
    int st_uid;
    int st_gid;
    int st_rdev;
    int st_pad2[2];
    int st_size;
    int st_pad3;
    int st_atime_sec;
    int st_atime_usec;
    int st_mtime_sec;
    int st_mtime_usec;
    int st_ctime_sec;
    int st_ctime_usec;
    int st_blksize;
    int st_blocks;
    char st_fstype[16];
};

static void convertStat( struct solaris_stat *out, struct stat *in )
{
    memset( out, 0, sizeof(struct solaris_stat) );
    out->st_dev = htonl(in->st_dev);
    out->st_ino = htonl(in->st_ino);
    out->st_mode = htonl(in->st_mode);
    out->st_nlink = htonl(in->st_nlink);
    out->st_uid = htonl(in->st_uid);
    out->st_gid = htonl(in->st_gid);
    out->st_rdev = htonl(in->st_rdev);
    out->st_size = htonl(in->st_size);
    out->st_blocks = htonl(in->st_blocks);
    out->st_atime_sec = htonl(in->st_atime);
    out->st_mtime_sec = htonl(in->st_mtime);
    out->st_ctime_sec = htonl(in->st_ctime);
    out->st_blksize = htonl(in->st_blksize);
}

#define o0 parms[0]
#define o1 parms[1]
#define o2 parms[2]
#define o3 parms[3]
#define o4 parms[4]
#define o5 parms[5]

int SolarisPersonality::handleSyscall( int callno, int *parms )
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
            dprintf( "\nopen( '%s', %x, %x )", (char *)ADDR(o0),o1,o2 );
            err = open( (char *)ADDR(o0), o1, o2 ); break;
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
        case 17: /* brk */
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
        case 18: /* stat */
            err = stat( (char *)ADDR(o0), &st );
            if( !err ) convertStat( (struct solaris_stat *)ADDR(o1), &st );
            break;
        case 19: /* lseek */ err = lseek( o0, o1, o2 ); break;
        case 20: /* getpid */
            err = getpid();
            o1 = getppid();
            if( o1 == -1 ) err = -1;
            break;
//        case 21: /* mount */
//        case 22: /* umount */            
        case 23: /* setuid */ err = setuid( o0 ); break;
        case 24: /* getuid */
            err = getuid();
            o1 = geteuid();
            if( o1 == -1 ) err = -1;
            break;
        case 28: /* fstat */
            err = fstat( o0, &st );
            if( !err ) convertStat( (struct solaris_stat *)ADDR(o1), &st );
            break;
        case 29: /* pause */ err = pause(); break;
        case 30: /* utime */ err = utime( (char *)ADDR(o0), (struct utimbuf *)ADDR(o1) ); break;
        case 33: /* access */ err = access( (char *)ADDR(o0), o1 ); break;
        case 34: /* nice */ err = nice( o0 ); break;
        case 37: /* kill */ err = kill( o0, o1 ); break;
        case 39: /* SYS_pgrpsys */
            switch( o0 ) {
                case 0: err = getpgrp(); break;
                case 1: err = setpgrp(); break;
                case 2: err = getsid( o1 ); break;
                case 3: err = setsid(); break;
                case 4: err = getpgid( o1 ); break;
                case 5: err = setpgid( o1, o2 ); break;
                default:
                    fprintf( stderr, "Error: unknown SYS_pgrpsys(%d)\n", o0 );
                    abort();
            }
            break;
        case 41: /* dup */  err = dup( o0 ); break;
        case 42: /* pipe */ err = pipe( (int *)ADDR(o0) ); break;
        case 46: /* setgid */ err = setgid( o0 ); break;
        case 47: /* getgid */
            err = getgid();
            o1 = getegid();
            if( o1 == -1 ) err = -1;
            break;
        case 59: /* execve */ /* FIXME: add some special handling here */
            err = ::execve( (char *)ADDR(o0), (char **)ADDR(o1), (char **)ADDR(o2) );
            break;
        case 60: /* umask */ err = umask( o0 ); break;
        case 61: /* chroot */ err = chroot( (char *)ADDR(o0) ); break;
        case 62: /* fcntl */
            switch( o1 ) { /* command */
                case F_GETLK:
                case F_SETLK:
                case F_SETLK64:
                case F_SETLKW:
                case F_SETLKW64:
                    err = fcntl( o0, o1, ADDR(o2) );
                    break;
                default:
                    err = fcntl( o0, o1, o2 );
            }
            break;
        case 79: /* rmdir */ err = rmdir( (char *)ADDR(o0) ); break;
        case 80: /* mkdir */ err = mkdir( (char *)ADDR(o0), o1 ); break;
        case 88: /* lstat */ 
            err = lstat( (char *)ADDR(o0), &st );
            if( !err ) convertStat( (struct solaris_stat *)ADDR(o1), &st );
            break;
        case 89: /* symlink */ err = symlink( (char *)ADDR(o0), (char *)ADDR(o1) ); break;
        case 90: /* readlink */ err = readlink( (char *)ADDR(o0), (char *)ADDR(o1), o2 ); break;
        case 93: /* fchmod */ err = fchmod( o0, o1 ); break;
        case 94: /* fchown */ err = fchown( o0, o1, o2 ); break;
        case 95: /* sigprocmask */ err = sigprocmask( o0, (sigset_t *)ADDR(o1), (sigset_t *)ADDR(o2) ); break;
        case 96: /* sigsuspend */ err = sigsuspend( (sigset_t *)ADDR(o0) ); break;
        case 97: /* sigaltstack */ err = sigaltstack( (stack_t *)ADDR(o0), (stack_t *)ADDR(o1) ); break;
        case 98: /* sigaction */ err = handleSigaction( o0, (struct sigaction *)ADDR(o1), (struct sigaction *)ADDR(o2) ); break;
        case 99: /* sigpending */ err = sigpending( (sigset_t *)ADDR(o0) ); break;
        case 115: /* mmap */
#ifdef __linux__ /* We don't have memcntl, so for the time being... */
            o2 = PROT_READ|PROT_WRITE|PROT_EXEC;
#endif
            flags = o3&0x1F;
            if( o3 & 0x100 ) flags |= MAP_ANON;
            
            if( o0 == 0 )
                err = (int)mmap( NULL, o1, o2, flags, o4, o5 );
            else
                err = (int)mmap( (caddr_t)ADDR(o0), o1, o2, flags, o4, o5 );
            if( err != -1 ) err = err - (int)vm;
//            fprintf( stderr, "mmap(0x%x,0x%x,0x%x,0x%x,0x%x,0x%x) = 0x%x\n",
//                     o0, o1, o2,
//                     o3, o4, o5, err );
            break;
        case 117: /* munmap */ err = munmap( (caddr_t)ADDR(o0), o1 ); break;
        case 135: /* uname */ err = handleUname( (struct solaris_utsname *)ADDR(o0) ); break;
        case 137: /* sysconfig */ err = handleSysconfig(o0); break;
        case 139: /* sysinfo */ err = handleSysinfo(o0, (char *)ADDR(o1), o2); break;
        case 175: /* llseek */ err = lseek( o0, o1, o2 ); break;

#ifndef __linux__ /* Solaris calls */
        case 100: /* get/setcontext */
            if( o0 == 0 ) err = getcontext( (ucontext_t *)ADDR(o1) );
            else if( o0 == 1 ) err = setcontext( (ucontext_t *)ADDR(o1) );
            else {
                fprintf( stderr, "bad syscall: SYS_context(%d,%x)\n", o0,o1);
                abort();
            }
            break;
        case 131: /* memcntl */
            err = memcntl((caddr_t)ADDR(o0), o1, o2,
                          (char *)o3, o4, o5 );
            break;
        case 213: /* getdents64 */ err = getdents( o0, (struct dirent *)ADDR(o1), o2 ); break;
        case 215: /* stat64 */  err = stat64( (char *)ADDR(o0), (struct stat64 *)ADDR(o1) ); break;
        case 216: /* lstat64 */ err = lstat64( (char *)ADDR(o0), (struct stat64 *)ADDR(o1) ); break;
        case 217: /* fstat64 */ err = fstat64( o0, (struct stat64 *)ADDR(o1) ); break;
        case 225: /* open64 */ err = open64( (char *)ADDR(o0), o1, o2 ); break;
#else
        case 131: err = 0; break; /* memcntl: Fake it */
            
#endif
        case 230: /* so_socket */ err = socket( o0, o1, o2 ); break;
        case 232: /* bind */ err = bind( o0, (struct sockaddr *)ADDR(o1), o3 ); break;
        case 233: /* listen */ err = listen( o0, o1 ); break;
        case 234: /* accept */ err = accept( o0, (struct sockaddr *)ADDR(o1), (socklen_t *)ADDR(o2) ); break;
        case 235: /* connect */ err = connect( o0, (struct sockaddr *)ADDR(o1), o2 ); break;
        case 236: /* shutdown */ err = shutdown( o0, o1 ); break;
        case 237: /* recv */ err = recv( o0, (char *)ADDR(o1), o2, o3 ); break;
        case 238: /* recvfrom */ err = recvfrom( o0, (char *)ADDR(o1), o2, o3,
                                                 (struct sockaddr *)ADDR(o4), (socklen_t *)ADDR(o5) ); break;
        case 240: /* send */err = send( o0, ADDR(o1), o2, o3 ); break;
        case 242: /* sendto */ err = sendto( o0, (char *)ADDR(o1), o2, o3,
                                             (struct sockaddr *)ADDR(o4), o5 ); break;
        case 243: /* getpeername */ err = getpeername( o0, (struct sockaddr *)ADDR(o1), (socklen_t *)ADDR(o2) ) ; break;
        case 244: /* getsockname */ err = getsockname( o0, (struct sockaddr *)ADDR(o1), (socklen_t *)ADDR(o2) ) ; break;
        case 54: /* ioctl */ /* oh fsck...*/
            switch( o1 ) {
                case SOLARIS_TCGETA:
                    err = ioctl( o0, TCGETA, ADDR(o2) ); break;
                case SOLARIS_TCGETS:
                case SOLARIS_TIOCGWINSZ: case SOLARIS_TIOCGPGRP:
                case SOLARIS_MNTIOC_NMNTS: case SOLARIS_MNTIOC_GETDEVLIST:
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
    }
    
    if( err == -1 ) {
        setReg_CF( 1 );
        o0 = -errno;
        dprintf( " = ERR %d\n", errno );
    } else {
        setReg_CF( 0 );
        o0 = err;
        dprintf( " = %x\n", err );
    }
    return o0;
}

int SolarisPersonality::handleAuxv( AUXV_T *auxv )
{
#define AUX_ENT( n, t, v ) auxv[n].a_type = t; auxv[n].a_un.a_val = v;
	AUX_ENT(0, AT_SUN_UID, geteuid() );
    AUX_ENT(1, AT_SUN_RUID, getuid() );
    AUX_ENT(2, AT_SUN_GID, getegid() );
    AUX_ENT(3, AT_SUN_RGID, getgid() );
    AUX_ENT(4, AT_SUN_EXECNAME, 0 ); /* Name will be filled in later */
#undef AUX_ENT
    return 5;
}
