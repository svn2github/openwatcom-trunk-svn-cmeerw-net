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
* Description:  OS/2 32-bit DLL startup code.
*
****************************************************************************/


#include "variety.h"
#include "liballoc.h"
#include <stdio.h>
#include <io.h>
#include <i86.h>
#include <stdlib.h>
#include <string.h>
#define INCL_DOSMISC
#define INCL_DOSSEMAPHORES
#define INCL_DOSPROCESS
#define INCL_DOSEXCEPTIONS
#define INCL_DOSMODULEMGR
#include <wos2.h>
#include "initfini.h"
#include "osthread.h"

extern  unsigned            __hmodule;

extern  unsigned APIENTRY   LibMain( unsigned, unsigned );
extern  void                __CommonInit(void);
extern  int                 __disallow_single_dgroup(unsigned);

#ifdef __SW_BR
    int                     __Is_DLL;           /* TRUE => DLL, else not a DLL */
    char                    *_LpDllName;        /* pointer to dll name */
    wchar_t                 *_LpwDllName;       /* pointer to dll name */
#else
    extern      char        *_LpDllName;        /* pointer to dll name */
    extern      wchar_t     *_LpwDllName;       /* pointer to dll name */
    extern      char        *_Envptr;
    _WCRTLINK extern char       *_LpCmdLine;    /* pointer to command line */
    _WCRTLINK extern wchar_t    *_LpwCmdLine;   /* pointer to command line */
    _WCRTLINK extern char       *_LpPgmName;    /* pointer to program name */
    _WCRTLINK extern wchar_t    *_LpwPgmName;   /* pointer to program name */
    extern      unsigned    __MaxThreads;
    extern      unsigned    __ASTACKSIZ;        /* alternate stack size */
    extern      char        *__ASTACKPTR;       /* alternate stack pointer */
    extern      char        *_STACKLOW;

    extern      void        __OS2Init(int, void *);
    extern      void        __OS2Fini(void);
    extern      int         __OS2AddThread(int, void *);
    extern      void        __shutdown_stack_checking();
    extern      void        *__InitThreadProcessing(void);
    extern      void        __InitMultipleThread(void);
    extern      thread_data *__AllocInitThreadData(thread_data *tdata);
    extern      void        __FreeInitThreadData(thread_data *);
    extern      thread_data *__FirstThreadData;

    #ifdef __386__
        #pragma aux     __ASTACKPTR "*";
        #pragma aux     __ASTACKSIZ "*";
    #endif
#endif

static void APIENTRY LibMainExitList( ULONG reason ) {
    APIRET rc;
    reason = reason;
    #ifndef __SW_BR
        if( _LpwCmdLine ) {
            lib_free( _LpwCmdLine );
            _LpwCmdLine = NULL;
        }
        if( _LpwPgmName ) {
            lib_free( _LpwPgmName );
            _LpwPgmName = NULL;
        }
    #endif
    #ifdef __SW_BR
        __FiniRtns( 0, 255 );
    #else
        __FiniRtns( FINI_PRIORITY_EXIT, 255 );
        // calls to free memory have to be done before semaphores closed
        __FreeInitThreadData( __FirstThreadData );
        __OS2Fini(); // must be done before following finalizers get called
        __FiniRtns( 0, FINI_PRIORITY_EXIT-1 );
    #endif
    #ifndef __SW_BR
        __shutdown_stack_checking();
    #endif
    rc = DosExitList( EXLST_EXIT, LibMainExitList );
}


unsigned __LibMain( unsigned hmod, unsigned termination )
/*******************************************************/
{
    static int  processes;
    unsigned    rc;

    if( termination != 0 ) {
        rc = LibMain( hmod, termination );
        --processes;
        return( rc );
    }
    ++processes;
    if( processes > 1 ) {
        if( __disallow_single_dgroup(hmod) ) {
            return( 0 );
        }
    }
    __hmodule = hmod;
    #ifdef __SW_BR
    {
        static char fname[_MAX_PATH];
        static wchar_t wfname[_MAX_PATH];
        __Is_DLL = 1;
        __InitRtns( 255 );
        DosQueryModuleName( hmod, sizeof( fname ), fname );
        _LpDllName = fname;
        _LpwDllName = wfname;
        _atouni( _LpwDllName, _LpDllName );
    }
    #else
    {
        PTIB        pptib;
        PPIB        pppib;
        unsigned    i;
		
        DosGetInfoBlocks( &pptib, &pppib );
        _Envptr = pppib->pib_pchenv;
        _LpCmdLine = pppib->pib_pchcmd;
        while( *_LpCmdLine ) {          // skip over program name
            _LpCmdLine++;
        }
        _LpCmdLine++;
        _LpwCmdLine = lib_malloc( (strlen( _LpCmdLine ) + 1) * sizeof( wchar_t ) );
        _atouni( _LpwCmdLine, _LpCmdLine );
        {
            // ugly stuff to deal with two copies of .exe name in the
            // environment space. apparently the OS fullpath name is
            // just before this one in the environment space
            char    *cmd_path;
            cmd_path = pppib->pib_pchcmd;
            for( cmd_path -= 2; *cmd_path != '\0'; --cmd_path );
            ++cmd_path;
            _LpPgmName = cmd_path;
            _LpwPgmName = lib_malloc( (strlen( _LpPgmName ) + 1) * sizeof( wchar_t ) );
            _atouni( _LpwPgmName, _LpPgmName );
        }
        __InitRtns( 1 );
        if( __InitThreadProcessing() == NULL ) return( 0 );
        __OS2Init( TRUE, __AllocInitThreadData( NULL ) );
        for( i = 2; i <= __MaxThreads; i++ ) {
            if( !__OS2AddThread( i, NULL ) ) return( 0 );
        }
        __InitRtns( 15 );
        __InitMultipleThread();
        {
            static char fname[_MAX_PATH];
            static wchar_t wfname[_MAX_PATH];
            DosQueryModuleName( hmod, sizeof( fname ), fname );
            _LpDllName = fname;
            _LpwDllName = wfname;
            _atouni( _LpwDllName, _LpDllName );
        }
        __InitRtns( 255 );
    }
    #endif
    // setup shutdown routine
    #define SHUTDOWN_ORDER 0x0000E000
    if( DosExitList( EXLST_ADD|SHUTDOWN_ORDER, LibMainExitList ) ) {
        __FiniRtns( 0, 255 );
        return( 0 );
    }
    __CommonInit();
    #ifndef __SW_BR
        /* allocate alternate stack for F77 */
        __ASTACKPTR = _STACKLOW + __ASTACKSIZ;
    #endif
    return( LibMain( hmod, termination ) );
}
#ifdef __386__
    #pragma aux __LibMain "*" parm caller []
#endif
