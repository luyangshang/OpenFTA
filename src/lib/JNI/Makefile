###############################################################################
## 
## OpenFTA - Fault Tree Analysis
## Copyright (C) 2005 FSC Limited
## 
## This program is free software; you can redistribute it and#or modify it
## under the terms of the GNU General Public License as published by the Free
## Software Foundation; either version 2 of the License, or (at your 
## option) any later version.
## 
## This program is distributed in the hope that it will be useful, but WITHOUT
## ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or 
## FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for 
## more details.
##
## You should have received a copy of the GNU General Public License along 
## with this program; if not, write to the Free Software Foundation, Inc., 
## 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
##
## To contact FSC, please send an email to support@fsc.co.uk or write to 
## FSC Ltd., Cardiff Business Technology Centre, Senghenydd Road, Cardiff,
## CF24 4AY.
##
###############################################################################

#################################################################################
## Module:
##      Formal-FTA Makefile
##
## SccsId:
##      %W%,    %E%, %U%
##
## Origin:
##      FSC FTA product
##
## Author:
##      Formal Software Construction Ltd.
##
################################################################################

# Include general purpose project Makefiles

include ../../master.Makefile


# Object files

OBJS = \
NativeCutSetsDialog.o \
NativeFTACanvas.o \
NativeFTAFrame.o \
NativeGateDialog.o \
NativeIntermediateEventDialog.o \
NativeMonteCarloDialog.o \
NativeNumericalProbabilityDialog.o \
NativePEDFrame.o \
NativeTransferTreeDialog.o \
NativeTreeTextDialog.o


# Externally visible file dependencies

include ${TOP}/Appn/Exports
include ${TOP}/Analysis/Exports
include ${TOP}/Canvas/Exports
include ${TOP}/PrimaryEvents/Exports
include ${TOP}/Utilities/Exports
include ${TOP}/Widgets/Exports


LIBRARIES = \
-L$(TOP)/PrimaryEvents/$(BUILDIR) -lprimaryevents \
-L$(TOP)/Utilities/$(BUILDIR) -lutilities \
-L$(TOP)/Canvas/$(BUILDIR) -lcanvas \
-L$(TOP)/Widgets/$(BUILDIR) -lwidgets \
-L$(TOP)/Analysis/$(BUILDIR) -lanalysis \
-L$(TOP)/Appn/$(BUILDIR) -lappn \
-L$(TOP)/PrimaryEvents/$(BUILDIR) -lprimaryevents \
-L$(TOP)/Utilities/$(BUILDIR) -lutilities \
-L$(TOP)/Canvas/$(BUILDIR) -lcanvas \
-L$(TOP)/Widgets/$(BUILDIR) -lwidgets \
-L$(TOP)/Analysis/$(BUILDIR) -lanalysis \
-L$(TOP)/Appn/$(BUILDIR) -lappn \
$(JLIBRARIES)


EXE	=   libfta.so


DEPENDENT_SRC = \
../Canvas/*.[ch] \
../Widgets/*.[ch] \
../Analysis/*.[ch] \
../PrimaryEvents/*.[ch] \
../Utilities/*.[ch]


DEPENDENT_LIBS = \
../Canvas/$(BUILDIR)/libcanvas.a \
../Widgets/$(BUILDIR)/libwidgets.a \
../Analysis/$(BUILDIR)/libanalysis.a \
../PrimaryEvents/$(BUILDIR)/libprimaryevents.a \
../Utilities/$(BUILDIR)/libutilities.a


SUBDIR	= \
../Analysis \
../Canvas \
../PrimaryEvents \
../Utilities \
../Widgets


# Define targets
# Define in order to be presented in MakeTool
#

all: $(EXE)


clean:
	-/bin/rm -r $(OBJS) $(EXE) core


veryclean:
	make clean
	-/bin/rm -r $(EXE).uu $(EXE).shar *.mlk *.fmt
	@for i in $(SUBDIR);\
        do (\
                echo Cleaning $$i ...;\
                cd $$i;\
                $(MAKE) veryclean FLAGS=$(FLAGS)\
        ); done\



spotless:
	make veryclean
	-/bin/rm -r *% *~ *.old
	@for i in $(SUBDIR);\
#        do (\
#                echo Cleansing $$i ...;\
#                cd $$i;\
#                $(MAKE) spotless FLAGS=$(FLAGS)\
#        ); done\


$(EXE): $(OBJS) $(DEPENDENT_LIBS)
	$(LINKCC) $(LFLAGS) $(OBJS) $(LIBRARIES) -o $(EXE)
#	@for i in $(SUBDIR);\
#        do (\
#                echo Making $$i ...;\
#                cd $$i;\
#                $(MAKE) FLAGS=$(FLAGS)\
#        ); done\
#        $(LINKCC) $(OBJS) $(LIBRARIES) -o $(EXE)
