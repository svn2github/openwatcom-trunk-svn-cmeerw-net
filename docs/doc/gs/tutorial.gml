.chap *refid=cptutor Hands-on Introduction to &product
.*
.np
Let's get started and introduce some of the tools that are in
&product.. The purpose of this chapter is to briefly test out the
new graphical tools in &product using an existing application.
.np
In this tutorial, we will take an existing set of
.if '&lang' eq 'C/C++' .do begin
C++
.do end
.if '&lang' eq 'FORTRAN 77' .do begin
FORTRAN 77
.do end
source files, create a project in our integrated development
environment, and perform the following tasks:
.begbull $compact
.bull
Add multiple targets
.bull
Make a target
.bull
Make all targets
.bull
Execute the program
.bull
Debug the program
.if '&lang' eq 'C/C++' .do begin
.bull
Use the &br
.do end
.bull
Correct errors
.bull
Sample and profile the executable
.bull
Save the project
.bull
Terminate the session
.endbull
.*
.section Outline
.*
.np
&company's &ide (&vip) manages the files and tools that a programmer
uses when developing a project. This includes all the source files,
include files, libraries, compiler(s), linkers, preprocessors, etc.
that one uses.
.np
The &vip has a graphical interface that makes it easy to visualize the
make-up of a project. A single &vip session shows a project. If the
project consists of a number of components, such as two executables
and one library, these are each shown as target windows in the
project window. Each target window shows the files that are needed to
construct the target and is associated via its filename extension with
a rule that describes the construction mechanism. For example, a
filename with the extension ".EXE" may be associated with the rule for
constructing 32-bit Windows executables, or a filename with the
extension ".LIB" may be associated with the rule for constructing static
libraries. Different projects can refer to the same target. If they
do, the target is shared and can be manipulated via either project,
with changes made through one affecting the other.
.np
The &vip itself is a collection of programs that manages the various
files and tools used to create the target libraries and executables.
It creates makefile(s) from the information in the target descriptions
and invokes &makname to construct the targets themselves. A configuration
file contains built-in knowledge of the &company compilers, editors,
Profiler, and &br, as well as all their switches.
:CMT. The open architecture of the configuration file makes it easy to add
:CMT. new tools and file types.
.*
.section The &product Tutorial
.*
.np
This tutorial walks you through the creation and execution of a
.if '&lang' eq 'C/C++' .do begin
C/C++
.do end
.if '&lang' eq 'FORTRAN 77' .do begin
FORTRAN 77
.do end
program under Windows. This will give you an understanding of the
basic concepts of the &vip and its components, and it will detail
the steps involved in project development. The result of this tutorial
is a three dimensional drawing of a kitchen which you can manipulate
using either the menus or the icons on the toolbar. You can rotate and
resize the drawing, as well as adjust the lighting and contrast.
.np
To begin, start the &vip..
This is done by double-clicking on the "&vip" icon in the &product
window.
.np
A status field at the bottom of the &vip window indicates the function
of the icon on the toolbar over which your mouse cursor is currently
positioned.
If the status area does not show you the function of the icons as you
move the mouse cursor over them, check that no item in the menu bar is
highlighted (if one is highlighted, press the Alt key).
.figure *depth='2.82' *scale=100 *file='ide1c' The initial &vip screen
.*
.beglevel
.*
.section Defining a Project
.*
.if '&lang' eq 'C/C++' .do begin
.sr smpdir='\SAMPLES\IDE'
.do end
.if '&lang' eq 'FORTRAN 77' .do begin
.sr smpdir='\SAMPLES\IDE\FORTRAN'
.do end
.*
.np
In this tutorial, you will be creating a new project called
.fi KITCHEN.
Here are the steps required to accomplish this task.
.autonote
.note
Define a new project by pulling down the
.us File
menu and selecting the
.us New Project...
item.
You can also define a new project by clicking on the "Create a new project"
icon on the toolbar.
.note
A choice of different sample project directories is available.
Assuming that you installed the &product software in the
.fi &pathnamup
directory, you will find the sample project directories in the
following directory:
.millust begin
&pathnamup.&smpdir.
.millust end
.pc
For purposes of this tutorial, we recommend that you select one
of the following project directories:
.begnote
.note WIN
for an example of 16-bit Windows 3.x application development
when using Windows 3.x under DOS as a host development system,
.note WIN386
for an example of 32-bit Windows 3.x application development
when using Windows 3.x under DOS as a host development system,
.note WIN32
for an example of 32-bit Win32 application development
when using Windows NT/2000/XP or Windows 95/98/Me as a host development system, and
.note OS2
for an example of 32-bit OS/2 application development
when using 32-bit OS/2 as a host development system.
.endnote
.np
Thus the
.mono target
that we refer to below should be one of
.mono WIN,
.mono WIN386,
.mono WIN32,
or
.mono OS2
depending on your selection.
The tutorial uses the
.mono WIN32
example for illustrative purposes.
You will find some minor variations from your selected target
environment.
.np
When asked for a project name, you can do one of two things:
.autonote
.note
enter the following pathname:
.millust begin
d:[path]&smpdir.\target\KITCHEN
.millust end
.pc
where
.mono d:[path]
is the drive and path where you installed the &company software, or
.note
use the file browser to select the following directory:
.millust begin
d:[path]&smpdir.\target
.millust end
.pc
and specify the filename
.fi kitchen.
.endnote
.np
.if '&lang' eq 'C/C++' .do begin
.figure *depth='2.50' *scale=100 *file='ide2c' Creating a new project
.do end
.if '&lang' eq 'FORTRAN 77' .do begin
.figure *depth='2.50' *scale=100 *file='ide2f' Creating a new project
.do end
.np
Press the Enter key or click on OK (OPEN).
.np
The project description will be stored in this file and the &vip will
set the current working directory to the specified path during your
session.
.note
You will be prompted for a target name. Since we will be attaching
pre-defined targets, just click the
.us Browse
button when prompted for the target name.
Select the "draw" target file (it will be one of
.fi draw16.tgt,
.fi draw.tgt,
.fi draw32.tgt,
.fi drawos2.tgt
depending on your selection of target).
.if '&lang' eq 'C/C++' .do begin
.figure *depth='2.50' *scale=100 *file='ide3c' Attaching existing targets
.do end
.if '&lang' eq 'FORTRAN 77' .do begin
.figure *depth='2.50' *scale=100 *file='ide3f' Attaching existing targets
.do end
.np
Press the Enter key or click on OK (OPEN).
.note
.ix 'Target Environment'
.ix 'Image Type'
You can ignore the settings displayed for
.us Target Environment
and
.us Image Type
since the target definition already exists (we created it for you).
The settings are important when you a defining a new target (i.e., one
that was not predefined).
.figure *depth='2.96' *scale=100 *file='ide4c' Selecting a target type
.np
Press the Enter key or click on OK.
.endnote
.np
A target window is created in the project window for the "draw"
target.
This window contains all of the files associated with the target.
You can click on any of the "Folder" icons to hide or un-hide all
files with a particular extension. For example, you may wish to un-hide
all the files with a
.fi .bmp
extension by clicking on the folder icon associated with bitmap files.
.*
.section Adding Multiple Targets
.*
.np
&company's &vip allows you to have multiple targets in any particular
project. Note that targets can be used by multiple projects. To add a
new target to the project, do the following.
.autonote
.note
Pull down the
.us Targets
menu and select the
.us New Target...
item.
.note
Enter
.fi button.tgt
as the target name for the new target to be added to the project.
Do not forget to include the
.fi .tgt
extension.
It is required when selecting a pre-existing target.
.note
.ix 'Target Environment'
.ix 'Image Type'
You can ignore the settings displayed for
.us Target Environment
and
.us Image Type
since the target definition already exists (we created it for you).
The settings are important when you a defining a new target (i.e., one
that was not predefined).
.note
Press the Enter key or click on OK.
.endnote
.np
A target window is created in the project window for the
.id button
target.
.*
.section Making a Target
.*
.np
&company's &vip will automatically generate the sequence of steps
required to build or "make" each of the targets in a project.
Note that the targets in a project can be made individually or
collectively.
To make the
.id button.lib
target, do the following.
.autonote
.note
Click on the window of the target you wish to make. In this case,
click on the
.id button.lib
target window.
.note
Pull down the
.us Targets
menu and select the
.us Make
item (you can also do this by clicking on the "Make the current
target" icon on the toolbar, or by right-clicking on the target and
selecting the
.us Make
item from the pop-up menu).
.endnote
.np
The &vip will now construct a makefile based on its knowledge of the
target and construction rules, and then invoke the make utility to
create the target, in this case
.id button.lib.
The output of this procedure is displayed in the Log window.
.*
.section Making All Targets
.*
.np
Click the "Make all targets in the project" icon on the toolbar to
make all of the targets in the current project. If one target depends
on another target, the latter target will be made first. In this
tutorial
.fi button.lib
will be made first (there will be nothing to do since it was made
previously) and then
.fi draw???.exe,
since
.fi button.lib
is in the list of files associated with
.fi draw???.exe.
In each case, the &vip constructs makefiles based on its knowledge of
the target and construction rules. The output of this procedure is
displayed in the Log window.
.if '&lang' eq 'C/C++' .do begin
.figure *depth='3.97' *scale=100 *file='ide5c' Making one or more targets
.do end
.if '&lang' eq 'FORTRAN 77' .do begin
.figure *depth='4.68' *scale=100 *file='ide5f' Making one or more targets
.do end
.*
.section Executing the Program
.*
.np
The project should have built without errors, so now you are ready to
execute the program you have developed.
.np
Click on the
.fi draw???.exe
target window and simply click on the "Run the current target" icon on
the toolbar. You can also do this by selecting
.us Run
from the
.us Targets
menu, or right-clicking anywhere on the target window outside of the
inner frame and selecting
.us Run
from the pop-up menu (right-clicking is context sensitive and the
pop-up menu that results will vary depending on the area of the window
in which you right-click).
.figure *depth='3.76' *scale=100 *file='ide6c' The kitchen demo
.np
The demo you have created is a simple three dimensional drawing of a
kitchen. By using either the icons on the toolbar or the menus you can
rotate the picture left, right, up, and down, make the picture
brighter or dimmer, move the picture closer or farther away, and
increase or decrease the amount of contrast (this latter feature is
found in the "Lighting" menu). Choose
.us Exit
from the
.us File
menu to exit the demo program when you are finished.
.*
.section Smart Editing
.*
.np
The &vip recognizes the type of file you wish to edit, and invokes the
appropriate editor for the task. To edit a file, you either
double-click on it or select it and click the "Edit" icon on the
toolbar. Files with a
.fi .c,
.fi .cpp,
.fi .h,
.fi .hpp,
.fi .for,
.fi .asm
or
.fi .rc
extension are edited with a text editor; files with a
.fi .bmp,
.fi .ico,
or
.fi .cur
extension are edited with the Image Editor; files with a
.fi .dlg
or
.fi .res
extension are edited with the Resource Editor.
:cmt. You can customize the edit actions for any particular extension to use
:cmt. your favorite editor (including third-party editors).
.if '&lang' eq 'C/C++' .do begin
.figure *depth='2.65' *scale=100 *file='ide7c' The &edname for Windows
.do end
.if '&lang' eq 'FORTRAN 77' .do begin
.figure *depth='3.47' *scale=100 *file='ide7f' The &edname for Windows
.do end
.np
Now we will edit one of the source files and introduce an error into
the application.
.if '&lang' eq 'C/C++' .do begin
.autonote
.note
Double-click on the "draw" source file (i.e.,
.fi draw???&cxt
.ct ) to load the source file into the editor.
.note
Scroll down to line 227 using the keyboard or mouse. You can also pull
down the
.us Edit
menu, select
.us Goto Line...,
and enter 227.
The &edname makes full use of colors and fonts to achieve syntax
highlighting. File templates for C, C++, and FORTRAN files are defined
to assist you in distinguishing the components of your code.
.note
Pull down the
.us Options
menu and select the
.us Colors
item.
.note
Click on a color from the palette, drag it to the word
.id if
on line 218, and release it. All keywords are now displayed in the
chosen color. Drag a different color to a comment line (line 225) and
all comments will display in that color. Similarly, you can select the
Fonts item from the Options menu, select a font style and size, and
drag it to your source file. Close the Fonts and Colors dialog by
double-clicking in the upper left hand corner.
.note
You can now save this color and font configuration for all
.fi &xxt.
files by pulling down the
.us Options
menu, selecting the
.us General...,
item and clicking next to
.us Save configuration on exit
in the "Features" box. Press Enter or click on OK.
.note
Now, to introduce an error into the application, replace the line
.id #if 0
with
.id #if 1.
.note
Save your changes by clicking on the "Write the current file to disk"
icon or select
.us Save
from the
.us File
menu.
.note
Return to the &vip (by clicking on it if it is visible on your screen,
or by using Alt-Tab), re-make your project, and run it. A fault occurs
in your application, so the next step is to track down the problem
using the &dbgname..
.endnote
.do end
.if '&lang' eq 'FORTRAN 77' .do begin
.autonote
.note
Double-click on the "draw" source file (i.e.,
.fi draw???&cxt
.ct ) to load the source file into the editor.
.note
Search for the line containing the text:
.millust begin
c$ifdef undefined
.millust end
.np
For Windows users, "Find" is in the "Edit" menu.
If you are building a WIN386 application, search for the second line
containing this text (located in subroutine
.mono memclear
.ct ).
.note
Now, to introduce an error into the application, add the line
.millust begin
c$define undefined
.millust end
just before the line containing
.mono c$ifdef.
.note
Save your changes by clicking on the "Write the current file to disk"
icon or select
.us Save
from the
.us File
menu.
.note
Return to the &vip (by clicking on it if it is visible on your screen,
or by using Alt-Tab), re-make your project, and run it. A fault occurs
in your application, so the next step is to track down the problem
using the &dbgname..
.endnote
.do end
.*
.section Debugging the Program
.*
.np
To debug a program it must first be compiled to include debugging
information. The &vip maintains two sets of switches for each target in
a project. These are known as the Development Switches and the Release
Switches.
.autonote
.note
Right click on
.fi draw???&cxt.
and select
.us Source options
from the pop-up menu.
Select
.if '&lang' eq 'C/C++' .do begin
.us C Compiler Switches
.do end
.if '&lang' eq 'FORTRAN 77' .do begin
.us Fortran Compiler Switches
.do end
from the sub-menu.
.np
By default, your target is placed in development mode with the
debugging switches for the compiler and linker set to include
debugging information. You can either set the switches in each
category manually or you can copy the default Release switches using
the CopyRel button. This method of setting switches is especially
convenient since you can specify everything from diagnostic,
optimization, and code generation switches to special linker switches
all without having to memorize a cryptic switch name &mdash. you
simply click next to the switches you wish to use for a particular
item.
.note
Scroll through the categories using the >> button until you get to:
.millust begin
.if '&lang' eq 'C/C++' .do begin
6. Debugging Switches
.do end
.if '&lang' eq 'FORTRAN 77' .do begin
4. Debugging Switches
.do end
.millust end
.pc
We can see that full debugging information was used in the compile, so
just click on
.us Cancel
to exit this screen.
.if '&lang' eq 'C/C++' .do begin
.figure *depth='2.39' *scale=100 *file='ide8c' Setting compiler switches
.do end
.if '&lang' eq 'FORTRAN 77' .do begin
.figure *depth='2.21' *scale=100 *file='ide8f' Setting compiler switches
.do end
.note
To invoke the debugger, pull down the
.us Targets
menu and choose the
.us Debug
item or select the "Debug the current target" icon from the toolbar.
.endnote
.np
The &dbgname is designed to be as convenient and intuitive as
possible, while at the same time providing a comprehensive and
flexible environment for serious debugging. You can configure your
environment to display exactly the information you require to be most
productive. Among the windows available are source and assembly,
modules, functions, calls, threads, images, watches, locals, globals,
file variables, registers, 80x87 FPU, stack, I/O ports, memory display,
and a log window. You can step through your source
using the keys or icons on the toolbar. Execute one line at a time by
stepping over calls or stepping into calls, or execute until the
current function returns. Right-mouse button functionality gives
context-sensitive pop-up menus.
.if '&lang' eq 'C/C++' .do begin
.figure *depth='4.29' *scale=100 *file='ide9c' The &dbgname
.do end
.if '&lang' eq 'FORTRAN 77' .do begin
.figure *depth='4.78' *scale=100 *file='ide9f' The &dbgname
.do end
.np
We know that a fault has occurred in
.fi draw???.exe,
so we will run the application and examine the state of the program
when the fault occurs.
.if '&lang' eq 'C/C++' .do begin
.autonote
.note
Click on the "go!" icon on the toolbar to begin execution of the
program. The exception occurs and the source window shows the line
.millust begin
*pwidth = bitmap.bmWidth + 5;
.millust end
in the function
.id button_size
as the last line executed before the exception. Examining the Locals
window you will see that
.id pwidth
is a NULL pointer, hence the exception.
.note
We can now move up the call stack by clicking on the "Move up the call stack"
icon on the toolbar (red up arrow) to follow the program's
execution. On the previous line, we see
.id button_size
is called from
.id add_button.
Moving up the call stack again, we see
.id add_button
is called with NULL as its fifth parameter. An artificial error has been
introduced for the purposes of this tutorial. It is located several lines
back in the source file.
.note
By replacing the line
.id #if 1
with
.id #if 0
we can bypass this error.
Right-click on the line
.id #if 1
and select Show, then Line... from the pop-up menus to see the line
number which must be corrected, then exit the debugger.
.note
Double-click on
.fi draw???&cxt.
to load the source file into the editor.
.note
Scroll down to line 227 using the keyboard or mouse, or pull down the
.us Edit
menu, select
.us Goto Line...,
and enter 227.
.note
Replace the line
.id #if 1
with
.id #if 0
and save your changes by clicking on the "Write the current file to
disk" icon or selecting
.us Save
from the
.us File
menu.
.note
Return to the &vip (by clicking on it if it is visible on your screen,
or by using Alt-Tab) and re-make your project.
.note
Run your project to see the kitchen demo.
.endnote
.do end
.if '&lang' eq 'FORTRAN 77' .do begin
.autonote
.note
Click on the "go!" icon on the toolbar to begin execution of the
program. The exception occurs and the source window shows the line
.millust begin
arr( cnt ) = ch
.millust end
in the subroutine
.id memclear
as the last line executed before the exception. Examining the Locals
window you will see that the array location or address,
.id locale,
is 0.
This means that the array
.id arr
was mapped to location 0.
For the 32-bit Windows example, we also needed to use the "array"
pragma to force a far pointer reference to cause the exception.
.note
We can now move up the call stack by clicking on the "Move up the call stack"
icon on the toolbar (red up arrow) to follow the program's
execution.
On the previous executable line of code, we see
.id memclear
is called from the main program with three arguments.
.millust begin
call memclear( 0, loc(Draw_area), isizeof(RECTL) )
.millust end
.np
We see that
.id memclear
is called with 0 as the first argument and
.id loc(Draw_area)
as the second argument which is backwards to the way it should be.
An artificial error was introduced for the purposes of this tutorial.
.note
By removing the line
.millust begin
c$define undefined
.millust end
.pc
we can undo this error.
Exit the debugger.
.note
Double-click on
.fi draw???&cxt.
to load the source file into the editor.
.note
Go to the line in question using "find" or "search" and delete the
line.
Save your changes by clicking on the "Write the current file to disk"
icon or selecting
.us Save
from the
.us File
menu.
.note
Return to the &vip (by clicking on it if it is visible on your screen,
or by using Alt-Tab) and re-make your project.
.note
Run your project to see the kitchen demo.
.endnote
.do end
.*
.if '&lang' eq 'C/C++' .do begin
.*
.section Using the Source Browser
.*
.np
Suppose you wanted to change the color of the tabletop in your
application. You can use the &brname to determine the code you will
need to change. The &brname provides an easy way to examine the class
definitions, member functions, and inheritance trees
associated with your C++ code.
First, you need to instruct the compiler to emit &brname information.
.figure *depth='3.50' *scale=100 *file='ide10c' The &brname
.autonote
.note
Right click on
.fi furnitu&xxt.,
then select
.us Source options
from the pop-up menus.
Select
.if '&lang' eq 'C/C++' .do begin
.us C++ Compiler Switches
.do end
.if '&lang' eq 'FORTRAN 77' .do begin
.us FORTRAN 77 Compiler Switches
.do end
from the sub-menu.
.cp 10
.note
Go to the
.millust begin
6. Debugging Switches
.millust end
.pc
category by selecting it from the drop-down list box or by scrolling
through the categories using the >> button.
.note
Select
.mono Emit Browser information [-db]
and click on OK.
.note
Click the "Make all targets in the project" icon to re-make the project.
The compiler will emit Browser information for
.fi furnitu&xxt.
in a file called
.fi draw???.dbr.
Now you are ready to browse the target's source.
.note
Pull down the
.us Targets
menu and select
.us Browse,
or click the "Browse the current target" icon on the toolbar. The
inheritance tree for the target is displayed.
.note
To view details on any particular class, double-click on the item for
information such as the location of the class definition, the private,
public, and protected functions of the class, and the class
inheritance. Branches of the inheritance tree can be collapsed and
expanded. A variety of tools are available to help you navigate your
.if '&lang' eq 'C/C++' .do begin
C++
.do end
.if '&lang' eq 'FORTRAN 77' .do begin
FORTRAN 77
.do end
source. Double-click on the
.id table
class.
.note
Double-click on the function
.id top_and_four_legs()
to see the details on this function.
.note
Select the variable
.id tabletop,
pull down the
.us Detail
menu, and select the
.us Goto Definition...
item. The Editor is invoked, loading the file
.fi furnitu&xxt.
which contains the definition of
.id top_and_four_legs.
.note
Next we will make some changes to
.fi furnitu&xxt.
in order to change the color of the tabletop. Scroll down to line 143
using the keyboard or mouse, or pull down the
.us Edit
menu, select
.us Goto Line...,
and enter 143
.note
Replace the line
.millust begin
tabletop->rgb(0,255,255);
.millust end
with
.millust begin
tabletop->black();
.millust end
.note
Save your changes by clicking on the "Write the current file to disk"
icon or selecting
.us Save
from the
.us File
menu.
.note
Shut down the &br before re-making the project.
.note
Return to the &vip (by clicking on it if it is visible on your screen,
or by using Alt-Tab).
.note
Click the "Make all targets in the project" icon to re-make the project.
.endnote
.*
.do end
.*
.section Correcting an Error
.*
.if '&lang' eq 'FORTRAN 77' .do begin
.np
We are now going to introduce an error into the source code to
illustrate another feature of the &vip..
.autonote
.note
Double-click on
.fi draw???&cxt.
to load the source file into the editor.
.note
Search for the line containing the text:
.millust begin
c$ifdef undefined
.millust end
.np
For Windows users, "Find" is in the "Edit" menu.
.note
Now, to introduce a syntax error into the application, add the line
.millust begin
hThree_d + 0
.millust end
just before the line containing
.mono c$ifdef.
.note
Save your changes by clicking on the "Write the current file to disk"
icon or select
.us Save
from the
.us File
menu.
.note
Return to the &vip (by clicking on it if it is visible on your screen,
or by using Alt-Tab).
.note
Click the "Make all targets in the project" icon to re-make the project.
.endnote
.do end
.np
An error is encountered during the make and error message(s) appear in
the log window. Additional information on the error is available by
selecting the error, pulling down the
.us Log
menu and selecting the
.us Help on Message
item.
.if '&lang' eq 'C/C++' .do begin
.autonote
.note
Double-click on the error message
.code begin
furnitu.cpp (132): Error! E029: (col 15) symbol 'black' has not been declared.
.code end
.np
The offending source file (
.ct
.fi furnitu&xxt.
.ct )
is loaded into the Editor and the cursor is positioned at the line
which caused the error. Apparently, black has not been defined as a
color.
.note
Restart the &br..
.note
Double-click on
.id color
in the Inheritance window to see the member functions of the class
.id color.
Among our choices are
.id blue(),
.id green(),
and
.id red().
.note
Press the Alt-Tab key combination to return to the Editor and replace
the line
.millust begin
tabletop->black();
.millust end
with
.millust begin
tabletop->red();
.millust end
.note
Save your changes.
.note
Return to the &vip and re-make the project.
.note
Run the program to see the changes you have made to the tabletop.
.endnote
.do end
.if '&lang' eq 'FORTRAN 77' .do begin
.autonote
.note
Double-click on the error message
.code begin
drawwin.for(83): *ERR* ST-17 column 9, undecodeable statement or
misspelled word HTHREE_D
.code end
.np
The offending source file (
.ct
.fi draw???&cxt.
.ct )
is loaded into the Editor and the cursor is positioned at the line
which caused the error.
.note
Delete the line to correct the error.
.note
Save your changes.
.note
Return to the &vip and re-make the project.
.endnote
.do end
.*
.section Editing a Bitmap
.*
.np
You can edit bitmaps, icons, or cursors associated with your project
using &company's Image Editor. Double-click on a file with a
.fi .bmp,
.fi .ico,
or
.fi .cur
extension and the file is loaded into the Image Editor. The editor has
many features to design your images, including resizing, rotation,
shifting, and a utility to take a "snapshot" of another image and
import it.
.np
.figure *depth='4.80' *scale=100 *file='ide11c' The &company Image Editor
.np
Suppose you wanted to change the color of the right-arrow icon in your
application.
.autonote
.note
If the "Folder" icon next to
.fi .bmp
is closed, click on it to restore all the files with a
.fi .bmp
extension to the file list.
.note
Scroll the window until the file
.fi right.bmp
is visible.
.note
Double-click on
.fi right.bmp
in the
.fi draw???.exe
target window.
.note
Select the "Paint Can" icon from the Tool Palette.
.note
Select a color from the Color Palette.
.note
Click on the arrow.
.note
Save your changes using the "Save" icon on the toolbar and exit the
Image Editor.
.note
Click the "Make all targets in the project" icon to rebuild the project
with the change incorporated.
.endnote
.*
.section Editing Menus
.*
.np
Next, you will add source files to the list of items that make up
.fi draw???.exe.
.autonote
.note
Pull down the
.us Sources
menu and select the
.us New Source...
item.
.begnote
.mnote Note:
You can do this either by choosing from the menu bar or by positioning
the mouse over the file list area and clicking the right mouse button.
The &vip displays a pop-up menu from which you can choose the desired
action.
.endnote
.note
Enter the filename
.fi draw.res
(or
.fi drawos2.res
for OS/2).
For OS/2, click OK when you have entered the source file name.
For all other systems, click on Add when you have entered the
source file name and then click on Close.
Now we will remove the
.fi .rc
file from the project so that our changes to the
.fi .res
file will not be overwritten. When an
.fi .rc
file is present, the
.fi .res
file is generated from the
.fi .rc
file.
.note
Right click on
.fi draw.rc
(or
.fi drawos2.rc
for OS/2), then select
.us Remove Source
from the pop-up menu.
.note
Double-click on
.fi draw.res
(or
.fi drawos2.res
.ct ).
The Resource Editor is invoked, displaying all the available resources
(in this case, icons, bitmaps, and menus).
.figure *depth='3.58' *scale=100 *file='ide12c' The &company Resource Editor
.note
Click on "Menu Resources".
.note
Double-click on "DrawMenu" in the right-hand box.
This will bring up the Menu Editor.
The Menu Editor displays the menus defined for the resource
.id DrawMenu.
You can specify pop-up menus, menu items and sub-items, text,
separators, attributes, break styles, and memory flags.
.figure *depth='5.21' *scale=100 *file='ide13c' The &company Menu Editor
.note
Click on
.id MENUITEM "&Dimmer"
in the item list window.
.note
In the "Item Text" window change the item to
.id &Darker
and then click on the "Change" button.
.note
Select
.us Update
from the
.us File
menu or click on the "Update the file with this menu" icon.
.note
Exit the Menu Editor.
.note
Now, select
.us Save
from the
.us File
menu or click on the "Save this file" icon and exit the Resource
Editor.
.note
Click the "Make all targets in the project" icon to re-make the project.
.endnote
.*
.section Sampling and Profiling an Executable
.*
.np
Together, the &smpname and the &prfname allow you to pinpoint the
areas of your code that are the most heavily used, indicating possible
candidates for performance improvements.
.autonote
.note
Click on the
.fi draw???.exe
target.
.note
Select the
.us Sample
item from the
.us Targets
menu item, or click on the "Run and sample the current target" icon
from the toolbar. The &smpname is invoked and your application begins
to execute.
.note
Try rotating and resizing the image a few times. The sampler takes a
"snapshot" of the code that is being executed at regular intervals.
:cmt. When you close your application, a dialog indicates the number of
:cmt. modules found, the rate of sampling, and the total execution time.
Exit the application.
A samples file with extension
.fi .smp
is created in the current directory.
This file is input for the profiler.
.note
You are now ready to profile the executable. Do this by selecting
.us Profile
from the
.us Targets
menu, or by clicking on the "Profile the current target" icon on the toolbar.
The profiler scans the
.fi .smp
file and reports the activity in the various modules of the
application. The percentage of time spent in the modules is indicated
as an absolute percentage (percent of total samples) and as a relative
percentage (percent of samples in the
.fi .exe
image).
.note
Double-click on the module or routine names to step down to the exact
source being executed when a sample was taken. For more details, you
can adjust the sampling rate of the Sampler to get a better picture of
your code. To do this, exit the Profiler, pull down the
.us Targets
menu, and select
.us Target options,
then
.us Sample Switches...
from the pop-up menus. Specify a sampling rate such as 2 (for 2
milliseconds), click on OK, then run the Sampler and Profiler again.
.endnote
.if '&lang' eq 'C/C++' .do begin
.figure *depth='3.36' *scale=100 *file='ide14c' The &prfname
.do end
.if '&lang' eq 'FORTRAN 77' .do begin
.figure *depth='2.86' *scale=100 *file='ide14f' The &prfname
.do end
.*
.section Saving the Project and Terminating the Session
.*
.np
You can now exit the &vip session by selecting
.us Exit
from the
.us File
menu.
If you have not already saved your project, you will be prompted to do
so. Choose "Yes" and the session ends.
.*
.endlevel
.*
.section Tutorial Review
.*
.np
In this tutorial, you created a project called
.fi kitchen.wpj,
which was composed of two targets:
.fi draw???.exe
and
.fi button.lib.
You compiled and linked it into an executable program using the WMAKE
utility, the
.if '&lang' eq 'C/C++' .do begin
&company C and C++ compilers,
.do end
.if '&lang' eq 'FORTRAN 77' .do begin
&product compiler,
.do end
and the &lnkname.. You executed it both directly and under the
control of the &dbgname... You browsed the source, and made
changes using the text and resource editors. Finally, you sampled and
profiled the application.
.np
When you saved the project, you created the following permanent files:
.begbull
.bull
.fi kitchen.wpj
&mdash. describes the screen layout and refers to the target files
called
.fi draw???.tgt
and
.fi button.tgt.
.bull
.fi draw???.tgt
&mdash. describes the target executable
.fi draw???.exe
and all switches required to link it. It also describes the
.if '&lang' eq 'C/C++' .do begin
.fi .c
and
.fi .cpp
.do end
.if '&lang' eq 'FORTRAN 77' .do begin
.fi .for
.do end
files and switches required to compile them.
.bull
.fi button.tgt
&mdash. describes the target library and all switches required to
create it. It also describes the
.fi &cxt.
file and the switches used to build the library.
.endbull
