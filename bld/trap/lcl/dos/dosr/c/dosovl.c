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


#include <string.h>
#include <i86.h>
#include "trpimp.h"

typedef addr32_ptr      dos_addr;

#include "ovltab.h"
#include "ovldbg.h"

extern unsigned_8       RunProg( trap_cpu_regs *, trap_cpu_regs * );
extern void             SetDbgTask( void );
extern void             SetUsrTask( void );
extern void             far OvlTrap( int );

extern trap_cpu_regs    TaskRegs;

static unsigned         OvlStateSize;
static int              (far *OvlRequest)( int, void far * );

static int far NoOvlsHdlr( int req, void far *data )
/**************************************************/
{
    return( 0 );
}

void NullOvlHdlr( void )
/**********************/
{
    OvlRequest = &NoOvlsHdlr;
}

#if 1 //support for the 9.5 overlay manager, remove at a later date.
static struct ovl_header        far *Hdr;
#endif

bool CheckOvl( addr32_ptr start )
/*******************************/
{
    struct ovl_header   far *hdr;

    hdr = MK_FP( start.segment, start.offset );
    if( hdr->signature == OVL_SIGNATURE ) {
#if 1 //support for the 9.5 overlay manager, remove at a later date.
        Hdr = hdr;
#endif
        hdr->hook = &OvlTrap;
        OvlRequest = MK_FP( start.segment, hdr->handler_offset );
        RunProg( &TaskRegs, &TaskRegs ); /* init overlay manager */
        return( TRUE );
    }
    return( FALSE );
}

unsigned ReqOvl_state_size( void )
/********************************/
{
    ovl_state_size_ret  *ret;

    ret = GetOutPtr( 0 );
    ret->size = OvlRequest( OVLDBG_GET_STATE_SIZE, 0 );
    OvlStateSize = ret->size;
    return( sizeof( *ret ) );
}

unsigned ReqOvl_read_state( void )
/********************************/
{
    OvlRequest( OVLDBG_GET_OVERLAY_STATE, GetOutPtr( 0 ) );
    return( OvlStateSize );
}

unsigned ReqOvl_write_state( void )
/*********************************/
{
    SetUsrTask(); /* overlay manager needs access to its file table */
    OvlRequest( OVLDBG_SET_OVERLAY_STATE, GetInPtr( sizeof( ovl_write_state_req ) ) );
    SetDbgTask();
    return( 0 );
}

unsigned ReqOvl_trans_vect_addr( void )
/*************************************/
{
    ovl_trans_vect_addr_req     *acc;
    ovl_trans_vect_addr_ret     *ret;

    acc = GetInPtr( 0 );
    ret = GetOutPtr( 0 );
    if( OvlRequest( OVLDBG_TRANSLATE_VECTOR_ADDR, &acc->ovl_addr ) ) {
        ret->ovl_addr = acc->ovl_addr;
    } else {
        ret->ovl_addr.sect_id = 0;
    }
    return( sizeof( *ret ) );
}

unsigned ReqOvl_trans_ret_addr( void )
/************************************/
{
    ovl_trans_ret_addr_req      *acc;
    ovl_trans_ret_addr_ret      *ret;

    acc = GetInPtr( 0 );
    ret = GetOutPtr( 0 );
    if( OvlRequest( OVLDBG_TRANSLATE_RETURN_ADDR, &acc->ovl_addr ) ) {
        ret->ovl_addr = acc->ovl_addr;
    } else {
        ret->ovl_addr.sect_id = 0;
    }
    return( sizeof( *ret ) );
}

unsigned ReqOvl_get_remap_entry( void )
/*************************************/
{
    ovl_get_remap_entry_req     *acc;
    ovl_get_remap_entry_ret     *ret;

    acc = GetInPtr( 0 );
    ret = GetOutPtr( 0 );
    ret->remapped = OvlRequest( OVLDBG_GET_MOVED_SECTION, &acc->ovl_addr );
    ret->ovl_addr = acc->ovl_addr;
    return( sizeof( *ret ) );
}

unsigned ReqOvl_get_data( void )
/******************************/
{
    ovl_get_data_req    *acc;
    ovl_get_data_ret    *ret;
    ovl_address         addr;

    acc = GetInPtr( 0 );
    ret = GetOutPtr( 0 );
    addr.sect_id = acc->sect_id;
    if( !OvlRequest( OVLDBG_GET_SECTION_DATA, &addr ) ) {
        addr.mach.segment = 0;
    }
#if 1 //support for the 9.5 overlay manager, remove at a later date.
    {
        ovl_table       *tbl;
        unsigned        num_sects;
        ovltab_entry    *curr;

        if( addr.mach.segment == 0 ) {
            OvlRequest( OVLDBG_GET_OVL_TBL_ADDR, &tbl );
            if( ( tbl->prolog.major == OVL_MAJOR_VERSION )
                && ( tbl->prolog.minor == OVL_MINOR_VERSION ) ) {
                num_sects = 0;
                for( curr = &tbl->entries[0]; curr->flags_anc != OVLTAB_TERMINATOR; ++curr ) {
                    ++num_sects;
                    if( num_sects > addr.sect_id ) {
                        break;
                    }
                }
                if( addr.sect_id <= num_sects ) {
                    addr.mach.segment = tbl->entries[addr.sect_id-1].code_handle;
                    if( addr.mach.segment == 0 )
                        addr.mach.segment = Hdr->dyn_area;
                    addr.sect_id = tbl->entries[addr.sect_id-1].num_paras;
                }
            }
        }
    }
#endif
    ret->segment = addr.mach.segment;
    ret->size = addr.sect_id * 16UL;
    return( sizeof( *ret ) );
}
