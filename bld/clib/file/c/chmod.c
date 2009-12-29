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
* Description:  DOS implementation of chmod().
*
****************************************************************************/


#include "variety.h"
#include "widechar.h"
#include <unistd.h>
#include <dos.h>
#ifdef __WIDECHAR__
    #include "mbwcconv.h"
#endif
#include "seterrno.h"
#include "tinyio.h"
#include "rtdata.h"
#include "_doslfn.h"

#ifdef _M_I86
  #ifdef __BIG_DATA__
    #define AUX_INFO    \
        parm caller     [dx ax] [bx] \
        modify exact    [ax cx dx];
  #else
    #define AUX_INFO    \
        parm caller     [dx] [bx] \
        modify exact    [ax cx];
  #endif
#else
    #define AUX_INFO    \
        parm caller     [edx] [ebx] \
        modify exact    [eax ecx];
#endif

extern unsigned __doserror_( unsigned );
#pragma aux __doserror_ "*"

extern unsigned __chmod_sfn( const char *path, unsigned pmode );
#pragma aux __chmod_sfn = \
        _SET_DSDX       \
        _MOV_AX_W _GET_ DOS_CHMOD \
        _INT_21         \
        "jc short L2"   \
        "and  cl,0feh"  \
        "test bl,80h"   \
        "jne short L1"  \
        "or   cl,01h"   \
"L1:"                   \
        _MOV_AX_W _SET_ DOS_CHMOD \
        _INT_21         \
"L2:"                   \
        _RST_DS         \
        "call __doserror_" \
        AUX_INFO

extern unsigned __chmod_lfn( const char *path, unsigned pmode );
#pragma aux __chmod_lfn = \
        _SET_DSDX       \
        LFN_GET_FILE_ATTR \
        "jc short L2"   \
        "and  cl,0feh"  \
        "test bl,80h"   \
        "jne short L1"  \
        "or   cl,01h"   \
"L1:"                   \
        LFN_SET_FILE_ATTR \
"L2:"                   \
        _RST_DS         \
        "call __doserror_" \
        AUX_INFO

_WCRTLINK int __F_NAME(chmod,_wchmod)( const CHAR_TYPE *pathname, int pmode )
{
#ifdef __WIDECHAR__
    char            mbPath[MB_CUR_MAX*_MAX_PATH];

    __filename_from_wide( mbPath, pathname );
#elif defined( __WATCOM_LFN__ )
    unsigned        rc = 0;
#endif
#if defined( __WIDECHAR__ ) && defined( __WATCOM_LFN__ )
    return( chmod( mbPath, pmode ) );
#else
  #if defined( __WATCOM_LFN__ )
    if( _RWD_uselfn && (rc = __chmod_lfn( __F_NAME(pathname,mbPath), pmode )) == 0 ) {
        return( rc );
    }
    if( IS_LFN_ERROR( rc ) ) {
        return( rc );
    }
  #endif
    return( __chmod_sfn( __F_NAME(pathname,mbPath), pmode ) );
#endif
}

