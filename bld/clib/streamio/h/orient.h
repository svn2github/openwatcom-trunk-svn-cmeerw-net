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
* Description:  Macro to deal with stream orientation.
*
****************************************************************************/


#ifdef __NETWARE__
    /* One less thing to worry about */
#else
  #ifdef __WIDECHAR__
    #define ORIENT_STREAM(stream,error_return)                  \
        if( _FP_ORIENTATION(stream) != _WIDE_ORIENTED ) {       \
            if( _FP_ORIENTATION(stream) == _NOT_ORIENTED ) {    \
                _FP_ORIENTATION(stream) = _WIDE_ORIENTED;       \
            } else {                                            \
                _ReleaseFile( stream );                         \
                return( error_return );                         \
            }                                                   \
        }
  #else
    #define ORIENT_STREAM(stream,error_return)                  \
        if( _FP_ORIENTATION(stream) != _BYTE_ORIENTED ) {       \
            if( _FP_ORIENTATION(stream) == _NOT_ORIENTED ) {    \
                _FP_ORIENTATION(stream) = _BYTE_ORIENTED;       \
            } else {                                            \
                _ReleaseFile( stream );                         \
                return( error_return );                         \
            }                                                   \
    }
  #endif
#endif
