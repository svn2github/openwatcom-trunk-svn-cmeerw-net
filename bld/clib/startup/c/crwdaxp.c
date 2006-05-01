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
* Description:  C runtime read/write data (Alpha AXP version).
*
****************************************************************************/


#include "variety.h"
#include <wctype.h>

_WCRTLINKD unsigned int   _dynend;      // top of dynamic data area
_WCRTLINKD unsigned int   _curbrk;      // top of usable memory
_WCRTLINKD char *         _LpCmdLine;   // pointer to raw command line
_WCRTLINKD char *         _LpPgmName;   // pointer to program name (for argv[0])
_WCRTLINKD char *         _LpDllName;   // pointer to dll name (for OS/2,WIN32)
_WCRTLINKD wchar_t *      _LpwCmdLine;  // pointer to wide raw command line
_WCRTLINKD wchar_t *      _LpwPgmName;  // pointer to wide program name (for argv[0])
_WCRTLINKD wchar_t *      _LpwDllName;  // pointer to wide dll name (for OS/2,WIN32)
_WCRTLINKD unsigned int   _STACKLOW;    // lowest address in stack
_WCRTLINKD unsigned int   _STACKTOP;    // highest address in stack
_WCRTLINKD void *         __ASTACKSIZ;  // alternate stack size
_WCRTLINKD void *         __ASTACKPTR;  // alternate stack pointer
_WCRTLINKD unsigned int   _cbyte;       // used by getch, getche
_WCRTLINKD unsigned int   _cbyte2;      // used by getch, getche
_WCRTLINKD char *         _Envptr;      // offset part of environment pointer
_WCRTLINKD unsigned short _Envseg;      // segment containing environment strings
_WCRTLINKD unsigned char  _osmajor;     // major OS version number
_WCRTLINKD unsigned char  _osminor;     // minor OS version number
_WCRTLINKD unsigned short _osbuild;     // operating system build number
_WCRTLINKD unsigned int   _osver;       // operating system build number
_WCRTLINKD unsigned int   _winmajor;    // operating system major version number
_WCRTLINKD unsigned int   _winminor;    // operating system minor version number
_WCRTLINKD unsigned int   _winver;    	// operating system version number
_WCRTLINKD void (*__FPE_handler)( int );// f-p exception handler
