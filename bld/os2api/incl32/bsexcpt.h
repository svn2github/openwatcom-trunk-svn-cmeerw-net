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
* Description:  OS/2 exception handling structures and constants.
*
****************************************************************************/


#ifndef __BSEXCPT_INCLUDED__
#define __BSEXCPT_INCLUDED__

#define XCPT_CONTINUE_SEARCH    0
#define XCPT_CONTINUE_EXECUTION (-1)
#define XCPT_CONTINUE_STOP      0x00716668

#define EH_NONCONTINUABLE 0x01
#define EH_UNWINDING      0x02
#define EH_EXIT_UNWIND    0x04
#define EH_STACK_INVALID  0x08
#define EH_NESTED_CALL    0x10
#define EH_SIGFTERM       0x20

#define UNWIND_ALL 0

#define XCPT_FATAL_EXCEPTION 0xC0000000
#define XCPT_SEVERITY_CODE   0xC0000000
#define XCPT_CUSTOMER_CODE   0x20000000
#define XCPT_FACILITY_CODE   0x1FFF0000
#define XCPT_EXCEPTION_CODE  0x0000FFFF

#define XCPT_UNKNOWN_ACCESS  0x00000000
#define XCPT_READ_ACCESS     0x00000001
#define XCPT_WRITE_ACCESS    0x00000002
#define XCPT_EXECUTE_ACCESS  0x00000004
#define XCPT_SPACE_ACCESS    0x00000008
#define XCPT_LIMIT_ACCESS    0x00000010
#define XCPT_DATA_UNKNOWN    0xFFFFFFFF

#define XCPT_SIGNAL_INTR     1
#define XCPT_SIGNAL_KILLPROC 3
#define XCPT_SIGNAL_BREAK    4
#define XCPT_SIGNAL_APTERM   8

#define XCPT_GUARD_PAGE_VIOLATION     0x80000001
#define XCPT_UNABLE_TO_GROW_STACK     0x80010001
#define XCPT_ACCESS_VIOLATION         0xC0000005
#define XCPT_IN_PAGE_ERROR            0xC0000006
#define XCPT_ILLEGAL_INSTRUCTION      0xC000001C
#define XCPT_INVALID_LOCK_SEQUENCE    0xC000001D
#define XCPT_NONCONTINUABLE_EXCEPTION 0xC0000024
#define XCPT_INVALID_DISPOSITION      0xC0000025
#define XCPT_UNWIND                   0xC0000026
#define XCPT_BAD_STACK                0xC0000027
#define XCPT_INVALID_UNWIND_TARGET    0xC0000028
#define XCPT_ARRAY_BOUNDS_EXCEEDED    0xC0000093
#define XCPT_FLOAT_DENORMAL_OPERAND   0xC0000094
#define XCPT_FLOAT_DIVIDE_BY_ZERO     0xC0000095
#define XCPT_FLOAT_INEXACT_RESULT     0xC0000096
#define XCPT_FLOAT_INVALID_OPERATION  0xC0000097
#define XCPT_FLOAT_OVERFLOW           0xC0000098
#define XCPT_FLOAT_STACK_CHECK        0xC0000099
#define XCPT_FLOAT_UNDERFLOW          0xC000009A
#define XCPT_INTEGER_DIVIDE_BY_ZERO   0xC000009B
#define XCPT_INTEGER_OVERFLOW         0xC000009C
#define XCPT_PRIVILEGED_INSTRUCTION   0xC000009D
#define XCPT_DATATYPE_MISALIGNMENT    0xC000009E
#define XCPT_BREAKPOINT               0xC000009F
#define XCPT_SINGLE_STEP              0xC00000A0
#define XCPT_PROCESS_TERMINATE        0xC0010001
#define XCPT_ASYNC_PROCESS_TERMINATE  0xC0010002
#define XCPT_SIGNAL                   0xC0010003
#define XCPT_B1NPX_ERRATA_02          0xC0010004

#define CONTEXT_CONTROL        1
#define CONTEXT_INTEGER        2
#define CONTEXT_SEGMENTS       4
#define CONTEXT_FLOATING_POINT 8
#define CONTEXT_FULL           (CONTEXT_CONTROL | CONTEXT_INTEGER | \
                                CONTEXT_SEGMENTS | CONTEXT_FLOATING_POINT)

#define END_OF_CHAIN ((PEXCEPTIONREGISTRATIONRECORD) -1)

#define EXCEPTION_MAXIMUM_PARAMETERS 4

typedef _Packed struct _FPREG {
    ULONG      losig;
    ULONG      hisig;
    USHORT     signexp;
} FPREG, *PFPREG;

typedef struct _CONTEXTRECORD {
    ULONG     ContextFlags;
    ULONG     ctx_env[7];
    FPREG     ctx_stack[8];
    ULONG     ctx_SegGs;
    ULONG     ctx_SegFs;
    ULONG     ctx_SegEs;
    ULONG     ctx_SegDs;
    ULONG     ctx_RegEdi;
    ULONG     ctx_RegEsi;
    ULONG     ctx_RegEax;
    ULONG     ctx_RegEbx;
    ULONG     ctx_RegEcx;
    ULONG     ctx_RegEdx;
    ULONG     ctx_RegEbp;
    ULONG     ctx_RegEip;
    ULONG     ctx_SegCs;
    ULONG     ctx_EFlags;
    ULONG     ctx_RegEsp;
    ULONG     ctx_SegSs;
} CONTEXTRECORD, *PCONTEXTRECORD;

typedef struct _EXCEPTIONREPORTRECORD {
    ULONG                         ExceptionNum;
    ULONG                         fHandlerFlags;
    struct _EXCEPTIONREPORTRECORD *NestedExceptionReportRecord;
    PVOID                         ExceptionAddress;
    ULONG                         cParameters;
    ULONG                         ExceptionInfo[EXCEPTION_MAXIMUM_PARAMETERS];
} EXCEPTIONREPORTRECORD, *PEXCEPTIONREPORTRECORD;

struct _EXCEPTIONREGISTRATIONRECORD;

typedef ULONG APIENTRY _ERR(PEXCEPTIONREPORTRECORD, struct _EXCEPTIONREGISTRATIONRECORD *,
                            PCONTEXTRECORD, PVOID);

typedef struct _EXCEPTIONREGISTRATIONRECORD {
    struct _EXCEPTIONREGISTRATIONRECORD *prev_structure;
    _ERR                                *ExceptionHandler;
} EXCEPTIONREGISTRATIONRECORD, *PEXCEPTIONREGISTRATIONRECORD;

#endif
