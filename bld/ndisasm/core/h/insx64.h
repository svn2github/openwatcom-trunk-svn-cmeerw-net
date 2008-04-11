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
* Description:  Table of AMD64 opcodes and corresponding decode routines.
*                without extension 3DNow!, MMX, SSE, SSE2 and SSE3
*
****************************************************************************/

/*
 * It must be processed as first instruction table before any others
 * It is opcode map with full prefix processing.
 *
 */

/*
      Idx,              Name,           Opcode,         Mask,           Handler
*/

// PREFIXES
inspick( rex_pr,        "rex",          0x00000040,     0x000000f0,     X64PrefixRex )
inspick( opnd_pr,       "opnd",         0X00000066,     0x000000ff,     X64PrefixOpnd )
inspick( addr_pr,       "addr",         0X00000067,     0x000000ff,     X64PrefixAddr )
inspick( cs_pr,         "cs",           0x0000002e,     0x000000ff,     X64PrefixCS )
inspick( fs_pr,         "fs",           0x00000064,     0x000000ff,     X64PrefixFS )
inspick( gs_pr,         "gs",           0x00000065,     0x000000ff,     X64PrefixGS )
inspick( ss_pr,         "ss",           0x00000036,     0x000000ff,     X64PrefixSS )
inspick( ds_pr,         "ds",           0x0000003e,     0x000000ff,     X64PrefixDS )
inspick( es_pr,         "es",           0x00000026,     0x000000ff,     X64PrefixES )

// Instructions
inspick( wait,          "wait",         0x0000009b,     0x000000ff,     X64NoOp_8 )
inspick( sahf,          "sahf",         0x0000009e,     0x000000ff,     X64NoOp_8 )
inspick( iretd,         "iretd",        0x000000cf,     0x000000ff,     X64NoOp_8 )
inspick( iret,          "iret",         0x00000000,     0x00000000,     NULL )
inspick( iretq,         "iretq",        0x00000000,     0x00000000,     NULL )

/* sorted */

inspick( adc,           "adc",          0x00000010,     0x000000fc,     X64RegModRM_16 )
inspick( adc2,          "adc",          0x00000014,     0x000000fe,     X64ImmAcc_8 )
inspick( adc3,          "adc",          0x00001080,     0x000038fc,     X64ModRMImm_16 )
inspick( add,           "add",          0x00000000,     0x000000fc,     X64RegModRM_16 )
inspick( add2,          "add",          0x00000004,     0x000000fe,     X64ImmAcc_8 )
inspick( add3,          "add",          0x00000080,     0x000038fc,     X64ModRMImm_16 )
inspick( and,           "and",          0x00000020,     0x000000fc,     X64RegModRM_16 )
inspick( and2,          "and",          0x00000024,     0x000000fe,     X64ImmAcc_8 )
inspick( and3,          "and",          0x00002080,     0x000038fc,     X64ModRMImm_16 )
inspick( bsf,           "bsf",          0x0000bc0f,     0x0000ffff,     X64RegModRM_24B )
inspick( bsr,           "bsr",          0x0000bd0f,     0x0000ffff,     X64RegModRM_24B )
inspick( bswap,         "bswap",        0x0000c80f,     0x0000f8ff,     X64Reg_16 )
inspick( bt,            "bt",           0x0000a30f,     0x0000ffff,     X64RegModRM_24B )
inspick( bt2,           "bt",           0x0020ba0f,     0x0038ffff,     X64ModRMImm_24 )
inspick( btc,           "btc",          0x0000bb0f,     0x0000ffff,     X64RegModRM_24B )
inspick( btc2,          "btc",          0x0038ba0f,     0x0038ffff,     X64ModRMImm_24 )
inspick( btr,           "btr",          0x0000b30f,     0x0000ffff,     X64RegModRM_24B )
inspick( btr2,          "btr",          0x0030ba0f,     0x0038ffff,     X64ModRMImm_24 )
inspick( bts,           "bts",          0x0000ab0f,     0x0000ffff,     X64RegModRM_24B )
inspick( bts2,          "bts",          0x0028ba0f,     0x0038ffff,     X64ModRMImm_24 )
inspick( call,          "call",         0x000000e8,     0x000000ff,     X64Rel_8 )
inspick( call2,         "call",         0x000010ff,     0x000038ff,     X64ModRM_16 )
//inspick( call3,         "call",         0x0000009a,     0x000000ff,     X64Abs_8 )
inspick( call4,         "call",         0x000018ff,     0x000038ff,     X64ModRM_16 )
inspick( cwde,          "cwde",         0x00000098,     0x000000ff,     X64NoOp_8 )
// Same as cwde
inspick( cbw,           "cbw",          0x00000000,     0x00000000,     NULL )
inspick( cdqe,          "cdqe",         0x00000000,     0x00000000,     NULL )
inspick( clc,           "clc",          0x000000f8,     0x000000ff,     X64NoOp_8 )
inspick( cld,           "cld",          0x000000fc,     0x000000ff,     X64NoOp_8 )
//inspick( clflush,       "clflush",      0x0000aefc,     0x0000ffff,     NULL )
inspick( cli,           "cli",          0x000000fa,     0x000000ff,     X64NoOp_8 )
inspick( cmc,           "cmc",          0x000000f5,     0x000000ff,     X64NoOp_8 )
inspick( cmp,           "cmp",          0x00000038,     0x000000fc,     X64RegModRM_16 )
inspick( cmp2,          "cmp",          0x0000003c,     0x000000fe,     X64ImmAcc_8 )
inspick( cmp3,          "cmp",          0x00003880,     0x000038fc,     X64ModRMImm_16 )
inspick( cmps,          "cmps",         0x000000a6,     0x000000fe,     X64String_8 )
inspick( cmpxchg,       "cmpxchg",      0x0000b00f,     0x0000feff,     X64RegModRM_24 )
inspick( cmpxchg8b,     "cmpxchg8b",    0x0008c70f,     0x0038ffff,     X64ModRM_24 )
inspick( cmovo,         "cmovo",        0x0000400f,     0x0000ffff,     X64MovCC_24 )
inspick( cmovno,        "cmovno",       0x0000410f,     0x0000ffff,     X64MovCC_24 )
inspick( cmovb,         "cmovb",        0x0000420f,     0x0000ffff,     X64MovCC_24 )
inspick( cmovae,        "cmovae",       0x0000430f,     0x0000ffff,     X64MovCC_24 )
inspick( cmove,         "cmove",        0x0000440f,     0x0000ffff,     X64MovCC_24 )
inspick( cmovne,        "cmovne",       0x0000450f,     0x0000ffff,     X64MovCC_24 )
inspick( cmovbe,        "cmovbe",       0x0000460f,     0x0000ffff,     X64MovCC_24 )
inspick( cmova,         "cmova",        0x0000470f,     0x0000ffff,     X64MovCC_24 )
inspick( cmovs,         "cmovs",        0x0000480f,     0x0000ffff,     X64MovCC_24 )
inspick( cmovns,        "cmovns",       0x0000490f,     0x0000ffff,     X64MovCC_24 )
inspick( cmovp,         "cmovp",        0x00004a0f,     0x0000ffff,     X64MovCC_24 )
inspick( cmovpo,        "cmovpo",       0x00004b0f,     0x0000ffff,     X64MovCC_24 )
inspick( cmovl,         "cmovl",        0x00004c0f,     0x0000ffff,     X64MovCC_24 )
inspick( cmovge,        "cmovge",       0x00004d0f,     0x0000ffff,     X64MovCC_24 )
inspick( cmovle,        "cmovle",       0x00004e0f,     0x0000ffff,     X64MovCC_24 )
inspick( cmovg,         "cmovg",        0x00004f0f,     0x0000ffff,     X64MovCC_24 )
inspick( cpuid,         "cpuid",        0x0000a20f,     0x0000ffff,     X64NoOp_16 )
inspick( cdq,           "cdq",          0x00000099,     0x000000ff,     X64NoOp_8 )
// Same as cdq
inspick( cwd,           "cwd",          0x00000000,     0x00000000,     NULL )
inspick( cqo,           "cqo",          0x00000000,     0x00000000,     NULL )
inspick( dec,           "dec",          0x000008fe,     0x000038fe,     X64ModRM_16 )
// dec2 is replaced by REX prefix
inspick( div,           "div",          0x000030f6,     0x000038fe,     X64ModRM_16 )
inspick( enter,         "enter",        0x000000c8,     0x000000ff,     X64ImmImm_8 )
inspick( halt,          "hlt",          0x000000f4,     0x000000ff,     X64NoOp_8 )
inspick( idiv,          "idiv",         0x000038f6,     0x000038fe,     X64ModRM_16 )
inspick( imul,          "imul",         0x000028f6,     0x000038fe,     X64ModRM_16 )
inspick( imul2,         "imul",         0x0000af0f,     0x0000ffff,     X64RegModRM_24B )
inspick( imul3,         "imul",         0x00000069,     0x000000fd,     X64RegModRM_16B )
inspick( in,            "in",           0x000000e4,     0x000000fe,     X64ImmAcc_8 )
inspick( in2,           "in",           0x000000ec,     0x000000fe,     X64AccAcc_8 )
inspick( inc,           "inc",          0x000000fe,     0x000038fe,     X64ModRM_16 )
// inc2 is replaced by REX prefix
inspick( ins,           "ins",          0x0000006c,     0x000000fe,     X64String_8 )
inspick( int,           "int",          0x000000cc,     0x000000fe,     X64Imm_8 )
inspick( into,          "into",         0x000000ce,     0x000000ff,     X64NoOp_8 )
inspick( jo,            "jo",           0x00000070,     0x000000ff,     X64JmpCC_8 )
inspick( jno,           "jno",          0x00000071,     0x000000ff,     X64JmpCC_8 )
inspick( jb,            "jb",           0x00000072,     0x000000ff,     X64JmpCC_8 )
inspick( jae,           "jae",          0x00000073,     0x000000ff,     X64JmpCC_8 )
inspick( je,            "je",           0x00000074,     0x000000ff,     X64JmpCC_8 )
inspick( jne,           "jne",          0x00000075,     0x000000ff,     X64JmpCC_8 )
inspick( jbe,           "jbe",          0x00000076,     0x000000ff,     X64JmpCC_8 )
inspick( ja,            "ja",           0x00000077,     0x000000ff,     X64JmpCC_8 )
inspick( js,            "js",           0x00000078,     0x000000ff,     X64JmpCC_8 )
inspick( jns,           "jns",          0x00000079,     0x000000ff,     X64JmpCC_8 )
inspick( jp,            "jp",           0x0000007a,     0x000000ff,     X64JmpCC_8 )
inspick( jpo,           "jpo",          0x0000007b,     0x000000ff,     X64JmpCC_8 )
inspick( jl,            "jl",           0x0000007c,     0x000000ff,     X64JmpCC_8 )
inspick( jge,           "jge",          0x0000007d,     0x000000ff,     X64JmpCC_8 )
inspick( jle,           "jle",          0x0000007e,     0x000000ff,     X64JmpCC_8 )
inspick( jg,            "jg",           0x0000007f,     0x000000ff,     X64JmpCC_8 )
inspick( jo2,           "jo",           0x0000800f,     0x0000ffff,     X64JmpCC_16 )
inspick( jno2,          "jno",          0x0000810f,     0x0000ffff,     X64JmpCC_16 )
inspick( jb2,           "jb",           0x0000820f,     0x0000ffff,     X64JmpCC_16 )
inspick( jae2,          "jae",          0x0000830f,     0x0000ffff,     X64JmpCC_16 )
inspick( je2,           "je",           0x0000840f,     0x0000ffff,     X64JmpCC_16 )
inspick( jne2,          "jne",          0x0000850f,     0x0000ffff,     X64JmpCC_16 )
inspick( jbe2,          "jbe",          0x0000860f,     0x0000ffff,     X64JmpCC_16 )
inspick( ja2,           "ja",           0x0000870f,     0x0000ffff,     X64JmpCC_16 )
inspick( js2,           "js",           0x0000880f,     0x0000ffff,     X64JmpCC_16 )
inspick( jns2,          "jns",          0x0000890f,     0x0000ffff,     X64JmpCC_16 )
inspick( jp2,           "jp",           0x00008a0f,     0x0000ffff,     X64JmpCC_16 )
inspick( jpo2,          "jpo",          0x00008b0f,     0x0000ffff,     X64JmpCC_16 )
inspick( jl2,           "jl",           0x00008c0f,     0x0000ffff,     X64JmpCC_16 )
inspick( jge2,          "jge",          0x00008d0f,     0x0000ffff,     X64JmpCC_16 )
inspick( jle2,          "jle",          0x00008e0f,     0x0000ffff,     X64JmpCC_16 )
inspick( jg2,           "jg",           0x00008f0f,     0x0000ffff,     X64JmpCC_16 )
inspick( jcxz,          "jcxz",         0x000000e3,     0x000000ff,     X64JmpCC_8 )
inspick( jecxz,         "jecxz",        0x00000000,     0x00000000,     NULL )
inspick( jrcxz,         "jrcxz",        0x00000000,     0x00000000,     NULL )
inspick( jmp,           "jmp",          0x000000eb,     0x000000ff,     X64JmpCC_8 )
inspick( jmp1,          "jmp",          0x000000e9,     0x000000ff,     X64Rel_8 )
inspick( jmp2,          "jmp",          0x000020ff,     0x000038ff,     X64ModRM_16 )
//inspick( jmp3,          "jmp",          0x000000ea,     0x000000ff,     X64Abs_8 )
inspick( jmp4,          "jmp",          0x000028ff,     0x000038ff,     X64ModRM_16 )
inspick( lahf,          "lahf",         0x0000009f,     0x000000ff,     X64NoOp_8 )
inspick( lfs,           "lfs",          0x0000b40f,     0x0000ffff,     X64RegModRM_24B )
inspick( lgs,           "lgs",          0x0000b50f,     0x0000ffff,     X64RegModRM_24B )
inspick( lss,           "lss",          0x0000b20f,     0x0000ffff,     X64RegModRM_24B )
inspick( lea,           "lea",          0x0000008d,     0x000000ff,     X64RegModRM_16B )
inspick( leave,         "leave",        0x000000c9,     0x000000ff,     X64NoOp_8 )
inspick( lods,          "lods",         0x000000ac,     0x000000fe,     X64String_8 )
inspick( loop,          "loop",         0x000000e2,     0x000000ff,     X64JmpCC_8 )
inspick( loopw,         "loopw",        0x00000000,     0x00000000,     NULL )
inspick( loopd,         "loopd",        0x00000000,     0x00000000,     NULL )
inspick( loopq,         "loopq",        0x00000000,     0x00000000,     NULL )
inspick( loopz,         "loopz",        0x000000e1,     0x000000ff,     X64JmpCC_8 )
inspick( loopzw,        "loopzw",       0x00000000,     0x00000000,     NULL )
inspick( loopzd,        "loopzd",       0x00000000,     0x00000000,     NULL )
inspick( loopzq,        "loopzq",       0x00000000,     0x00000000,     NULL )
inspick( loopnz,        "loopnz",       0x000000e0,     0x000000ff,     X64JmpCC_8 )
inspick( loopnzw,       "loopnzw",      0x00000000,     0x00000000,     NULL )
inspick( loopnzd,       "loopnzd",      0x00000000,     0x00000000,     NULL )
inspick( loopnzq,       "loopnzq",      0x00000000,     0x00000000,     NULL )

inspick( mov,           "mov",          0x00000088,     0x000000fc,     X64RegModRM_16 )
inspick( mov2,          "mov",          0x000000c6,     0x000038fe,     X64ModRMImm_16 )
inspick( mov3,          "mov",          0x000000b0,     0x000000f0,     X64ImmReg_8 )
inspick( mov4,          "mov",          0x000000a0,     0x000000fc,     X64MemAbsAcc_8 )
//inspick( mov5,          "mov",          0x0000200f,     0x0000fdff,     X64CRegReg_24 )
//inspick( mov6,          "mov",          0x0000210f,     0x0000fdff,     X64DRegReg_24 )
inspick( mov7,          "mov",          0x0000008c,     0x000000fd,     X64SRegModRM_16 )
//inspick( mov8,          "mov",          0x0000240f,     0x0000fdff,     X64TRegReg_24 )
inspick( movs,          "movs",         0x000000a4,     0x000000fe,     X64String_8 )
inspick( movsx,         "movsx",        0x0000be0f,     0x0000feff,     X64RegModRM_24C )
inspick( movsxd,        "movsxd",       0x00000063,     0x000000ff,     X64RegModRM_16C )
inspick( movzx,         "movzx",        0x0000b60f,     0x0000feff,     X64RegModRM_24C )
inspick( mul,           "mul",          0x000020f6,     0x000038fe,     X64ModRM_16 )
inspick( neg,           "neg",          0x000018f6,     0x000038fe,     X64ModRM_16 )
inspick( not,           "not",          0x000010f6,     0x000038fe,     X64ModRM_16 )
inspick( or,            "or",           0x00000008,     0x000000fc,     X64RegModRM_16 )
inspick( or2,           "or",           0x0000000c,     0x000000fe,     X64ImmAcc_8 )
inspick( or3,           "or",           0x00000880,     0x000038fc,     X64ModRMImm_16 )
inspick( out,           "out",          0x000000e6,     0x000000fe,     X64ImmAcc_8 )
inspick( out2,          "out",          0x000000ee,     0x000000fe,     X64AccAcc_8 )
inspick( outs,          "outs",         0x0000006e,     0x000000fe,     X64String_8 )
inspick( pop,           "pop",          0x0000008f,     0x000038ff,     X64ModRM_16 )
inspick( pop2,          "pop",          0x00000058,     0x000000f8,     X64Reg_8 )
//inspick( pop3d,         "pop",          0x00000007,     0x000000ff,     X64SReg_8 )
//inspick( pop3e,         "pop",          0x00000017,     0x000000ff,     X64SReg_8 )
//inspick( pop3s,         "pop",          0x0000001f,     0x000000ff,     X64SReg_8 )
inspick( pop4f,         "pop",          0x0000a10f,     0x0000ffff,     X64SReg_16 )
inspick( pop4g,         "pop",          0x0000a90f,     0x0000ffff,     X64SReg_16 )
inspick( push,          "push",         0x000030ff,     0x000038ff,     X64ModRM_16 )
inspick( pushw,         "pushw",        0x00000000,     0x00000000,     NULL )
//inspick( pushd,         "pushd",        0x00000000,     0x00000000,     NULL )
inspick( push2,         "push",         0x00000050,     0x000000f8,     X64Reg_8 )
inspick( push3,         "push",         0x00000006,     0x000000e7,     X64SReg_8 )
inspick( push4f,        "push",         0x0000a00f,     0x0000ffff,     X64SReg_16 )
inspick( push4g,        "push",         0x0000a80f,     0x0000ffff,     X64SReg_16 )
inspick( push5,         "push",         0x00000068,     0x000000fd,     X64Imm_8 )

inspick( popf,          "popf",         0x00000000,     0x00000000,     NULL )
inspick( popfq,         "popfq",        0x0000009d,     0x000000ff,     X64NoOp_8 )

inspick( pushf,         "pushf",        0x00000000,     0x00000000,     NULL )
inspick( pushfq,        "pushfq",       0x0000009c,     0x000000ff,     X64NoOp_8 )
inspick( rcl,           "rcl",          0x000010d0,     0x000038fc,     X64Shift_16 )
inspick( rcl2,          "rcl",          0x000010c0,     0x000038fe,     X64ModRMImm_16 )
inspick( rcr,           "rcr",          0x000018d0,     0x000038fc,     X64Shift_16 )
inspick( rcr2,          "rcr",          0x000018c0,     0x000038fe,     X64ModRMImm_16 )

inspick( ret,           "ret",          0x000000c3,     0x000000ff,     X64NoOp_8 )
inspick( ret2,          "ret",          0x000000c2,     0x000000ff,     X64Imm_8 )
inspick( retf,          "retf",         0x000000cb,     0x000000ff,     X64NoOp_8 )
inspick( retf2,         "retf",         0x000000ca,     0x000000ff,     X64Imm_8 )
inspick( ror,           "ror",          0x000008d0,     0x000038fc,     X64Shift_16 )
inspick( ror2,          "ror",          0x000008c0,     0x000038fe,     X64ModRMImm_16 )
inspick( rol,           "rol",          0x000000d0,     0x000038fc,     X64Shift_16 )
inspick( rol2,          "rol",          0x000000c0,     0x000038fe,     X64ModRMImm_16 )

inspick( sbb,           "sbb",          0x00000018,     0x000000fc,     X64RegModRM_16 )
inspick( sbb2,          "sbb",          0x0000001c,     0x000000fe,     X64ImmAcc_8 )
inspick( sbb3,          "sbb",          0x00001880,     0x000038fc,     X64ModRMImm_16 )
inspick( scas,          "scas",         0x000000ae,     0x000000fe,     X64String_8 )
inspick( seto,          "seto",         0x0000900f,     0x0038ffff,     X64SetCC )
inspick( setno,         "setno",        0x0000910f,     0x0038ffff,     X64SetCC )
inspick( setb,          "setb",         0x0000920f,     0x0038ffff,     X64SetCC )
inspick( setae,         "setae",        0x0000930f,     0x0038ffff,     X64SetCC )
inspick( sete,          "sete",         0x0000940f,     0x0038ffff,     X64SetCC )
inspick( setne,         "setne",        0x0000950f,     0x0038ffff,     X64SetCC )
inspick( setbe,         "setbe",        0x0000960f,     0x0038ffff,     X64SetCC )
inspick( seta,          "seta",         0x0000970f,     0x0038ffff,     X64SetCC )
inspick( sets,          "sets",         0x0000980f,     0x0038ffff,     X64SetCC )
inspick( setns,         "setns",        0x0000990f,     0x0038ffff,     X64SetCC )
inspick( setp,          "setp",         0x00009a0f,     0x0038ffff,     X64SetCC )
inspick( setpo,         "setpo",        0x00009b0f,     0x0038ffff,     X64SetCC )
inspick( setl,          "setl",         0x00009c0f,     0x0038ffff,     X64SetCC )
inspick( setge,         "setge",        0x00009d0f,     0x0038ffff,     X64SetCC )
inspick( setle,         "setle",        0x00009e0f,     0x0038ffff,     X64SetCC )
inspick( setg,          "setg",         0x00009f0f,     0x0038ffff,     X64SetCC )
inspick( shl,           "shl",          0x000020d0,     0x000038fc,     X64Shift_16 )
inspick( shl2,          "shl",          0x000020c0,     0x000038fe,     X64ModRMImm_16 )
inspick( shr,           "shr",          0x000028d0,     0x000038fc,     X64Shift_16 )
inspick( shr2,          "shr",          0x000028c0,     0x000038fe,     X64ModRMImm_16 )
inspick( sar,           "sar",          0x000038d0,     0x000038fc,     X64Shift_16 )
inspick( sar2,          "sar",          0x000038c0,     0x000038fe,     X64ModRMImm_16 )

inspick( shld,          "shld",         0x0000a50f,     0x0000ffff,     X64RegModRM_24B )
inspick( shld2,         "shld",         0x0000a40f,     0x0000ffff,     X64RegModRM_24B )
inspick( shrd,          "shrd",         0x0000ad0f,     0x0000ffff,     X64RegModRM_24B )
inspick( shrd2,         "shrd",         0x0000ac0f,     0x0000ffff,     X64RegModRM_24B )

inspick( stc,           "stc",          0x000000f9,     0x000000ff,     X64NoOp_8 )
inspick( std,           "std",          0x000000fd,     0x000000ff,     X64NoOp_8 )
inspick( sti,           "sti",          0x000000fb,     0x000000ff,     X64NoOp_8 )
inspick( stos,          "stos",         0x000000aa,     0x000000fe,     X64String_8 )

inspick( sub,           "sub",          0x00000028,     0x000000fc,     X64RegModRM_16 )
inspick( sub2,          "sub",          0x0000002c,     0x000000fe,     X64ImmAcc_8 )
inspick( sub3,          "sub",          0x00002880,     0x000038fc,     X64ModRMImm_16 )

inspick( test,          "test",         0x00000084,     0x000000fe,     X64RegModRM_test )
inspick( test2,         "test",         0x000000a8,     0x000000fe,     X64ImmAcc_8 )
inspick( test3,         "test",         0x000000f6,     0x000038fe,     X64ModRMImm_16 )

inspick( xadd,          "xadd",         0x0000c00f,     0x0000feff,     X64RegModRM_24 )
inspick( xchg,          "xchg",         0x00000086,     0x000000fe,     X64RegModRM_test )
inspick( xchg2,         "xchg",         0x00000090,     0x000000f8,     X64Reg_8 )
inspick( xlat,          "xlat",         0x000000d7,     0x000000ff,     X64NoOp_8 )
inspick( xor,           "xor",          0x00000030,     0x000000fc,     X64RegModRM_16 )
inspick( xor2,          "xor",          0x00000034,     0x000000fe,     X64ImmAcc_8 )
inspick( xor3,          "xor",          0x00003080,     0x000038fc,     X64ModRMImm_16 )
// part of xchg
inspick( nop,           "nop",          0x00000090,     0x00000000,     NULL )
