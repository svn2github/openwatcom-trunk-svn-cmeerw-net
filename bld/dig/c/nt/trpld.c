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
* Description:  WHEN YOU FIGURE OUT WHAT THIS FILE DOES, PLEASE
*               DESCRIBE IT HERE!
*
****************************************************************************/


#include <stdio.h>
#include <windows.h>
#include <dos.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include "trpimp.h"
#include "tcerr.h"

static HANDLE   TrapFile;
static trap_version (TRAPENTRY *InitFunc)();
static void (TRAPENTRY *FiniFunc)();
static void (TRAPENTRY *InfoFunction)( HWND );

extern trap_version     TrapVer;
extern unsigned         (TRAPENTRY *ReqFunc)( unsigned, mx_entry *,
                                        unsigned, mx_entry * );

void TellHWND( HWND hwnd )
{
    if( InfoFunction != NULL ) {
        InfoFunction( hwnd );
    }
}

void KillTrap()
{
    ReqFunc = NULL;
    if( FiniFunc != NULL ) FiniFunc();
    FiniFunc = NULL;
    InfoFunction = NULL;
    InitFunc = NULL;
    if( TrapFile != 0 ) FreeLibrary( TrapFile );
    TrapFile = 0;
}

char *LoadTrap( char *trapbuff, char *buff, trap_version *trap_ver )
{
    char                trpfile[256];
    char                *ptr;
    char                *parm;
    char                *dst;
    char                have_ext;
    char                chr;

    if( trapbuff == NULL ) trapbuff = "std";
    have_ext = FALSE;
    ptr = trapbuff;
    dst = (char *)trpfile;
    for( ;; ) {
        chr = *ptr;
        if( chr == '\0' || chr == ';' ) break;
        switch( chr ) {
        case ':':
        case '/':
        case '\\':
            have_ext = 0;
            break;
        case '.':
            have_ext = 1;
            break;
        }
        *dst++ = chr;
        ++ptr;
    }
    if( !have_ext ) {
        *dst++ = '.';
        *dst++ = 'd';
        *dst++ = 'l';
        *dst++ = 'l';
    }
    *dst = '\0';
    parm = (*ptr != '\0') ? ptr + 1 : ptr;
    TrapFile = LoadLibrary( trpfile );
    if( TrapFile == NULL ) {
        sprintf( buff, TC_ERR_CANT_LOAD_TRAP, trpfile );
        return( buff );
    }
    InitFunc = (LPVOID) GetProcAddress( TrapFile, (LPSTR)1 );
    FiniFunc = (LPVOID) GetProcAddress( TrapFile, (LPSTR)2 );
    ReqFunc  = (LPVOID) GetProcAddress( TrapFile, (LPSTR)3 );
    InfoFunction = (LPVOID) GetProcAddress( TrapFile, (LPSTR)4 );
    strcpy( buff, TC_ERR_WRONG_TRAP_VERSION );
    if( InitFunc == NULL || FiniFunc == NULL || ReqFunc == NULL
        /* || LibListFunc == NULL */ ) {
        KillTrap();
        return( buff );
    }
    *trap_ver = InitFunc( parm, trpfile, trap_ver->remote );
    if( trpfile[0] != '\0' ) {
        KillTrap();
        strcpy( buff, (char *)trpfile );
        return( buff );
    }
    if( !TrapVersionOK( *trap_ver ) ) {
        KillTrap();
        return( buff );
    }
    TrapVer = *trap_ver;
    return( NULL );
}
