/****************************************************************************
*
*                            Open Watcom Project
*
*    Portions Copyright (c) 1983-2002 Sybase, Inc. All Rights Reserved.
*
*  ========================================================================
*
*    This file contains Original Code and/or Modifications of Original
*    Code as defined in and that are subject to the Sybase Open Watcom
*    Public License version 1.0 (the 'License'). You may not use this file
*    except in compliance with the License. BY USING THIS FILE YOU AGREE TO
*    ALL TERMS AND CONDITIONS OF THE LICENSE. A copy of the License is
*    provided with the Original Code and Modifications, and is also
*    available at www.sybase.com/developer/opensource.
*
*    The Original Code and all software distributed under the License are
*    distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
*    EXPRESS OR IMPLIED, AND SYBASE AND ALL CONTRIBUTORS HEREBY DISCLAIM
*    ALL SUCH WARRANTIES, INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF
*    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR
*    NON-INFRINGEMENT. Please see the License for the specific language
*    governing rights and limitations under the License.
*
*  ========================================================================
*
* Description:  Module implementing linux system calls interface
*
****************************************************************************/

// TODO: I think this should all be split up into separate modules for
//       each of the functions implemented to ensure the runtime library
//       will static link as small as possible. KB.

#include <stdarg.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/ptrace.h>
#include "syslinux.h"

/* user-visible error numbers are in the range -1 - -124 */

#define __syscall_return(type, res)                     \
if ((u_long)(res) >= (u_long)(-125)) {                  \
    errno = -(res);                                     \
    res = (u_long)-1;                                   \
}                                                       \
return (type)(res);

u_long sys_brk(u_long brk)
{
    u_long newbrk;

    newbrk = sys_call1(SYS_brk,brk);
    if( newbrk < brk )
        return (u_long)-1;
    return newbrk;
}

long sys_exit(int error_code)
{
    return sys_call1(SYS_exit,error_code);
}

long sys_open(const char * filename, int flags, int mode)
{
    u_long res = sys_call3(SYS_open, (u_long)filename, flags, mode);
    __syscall_return(ssize_t,res);
}

_WCRTLINK char *getcwd( char *__buf, size_t __size )
{
    u_long res = sys_call2(SYS_getcwd, (u_long)__buf, __size);
    __syscall_return(char *,res);
}

_WCRTLINK int fcntl( int __fildes, int __cmd, ... )
{
    u_long      rest;
    va_list     args;
    u_long      res;

    va_start( args, __cmd );
    rest = va_arg( args, u_long );
    va_end( args );
    res = sys_call3(SYS_fcntl, (u_long)__fildes, (u_long)__cmd, rest);
    __syscall_return(int,res);
}

_WCRTLINK int ioctl( int __fd, unsigned long int __request, ... )
{
    u_long      argp;
    va_list     args;
    u_long      res;

    va_start( args, __request );
    argp = va_arg( args, u_long );
    va_end( args );
    res = sys_call3(SYS_ioctl, (u_long)__fd, (u_long)__request, argp);
    __syscall_return(int,res);
}

_WCRTLINK void (*signal(int signum, void (*sighandler)(int)))(int)
{
    u_long res = sys_call2(SYS_signal, (u_long)signum, (u_long)sighandler);
    __syscall_return(void (*)(int),res);
}

_WCRTLINK int unlink( const char *filename )
{
    u_long res = sys_call1(SYS_unlink, (u_long)filename);
    __syscall_return(int,res);
}

_WCRTLINK time_t time( time_t *t )
{
    u_long res = sys_call1(SYS_time, (u_long)time);
    __syscall_return(int,res);
}

_WCRTLINK int gettimeofday( struct timeval *__tv, struct timezone *__tz )
{
    u_long res = sys_call2(SYS_gettimeofday, (u_long)__tv, (u_long)__tz);
    __syscall_return(int,res);
}

_WCRTLINK extern int settimeofday( const struct timeval *__tv, const struct timezone *__tz )
{
    u_long res = sys_call2(SYS_settimeofday, (u_long)__tv, (u_long)__tz);
    __syscall_return(int,res);
}

_WCRTLINK int access( const char *filename, int mode )
{
    u_long res = sys_call2(SYS_access, (u_long)filename, mode);
    __syscall_return(int,res);
}

_WCRTLINK int stat( const char *filename, struct stat * __buf )
{
    u_long res = sys_call2(SYS_stat, (u_long)filename, (u_long)__buf);
    __syscall_return(int,res);
}

_WCRTLINK int fstat( int __fildes, struct stat * __buf )
{
    u_long res = sys_call2(SYS_fstat, __fildes, (u_long)__buf);
    __syscall_return(int,res);
}

_WCRTLINK ssize_t read( int __fildes, void *__buf, size_t __len )
{
    u_long res = sys_call3(SYS_read, __fildes, (u_long)__buf, __len);
    __syscall_return(ssize_t,res);
}

_WCRTLINK ssize_t write( int __fildes, const void *__buf, size_t __len )
{
    u_long res = sys_call3(SYS_write, __fildes, (u_long)__buf, __len);
    __syscall_return(ssize_t,res);
}

_WCRTLINK off_t lseek( int __fildes, off_t __offset, int __whence )
{
    u_long res = sys_call3(SYS_lseek, __fildes, __offset, __whence);
    __syscall_return(off_t,res);
}

_WCRTLINK int close( int __fildes )
{
    u_long res = sys_call1(SYS_close, __fildes);
    __syscall_return(int,res);
}

_WCRTLINK int fsync( int __fildes )
{
    u_long res = sys_call1(SYS_fsync, __fildes);
    __syscall_return(int,res);
}

_WCRTLINK int ioperm( unsigned long __from, unsigned long __num, int __turn_on )
{
    u_long res = sys_call3(SYS_ioperm, __from, __num, __turn_on);
    __syscall_return(int,res);
}

_WCRTLINK int iopl( int __level )
{
    u_long res = sys_call1(SYS_iopl, __level);
    __syscall_return(int,res);
}

_WCRTLINK int nice( int __val )
{
    u_long res = sys_call1(SYS_nice, __val);
    __syscall_return(int,res);
}

_WCRTLINK int select( int __width, fd_set * __readfds, fd_set * __writefds, fd_set * __exceptfds, struct timeval * __timeout )
{
    u_long res = sys_call5(SYS_select, __width, (u_long)__readfds, (u_long)__writefds, (u_long)__exceptfds, (u_long)__timeout);
    __syscall_return(int,res);
}

_WCRTLINK int ptrace(int request, int pid, int addr, int data)
{
	long    res,ret;
	
    if (request > 0 && request < 4)
        *((long**)&data) = &ret;
    res = sys_call4(SYS_ptrace, request, pid, addr, data);
	if (res >= 0) {
		if (request > 0 && request < 4) {
			errno = 0;
			return (ret);
		}
		return (int)res;
	}
	errno = -res;
	return -1;
}

int readlink(const char * path, char *buf, size_t bufsiz)
{
    u_long res = sys_call3(SYS_readlink, (u_long)path, (u_long)buf, bufsiz);
    __syscall_return(int,res);
}
