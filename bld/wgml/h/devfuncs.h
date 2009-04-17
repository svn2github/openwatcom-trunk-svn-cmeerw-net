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
* Description:  Defines and declares items used to interpret function blocks:
*               this variable:
*                   pages
*               and these functions:
*                   df_interpret_device_functions()
*                   df_interpret_driver_functions()
*                   df_populate_device_table()
*                   df_populate_driver_table()
*                   df_set_horizontal()
*                   df_set_vertical()
*                   df_setup()
*                   df_teardown()
*                   fb_enterfont()
*                   fb_firstword()
*                   fb_init()
*                   fb_lineproc_endvalue()
*                   fb_normal_vertical_positioning()
*                   fb_thickness()
*
****************************************************************************/

#ifndef DEVFUNCS_H_INCLUDED
#define DEVFUNCS_H_INCLUDED

#include <stdint.h>
#include <stdio.h>

/* The variables in copfiles.h must be included as extern */

#define suppress

#include "copfiles.h"

#undef suppress

/* Variable declarations. */

#ifndef global
    #define global  extern
#endif

global uint32_t         pages;          // current page number

/* Reset so can be reused with other headers. */

#undef global

/* Function declarations. */

#ifdef  __cplusplus
extern "C" {    /* Use "C" linkage when in C++ mode. */
#endif
extern void df_interpret_device_functions( uint8_t * in_function );
extern void df_interpret_driver_functions( uint8_t * in_function );
extern void df_populate_device_table( void );
extern void df_populate_driver_table( void );
extern void df_set_horizontal( uint32_t h_start );
extern void df_set_vertical( uint32_t v_start );
extern void df_setup( void );
extern void df_teardown( void );
extern void fb_enterfont( void );
extern void fb_firstword( line_proc * in_block );
extern void fb_init( init_block * in_block );
extern void fb_lineproc_endvalue( void );
extern void fb_normal_vertical_positioning( void );
extern void fb_thickness( uint8_t * in_function, uint32_t h_start, uint32_t v_start, uint32_t h_len, uint32_t v_len, uint32_t thickness, char * name );

#ifdef  __cplusplus
}   /* End of "C" linkage for C++. */
#endif

#endif  /* DEVFUNCS_H_INCLUDED */
