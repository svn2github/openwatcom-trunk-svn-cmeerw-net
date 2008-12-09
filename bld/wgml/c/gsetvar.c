/****************************************************************************
*
*                            Open Watcom Project
*
*  Copyright (c) 2004-2008 The Open Watcom Contributors. All Rights Reserved.
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
* Description:  process :set tag, .se and .sr keywords and symbolic substitution
*               still incomplete
****************************************************************************/

#define __STDC_WANT_LIB_EXT1__  1      /* use safer C library              */

#include <stdarg.h>
#include <errno.h>

#include "wgml.h"
#include "gvars.h"



/* construct symbol name and optionally subscript from input
 *
 *
 */

char    *scan_sym( char * p, symvar * sym, sub_index * subscript )
{
    size_t      k;
    char    *   sym_start;
    bool        scan_err;

    scan_err = false;
    k = 0;
    sym->next = NULL;
    sym->flags = 0;
    while( *p && *p == ' ' ) {          // skip over spaces
        p++;
    }
    if( *p == '*' ) {                   // local var
        p++;
        sym->flags |= local_var;
    }
    sym_start = p;

    while( *p &&
           (isalnum( *p )
            || *p == '@' || *p == '#' || *p == '$' || *p == '_') ) {

        if( k < SYM_NAME_LENGTH ) {
            if( (k == 3) && (sym->name[0] != '$') ) {
                if( sym->name[ 0 ] == 's' &&
                    sym->name[ 1 ] == 'y' &&
                    sym->name[ 2 ] == 's' ) {

                    sym->name[ 0 ] = '$';   // create sys shortcut $
                    k = 1;
                }
            }
            sym->name[ k++ ] = tolower( *p );
            sym->name[ k ] = '\0';
        } else {
            if( !scan_err ) {
                out_msg( "WNG_SYMBOL_NAME_TOO_LONG %s\n", sym_start );
                wng_count++;
            }
        }
        p++;
    }
    *subscript = no_subscript;          // not subscripted
    if( *p == '(' ) {                   // subscripted
        char    *   psave = p;
        int         sign = 1;           // default positive
        long        val  = 0;

        p++;
        if( !isdigit( *p ) ) {
            if( *p == '-' ) {
                sign = -1;
            } else if( *p == '+' ) {
                sign = 1;
            } else {
                scan_err = true;
            }
        }
        while( *p && *p != ')' ) {
            if( isdigit( *p ) ) {
                val = val * 10 + *p - '0';
            }
            p++;
        }
        if( !scan_err && (*p == ')') ) {
            p++;
        }
        *subscript = val * sign;
        if( scan_err ) {
           p = psave;
        }
    }
    return( p );
}

/***************************************************************************/
/*  processing  SET                                                        */
/*                                                                         */
/*                          = <character string>                           */
/*         .SE       symbol = <numeric expression>                         */
/*                          <OFF>                                          */
/*                                                                         */
/***************************************************************************/

void    scr_se( void )
{
    char        *   p;
    char        *   valstart;
    symvar          sym;
    sub_index       subscript;
    bool            scan_err;
    int             rc;
    symvar      * * working_dict;

    subscript = no_subscript;           // not subscripted
    scan_err = false;
    p = scan_sym( scan_start, &sym, &subscript );

    if( sym.flags & local_var ) {
        working_dict = &input_cbs->local_dict;
    } else {
        working_dict = &global_dict;
    }

    while( *p && *p == ' ' ) {          // skip over spaces
        p++;
    }
    if( *p == '\0' ) {
        out_msg( "WNG_SYMBOL_VALUE_MISSING for %s\n", sym.name );
        show_include_stack();
        wng_count++;
        scan_err = true;
    } else {
        if( *p == '(' ) {               // subscripted
            int sign = 1;               // default positive
            long val  = 0;

            p++;
            if( !isdigit( *p ) ) {
                if( *p == '-' ) {
                    sign = -1;
                } else if( *p == '+' ) {
                    sign = 1;
                } else {
                    if( *p == ')' ) {
                        out_msg( "ERR_SYMBOL_autoincrement not supported %s\n",
                                 sym.name );
                        show_include_stack();
                        err_count++;
                        scan_err = true;
                    } else {
                        out_msg( "ERR_SYMBOL_subscript invalid %s\n", sym.name );
                        show_include_stack();
                        err_count++;
                        scan_err = true;
                    }
                }
            }
            while( *p && *p != ')' ) {
                if( isdigit( *p ) ) {
                    val = val * 10 + *p - '0';
                }
                p++;
            }
            if( *p == ')' ) {
                p++;
            }
            subscript = val * sign;
        }
        while( *p && *p == ' ' ) {      // skip over spaces
            p++;
        }
        if( *p == '=' ) {
            p++;
            while( *p && *p == ' ' ) {  // skip over spaces
                p++;
            }
            valstart = p;
            if( *valstart == '\'' || *valstart == '"' ) { // quotes ?
                p++;
                while( *p && (*valstart != *p) ) { // look for quote end
                    ++p;
                }
                if( *p == *valstart ) { // delete quotes
                    valstart++;
                    *p = '\0';
                }
            }

            rc = add_symvar( working_dict, sym.name, valstart, subscript, sym.flags );

        } else {                        // OFF value = delete variable ?
            if( tolower( *p )       == 'o' &&
                tolower( *(p + 1) ) == 'f' &&
                tolower( *(p + 2) ) == 'f' &&
                *(p + 3)            == '\0' ) {
                p += 3;
                sym.flags |= deleted;
            } else {
                out_msg( "WNG_SYMBOL_VALUE_INVALID for %s (%s)\n", sym.name, p );
                wng_count++;
                scan_err = true;
            }
        }
    }
    return;
}


typedef enum {                          // type of constants
    BIN     = 1,
    DEC     = 3,
    HEX     = 4,
    CHAR    = 8
} selfdef_types;

typedef enum {
    condA   = 0,                        // inital state
    condB   = 1,                        // left paren, Binary + or -
    condC   = 2,                        // unary + or -
    condD   = 3,                        // * or /
    condE   = 4,                        // absolute term or right paren
} conditions;

typedef enum  {                      // internal character representation for
                                        // easier grouping of chars
   i_0      = 0,
   i_1      = 1,
   i_2      = 2,
   i_3      = 3,
   i_4      = 4,
   i_5      = 5,
   i_6      = 6,
   i_7      = 7,
   i_8      = 8,
   i_9      = 9,
   i_a      = 0x0a,
   i_b      = 0x0b,
   i_c      = 0x0c,
   i_d      = 0x0d,
   i_e      = 0x0e,
   i_f      = 0x0f,
   i_g      = 0x10,
   i_h      = 0x11,
   i_i      = 0x12,
   i_j      = 0x13,
   i_k      = 0x14,
   i_l      = 0x15,
   i_m      = 0x16,
   i_n      = 0x17,
   i_o      = 0x18,
   i_p      = 0x19,
   i_q      = 0x1a,
   i_r      = 0x1b,
   i_s      = 0x1c,
   i_t      = 0x1d,
   i_u      = 0x1e,
   i_v      = 0x1f,
   i_w      = 0x20,
   i_x      = 0x21,
   i_y      = 0x22,
   i_z      = 0x23,
   i_dollar = 0x24,                     // $
   i_pound  = 0x25,                     // #
   i_at     = 0x26,                     // @
                   // up to here alphanumeric
   i_plus   = 0x27,                     // +
   i_minus  = 0x28,                     // -
   i_mult   = 0x29,                     // *
   i_aster  = 0x29,                     // = imult
   i_divid  = 0x2a,                     // /
   i_slash  = 0x2a,                     // = idivid
   i_comma  = 0x2b,                     // ,
   i_equal  = 0x2c,                     // =
   i_ampsd  = 0x2d,                     // &
   i_priod  = 0x2e,                     // .
   i_lparn  = 0x2f,                     // (
   i_rparn  = 0x30,                     // )
   i_quote  = 0x31,                     // '
   i_dquot  = 0x32,                     // "
   i_blank  = 0x33,
   i_alpha  = 0x26                      // = iat
} ichar;

#if 0
static const UCHAR ext2int[256] = {

    /* .0     .1     .2     .3     .4     .5     .6     .7  */
    '\xff','\xff','\xff','\xff','\xff','\xff','\xff','\xff', //0x
    '\xff','\xff','\xff','\xff','\xff','\xff','\xff','\xff',

    '\xff','\xff','\xff','\xff','\xff','\xff','\xff','\xff', //1x
    '\xff','\xff','\xff','\xff','\xff','\xff','\xff','\xff',

    '\x33','\xFF','\x32','\x25','\x24','\xff','\x17','\x31', //2x
    '\x2f','\x30','\x29','\x27','\x2b','\x28','\x2e','\x2a',

    '\x00','\x01','\x02','\x03','\x04','\x05','\x06','\x07', //3x
    '\x08','\x09','\xff','\xff','\xff','\x2c','\xff','\xff',

    '\x26','\x0a','\x0b','\x0c','\x0e','\x0F','\x10','\x11', //4x
    '\x12','\x13','\x14','\x15','\x16','\x17','\x18','\x19',

    '\x1a','\x1b','\x1c','\x1d','\x1e','\x1F','\x20','\x21', //5x
    '\x22','\x23','\x24','\xff','\xff','\xff','\xff','\xff',

    '\xff','\xff','\xff','\xff','\xff','\xff','\xff','\xff', //6x
    '\xFF','\xFF','\xff','\xff','\xff','\xff','\xff','\xff',

    '\xFF','\xFF','\xFF','\xFF','\xFF','\xFF','\xFF','\xFF', //7x
    '\xFF','\xff','\xff','\xff','\xff','\xff','\xff','\xff',

    /* .0     .1     .2     .3     .4     .5     .6     .7  */
    '\xFF','\x61','\x62','\x63','\x64','\x65','\x66','\x67',
    '\x68','\x69','\xFF','\xFF','\xFF','\xFF','\xFF','\xFF',

    '\xFF','\x6A','\x6B','\x6C','\x6D','\x6E','\x6F','\x70',
    '\x71','\x72','\xFF','\xFF','\xFF','\xFF','\xFF','\xFF',

    '\xFF','\xe1','\x73','\x74','\x75','\x76','\x77','\x78',
    '\x79','\x7A','\xFF','\xFF','\xFF','\x5B','\xFF','\xFF',

    '\xFF','\xFF','\xFF','\xFF','\xFF','\x40','\xFF','\xFF',
    '\xFF','\xFF','\xFF','\x7C','\xFF','\x5D','\xFF','\xFF',

    '\x84','\x41','\x42','\x43','\x44','\x45','\x46','\x47',
    '\x48','\x49','\xFF','\xFF','\xDD','\xFF','\xFF','\xFF',

    '\x81','\x4A','\x4B','\x4C','\x4D','\x4E','\x4F','\x50',
    '\x51','\x52','\xFF','\xFF','\x7D','\xFF','\xFF','\xFF',

    '\x99','\xFF','\x53','\x54','\x55','\x56','\x57','\x58',
    '\x59','\x5A','\xFF','\xFF','\x5C','\xFF','\xFF','\xFF',

    '\x30','\x31','\x32','\x33','\x34','\x35','\x36','\x37',
    '\x38','\x39','\xFF','\xFF','\x5D','\xFF','\xFF','\xFF'
        };
#endif




static  ichar ext_i( char c )
{
    switch( c ) {
    case   '0':  return( i_0 ); break;
    case   '1':  return( i_1 ); break;
    case   '2':  return( i_2 ); break;
    case   '3':  return( i_3 ); break;
    case   '4':  return( i_4 ); break;
    case   '5':  return( i_5 ); break;
    case   '6':  return( i_6 ); break;
    case   '7':  return( i_7 ); break;
    case   '8':  return( i_8 ); break;
    case   '9':  return( i_9 ); break;
    case   'A':  return( i_a ); break;
    case   'B':  return( i_b ); break;
    case   'C':  return( i_c ); break;
    case   'D':  return( i_d ); break;
    case   'E':  return( i_e ); break;
    case   'F':  return( i_f ); break;
    case   'G':  return( i_g ); break;
    case   'H':  return( i_h ); break;
    case   'I':  return( i_i ); break;
    case   'J':  return( i_j ); break;
    case   'K':  return( i_k ); break;
    case   'L':  return( i_l ); break;
    case   'M':  return( i_m ); break;
    case   'N':  return( i_n ); break;
    case   'O':  return( i_o ); break;
    case   'P':  return( i_p ); break;
    case   'Q':  return( i_q ); break;
    case   'R':  return( i_r ); break;
    case   'S':  return( i_s ); break;
    case   'T':  return( i_t ); break;
    case   'U':  return( i_u ); break;
    case   'V':  return( i_v ); break;
    case   'W':  return( i_w ); break;
    case   'X':  return( i_x ); break;
    case   'Y':  return( i_y ); break;
    case   'Z':  return( i_z ); break;

    case   '$':  return( i_dollar ); break;
    case   '#':  return( i_pound ); break;
    case   '@':  return( i_at ); break;
    case   '+':  return( i_plus ); break;
    case   '-':  return( i_minus ); break;
    case   '*':  return( i_aster ); break;
    case   '/':  return( i_slash ); break;
    case   ',':  return( i_comma ); break;
    case   '=':  return( i_equal ); break;
    case   '&':  return( i_ampsd ); break;
    case   '.':  return( i_priod ); break;
    case   '(':  return( i_lparn ); break;
    case   ')':  return( i_rparn ); break;
    case   '\'': return( i_quote ); break;
    case   '"':  return( i_dquot ); break;
    case   ' ':  return( i_blank ); break;
    default:
        return( c );
        break;
    }
}










char    num_sign = ' ';                 // unary operator






typedef enum {
    opend       = 0,
    opplus,
    opminus,
    opuplus,
    opuminus,
    opstar,
    opslash
} oper_hier;


#define MAXTERMS    25
#define MAXPARENS   10
#define MAXOPERS    MAXTERMS * 2
static struct  {
    long        terms[ MAXTERMS ];
    int16_t     rlist[ MAXTERMS ];
    int8_t      ntrms[ MAXTERMS ];
    oper_hier   oprns[ MAXOPERS ];

    int         currterm;
    int         currrlist;
    int         currntrms;
    int         curroper;

    int         currparn;

    conditions  cond;
    int         endoe;
    int         newop;
} termswk;


static int      charcnt;
static int      charcntmax;
static long     result;
static char     result_c[ 5 ];


/***************************************************************************/
/*  logic from cbt282.122                                                  */
/*  self defining value convert                                            */
/***************************************************************************/

static  char *sdvcf( selfdef_types typ, int bitcnt, char *s, char *stop )
{
        long    res;
        char    c;
        ichar   ic;
static  int     sdv08[ 8 ] = { 1, 0, 9, 15, 0,0,0, 255 };
        char    resc[ 5 ] = "    ";

    res = 0;
    charcnt = 0;
    sdv08[ 1 ] = typ;

    for( ;; ) {
        if( s > stop ) {
            break;
        }
        c = *s;
        ic = ext_i( c );
        if( typ == DEC || typ == BIN ) {
            if( !isdigit( c ) ) {
                break;
            }
            if( ic > sdv08[ typ - 1 ] ) {
                break;
            }
        } else {
            if( typ == HEX ) {
                if( ic > i_f ) {
                    break;              // no hex digit
                }
            }
        }

        if( typ == CHAR ) {
            if( c == '\'') {
                if( *(s+1) == '\'' ) {
                    s++;
                    continue;
                } else {
                    break;
                }
            }
            resc[ 0 ] = resc[ 1 ];
            resc[ 1 ] = resc[ 2 ];
            resc[ 2 ] = resc[ 3 ];
            resc[ 3 ] = *s;
        } else {
            if( typ == DEC ) {
                if( res >= INT_MAX / 10 ) {
                    charcnt = 10;       // overflow
                } else {
                    res = res * 10 + ic;
                    if( res > INT_MAX - 1 ) {
                        charcnt = 10;
                    }
                }
            } else {
                res <<= bitcnt;
                res += ic;
            }
        }
        charcnt++;
        s++;
    }
    if( typ == CHAR ) {
        strcpy_s( result_c, sizeof( result_c ), resc );
    } else {
        result  = res;
    }
    return( s );
}


/***************************************************************************/
/*  logic from cbt282.122                                                  */
/***************************************************************************/

condcode getnum( getnum_block *gn )
{
    char    *a;                         // arg start
    char    *z;                         // arg stop
    char    c;
    ichar   ic;



    memset( &termswk, sizeof( termswk ), '\0' );
/*
    termswk.currterm = 0;
    termswk.currparn = 0;
    termswk.curroper = 0;
    termswk.currrlist = 0;
*/
    a = gn->argstart;
    z = gn->argstop;
    while( a < z && *a == ' ' ) {
        a++;                            // skip leading blanks
    }
    gn->errstart = a;
    gn->first    = a;
    if( a > z ) {
        gn->cc = omit;
        return( omit );                 // nothing there
    }
    c = *a;
    ic = ext_i( c );                    // to internal representation
    if( c == '+' || c == '-' ) {
        num_sign = c;                   // unary sign
    }

//look:
    for( ;; ) {
        if( a > z ) {
            break;
        }

    /***********************************************************************/
    /*  test/expand  pageno symbol ???? not implemented                    */
    /***********************************************************************/

//look02:
        c = *a;
        ic = ext_i( c );                // to internal representation
        if( ic <= i_alpha ) {
            gn->errstart = a;
            if( isdigit( c ) ) {
                a = sdvcf( DEC, 8, a, z );
                termswk.terms[ termswk.currterm ] = result;
                charcntmax = 8;
            } else {
                if( *(a+1) != '\'' ) {
                    gn->cc = notnum;
                    gn->error = illchar;
                    return( notnum );
                }
//nextq:
//first char alphabetic , 2nd is quote
                c = tolower( c );
                ic = ext_i( c );
                switch ( c ) {
                case 'b' :                  // binary selfdefining value
                    a += 2;
                    a = sdvcf( BIN, 32, a, z );
                    termswk.terms[ termswk.currterm ] = result;
                    charcntmax = 32;
                    break;
                case 'c' :                  // char self defining value
                    a += 2;
                    a = sdvcf( CHAR, 4, a, z );
                    termswk.terms[ termswk.currterm ] = (long) result_c;
                    charcntmax = 4;
                    break;
                case 'x' :                  // hex self defining value
                    a += 2;
                    a = sdvcf( HEX, 8, a, z );
                    termswk.terms[ termswk.currterm ] = result;
                    charcntmax = 8;
                    break;
                default:                    // invalid
                    gn->cc = notnum;
                    gn->error = illchar;
                    return( notnum );
                    break;
                }
//ckq:

                if( *a != '\'' ) {          // closing quote?
                    gn->cc = notnum;
                    gn->error = illchar;
                    return( notnum );
                } else {
                    a++;
                }
                if( charcnt <= 0 ) {        // charcnt is set in sdvcf()
                    gn->cc = notnum;
                    gn->error = illchar;
                    return( notnum );
                }
//absd:
                if( charcnt > charcntmax ) {// actual length > maximum
                    gn->cc = notnum;
                    gn->error = selfdef;
                    return( notnum );
                }
                if( termswk.cond > condA ) {
//absck:
                    if( termswk.cond >= condE ) {   // 2 terms in a row
                        gn->cc = notnum;
                        gn->error = ilorder;
                        return( notnum );
                    }
                }
//compt4:
                termswk.cond = condE;       // set absolute term
//  goto compt;

            }
        } else {

//notam:    first char not alfanumeric

            if( ic < i_comma ) {

//ltcom:    first char is one of + - * /
                if( ic >= i_aster ) {       // mult / div
                    if( termswk.cond < condE ) {
                        gn->cc = notnum;
                        gn->error = operr;
                        return( notnum );
                    }
                    termswk.cond = condD;   // set mult / div condition
                    termswk.newop = ic - (i_alpha - 2); // set hierarchy code
                } else {                    // leaves + or -
                    if( termswk.cond == condE ) {
//plmin3:
                        termswk.cond = condB;   // binary + or -
                        termswk.newop = ic - i_alpha;   // set hierarchy code

                    } else {
                        if( termswk.cond != condA ) {
                            termswk.cond = condC;   // unary + or -
//plmin2:
                            termswk.newop = ic - (i_alpha - 2);// set hierarchy code
                        } else {
                            termswk.cond = condE;

                            goto compt; // +++++++++++++++++++++++++++++++++
                        }
                    }
                }
//loop:
                if( termswk.curroper == 0 ) {
                    if( termswk.endoe ) {

                        break;
                    }
//put:
                    termswk.oprns[ termswk.curroper ] = termswk.newop;
                    termswk.curroper++;
                    a++;
                    continue;
                } else {
                    if( termswk.oprns[ termswk.curroper - 1 ] >= termswk.newop ) {
//le:
                        if( termswk.newop == opuminus || termswk.newop == opuplus) {
                            termswk.oprns[ termswk.curroper ] = termswk.newop;
                            termswk.curroper++;
                            a++;
                            continue;
                        } else {
                            if( termswk.oprns[ termswk.curroper - 1 ] ) {
//work:
                      //          int     ncomp = termswk.currrlist;
                        //        long    *firstx1 = &termswk.terms[ termswk.currterm -2 ];



                                switch ( termswk.oprns[ termswk.curroper ] ) {
                                case opplus :
                                    break;
                                case opminus :
                                    break;
                                case opuplus :
                                    break;
                                case  opuminus:
                                    break;
                                case opstar :
                                case opslash :
                                    break;
                                default:
                                    break;
                                }


                  //              goto work;


                            } else {
                                termswk.currparn--;

                                termswk.curroper--;

                                a++;
                                continue;
                            }
                        }
                    } else {
                        if( opslash == termswk.newop &&
                            termswk.oprns[ termswk.curroper - 1 ] == opstar ) {

                    //        goto work;  // +++++++++++++++++++++

                        }
                        termswk.oprns[ termswk.curroper ] = termswk.newop;
                        termswk.curroper++;
                        a++;
                        continue;
                    }
                }
            } else {                        // not less i_comma
                if( ic == i_lparn ) {
                    if( termswk.cond < condE ) {
                        termswk.cond = condB;   // left paren cond
                        if( termswk.currparn >= MAXPARENS - 1 ) {
                            gn->cc = notnum;
                            gn->error = parerr;
                            return( notnum );
                        }
                        termswk.currparn++;
                        termswk.oprns[ termswk.curroper ] = 0;
                        termswk.curroper++;
                        a++;
                    } else {
//lpend:
                    }
                } else {
                    if( ic == i_rparn ) {
//rpar:
                        if( termswk.cond < condE ) {// right paren following term?
                            gn->cc = notnum;
                            gn->error = operr;
                            return( notnum );
                        }
                        termswk.cond = condE;   // right paren cond
                        if( termswk.currparn == 0 ) {   // no paren open?
                            gn->cc = notnum;
                            gn->error = illchar;
                            return( notnum );
                        }
//noend:
                        termswk.newop = opend;

//   b loop:

                    } else {
                        if( ic == i_blank ) {
                            if( gn->ignore_blanks == 0 ) {
                                if( termswk.cond < condE || termswk.currparn > 0) {
                                    gn->cc = notnum;// no term prceceding
                                    gn->error = enderr;
                                    return( notnum );
                                }
//endng:
                                termswk.endoe = true;   // end of expression
//noend:
                                termswk.newop = opend;

//   b loop:


                            } else {
                                a++;    // skip over blank
                                continue;
                            }
                        } else {
                            gn->cc = notnum;
                            gn->error = illchar;
                            return( notnum );
                        }
                    }
                }
            }
        }
compt:
        if( termswk.currterm >= MAXTERMS - 1 ) {
            gn->cc = notnum;
            gn->error = mnyerr;         // too many terms
            return( notnum );
        }
        termswk.ntrms[ termswk.currterm ] = 2;
        termswk.currterm++;
        termswk.currrlist++;
    }                                   // for(;;)


    return( 0 );
}

/******* from http://www.openwatcom.org/ftp/manuals/1.5/wgmlref.pdf
           or docs\doc\wgmlref\*.gml

8.6 Symbolic Substitution
A symbol is a name which represents an arbitrary string of text. Once a symbol
is assigned a text value, the symbol can be used in the document source in
place of that text. Consider the following:
:SET symbol='product'
     value='WATCOM Script/GML'.
:GDOC.
:BODY.
:P.
Symbolic substitution is quite
simple with &product..
:eGDOC.

The document, when processed, may appear as follows:

Symbolic substitution is quite simple
with WATCOM Script/GML.

A symbol name is defined and assigned a string of text with the :set tag.
The value of the symbol name can be defined at any point in the document file.
Any valid character string may be assigned to the symbol name. When the symbol
is referenced later, the value is substituted into the input text. The
substitution is done before the source text or input translation is processed
by WATCOM Script/GML.
A symbol name is preceded by an ampersand(&) when referenced, and is terminated
by any character not valid in a symbol name. If the terminating character is a
period, it is considered part of the symbol specification (you must therefore
remember to specify two periods if a symbol ends a sentence).
The recognition of a symbol name is case insensitive.
The symbol name should not have a length greater than ten characters, and may
only contain letters, numbers, and the characters @, #, $ and underscore(_).
Specifying the letters SYS as the first three characters of the symbol name is
equivalent to specifying a ollar($) sign.
Recursive substitution is performed on a symbol. This means that the text
substituted for a symbol is checked for the presence of more symbol names.
As well, if the symbol name is immediately followed by another symbol name
(no intervening period or blanks), new names can be constructed from the
successive substitutions. For example:
:SET symbol='prodgml'
     value='WATCOM Script/GML'.
:SET symbol='prodname'
     value='gml'.
:GDOC.
:BODY.
:P.
Symbolic substitution is quite simple with &prod&prodname...
:eGDOC.

The first part of the symbol sequence, &prod, does not exist as a defined symbol.
However, when &prodname. is substituted, the resulting symbol name &prodgml
exists. The resulting substitution produces the following:

Symbolic substitution is quite simple with WATCOM Script/GML.

If an asterisk is specified immediately before the symbol name
(ie symbol='*prodname' or &*prodname.), then the symbol is local. Local symbols
may not be referenced outside the file or macro in which they are defined.
If an undefined local symbol is referenced in a macro, it is replaced with an
empty value.



9.70 SET
Format: :SET symbol='symbol-name'
             value='character-string'
                   delete.
This tag defines and assigns a value to a symbol name.
The symbol attribute must be specified. The value of this attribute is the
name of the symbol being defined, and cannot have a length greater than ten
characters. The symbol name may only contain letters, numbers, and the
characters @, #, $ and underscore(_).
The value attribute must be specified. The attribute value delete or a valid
character string may be assigned to the symbol name. If the attribute value
delete is used, the symbol referred to by the symbol name is deleted. Refer to
"Symbolic Substitution" for more information about symbol substitution.


********** from docs\gml\manuals\script-tso.txt ****************************


 SET SYMBOL allows the user to assign a character or numeric value to a
 Set Symbol.   It is  an alternate to .SR that will  function even when
 .SU is OFF.

                          = <character string>
         .SE       symbol = <numeric expression>
                          <OFF>

 This control word does not cause  a break.   Substitution is automati-
 cally applied  to the operands to  replace any Set Symbols  with their
 values,  and the result  is then processed as if the  .SR control word
 had been used instead.  For details and examples, see the .SR descrip-
 tion.

...

 SET REFERENCE assigns a character or numeric value to a Set Symbol.

                          = <character string>
         .SR       symbol = <numeric expression>
                          <OFF>


 This control word does not cause a break.  The Set Symbol specified as
 the  first operand  is  assigned the  value  specified  by the  second
 operand.   If  the second operand is  omitted,  a null string  will be
 assigned.   If the Set Symbol did not previously exist, it is created.
 The Symbol name must be followed by  an equal sign "=" with or without
 intervening blanks.

 symbol =:  The name  of a Set Symbol may consist of  up to ten charac-
    ters.   Names may include only  uppercase or lowercase alphabetics,
    digits, the "$", "#", and "@", and the underscore ("_").   When the
    UPPER option or ".SU UPPER" is in effect,  all lowercase alphabetic
    characters in a Set Symbol name are treated as if entered in upper-
    case.   The Set  Symbol name may optionally be  subscripted with an
    integer,  signed or unsigned.   A subscript may range from -1000000
    to +1000000.   A zero subscript is  logically the same as having no
    subscript.  The subscript value may be implied by specifying a null
    subscript in the form "()":
      .sr symbol() = ...
    has the same effect as
      .sr symbol = &symbol + 1
      .sr symbol(&symbol) = ...
    because SCRIPT  uses "array position zero"  to retain the  count of
    the number of elements in the array.
 symbol =  character string:   The string  operand may  be a  delimited
    string or an undelimited string.   In the first case,  if the first
    character is a ' (quote), " (double quote), / (slash),  | (or bar),
    !  (exclamation mark),  ^ (not sign),   or � (cent sign)  then that
    first character will be treated as  the delimiter if the last char-
    acter in  the operand  matches the  initial delimiter.    The value
    assigned will be from the character following the leading delimiter
    up to  the character  preceding the  final delimiter.    Blanks and
    embedded  delimiters are  considered  to be  part  of the  assigned
    value.
       If there is  no matching terminating delimiter,   the operand is
    treated as an undelimited string,   and the value assigned consists
    of everything up  to and including the last  non-blank character on
    the input line.
 symbol 'character string:   The equal sign in a  symbol assignment may
    be omitted if the  first character of the value is  a single quote.
    The value  is taken from the  character following the quote  to the
    end of the record, not including the last character if it also is a
    single quote.   If  the value is to include trailing  blanks then a
    terminating quote must be used.
 symbol =  numeric expression:  Numeric  values may consist  of expres-
    sions (see Examples for details).  Integer decimal terms as well as
    binary,  character,   and hexadecimal  self-defining terms  (in the
    ASSEMBLER sense)  are  supported.   Parentheses and unary  plus and
    minus operators  are fully  supported.   Blanks  between terms  and
    operators are optional.
 symbol = %:  The page-number symbol (see .PS)  if entered by itself or
    symbolically as &SYSPS, will be treated as though the current prin-
    table  output page  number had  been  specified in  its place  (the
    System Set Symbol &SYSPPAGE also  provides this ability).   If this
    form of  the control  word is used  within a Text  Block such  as a
    Footnote or  Floating Keep,   the value of  the symbol  is actually
    assigned twice:   once immediately and a second time when the block
    finally prints.    The page  number string is  always treated  as a
    character operand, even if the current page number is all numeric.
 symbol OFF:  Undefines the specified Set Symbol name.

 NOTES
 (1) If a character string is the assigned value, it must consist of no
     more characters  than the maximum  allowed by the  SRLength option
     (default is 240), or the string will be truncated on the right.
 (2) A  complete list  of  System Set  Symbols  and  their purposes  is
     provided in an Appendix to this manual.

 EXAMPLES
 (1) The following demonstrate valid numeric (integer) expressions:
       .sr i=5
       .sr i= +5
       .sr i=((+5*3-1)*7)+1
       .sr i = ((X'F05'+C'A')* B'1111')/16
       .sr i(1+1) = (1+1)

 (2) The following demonstrate character-string assignments:
       .sr x = 3.5
       .sr c=abcdefg
       .sr c = 'ABCDEFG'
       .sr c = "don't be silly"
       .sr string = 'undelimited string
       .sr alpha(1) = 'A'
       .sr alpha(2) = B
       .sr pagenum = %
 (3) The sequence:
       .sr equno=0
           .
           .
       .sr equno = &equno + 1
       .sr xeqn = &equno
        (&equno):  x = erfc(a - bc)
       .sr equno = &equno +1
        (&equno):  y = erfc(a + bc)
        Using Equation &xeqn gives ...
     produces the following results:
        (1):  x = erfc(a - bc)
        (2):  y = erfc(a + bc)
        Using Equation 1 gives ...
 (4) The sequence:
       .sr a = 0
       .sr b = -5
       .sr c = a
       .sr d = 'b'
       .sr &c = &&d+1
       The value of "a" is &a..
       The value of "b" is &b..
       The value of "c" is &c..
       The value of "d" is &d..
     produces the following results:
       The value of "a" is -4.
       The value of "b" is -5.
       The value of "c" is a.
       The value of "d" is b.
 (5) To undefine a Set Symbol:
       .sr var = first;.* Set Symbol 'var' has a value
       .sr var OFF    ;.* Set Symbol 'var' is undefined
       .sr var = next ;.* Set Symbol 'var' defined again

*************************************************************************************/
