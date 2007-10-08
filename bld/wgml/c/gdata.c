/****************************************************************************
*
*                            Open Watcom Project
*
*  Copyright (c) 2004-2007 The Open Watcom Contributors. All Rights Reserved.
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
* Description:  define and initialize global variables for wgml
*
****************************************************************************/
#include    "wgml.h"

#define global                          // allocate storage for global vars
#include    "gvars.h"
#undef  global

#include    "swchar.h"

/***************************************************************************/
/*  Read an environment variable and return content in allocated buffer    */
/***************************************************************************/

char *GML_get_env( char *name )
{
    errno_t     rc;
    size_t      len;
    char       *value;
    size_t      maxsize;

    maxsize = 128;
    value = mem_alloc( maxsize );
    rc = getenv_s( &len, value, maxsize, name );
    if( rc ) {
        mem_free( value );
        value = NULL;
        if( len ) {   /*  we need more space */
            maxsize = len + 1;
            value = mem_alloc( maxsize );
            rc = getenv_s( &len, value, maxsize, name );
        }
    }
    if( len == 0 ) {
        if( value != NULL ) {
            mem_free( value );
        }
        value = NULL;
    }
    return( value );
}


/***************************************************************************/
/*  get the wgml environment variables                                     */
/***************************************************************************/

void get_env_vars( void )
{
    Pathes  = GML_get_env( "PATH" );
    GMLlibs = GML_get_env( GMLLIB );
    GMLincs = GML_get_env( GMLINC );
}

/***************************************************************************/
/*  Init some global variables                                             */
/***************************************************************************/

void init_global_vars( void )
{

    memset( &GlobalFlags, 0, sizeof( GlobalFlags ) );
    GlobalFlags.wscript = TRUE;         // we want (w)script support
    GlobalFlags.warning = TRUE;         // display warnings default

    memset( &ProcFlags, 0, sizeof( ProcFlags ) );

    try_file_name       = NULL;
    Pathes              = NULL;         // content of PATH environment var
    GMLlibs             = NULL;         // content of GMLLIB environment var
    GMLincs             = NULL;         // content of GMLINC environment var

    master_fname        = NULL;         // Master input file name
    master_fname_attr   = NULL;         // Master input file name attributes
    line_from           = 1;            // default first line to process
    line_to             = ULONG_MAX -1; // default last line to process

    file_cbs            = NULL;         // list of open GML files
    out_file            = NULL;         // output file name
    inc_level           = 0;            // include nesting level
    switch_char         = _dos_switch_char();
    alt_ext             = mem_alloc( 5 );   // alternate extension   .xxx
    *alt_ext            = '\0';

    def_ext             = mem_alloc( sizeof( GML_EXT ) );
    strcpy_s( def_ext, sizeof( GML_EXT ), GML_EXT );

    err_count           = 0;            // total error count
    wng_count           = 0;            // total warnig count

    GML_char            = GML_CHAR_DEFAULT; // GML start char
    SCR_char            = SCR_CHAR_DEFAULT; // Script start char
    CW_sep_char         = CW_SEP_CHAR_DEFAULT;  // control word seperator

    CPI                 = 10;           // chars per inch
    CPI_units           = SU_chars;

    LPI                 = 6;            // lines per inch
    LPI_units           = SU_lines;

    memset( &bind_odd, 0, sizeof( bind_odd ) ); // bind value odd pages
    bind_odd.su_u        = SU_chars_lines;

    memset( &bind_even, 0, sizeof( bind_even ) );   // bind value Even pages
    bind_even.su_u       = SU_chars_lines;

    passes              = 1;            // default number of passes

    buf_size            = BUF_SIZE;
    buffer              = NULL;
    buff2               = NULL;
//    buffer              = mem_alloc( buf_size );
//    buff2               = mem_alloc( buf_size );
}

