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
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#if defined(__WATCOMC__)
    #include <sys/ioctl.h>
#elif !defined( HP )
    #include <termio.h>
#else
    #include <stdarg.h>
#endif

#if defined(__SUNPRO_CC) || defined(__SUNPRO_C)
    // curses.h when using SUNPRO compiler has its own typedef for bool
    #define bool        curses_bool
    #include <curses.h>
    #include <term.h>
    #undef bool
#else
    #include <curses.h>
#include <term.h>
#endif

#ifndef __WATCOMC__
#include <alloca.h>
#endif
#include <unistd.h>

#include "stdui.h"
#include "qnxuiext.h"
#include "tixparse.h"
#include "trie.h"
#ifndef __WATCOMC__
#include "clibext.h"
#endif
#if 0
#include "stdtypes.h"
#include "unixmisc.h"

#include "wlangfmt.h"
#endif

extern char ui_tix_path[];
extern int ui_tix_missing( const char *name );

typedef enum {
    TT_CODE,
    TT_STRING,
    TT_EOF
} tix_token;

static FILE     *in_file= NULL;

char            ti_char_map[256][4];
unsigned char   _ti_alt_map[32];

void tix_error( char *str )
/*************************/
{
    static char header[]= "\nError in ";
    extern char *UITermType;

#define uiwrite( s ) write( UIConHandle, s, strlen( s ) );

    write( UIConHandle, header, sizeof( header )-1 );
    uiwrite( UITermType );
    uiwrite( ": " );
    uiwrite( str );
    uiwrite( "\n" );
}

FILE *ti_fopen( char *fnam )
/**************************/
{
    FILE        *res;
    char        *homeDir;
    char        fpath[_MAX_PATH+1];

    if( fnam==NULL || fnam[0]=='\0' ) {
        return( NULL );
    }

    // first look in current directory
    res = fopen( fnam, "r" );
    if( res != NULL ) {
        return( res );
    }

    // if it's not there, look in the user's home directory
    homeDir = getenv( "HOME" );
    if( homeDir != NULL && homeDir[0] != '\0' ) {
        strcpy( fpath, homeDir );
        strcat( fpath, "/" );
        strcat( fpath, fnam );

        res = fopen( fpath, "r" );
        if( res != NULL ) {
            return( res );
        }
    }
#if 0
    if( LS_QualifySqlAnyFile( fnam, fpath, sizeof(fpath) ) ) {
        res = fopen( fpath, "r" );
        if( res != NULL ) {
            return( res );
        }
    }
#endif

    // finally, look in /usr/watcom/tix/<name>
//    strcpy( fpath, TIX_PATH_NAME );
    strcpy( fpath, ui_tix_path );
    strcat( fpath, fnam );
    res = fopen( fpath, "r" );
    return( res );
}

int init_tix_scanner( char *name )
/********************************/
{
    char        tix_name[19];


    if( name ) {
        if( *name ) {
            strcpy( tix_name, name );
            strcat( tix_name, ".tix" );
            in_file = ti_fopen( tix_name );
            if( in_file != NULL ) return( 1 );
        }
        if( strstr( name, "qnx" ) != 0 ) {
            in_file = ti_fopen( "qnx.tix" );
            if( in_file != NULL ) return( 1 );
        } else if( strstr( name, "ansi" ) != 0 ) {
            in_file = ti_fopen( "ansi.tix" );
            if( in_file != NULL ) return( 1 );
        } else if( strstr( name, "xterm" ) != 0 ) {
            in_file = ti_fopen( "xterm.tix" );
            if( in_file != NULL ) return( 1 );
            in_file = ti_fopen( "ansi.tix" );
            if( in_file != NULL ) return( 1 );
        }
    }
    in_file = ti_fopen( "default.tix" );
    return( in_file != NULL );
}

void close_tix_scanner( void )
/****************************/
{
    if( in_file!=NULL ){
        fclose( in_file );
    }
}

static tix_token get_tix_token( char *buff )
/******************************************/
{
    int         c;
    char        *p;
    char        *end;
    unsigned    num;
    char        endc;

    for( ;; ) {
        c = getc( in_file );
        if( c == EOF ) return( TT_EOF );
        if( c == '#' ) {
            /* eat a comment */
            for( ;; ) {
                c = getc( in_file );
                if( c == EOF ) return( TT_EOF );
                if( c == '\n' ) break;
            }
        }
        if( !isspace( c ) ) break;
    }
    p = buff;
    if( c == '\'' || c == '\"' ) {
        /* collect a string */
        endc = c;
        for( ;; ) {
            c = getc( in_file );
            if( c == EOF ) break;
            if( c == '\r' ) break;
            if( c == '\n' ) break;
            if( c == endc ) break;
            if( c == '\\' ) {
                c = getc( in_file );
                if( c == EOF ) break;
                switch( c ) {
                case 'a':
                    c = '\a';
                    break;
                case 'b':
                    c = '\b';
                    break;
                case 'e':
                    c = '\x1b';
                    break;
                case 'f':
                    c = '\f';
                    break;
                case 'n':
                    c = '\n';
                    break;
                case 'r':
                    c = '\r';
                    break;
                case 't':
                    c = '\t';
                    break;
                case 'v':
                    c = '\b';
                    break;
                case 'x':
                    num = 0;
                    for( ;; ) {
                        c = getc( in_file );
                        if( c == EOF ) break;
                        if( isdigit( c ) ) {
                            c = c - '0';
                        } else if( c >= 'A' && c <= 'F' ) {
                            c = c - 'A' + 10;
                        } else if( c >= 'a' && c <= 'f' ) {
                            c = c - 'a' + 10;
                        } else {
                            ungetc( c, in_file );
                        }
                        num = (num << 8) + c;
                    }
                    c = num;
                    break;
                }
            }
            *p++ = c;
        }
        *p = '\0';
        return( TT_CODE );
    } else {
        /* collect a string or number */
        for( ;; ) {
            *p++ = c;
            c = getc( in_file );
            if( c == EOF ) break;
            if( isspace( c ) ) break;
            if( c == '#' ) {
                ungetc( c, in_file );
                break;
            }
        }
        *p = '\0';
        num = strtoul( buff, &end, 0 );
        if( end != p ) return( TT_STRING );
        buff[0] = num & 0xff;
        buff[1] = num >> 8;
        return( TT_CODE );
    }
}

static unsigned get_tix_code( unsigned char *buff )
/*************************************************/
{
    if( get_tix_token( (char *)buff ) != TT_CODE ) {
        tix_error( "expecting code" );
        return( ~0U );
    }
    return( buff[0] + (buff[1] << 8) );
}

static const char acs_default[] =
        "q-x|l.m`k.j\'n+w-v-t|u|~*+>,<-^.vO#f`g?a#h#";

static char find_acs_map( char c, const char *acs )
/*************************************************/
{
    if( acs ) {
        for( ;; ) {
            if( acs[0] == '\0' ) break;
            if( acs[0] == c ) return( acs[1] );
            ++acs;
            if( acs[0] == '\0' ) break;
            ++acs;
        }
    }
    return( '\0' );
}

static int do_parse( void )
/*************************/
{
    char        buff[80];
    char        input[80];
    tix_token   tok;
    unsigned    code;
    char        c;

    tok = get_tix_token( buff );
    for( ;; ) {
        if( tok == TT_EOF ) break;
        if( tok != TT_STRING ) {
            tix_error( "expecting directive" );
            return( 0 );
        }
        if( stricmp( buff, "display" ) == 0 ) {
            code = get_tix_code( (unsigned char *)buff );
            if( code == ~0U ) return( 0 );
            tok = get_tix_token( buff );
            if( tok == TT_EOF ) break;
            if( tok == TT_STRING ) {
                if( stricmp( buff, "alt" ) != 0 ) {
                    tix_error( "expecting alt" );
                    return( 0 );
                }
                tok = get_tix_token( buff );
                if( tok == TT_EOF ) break;
                c = find_acs_map( buff[0], acs_chars );
                if( c != '\0' ) {
                    ti_alt_map_set( code );
                } else {
                    c = find_acs_map( buff[0], acs_default );
                    if( c == '\0' ) {
                        c = buff[0];
                        ti_alt_map_set( code );
                    }
                }
                buff[0] = c;
            }
            if( tok != TT_CODE ) {
                tix_error( "expecting display code" );
                return( 0 );
            }
            ti_char_map[code][0] = buff[0];
            tok = get_tix_token( buff );
        } else if( stricmp( buff, "key" ) == 0 ) {
            code = get_tix_code( (unsigned char *)buff );
            if( code == ~0U ) return( 0 );
            input[0] = '\0';
            for( ;; ) {
                tok = get_tix_token( buff );
                if( tok != TT_CODE ) break;
                strcat( input, buff );
            }
            TrieAdd( code, input );
        } else {
            tix_error( "unknown directive" );
        }
    }
    return( 1 );
}

struct charmap {
    unsigned char cp437;
    unsigned char vt100;
    unsigned short unicode;
};

static struct charmap default_tix[] = {
    /* arrows */
    0x10, '+', 0x25ba, 0x11, ',', 0x25c4, 0x1e, '-', 0x25b2, 0x1f, '.', 0x25bc,
    0x1a, '+', 0x2192, 0x1b, ',', 0x2190, 0x18, '-', 0x2191, 0x19, '.', 0x2193,
    /* squares */
    0xb0, 'a', 0x2591, 0xb1, 'a', 0x2592, 0xb2, 'O', 0x2593, 0xdb, 'O', 0x2588,
    0xdc, ' ' | 0x80, 0x2584, 0xdd, 'O', 0x258c, 0xde, ' ' | 0x80, 0x2590,
    0xdf, 'O', 0x2580,
    /* line drawing */
    0xb3, 'x', 0x2502, 0xb4, 'u', 0x2524, 0xb5, 'u', 0x2561, 0xb6, 'u', 0x2562,
    0xb7, 'k', 0x2556, 0xb8, 'k', 0x2555, 0xb9, 'u', 0x2563, 0xba, 'x', 0x2551,
    0xbb, 'k', 0x2557, 0xbc, 'j', 0x255d, 0xbd, 'j', 0x255c, 0xbe, 'j', 0x255b,
    0xbf, 'k', 0x2510, 0xc0, 'm', 0x2514, 0xc1, 'v', 0x2534, 0xc2, 'w', 0x252c,
    0xc3, 't', 0x251c, 0xc4, 'q', 0x2500, 0xc5, 'n', 0x253c, 0xc6, 't', 0x255e,
    0xc7, 't', 0x255f, 0xc8, 'm', 0x255a, 0xc9, 'l', 0x2554, 0xca, 'v', 0x2569,
    0xcb, 'w', 0x2566, 0xcc, 't', 0x2560, 0xcd, 'q', 0x2550, 0xce, 'n', 0x256c,
    0xcf, 'v', 0x2567, 0xd0, 'v', 0x2568, 0xd1, 'w', 0x2564, 0xd2, 'w', 0x2565,
    0xd3, 'j', 0x2559, 0xd4, 'j', 0x2558, 0xd5, 'l', 0x2552, 0xd6, 'l', 0x2553,
    0xd7, 'n', 0x256b, 0xd8, 'n', 0x256a, 0xd9, 'j', 0x2518, 0xda, 'l', 0x250c,
    /* misc */
    0xf1, 'g', 0x00b1, 0xf8, 'f', 0x00b0, 0xf9, '~', 0x2219, 0xfa, '~', 0x00b7,
    0xfe, 'h', 0x25a0, 0xff, ' ' | 0x80, 0xa0,
    0xf0, '#', 0x2261, 0x12, '/', 0x2195, 0xfb, '*', 0x221a
};

static char alt_keys[] = "QWERTYUIOP[]\r\0ASDFGHJKL;'`\0\\ZXCVBNM,./";
static char alt_num_keys[] = "1234567890-=";
static char esc_str[] = "\033A";

/* use above table if no .tix file is found */
static int do_default( void )
/***************************/
{
    unsigned char       code, c, cmap;
    unsigned short      unicode;
    int                 i;
    char               *s;
    int                 utf8_mode = 0;

    if ( ( ( s = getenv( "LC_ALL" ) )   && *s ) ||
         ( ( s = getenv( "LC_CTYPE" ) ) && *s ) ||
         ( ( s = getenv( "LANG" ) )     && *s ) ) 
        if ( strstr( s, "UTF" ) || strstr( s, "utf" ) )
            utf8_mode = 1;

    for( i = 0; i < sizeof( default_tix ) / sizeof( default_tix[0] ) ; i ++ ) {
        code = default_tix[i].cp437;
        cmap = c = default_tix[i].vt100;
        unicode = default_tix[i].unicode;
        ti_char_map[code][0] = cmap;
        if( utf8_mode ) {
            wctomb( ti_char_map[code], unicode );
        } else if( (c & 0x80) == 0 ) {
            cmap = find_acs_map( c, acs_chars );
            if( cmap != '\0' ) {
                ti_alt_map_set( code );
            } else {
                cmap = find_acs_map( c, acs_default );
                if( cmap == '\0' ) {
                    cmap = c;
                    ti_alt_map_set( code );
                }
            }
            ti_char_map[code][0] = cmap;
        }
    }
    for( i = 0; i < sizeof( alt_keys ); i++ ) {
        if ( alt_keys[i] ) {
            esc_str[1] = alt_keys[i];
            TrieAdd( 0x110 + i, esc_str );
            if( alt_keys[i] >= 'A' && alt_keys[i] <= 'Z' ) {
                esc_str[1] += 0x20;
                TrieAdd( 0x110 + i, esc_str );
            }
        }
    }
    for( i = 0; i < sizeof( alt_num_keys ); i++ ) {
        if ( alt_num_keys[i] ) {
            esc_str[1] = alt_num_keys[i];
            TrieAdd( 0x178 + i, esc_str );
        }
    }
    /* sticky function key ^F */
    TrieAdd( 0xff0, "\6");
    /* sticky ALT ^A */
    TrieAdd( 0xff3, "\1");
    s = getenv("TERM");
    if( s != NULL && strncmp( s, "xterm", 5 ) == 0 ) {
        /* special xterm keys available in recent xterms */
        TrieAdd( EV_CTRL_CURSOR_UP, "\033[1;5A" );
        TrieAdd( EV_CTRL_CURSOR_DOWN, "\033[1;5B" );
        TrieAdd( EV_CTRL_CURSOR_RIGHT, "\033[1;5C" );
        TrieAdd( EV_CTRL_CURSOR_LEFT, "\033[1;5D" );
        TrieAdd( EV_CTRL_HOME, "\033[1;5H" );
        TrieAdd( EV_CTRL_END, "\033[1;5F" );
        TrieAdd( EV_CTRL_PAGE_UP, "\033[5;5~" );
        TrieAdd( EV_CTRL_PAGE_DOWN, "\033[6;5~" );

        /* slightly older xterms report these sequences... */
        TrieAdd( EV_CTRL_CURSOR_UP, "\033[O5A" );
        TrieAdd( EV_CTRL_CURSOR_DOWN, "\033[O5B" );
        TrieAdd( EV_CTRL_CURSOR_RIGHT, "\033[O5C" );
        TrieAdd( EV_CTRL_CURSOR_LEFT, "\033[O5D" );
        TrieAdd( EV_CTRL_HOME, "\033[O5H" );
        TrieAdd( EV_CTRL_END, "\033[O5F" );
    }
    if( utf8_mode && s != NULL && strncmp( s, "linux", 5 ) == 0 ) {
        /* force UTF-8 mode if the locale is set that way; *
         * we may be on a new VT on the Linux console      */
        write( UIConHandle, "\033%G", 3 );
    }
    return( 1 );
}


int ti_read_tix( char *termname )
/*******************************/
{
    int         i;
    int         ret;

    memset( _ti_alt_map, 0, sizeof( _ti_alt_map ) );

    for( i = 0; i < sizeof( ti_char_map ) / sizeof( ti_char_map[0] ); i++ )
        ti_char_map[i][0]=i;

    if( !init_tix_scanner( termname ) ) {
        return( do_default() );
    }
    ret = do_parse();
    close_tix_scanner();
    return( ret );
}
