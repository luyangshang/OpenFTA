OpenFTA 1.0
-----------
This directory contains the following sub directories:

bin - location of the scripts/binaries required to run OpenFTA 
src - the location of the source files
examples - example fault trees


Building OpenFTA - Linux
------------------------
The Linux build is controlled via the use of makefiles. Before the 
source code can be compiled the makefiles will have to be customised 
to the setup of the development machine. Examples of the customisable
makefiles are given in the "src" directory:

linux.gcc.Makefile - an example linux makefile using the GCC compiler, 
                     Java 1.4.2_04 with default optimisation. 
linux.gcc.debug.Makefile - as above, but has debug information included 
                           in the build and has no optimisation.

These files are referenced by the master.Makefile. Once the customisable
makefile has been edited, or a new one created, ensure that the desired 
makefile is included by the master.Makefile.

Once the makefiles have been setup, the build can be invokde by typing 
"make" while in the "src" directory. Typing the command "make bin" will 
both compile the source code and copy the necessary binaries to the 
"bin" directory ready to be executed.


Running OpenFTA - Linux
-----------------------
OpenFTA is lauched using a script in the "bin" directory.

First ensure that the "JAVAHOME" environment variable declared in the 
"OpenFTA.sh" script file points to a valid java installation.

Once the script is valid, type "OpenFTA.sh" to launch OpenFTA.


Building OpenFTA - Windows
--------------------------
The Java GUI is compiled by the use of the batch script (found in "src") 
"build_gui.bat". This will need to be editted so that the %JAVAHOME% 
environment variable points to a valid Java installation.

The native library, found in the "src->lib" directory, is compiled using 
the "NativeFTA.sln" Microsoft Developer Studio solution (this requires 
Microsoft Developer Studio .NET). The solution may need to be updated to 
reflect the location of Java Development Kit on the development machine 
(The default location is "C:\j2sdk1.4.2_04").

Another batch script exists, "build_bin.bat", that copies the required 
binaries to the "bin" directory.


Running OpenFTA - Linux
-----------------------
OpenFTA is lauched using a script in the "bin" directory.

First ensure that the "%JAVAHOME%" environment variable declared in the 
"OpenFTA.sh" script file points to a valid java installation. Then
ensure that you have permission to run the script by typing 
"chmod a+x OpenFTA.sh".

Once the script is valid, type "./OpenFTA.sh" (or double click on it) to 
launch OpenFTA.


A Note About Java
-----------------
The version of Java used to build and test OpenFTA was 1.4.2_04. OpenFTA 
should build and run with any version of Java from 1.3 upwards.

