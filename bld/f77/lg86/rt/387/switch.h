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


#define _DEVELOPMENT    _OFF    // are we developing?
#define _MEMTRACK       _OFF    // memory tracking during development?
#define _8087           _ON     // 8087 present?
#define _EDITOR         _OFF    // editor interface available?
#define _OBJECT         _OFF    // generate object files?
#define _EXE            _OFF    // generate EXE files?
#define _CSET           _ASCII  // character set?
#define _TARGET         _80386  // target machine?
#define _OPSYS          _PCDOS  // operating system?
#define _OPT_CG         _OFF    // optimizing code generator used?
#define _SA_LIBRARY             // generate stand alone library
#define _SysMemAlloc    malloc  // define system memory manager
#define _SysMemFree     free
#pragma aux _SysMemInit =
#pragma aux _SysMemFini =

#include "samaps.h"
