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
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "stdnt.h"
#include "watcom.h"
#include "trperr.h"
#include "srvcdbg.h"
#define ERR_CODES
#include "dosmsgs.h"

#ifndef CREATE_SEPARATE_WOW_VDM
#define CREATE_SEPARATE_WOW_VDM     0x00000800  // new for NT 3.5 (daytona)
#endif

/*
 * executeUntilStart - run program until start address hit
 */
static BOOL executeUntilStart( BOOL was_running )
{
    HANDLE      ph,th;
    brkpnt_type old;
    brkpnt_type brk = BRK_POINT;
    LPVOID      base;
    DWORD       bytes;
    CONTEXT     con;
    thread_info *ti;

    ph = DebugEvent.u.CreateProcessInfo.hProcess;
    th = DebugEvent.u.CreateProcessInfo.hThread;
    if( !was_running ) {
        /*
         * if we are not debugging an already running app, then we
         * plant a breakpoint at the first instruction of our new app
         */
        base = DebugEvent.u.CreateProcessInfo.lpStartAddress;
        ReadProcessMemory( ph, (LPVOID) base, (LPVOID) &old, sizeof(old), (LPDWORD) &bytes );
        WriteProcessMemory( ph, (LPVOID) base, (LPVOID) &brk, sizeof(brk), (LPDWORD) &bytes );
    } else {
        // a trick to make app execute long enough to hit a breakpoint
        PostMessage( HWND_TOPMOST, WM_TIMECHANGE, 0, 0 );
    }

    for( ;; ) {
        /*
         * if we encounter anything but a break point, then we are in
         * trouble!
         */
        if( DebugExecute( STATE_IGNORE_DEBUG_OUT | STATE_IGNORE_DEAD_THREAD,
                        NULL, FALSE ) & COND_BREAK ) {
            ti = FindThread( DebugEvent.dwThreadId );
            MyGetThreadContext( ti, &con );
            if( was_running ) {
                AdjustIP( &con, sizeof( brk ) );
                MySetThreadContext( ti, &con );
                return( TRUE );
            }
            if( StopForDLLs ) {
                /*
                 * the user has asked us to stop before any DLL's run
                 * their startup code (";dll"), so we do.
                 */
                WriteProcessMemory( ph, (LPVOID) base, (LPVOID) &old, sizeof(old),
                                        (LPDWORD) &bytes );
                AdjustIP( &con, sizeof( brk ) );
                MySetThreadContext( ti, &con );
                return( TRUE );
            }
            if( (AdjustIP( &con, 0 ) == (DWORD) base) ) {
                /*
                 * we stopped at the applications starting address,
                 * so we can offically declare that the app has loaded
                 */
                WriteProcessMemory( ph, (LPVOID) base, (LPVOID) &old, sizeof(old),
                                        (LPDWORD) &bytes );
                return( TRUE );
            }
            /*
             * skip this breakpoint and continue
             */
            AdjustIP( &con, sizeof( brk ) );
            MySetThreadContext( ti, &con );
        } else {
            return( FALSE );
        }
    }

} /* executeUntilStart */

#ifdef WOW
/*
 * addKERNEL - add the KERNEL module to the library load (WOW)
 */
static void addKERNEL( void )
{
    #if 0
        /*
         * there are bugs in the way VDMDBG.DLL implements some of this
         * stuff, so this is currently disabled
         */
        MODULEENTRY             me;
        thread_info             *ti;
        IMAGE_NOTE              im;

        ti = FindThread( DebugeeTid );
        me.dwSize = sizeof( MODULEENTRY );
        if( pVDMModuleFirst( ProcessInfo.process_handle, ti->thread_handle,
                        &me, NULL, 0 ) ) {
            do {
                if( !memicmp( me.szModule, "KERNEL", 6 ) ) {
                    memcpy( &im.Module, &me.szModule, sizeof( me.szModule ) );
                    memcpy( &im.FileName, &me.szExePath, sizeof( me.szExePath ) );
                    AddLib( TRUE, &im );
                    break;
                }
                me.dwSize = sizeof( MODULEENTRY );
            } while( pVDMModuleNext( ProcessInfo.process_handle, ti->thread_handle,
                            &me, NULL, 0 ) );
        }
    #else
        IMAGE_NOTE              im;

        /*
         * this is a giant kludge, but it works.  Since KERNEL is already
         * loaded in the WOW , we never get a DLL load notification, so
         * we can't show any symbols.  This fakes up the necessary information
         */
        strcpy( im.Module, "KERNEL" );
        GetSystemDirectory( im.FileName, sizeof( im.FileName ) );
        strcat( im.FileName, "\\KRNL386.EXE" );
        AddLib( TRUE, &im );
    #endif

} /* addKERNEL */

/*
 * addAllWOWModules - add all modules as libraries.  This is invoked if
 *                    WOW was already running, since we will get no
 *                    lib load notifications if it was.
 */
static void addAllWOWModules( void )
{
    MODULEENTRY         me;
    thread_info         *ti;
    IMAGE_NOTE          im;

    ti = FindThread( DebugeeTid );
    me.dwSize = sizeof( MODULEENTRY );
    if( pVDMModuleFirst( ProcessInfo.process_handle, ti->thread_handle,
                    &me, NULL, 0 ) ) {
        do {
            if( !strcmp( me.szModule, WOWAppInfo.modname ) ) {
                continue;
            }
            memcpy( &im.Module, &me.szModule, sizeof( me.szModule ) );
            memcpy( &im.FileName, &me.szExePath, sizeof( me.szExePath ) );
            AddLib( TRUE, &im );
            me.dwSize = sizeof( MODULEENTRY );
        } while( pVDMModuleNext( ProcessInfo.process_handle, ti->thread_handle,
                        &me, NULL, 0 ) );
    }

} /* addAllWOWModules */

/*
 * executeUntilVDMStart - go until we hit our first VDM exception
 */
static BOOL executeUntilVDMStart( void )
{
    int         rc;

    for( ;; ) {
        rc = DebugExecute( STATE_WAIT_FOR_VDM_START, NULL, FALSE );
        if( rc == COND_VDM_START ) {
            return( TRUE );
        }
        return( FALSE );
    }

} /* executeUntilVDMStart */

/*
 * EnumWOWProcessFunc - callback for each WOW process in the system
 */
BOOL WINAPI EnumWOWProcessFunc( DWORD pid, DWORD attrib, LPARAM lparam )
{
    if( attrib & WOW_SYSTEM ) {
        *(DWORD *) lparam = pid;
        return( FALSE );
    }
    return( TRUE );

} /* EnumWOWProcessFunc */
#else
BOOL WINAPI EnumWOWProcessFunc( DWORD pid, DWORD attrib, LPARAM lparam )
{
    *(DWORD *) lparam = 0;
    return( FALSE );
}
#endif

/*
 * AccLoadProg - create a new process for debugging
 */
unsigned ReqProg_load( void )
{
    char                *parm,*src,*dst, *endsrc;
    char                exe_name[PATH_MAX];
    char                ch;
    BOOL                rc;
    int                 len;
    CONTEXT             con;
    thread_info         *ti;
    HANDLE              handle;
    prog_load_req       *acc;
    prog_load_ret       *ret;
    header_info         hi;
    WORD                stack;
    WORD                version;
    DWORD               pid,pid_started;
    DWORD               cr_flags;
    char                * buff = NULL;
    size_t              nBuffRequired = 0;
    char                *dll_name, *service_name, *dll_destination, *service_parm;

    acc = GetInPtr( 0 );
    ret = GetOutPtr( 0 );
    parm = GetInPtr( sizeof( *acc ) );

    /*
     * reset status variables
     */
    LastExceptionCode = -1;
    DebugString = NULL;
    DebugeeEnded = FALSE;
    RemoveAllThreads();
    FreeLibList();
    DidWaitForDebugEvent = FALSE;
    DebugeePid = NULL;
    DebugeeTid = NULL;

    /*
     * check if pid is specified
     */
    ParseServiceStuff( parm, &dll_name, &service_name, &dll_destination, &service_parm );
    pid = 0;
    src = parm;
    
    /*
    //  Just to be really safe!
    */
    nBuffRequired = GetTotalSize() + PATH_MAX + 16;
    if(NULL == (buff = malloc(nBuffRequired))){
        ret->err = ERROR_NOT_ENOUGH_MEMORY;
        return (sizeof(*ret));
    }

    if( *src == '#' ) {
        src++;
        pid = strtoul( src, &endsrc, 16 );
        if( pid == 0 ) pid = -1;
        strcpy( buff, endsrc );
    } else {
        while( *src ) {
            if( !isdigit( *src ) ) {
                break;
            }
            src++;
        }
        if( *src == 0 && src != parm ) {
            pid = atoi( parm );
        }
    }

    /*
     * get program to debug.  If the user has specified a pid, then
     * skip directly to doing a DebugActiveProcess
     */
    IsWOW = FALSE;
    IsDOS = FALSE;
    if( pid == 0 ) {
        if( FindFilePath( parm, exe_name, ExtensionList ) != 0 ) {
            ret->err = ERROR_FILE_NOT_FOUND;
            if(buff){
                free(buff);
                buff = NULL;
            }
            return( sizeof( *ret ) );
        }

        /*
         * Get type of application
         */
        handle = CreateFile( (LPTSTR) exe_name, GENERIC_READ, FILE_SHARE_READ,
                            NULL, OPEN_EXISTING, 0, 0 );
        if( handle == (HANDLE)-1 ) {
            ret->err = GetLastError();
            if(buff){
                free(buff);
                buff = NULL;
            }
            return( sizeof( *ret ) );
        }
        GetFullPathName( exe_name, MAX_PATH, CurrEXEName, NULL );

        /*
         * get the parm list
         */
        if( strchr( CurrEXEName, ' ' ) != NULL ) {
            strcpy( buff, "\"" );
            strcat( buff, CurrEXEName );
            strcat( buff, "\"" );
        } else {
            strcpy( buff, CurrEXEName );
        }
        dst = &buff[strlen(buff)];
        src = parm;
        while( *src != 0 ) {
            ++src;
        }
        len = &parm[ GetTotalSize() - sizeof( *acc ) ] - src;
        for( ;; ) {
            if( len == 0 ) break;
            ch = *src;
            if( ch == 0 ) {
                ch = ' ';
            }
            *dst = ch;
            ++dst;
            ++src;
            --len;
        }
        *dst = 0;

        cr_flags = DEBUG_ONLY_THIS_PROCESS;

        if( !GetEXEHeader( handle, &hi, &stack ) ) {
            ret->err = GetLastError();
            if(buff){
                free(buff);
                buff = NULL;
            }
            return( sizeof( *ret ) );
        }
        if( hi.sig == EXE_PE ) {
            DebugeeSubsystem = hi.peh.subsystem;
            if( DebugeeSubsystem == SS_WINDOWS_CHAR ) {
                cr_flags |= CREATE_NEW_CONSOLE;
            }
        } else if( hi.sig == EXE_NE ) {
            IsWOW = TRUE;
            /*
             * find out the pid of WOW, if it is already running.
             */
            pVDMEnumProcessWOW( EnumWOWProcessFunc, (LPARAM) &pid );
            if( pid != 0 ) {
                version = LOWORD( GetVersion() );
                if( LOBYTE( version ) == 3 && HIBYTE( version ) < 50 ) {
                    int kill = MessageBox( NULL, TRP_NT_wow_warning,
                            TRP_The_WATCOM_Debugger,
                        MB_APPLMODAL+MB_YESNO );
                    if( kill == IDYES ) {
                        HANDLE hprocess = OpenProcess( PROCESS_TERMINATE+STANDARD_RIGHTS_REQUIRED, FALSE, pid );
                        if( hprocess != 0 && TerminateProcess( hprocess, 0 ) ) {
                            CloseHandle( hprocess );
                            pid = 0;
                        }
                    }
                } else {
                    cr_flags |= CREATE_SEPARATE_WOW_VDM;
                    pid = 0; // always start a new VDM.
                }
            }
            if( pid != 0 ) {
                ret->err = GetLastError();
                if(buff){
                    free(buff);
                    buff = NULL;
                }
                return( sizeof( *ret ) );
            }
        } else {
            IsDOS = TRUE;
        }
        CloseHandle( handle );
    }

    /*
     * start the debugee
     */
    pid_started = pid;
    if( *dll_name ) {
        strcat( buff, LOAD_PROG_STR_DELIM );
        strcat( buff, LOAD_PROG_STR_DLLNAME );
        strcat( buff, dll_name );
    }
    if( *service_name ) {
        strcat( buff, LOAD_PROG_STR_DELIM );
        strcat( buff, LOAD_PROG_STR_SERVICE );
        strcat( buff, service_name );
    }
    if( *dll_destination ) {
        strcat( buff, LOAD_PROG_STR_DELIM );
        strcat( buff, LOAD_PROG_STR_COPYDIR );
        strcat( buff, dll_destination );
    }
    if( *service_parm ) {
        strcat( buff, LOAD_PROG_STR_DELIM );
        strcat( buff, LOAD_PROG_STR_SERVICEPARM );
        strcat( buff, service_parm );
    }
    ret->err = StartControlThread( buff, &pid_started, cr_flags );
    if( ret->err != 0 ) {
        if(buff){
            free(buff);
            buff = NULL;
        }
        return( sizeof( *ret ) );
    }
    /*
     * CREATE_PROCESS_DEBUG_EVENT will always be the first debug event.
     * If it is not, then something is horribly wrong.
     */
    rc = MyWaitForDebugEvent();
    if( !rc || (DebugEvent.dwDebugEventCode != CREATE_PROCESS_DEBUG_EVENT) ||
                (DebugEvent.dwProcessId != pid_started ) ) {
        ret->err = GetLastError();
        if(buff){
            free(buff);
            buff = NULL;
        }
        return( sizeof( *ret ) );
    }
    ProcessInfo.pid = DebugEvent.dwProcessId;
    ProcessInfo.process_handle = DebugEvent.u.CreateProcessInfo.hProcess;
    ProcessInfo.base_addr = DebugEvent.u.CreateProcessInfo.lpBaseOfImage;
    AddProcess( &hi );
    AddThread( DebugEvent.dwThreadId, DebugEvent.u.CreateProcessInfo.hThread,
                        DebugEvent.u.CreateProcessInfo.lpStartAddress );
    DebugeePid = DebugEvent.dwProcessId;
    DebugeeTid = DebugEvent.dwThreadId;
    LastDebugEventTid = DebugEvent.dwThreadId;

#ifdef WOW
    if( IsWOW ) {

        ret->flags = LD_FLAG_IS_PROT;
        ret->err = 0;
        ret->task_id = DebugeePid;
        /*
         * we use our own CS and DS as the Flat CS and DS, for lack
         * of anything better
         */
        FlatDS = DS();
        FlatCS = CS();
        if( !executeUntilVDMStart() ) {
            ret->err = GetLastError();
            if(buff){
                free(buff);
                buff = NULL;
            }
            return( sizeof( *ret ) );
        }
        if( pid != NULL ) {
            addAllWOWModules();
        } else {
            addKERNEL();
        }
        /*
         * we save the starting CS:IP of the WOW app, since we will use
         * it to force execution of code later
         */
        ti = FindThread( DebugeeTid );
        MyGetThreadContext( ti, &con );
        WOWAppInfo.segment = (WORD) con.SegCs;
        WOWAppInfo.offset = (WORD) con.Eip;
        con.SegSs = con.SegDs; // Wow lies to us about the stack segment.  Reset it
        con.Esp = stack;
        MySetThreadContext( ti, &con );
    } else
#endif
           {
        DWORD base;

        if( pid == 0 ) {
            base = (DWORD)DebugEvent.u.CreateProcessInfo.lpStartAddress;
        } else {
            base = 0;
        }

        ret->flags = 0;
        ret->err = 0;
        ret->task_id = DebugeePid;
        if( executeUntilStart( pid != 0 ) ) {
            DWORD old;
            /*
             * make the application load our DLL, so that we can have it
             * run code out of it.  One small note: this will not work right
             * if the app does not load our DLL at the same address the
             * debugger loaded it at!!!
             */

            ti = FindThread( DebugeeTid );
            MyGetThreadContext( ti, &con );
            old = AdjustIP( &con, 0 );
            if( base != 0 ) SetIP( &con, base );
            MySetThreadContext( ti, &con );
            SetIP( &con, old );
            MySetThreadContext( ti, &con );
        }
        ti = FindThread( DebugeeTid );
        MyGetThreadContext( ti, &con );
#if defined(MD_x86)
        FlatCS = con.SegCs;
        FlatDS = con.SegDs;
#endif
    }
    ret->flags |= LD_FLAG_HAVE_RUNTIME_DLLS;
    if( pid != 0 ) {
        ret->flags |= LD_FLAG_IS_STARTED;
    }
    ret->mod_handle = 0;

    if(buff){
        free(buff);
        buff = NULL;
    }
    return( sizeof( *ret ) );

}

unsigned ReqProg_kill( void )
{
    prog_kill_ret       *ret;

    ret = GetOutPtr( 0 );
    ret->err = 0;
    DelProcess( TRUE );
    StopControlThread();
    return( sizeof( *ret ) );
}
