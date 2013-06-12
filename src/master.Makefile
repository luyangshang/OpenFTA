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

################################################################################
##
## Module:
##      Master Makefile
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
##
## Variables and targets shared by all makefiles.
##
################################################################################

TOP = ..

#Include appropriate platform specific makefile 
include ${TOP}/../linux.gcc.Makefile

# Include Paths

INCLUDES = \
$(JINCLUDES) \
-I${TOP} \
-I${TOP}/Appn \
-I${TOP}/Widgets \
-I${TOP}/Analysis \
-I${TOP}/Canvas \
-I${TOP}/PrimaryEvents \
-I${TOP}/Utilities \
-I${TOP}/JNI \
-I$(TOP)/../gui/FTAGUI

# Java details
JAVAC = $(JAVAHOME)/bin/javac
JAVAH = $(JAVAHOME)/bin/javah
JAR = $(JAVAHOME)/bin/jar
JARFLAGS = -cvf
CLASSPATH = $(TOP):$(JAVAHOME)/lib/tools.jar:$(JAVAHOME)/lib/dt.jar


%.o: %.c
	${CC} ${FEATURES} $(INCLUDES) ${CFLAGS} $(BUILDDIR)$<


%.class: %.java
	${JAVAC} -classpath $(CLASSPATH) $<

%.h: %.class
	${JAVAH} -classpath $(CLASSPATH) FTAGUI.$*


XLIBS = \
-L$(XRTHOME)/lib -lxrtgear -lXpm \
-L/usr/openwin/lib -lXext \
-L/usr/dt/lib -lXm -lXt -lX11 -lgen -lm
