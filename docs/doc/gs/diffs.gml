.chap *refid=diffs Release Notes for &product &ver..&rev
.*
.np
There are a number of enhancements and changes in this new version of
&product..
The following sections outline most of the major changes.
You should consult the User's Guide for full details on these and
other changes made to the compiler and related tools.
You should check the next section to determine if you need to
recompile your application.
.*
.if '&lang' eq 'C/C++' .do begin
.*
.*
.section Differences from Open Watcom Version 1.2
.*
.np
Following is a list of changes made in &product 1.3:
:cmt Reflects main Perforce branch as of 2004/06/25
:cmt Good way to get list of changes since certain date:
:cmt p4 changes -l @yyyy/mm/dd,#head
.begbull
.bull
The C++ compiler now restricts the scope of variables declared in a for
loop to the scope of that loop in accordance with ISO C++, not extending
the scope beyond the loop (ARM compliant behaviour). Code relying on the
pre-standard behaviour must either be changed or compiled with new -zf
switch which reverts to old scoping rules.
.bull
Support for default template arguments has been added to the C++ compiler.
.bull
Support for alternative tokens (and, xor etc.) has been added to the C++
compiler. It is enabled by default, can be turned off with the new -zat
switch.
.bull
The C runtime library has been made significantly more C99 compliant. A number
of new headers have been added (inttypes.h, stdbool.h, stdint.h, wctype.h) and
corresponding new functions implemented. Wide character classification
functions were moved out of ctype.h into wctype.h. C99 va_copy macro was
added to stdarg.h.
.bull
Added 'cname' style C++ headers.
.bull
Support for SSE, SSE2, SSE3 and 3DNow! instruction sets has been added. Affected
tools are the assembler (wasm), as well as all x86 compilers, disassembler and
debugger. The debugger now also supports MMX registers formatted as floats
(for 3DNow!) as well as a new XMM register window for SSE.
.bull
Inline assembler directives .MMX, .K3D, .XMM, .XMM2 and .XMM3 are now supported in
the _asm as well as #pragma aux style inline assembler interface. Note: .MMX
directive is now required (in addition to .586) to use MMX instructions.
.bull
C compiler performance has been significantly improved (up to 5-10 times speedup)
when compiling large and complex source files.
.bull
All x86 compilers now have the ability to perform no truncation when converting
floating point values to integers. Additionally, 32-bit x86 compilers have the
option to inline the rounding code instead of calling __CHP.
.bull
The C lexical scanner no longer evaluates constants with (U)LL suffix that
fit into 32 bits as zero (1ULL was wrong, LONGLONG_MAX was correct).
.bull
C and C++ x86 inline assembler has been fixed to properly process hexadecimal
constants postfixed with 'h'.
.bull
The C compiler now supports the C99 'inline' keyword, in addition to previously
supported '_inline' and '__inline' keywords.
.bull
The C compiler now treats a sequence of adjacent character strings as wide if
any of the components are wide (required by C99), instead of depending on the
type of the last component. For example, L"foo " "bar" is now interpreted as
L"foo bar", instead of "foo bar".
.bull
The internal C compiler limit on complex expressions has been increased
and if it is still insufficient, the compiler now reports an error instead of
crashing.
.bull
The C compiler now issues a warning on the default warning level if a function
with no prototype is referenced. This was previously warning W301 (level 3), now
it is warning W131 (level 1).
.bull
Warning "W132: No storage class or type specified" has been added to the C compiler.
This warning is issued if a variable is declared without specifying either storage
class or type. This is not allowed in C89.
.bull
Warning "W304: Return type 'int' assumed for function 'foo'" has been added.
This warning is issued if a function is declared without specifying return type.
This is allowed in C89 but not in C99.
.bull
Warning "W305: Type 'int' assumed in declaration of 'foo'" has been added to the
C compiler. This warning is issued if a variable is declared without specifying
its type. This is allowed in C89 but not in C99. Note that if warning W132 is
issued, W305 applies as well.
.bull
The C compiler now properly warns if a function with implied 'int' return type
fails to return a value. This potential error was previously undetected.
.bull
C++ compiler diagnostic messages have been made more consistent and slightly more
detailed.
.bull
Linker for Win32 targets can now create file checksums. These are primarily used
for DLLs and device drivers, but can be applied to all Win32 PE images if required.
.bull
Linker for Win32 targets can now set operating system version requirements into
the PECOFF optional header (Microsoft extended header).
.bull
Linker for Win32 targets can now set the linker version number into the PE
optional header (Microsoft extended header).
.bull
The linker will now eliminate zero-sized segments from NE format (16-bit OS/2
and Windows) executables. This fixes a problem where Windows 3.x would refuse
to load an executable with zero sized segment. This could happen especially
with C++ programs where some segments may have ended up empty after eliminating
unused functions.
.bull
The linker now (again) produces correct Watcom style debugging information. This
was a regression introduced in previous version.
.bull
Command line parsing for wccxxx, wppxxx and cl has been changed such that a
double backslash inside a quoted string is collapsed to a single backslash,
and hence "foo\\" now translates to 'foo\' and not 'foo\"'.
.bull
The IDE and other graphical tools no longer leak system resources (a bug introduced
in version 1.2).
.bull
The Image Editor limit on bitmap size has been changed from 512x512 pixels to
2048x2048 pixels.
.bull
The source browser now correctly decodes array information; Version 11.0c of
Watcom C/C++ started emitting array browse information in a new format and the
browser hadn't been updated accordingly.
.bull
The NT debugger trap file has been changed so an exception generated during a
step operation is handled correctly. Previously, the single step flag was not being
cleared and when the exception was being offered to the debuggee's own exception
handlers, a single step exception occurred in NT's exception handler rather than
the exception being passed back to our handler.
.bull
The OS/2 debuggers now dynamically allocate buffer for the command line,
preventing crashes when the command line was over approx. 260 bytes long.
.bull
The NetWare 5 debugger NLM has been changed to use kernel primitives. Previous
version were using legacy semaphores.
.bull
The make program (wmake) has been sped up very slightly. Also the 'echo' command
is now internal and no longer spawns the system command interpreter.
.bull
The precision of DBL_MAX, DBL_MIN and DBL_EPSILON has been increased; the non-standard
variants prefixed with an underscore have been removed.
.bull
The C99 functions atoll(), lltoa(), ulltoa(), strtoll(), strtoull() and corresponding
wide character functions have been added to the C runtime library.
.bull
The _beginthread() function now consistently returns -1 in case of error on
all platforms.
.bull
The stdaux and stdprn streams are now only defined on DOS based platforms, ie.
DOS, Win16 and Win386. No other platforms support stdaux or stdprn.
.bull
The assert() macro now prints function name in addition to source file and
line number, in accordance with C99.
.bull
The _heapchk() function will now always perform a consistency check on the
heap, where it would previously only check consistency if there had been
allocations/frees since last call to _heapchk(). As a consequence, _heapchk()
previously did not detect certain instances of heap corruption.
.bull
[OS/2 32-bit] The default __disallow_single_dgroup() implementation no longer
statically links agaist PMWIN.DLL. This allows DLLs to load on systems where
PMWIN.DLL isn't present.
.bull
[OS/2 32-bit] Re-implemented clock(). The new implementation uses the OS
millisecond counter and is hence not susceptible to TZ changes. It is also
smaller, faster and more accurate, although it may wrap around earlier than
the original implementation.
.bull
The disassembler (wdis) now correctly processes x86 'push 8-bit immediate'
instructions.
.bull
The disassembler now correctly processes absolute memory references. All memory
references without fixup are now disassembled as ds:[...] or sreg:[...].
.bull
Several DirectX Win32 programming samples have been added. Note that a separate
DirectX SDK (available from Microsoft) is required to build these sample programs.
.endbull
.*
.*
.section Differences from Open Watcom Version 1.1
.*
.np
Following is a list of changes made in &product 1.2:
:cmt Reflects main Perforce branch as of 2003/12/1
.begbull
.bull
Handling of default libraries for Win32 targets has been changed. Previously
default library records were included in the C runtime library, now they are
specified part of the wlink Win32 target definitions in wlsystem.lnk. The
list of libraries has changed from previous version as well and now reflects
the Microsoft compiler defaults; existing IDE projects and makefiles may need
to have additional libraries specified if those are no longer part of the
default list.
.bull
The C compiler now performs stricter checking on function prototypes
and pointer operations. This may lead to previously undiagnosed
warnings/errors appearing when compiling incorrect or ambiguous code.
.bull
The C compiler diagnostic messages have been improved to print more
information, making it easier to isolate the problem.
.bull
A new warning (W130) has been added to the C compiler to diagnose possible
precision loss on assignment operations. This warning is never on by default
and must be enabled through '#pragma enable_message(130)' or '-wce=130'
switch.
.bull
Support for C99 style variable argument macros (and __VA_ARGS__) has
been added to the C and C++ compilers.
.bull
Added support for the __func__ symbol (name of the current function)
which is equivalent to the already existing __FUNCTION__ symbol.
.bull
Better C99 style support for "long long" type is now available in the
C and C++ compilers. LL, ULL and LLU suffixes are recognized for constants.
"long long int" is now also recognized.
.bull
Added C99 style *LLONG_MIN/MAX defines to limits.h.
.bull
The C++ compiler has been fixed to properly accept source files where
a template was the last item in a namespace or an external linkage.
.bull
Several new -adxx options have been added to the C and C++ compilers
to support automatic generation of 'make' style dependency files.
.bull
The C compiler has been fixed to correctly diagnose illegal union
assignments.
.bull
The C compiler now issues warnings on operations involving pointers
to different but compatible unions.
.bull
The C and C++ compilers now ensure word alignment of wide character
string literals to satisfy Win32 API restrictions.
.bull
The __UNIX__ macro is now supported in C and C++ compilers, wmake and
wasm. It is currently defined for QNX and Linux targets.
.bull
Default windowing support has been re-enabled for Win16 and Win386
runtime libraries.
.bull
Since default windowing is no longer supported on most platforms, the
Programmer's Guide and IDE tutorial have been updated to reflect that
fact.
.bull
The Win32 GUI tools now support the Windows XP look and feel.
.bull
AutoCAD and MFC targets have been removed from the IDE, the -bw switch
(default windowing) is no longer available in the IDE for OS/2 and Win32
targets.
.bull
Manual for the CauseWay DOS extender has been added.
.bull
The dmpobj tool has been added. This utility dumps the contents of OMF
object files and can be useful to developers.
.bull
Several system definitions have been added to wlink: os2_pm (16-bit OS/2
Presentation Manager executable), os2_dll (16-bit OS/2 DLL) and os2v2_dll
(32-bit OS/2 DLL).
.bull
The linker has been fixed to read "AR" style archives produced by third
party tools.
.bull
The linker has been fixed to prevent crashes when linking with COFF files
providing uninitialised COMDAT entries
.bull
Several linker crashes related to ELF object files and executables have
been resolved.
.bull
Updated wlink to call wlib with the -c (case sensitive) option when
creating import libraries. This fixes problems with DLLs that export
symbols differring only in case.
.bull
The C runtime library has been optimized to produce smaller
executables.
.bull
The printf() function now supports the "ll" format specifier for
"long long" integers.
.bull
The printf() function has been enhanced to support %b format specifier
for bitfields.
.bull
Execution order of C library termination routines is now better
defined to prevent instances where temporary files created through
mktemp() could be left behind.
.bull
[OS/2 32-bit] To prevent crashes, termination code is not run if second
instance of a DLL failed to load due to single DGROUP.
.bull
[OS/2 32-bit] The __grow_handles() function was incorrectly adding n
requested handles to existing limit instead of setting the limit to n.
.bull
[OS/2 32-bit] Fixed a problem with _STACKLOW in multithreaded programs
and DLLs. This prevents crashes where Fortran DLLs would run out of stack.
.bull
[OS/2 16-bit] Fixed default math exception handler which wasn't popping
the FP status word off the stack and would therefore crash on exit.
.bull
The Win32 Image Editor has been enhanced with drag-and-drop support.
.bull
The IDE has been fixed to properly handle mixed case filenames.
.bull
The Microsoft compatibility tools (NMAKE, CL) have been fixed to better
handle command line arguments.
.bull
The Dialog Editor (wde) has been fixed to prevent occasional DDE related
crashes when run from inside the Resource Editor (wre).
.bull
The 'Change font' option no longer crashes the GUI debugger (wdw).
.bull
On OS/2, wdw now intercepts the F10 key instead of passing it on
to the system.
.bull
The code generator now deletes object files if it was interrupted.
Previously zero-length invalid object files could be left behind,
interfering with make operation.
.bull
The wasm assembler has been enhanced to generate file dependency information
usable by wmake.
.bull
Numerous minor fixes have been made to wasm.
.bull
Compatibility with MASM 6 has been improved with wasm.
.bull
Support for sysenter and sysexit instructions has been added to wasm
and wdis.
.bull
Disassembly of xchg and bound instructions has been fixed in wdis
(corrected order of operands).
.bull
Several previously undocumented wmake directives have been documented.
.bull
A -sn ('noisy') option has been added to wmake to print all commands
that wmake executes, including silent ones.
.bull
The w32api project has been updated to the latest version.
.bull
The os2api project has been enhanced - added multimedia headers and
libraries and numerous fixes have been made to the header files.
.bull
The debugger now supports the F7 key as a shortcut for "run to cursor".
This is consistent with CodeView.
.bull
New internal variable dbg$ntid (next thread id) has been added to the
debugger. This permits automated iteration of all threads.
.bull
The wsample tool has been updated to dynamically allocate storage for
command line, where previously the command line length was limited to
128 characters.
.bull
The FORTRAN compiler has been changed to preserve case of symbols with
linkage other than FORTRAN. This is especially helpful when calling OS
API functions and using case sensitive link step (now default).
.endbull
.*
.*
.section Differences from Open Watcom Version 1.0
.*
.np
A number of problems has been corrected in &product 1.1.
.begbull
.bull
Using the -ol option (loop optimization) sometimes resulted in generation
of incorrect code.
.bull
The
.kw printf()
function could access too much data when formatting strings, causing page
faults.
.bull
NANs, INFs and denormals were not handled correctly by the math emulation
library.
.bull
The assembler did not generate implicit segment override prefixes when
accessing data in code segment.
.bull
The
.kw clock()
function sometimes produced incorrect results on OS/2 if the
.mono TZ
environment variable was set.
.bull
The Open Watcom editor (vi/viw) has been changed to store temporary files in directory
designated by the
.mono TMP
environment variable, instead of using the
.mono TMPDIR
environment variable.
.bull
Many packaging problems (missing files) have been resolved.
.endbull
.*
.*
.section Differences from Version 11.0
.*
.np
&product 1.0 is not substantially different from Watcom &lang
version 11.0. There are however several changes that may require you
to change your source and/or makefiles or project files.
.begbull
.bull
The C compiler now implements stricter function prototype processing. This
may result in new warning or error messages on code that was previously
compiling cleanly but contained mismatches between function prototypes
and actual definitions. The C++ compiler did not have this problem.
.bull
The linker now defaults to case sensitive symbol resolution. In most
cases this is not a problem and may in fact avoid certain problems.
If your project relies on case insensitive linking, please add
.kw OPTION NOCASEEX
to your linker directives.
.bull
Default windowing support has been removed from the runtime libraries.
.bull
Components of the Win32 SDK and OS/2 Toolkits are no longer supplied.
.bull
MFC libraries, source code and documentation are no longer supplied.
.endbull
.if &version ge 110 .do begin
.*
.section Changes in 11.0 that may Require Recompilation
.*
.np
Do not attempt to mix object code generated by earlier versions of the
compilers with object code generated by this release or with the
libraries provided in this release.
.np
A new C++ object model has been implemented.
If you have undefined references to
.mono __wcpp_3_*
names, you have old object code.
If you have undefined references to
.mono __wcpp_4_*,
you have old libraries and new object code.
.begnote
.note clock()
The
.kw clock
function accuracy has changed from 100 ticks per second to 1000 ticks
per second (i.e.,
.kw CLOCKS_PER_SEC
has changed).  Source code that
uses the
.kw clock
function and
.kw CLOCKS_PER_SEC
in its calculations must be recompiled before linking the application
with new libraries.
.endnote
.*
.section Major Differences from Version 10.6
.*
.np
The following sections summarize the major differences from the
previous release.
.begbull
.bull
In general, we have improved Microsoft compatibility in our compilers
(more warnings instead of errors, support for MS extensions, etc.) and
tools.
.bull
Some of the Win32 and 32-bit OS/2 versions of our tools are now
available in DLL form.
.millust begin
EXE      DLL        Description
------   -------    -----------------------
wcc      wccd       16-bit x86 C compiler
wcc386   wccd386    32-bit x86 C compiler
:cmt. wccaxp   wccdaxp    Alpha AXP  C compiler
wpp      wppdi86    16-bit x86 C++ compiler
wpp386   wppd386    32-bit x86 C++ compiler
:cmt. wppaxp   wppdaxp    Alpha AXP  C++ compiler
wlink    wlink      &lnkname
wlib     wlibd      &libname
.millust end
.np
This provides better performance when using the &ide or &makname..
See the description of the
.mono !loaddll
preprocessing directive in &makname for more information.
:cmt. .bull
:cmt. The Win32 version of our tools can be used (hosted) under both Intel
:cmt. and DEC Alpha AXP platforms
:cmt. and can create Win32 applications (targets) for both Intel or DEC
:cmt. Alpha AXP platforms.
.endbull
.*
.beglevel
.*
.section Changes to the C++ Compiler for 11.0
.*
.begbull
.bull
The C++ compiler now optimizes empty base-classes to occupy zero
storage in the derived class memory layout.
The C++ Working Paper recently allowed this optimization to be
performed by conforming implementations.
Furthermore, the optimization has speed and size benefits.
There are certain classes of (broken) C++ programs that may not
function properly with the new optimization.
If you explicitly memset() an empty base class, you will be clearing
memory that you may not expect to be cleared since the "zero sized"
base class in actual fact shares storage with the first member of the
derived class.
A memset() of the entire derived class is fine though.
.bull
We have added support for the
.mono mutable
keyword which is used to indicate data members that can be modified
even if you have a
.us const
pointer to the class.
.exam begin
class S {
    mutable int x;
    void foo() const;
};

void S::foo() const {
    x = 1;  // OK since it is mutable
}
.exam end
.bull
We have added support for the
.mono bool
type along with
.mono true
and
.mono false.
.bull
We have added support for the
.mono explicit
attribute.
It marks a constructor so that it will not be considered for
overloading during implicit conversions.
.exam begin
struct S {
    explicit S( int );
};

S v = 1;    // error; cannot convert 'int' to 'S'
.exam end
.np
Suppose the class was changed as follows:
.exam begin
struct S {
  explicit S(int );
  S( char );
};

S v = 1;  // OK; S( char ) is called
.exam end
.np
The fact that
.mono S(int)
is not considered leaves
.mono S(char)
as the only way to satisfy the implicit conversion.
.bull
We have added support for namespaces.
.millust begin
namespace x {
    // anything that can go in file-scope
}
namespace {
    // anything in here is local to your module!
}
.millust end
.np
In the above example, you can access names in the namespace "x"
by "x::" scoping.
Alternatively, you can use the "using namespace x" statement
(thereby eliminating the need for "x::" scoping).
You can include a part of the namespace into the current scope with
the "using x::member" statement.
(also eliminating the need for "x::" scoping).
.autopoint
.point
Namespaces eliminate the hand mangling of names.
For example, instead of prefixing names with a distinguishing string
like "XPQ_" (e.g., XPQ_Lookup), you can put the names in a namespace
called "XPQ".
.point
Namespaces allow for private names in a module.
This is most useful for types which are used in a single module.
.point
Namespaces encourage the meaningful classification of implementation
components.
For example, code-generation components might reside in a namespace
called "CodeGen".
.endpoint
.bull
We have added support for RTTI (Run-Time Type Information).
.bull
We have added support for the new C++ cast notation.
It allows you to use less powerful casts that the all powerful C-style
cast and to write more meaningful code.
The idea is to eliminate explicit casts by using a more meaningful
new-style cast.
The new C++ casts are:
.np
.bi reinterpret_cast
.us < type-id >(expr)
.br
.bi const_cast
.us < type-id >( expr )
.br
.bi static_cast
.us < type-id >( expr )
.br
.bi dynamic_cast
.us < type-id >( expr )
(part of RTTI)
.bull
We have improved (faster) pre-compiled header support.
.bull
.ix '__int64'
We have added "long long" (64-bit integer) support in the form
of a new
.us __int64
type.
.bull
The default structure packing was changed from "zp1" to "zp2" in the
16-bit compiler and from "zp1" to "zp8" in the 32-bit compiler.
.bull
The default type of debugging information that is included in object
files is "Dwarf".
It used to be "Watcom".
.bull
A new double-byte string processing option has been added (zkl).
When this option is specified, the local or current code page
character set is used to decide if the compiler should process strings
as if they might contain double-byte characters.
.endbull
.*
.section Changes to the C Compiler for 11.0
.*
.begbull
.bull
We have improved (faster) pre-compiled header support.
.bull
.ix '__int64'
We have added "long long" (64-bit integer) support in the form
of a new
.us __int64
type.
.bull
The default structure packing was changed from "zp1" to "zp2" in the
16-bit compiler and from "zp1" to "zp8" in the 32-bit compiler.
.bull
The default type of debugging information that is included in object
files is "Dwarf".
It used to be "Watcom".
.bull
A new double-byte string processing option has been added (zkl).
When this option is specified, the local or current code page
character set is used to decide if the compiler should process strings
as if they might contain double-byte characters.
.endbull
.*
.section Changes to the Code Generator for 11.0
.*
.begbull
:cmt. .bull
:cmt. Generation of code for the Digital Equipment Corporation Alpha AXP
:cmt. processor is now possible.
:cmt. .bull
:cmt. For the DEC Alpha AXP, we support Microsoft-compatible calling conventions
:cmt. and in-line assembly formats.
.bull
We support Microsoft-compatible in-line assembly formats using the
"_asm" keyword.
.bull
A new optimization, "branch prediction", has been added.
This optimization is enabled by the "ob" or "ox" compiler options.
The code generator tries to increase the density of cache use by
predicting branches based upon heuristics (this optimization
is especially important for Intel's Pentium Pro).
.bull
We have added Multi-media Extensions (MMX) support to the in-line
assemblers.
.bull
.ix '__int64'
We have added "long long" (64-bit integer) support in the form
of a new
.us __int64
type.
.endbull
.*
.section Changes to the Compiler Tools for 11.0
.*
.begbull
:cmt. .bull
:cmt. The &dbgname now supports debugging of applications running on the DEC
:cmt. Alpha AXP processor.
.bull
The &lnkname supports incremental linking.
.bull
The &lnkname can now process COFF and ELF format object files, as
well as OMF et al.
The &lnkname can now read both AR-format (Microsoft compatible)
libraries and old-style OMF libraries.
.bull
Support for creating 16-bit DOS overlaid executables has been removed
from the linker.
.bull
The &libname (&libcmdup) can now process COFF and ELF format object
files, as well as OMF et al.
The &libname can now read/write both AR-format (Microsoft compatible)
libraries and old-style OMF libraries.
The default output format is AR-format and this can be changed by
switches.
The &libname can output various format import libraries.
.bull
We have added Multi-media Extensions (MMX) support to the &asmname
(&asmcmdup).
:cmt. .bull
:cmt. A new assembler for the DEC Alpha AXP is included in the package.
.bull
A new version of the &disname (&discmdup) can disassemble Intel
or Alpha AXP object code files.
It can process ELF, COFF or OMF object files and ELF, COFF or PE
format (Win32) executables.
The &disname looks at image file being input to determine the
processor-type (defaults to Intel).
.np
The old disassembler (WDISASM) has been retired and is not included in
the package.
.bull
We have added new tool front-ends that emulate Microsoft tools.
These are:
.begbull $compact
.bull
nmake
.bull
cl
.bull
link
.bull
lib
.bull
rc
.bull
cvtres
.endbull
.np
These programs take the usual Microsoft arguments and translate them,
where possible, into equivalent &company arguments and spawn the
equivalent &company tools.
.bull
&makname now processes Microsoft format makefiles when the "ms" option
is used.
.endbull
.*
.section Changes to the C/C++ Libraries for 11.0
.*
.begbull
.bull
We have added multi-byte and wide character (including UNICODE)
support to the libraries.
.bull
We include run-time DLLs for the C, Math and C++ Libraries.
.bull
We have added Multi-media Extensions (MMX) support to the libraries.
.bull
The following new functions were added to the library...
.begnote
.note multi-byte functions
.endnote
.bull
The
.kw clock
function accuracy has changed from 100 ticks per second to 1000 ticks
per second (i.e.,
.kw CLOCKS_PER_SEC
has changed).
.bull
A "commit" flag ("c") was added to the fopen()
.us mode
argument.
.bull
.ix 'BINMODE'
The global translation mode flag default is "text" unless you
explicitly link your program with
.fi BINMODE.OBJ.
.bull
Processing of the "0" flag in the format string for the printf()
family of functions has been corrected such that when a precision is
specified, the "0" flag is ignored.
.exam begin
printf( "%09.3lf\n", 1.34242 ); // "0" flag is ignored
printf( "%09lf\n", 1.34242 ); // "0" flag is not ignored
.exam end
.bull
.ix '__int64'
Support for printing
.us __int64
values was added to
.kw printf
and related functions.
.bull
.ix '__int64'
Support for scanning
.us __int64
values was added to
.kw scanf
and related functions.
.bull
The Win32
.kw _osver
variable was added to the library.
.bull
The Win32
.kw _winmajor,
.kw _winminor
and
.kw _winver
variables were added to the library.
.endbull
.*
.section Changes to the DOS Graphics Library for 11.0
.*
.begbull
.bull
The graphics library now performs the VESA test
.us before
testing for vendor specific graphics cards.
This fix is intended to broaden the number of graphics cards that are
supported.
.endbull
.*
.section Changes in Microsoft Foundation Classes Support for 11.0
.*
.begbull
.bull
Version 4.1 of the 32-bit MFC is included in the package.
.bull
Version 2.52b of the 16-bit MFC is included in the package.
.endbull
.*
.section Changes in Microsoft Win32 SDK Support for 11.0
.*
.begbull
.bull
The Win32 SDK is supported for Windows 95 and Windows NT platforms.
.endbull
.*
.section Changes in Blue Sky's Visual Programmer for 11.0
.*
.begbull
.bull
A new 32-bit version of Visual Programmer is included in the package.
This version runs on 32-bit Windows 95 and NT.
The 16-bit version of Visual Programmer is no longer included in the
package.
.bull
You can generate 16-bit applications with it, but you must be careful
to avoid using Win95 controls.
.bull
This new version fixes all known bugs in the previous version.
.endbull
.*
.endlevel
.*
.do end
.*
.if &version ge 106 .do begin
.*
.section Changes in 10.6 that may Require Recompilation
.*
.begnote
.note _diskfree_t
The struct members of the _diskfree_t structure has been changed from
UNSIGNED SHORTs to UNSIGNED INTs. This is to deal with possible HPFS
partitions whose size will overflow a short, as well as Microsoft
compatibility.
.note clock()
The
.kw clock
function accuracy has changed from 100 ticks per second to 1000 ticks
per second (i.e.,
.kw CLOCKS_PER_SEC
has changed).  Source code that
uses the
.kw clock
function and
.kw CLOCKS_PER_SEC
in its calculations must be recompiled before linking the application
with new libraries.
.endnote
.*
.section Major Differences from Version 10.5
.*
.np
The following sections summarize the major differences from the
previous release of Watcom &lang..
.*
.beglevel
.*
.section Windows 95 Help File Format
.*
.np
We have included Windows 95 format help files.
.*
.section Changes to the C++ Compiler in 10.6
.*
.np
We have improved Microsoft compatibility so that Win32 SDK and MFC
header files can be compiled without change.
The following changes were required to support Win32 SDK header files.
.begbull
.bull
We recognize the single underscore versions of
.mono __stdcall,
.mono __inline,
and
.mono __fastcall
keywords.
.bull
The
.mono _fastcall
and
.mono __fastcall
keywords are scanned but ignored since they refer to a particular
Microsoft code generation technique.
&company's generated code is always "fast".
.endbull
.np
The following changes were required to support MFC source code.
.begbull
.bull
When /bt=DOS is specified, define
.mono _DOS.
.bull
When /bt=WINDOWS is specified, define
.mono _WINDOWS.
.bull
When /m[s|m|c|l|h] is specified, define
.mono __SW_M[S|M|C|L|H]
and
.mono _M_I86[S|M|C|L|H]M.
.endbull
.np
The compiler now supports the C++ Standard Template Library (STL).
This library is available at the ftp site "butler.hpl.hp.com".
When compiling applications that use the STL, you must use the
"hd" compiler option for debugging info (the "hw" option causes
too much debug information to be generated).
.*
.section Changes to the C Compiler in 10.6
.*
.np
We have improved Microsoft compatibility so that Win32 SDK and MFC
header files can be compiled without change.
The following changes were required to support Win32 SDK header files.
.begbull
.bull
Support for the single underscore version of the
.mono __stdcall
keyword.
.bull
When /bt=DOS is specified, define
.mono _DOS.
.bull
When /bt=WINDOWS is specified, define
.mono _WINDOWS.
.endbull
.np
The following changes were required to support SDK sample code.
.begbull
.bull
You can specify calling convention information in a function prototype
and you do not have to specify the same information in the definition.
(Note: This is required by the OS/2 Warp SDK samples.)
.bull
Structured exception handling is supported (
.ct
.mono __try,
.mono __except
and
.mono __finally
keywords).
.bull
Allow initialization of automatic array/struct data using variables
and function calls.
.endbull
.*
.section Changes to the C Library in 10.6
.*
.np
The following new functions were added to the library.
.begnote
.note _getw
read int from stream file
.note _putw
write int to stream file
.endnote
.np
The
.kw clock
function accuracy has changed from 100 ticks per second to 1000 ticks
per second (i.e.,
.kw CLOCKS_PER_SEC
has changed).
.*
.section Changes in Microsoft Foundation Classes Support for 10.6
.*
.begbull
.bull
Version 3.2 of the 32-bit MFC is included in the package.
.bull
Version 2.52b of the 16-bit MFC is included in the package.
.endbull
.*
.section Changes to the Image Editor in 10.6
.*
.begbull
.bull
Support has been added for 256 colour bitmaps.
.bull
Support has been added for 16 X 16 icons.
.bull
Support has been added for 48 X 48 icons.
.endbull
.*
.section Changes to the Dialog Editor in 10.6
.*
.begbull
.bull
Support has been added for Windows 95 controls.
.bull
Support has been added for adding new control styles to existing
controls.
.bull
Support has been added for new dialog styles.
.bull
Support has been added for allowing help IDs to be specified in dialog
and control statements.
.bull
Support has been added for generating new resource statements in .RC
files.
.endbull
.*
.section Changes to the Resource Editor in 10.6
.*
.begbull
.bull
Support has been added for new Windows 95 DIALOGEX resource type.
.bull
Support has been added for generating new DIALOGEX resource statements
in .RC files.
.endbull
.*
.section Changes to the Resource Compiler in 10.6
.*
.begbull
.bull
Support has been added for extended styles for dialogs.
.bull
Support has been added for the RCINCLUDE keyword.
.endbull
.*
.endlevel
.*
.do end
.*
.section Major Differences from Version 10.0
.*
.begbull
.bull
New installation program
.if &e'&beta eq 0 .do begin
.bull
&vpname for Windows (MFC) applications
.do end
.bull
MFC 3.0 support
.bull
Native C++ exception handling support
.bull
Improved language compatibility with Microsoft
.bull
Browser can now be used to browse C code
.bull
OS/2 3.0 Warp support
.bull
Toolkit for OS/2 1.3
.bull
Windows NT 3.5 support
.bull
Toolkit for Windows NT 3.5
.bull
Windows 95 (Chicago) support
.bull
Source Revision Control System hooks in editor
.bull
TCP/IP remote debug servers for OS/2 and Windows NT/95
.endbull
.np
In addition to these new features, we have also made a number of
improvements to the software.
.autonote
.note
The editor is more tightly integrated with the IDE.
.note
It is now easier to select your own favourite editor from the IDE.
.note
The keyboard interface in the Integrated Development Environment (IDE)
has been improved.
.note
The "fr" option, which is supported by the compilers & assembler,
can be used to name the error file drive, path, file name and/or
extension.
.note
We have added the "t<number>" option to the C++ compiler to set the
number of spaces in a tab stop (for column numbers in error messages).
.note
The C compiler now supports @filename on the command line like the C++
compiler currently does.
.note
The "__stdcall" linkage convention has changed.
All C symbols (extern "C" symbols in C++) are now suffixed by "@nnn"
where "nnn" is the sum of the argument sizes (each size is rounded up
to a multiple of 4 bytes so that char and short are size 4).
When the argument list contains "...", the "@nnn" suffix is omitted.
This was done for compatibility with Microsoft.
Use the "zz" option for backwards compatibility.
.note
The 32-bit "__cdecl" linkage convention has changed.
Watcom &lang 10.0 __cdecl did not match the Microsoft Visual C++ __cdecl
in terms of the binary calling convention; Visual C++ saves EBX in a
__cdecl function but Watcom &lang 10.0 modified EBX.
Watcom &lang has been changed to match Visual C/C++.
.np
If you wrote a "__cdecl" function in an earlier version of Watcom &lang,
the EBX register was not saved/restored.
Starting with release 10.5, the EBX register will be saved/restored
in the prologue/epilogue of a "__cdecl" function.
.np
Another implication of this change is that "__cdecl" functions
compiled with an earlier version of Watcom &lang don't match the calling
conventions of the current version.
The solution is either to recompile the functions or to define a
"__cdecl_old" pragma that matches the old calling conventions.
.millust begin
#pragma aux __cdecl_old "_*" \
            parm caller [] \
            value struct float struct routine [eax] \
            modify [eax ebx ecx edx];

#pragma aux (__cdecl_old) foo;

extern int foo( int a, int b );

void main()
{
    printf( "%d\n", foo( 1, 2 ) );
}

.millust end
.note
We now allow:
.millust begin
extern "C" int __cdecl x;
.millust end
.np
It must be
.id extern "C"
for
.id __cdecl
to take effect since variables have their type mangled into the name
for "C++" linkage.
.note
In C++, we have removed the warning for "always true/false"
expressions if the sub-expressions are constant values.
.note
We have added support for:
.millust begin
#pragma pack(push,4);
#pragma pack(push);
#pragma pack(pop)
.millust end
.note
We have added support for:
.millust begin
#pragma comment(lib,"mylib.lib")
.millust end
.pc
which has the same semantics as:
.millust begin
#pragma library( "mylib.lib" )
.millust end
.note
We have added support for expanding macros in the code_seg/data_seg
pragmas:
.millust begin
#define DATA_SEG_NAME "MYDATA"
#define CODE_SEG_NAME "MYCODE"

#pragma data_seg( DATA_SEG_NAME )
int x = 3;

#pragma code_seg( CODE_SEG_NAME )
int fn() {
    return x;
}
.millust end
.note
We have fixed the 16-bit compiler so that it matches the Microsoft
16-bit C compiler for the following cases:
.begbull
.bull
If a pascal function is defined when compiling for Windows 3.x, use the
fat Windows 3.x prologue in the function.
.bull
If a cdecl function is defined when compiling for Windows 3.x, use the fat
Windows 3.x prologue in the function.
.endbull
.note
We have fixed the compiler so that
.millust begin
#include </dir/file.h>
.millust end
.pc
works as expected (it was searching along the INCLUDE path only).
.note
In C++, we have fixed a problem where an import was generated in the
object file for a virtual function call.
This will reduce the size of executables under certain circumstances.
.note
In C++, we have removed the prohibition of pointer to array of unknown
size declarations.
.exam begin
int (*p)[];
.exam end
.note
In C++, we have fixed the diagnosis of lexical problems during macro
expansion to remove spurious warnings.
.exam begin
#define stringize( x )  #x

stringize( 2131231236172637126371273612763612731 )
.exam end
.note
We have corrected the check for too many bytes in #pragma for
assembler style aux #pragmas.
.note
Undeclared class names in elaborated class specifiers are now declared
in the nearest enclosing non-class scope.
Undeclared classes are also allowed in arguments now.
.exam begin
struct S {
    // used to declared ::S::N but now declares ::N
    struct N *p;
};

void foo( struct Z *p );    // declares ::Z
.exam end
.note
We have fixed unduly harsh restriction on virtual ...-style functions.
They are now allowed in single inheritance hierarchies as long as the
return type is not changed when the virtual function is overridden. In
multiple inheritance hierarchies, an implementation restriction is
still present for generating a 'this' adjustment thunk for virtual
...-style functions.
.note
We have fixed line number information for multi-line statement
expressions in some weird cases.
.note
We have fixed function template parsing of user-defined conversions
that use an uninstantiated class in their operator name.
.exam begin
void ack( int );

template <class T>
    struct S {
        S( T x )
        {
            ack( x );
        }
    };
.exam break
template <class T>
    struct W {
        operator S<T>();
    };
.exam break
template <class T>
    W<T>::operator S<T>() {
        return 0;
    }
.exam end
.note
We have fixed a compiler problem that caused a linker warning "lazy
reference for <virtual-fn> has different default resolutions" in cases
where the compiler or programmer optimized virtual function calls to
direct calls in modules that also contained virtual calls.
.exam begin
T.H
    struct S {
        virtual int foo() { return __LINE__; }
    };
    struct T : S {
        virtual int foo() { return __LINE__; }
    };
.exam break
T1.CPP
    #include "t.h"
    struct Q : T {
        virtual int foo() { return S::foo() + __LINE__; }
    };

    void foo( T *p )
    {
        Q y;
        y.foo();
        p->foo();
    }
.exam break
T2.CPP
    #include "t.h"

    void foo( T *p );

    void ack( T *p ) {
        p->foo();
        foo(p);
    }
.exam break
    main() {
        T q;
        ack( &q );
    }
.exam end
.note
When a class value is returned and is immediately (in the same
expression) used to call a member function, the value may not be
stored in memory.
.np
Work around: introduce a temporary
.exam begin
struct S {
    int v;
    int member();
};

S foo();
.exam break
void example( void )
{
    // foo().member();  // replace this line with:
    S temp = foo();
    temp.member();
}
.exam end
.note
Throwing pointers to functions did not work when the size of a
function pointer is greater than the size of a data pointer.
.np
Work around: place the function pointer in a class and throw the class
object.
.note
We have fixed default argument processing for const references to an
abstract class. The following example would not compile properly:
.exam begin
struct A {
    virtual int foo() = 0;
};

A &foo();
.exam break
void ack( A const &r = foo() );

void bar() {
    ack();
}
.exam end
.note
We have made "DllMain" default to extern "C" linkage for Microsoft
Visual C++ compatibility.
.note
We have duplicated a Microsoft Visual C++ extension that was required
to parse the Windows 95 SDK header files.
.exam begin
typedef struct S {
} S, const *CSP;
     ^^^^^- not allowed in ISO C or ISO C++
.exam end
.note
We now do not warn about starting a nested comment if the comment is
just about to end.
.np
We also fixed the code that figures out where a comment was started so
that a nested comment warning is more helpful.
.exam begin
        /*/////////*/
                   ^-
.exam end
.note
We have fixed a problem where extra informational notes were not being
printed for the error message that exceeded the error message limit.
.exam begin
// compile -e2
struct S {
    void foo();
};
.exam break
void foo( S const *p )
{
    p->foo();
    p->foo();
    p->foo();
    p->foo();
}
.exam end
.note
We have fixed a problem where the line number for an error message was
incorrect.
.exam begin
struct S {
    void foo() const;
    void bar();
};
.exam break
void S::foo() const
{
    bar();

    this->bar();

}
.exam end
.note
We have fixed output of browser information for instantiated function
template typedefs.
.note
We have upgraded the C++ parser so that casts and member pointer
dereferences can appear on the left hand side of the assignment
expression without parentheses.
.exam begin
p->*mp = 1;
(int&)x = 1;
.exam end
.note
In several cases, when a function return or a construction was
immediately dotted in an expression, the generated code was incorrect:
.exam begin
struct S {
  int x;
  int foo();
};
.exam break
extern S gorf();

void bar()
{
    gorf().foo();
}
.exam end
.np
The work around was to break the statement in two:
.exam begin
S temp = gorf();
temp.foo();
.exam end
.note
In several cases, when a function return or a construction was
immediately addressed in an expression, the generated code was
incorrect:
.exam begin
struct S {
  int x;
};
.exam break
extern void fun( S* );

extern S gorf();
.exam break
void bar()
{
    fun( &gorf() );
}
.exam end
.np
The work around was to break the statement in two:
.exam begin
S temp = gorf();
fun( &temp );
.exam end
.note
We have added support for:
.millust begin
#pragma error "error message"
.millust end
.np
Use the ISO/ANSI method because it is more portable and acceptable
(Microsoft header files use the less portable #pragma when there
is a perfectly fine, portable way to issue a message).
.np
The portable, acceptable method is:
.millust begin
#error "error message"
.millust end
.note
We have added support for
.mono __declspec(dllexport),
.mono __declspec(dllimport),
.mono __declspec(thread),
and
.mono __declspec(naked)
for Win32 (i.e., WinNT 3.5 and Win95) programs. Here are some
examples:
.exam begin
__declspec(dllexport) int a;        // export 'a' variable
__declspec(dllexport) int b()       // export 'b' function
{
}
.exam break
struct __declspec(dllexport) S {
    static int a;                   // export 'a' static member
    void b();                       // export 'b' member fn
};
.exam break
extern __declspec(dllimport) int a; // import 'a' from a .DLL
extern __declspec(dllimport) int b();//import 'b' from a .DLL

struct __declspec(dllimport) I {
    static int a;                   // import 'a' static member
    void b();                       // import 'b' member fn
};
.exam end
.note
The C++ compiler generates better error messages for in-class
initializations and pure virtual functions.
.exam begin
struct S {
    static int const a = 0;
    static int const b = 1;
    void foo() = 0;
    void bar() = 1;
    virtual void ack() = 0;
    virtual void sam() = 1;
};
.exam end
.note
We have fixed macro processing code so that the following program
compiles correctly. The compiler was not treating "catch" as a keyword
after the expansion of "catch_all".
.exam begin
#define catch(n) catch(n &exception)
#define xall (...)
#define catch_all catch xall
.exam break
main()
{
    try{
    }
    catch_all{
    }
}
.exam end
.note
We have fixed a problem where
.mono #pragma code_seg
caused a page fault in the compiler when the code_seg was empty.
.note
We have fixed a rare problem where a #include of a file that was
previously included caused the primary source file to finish up if the
CR/LF pair for the line that the #include was on, straddled the C++
compiler's internal buffering boundary.
.note
We have added support for
.mono #pragma message( "message text" ).
It outputs a message to stdout when encountered. It is used in
Microsoft SDK header files to warn about directly including header
files and obsolete files.
.note
We have fixed #pragma code_seg/data_seg to properly set the class name
of the new segment in the object file.
.note
We have a fixed a problem with the -zm -d2 options that caused a
compiler fault in some circumstances.
.note
We have fixed default library records in .OBJ file so that user
libraries are ahead of default compiler libraries in the linker search
order.
.note
We have fixed handling of intrinsic math functions so that the code
generator will treat functions like sqrt as an operator.
.note
We have added support for using OS-specific exception handling
mechanisms for C++ exception handling during code generation. Enable
it with the new -zo option.
.note
.mono __stdcall
functions now have Microsoft Visual C/C++ compatible name mangling.
.note
We have added a number of new functions to the C Library. These have
been added to improve Microsoft compatibility.
.millust begin
dllmain (nt only)
libmain (nt only)
_access
_dos_commit
_dup
_ecvt
_fcvt
_fstat
_fstrdup
_gcvt
_itoa
_itoa
_locking
_lseek
_ltoa
_ltoa
_memicmp
_set_new_handler
_stat
_strdate
_strdup
_stricmp
_strlwr
_strnicmp
_strrev
_strtime
_strupr
_tolower
_toupper
__isascii
__iscsym
__iscsymf
.millust end
.note
In version 9.5, the linker used to include LIBFILE object files in
reverse order (i.e., the last one listed was the first to be
included). We have corrected this behaviour so that they are included
in the order listed.
.millust begin
Directive           Old Order   New Order
-------------       ---------   ---------
FILE    obj_a           3           3
LIBFILE obj_b           2           1
LIBFILE obj_c           1           2
FILE    obj_d           4           4
.millust end
.np
In the above example, the object files will be included in the order
indicated (LIBFILE object files are always included first).
.endnote
.*
.beglevel
.*
.section Changes in 10.5 that may Require Recompilation
.*
.begnote
.note __stdcall
If you use the __stdcall attribute within a program then you must
re-compile the function definition and all callers of the __stdcall
function.
.note __cdecl
The __cdecl attribute is not heavily used in Win32 programming so the
impact should be minimal but if you do use __cdecl within your own
programs, a re-compilation will be necessary.
.endnote
.*
.endlevel
.*
.section Major Differences from Version 10.0 LA
.*
.np
If you have .tgt files created with the Limited Availability
or Beta Integrated Development Environment, when you load
them, the target window may say "Obsolete Form: rename target type".
If it does:
.autopoint
.point
Select the target window by clicking in it,
.point
Choose "rename target" from the target menu (a rename target dialog
will appear),
.point
Reselect the target type for this target (e.g., Win32 EXE), and
.point
Select OK.
.endpoint
.np
You should not continue to use .cfg files from the Limited
Availability version of the compiler. Several new features have been
added. Using the old files will cause problems.
.np
The C++ compiler calling conventions have changed. Any program that
passes a "data only" class or struct as a parameter, or returns a C++
object will need to be recompiled. We recommend that you recompile
your application.
.np
The C++ compiler now supports the use of the
.kw __export,
.kw __cdecl,
.kw __pascal,
.kw __stdcall
and
.kw __syscall
keyword on class definitions.
These keywords will affect all members of the defined class.
.*
.section Major Differences from Watcom C9.5 /386
.*
.begbull
.bull
The functionality of Watcom C/C++(16) and Watcom C/C++(32) is included in a single
package.
.bull
An Integrated Development Environment for Windows 3.x,
Windows NT, Windows 95 and OS/2 PM is included.
.bull
New, redesigned debugger with GUI interfaces for Windows 3.x,
Windows NT, Windows 95 and OS/2 PM is included.
.bull
The optimizer has been enhanced.
.bull
C++ Class Browser
.bull
New, redesigned user interface for the Profiler.
.bull
New support for C and C++ precompiled header files.
.bull
Windows resource editing tools are included:
.begpoint $compact
.point Dialog Editor
.point Bitmap Editor
.point Resource Editor
.point Menu Editor
.point String Editor
.point Accelerator Editor
.endpoint
.bull
Windows development tools are included:
.begpoint $compact
.point Dr. Watcom (a post mortem debug utility)
.point Spy (Windows message spy program)
.point DDESpy
.point Heap Walker
.point Zoom
.endpoint
.bull
On-line documentation is included.
.bull
Microsoft Foundation Classes for 32-bit Windows applications (&mfc32)
and 16-bit Windows 3.1 applications (&mfc16) is included.
.bull
Creation of FlashTek DOS extender applications is supported.
.bull
Compiler executables have been created that run under all supported
operating systems. They are located in the BINW directory.
.endbull
.*
.beglevel
.*
.section Items No Longer Supported
.*
.begbull
.bull
PenPoint development
.bull
Debugging of Ergo OS/386 DOS extender applications
.bull
DESQView remote debugging
.endbull
.*
.section Changes in 10.0 that may Require Recompilation
.*
.np
.us All C++ applications will require recompilation
due to changes in the underlying object model.
C applications should not require recompilation, but
.us you should recompile your application if you want
.us to take full advantage new features in the debugger.
The changes to the C++ object model are:
.begbull
.bull
Virtual table layout changed (NULL entry at offset 0 removed)
.bull
derived class packing adjusted to minimize padding bytes
.bull
exception handling code is improved (incompatible with 9.5)
.bull
name mangling for 'char' reduced from two chars to one char
.endbull
.*
.endlevel
.*
.section Major Differences from Watcom C9.01 /386
.*
.begbull
.bull
C++ support added
.bull
Pentium optimizations added ("5r", "5s", "fp5" options)
.bull
Windows NT support added
.bull
Microsoft resource compiler (RC.EXE) replaced with Watcom resource
compiler (WRC.EXE)
.bull
OS/2 libraries modified so that single library supports single and
multiple threads and DLL's
.bull
"fpi287" switch renamed to "fp2"
.bull
#pragma intrinsic and #pragma function added
.bull
80x87 usage modified so that compiler assumes all eight registers are
available upon entry to function instead of only four registers.
"fpr" option added for reverse compatibility with object files compiled
with previous versions of compiler
.endbull
.beglevel
.*
.section Changes that may Require Recompilation
.*
.np
The
.us stat
structure in "stat.h" changed in version 9.5.
Any object files compiled with an earlier version of the compiler
will have to be recompiled if they use the stat structure.
.np
A new function
.us _grow_handles
was added to version 9.5 for growing the number of available file
handles.
Details on how to use this function can be found in the
.us Watcom C/C++&S'32. Commonly Asked Questions & Answers
booklet.
.np
If you compile with structure packing (/zp2, /zp4, /zp8) or use the
"pack" pragma to specify a packing value other than 1, and you have
structures that contain other structures, field offsets and structure
sizes may be different depending on the contents of the structures.
.np
In version 9.01,
the new Windows Supervisor now has 32 user-defined callbacks.
If you have any user-defined callbacks in your Windows program, you
must recompile because the constant definitions have changed.
.np
In version 9.0,
the compiler will not use FS by default to avoid conflicts with new
operating systems (OS/2 2.0, PenPoint, Windows NT).
This will cause compile errors if you have defined a pragma that
uses the FS register.
.np
In version 8.5,
the compiler was changed so that it by default does not save
and restore segment registers across function calls.
This is to solve problems that occur where a segment register is
saved and restored in a function that tries to free the segment.
When the segment register is popped from the stack in the epilogue,
a general protection exception occurs because the selector is no
longer valid.
In order to provide backward compatibility with existing code, we have
added a "-r" option that will cause the compiler to save and restore
segment registers across calls.
The C run-time library has been compiled with the "-r" option so that it
will work should you choose to compile your application with the same
option.
.np
The packing algorithm was also changed in version 8.5.
If you are using one of the "-zp2, -zp4, or -zp8" options to pack
structures, you must recompile your application.
The packing algorithm has been changed so that the minimum number of
slack bytes are inserted into structures to align fields onto their
required alignment.
.*
.endlevel
.*
.section Major Differences from Watcom C9.0 /386
.*
.begbull
.bull
Windows 3.1 SDK components
.bull
Support for Windows 3.1 DLLs
.bull
On-line Watcom C Library Reference help file for OS/2 2.0.
Help can be accessed by issuing the command "VIEW WATCOMC".
The command can be followed by a topic. e.g. VIEW WATCOMC PRINTF.
.endbull
.cp 11
.*
.beglevel
.*
.section Command Line Options added to Watcom C9.0 /386
.*
.begnote $break
.note 4r
Use register calling conventions and optimize for 486.
.note 4s
Use stack calling conventions and optimize for 486.
.note ee
Generate a call to __EPI at the end of a function
.note ep{=number}
Generate a call to __PRO at the start of a function
.note oe
In-line user defined functions.
.note or
Reorder instructions to take advantage of 486 pipelining.
.note zff
Allows the FS register to be used by the code generator for far pointers.
.note zfp
Disallows use of the FS register.
This is the default in flat memory model,
because operating systems are now using FS to
point to important information. (e.g. OS/2 2.0, PenPoint, Windows NT).
.note zm
Places each function into a separate segment.
This will allow for smart linking.
.endnote
.*
.endlevel
.*
.section Major Differences from Watcom C8.5 /386
.*
.np
Several major items have been added to Watcom C9.0 /386. They are:
.begbull $compact
.bull
486 instruction selection
.bull
486 instruction scheduling
.bull
Tail recursion elimination
.bull
Function inlining
.bull
strcmp function optimized for 486
.bull
Support for OS/2 2.0
.bull
New keywords added (_far16, _Seg16, _Packed, _Cdecl, _Pascal, _System)
.bull
Linkage pragma for compatibility with IBM C Set/2
.bull
Based pointers
.bull
Machine language code sequences can now be entered into pragmas
using assembly language instead of numeric codes.
.* .bull
.* Post-mortem debugging capability for Windows 3.x.
.bull
Remote debugging over the parallel port using either a "LapLink" cable
or a "Flying Dutchman" cable.
.bull
Remote debugging of PenPoint applications
.endbull
.cp 10
.*
.beglevel
.*
.section Command Line Options added to Watcom C8.5 /386
.*
.begnote $break $compact
.note d1+
to generate line number information plus typing information for global
symbols and local structs and arrays
.note ei
force all enumerated types to be of type 'int'
.note en
emit routine name before prologue
.note ez
generate PharLap EZ-OMF object file
.note fpi287
for programs that run on a system with a 287
.note of
to generate traceable stack frames
.note of+
to generate traceable stack frames for all functions
.note om
to generate in-line math functions
.note p
to generate preprocessor output
.note pl
to generate preprocessor output with #line directives
.note pc
to generate preprocessor output preserving comments
.note plc
to generate preprocessor output with #line directives and preserving
comments
.note r
save/restore segment registers across calls
.note we
treat warnings as errors
.endnote
.*
.endlevel
.*
.section Major Differences from Watcom C8.0 /386
.*
.np
Several major items were added to Watcom C8.5 /386. They are:
.begbull $compact
.bull
Royalty-free 32-bit DOS extender
.bull
Windows 3.0 support
.bull
Windows SDK components included
.bull
Improved optimizations
.bull
OS/2 hosted version of the compiler
.bull
The compiler now generates Microsoft format object files by default.
Use the '/ez' option to generate Phar Lap EZ-OMF object files.
.bull
More library functions to create higher compatibility with
Microsoft C 5.1 and Microsoft C 6.0.
.bull
Preprocessor output from the compiler
.bull
Standalone help utility
.bull
Object module convert utility can convert debugging information into
CodeView format
.bull
Protected-mode version of the linker
.bull
Debugger support for ADS applications
.bull
Support for Pharlap 3.0 and Pharlap 4.0
.bull
Support for Tenberry Software DOS/4G
.bull
Support for Intel 386/486 Code Builder Kit
.bull
Support for UNICODE.
Literal strings and character constants that are preceded by "L" map
code page 437 onto UNICODE unless one of the options "-zk0, -zk1, or -zk2"
is specified.
.endbull
.*
.beglevel
.*
.section Command Line Options added to Watcom C8.0 /386
.*
.* .begnote $break
.begnote $compact
.note d2
symbolic debugging information
.note oc
disable "call" followed by "ret" being changed into a "jmp"
optimization
.note u<name>
undefine a pre-defined name
.note zc
places literal strings in the CODE segment
.note zk{0,1,2}
double-byte character support
.endnote
.*
.endlevel
.*
.section Major Differences from Watcom C7.0 /386
.*
Several major items have been added to Watcom C8.0 /386. They are:
.begbull
.bull
Linker
.bull
Librarian
.bull
Graphics library
.bull
More library functions to create higher compatibility with
Microsoft C 5.1 and Microsoft C 6.0.
.endbull
.np
The professional edition also gives you the following most asked for
features:
.begbull
.bull
Protected-mode version of the compiler
.bull
Full-screen source-level debugger
.bull
Execution profiler
.endbull
.*
.beglevel
.*
.section Protected-mode Compiler and Linker
.*
.np
The protected-mode version of the compiler "WCC386P.EXE" and linker
"WLINKP.EXE" use a DPMI compliant DOS extender.
This allows you to run the compiler and linker on a normal
DOS system or in a Windows 3.x DOS box operating in enhanced mode.
.*
.endlevel
.*
.do end
.*
.if '&lang' eq 'FORTRAN 77' .do begin
.*
.if &version ge 110 .do begin
.*
.section Changes in 11.0 that may Require Recompilation
.*
.np
Do not attempt to mix object code generated by earlier versions of the
compilers with object code generated by this release or with the
libraries provided in this release.
.begnote
.note clock()
The
.kw clock
function accuracy has changed from 100 ticks per second to 1000 ticks
per second (i.e.,
.kw CLOCKS_PER_SEC
has changed).  Source code that
uses the
.kw clock
function and
.kw CLOCKS_PER_SEC
in its calculations must be recompiled before linking the application
with new libraries.
.endnote
.*
.section Major Differences from Version 10.6
.*
.np
The following sections summarize the major differences from the
previous release of Watcom &lang..
.begbull
.bull
Some of the Win32 and 32-bit OS/2 versions of our tools are now
available in DLL form.
.millust begin
EXE      DLL        Description
------   -------    -----------------------
wlink    wlink      &lnkname
wlib     wlibd      &libname
.millust end
.np
This provides better performance when using the &ide or &makname..
See the description of the
.mono !loaddll
preprocessing directive in &makname for more information.
.endbull
.*
.beglevel
.*
.section Changes to the Code Generator for 11.0
.*
.begbull
.bull
A new optimization, "branch prediction", has been added.
This optimization is enabled by the "OBP" or "OX" compiler options.
The code generator tries to increase the density of cache use by
predicting branches based upon heuristics (this optimization
is especially important for Intel's Pentium Pro).
.bull
We have added Multi-media Extensions (MMX) support to the in-line
assembler.
.endbull
.*
.section Changes to the Compiler Tools for 11.0
.*
.begbull
.bull
The &lnkname supports incremental linking.
.bull
The &lnkname can now process COFF and ELF format object files, as
well as OMF et al.
The &lnkname can now read both AR-format (Microsoft compatible)
libraries and old-style OMF libraries.
.bull
Support for creating 16-bit DOS overlaid executables has been removed
from the linker.
.bull
The &libname (&libcmdup) can now process COFF and ELF format object
files, as well as OMF et al.
The &libname can now read/write both AR-format (Microsoft compatible)
libraries and old-style OMF libraries.
The default output format is AR-format and this can be changed by
switches.
The &libname can output various format import libraries.
.bull
We have added Multi-media Extensions (MMX) support to the &asmname
(&asmcmdup).
.bull
A new version of the &disname (&discmdup) is included.
It can process ELF, COFF or OMF object files and ELF, COFF or PE
format (Win32) executables.
.np
The old disassembler (WDISASM) has been retired and is not included in
the package.
.bull
We have added new tool front-ends that emulate Microsoft tools.
These are:
.begbull $compact
.bull
rc
.endbull
.np
These programs take the usual Microsoft arguments and translate them,
where possible, into equivalent &company arguments and spawn the
equivalent &company tools.
.bull
&makname now processes Microsoft format makefiles when the "ms" option
is used.
.endbull
.*
.section Changes to the C/C++ Libraries for 11.0
.*
.begbull
.bull
We have added multi-byte and wide character (including UNICODE)
support to the libraries.
.bull
We have added Multi-media Extensions (MMX) support to the libraries.
.bull
The
.kw clock
function accuracy has changed from 100 ticks per second to 1000 ticks
per second (i.e.,
.kw CLOCKS_PER_SEC
has changed).
.endbull
.*
.section Changes to the DOS Graphics Library for 11.0
.*
.begbull
.bull
The graphics library now performs the VESA test
.us before
testing for vendor specific graphics cards.
This fix is intended to broaden the number of graphics cards that are
supported.
.endbull
.*
.section Changes in Microsoft Win32 SDK Support for 11.0
.*
.begbull
.bull
The Win32 SDK is included for Windows 95 and Windows NT
platforms.
.endbull
.*
.endlevel
.*
.do end
.*
.if &version ge 106 .do begin
.*
.section Changes in 10.6 that may Require Recompilation
.*
.begnote
.note _diskfree_t
The struct members of the _diskfree_t structure has been changed from
UNSIGNED SHORTs to UNSIGNED INTs. This is to deal with possible HPFS
partitions whose size will overflow a short, as well as Microsoft
compatibility.
.note clock()
The
.kw clock
function accuracy has changed from 100 ticks per second to 1000 ticks
per second (i.e.,
.kw CLOCKS_PER_SEC
has changed).  Source code that
uses the
.kw clock
function and
.kw CLOCKS_PER_SEC
in its calculations must be recompiled before linking the application
with new libraries.
.endnote
.*
.section Major Differences from Version 10.5
.*
.np
The following sections summarize the major differences from the
previous release of Watcom &lang..
.*
.beglevel
.*
.section Windows 95 Help File Format
.*
.np
We have included Windows 95 format help files.
.*
.section Changes to the C Library
.*
.np
The following new functions were added to the library.
.begnote
.note _getw
read int from stream file
.note _putw
write int to stream file
.endnote
.np
The
.kw clock
function accuracy has changed from 100 ticks per second to 1000 ticks
per second (i.e.,
.kw CLOCKS_PER_SEC
has changed).
.*
.section Changes to the Image Editor
.*
.begbull
.bull
Support has been added for > 16 colours for bitmaps.
.bull
Support has been added for 16 X 16 icons.
.bull
Support has been added for 48 X 48 icons.
.endbull
.*
.section Changes to the Dialog Editor
.*
.begbull
.bull
Support has been added for Windows 95 controls.
.bull
Support has been added for adding new control styles to existing
controls.
.bull
Support has been added for new dialog styles.
.bull
Support has been added for allowing help IDs to be specified in dialog
and control statements.
.bull
Support has been added for generating new resource statements in .RC
files.
.endbull
.*
.section Changes to the Resource Editor
.*
.begbull
.bull
Support has been added for new Windows 95 resource types.
.bull
Support has been added for generating new resource statements in .RC
files.
.endbull
.*
.section Changes to the Resource Compiler
.*
.begbull
.bull
Support has been added for extended styles for dialogs.
.bull
Support has been added for the RCINCLUDE keyword.
.endbull
.*
.endlevel
.*
.do end
.*
.section Major Differences from Version 9.5
.*
.begbull
.bull
The functionality of &watf16 and &watf32 is included in a single
package.
.bull
OS/2 3.0 Warp is supported.
.bull
Windows NT 3.5 is supported.
.bull
Windows 95 (Chicago) is supported.
.bull
An Integrated Development Environment for Windows 3.x, Windows 95,
Windows NT and OS/2 PM is included.
.bull
New, redesigned debugger with GUI interfaces for Windows 3.x, Windows
95, Windows NT and OS/2 PM is included.
.bull
TCP/IP remote debug servers for OS/2 and Windows NT are included.
.bull
The optimizer has been enhanced.
.bull
New, redesigned user interface for the Profiler.
.bull
Windows resource editing tools are included:
.begpoint $compact
.point Dialog Editor
.point Bitmap Editor
.point Resource Editor
.point Menu Editor
.point String Editor
.point Accelerator Editor
.endpoint
.bull
Windows development tools are included:
.begpoint $compact
.point Dr. Watcom (a post mortem debug utility)
.point Spy (Windows message spy program)
.point DDESpy
.point Heap Walker
.point Zoom
.endpoint
.bull
On-line documentation is included.
.bull
Creation of FlashTek DOS extender applications is supported.
.bull
Compiler executables have been created that run under all supported
operating systems.
They are located in the BINW directory.
.endbull
.*
.beglevel
.*
.section Items No Longer Supported
.*
.begbull
.bull
Debugging of Ergo OS/386 DOS extender applications
.bull
DESQView remote debugging
.endbull
.*
.section Changes that may Require Recompilation
.*
.np
FORTRAN applications should not require recompilation, but
.us you should recompile your application if you want
.us to take full advantage new features in the debugger.
.*
.endlevel
.*
.section Major Differences from Version 9.0
.*
.np
The following lists some of the new features/differences between version
9.5 and 9.0.
.autopoint
.point
The "/resource" option has been added and allows run-time messages
to be placed in the executable file as resource information instead of
being linked with the application.
The resource file containing these messages is called "wf77.res" and is
locating in the root of the directory in which you installed the software.
The strip utility (WSTRIP) can be used to place the resource information in
the executable file.
Special libraries for processing resource information will be required when
you link your application.
These libraries are called "wresf.lib" (for 32-bit register calling
convention), "wresfs.lib" (for 32-bit stack calling convention),
"wresm.lib" (for 16-bit medium memory model),
and "wresl.lib" (for 16-bit large and huge memory models)
and are located in the "lib386" and "lib286" directories of the
directory in which you installed the software.
.point
The "/5" option has been added and causes the compiler to generate
code that is optimized for the Pentium.  This is the default.
The "/fp5" option has been added and causes the compiler to generate
floating-point code that is optimized for the Pentium.  This is not
the default.  The default floating-point option is "/fp3".
.point
The "/3" option has been added and causes the compiler to generate
code optimized for a 80386 processor.  The "/4" option has been added
and causes the compiler to generate code optimized for a 80486 processor.
The "/5" option has been added and causes the compiler to generate
code that is optimized for the Pentium.  In addition, the "/fp5" option
has been added and causes the compiler to generate floating-point code
that is optimized for the Pentium.
.point
The "/descriptor" option has been added which specifies that string
descriptors are to be passed for character arguments.  You can specify
the "/nodescriptor" option if you do not want string descriptors to be
passed for character arguments.  Instead, the pointer to the actual
character data and the length will be passed as two arguments.  The
arguments for the length will be passed as additional arguments following
the normal argument list.  For character functions, the pointer to the
data and the length of the character function will be passed as the first
two arguments.
.point
The "oi" option has been added.  This option causes code for statement
functions to be generated in-line.
.point
The floating-point model used when generating in-line 80x87 instructions
has changed.  If you wish to call subprograms compiled with version 9.0
you must specify the "/fpr" option.
.point
Support for records in text files separated by only a line feed character
has been added.  Previously, records in text files had to be separated
by a carriage return/line feed sequence.
.point
The "/debug" option has been divided into two separate options.  The
"/bounds" option generates array and character subscript checking code.
The "/trace" option generates code that allows a run-time traceback
to be generated when a error is issued.
.point
The "/quiet" option has been added to suppress banner/summary
information.  The "terminal" option only affects the display of diagnostic
messages (extensions, warnings and errors).  For example, compiling with
the "/quiet" option will only display diagnostic messages (if any).
.point
The "/cc" option has been added.  It specifies that unit 6 is a carriage
control device and that output to this unit will be assumed to contain
carriage control characters.  Note that a blank carriage controle
character will automatically be generated for list-directed output.
.point
The "/automatic" option causes all local variables (including arrays) to
be allocated on the stack.  Care should be taken when using this option
since the stack requirements of your application may increase
dramatically.
.point
The "/wild" option has been added. It suppresses the compile-time
checking that normally causes an error to be issued when an attempt is
made to transfer control into a block structure from outside the block
structure and vice versa.  This option is only to be used when compiling
existing code that contains GOTO statements of this type; it does not
encourage this style of programming.
.point
National language support has been added.  The "/japanese" option causes
the compiler to process Japanese double-byte characters in variable names
and character data.  Similarly, the "/chinese" option will process
Traditional Chinese characters and the "/korean" will process Korean
characters.
.point
The "/ob" option has been added.  This allows the code generator to use
register ESP as a base register to reference local variables and
subprogram arguments.  Note that when this option is specified, the
compiler will abort when there is not enough memory to optimize the
subprogram.  By default, the code generator uses more memory-efficient
algorithms when a low-on-memory condition is detected.
.point
The GROWHANDLES() function has be added.  This function increases the
number of files that can be opened by a program to the specified
amount.  The value returned is the new maximum.  This may be less than
the requested amount because of operating system limitations.
.point
The IARGC() and IGETARG() functions have been added.  IARGC is an integer
function that returns the argument count.  IGETARG is an integer function
that takes two arguments.  The first argument is of type INTEGER and is
the index of the argument to be returned.  The second argument is of type
CHARACTER and is used to return the argument.  The value returned by the
function is the length of the argument.
.point
The SHARE= specifier has been added to the OPEN statement.  It allows
the program to specify how other processes can access a file that is
opened by the original process.  Possible values for the SHARE= specifier
are as follows.
.illust begin
DENYRW          - deny read and write access
DENYWR          - deny write access
DENYRD          - deny read access
DENYNO          - allow read and write access
COMPAT          - compatibility mode
.illust end
.endpoint
.*
.do end
