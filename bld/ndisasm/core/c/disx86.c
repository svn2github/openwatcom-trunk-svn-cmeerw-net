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
#include <ctype.h>
#include "distypes.h"
#include "dis.h"

extern long SEX( unsigned long v, unsigned bit );

extern const dis_range          X86RangeTable[];
extern const unsigned char      X86MaxInsName;
extern const dis_ins_descript   DisInstructionTable[];
extern const unsigned short     DisRefTypeTable[];

#if DISCPU & DISCPU_x86

typedef union {
    unsigned_8 full;
    struct {
        unsigned_8  base  : 3;
        unsigned_8  index : 3;
        unsigned_8  scale : 2;
    } split;
} SIB;


/*=====================================================================*/
/*                        ENUMERATED TYPES                             */
/*=====================================================================*/
typedef enum {
    MOD_0 = 0x0,
    MOD_1 = 0x1,
    MOD_2 = 0X2,
    MOD_3 = 0X3
} MOD;

typedef enum {
    RM_0 = 0x0,
    RM_1 = 0x1,
    RM_2 = 0X2,
    RM_3 = 0X3,
    RM_4 = 0x4,
    RM_5 = 0X5,
    RM_6 = 0X6,
    RM_7 = 0X7,
    REG_AX = 0x0,
    REG_CX = 0X1,
    REG_DX = 0x2,
    REG_BX = 0x3,
    REG_SP = 0x4,
    REG_AH = 0x4,
    REG_BP = 0x5,
    REG_CH = 0x5,
    REG_SI = 0X6,
    REG_DH = 0x6,
    REG_DI = 0x7,
    REG_BH = 0x7,
    REG_CR0 = 0x0,
    REG_CR2 = 0X2,
    REG_CR3 = 0X3,
    REG_CR4 = 0X4,
    REG_DR0 = 0X0,
    REG_DR1 = 0x1,
    REG_DR2 = 0x2,
    REG_DR3 = 0x3,
    REG_DR6 = 0x6,
    REG_DR7 = 0x7,
    REG_TR3 = 0x3,
    REG_TR4 = 0x4,
    REG_TR5 = 0x5,
    REG_TR6 = 0x6,
    REG_TR7 = 0x7,
    REG_ES  = 0x0,
    REG_CS  = 0x1,
    REG_SS  = 0x2,
    REG_DS  = 0x3,
    REG_FS  = 0x4,
    REG_GS  = 0x5
} RM;

typedef enum {
    SC_0 = 0x0,
    SC_1 = 0x1,
    SC_2 = 0X2,
    SC_3 = 0X3
} SCALE;

typedef enum {
    W_DEFAULT = 0x1,
    W_FULL    = 0x1,    // Full Size Operand
    W_BYTE    = 0x0     // Byte Size Operand
} WBIT;

typedef enum {
    S_DEFAULT = 0x0,
    S_FULL    = 0x0,    // Full Mode for Immediate Value Fetch
    S_BYTE    = 0x1     // Byte Mode for Immediate Value Fetch
} SBIT;

typedef enum {
    MF_0 = 0x0,
    MF_1 = 0x1,
    MF_2 = 0x2,
    MF_3 = 0X3
} MF_BITS;

typedef enum {
    FB    = 0x0,
    FE    = 0x1,
    FBE   = 0x2,
    FU    = 0x3,
} COND;

/*=====================================================================*/
/*                FUNCTIONS TO FETCH BYTE/WORD/DWORD                   */
/*=====================================================================*/



static unsigned_8 GetUByte( void *d, unsigned off )
/**************************************************
 * Get Unsigned Byte
 */
{
    unsigned_8  data;

    //NYI: have to error check return code
    DisCliGetData( d, off, sizeof(data), &data );
    return( data );
}

static unsigned_16 GetUShort( void *d, unsigned off )
/****************************************************
 * Get Unsigned Word ( 2 bytes )
 */
{
    struct {
        unsigned_8 lo;
        unsigned_8 hi;
    }   data;

    //NYI: have to error check return code
    DisCliGetData( d, off, sizeof(data), &data );
    return( (data.hi << 8) | data.lo );
}

static unsigned_32 GetULong( void *d, unsigned off )
/***************************************************
 * Get Double Word
 */
{
    struct {
        unsigned_8 lo;
        unsigned_8 mid_lo;
        unsigned_8 mid_hi;
        unsigned_8 hi;
    }   data;

    //NYI: have to error check return code
    DisCliGetData( d, off, sizeof(data), &data );
    return( ((unsigned long)data.hi << 24)
        |   ((unsigned long)data.mid_hi << 16)
        |   (data.mid_lo << 8)
        |    data.lo );
}


static int GetSByte( void *d, unsigned off )
/*******************************************
 * Get Signed Byte
 */
{
    return( SEX( GetUByte( d, off ), 7 ) );
}

static int GetSShort( void *d, unsigned off )
/********************************************
 * Get Signed Word
 */
{
    return( SEX( GetUShort( d, off ), 15 ) );
}



/*=====================================================================*/
/*                PREFIX HANDLERS                                      */
/*=====================================================================*/



dis_handler_return X86PrefixFwait( dis_handle *h, void *d, dis_dec_ins *ins )
/****************************************************************************
 * FWait
 */
{
    unsigned    code;
    unsigned    instruct_size;

    ins->size += 1;
    instruct_size = ins->size;

    for( ;; ) {
        code = GetUByte( d, instruct_size );
        if( (code & 0xf8) == 0xd8 ) break;
        // Look Ahead for Prefixes
        switch( code ) {
        case 0x67:
        case 0x66:
        case 0x2e:
        case 0x3e:
        case 0x26:
        case 0x64:
        case 0x65:
        case 0x36:
            break;
        default:
            // A non-floating point instruction is found so quit
            ins->num_ops = 0;
            ins->type = DI_X86_fwait;
            return( DHR_DONE );
        }
        ++instruct_size;
    }
    ins->flags ^= DIF_X86_FWAIT;
    return( DHR_CONTINUE );
}

dis_handler_return X86PrefixOpnd( dis_handle *h, void *d, dis_dec_ins *ins )
/***************************************************************************
 * Operand Size Override
 */
{
    ins->size += 1;
    ins->flags ^= (DIF_X86_OPND_LONG | DIF_X86_OPND_SIZE);
    return( DHR_CONTINUE );
}

dis_handler_return X86PrefixAddr( dis_handle *h, void *d, dis_dec_ins *ins )
/***************************************************************************
 * Address Size Override
 */
{
    ins->size += 1;
    ins->flags ^= (DIF_X86_ADDR_LONG | DIF_X86_ADDR_SIZE);
    return( DHR_CONTINUE );
}

dis_handler_return X86PrefixRepe( dis_handle *h, void *d, dis_dec_ins *ins )
/***************************************************************************
 * REP/REPE/REPZ Override
 */
{
    ins->size += 1;
    ins->flags ^= DIF_X86_REPE;
    return( DHR_CONTINUE );
}

dis_handler_return X86PrefixRepne( dis_handle *h, void *d, dis_dec_ins *ins )
/****************************************************************************
 * REPNE/REPNZ
 */
{
    ins->size += 1;
    ins->flags ^= DIF_X86_REPNE;
    return( DHR_CONTINUE );
}

dis_handler_return X86PrefixLock( dis_handle *h, void *d, dis_dec_ins *ins )
/***************************************************************************
 * Lock Prefix
 */
{
    ins->size += 1;
    ins->flags ^= DIF_X86_LOCK;
    return( DHR_CONTINUE );
}

dis_handler_return X86PrefixCS( dis_handle *h, void *d, dis_dec_ins *ins )
/*************************************************************************
 * CS Segment Override
 */
{
    ins->size += 1;
    ins->flags ^= DIF_X86_CS;
    return( DHR_CONTINUE );
}

dis_handler_return X86PrefixSS( dis_handle *h, void *d, dis_dec_ins *ins )
/*************************************************************************
 * SS Segment Override
 */
{
    ins->size += 1;
    ins->flags ^= DIF_X86_SS;
    return( DHR_CONTINUE );
}

dis_handler_return X86PrefixDS( dis_handle *h, void *d, dis_dec_ins *ins )
/*************************************************************************
 * DS Segment Override
 */
{
    ins->size += 1;
    ins->flags ^= DIF_X86_DS;
    return( DHR_CONTINUE );
}

dis_handler_return X86PrefixES( dis_handle *h, void *d, dis_dec_ins *ins )
/*************************************************************************
 * ES Segment Override
 */
{
    ins->size += 1;
    ins->flags ^= DIF_X86_ES;
    return( DHR_CONTINUE );
}

dis_handler_return X86PrefixFS( dis_handle *h, void *d, dis_dec_ins *ins )
/*************************************************************************
 * FS Segment Override
 */
{
    ins->size += 1;
    ins->flags ^= DIF_X86_FS;
    return( DHR_CONTINUE );
}

dis_handler_return X86PrefixGS( dis_handle *h, void *d , dis_dec_ins *ins )
/**************************************************************************
 * GS Segment Override
 */
{
    ins->size += 1;
    ins->flags ^= DIF_X86_GS;
    return( DHR_CONTINUE );
}



/*=====================================================================*/
/*                UTILITY FUNCTIONS                                    */
/*=====================================================================*/



/*
 * Returns true if a segment override prefix has been encountered
 */
#define SEGOVER (DIF_X86_CS|DIF_X86_DS|DIF_X86_ES|DIF_X86_FS|DIF_X86_GS|DIF_X86_SS)
#define X86SegmentOverride( ins )       ((ins)->flags & SEGOVER)

dis_register X86GetRegister_D( WBIT w, RM reg, dis_dec_ins *ins )
{
    switch(reg) {
    case REG_AX: return(DR_X86_eax);
    case REG_CX: return(DR_X86_ecx);
    case REG_DX: return(DR_X86_edx);
    case REG_BX: return(DR_X86_ebx);
    case REG_SP: return(DR_X86_esp);
    case REG_BP: return(DR_X86_ebp);
    case REG_SI: return(DR_X86_esi);
    case REG_DI: return(DR_X86_edi);
    default:     return(DR_NONE);
    }
}

dis_register X86GetRegister_W( WBIT w, RM reg, dis_dec_ins *ins )
{
    switch(reg) {
    case REG_AX: return(DR_X86_ax);
    case REG_CX: return(DR_X86_cx);
    case REG_DX: return(DR_X86_dx);
    case REG_BX: return(DR_X86_bx);
    case REG_SP: return(DR_X86_sp);
    case REG_BP: return(DR_X86_bp);
    case REG_SI: return(DR_X86_si);
    case REG_DI: return(DR_X86_di);
    default:     return(DR_NONE);
    }
}

dis_register X86GetRegister_B( WBIT w, RM reg, dis_dec_ins *ins )
{
    switch(reg) {
    case REG_AX: return(DR_X86_al);
    case REG_CX: return(DR_X86_cl);
    case REG_DX: return(DR_X86_dl);
    case REG_BX: return(DR_X86_bl);
    case REG_AH: return(DR_X86_ah);
    case REG_CH: return(DR_X86_ch);
    case REG_DH: return(DR_X86_dh);
    case REG_BH: return(DR_X86_bh);
    default:     return(DR_NONE);
    }
}

dis_register X86FGetSTReg(WBIT w, RM reg, dis_dec_ins *ins )
/**********************************************************************
 *  Get ST  (Floating Point) Register  w - not used
 */
{
    switch( reg ) {
    case RM_0: return( DR_X86_st  );
    case RM_1: return( DR_X86_st1 );
    case RM_2: return( DR_X86_st2 );
    case RM_3: return( DR_X86_st3 );
    case RM_4: return( DR_X86_st4 );
    case RM_5: return( DR_X86_st5 );
    case RM_6: return( DR_X86_st6 );
    case RM_7: return( DR_X86_st7 );
    default:   return( DR_NONE );
    }
}

dis_register X86GetMMReg(WBIT w, RM reg, dis_dec_ins *ins )
/**********************************************************************
 *  Get MM  (Multimedia) Register  w - not used
 */
{
    switch( reg ) {
    case RM_0: return( DR_X86_mm0 );
    case RM_1: return( DR_X86_mm1 );
    case RM_2: return( DR_X86_mm2 );
    case RM_3: return( DR_X86_mm3 );
    case RM_4: return( DR_X86_mm4 );
    case RM_5: return( DR_X86_mm5 );
    case RM_6: return( DR_X86_mm6 );
    case RM_7: return( DR_X86_mm7 );
    default:   return( DR_NONE );
    }
}

dis_register X86GetCRegister( WBIT w, RM reg, dis_dec_ins *ins )
/**********************************************************************
 *  Get Control Register
 */
{
    switch( reg ) {
    case REG_CR0: return( DR_X86_cr0 );
    case REG_CR2: return( DR_X86_cr2 );
    case REG_CR3: return( DR_X86_cr3 );
    case REG_CR4: return( DR_X86_cr4 );
    default     : return( DR_NONE );
    }
}

dis_register X86GetDRegister( WBIT w, RM reg, dis_dec_ins *ins )
/**********************************************************************
 *  Get Debug Register
 */
{
    switch( reg ) {
    case REG_DR0: return( DR_X86_dr0 );
    case REG_DR1: return( DR_X86_dr1 );
    case REG_DR2: return( DR_X86_dr2 );
    case REG_DR3: return( DR_X86_dr3 );
    case REG_DR6: return( DR_X86_dr6 );
    case REG_DR7: return( DR_X86_dr7 );
    default     : return( DR_NONE );
    }
}

dis_register X86GetTRegister( WBIT w, RM reg, dis_dec_ins *ins )
/**********************************************************************
 *  Get Test Register 80486
 */
{
    switch( reg ) {
    case REG_TR3: return( DR_X86_tr3 );
    case REG_TR4: return( DR_X86_tr4 );
    case REG_TR5: return( DR_X86_tr5 );
    case REG_TR6: return( DR_X86_tr6 );
    case REG_TR7: return( DR_X86_tr7 );
    default     : return( DR_NONE );
    }
}

dis_register X86GetSRegister( WBIT w, RM reg, dis_dec_ins *ins )
/**********************************************************************
 *  Get Segment Register
 */
{
    switch( reg ) {
    case REG_ES: return( DR_X86_es );
    case REG_CS: return( DR_X86_cs );
    case REG_SS: return( DR_X86_ss );
    case REG_DS: return( DR_X86_ds );
    case REG_FS: return( DR_X86_fs );
    case REG_GS: return( DR_X86_gs );
    default     : return( DR_NONE );
    }
}

dis_register X86GetRegister( WBIT w, RM reg, dis_dec_ins *ins )
/**********************************************************************
 *  Get Register
 *                 w   =  1  (default) use full width of operand
 *                     =  0            use byte size operand
 */
{
    switch(ins->type) {
    case DI_X86_lldt:
    case DI_X86_sldt:
    case DI_X86_ltr:
    case DI_X86_lmsw:
    case DI_X86_smsw:
    case DI_X86_str:
    case DI_X86_verr:
    case DI_X86_verw:
    case DI_X86_arpl:
        return( X86GetRegister_W( w, reg, ins ) );
    case DI_X86_bswap:
        return( X86GetRegister_D( w, reg, ins ) );
    default:
        if( w != W_FULL ) {
            return( X86GetRegister_B ( w, reg, ins ) );
        } else if( DIF_X86_OPND_LONG & ins->flags ) {
            return( X86GetRegister_D( w, reg, ins ) );
        } else {
            return( X86GetRegister_W( w, reg, ins ) );
        }
    }
}


static void X86GetModRM_S(WBIT w, MOD mod, RM rm, void * d,
                           dis_dec_ins *ins, dis_ref_type ref_type,
           dis_register (*func) (WBIT w,RM reg, dis_dec_ins* ins))
/**********************************************************************
 *  Get Mod RM  - 16-Bit Operand Version
 */
{
    int oper;

    oper = ins->num_ops;
    ++ins->num_ops;

    ins->op[oper].type = DO_MEMORY_ABS;
    ins->op[oper].ref_type = ref_type;
    ins->op[oper].value = 0;
    ins->op[oper].scale = 1;

    switch(rm)  {
    case RM_0:
        ins->op[oper].base  = DR_X86_bx;
        ins->op[oper].index = DR_X86_si;
        break;
    case RM_1:
        ins->op[oper].base  = DR_X86_bx;
        ins->op[oper].index = DR_X86_di;
        break;
    case RM_2:
        ins->op[oper].base  = DR_X86_bp;
        ins->op[oper].index = DR_X86_si;
        break;
    case RM_3:
        ins->op[oper].base  = DR_X86_bp;
        ins->op[oper].index = DR_X86_di;
        break;
    case RM_4:
        ins->op[oper].base  = DR_X86_si;
        ins->op[oper].index = DR_NONE;
        break;
    case RM_5:
        ins->op[oper].base  = DR_X86_di;
        ins->op[oper].index = DR_NONE;
        break;
    case RM_6:
        ins->op[oper].base  = DR_X86_bp;
        ins->op[oper].index = DR_NONE;
        break;
    case RM_7:
        ins->op[oper].base  = DR_X86_bx;
        ins->op[oper].index = DR_NONE;
        break;
    }

    switch( mod ) {
    case MOD_0:
        if(rm == RM_6) {
            ins->op[oper].base = DR_NONE;
            ins->op[oper].op_position = ins->size;
            ins->op[oper].value = GetSShort(d,ins->size);
            ins->size += 2;
        }
        break;
    case MOD_1:
        ins->op[oper].op_position = ins->size;
        ins->op[oper].value = GetSByte(d,ins->size);
        ins->size += 1;
        break;
    case MOD_2:
        ins->op[oper].op_position = ins->size;
        ins->op[oper].value = GetSShort(d,ins->size);
        ins->size += 2;
        break;
    case MOD_3:
        ins->op[oper].type = DO_REG;
        ins->op[oper].base = func(w,rm,ins);
        ins->op[oper].index = DR_NONE;
        break;
    }
}


static void X86GetSIB(unsigned_8 scale, RM base, RM index,
                       dis_dec_ins *ins, unsigned oper)
/**********************************************************************
 * Get SIB - only used in 32-Bit Operand Mode
 */
{
    ins->op[oper].base = X86GetRegister_D( W_DEFAULT, base, ins );
    if( index == RM_4 ) {
        ins->op[oper].scale = 1;
        ins->op[oper].index = DR_NONE;
    } else {
        ins->op[oper].scale = 1 << scale;
        ins->op[oper].index = X86GetRegister_D( W_DEFAULT, index, ins );
    }
}

static void X86GetModRM_L(WBIT w, MOD mod, RM rm, void * d,
                           dis_dec_ins *ins, dis_ref_type ref_type,
           dis_register (*func) (WBIT w,RM reg, dis_dec_ins* ins)) {
/**********************************************************************
 *   Get MOD RM - 32-Bit Version
 */

    SIB sib;
    int oper;

    oper = ins->num_ops;
    ++ins->num_ops;

    ins->op[oper].type = DO_MEMORY_ABS;
    ins->op[oper].ref_type = ref_type;
    ins->op[oper].value = 0;
    ins->op[oper].scale = 1;
    ins->op[oper].index = DR_NONE;
    ins->op[oper].base  = DR_NONE;

    switch( rm )  {
    case RM_0:
        ins->op[oper].base  = DR_X86_eax;
        break;
    case RM_1:
        ins->op[oper].base  = DR_X86_ecx;
        break;
    case RM_2:
        ins->op[oper].base  = DR_X86_edx;
        break;
    case RM_3:
        ins->op[oper].base  = DR_X86_ebx;
        break;
    case RM_4:
        if( mod != MOD_3) {
            sib.full = GetUByte(d,ins->size);
            ins->size += 1;
            X86GetSIB(sib.split.scale,
                       sib.split.base,
                       sib.split.index,
                       ins,
                       oper);
            if( mod == MOD_0 ) {
                if( ins->op[oper].base == DR_X86_ebp ) {
                    ins->op[oper].base = DR_NONE;
                    ins->op[oper].op_position = ins->size;
                    ins->op[oper].value = GetULong(d,ins->size);
                    ins->size += 4;
                }
            }
        }
        break;
    case RM_5:
        ins->op[oper].base  = DR_X86_ebp;
        break;
    case RM_6:
        ins->op[oper].base  = DR_X86_esi;
        break;
    case RM_7:
        ins->op[oper].base  = DR_X86_edi;
        break;
    }

    switch( mod ) {
    case MOD_0:
        if( rm == RM_5 ) {
            ins->op[oper].base = DR_NONE;
            ins->op[oper].op_position = ins->size;
            ins->op[oper].value = GetULong(d,ins->size);
            ins->size += 4;
        }
        break;
    case MOD_1:
        ins->op[oper].op_position = ins->size;
        ins->op[oper].value = GetSByte(d,ins->size);
        ins->size += 1;
        break;
    case MOD_2:
        ins->op[oper].op_position = ins->size;
        ins->op[oper].value = GetULong(d,ins->size);
        ins->size += 4;
        break;
    case MOD_3:
        ins->op[oper].type = DO_REG;
        ins->op[oper].base = func(w,rm,ins);
        break;
    }
}

/*=====================================================================*/
/*               Get MOD/RM as Operand                                 */
/*=====================================================================*/
static void X86GetModRM(WBIT w, MOD mod, RM rm, void * d,
                         dis_dec_ins *ins, dis_ref_type ref_type)
/**********************************************************************
 */
{
    if( DIF_X86_ADDR_LONG & ins->flags ) {
        X86GetModRM_L(w, mod, rm, d ,ins, ref_type, X86GetRegister);
    } else {
        X86GetModRM_S(w, mod, rm, d ,ins, ref_type, X86GetRegister);
    }
}

static void X86GetModRM_W(WBIT w, MOD mod, RM rm, void * d,
                         dis_dec_ins *ins)
/**********************************************************************
 * 16-Bit Operand Version
 */
{
    if( DIF_X86_ADDR_LONG & ins->flags ) {
        X86GetModRM_L(w, mod, rm, d ,ins, DRT_X86_WORD, X86GetRegister_W);
    } else {
        X86GetModRM_S(w, mod, rm, d ,ins, DRT_X86_WORD, X86GetRegister_W);
    }
}

static void X86GetModRM_B(WBIT w, MOD mod, RM rm, void * d,
                         dis_dec_ins *ins)
/**********************************************************************
 * 8-Bit Operand Version
 */
{
    if( DIF_X86_ADDR_LONG & ins->flags ) {
        X86GetModRM_L(w, mod, rm, d ,ins, DRT_X86_BYTE, X86GetRegister_B);
    } else {
        X86GetModRM_S(w, mod, rm, d ,ins, DRT_X86_BYTE, X86GetRegister_B);
    }
}

static void X86FGetModRM(WBIT w, MOD mod, RM rm, void * d,
                         dis_dec_ins *ins, dis_ref_type ref_type)
/**********************************************************************
 * Gets the Floating Point MOD/RM
 * NOT the same with X86GetModRM it returns ST instead of a register
 * if MOD = MOD_3
 */
{
    if( DIF_X86_ADDR_LONG & ins->flags ) {
        X86GetModRM_L(w, mod, rm, d ,ins, ref_type, X86FGetSTReg);
    } else {
        X86GetModRM_S(w, mod, rm, d ,ins, ref_type, X86FGetSTReg);
    }
}

static void X86GetMMModRM(WBIT w, MOD mod, RM rm, void * d,
                         dis_dec_ins *ins, dis_ref_type ref_type)
/**********************************************************************
 * Gets the Floating Point MOD/RM
 * NOT the same with X86GetModRM it returns MM instead of a register
 * if MOD = MOD_3
 */
{
    if( DIF_X86_ADDR_LONG & ins->flags ) {
        X86GetModRM_L(w, mod, rm, d ,ins, ref_type, X86GetMMReg);
    } else {
        X86GetModRM_S(w, mod, rm, d ,ins, ref_type, X86GetMMReg);
    }
}

/*=====================================================================*/
/*               Get Register as Operands                              */
/*=====================================================================*/

static dis_ref_type X86RegRefType( dis_register reg )
{
    if( reg <= DR_X86_bl ) return( DRT_X86_BYTE );
    if( reg <= DR_X86_di ) return( DRT_X86_WORD );
    if( reg <= DR_X86_edi ) return( DRT_X86_DWORD );
    if( reg <= DR_X86_st7 ) return( DRT_X86_TBYTE );
    if( reg <= DR_X86_mm7 ) return( DRT_X86_MM64 );
    if( reg <= DR_X86_dr7 ) return( DRT_X86_DWORD );
    return( DRT_X86_WORD );
}

static void X86FGetST( RM reg, dis_dec_ins *ins )
/**********************************************************************
 *  Get ST - Floating Point Stack Register
 */
{
    int oper;

    oper = ins->num_ops;
    ins->op[oper].type = DO_REG;
    ins->op[oper].base = X86FGetSTReg( W_DEFAULT, reg, ins );
    ins->op[oper].ref_type = DRT_X86_TBYTE;
    ++ins->num_ops;
}

static void X86GetMM( RM reg, dis_dec_ins *ins )
/**********************************************************************
 *  Get MM  Register
 */
{
    int oper;

    oper = ins->num_ops;
    ins->op[oper].type = DO_REG;
    ins->op[oper].base = X86GetMMReg( W_DEFAULT, reg, ins );
    ins->op[oper].ref_type = DRT_X86_MM64;
    ++ins->num_ops;
}

static void X86GetReg( WBIT w, RM reg, dis_dec_ins *ins )
/**********************************************************************
 *  Get Register
 */
{
    int oper;

    oper = ins->num_ops;
    ins->op[oper].type = DO_REG;
    ins->op[oper].base = X86GetRegister( w, reg, ins );
    ins->op[oper].ref_type = X86RegRefType( ins->op[oper].base );
    ++ins->num_ops;
}

static void X86GetSReg( WBIT w, RM reg, dis_dec_ins *ins )
/**********************************************************************
 *  Get S Registers
 */
{
    int oper;

    oper = ins->num_ops;
    ins->op[oper].type = DO_REG;
    ins->op[oper].base = X86GetSRegister( w, reg, ins );
    ins->op[oper].ref_type = DRT_X86_WORD;
    ++ins->num_ops;
}

/*=====================================================================*/
/*               Get Immediate Value                                   */
/*=====================================================================*/


static void X86GetImmedVal( SBIT s, WBIT w, void *d, dis_dec_ins *ins )
/*********************************************************************
 * Get Immediate Value
 *                   s = 0  (S_FULL / S_DEFAULT)
 *                   s = 1  (S_BYTE)
 */
{
    int oper;

    oper                      = ins->num_ops;
    ins->op[oper].op_position = ins->size;
    ins->op[oper].type        = DO_IMMED;
    ++ins->num_ops;

    if( w == W_FULL && !s ) {
        if( ins->flags & DIF_X86_OPND_LONG ) {
            ins->op[oper].value = GetULong(d,ins->size);
            ins->size   += 4;
        } else {
            ins->op[oper].value = GetSShort(d,ins->size);
            ins->size   += 2;
        }
    } else {
        ins->op[oper].value = GetSByte(d,ins->size);
        ins->size   += 1;
    }
}

static void X86GetUImmedVal( SBIT s, WBIT w, void *d, dis_dec_ins *ins )
/*********************************************************************
 * Get Unsigned Immediate Value
 *                   s = 0  (S_FULL / S_DEFAULT)
 *                   s = 1  (S_BYTE)
 */
{
    int oper;

    oper                      = ins->num_ops;
    ins->op[oper].op_position = ins->size;
    ins->op[oper].type        = DO_IMMED;
    ++ins->num_ops;

    if( w == W_FULL && !s ) {
        if(ins->flags & DIF_X86_OPND_LONG) {
            ins->op[oper].value = GetULong(d,ins->size);
            ins->size   += 4;
        } else {
            ins->op[oper].value = GetUShort(d,ins->size);
            ins->size   += 2;
        }
    } else {
        ins->op[oper].value = GetUByte(d,ins->size);
        ins->size   += 1;
    }
}

/*=====================================================================*/
/*               Get Absolute Value                                    */
/*=====================================================================*/

static void X86GetAbsVal( void *d, dis_dec_ins *ins )
/*********************************************************************
 * Get Absolute Value
 */
{
    int oper;

    oper = ins->num_ops;
    ins->op[oper].op_position = ins->size;
    ins->op[oper].type = DO_ABSOLUTE;
    ++ins->num_ops;
    if( ins->flags & DIF_X86_ADDR_LONG ) {
        ins->op[oper].value = GetULong(d,ins->size);
        ins->size += 4;
    } else {
        ins->op[oper].value = GetSShort(d,ins->size);
        ins->size   += 2;
    }

    // Get Segment Value
    ins->op[oper].extra = GetSShort( d, ins->size );
    ins->size += 2;
    ins->op[oper].type |= DO_EXTRA;

}

/*=====================================================================*/
/*               Get Relative Value                                    */
/*=====================================================================*/

static void X86GetRelVal_8( void *d, dis_dec_ins *ins )
/*********************************************************************
 * Get Relative Value   - 1 byte
 */
{
    int oper;

    oper = ins->num_ops;
    ins->op[oper].op_position = ins->size;
    ins->op[oper].type = DO_RELATIVE;
    ++ins->num_ops;
    ins->op[oper].value = GetSByte(d,ins->size);
    ins->size += 1;
    ins->op[oper].value += ins->size;
}

static void X86GetRelVal( void *d, dis_dec_ins *ins )
/*********************************************************************
 * Get Relative Value
 */
{
    int oper;

    oper = ins->num_ops;
    ins->op[oper].op_position = ins->size;
    ins->op[oper].type = DO_RELATIVE;
    ++ins->num_ops;
    if( ins->flags & DIF_X86_ADDR_LONG ) {
        ins->op[oper].value = GetULong(d,ins->size);
        ins->size += 4;
    } else {
        ins->op[oper].value = GetSShort(d,ins->size);
        ins->size += 2;
    }
    ins->op[oper].value += ins->size;
}

/*=====================================================================*/
/*               Get Reference Type                                    */
/*=====================================================================*/

dis_ref_type  X86GetRefType( WBIT w, dis_dec_ins *ins )
/*********************************************************************
 * Get Reference Type
 */
{
    // Instructions that have default ref type
    switch( ins->type ) {
    case DI_X86_lldt:
    case DI_X86_sldt:
    case DI_X86_ltr:
    case DI_X86_lmsw:
    case DI_X86_smsw:
    case DI_X86_str:
    case DI_X86_invlpg:
    case DI_X86_verr:
    case DI_X86_verw:
    case DI_X86_arpl:
        return( DRT_X86_WORD );
    case DI_X86_cmpxchg8b:
        return( DRT_X86_QWORD );
    }

    if( w == W_FULL ) {
        if( ins->flags & DIF_X86_OPND_LONG ) {
            switch( ins->type ) {
            case DI_X86_lds:
            case DI_X86_les:
            case DI_X86_lfs:
            case DI_X86_lgs:
            case DI_X86_lss:
            case DI_X86_call4:
            case DI_X86_jmp4:
                return( DRT_X86_FARPTR48 );
            case DI_X86_bound:
                return( DRT_X86_QWORD );
            case DI_X86_lgdt:
            case DI_X86_lidt:
            case DI_X86_sgdt:
            case DI_X86_sidt:
                return( DRT_X86_MEM1632 );
            default:
                return( DRT_X86_DWORD );
            }
        } else {
            switch( ins->type ) {
            case DI_X86_lds:
            case DI_X86_les:
            case DI_X86_lfs:
            case DI_X86_lgs:
            case DI_X86_lss:
            case DI_X86_call4:
            case DI_X86_jmp4:
                return( DRT_X86_FARPTR32 );
            case DI_X86_bound:
                return( DRT_X86_DWORD );
            case DI_X86_lgdt:
            case DI_X86_lidt:
            case DI_X86_sgdt:
            case DI_X86_sidt:
                return( DRT_X86_MEM1624 );
            default:
                return( DRT_X86_WORD );
            }
        }
    }
    return( DRT_X86_BYTE );
}

dis_ref_type X86FGetRefType( MF_BITS mf, dis_dec_ins * ins )
/*********************************************************************
 * Get Reference Type - Floating Point Instructions
 */
{
    switch( mf ) {
    case MF_0: return( DRT_X86_DWORDF );
    case MF_1: return( DRT_X86_DWORD );
    case MF_2: return( DRT_X86_QWORDF );
    case MF_3: return( DRT_X86_WORD );
    default:  return( DRT_X86_WORD );
    }
}

dis_ref_type X86FGetRefTypeEnv( dis_dec_ins * ins )
/*********************************************************************
 * Get Reference Type - Floating Point Environment Instructions
 */
{
    if( ins->flags & DIF_X86_OPND_LONG ) {
        switch( ins->type ) {
        case DI_X86_fnsave00:
        case DI_X86_fnsave01:
        case DI_X86_fnsave10:
        case DI_X86_frstor00:
        case DI_X86_frstor01:
        case DI_X86_frstor10:
            return( DRT_X86_BYTE108 );
        case DI_X86_fnstenv00:
        case DI_X86_fnstenv01:
        case DI_X86_fldenv00:
        case DI_X86_fldenv01:
        case DI_X86_fldenv10:
            return( DRT_X86_BYTE28 );
        default:
            return( DRT_X86_DWORD );
        }
    } else {
        switch( ins->type ) {
        case DI_X86_fnsave00:
        case DI_X86_fnsave01:
        case DI_X86_fnsave10:
        case DI_X86_frstor00:
        case DI_X86_frstor01:
        case DI_X86_frstor10:
            return( DRT_X86_BYTE94 );
        case DI_X86_fnstenv00:
        case DI_X86_fnstenv01:
        case DI_X86_fnstenv10:
        case DI_X86_fldenv00:
        case DI_X86_fldenv01:
        case DI_X86_fldenv10:
            return( DRT_X86_BYTE14 );
        default:
            return( DRT_X86_WORD );
        }
    }
}


/*=====================================================================*/
/*               GET REGISTER AND MOD/RM FUNCTIONS                     */
/*=====================================================================*/


static void X86GetRegModRM(SBIT dir, WBIT w, MOD mod, RM rm, RM reg,
                     void * d, dis_dec_ins *ins)
/**********************************************************************/
//    dir                   1                 0
//   Destination           Reg              MODRM
//   Source               MODRM              Reg
{
    if( dir ) {
        X86GetReg( w, reg, ins );
        X86GetModRM(w, mod, rm, d, ins, X86GetRefType( w,ins ) );
    } else {
        X86GetModRM(w, mod, rm, d, ins, X86GetRefType( w,ins ) );
        X86GetReg( w, reg, ins );
    }
}

static void X86GetMMRegModRM(SBIT dir, WBIT w, MOD mod, RM rm, RM reg,  dis_ref_type ref_type,
                     void * d, dis_dec_ins *ins)
/**********************************************************************/
//    dir                   1                 0
//   Destination           Reg              MODRM
//   Source               MODRM              Reg
{

    if( dir ) {
        X86GetMMModRM(w, mod, rm, d, ins, ref_type);
        X86GetMM( reg, ins );
    } else {
        X86GetMM( reg, ins );
        X86GetMMModRM(w, mod, rm, d, ins, ref_type);
    }
}

static void X86GetMMRegModRMMixed(SBIT dir, WBIT w, MOD mod, RM rm, RM reg,  dis_ref_type ref_type,
                     void * d, dis_dec_ins *ins)
/*************************************************************************************************/
//    dir                   1                 0
//   Destination           Reg              MODRM
//   Source               MODRM              Reg
//   but MODRM specifies an integer register for the movd instructions (not an MMX register)
{

    if( dir ) {
        X86GetModRM(w, mod, rm, d, ins, ref_type);
        X86GetMM( reg, ins );
    } else {
        X86GetMM( reg, ins );
        X86GetModRM(w, mod, rm, d, ins, ref_type);
    }
}

static void X86GetMMRegModRM_B(MOD mod, RM rm, RM reg,  dis_ref_type ref_type,
                     void * d, dis_dec_ins *ins)
/**********************************************************************/
//   Destination           Reg
//   Source               MODRM
{
    X86GetMM( reg, ins );
    X86GetMMModRM(W_DEFAULT, mod, rm, d, ins, ref_type);
}

void X86GetRegModRM_B( MOD mod, RM rm, RM reg, void *d, dis_dec_ins *ins )
/**********************************************************************/
// Destination:  Register
// Source     :  MODRM
{
    X86GetReg( W_DEFAULT, reg, ins );
    X86GetModRM(W_DEFAULT, mod, rm, d, ins, X86GetRefType( W_DEFAULT ,ins ));
}



/*=====================================================================*/
/*                8-BIT OPCODE FUNCTIONS                               */
/*=====================================================================*/



typedef union {
    unsigned_32 full;
    struct {
        unsigned_32 w  : 1;
        unsigned_32 dir: 1;
        unsigned_32 op1: 6;
    } type1;
    struct {
        unsigned_32 reg: 3;
        unsigned_32 w  : 1;
        unsigned_32 op1: 4;
    } type2;
    struct {
        unsigned_32 w  : 1;
        unsigned_32 s  : 1;
        unsigned_32 op1: 6;
    } type3;
    struct {
        unsigned_32 op2: 3;
        unsigned_32 sreg:2;
        unsigned_32 op1: 3;
    } sreg;
} code_8;


dis_handler_return X86SReg_8( dis_handle *h, void *d, dis_dec_ins *ins)
/*********************************************************************
 *  OOO SS OOO
 *     sreg
 */
{
    code_8 code;

    code.full = ins->opcode;
    ins->size    += 1;
    ins->num_ops = 0;

    switch( ins->type ) {
    case DI_X86_push3:
        ins->op[ins->num_ops].base = X86GetRegister( W_DEFAULT,REG_SP,ins );
        ins->op[ins->num_ops].type = DO_MEMORY_ABS | DO_HIDDEN;
        ins->op[ins->num_ops].ref_type = X86GetRefType(W_DEFAULT,ins);
        ++ins->num_ops;
        break;
    }
    X86GetSReg(W_DEFAULT, code.sreg.sreg, ins);
    switch( ins->type ) {
    case DI_X86_pop3d:
    case DI_X86_pop3e:
    case DI_X86_pop3s:
        ins->op[ins->num_ops].base = X86GetRegister( W_DEFAULT,REG_SP,ins );
        ins->op[ins->num_ops].type = DO_MEMORY_ABS | DO_HIDDEN;
        ins->op[ins->num_ops].ref_type = X86GetRefType  (W_DEFAULT,ins);
        ++ins->num_ops;
        break;
    }

    if( DIF_X86_OPND_SIZE & ins->flags ) {
        if( DIF_X86_OPND_LONG & ins->flags ) {
            switch( ins->type ) {
            case DI_X86_push3:
                ins->type = DI_X86_pushd;
                break;
            }
        } else {
            switch( ins->type ) {
            case DI_X86_push3:
                ins->type = DI_X86_pushw;
                break;
            }
        }
    }
    return( DHR_DONE );
}

dis_handler_return X86NoOp_8( dis_handle *h, void *d, dis_dec_ins *ins)
/*********************************************************************
 *  OOOO OOOW
 */
{
    code_8 code;

    code.full = ins->opcode;
    ins->size    += 1;
    ins->num_ops = 0;

    switch( ins->type ) {
    case DI_X86_popa:
    case DI_X86_popf:
    case DI_X86_pusha:
    case DI_X86_pushf:
        ins->op[ins->num_ops].base = X86GetRegister( W_DEFAULT,REG_SP,ins );
        ins->op[ins->num_ops].type = DO_MEMORY_ABS | DO_HIDDEN;
        ins->op[ins->num_ops].ref_type = X86GetRefType(W_DEFAULT,ins);
        ++ins->num_ops;
        break;
    case DI_X86_xlat:
        if( DIF_X86_ADDR_LONG & ins->flags ) {
            ins->op[ins->num_ops].base = X86GetRegister_D( W_DEFAULT,REG_BX,ins );
        } else {
            ins->op[ins->num_ops].base = X86GetRegister_W( W_DEFAULT,REG_BX,ins );
        }
        ins->op[ins->num_ops].type = DO_MEMORY_ABS;
        ins->op[ins->num_ops].ref_type = X86GetRefType(W_DEFAULT,ins);
        ++ins->num_ops;
        break;
    }

    if( ins->flags & DIF_X86_OPND_LONG ) {
        switch(ins->type) {
        case DI_X86_cbw :
            ins->type = DI_X86_cwde;
            break;
        case DI_X86_cwd :
            ins->type = DI_X86_cdq;
            break;
        case DI_X86_iret:
            ins->type = DI_X86_iretd;
            break;
        case DI_X86_popa:
            ins->type = DI_X86_popad;
            break;
        case DI_X86_popf:
            ins->type = DI_X86_popfd;
            break;
        case DI_X86_pusha:
            ins->type = DI_X86_pushad;
            break;
        case DI_X86_pushf:
            ins->type = DI_X86_pushfd;
            break;
        }
    }
    return( DHR_DONE );
}

dis_handler_return X86String_8( dis_handle *h, void *d, dis_dec_ins *ins )
/**********************************************************************
 * String Functions
 *  OOOO OOOW
 */
{
    code_8 code;

    code.full = ins->opcode;
    ins->size += 1;
    ins->num_ops = 2;

    switch( ins->type ) {
    case DI_X86_cmps:
        ins->op[0].base = X86GetRegister(W_DEFAULT,REG_SI,ins );
        ins->op[0].type = DO_MEMORY_ABS;
        ins->op[0].ref_type = X86GetRefType(code.type1.w, ins);
        ins->op[1].base = X86GetRegister(W_DEFAULT,REG_DI,ins );
        ins->op[1].type = DO_MEMORY_ABS;
        ins->op[1].ref_type = X86GetRefType(code.type1.w, ins);
        break;
    case DI_X86_movs:
        ins->op[0].base = X86GetRegister(W_DEFAULT,REG_DI,ins );
        ins->op[0].type = DO_MEMORY_ABS;
        ins->op[0].ref_type = X86GetRefType(code.type1.w, ins);
        ins->op[1].base = X86GetRegister(W_DEFAULT,REG_SI,ins );
        ins->op[1].type = DO_MEMORY_ABS;
        ins->op[1].ref_type = X86GetRefType(code.type1.w, ins);
        break;
    case DI_X86_ins:
        ins->op[0].base = X86GetRegister(W_DEFAULT,REG_DI,ins );
        ins->op[0].type = DO_MEMORY_ABS;
        ins->op[0].ref_type = X86GetRefType(code.type1.w, ins);
        ins->op[1].base = DR_X86_dx;
        ins->op[1].type = DO_REG;
        ins->op[1].ref_type = X86GetRefType(code.type1.w, ins);
        break;
    case DI_X86_lods:
        ins->op[0].base = X86GetRegister(W_DEFAULT,REG_SI,ins );
        ins->op[0].type = DO_MEMORY_ABS;
        ins->op[0].ref_type = X86GetRefType(code.type1.w, ins);
        ins->num_ops = 1;
        break;
    case DI_X86_outs:
        ins->op[0].base = DR_X86_dx;
        ins->op[0].type = DO_REG;
        ins->op[0].ref_type = X86GetRefType(code.type1.w, ins);
        ins->op[1].base = X86GetRegister(W_DEFAULT,REG_SI,ins );
        ins->op[1].type = DO_MEMORY_ABS;
        ins->op[1].ref_type = X86GetRefType(code.type1.w, ins);
        break;
    case DI_X86_stos:
    case DI_X86_scas:
        ins->op[0].base = X86GetRegister(W_DEFAULT,REG_DI,ins );
        ins->op[0].type = DO_MEMORY_ABS;
        ins->op[0].ref_type = X86GetRefType(code.type1.w, ins);
        ins->num_ops = 1;
        break;
    }
    return( DHR_DONE );
}

dis_handler_return X86ImmAcc_8( dis_handle *h, void *d, dis_dec_ins *ins )
/**********************************************************************
 * Immediate to Accumulator EAX, AX, or AL
 * Format:    OOOO OOOW
 */
{
    code_8 code;

    code.full    = ins->opcode;
    ins->num_ops = 0;
    ins->size   += 1;
    switch( ins->type ) {
    case DI_X86_in:
        X86GetReg(code.type1.w, REG_AX, ins);
        X86GetUImmedVal( S_DEFAULT, code.type1.w, d, ins);
        break;
    case DI_X86_out:
        X86GetUImmedVal( S_DEFAULT, code.type1.w, d, ins);
        X86GetReg(code.type1.w, REG_AX, ins);
        break;
    default:
        X86GetReg(code.type1.w, REG_AX, ins);
        X86GetImmedVal( S_DEFAULT, code.type1.w, d, ins);
        break;
    }
    return( DHR_DONE );
}

dis_handler_return X86MemAbsAcc_8( dis_handle *h, void *d, dis_dec_ins *ins )
/**********************************************************************
 * Absolute Memory to Accumulator
 * Format:    OOOO OODW
 */
{
    code_8 code;

    code.full    = ins->opcode;
    ins->num_ops = 0;
    ins->size   += 1;
    if( code.type1.dir ) {
        ins->op[0].type = DO_MEMORY_ABS;
        ins->op[0].ref_type = X86GetRefType( code.type1.w , ins );
        ins->op[0].op_position = ins->size;
        if( ins->flags & DIF_X86_ADDR_LONG ) {
            ins->op[0].value = GetULong(d,ins->size);
            ins->size += 4;
        } else {
            ins->op[0].value = GetSShort(d,ins->size);
            ins->size   += 2;
        }
        ++ins->num_ops;
        X86GetReg(code.type1.w, REG_AX, ins);
    } else {
        X86GetReg(code.type1.w, REG_AX, ins);
        ins->op[1].type  = DO_MEMORY_ABS;
        ins->op[1].ref_type = X86GetRefType( code.type1.w , ins );
        ins->op[1].op_position = ins->size;
        if( ins->flags & DIF_X86_ADDR_LONG ) {
            ins->op[1].value = GetULong(d,ins->size);
            ins->size += 4;
        } else {
            ins->op[1].value = GetSShort(d,ins->size);
            ins->size += 2;
        }
        ++ins->num_ops;
    }
    return( DHR_DONE );
}


dis_handler_return X86Abs_8( dis_handle *h, void *d, dis_dec_ins *ins)
/**********************************************************************/
{
    ins->size   += 1;
    ins->num_ops = 0;
    X86GetAbsVal(d, ins);
    return( DHR_DONE );
}


dis_handler_return X86Rel_8( dis_handle *h, void *d, dis_dec_ins *ins)
/**********************************************************************/
{
    ins->size   += 1;
    ins->num_ops = 0;
    X86GetRelVal(d, ins);
    return( DHR_DONE );
}

dis_handler_return X86Imm_8( dis_handle *h, void *d, dis_dec_ins *ins)
/**********************************************************************/
//  Byte      OOOO OOSW
{
    code_8 code;

    code.full = ins->opcode;

    ins->size   += 1;
    ins->num_ops = 0;
    ins->op[0].type = DO_IMMED;
    switch( ins->type ) {
    case DI_X86_int:
        if( code.type3.w ) {
            ins->op[0].value = GetUByte( d, ins->size );
            ins->size += 1;
        } else {
            ins->op[0].value = 3;
        }
        ++ins->num_ops;
        break;
    case DI_X86_ret2:
    case DI_X86_retf2:
        ins->op[0].value = GetUShort( d, ins->size );
        ins->size += 2;
        ++ins->num_ops;
        break;
    default:
        X86GetImmedVal( code.type3.s, W_DEFAULT, d, ins );
        break;
    }
    if( DIF_X86_OPND_SIZE & ins->flags ) {
        if( DIF_X86_OPND_LONG & ins->flags ) {
            switch( ins->type ) {
            case DI_X86_push5:
                ins->type = DI_X86_pushd;
                break;
            }
        } else {
            switch( ins->type ) {
            case DI_X86_push5:
                ins->type = DI_X86_pushw;
                break;
            }
        }
    }
    return( DHR_DONE );
}


dis_handler_return X86ImmReg_8( dis_handle *h, void *d, dis_dec_ins * ins)
/**********************************************************************/
//  8-bit   OOOO WRRR : Imm
{
    code_8 code;

    code.full = ins->opcode;
    ins->num_ops = 0;
    ++ins->size;
    X86GetReg( code.type2.w, code.type2.reg, ins );
    X86GetImmedVal( S_DEFAULT, code.type2.w, d, ins );
    return( DHR_DONE );
}

dis_handler_return X86ImmImm_8( dis_handle *h, void *d, dis_dec_ins * ins)
/**********************************************************************/
//  8-bit   OOOO OOOO : Imm16 Imm8
{
    ins->num_ops = 2;
    ins->size   += 1;
    ins->op[0].value = GetSShort( d,ins->size );
    ins->op[0].type = DO_IMMED;
    ins->size   += 2;
    ins->op[1].value = GetSByte( d,ins->size );
    ins->op[1].type = DO_IMMED;
    ins->size   += 1;
    return( DHR_DONE );
}

dis_handler_return X86Reg_8( dis_handle *h, void *d , dis_dec_ins *ins)
/**********************************************************************/
//  Byte       OOOO  ORRR
{
    code_8 code;

    ins->size      += 1;
    ins->num_ops    = 0;
    code.full       = ins->opcode;

    switch( ins->type ) {
    case DI_X86_xchg2:
        if( code.type2.reg == REG_AX ) {
            ins->type = DI_X86_nop;
        } else {
            X86GetReg( W_DEFAULT, code.type2.reg, ins );
            X86GetReg( W_DEFAULT,REG_AX,ins );
        }
        return( DHR_DONE );
    case DI_X86_push2:
        ins->op[ins->num_ops].base = X86GetRegister( W_DEFAULT,REG_SP,ins );
        ins->op[ins->num_ops].type = DO_MEMORY_ABS | DO_HIDDEN;
        ins->op[ins->num_ops].ref_type = X86GetRefType( W_DEFAULT,ins);
        ++ins->num_ops;
        break;
    }
    X86GetReg( W_DEFAULT, code.type2.reg, ins );
    switch( ins->type ) {
    case DI_X86_pop2:
        ins->op[ins->num_ops].base = X86GetRegister( W_DEFAULT,REG_SP,ins );
        ins->op[ins->num_ops].type = DO_MEMORY_ABS | DO_HIDDEN;
        ins->op[ins->num_ops].ref_type = X86GetRefType(W_DEFAULT,ins);
        ++ins->num_ops;
        break;
    }
    if( DIF_X86_OPND_SIZE & ins->flags ) {
        if( DIF_X86_OPND_LONG & ins->flags ) {
            switch( ins->type ) {
            case DI_X86_push2:
                ins->type = DI_X86_pushd;
                break;
            }
        } else {
            switch( ins->type ) {
            case DI_X86_push2:
                ins->type = DI_X86_pushw;
                break;
            }
        }
    }
    return( DHR_DONE );
}


dis_handler_return X86AccAcc_8( dis_handle *h, void *d, dis_dec_ins *ins )
/**********************************************************************/
// Accum to Accumulator EAX, AX, or AL to DX
// Format:    0000 000 W
//              op1    w
{
    code_8 code;

    code.full    = ins->opcode;
    ins->size   += 1;
    ins->num_ops = 0;
    switch( ins->type ) {
    case DI_X86_in2:
        X86GetReg(code.type1.w, REG_AX, ins);
        ins->op[1].type = DO_REG;
        ins->op[1].base = DR_X86_dx;
        ++ins->num_ops;
        break;
    case DI_X86_out2:
        ins->op[0].type = DO_REG;
        ins->op[0].base = DR_X86_dx;
        ++ins->num_ops;
        X86GetReg(code.type1.w, REG_AX, ins);
        break;
    }
    return( DHR_DONE );
}


dis_handler_return X86JmpCC_8( dis_handle *h, void *d, dis_dec_ins *ins )
/**********************************************************************
 *  Jump Handler
 */
{
    code_8 code;

    code.full = ins->opcode;
    ins->size += 1;
    ins->num_ops = 0;

    X86GetRelVal_8( d, ins );
    switch( ins->type ) {
    case DI_X86_jcxz:
        if( ins->flags & DIF_X86_ADDR_LONG ){
            ins->type = DI_X86_jecxz;
        }
        return( DHR_DONE );
    }

    if( ins->flags & DIF_X86_ADDR_SIZE ) {
        if( ins->flags & DIF_X86_ADDR_LONG ) {
            switch( ins->type ) {
            case DI_X86_loop:
                ins->type = DI_X86_loopd;
                break;
            case DI_X86_loopz:
                ins->type = DI_X86_loopzd;
                break;
            case DI_X86_loopnz:
                ins->type = DI_X86_loopnzd;
                break;
            }
        } else {
            switch( ins->type ) {
            case DI_X86_loop:
                ins->type = DI_X86_loopw;
                break;
            case DI_X86_loopz:
                ins->type = DI_X86_loopzw;
                break;
            case DI_X86_loopnz:
                ins->type = DI_X86_loopnzw;
                break;
            }
        }
    }
    return( DHR_DONE );
}


/*=====================================================================*/
/*                16-BIT OPCODE FUNCTIONS                              */
/*=====================================================================*/

typedef union {
    unsigned_32 full;
    struct {
        unsigned_32 w  : 1;
        unsigned_32 s  : 1; //  SBIT
        unsigned_32 op1: 6;
        unsigned_32 rm : 3;
        unsigned_32 reg: 3;
        unsigned_32 mod: 2;
    } type1;
    struct {
        unsigned_32 w  : 1;
        unsigned_32 dir: 1; // direction
        unsigned_32 op1: 6;
        unsigned_32 rm : 3;
        unsigned_32 reg: 3;
        unsigned_32 mod: 2;
    } type2;
    struct {
        unsigned_32 op1: 8;
        unsigned_32 op2: 3;
        unsigned_32 sreg:3;
    } sreg;
    struct {
        unsigned_32 w  : 1;
        unsigned_32 cl : 1;
        unsigned_32 op1: 6;
        unsigned_32 rm : 3;
        unsigned_32 reg: 3;
        unsigned_32 mod: 2;
    } shift;
} code_16;

dis_handler_return X86Shift_16 ( dis_handle *h, void *d, dis_dec_ins *ins)
/*********************************************************************
 *  OOOO OO C W : MOD OOO RM
 *         CL W
 */
{
    code_16 code;

    code.full = ins->opcode;
    ins->size    += 2;
    ins->num_ops = 0;

    X86GetModRM(code.shift.w,
                 code.shift.mod,
                 code.shift.rm,
                 d, ins, X86GetRefType(code.shift.w,ins));

    if(code.shift.cl) {
        ins->op[ins->num_ops].base = DR_X86_cl;
        ins->op[ins->num_ops].type = DO_REG;
        ++ins->num_ops;
    } else {
        ins->op[ins->num_ops].value = 1;
        ins->op[ins->num_ops].type = DO_IMMED;
        ++ins->num_ops;
    }
    return( DHR_DONE );

}

dis_handler_return X86SReg_16( dis_handle *h, void *d, dis_dec_ins *ins)
/*********************************************************************
 *  OOO SS OOO
 *     sreg
 */
{
    code_16 code;

    code.full = ins->opcode;
    ins->size    += 2;
    ins->num_ops = 0;
    switch( ins->type ) {
    case DI_X86_push4f:
    case DI_X86_push4g:
        ins->op[ins->num_ops].base = X86GetRegister( W_DEFAULT,REG_SP,ins );
        ins->op[ins->num_ops].type = DO_MEMORY_ABS | DO_HIDDEN;
        ins->op[ins->num_ops].ref_type = X86GetRefType(W_DEFAULT,ins);
        ++ins->num_ops;
        break;
    }

    switch( code.sreg.sreg ) {
    case REG_FS:
    case REG_GS:
        break;
    default:
        return( DHR_INVALID );
    }

    X86GetSReg(W_DEFAULT, code.sreg.sreg, ins);

    switch( ins->type ) {
    case DI_X86_pop4f:
    case DI_X86_pop4g:
        ins->op[ins->num_ops].base = X86GetRegister( W_DEFAULT,REG_SP,ins );
        ins->op[ins->num_ops].type = DO_MEMORY_ABS | DO_HIDDEN;
        ins->op[ins->num_ops].ref_type = X86GetRefType(W_DEFAULT,ins);
        ++ins->num_ops;
        break;
    }

    if( DIF_X86_OPND_SIZE & ins->flags ) {
        if( DIF_X86_OPND_LONG & ins->flags ) {
            switch( ins->type ) {
            case DI_X86_push4f:
                ins->type = DI_X86_pushd;
                break;
            case DI_X86_push4g:
                ins->type = DI_X86_pushd;
                break;
            }
        } else {
            switch( ins->type ) {
            case DI_X86_push4f:
                ins->type = DI_X86_pushw;
                break;
            case DI_X86_push4g:
                ins->type = DI_X86_pushw;
                break;
            }
        }
    }
    return( DHR_DONE );
}

dis_handler_return X86NoOp_16( dis_handle *h, void *d , dis_dec_ins *ins)
/**********************************************************************
 *  OOOO OOOO : OOOO OOOO
 */
{
    ins->size += 2;
    ins->num_ops = 0;
    return( DHR_DONE );
}

dis_handler_return X86ModRM_16( dis_handle *h, void *d, dis_dec_ins *ins)
/**********************************************************************
 *  16-Bit     OOOO OOOW : MM   OOO    RRR
 */
{
    code_16 code;

    code.full = ins->opcode;
    ins->num_ops = 0;

    switch( ins->type ) {
    case DI_X86_push:
        ins->op[ins->num_ops].base = X86GetRegister( W_DEFAULT,REG_SP,ins );
        ins->op[ins->num_ops].type = DO_MEMORY_ABS | DO_HIDDEN;
        ins->op[ins->num_ops].ref_type = X86GetRefType(W_DEFAULT,ins);
        ++ins->num_ops;
        break;
    }

    ins->size += 2;
    X86GetModRM(code.type1.w,
                 code.type1.mod,
                 code.type1.rm,
                 d, ins, X86GetRefType(code.type1.w,ins));

    switch( ins->type ) {
    case DI_X86_pop:
        ins->op[ins->num_ops].base = X86GetRegister( W_DEFAULT,REG_SP,ins );
        ins->op[ins->num_ops].type = DO_MEMORY_ABS | DO_HIDDEN;
        ins->op[ins->num_ops].ref_type = X86GetRefType(W_DEFAULT,ins);
        ++ins->num_ops;
        break;
    }

    if( DIF_X86_OPND_SIZE & ins->flags ) {
        if( DIF_X86_OPND_LONG & ins->flags ) {
            switch( ins->type ) {
            case DI_X86_push:
                ins->type = DI_X86_pushd;
                break;
            }
        } else {
            switch( ins->type ) {
            case DI_X86_push:
                ins->type = DI_X86_pushw;
                break;
            }
        }
    }
    return( DHR_DONE );
}


dis_handler_return X86ModRMImm_16( dis_handle *h, void *d, dis_dec_ins *ins )
/**********************************************************************/
// Format:    OOOO OOO W MM  OOO RRR
//              op1    w mod op2 rm
{

    code_16 code;

    code.full     = ins->opcode;
    ins->num_ops = 0;
    ins->size    += 2;
    X86GetModRM(code.type1.w,
                 code.type1.mod,
                 code.type1.rm,
                 d, ins, X86GetRefType(code.type1.w, ins));
    switch(ins->type ) {
    case DI_X86_rcl2:
    case DI_X86_rcr2:
    case DI_X86_ror2:
    case DI_X86_rol2:
    case DI_X86_sar2:
    case DI_X86_shr2:
    case DI_X86_shl2:
        X86GetImmedVal( S_BYTE, code.type1.w, d, ins );
        break;
    case DI_X86_mov2:
    case DI_X86_test2:
    case DI_X86_test3:
        X86GetImmedVal( S_DEFAULT, code.type1.w, d, ins );
        break;
    default:
        X86GetImmedVal( code.type1.s, code.type1.w, d, ins );
        break;
    }
    return( DHR_DONE );
}

dis_handler_return X86ModRMAcc_16( dis_handle *h, void *d, dis_dec_ins *ins )
/**********************************************************************
 * Immediate to Accumulator EAX, AX, or AL
 * Format:   OOOO OOOW MM OOO RRR
 */
{
    code_16 code;

    code.full    = ins->opcode;
    ins->num_ops = 0;
    ins->size   += 1;
    X86GetReg(code.type1.w, REG_AX, ins);
    X86GetModRM(code.type1.w,
                 code.type1.mod,
                 code.type1.rm,
                 d, ins, X86GetRefType(code.type1.w,ins));
    return( DHR_DONE );
}

dis_handler_return X86RegModRM_16( dis_handle *h, void *d, dis_dec_ins *ins )
/**********************************************************************/
// Format:    OOOO OO   D         W MM   REG RRR
//              op1    direction  w mod reg1 rm/reg2
{
    code_16 code;

    code.full    = ins->opcode;
    ins->num_ops = 0;
    ins->size   += 2;
    X86GetRegModRM(code.type2.dir, code.type2.w, code.type2.mod,
                    code.type2.rm, code.type2.reg, d, ins);
    return( DHR_DONE );
}

dis_handler_return X86RegModRM_test( dis_handle *h, void *d, dis_dec_ins *ins )
/**********************************************************************/
// Format:    OOOO OOO W MM   REG RRR
//              op1    w mod reg1 rm/reg2
{
    code_16 code;

    code.full    = ins->opcode;
    ins->num_ops = 0;
    ins->size   += 2;
    X86GetModRM( code.type2.w, code.type2.mod, code.type2.rm,
                    d, ins, X86GetRefType( code.type2.w, ins ) );
    X86GetReg( code.type2.w, code.type2.reg, ins );
    return( DHR_DONE );
}



dis_handler_return X86RegModRM_16B( dis_handle *h, void *d, dis_dec_ins *ins )
/**********************************************************************/
// Format:    OOOO OOSO   MM REG RRR
//              op1      mod reg1 rm/reg2
{
    code_16 code;

    code.full     = ins->opcode;
    ins->num_ops  = 0;
    ins->size    += 2;
    if( code.type1.mod == MOD_3 ) {
        switch( ins->type ) {
        case DI_X86_lds:
        case DI_X86_les:
        case DI_X86_lea:
        case DI_X86_call4:
        case DI_X86_jmp4:
            return( DHR_INVALID );
        }
    }

    X86GetRegModRM_B( code.type1.mod, code.type1.rm, code.type1.reg, d, ins);

    switch( ins->type ) {
    case DI_X86_imul3:
        if( ins->op[0].type == ins->op[1].type ) {
            if( ins->op[0].base == ins->op[1].base ) {
                ins->op[1].type |= DO_HIDDEN;
            }
        }
        X86GetImmedVal( code.type1.s, W_DEFAULT, d, ins);
        break;
    case DI_X86_lea:
        ins->op[1].type = DO_ABSOLUTE;
        break;
    }
    return( DHR_DONE );
}

dis_handler_return X86Reg_16( dis_handle *h, void *d , dis_dec_ins *ins)
/**********************************************************************/
//  Format:
//  Byte       OOOO OOOO OOOO ORRR
//                op1          reg
{
    code_16 code;

    code.full       = ins->opcode;
    ins->num_ops    = 0;
    ins->size      +=2;
    X86GetReg(W_DEFAULT, code.type1.rm,ins);

    return( DHR_DONE );
}

dis_handler_return X86JmpCC_16( dis_handle *h, void *d, dis_dec_ins *ins )
/**********************************************************************
 *  Jump Handler
 */
{
    code_16 code;

    code.full = ins->opcode;
    ins->size += 2;
    ins->num_ops = 0;
    X86GetRelVal( d, ins );
    return( DHR_DONE );
}

dis_handler_return X86SRegModRM_16( dis_handle *h, void * d, dis_dec_ins *ins )
/**********************************************************************/
{
    code_16 code;

    code.full     = ins->opcode;
    ins->size    += 2;
    ins->num_ops = 0;
    if( code.type2.dir ) {
        X86GetSReg( W_DEFAULT, code.type2.reg, ins );
        if( ins->op[0].base == DR_NONE ) {
            return ( DHR_INVALID );
        }
        X86GetModRM_W(W_DEFAULT, code.type2.mod, code.type2.rm, d, ins);
    } else {
        X86GetModRM_W(W_DEFAULT, code.type2.mod, code.type2.rm, d, ins);
        X86GetSReg( W_DEFAULT, code.type2.reg, ins );
        if( ins->op[1].base == DR_NONE ) {
            return( DHR_INVALID );
        }
    }
    return( DHR_DONE );
}


/*=====================================================================*/
/*                24-BIT OPCODE FUNCTIONS                              */
/*=====================================================================*/

typedef union {
    unsigned_32 full;
    struct {
        unsigned_32 op1: 8;
        unsigned_32 w  : 1;
        unsigned_32 s  : 1;
        unsigned_32 op2: 6;
        unsigned_32 rm : 3;
        unsigned_32 reg: 3;
        unsigned_32 mod: 2;
    } type1;
    struct {
        unsigned_32 op1: 8;
        unsigned_32 w  : 1;
        unsigned_32 dir: 1;
        unsigned_32 op2: 6;
        unsigned_32 rm : 3;
        unsigned_32 reg: 3;
        unsigned_32 mod: 2;
    } type2;
} code_24;

dis_handler_return X86SetCC( dis_handle *h, void *d, dis_dec_ins *ins)
/*********************************************************************
 *  SetCC instruction
 */
{
    code_24 code;

    code.full = ins->opcode;
    ins->num_ops = 0;
    ins->size += 3;
    X86GetModRM_B( W_DEFAULT, code.type1.mod, code.type1.rm, d, ins );
    return( DHR_DONE );
}

dis_handler_return X86MovCC_24( dis_handle *h, void *d, dis_dec_ins *ins )
/**********************************************************************
 *  MovCC instruction
 */
{
    code_24 code;

    code.full = ins->opcode;
    ins->size += 3;
    ins->num_ops = 0;
    // The r/rm fields are reversed for CMOVcc instructions
    ins->op[0].base  = X86GetRegister( W_DEFAULT, code.type1.reg, ins );
    ins->op[0].type  = DO_REG;
    ++ins->num_ops;
    X86GetModRM( W_DEFAULT, code.type1.mod, code.type1.rm, d, ins,
                    X86GetRefType( W_DEFAULT, ins ) );
    return( DHR_DONE );
}

dis_handler_return X86CRegReg_24( dis_handle *h, void * d, dis_dec_ins *ins )
/**********************************************************************/
{
    code_24 code;

    code.full     = ins->opcode;
    ins->size    += 3;
    ins->num_ops = 2;
    if( code.type2.dir ) {
        ins->op[1].type = DO_REG;
        ins->op[1].base = X86GetRegister_D( W_DEFAULT, code.type2.rm, ins );
        ins->op[0].type = DO_REG;
        ins->op[0].base = X86GetCRegister( W_DEFAULT, code.type2.reg, ins );
        if( ins->op[0].base == DR_NONE ) {
            return( DHR_INVALID );
        }
    } else {
        ins->op[0].type = DO_REG;
        ins->op[0].base = X86GetRegister_D( W_DEFAULT, code.type2.rm, ins );
        ins->op[1].type = DO_REG;
        ins->op[1].base = X86GetCRegister( W_DEFAULT, code.type2.reg, ins );
        if( ins->op[1].base == DR_NONE ) {
            return( DHR_INVALID );
        }
    }
    return( DHR_DONE );
}


dis_handler_return X86DRegReg_24( dis_handle *h, void * d, dis_dec_ins *ins )
/**********************************************************************/
{
    code_24 code;

    code.full     = ins->opcode;
    ins->size    += 3;
    ins->num_ops = 2;
    if( code.type2.dir ) {
        ins->op[1].type = DO_REG;
        ins->op[1].base = X86GetRegister_D( W_DEFAULT, code.type2.rm, ins );
        ins->op[0].type = DO_REG;
        ins->op[0].base = X86GetDRegister( W_DEFAULT, code.type2.reg, ins );
        if( ins->op[0].base == DR_NONE ) {
            return( DHR_INVALID );
        }
    } else {
        ins->op[0].type = DO_REG;
        ins->op[0].base = X86GetRegister_D( W_DEFAULT, code.type2.rm, ins );
        ins->op[1].type = DO_REG;
        ins->op[1].base = X86GetDRegister( W_DEFAULT, code.type2.reg, ins );
        if( ins->op[1].base == DR_NONE ) {
            return( DHR_INVALID );
        }
    }
    return( DHR_DONE );
}

dis_handler_return X86TRegReg_24( dis_handle *h, void * d, dis_dec_ins *ins )
/**********************************************************************/
{
    code_24 code;

    code.full     = ins->opcode;
    ins->size    += 3;
    ins->num_ops = 2;
    if( code.type2.dir ) {
        ins->op[1].type = DO_REG;
        ins->op[1].base = X86GetRegister_D( W_DEFAULT, code.type2.rm, ins );
        ins->op[0].type = DO_REG;
        ins->op[0].base = X86GetTRegister( W_DEFAULT, code.type2.reg, ins );
        if( ins->op[0].base == DR_NONE ) {
            return( DHR_INVALID );
        }
    } else {
        ins->op[0].type = DO_REG;
        ins->op[0].base = X86GetRegister_D( W_DEFAULT, code.type2.rm, ins );
        ins->op[1].type = DO_REG;
        ins->op[1].base = X86GetTRegister( W_DEFAULT, code.type2.reg, ins );
        if( ins->op[1].base == DR_NONE ) {
            return( DHR_INVALID );
        }
    }
    return( DHR_DONE );
}

dis_handler_return X86ModRMImm_24( dis_handle *h, void *d, dis_dec_ins *ins )
/**********************************************************************/
// Format:    OOOO OOOO OOOO OO S X   MM  OOO RRR
//                op1    op2    s X  mod op3 rm

{
    code_24 code;

    code.full     = ins->opcode;
    ins->size    += 3;
    ins->num_ops = 0;

    X86GetModRM(W_DEFAULT,
                 code.type1.mod,
                 code.type1.rm,
                 d, ins, X86GetRefType(W_DEFAULT, ins));
    X86GetImmedVal( code.type1.s, W_DEFAULT, d, ins);
    return( DHR_DONE );
}

dis_handler_return X86ModRM_24( dis_handle *h, void *d, dis_dec_ins *ins )
/**********************************************************************/
// Format:    OOOO OOOO OOOO OOOO    MM   OOO RRR
//                      op1         mod  op2 rm/reg2
{

    code_24 code;

    code.full = ins->opcode;
    ins->num_ops = 0;
    ins->size    += 3;
    if( code.type1.mod == MOD_3 ) {
        switch( ins->type ) {
        case DI_X86_lgdt:
        case DI_X86_lidt:
        case DI_X86_sgdt:
        case DI_X86_sidt:
        case DI_X86_cmpxchg8b:
            return( DHR_INVALID );
        }
    }
    X86GetModRM(W_DEFAULT, code.type1.mod,code.type1.rm, d, ins,
                  X86GetRefType(W_DEFAULT,ins));
    return( DHR_DONE );
}

dis_handler_return X86RegModRM_24( dis_handle *h, void *d, dis_dec_ins *ins )
/**********************************************************************/
// Format:    OOOO OOOO OOOO OO   D         W MM   REG RRR
//                       op1    direction   w mod reg1 rm/reg2
{

    code_24 code;

    code.full = ins->opcode;
    ins->size    += 3;
    ins->num_ops = 0;
    X86GetRegModRM(code.type2.dir, code.type2.w, code.type2.mod,
                    code.type2.rm, code.type2.reg, d, ins);
    return( DHR_DONE );
}


dis_handler_return X86RegModRM_24B( dis_handle *h, void *d, dis_dec_ins *ins )
/**********************************************************************/
// Format:    OOOO OOOO OOOO OOOO MM REG RRR
//                     op1       mod reg1 rm/reg2
{
    code_24 code;

    code.full = ins->opcode;
    ins->size    += 3;
    ins->num_ops = 0;

    if( code.type1.mod == MOD_3 ) {
        switch( ins->type ) {
        case DI_X86_lgs:
        case DI_X86_lss:
        case DI_X86_lfs:
            return( DHR_INVALID );
        }
    }

    switch(ins->type) {
    case DI_X86_cmpxchg8b:
        X86GetModRM(W_DEFAULT, code.type1.mod, code.type1.rm, d, ins,
                               X86GetRefType( W_DEFAULT ,ins ) );
        break;
    case DI_X86_shld:
    case DI_X86_shrd:
        X86GetModRM(W_DEFAULT, code.type1.mod, code.type1.rm, d, ins,
                               X86GetRefType( W_DEFAULT ,ins ) );
        X86GetReg( W_DEFAULT, code.type1.reg, ins );
        ins->op[ins->num_ops].base = DR_X86_cl;
        ins->op[ins->num_ops].type = DO_REG;
        ++ins->num_ops;
        break;
    case DI_X86_shld2:
    case DI_X86_shrd2:
        X86GetModRM(W_DEFAULT, code.type1.mod, code.type1.rm, d, ins,
                               X86GetRefType( W_DEFAULT ,ins ) );
        X86GetReg( W_DEFAULT, code.type1.reg, ins );
        ins->op[ins->num_ops].value = GetUByte(d, ins->size);
        ins->op[ins->num_ops].type = DO_IMMED;
        ++ins->size;
        ++ins->num_ops;
        break;
    default:
        X86GetRegModRM_B( code.type1.mod, code.type1.rm,
                           code.type1.reg, d, ins);
        break;
    }
    return( DHR_DONE );
}

dis_handler_return X86RegModRM_24C( dis_handle *h, void *d, dis_dec_ins *ins )
/**********************************************************************/
// Format:    OOOO OOOO OOOO OOOW MM REG RRR
{
    code_24 code;

    code.full = ins->opcode;
    ins->size += 3;
    ins->num_ops = 0;

    if( code.type1.w ) {
        ins->op[0].base  = X86GetRegister_D( W_DEFAULT, code.type1.reg, ins );
        ins->op[0].type  = DO_REG;
        ++ins->num_ops;
        X86GetModRM_W(W_DEFAULT, code.type1.mod, code.type1.rm, d, ins);
    } else {
        if( ins->flags & DIF_X86_OPND_LONG ) {
            ins->op[0].base  = X86GetRegister_D( W_DEFAULT, code.type1.reg, ins );
            ins->op[0].type  = DO_REG;
            ++ins->num_ops;
            X86GetModRM_B(W_DEFAULT, code.type1.mod, code.type1.rm, d, ins);
        } else {
            ins->op[0].base  = X86GetRegister_W( W_DEFAULT, code.type1.reg, ins );
            ins->op[0].type  = DO_REG;
            ++ins->num_ops;
            X86GetModRM_B(W_DEFAULT, code.type1.mod, code.type1.rm, d, ins);
        }
    }
    return( DHR_DONE );
}



/*=====================================================================*/
/*                Floating Point Instruction                           */
/*=====================================================================*/

typedef union {
    unsigned_32 full;
    struct {
        unsigned_32 op1: 1;
        unsigned_32 intgr: 1;
        unsigned_32 w  : 1; // 1 if 16-Bit Int  0 if 32-Bit Int
    } size;
    struct {
        unsigned_32 op2: 1;
        unsigned_32 opa: 2;
        unsigned_32 op1: 5;
        unsigned_32 rm : 3;
        unsigned_32 opb: 2;
        unsigned_32 op3: 1;
        unsigned_32 mod: 2;
    } type1;
    struct {
        unsigned_32 opa: 1;
        unsigned_32 mf : 2;
        unsigned_32 op1: 5;
        unsigned_32 rm : 3;
        unsigned_32 opb: 3;
        unsigned_32 mod: 2;
    } type2;
    struct {
        unsigned_32 opa: 1;
        unsigned_32 p  : 1;
        unsigned_32 d  : 1;
        unsigned_32 op1: 5;
        unsigned_32 st : 3;
        unsigned_32 r  : 1;
        unsigned_32 opb: 2;
        unsigned_32 op2: 2;
    } type3;
    struct {
        unsigned_32 op1: 8;
        unsigned_32 op : 5;
        unsigned_32 op2: 3;
    } type4;
    struct {
        unsigned_32 n  : 1;
        unsigned_32 op1: 7;
        unsigned_32 st : 3;
        unsigned_32 cond:2;
    } cond;
} fl_pt;


dis_handler_return X86FTypeCond( dis_handle *h, void *d, dis_dec_ins * ins )
/**********************************************************************
 *
 */
{
    fl_pt code;

    code.full = ins->opcode;
    ins->size += 2;
    ins->num_ops = 0;
    X86FGetST(RM_0,ins);
    X86FGetST(code.cond.st,ins);
    return( DHR_DONE );
}

dis_handler_return X86FType3( dis_handle *h, void *d, dis_dec_ins *ins )
/**********************************************************************
 *    use only when MODRM = MOD_3
 *    p bit = 0   destination ST(0)
 *
 *    p bit not set use:
 *    d bit = 0 destination ST(0) source ST(i)
 *    d bit = 1 destination ST(i) source ST(0)
 */
{
    fl_pt code;

    code.full = ins->opcode;
    ins->size += 2;
    ins->num_ops = 0;
    ins->flags |= DIF_X86_FP_INS;

    // If the POP bit is set change the instruction to the pop instruction
    if( code.type3.p ) {

        // The Only possible destination is ST
        X86FGetST(code.type3.st,ins);
        X86FGetST(RM_0,ins);
        return( DHR_DONE );
    }

    // Change the type of instruction used
    if(!code.type3.d && code.type3.r || code.type3.d && !code.type3.r) {
        switch(ins->type) {
        case DI_X86_fdiv11:
            ins->type = DI_X86_fdivr11;
            break;
        case DI_X86_fsub11:
            ins->type = DI_X86_fsubr11;
            break;
        }
    } else {
        switch(ins->type) {
        case DI_X86_fdivr11:
            ins->type = DI_X86_fdiv11;
            break;
        case DI_X86_fsubr11:
            ins->type = DI_X86_fsub11;
            break;
        }
    }

    // Direction of the instruction
    if( code.type3.d ) {
        X86FGetST(code.type3.st, ins);
        X86FGetST(RM_0, ins);
    } else {
        X86FGetST(RM_0, ins);
        X86FGetST(code.type3.st, ins);
    }
    return( DHR_DONE );
}

dis_handler_return X86FType3B( dis_handle *h, void *d, dis_dec_ins *ins )
/**********************************************************************
 *  Type 3 - One Register Version - One Operand
 */
{
    fl_pt code;

    code.full = ins->opcode;

    ins->size += 2;
    ins->num_ops = 0;
    ins->flags |= DIF_X86_FP_INS;
    X86FGetST(code.type3.st,ins);
    return( DHR_DONE );
}

dis_handler_return X86FType3C( dis_handle *h, void *d, dis_dec_ins *ins )
/**********************************************************************
 *  Type 3 - Two Register - Destination is ST(0)
 */
{
    fl_pt code;

    code.full = ins->opcode;
    ins->size += 2;
    ins->num_ops = 0;
    ins->flags |= DIF_X86_FP_INS;
    X86FGetST(RM_0,ins);
    X86FGetST(code.type3.st,ins);
    return( DHR_DONE );
}

dis_handler_return X86FType2( dis_handle *h, void *d, dis_dec_ins *ins )
/***********************************************************************
 * Handles floating point instructions that have
 * MODRM as the source operands
 * Be careful not to call this function when MOD can be MOD_3.
 *
 */
{
    fl_pt code;

    code.full    = ins->opcode;
    ins->flags  |= DIF_X86_FP_INS;
    ins->num_ops = 0;
    ins->size += 2;

    /* if MOD == MOD_3 then the instruction is of type 3 */
    if( code.type2.mod == MOD_3 ) {
        return( DHR_INVALID );
    }
    X86FGetModRM(W_DEFAULT,code.type2.mod, code.type2.rm, d, ins,
                  X86FGetRefType ( code.type2.mf, ins ) );
    return( DHR_DONE );
}


dis_handler_return X86FTypeFSTSWAX ( dis_handle *h, void *d, dis_dec_ins *ins )
/*****************************************************************************/
{
    ins->size += 2;
    ins->num_ops = 0;

    ins->flags |= DIF_X86_FP_INS;
    if( ins->flags & DIF_X86_FWAIT ) {
        ins->type = DI_X86_fstsw2;
    }
    ins->num_ops    = 1;
    ins->op[0].type = DO_REG;
    ins->op[0].base = X86GetRegister_W(W_DEFAULT, REG_AX, ins);
    return( DHR_DONE );
}

dis_handler_return X86FType4 ( dis_handle *h, void *d, dis_dec_ins *ins )
/**********************************************************************
 *  Type 4 and 5 are the same
 */
{
    ins->size += 2;
    ins->num_ops = 0;

    ins->flags |= DIF_X86_FP_INS;
    if( ins->flags & DIF_X86_FWAIT ) {
        switch( ins->type ) {
        /* Instructions with FWAIT names */
        case DI_X86_fnclex:
            ins->type = DI_X86_fclex;
            break;
        case DI_X86_fndisi:
            ins->type = DI_X86_fdisi;
            break;
        case DI_X86_fneni:
            ins->type = DI_X86_feni;
            break;
        case DI_X86_fninit:
            ins->type = DI_X86_finit;
            break;
        }
    }
    return( DHR_DONE );
}


dis_handler_return X86FType1( dis_handle *h, void *d, dis_dec_ins *ins )
/**********************************************************************
 */
{
    fl_pt code;

    ins->size += 2;
    ins->num_ops = 0;
    ins->flags |= DIF_X86_FP_INS;
    code.full = ins->opcode;

    switch( ins->type ) {
    case DI_X86_fbstp00:
    case DI_X86_fbstp01:
    case DI_X86_fbstp10:
    case DI_X86_fbld00:
    case DI_X86_fbld01:
    case DI_X86_fbld10:
        X86GetModRM(W_DEFAULT, code.type1.mod, code.type1.rm, d, ins, DRT_X86_BCD);
        break;
    case DI_X86_fistp6400:
    case DI_X86_fistp6401:
    case DI_X86_fistp6410:
    case DI_X86_fild6400:
    case DI_X86_fild6401:
    case DI_X86_fild6410:
        X86GetModRM(W_DEFAULT, code.type1.mod, code.type1.rm, d, ins, DRT_X86_QWORD);
        break;
    case DI_X86_fstp8000:
    case DI_X86_fstp8001:
    case DI_X86_fstp8010:
    case DI_X86_fld8000:
    case DI_X86_fld8001:
    case DI_X86_fld8010:
        X86GetModRM(W_DEFAULT, code.type1.mod, code.type1.rm, d, ins, DRT_X86_TBYTE);
        break;
    case DI_X86_fnstcw00:
    case DI_X86_fnstcw01:
    case DI_X86_fnstcw10:
    case DI_X86_fldcw00:
    case DI_X86_fldcw01:
    case DI_X86_fldcw10:
        X86GetModRM(W_DEFAULT, code.type1.mod, code.type1.rm, d, ins, DRT_X86_WORD);
        break;
    default:
        X86GetModRM(W_DEFAULT,
                          code.type1.mod, code.type1.rm,
                          d, ins, DRT_X86_QWORD);
        break;
    }

    if( ins->flags & DIF_X86_FWAIT ) {
        switch( ins->type ) {
        case DI_X86_fnstcw00:
        case DI_X86_fnstcw01:
        case DI_X86_fnstcw10:
            ins->type = DI_X86_fstcw;
            break;
        case DI_X86_fnstsw00:
        case DI_X86_fnstsw01:
        case DI_X86_fnstsw10:
            ins->type = DI_X86_fstsw;
            break;
        }
    }
    return( DHR_DONE );
}


dis_handler_return X86FTypeEnv( dis_handle *h, void *d, dis_dec_ins *ins )
/**********************************************************************
 *  Loading/Unloading/etc FPU Environment
 */
{
    fl_pt code;

    ins->num_ops = 0;

    code.full = ins->opcode;
    ins->flags |= DIF_X86_FP_INS;

    if( code.type1.mod == MOD_3 ) {
        return ( DHR_INVALID );
    }

    ins->size += 2;

    X86GetModRM(W_DEFAULT, code.type1.mod, code.type1.rm,
                  d, ins, X86FGetRefTypeEnv( ins ));

    // There is a change in the use - mode
    if( ins->flags & DIF_X86_OPND_SIZE ) {
        if( DIF_X86_OPND_LONG & ins->flags ) {
            switch( ins->type ) {
            case DI_X86_fldenv00:
            case DI_X86_fldenv01:
            case DI_X86_fldenv10:
                ins->type = DI_X86_fldenvd;
                break;
            case DI_X86_frstor00:
            case DI_X86_frstor01:
            case DI_X86_frstor10:
                ins->type = DI_X86_frstord;
                break;
            case DI_X86_fnsave00:
            case DI_X86_fnsave01:
            case DI_X86_fnsave10:
                ins->type = DI_X86_fnsaved;
                break;
            case DI_X86_fnstenv00:
            case DI_X86_fnstenv01:
            case DI_X86_fnstenv10:
                ins->type = DI_X86_fnstenvd;
                break;
            }
        } else {
            switch(ins->type) {
            case DI_X86_fldenv00:
            case DI_X86_fldenv01:
            case DI_X86_fldenv10:
                ins->type = DI_X86_fldenvw;
                break;
            case DI_X86_frstor00:
            case DI_X86_frstor01:
            case DI_X86_frstor10:
                ins->type = DI_X86_frstorw;
                break;
            case DI_X86_fnsave00:
            case DI_X86_fnsave01:
            case DI_X86_fnsave10:
                ins->type = DI_X86_fnsavew;
                break;
            case DI_X86_fnstenv00:
            case DI_X86_fnstenv01:
            case DI_X86_fnstenv10:
                ins->type = DI_X86_fnstenvw;
                break;
            }
        }
    }
    if( ins->flags & DIF_X86_FWAIT ) {
        switch( ins->type ) {
        case DI_X86_fnsave00:
        case DI_X86_fnsave01:
        case DI_X86_fnsave10:
            ins->type = DI_X86_fsave;
            break;
        case DI_X86_fnsavew:
            ins->type = DI_X86_fsavew;
            break;
        case DI_X86_fnsaved:
            ins->type = DI_X86_fsaved;
            break;
        case DI_X86_fnstenv00:
        case DI_X86_fnstenv01:
        case DI_X86_fnstenv10:
            ins->type = DI_X86_fstenv;
            break;
        case DI_X86_fnstenvw:
            ins->type = DI_X86_fstenvw;
            break;
        case DI_X86_fnstenvd:
            ins->type = DI_X86_fstenvd;
            break;
        }
    }
    return( DHR_DONE );
}

/*=====================================================================*/
/*                Multimedia Extension Instructions                    */
/*=====================================================================*/
typedef union {
    unsigned_32 full;
    struct {
        unsigned_32 op1: 8;
        unsigned_32 op3: 4;
        unsigned_32 dir: 1;
        unsigned_32 op2: 3;
        unsigned_32 rm : 3;
        unsigned_32 mm : 3;
        unsigned_32 mod: 2;
    } type1;
} mm;


dis_handler_return X86MMNoOp( dis_handle *h, void *d, dis_dec_ins *ins )
/**********************************************************************
 *  Multi-Media No Operands
 */
{
    ins->num_ops = 0;
    ins->size +=2;
    return( DHR_DONE );
}

dis_handler_return X86MMRegModRM32( dis_handle *h, void *d, dis_dec_ins *ins )
/**********************************************************************
 *  Multi-Media 32-Bit RegModRM
 */
{
    mm code;

    code.full = ins->opcode;
    ins->num_ops = 0;
    ins->size +=3;

    X86GetMMRegModRMMixed(code.type1.dir, W_DEFAULT,
                   code.type1.mod, code.type1.rm, code.type1.mm, DRT_X86_MM32, d, ins) ;
    return( DHR_DONE );
}

dis_handler_return X86MMRegModRM64( dis_handle *h, void *d, dis_dec_ins *ins )
/**********************************************************************
 *  Multi-Media 64-Bit RegModRM
 */
{
    mm code;

    code.full = ins->opcode;
    ins->num_ops = 0;
    ins->size +=3;

    X86GetMMRegModRM(code.type1.dir, W_DEFAULT,
                   code.type1.mod, code.type1.rm, code.type1.mm, DRT_X86_MM64, d, ins) ;
    return( DHR_DONE );
}

dis_handler_return X86MMRegModRM64_B( dis_handle *h, void *d, dis_dec_ins *ins )
/**********************************************************************
 *  Multi-Media 64-Bit RegModRM - Destination only mm
 */
{
    mm code;

    code.full = ins->opcode;
    ins->num_ops = 0;
    ins->size +=3;

    X86GetMMRegModRM_B( code.type1.mod, code.type1.rm, code.type1.mm, DRT_X86_MM64, d, ins);
    return( DHR_DONE );
}

dis_handler_return X86MMImmMM( dis_handle *h, void *d, dis_dec_ins * ins)
/*********************************************************************
 * Dest: MM
 * source: immediate 8
 */
{
    mm code;

    code.full = ins->opcode;
    ins->num_ops = 0;
    ins->size += 3;
    X86GetMM( code.type1.rm, ins );
    X86GetUImmedVal( S_BYTE, W_DEFAULT, d, ins );
    return( DHR_DONE );
}

/*=====================================================================*/
/*                HOOK FUNCTIONS                                       */
/*=====================================================================*/


static unsigned AddRefType( char *p, unsigned len, dis_ref_type ref_type )
{
    switch( ref_type ) {
    case DRT_X86_BYTE:
        p[len++] = 'b';
        break;
    case DRT_X86_WORD:
        p[len++] = 'w';
        break;
    case DRT_X86_DWORD:
    case DRT_X86_DWORDF:
        p[len++] = 'l';
        break;
    }
    return( len );
}

static dis_ref_type GetRefType( dis_dec_ins *ins, unsigned op )
{
    unsigned    reg;

    switch( ins->op[op].type & DO_MASK ) {
    case DO_REG:
        reg = ins->op[op].base;
        if(                      reg <= DR_X86_bl ) return( DRT_X86_BYTE );
        if( reg >= DR_X86_ax  && reg <= DR_X86_di ) return( DRT_X86_WORD );
        if( reg >= DR_X86_eax && reg <= DR_X86_edi ) return( DRT_X86_DWORD );
        if( reg >= DR_X86_cr0 && reg <= DR_X86_dr7 ) return( DRT_X86_DWORD );
        if( reg >= DR_X86_es  && reg <= DR_X86_gs ) return( DRT_X86_WORD );
        break;
    case DO_MEMORY_ABS:
        return( ins->op[op].ref_type );
    }
    return( DRT_NONE );
}

static unsigned UnixMangleName( dis_dec_ins *ins, char *p, unsigned len )
{
    dis_ref_type        ref_type;
    unsigned            i;

    switch( ins->type ) {
    case DI_X86_arpl:
    case DI_X86_bound:
    case DI_X86_bswap:
    case DI_X86_call:
    case DI_X86_call2:
    case DI_X86_cmpxchg:
    case DI_X86_cmpxchg8b:
    //NYI: conditional move instructions?
    case DI_X86_cpuid:
    case DI_X86_invlpg:
    case DI_X86_jmp:
    case DI_X86_jmp2:
    case DI_X86_lgdt:
    case DI_X86_lidt:
    case DI_X86_lldt:
    case DI_X86_lmsw:
    case DI_X86_ltr:
    case DI_X86_pushw:
    case DI_X86_pushd:
    case DI_X86_rdmsr:
    case DI_X86_rdmpc:
    case DI_X86_seto:
    case DI_X86_setno:
    case DI_X86_setb:
    case DI_X86_setae:
    case DI_X86_sete:
    case DI_X86_setne:
    case DI_X86_setbe:
    case DI_X86_seta:
    case DI_X86_sets:
    case DI_X86_setns:
    case DI_X86_setp:
    case DI_X86_setpo:
    case DI_X86_setl:
    case DI_X86_setge:
    case DI_X86_setle:
    case DI_X86_setg:
    case DI_X86_sgdt:
    case DI_X86_sidt:
    case DI_X86_sldt:
    case DI_X86_str:
    case DI_X86_verr:
    case DI_X86_verw:
    case DI_X86_wbinvd:
    case DI_X86_wrmsr:
        return( len );
    case DI_X86_movsx:
    case DI_X86_movzx:
        len = AddRefType( p, len, GetRefType( ins, 0 ) );
        ref_type = GetRefType( ins, 1 );
        break;
    case DI_X86_call3:
    case DI_X86_call4:
        strcpy( p, "lcall" );
        return( 5 );
    case DI_X86_jmp3:
    case DI_X86_jmp4:
        strcpy( p, "ljmp" );
        return( 4 );
    case DI_X86_retf:
    case DI_X86_retf2:
        strcpy( p, "lret" );
        return( 4 );
    case DI_X86_cbw:
        strcpy( p, "cbtw" );
        return( 4 );
    case DI_X86_cwde:
        strcpy( p, "ctwl" );
        return( 4 );
    case DI_X86_cwd:
        strcpy( p, "ctwd" );
        return( 4 );
    case DI_X86_cdq:
        strcpy( p, "cltd" );
        return( 4 );
    default:
        for( i = 0; i < ins->num_ops; ++i ) {
            ref_type = GetRefType( ins, i );
            if( ref_type != DRT_NONE ) break;
        }
        break;
    }
    return( AddRefType( p, len, ref_type ) );
}

static unsigned X86InsHook( dis_handle *h, void *d, dis_dec_ins *ins,
        dis_format_flags flags, char *name )
/**********************************************************************/
{
    unsigned    len;
    char        *p;
    unsigned    op;
    dis_operand save;
    char        temp_buff[40];

    if( name == NULL ) name = temp_buff;
    p = name;
    if( ins->flags & DIF_X86_LOCK ) {
        p += DisGetString( DisInstructionTable[DI_X86_lock_pr].name, p, 0 );
        if( flags & DFF_X86_UNIX ) *p++ = ';';
        *p++ = ' ';
    }
    if( ins->flags & DIF_X86_REPNE ) {
        p += DisGetString( DisInstructionTable[DI_X86_repne_pr].name, p, 0 );
        if( flags & DFF_X86_UNIX ) *p++ = ';';
        *p++ = ' ';
    }
    if( ins->flags & DIF_X86_REPE ) {
        p += DisGetString( DisInstructionTable[DI_X86_repe_pr].name, p, 0 );
        if( flags & DFF_X86_UNIX ) *p++ = ';';
        *p++ = ' ';
    }
    len = DisGetString( DisInstructionTable[ins->type].name, p, 0 );
    if( flags & DFF_X86_UNIX ) {
        if( ins->num_ops >= 2 ) {
            op = ins->num_ops - 1;
            save = ins->op[op];
            ins->op[op] = ins->op[0];
            ins->op[0] = save;
        }
        len = UnixMangleName( ins, p, len );
    }
    p += len;
    if( !X86SegmentOverride( ins ) ) {
        switch( ins->type ) {
        case DI_X86_cmps:
        case DI_X86_ins:
        case DI_X86_lods:
        case DI_X86_movs:
        case DI_X86_outs:
        case DI_X86_scas:
        case DI_X86_stos:
            /* UnixMangleName will have already taken care of this */
            if( !(flags & DFF_X86_UNIX) ) {
                op = 0;
                if( (ins->op[op].type & DO_MASK) != DO_MEMORY_ABS ) op = 1;
                switch( ins->op[op].ref_type ) {
                case DRT_X86_BYTE:
                    *p++ = 'b';
                    break;
                case DRT_X86_WORD:
                    *p++ = 'w';
                    break;
                case DRT_X86_DWORD:
                    *p++ = 'd';
                    break;
                }
            }
            ins->op[0].type |= DO_HIDDEN;
            ins->op[1].type |= DO_HIDDEN;
            break;
        case DI_X86_xlat:
            *p++ = 'b';
            ins->op[0].type |= DO_HIDDEN;
            break;
        }
    }
    *p = '\0';
    return( p - name );
}

static unsigned X86FlagHook( dis_handle *h, void *d, dis_dec_ins *ins,
        dis_format_flags flags, char *name )
/**********************************************************************/
{
    return( 0 );
}

char *DisAddUnixReg( dis_register reg, char *p, dis_format_flags flags )
{
    if( reg == DR_NONE ) return( p );
    *p++ = '%';
    return( DisAddReg( reg, p, flags ) );
}

static char *DisOpUnixFormat( void *d, dis_dec_ins *ins, dis_format_flags flags,
                        unsigned i, char *p )
{
    switch( ins->type ) {
    case DI_X86_call2:
    case DI_X86_call4:
    case DI_X86_jmp2:
    case DI_X86_jmp4:
        /* indirect call/jump instructions */
        *p++ = '*';
        break;
    }
    switch( ins->op[i].type & DO_MASK ) {
    case DO_IMMED:
        *p++ = '$';
    }
    p += DisCliValueString( d, ins, i, p );
    switch( ins->op[i].type & DO_MASK ) {
    case DO_REG:
        p = DisAddUnixReg( ins->op[i].base, p, flags );
        break;
    case DO_ABSOLUTE:
    case DO_RELATIVE:
    case DO_MEMORY_ABS:
    case DO_MEMORY_REL:
        if( ins->op[i].base != DR_NONE || ins->op[i].index != DR_NONE ) {
            *p++ = '(';
            p = DisAddUnixReg( ins->op[i].base, p, flags );
            if( ins->op[i].index != DR_NONE ) {
                *p++ = ',';
                p = DisAddUnixReg( ins->op[i].index, p, flags );
                if( ins->op[i].scale != 1 ) {
                    *p++ = ',';
                    *p++ = '0' + ins->op[i].scale;
                }
            }
            *p++ = ')';
        }
        break;
    }
    return( p );
}

static char *DisOpMasmFormat( void *d, dis_dec_ins *ins, dis_format_flags flags,
                        unsigned i, char *p )
{
    unsigned    len;

    if( !(flags & DFF_X86_ALT_INDEXING) ||
        ((ins->op[i].base == DR_NONE) && (ins->op[i].index == DR_NONE)) ) {
        p += DisCliValueString( d, ins, i, p );
    }
    switch( ins->op[i].type & DO_MASK ) {
    case DO_REG:
        p = DisAddReg( ins->op[i].base, p, flags );
        break;
    case DO_ABSOLUTE:
    case DO_RELATIVE:
    case DO_MEMORY_ABS:
    case DO_MEMORY_REL:
        if( ins->op[i].base != DR_NONE || ins->op[i].index != DR_NONE ) {
            *p++ = '[';
            p = DisAddReg( ins->op[i].base, p, flags );
            if( ins->op[i].index != DR_NONE ) {
                if( ins->op[i].base != DR_NONE ) *p++ = '+';
                p = DisAddReg( ins->op[i].index, p, flags );
                if( ins->op[i].scale != 1 ) {
                    *p++ = '*';
                    *p++ = '0' + ins->op[i].scale;
                }
            }
            if( flags & DFF_X86_ALT_INDEXING ) {
                *p++ = '+';
                len = DisCliValueString( d, ins, i, p );
                switch( *p ) {
                case '-':
                    memmove( p - 1, p, len );
                    --p;
                    break;
                case '\0':
                    --p;
                    break;
                }
                p += len;
            }
            *p++ = ']';
        }
        break;
    }
    return( p );
}

static int NeedSizing( dis_dec_ins *ins, dis_format_flags flags, unsigned op_num )
{
    unsigned    i;

    if( flags & DFF_X86_UNIX ) return( FALSE );
    if( flags & DFF_ASM ) return( TRUE );
    switch( ins->type ) {
    case DI_X86_movsx:
    case DI_X86_movzx:
        /* these always need sizing */
        return( TRUE );
    }
    for( i = 0; i < ins->num_ops; ++i ) {
        switch( ins->op[i].type & DO_MASK ) {
        case DO_REG:
            /* if you've got a reg, you know the size */
            return( FALSE );
        }
    }
    return( TRUE );
}

static unsigned X86OpHook( dis_handle *h, void *d, dis_dec_ins *ins,
        dis_format_flags flags, unsigned op_num, char *op_buff )
/**********************************************************************/
{
    char        over;
    char        *p;
    unsigned    len;
    unsigned    ins_flags;

    ins_flags = ins->flags;
    p = op_buff;
    switch( ins->op[op_num].type & DO_MASK ) {
    case DO_MEMORY_ABS:
        if( NeedSizing( ins, flags, op_num ) ) {
            len = DisGetString( DisRefTypeTable[ins->op[op_num].ref_type], p, 0 );
            if( len != 0 ) {
                p += len;
                #define SUFFIX " ptr "
                memcpy( p, SUFFIX, sizeof( SUFFIX ) - 1 );
                p += sizeof( SUFFIX ) - 1;
            }
        }
        if( (ins_flags & SEGOVER) != 0 ) {
            switch( ins_flags & SEGOVER ) {
            case DIF_X86_CS:
                over = 'c';
                break;
            case DIF_X86_DS:
                over = 'd';
                break;
            case DIF_X86_ES:
                over = 'e';
                break;
            case DIF_X86_FS:
                over = 'f';
                break;
            case DIF_X86_GS:
                over = 'g';
                break;
            case DIF_X86_SS:
                over = 's';
                break;
            }
            ins_flags &= ~SEGOVER;
            if( flags & DFF_X86_UNIX ) *p++ = '%';
            if( flags & DFF_REG_UP ) {
                *p++ = toupper( over );
                *p++= 'S';
            } else {
                *p++ = over;
                *p++ = 's';
            }
            *p++ = ':';
        }
        break;
    case DO_ABSOLUTE:
    case DO_RELATIVE:
        if( (flags & DFF_ASM) && !(flags & DFF_X86_UNIX) ) {
            switch( ins->type ) {
            case DI_X86_call:
            case DI_X86_jmp1:
                #define NEAR_PTR        "near ptr "
                memcpy( p, NEAR_PTR, sizeof( NEAR_PTR ) - 1 );
                p += sizeof( NEAR_PTR ) - 1;
                break;
            case DI_X86_call3:
            case DI_X86_jmp3:
                #define FAR_PTR "far ptr "
                memcpy( p, FAR_PTR, sizeof( FAR_PTR ) - 1 );
                p += sizeof( FAR_PTR ) - 1;
                break;
            }
        }
        break;
    }
    if( flags & DFF_X86_UNIX ) {
        p = DisOpUnixFormat( d, ins, flags, op_num, p );
    } else {
        p = DisOpMasmFormat( d, ins, flags, op_num, p );
    }
    ins->flags = ins_flags;
    *p = '\0';
    return( p - op_buff );
}

const dis_cpu_data X86Data = {
    X86RangeTable, X86InsHook, X86FlagHook, X86OpHook, &X86MaxInsName, 1
};
#else

const dis_cpu_data X86Data;
#endif
