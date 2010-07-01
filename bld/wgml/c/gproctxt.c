/****************************************************************************
*
*                            Open Watcom Project
*
*  Copyright (c) 2004-2009 The Open Watcom Contributors. All Rights Reserved.
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
* Description:  WGML Process text not tags / controlwords
*
*
*               process_text            control routine
*               document_new_page       start a new page
*               document_top_banner     output top banner
*               do_justify              insert spaces between words
*               intrans                 perform input translation
*               next_tab                finds next tab stop
*               process_line_full
*               puncadj                 insert spaces after .:?
*               set_h_start             set horizontal start postion
*               set_v_start             set vertical start position
*               test_page_full          test if page_bottom reached
*               text_chars_width        width to first wgml tab
*               wgml_tabs               expands wgml tabs
*
****************************************************************************/

#define __STDC_WANT_LIB_EXT1__  1      /* use safer C library              */

#include "wgml.h"
#include "gvars.h"
#include "outbuff.h"


/***************************************************************************/
/*  puncadj modelled after the host ASM sources to get the same horizontal */
/*  spacing as wgml4, but still no luck                               TBD  */
/*  further testing showed that .:!? are used as full stop characters      */
/*  the others ;,) have no special effect                                  */
/***************************************************************************/

static  void    puncadj( text_line * line, int32_t * delta0, int32_t rem,
                         int32_t cnt, uint32_t lm )
{
    text_chars  *   tleft;              // first text_char to justify
    text_chars  *   tn;
    text_chars  *   tw;
    int32_t         delta;
    int32_t         loop_cnt;
    int32_t         space = wgml_fonts[0].spc_width;// TBD
    int32_t         spacew;
    int32_t         remw = rem - rem;   // TBD
    char            ch;
    bool            changed;

    tleft = line->first;
    for( tleft = line->first; tleft->x_address < lm ; tleft = tleft->next ) {
        if( tleft == NULL ) {
            return;                     // no justify due to left margin
        }
    }
    if( tleft->next == NULL ) {
        return;                         // only 1 text_chars no justify
    }

    if( ps_device ) {
        space /= 2;                     // TBD
//      space -= 3;                     // TBD
    }

    changed = false;
    delta = *delta0;
    loop_cnt = 3;                       // 3 passes
    while( loop_cnt > 2 && delta >= space ) {   // only 1 pass TBD
        if( ps_device ) {
            space = wgml_fonts[0].spc_width / 2;// TBD
//          space += loop_cnt - 1;      // TBD
        }

        /* from right to left search for stop chars */
        for( tw = line->last->prev; tw != tleft; tw = tw->prev ) {

            tn = tw->next;
            ch = tw->text[tw->count - 1];
            switch( loop_cnt ) {
            case   3:                   // test full stop
                if( ch == '.' || ch == '!' || ch == '?' || ch == ':' ) {
                    spacew = space;
                    if( remw > 0 ) {
                        spacew++;
                        remw--;
                    }
                    delta -= spacew;
                    while( tn != NULL ) {
                        tn->x_address += spacew;
                        tn = tn->next;
                    }
                    changed = true;
                }
                break;
            case   2:                   // test half stop
                if( ch == ':' || ch == ';' ) {
#if 0
                    delta -= space;
                    while( tn != NULL ) {
                        tn->x_address += space;
                        tn = tn->next;
                    }
#else
                    spacew = space;
                    if( remw > 0 ) {
                        spacew++;
                        remw--;
                    }
                    delta -= spacew;
                    while( tn != NULL ) {
                        tn->x_address += spacew;
                        tn = tn->next;
                    }
#endif
                    changed = true;
                }
                break;
            case   1:
                if( ch == ',' || ch == ')' ) {
#if 0
                    delta -= space;
                    while( tn != NULL ) {
                        tn->x_address += space;
                        tn = tn->next;
                    }
#else
                    spacew = space;
                    if( remw > 0 ) {
                        spacew++;
                        remw--;
                    }
                    delta -= spacew;
                    while( tn != NULL ) {
                        tn->x_address += spacew;
                        tn = tn->next;
                    }
#endif
                    changed = true;
                }
                break;
            default:
                break;
            }
        }
        loop_cnt--;
    }
    *delta0 = delta;
}


/***************************************************************************/
/*  return the width of text up to the first tab stop                      */
/***************************************************************************/

static uint32_t text_chars_width( uint8_t * text, uint32_t count, uint8_t font )
{
    int         i;
    uint32_t    cur_count   = 0;
    uint32_t    retval      = 0;

    for( i = 0; i < count; i++) {
        if( (text[i] == '\t') || (text[i] == tab_char) ) {
            break;
        }
        cur_count++;
    }
    // if there are no tabs, cur_count will equal count
    if( cur_count != 0 ) {
        retval = cop_text_width( text, cur_count, font );
    }
    return( retval );
}


/***************************************************************************/
/*  return the first tab stop after a given position                       */
/***************************************************************************/

static tab_stop * next_tab( uint32_t c_pos )
{
    int                 i;
    tab_stop            l_tab;
    tab_stop    *       retval      = NULL;
    uint32_t            r_count;
    uint32_t            r_length;
    uint32_t            r_width;

    l_tab = cur_tabs->tabs[cur_tabs->current - 1];
    if( c_pos > l_tab.column ) { // resize tab list
        r_width = c_pos - l_tab.column;
        r_count = (r_width / inter_tab) + 1;
        if( (l_tab.column + r_count) > cur_tabs->length ) {
            // add enough tabs to ensure this isn't done every time
            r_count /= TAB_COUNT;
            r_count++;
            r_length = cur_tabs->length + (r_count * TAB_COUNT);
            cur_tabs->tabs = mem_realloc( cur_tabs->tabs, r_length * sizeof( tab_stop ) );
            cur_tabs->length = r_length;
        }
        for( i = cur_tabs->current; i < cur_tabs->length; i++ ) {
            cur_tabs->tabs[i].column = cur_tabs->tabs[i - 1].column + inter_tab;
            cur_tabs->tabs[i].fill_char = ' ';
            cur_tabs->tabs[i].alignment = ' ';
        }
        cur_tabs->current = cur_tabs->length;
    }
    // find the first tab_stop to the right of cur_pos
    for( i = 0; i < cur_tabs->current; i++ ) {
        if( c_pos < cur_tabs->tabs[i].column ) {
            retval = &cur_tabs->tabs[i];
            break;
        }
    }
    ju_x_start = retval->column; // set left boundary for justification

    return( retval );
}


/***************************************************************************/
/*  expand wgml tabs infesting a text_chars instance                       */
/***************************************************************************/

static text_chars * wgml_tabs( text_chars * in_chars )
{
    int             i;
    tab_stop    *   n_stop;
    text_chars  *   c_chars         = in_chars;
    text_chars  *   retval          = in_chars;
    uint8_t     *   c_text          = in_chars->text;
    uint32_t        count           = in_chars->count;
    uint32_t        c_count         = 0;
    uint32_t        start;

    for( i = 0; i < count; i++) {   // locate the first wgml tab, if any
        if( (c_text[i] == '\t') || (c_text[i] == tab_char) ) {
            break;
        }
        c_count++;
    }
    if( c_count == count ) {        // there were no wgml tabs in in_chars
        return( retval );
    }
    if( (g_cur_h_start == g_cur_left) && (c_count == 0) ) {
        // in_chars is at the start of the line and starts with a wgml tab
        n_stop = next_tab( g_cur_h_start - g_page_left );
        g_cur_h_start = n_stop->column + g_page_left;
        in_chars->x_address = g_cur_h_start;
        i++;
        if( (c_text[i] == '\t') || (c_text[i] == tab_char) ) {
            // multiple initial wgml tabs: in_chars becomes empty
            in_chars->count = 0;
        } else {
            // the text after the initial tab is what in_chars will control
            start = i;
            for( i; i < count; i++ ) {
                if( (c_text[i] == '\t') || (c_text[i] == tab_char) ) {
                    break;                
                }
            }
            c_count = i - start;
            memmove_s( &in_chars->text[0], c_count, &in_chars->text[start], c_count);
            in_chars->count = c_count;
            in_chars->width = cop_text_width( in_chars->text, c_count, in_chars->font_number );
            g_cur_h_start += in_chars->width;
        }
    } else {
        // in_chars will control the text up to the first wgml tab, if any
        in_chars->count = c_count;
        if( in_chars->width > 0 ) { // previously set by text_chars_width()
            g_cur_h_start += in_chars->width;
        }
    }

    if( i < count ) { // there is at least one more tab to process
        n_stop = next_tab( g_cur_h_start - g_page_left );
        g_cur_h_start = n_stop->column + g_page_left;
        i++;
        start = i;
        for( i; i < count; i++ ) {
            for( i; i < count; i++ ) { // get the text up to the next tab
                if( (c_text[i] == '\t') || (c_text[i] == tab_char) ) {
                    break;
                }
            }
            // now create a new text_chars for the text
            c_count = i - start;
            retval = alloc_text_chars( &c_text[start], c_count, \
                                        in_chars->font_number );
            retval->type = in_chars->type;
            retval->x_address = g_cur_h_start;
            c_chars->next = retval;
            retval->prev = c_chars;
            c_chars = c_chars->next;
            retval->width = cop_text_width( retval->text, retval->count, \
                                                    in_chars->font_number );
            g_cur_h_start += retval->width;

            start = i + 1;  // set up for the next intertab text, if any
            n_stop = next_tab( g_cur_h_start - g_page_left );
            g_cur_h_start = n_stop->column + g_page_left;
        }
        i = count - 1; // tabs at the end of the line become empty text_chars
        if( (c_text[i] == '\t') || (c_text[i] == tab_char) ) {
            retval = alloc_text_chars( NULL, 0, in_chars->font_number );
            retval->type = in_chars->type;
            retval->x_address = g_cur_h_start;
            c_chars->next = retval;
            retval->prev = c_chars;
            c_chars = c_chars->next;
        }
    }

    return( retval );
}


/***************************************************************************/
/*  justification  experimental    treat half as left               TBD    */
/***************************************************************************/

void    do_justify( uint32_t lm, uint32_t rm, text_line * line )
{
    text_chars  *   tc;
    text_chars  *   tw;
    text_chars  *   tl;                 // last text_chars in line
    int32_t         sum_w;              // sum of words widths
    int32_t         hor_end;
    int32_t         cnt;                // # of text_chars
    int32_t         line_width;         // usable line length
    int32_t         delta;              // space increment between words
    int32_t         delta0;             // total space to distribute
    int32_t         delta1;
    int32_t         rem;              // possible part of space to distribute
    int32_t         deltarem;
    ju_enum         just;
    symsub  *       symjusub;         // for debug output string value of .ju

    if( ProcFlags.justify == ju_off || ProcFlags.literal || line == NULL
        || line->first == NULL) {
        return;
    }
    if( lm >= rm ) { // with tabbing, ju_x_start can be past the right margin
        return;
    }

    /***********************************************************************/
    /*  for PS device remainder decrement is treated differently      TBD  */
    /***********************************************************************/
    if( ps_device ) {
        deltarem = 1;                   // TBD was 2
    } else {
        deltarem = 1;
    }

    sum_w = 0;
    hor_end = 0;
    cnt = 0;
    tw = line->first;
    tl = line->last;
    tc = tw;
    hor_end = tl->x_address + tl->width;// hor end position

    do {                                // calculate used width
        if( tw->x_address >= lm ) {  // no justify for words left of ju start
            if( cnt == 0 ) {
                tc = tw;              // remember first text_char for justify
            }
            cnt++;                      // number of 'words'
            sum_w += tw->width;         // sum of 'words' widths
        }
        tw = tw->next;
    } while( tw != NULL );

    line_width = rm - lm;

    if( (sum_w <= 0) || (hor_end >= rm) || (line_width < 1) ) {
        return;                         // no justify needed / possible
    }
    delta0 = rm - hor_end;
    if( ProcFlags.justify == ju_on ) {
        if( cnt < 2 ) {      // one text_chars only, no full justify possible
            return;
        }
    }
    if( cnt < 2 ) {
        delta = delta0;                 // one text_chars gets all space
        rem   = 0;
    } else {
        delta = delta0 / (cnt - 1);
        rem   = delta0 % (cnt - 1);
    }

    if( input_cbs->fmflags & II_research && GlobalFlags.lastpass ) {
        find_symvar( &sys_dict, "$ju", no_subscript, &symjusub);// .ju as string
        out_msg( "\n ju_%s lm:%d %d rm:%d sum_w:%d hor_end:%d"
                 " delta:%d rem:%d delta0:%d cnt:%d\n", symjusub->value,
                 lm, line_width, rm, sum_w, hor_end, delta, rem, delta0, cnt );
    }
    if( delta < 1 && rem < 1 ) {        // nothing to distribute
        return;
    }
    switch( ProcFlags.justify ) { // convert inside / outside to left / right
    case ju_inside :                    // depending on odd / even page
        if( page & 1 ) {
            just = ju_left;
        } else {
            just = ju_right;
        }
        break;
    case ju_outside :
        if( page & 1 ) {
            just = ju_right;
        } else {
            just = ju_left;
        }
        break;
    default :
        just = ProcFlags.justify;
        break;
    }

    switch( just ) {                  // what type of justification is wanted
/*************************************
    case  ju_half :                   Treated as left ??? TBD
        delta /= 2;
        if( delta < 1 && rem < 1 ) {
            break;
        }
        // falltrough
************************************** */
    case  ju_on :
//      if( tc->x_address < lm ) {
//          break;                      // left of left margin no justify
//      }

        puncadj( line, &delta0, rem, cnt - 1, lm );

        hor_end = tl->x_address + tl->width;// hor end position
        delta0 = rm - hor_end;          // TBD
        if( cnt < 2 ) {
            delta = delta0;             // one text_chars gets all space
            rem   = 0;
        } else {
            delta = delta0 / (cnt - 1);
            rem   = delta0 % (cnt - 1);
        }

        if( input_cbs->fmflags & II_research && GlobalFlags.lastpass ) {
            test_out_t_line( line );
            out_msg( "\n ju_%s lm:%d %d rm:%d sum_w:%d hor_end:%d"
                     " delta:%d rem:%d delta0:%d cnt:%d\n", symjusub->value,
                     lm, line_width, rm, sum_w, hor_end, delta, rem, delta0,
                     cnt );
        }
        if( delta < 1 && rem < 1 ) {    // nothing to distribute
            return;
        }


        delta1 = delta;
        tw = tc->next;
        while( tw != NULL ) {
            if( rem > 0 ) {             // distribute remainder, too
                tw->x_address += delta + deltarem;
                delta += delta1 + deltarem;
//              if( !ps_device ) {      // TBD
                    rem -= deltarem;
//              }
            } else {
                tw->x_address += delta;
                delta += delta1;
            }
            tw = tw->next;
        }
        break;
    case  ju_half :                     // treat as left
    case  ju_left :
        delta = tc->x_address - lm;     // shift to the left
        if( delta < 1 ) {
            break;                      // already left
        }
        tw = tc;
        do {
            tw->x_address -= delta;
            tw = tw->next;
        } while( tw != NULL );
        break;
    case  ju_right :
        delta = rm - hor_end;           // shift right
        if( delta < 1 ) {
            break;                      // already at right margin
        }
        tw = tc;
        do {
           tw->x_address += delta;
           tw = tw->next;
        } while( tw != NULL );
        break;
    case  ju_centre :
        delta = (rm - hor_end) / 2;
        if( delta < 1 ) {
            break;                      // too wide no centre possible
        }
        tw = tc;
        do {
           tw->x_address += delta;
           tw = tw->next;
        } while( tw != NULL );
        break;
    default:
        break;
    }
}


/***************************************************************************/
/*  start a new document page                                              */
/***************************************************************************/

void document_new_page( void )
{
    if( GlobalFlags.lastpass ) {
       fb_document_page();
    }
    page++;
    ProcFlags.page_started = false;     // ? TBD
    post_skip = NULL;                   // TBD
    g_cur_v_start = g_page_top_org;
}


/***************************************************************************/
/*  output top banner                                                      */
/***************************************************************************/

void document_top_banner( void )
{
    g_cur_h_start = g_page_left_org;
    g_cur_left    = g_page_left_org;
    out_ban_top( sect_ban_top[page & 1] );
}

/***************************************************************************/
/*  if input translation is active                                         */
/*      look for input escape char and translate the following char,       */
/*      delete the escape char                                             */
/***************************************************************************/

void    intrans( char * data, uint16_t * len, uint8_t font )
{
    char    *   ps;                     // source ptr
    char    *   pt;                     // target ptr
    uint32_t    k;

    if( !ProcFlags.in_trans ) {
        return;                         // input translation not active
    }
    k = *len;
    ps = data;
    pt = data;
    for( k = 0; k <= *len; k++ ) {
        if( *ps == in_esc ) {           // translate needed
            ps++;                       // skip escape char
            k++;                        // and count
            *pt = cop_in_trans( *ps, font );   // translate
            ps++;
            pt++;
        } else {
            *pt++ = *ps++;              // else copy byte
        }
    }
    if( pt < ps ) {                     // something translated
        *len -= (ps - pt);              // new length
        *pt = ' ';
    }
}


/***************************************************************************/
/*  set position                                                           */
/***************************************************************************/
void set_h_start( void )
{
    if( !ProcFlags.keep_left_margin ) {
        g_cur_left = g_page_left + g_indent;
    }
    g_cur_h_start = g_cur_left;

}

/***************************************************************************/
/*  test whether page is full and finish page                              */
/***************************************************************************/

void    test_page_full( void )
{
    bool newpage;

    if( ProcFlags.page_started ) {
        if( bin_driver->y_positive == 0x00 ) {

            if( g_cur_v_start - wgml_fonts[g_curr_font_num].line_height
                    <= g_page_bottom ) {
                newpage = widow_check();
                if( !newpage ) {
                    finish_page();
                }
                p_char = NULL;
            }
        } else {
//          if( g_cur_v_start + wgml_fonts[g_curr_font_num].line_height
            if( g_cur_v_start >= g_page_bottom ) { // TBD
                newpage = widow_check();
                if( !newpage ) {
                    finish_page();
                }
                p_char = NULL;
            }
        }
    }
}


/***************************************************************************/
/*                                                                         */
/***************************************************************************/

void    process_line_full( text_line * a_line, bool justify )
{
    int32_t     widow;

    if( (a_line == NULL) || (a_line->first == NULL) ) { // why are we called?
        return;
    }
    if( !ProcFlags.prep_section ) {
        prepare_doc_sect( ProcFlags.doc_sect );
    }

    /***********************************************************************/
    /*  adjust vertical position from upper to lower border of line        */
    /***********************************************************************/
    if( bin_driver->y_positive == 0x00 ) {
        a_line->y_address -= a_line->line_height;
    } else {
        a_line->y_address += a_line->line_height;
    }
    g_cur_v_start = a_line->y_address;

    if( ProcFlags.test_widow ) {
        if( buf_lines == NULL ) {       // no buffered lines
            // if outside of widow area, reset widow test

            widow = g_cur_threshold * wgml_fonts[g_curr_font_num].line_height;

            if( bin_driver->y_positive == 0x00 ) {
                if( g_cur_v_start > g_page_bottom + widow ) {
                    ProcFlags.test_widow = false;
                }
            } else {
                if( g_cur_v_start < g_page_bottom + widow ) {
                    ProcFlags.test_widow = false;
                }
            }
        }
        if( ProcFlags.test_widow ) {    // inside widow area, buffer line
            if( justify && GlobalFlags.lastpass && !ProcFlags.literal
                                     && ProcFlags.justify > ju_off ) {
                do_justify( ju_x_start, g_page_right, a_line );
//              if( input_cbs->fmflags & II_research ) {// TBD
//                  test_out_t_line( a_line );
//              }
            }
            add_line_to_buf_lines( &buf_lines, a_line );
            buf_lines_cnt++;
        }
    }
    if( !ProcFlags.test_widow ) {
        if( GlobalFlags.lastpass ) {
            if( input_cbs->fmflags & II_research ) {
                test_out_t_line( a_line );
            }
            if( justify && !ProcFlags.literal && ProcFlags.justify > ju_off ) {
                do_justify( ju_x_start, g_page_right, a_line );
                if( input_cbs->fmflags & II_research ) {
                    test_out_t_line( a_line );
                }
            }
            fb_output_textline( a_line );
        }
        add_text_chars_to_pool( a_line );
    }
    a_line->first = NULL;
    ProcFlags.line_started = false;     // line is now empty
    ProcFlags.just_override = true;     // justify for following lines

    set_h_start();

    test_page_full();
}


/***************************************************************************/
/*  create a text_chars instance and fill it with a 'word'                 */
/***************************************************************************/

text_chars * process_word( char * pword, size_t count, uint8_t font_num )
{
    text_chars  *   n_char;

    n_char = alloc_text_chars( pword, count, font_num );
    intrans( n_char->text, &n_char->count, font_num );
    if( n_char->count == 0 ) {
        n_char->width = 0;
    } else {
        n_char->width = text_chars_width( n_char->text, n_char->count, font_num );
    }

    return( n_char );
}


/***************************************************************************/
/*  process text  (input line or part thereof)                             */
/*      if section start processing not yet done do it now                 */
/*      split into 'words'                                                 */
/*      translate if input translation active                              */
/*      calculate width with current font                                  */
/*      add text to output line                                            */
/*      handle line and page overflow conditions                           */
/***************************************************************************/

void    process_text( char * text, uint8_t font_num )
{
    text_chars          *   n_char;     // new text char
    size_t                  count;
    char                *   pword;
    char                *   p;
    static      bool        tabbing = false;
    static      text_type   typ = norm;
    static      text_type   typn = norm;

    p = text;
    if( t_line.first == NULL ) {    // first phrase in paragraph
        post_space = 0;
        if( ProcFlags.concat ) {    // ".co on": skip initial spaces
            while( *p == ' ' ) {
                p++;
            }
        } else {                    // ".co off": compute initial spacing
            while( *p == ' ' ) {
                post_space += wgml_fonts[font_num].spc_width;
                p++;
            }                    
        }
        ju_x_start = g_cur_h_start; // g_cur_h_start appears correct on entry
    } else {                        // subsequent phrase in paragraph
        if( ProcFlags.concat ) {    // ".co on"
            if( post_space == 0 ) {
                // compute initial spacing if needed; .ct may affect this
                if( (*p == ' ') || ((input_cbs->fmflags & II_sol) && \
                                            !ProcFlags.keep_left_margin)) {
                    post_space = wgml_fonts[font_num].spc_width;
                    if( is_stop_char( t_line.last->text[t_line.last->count - 1] ) ) {
                         post_space += wgml_fonts[font_num].spc_width;
                    }
                }
            }
            while( *p == ' ' ) {    // skip initial spaces
                p++;
            }                    
        } else {                    // ".co off": increment initial spacing
            while( *p == ' ' ) {
                post_space += wgml_fonts[font_num].spc_width;
                p++;
            }                    
        }
    }
    n_char = NULL;
    pword = p;                          // remember word start
    while( *p ) {
        if( *p == function_escape ) {   // special sub/superscript...
            switch( *(p + 1) ) {
            case function_subscript :   // start of subscript
                typn = sub;
                break;
            case function_superscript : // start of superscript
                typn = sup;
                break;
            case function_end:
            case function_sub_end:      // perhaps different action TBD
            case function_sup_end:
                typn = norm;
                break;
            default:
                out_msg( "gproctxt.c unknown function escape %#.02x\n",
                         *(p + 1) );
                wng_count++;
                show_include_stack();
                typn = norm;            // set normal mode TBD
            }
            if( p > pword ) {
                count = p - pword;      // no of bytes

                n_char = process_word( pword, count, font_num );
                n_char->type = typ;
//                n_char->t_flags = 0; // TBD
            }
            typ = typn;
            p += 2;
            pword = p;

        } else {                        // no function escape
            p++;
            if( *p ) {                  // process last word inside loop
                if( *p != ' ' ) {       // no space no word end
                    continue;
                }
                if( ProcFlags.in_trans && *(p - 1) == in_esc ) {
                    continue;           // guarded space no word end
                }
                if( !ProcFlags.concat ) { // .co off: include internal spaces
                    continue;
                }
            }
        }
        if( n_char == NULL ) {
            // can happen at end of phrase or after function_escape sequence
            if( p == pword ) { // avoid unwanted empty text_chars
                continue;
            }
            count = p - pword;          // no of bytes
            n_char = process_word( pword, count, font_num );
            n_char->type = typ;
//            n_char->t_flags = 0;      // TBD
            typ = typn;
        }
        // remove end-of-line spaces if .co off
        if( !ProcFlags.concat && (input_cbs->fmflags & II_eol) ) {
            while( n_char->text[--n_char->count] == ' ' );
            n_char->count++;
            n_char->width = text_chars_width( n_char->text, n_char->count, font_num );
        }
        g_cur_h_start += post_space;
        n_char->x_address = g_cur_h_start;
        if( post_space > 0 ) {
            tabbing = false;
            post_space = 0;
        }
        input_cbs->fmflags &= ~II_sol;  // no longer at start of line

        /***********************************************************/
        /*  Test if word exceeds right margin                      */
        /***********************************************************/

        if( !tabbing && (n_char->x_address + n_char->width > g_page_right) ) {
            process_line_full( &t_line, ProcFlags.concat
                                  && (ProcFlags.justify > ju_off) );
            p_char = NULL;
            if( !ProcFlags.page_started ) {
                document_new_page();    // page was full, start new one
                document_top_banner();
            }
            set_h_start();
            n_char->x_address = g_cur_h_start;
        }
        if( t_line.first == NULL ) {    // first element in output line
            calc_skip();
            test_page_full();
            if( !ProcFlags.top_ban_proc ) {
                document_new_page();
                document_top_banner();
            }
            t_line.first = n_char;
            t_line.y_address = g_cur_v_start;
            t_line.line_height = wgml_fonts[font_num].line_height;
            ju_x_start = n_char->x_address;
            ProcFlags.line_started = true;
        } else {
            p_char->next = n_char;
            n_char->prev = p_char;
            if( t_line.line_height < wgml_fonts[font_num].line_height ) {
                t_line.line_height = wgml_fonts[font_num].line_height;
            }
        }
        t_line.last = wgml_tabs( n_char );
        // tabbing is true if n_char contained at least one wgml tab
        if( n_char != t_line.last ) {
            tabbing = true;
        }
        p_char = t_line.last;

        g_cur_h_start = t_line.last->x_address + t_line.last->width;
        ProcFlags.page_started = true;

        // exit at end of text unless at end of input line
        if( !(input_cbs->fmflags & II_eol) && !*p ) {
            break;
        }
        post_space = wgml_fonts[font_num].spc_width;
        if( is_stop_char( t_line.last->text[t_line.last->count - 1] ) ) {
             post_space += wgml_fonts[font_num].spc_width;
        }
        if( ProcFlags.concat ) {     // ignore multiple blanks in concat mode
            if( *p == ' ' ) {
                while( *p == ' ' ) {
                    p++;
                }
            }
            p--;
        }
        pword = p + 1;               // new word start or end of input record
        n_char = NULL;
    }
    if( t_line.first != NULL ) {        // something in the line
        ProcFlags.page_started = true;

        if( !ProcFlags.concat ) {
            if( input_cbs->fmflags & II_eol ) {
                scr_process_break();    // TBD
                p_char = NULL;
#if 0
                process_line_full( &t_line, (ProcFlags.justify > ju_off) );
                p_char = NULL;
                if( !ProcFlags.page_started ) {
                    document_new_page();
                    document_top_banner();
                }
                set_h_start();
#endif
            }
        }
    }
}

