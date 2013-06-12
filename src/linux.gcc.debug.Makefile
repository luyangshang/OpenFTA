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
##	Linux gcc Makefile
##
## SccsId:
##	%W%,	%E%, %U%
##
## Origin:
##	FSC FTA Product
##
## Author:
##	Formal Software Construction Ltd.
##
################################################################################
##
## Variables and targets required for Linux GCC
##
################################################################################


# Compilation command and flags
CC = gcc
CFLAGS = -g -fPIC -c -DDEBUG

# Link command and flags
LINKCC = gcc
LFLAGS = -shared -fPIC -v

# Library build command and flags
AR = ar
ARFLAGS = -rv

# Build directory
BUILDIR = linux.debug

# Java details
JAVAHOME = /usr/local/j2sdk1.4.2_04
JAVAC = $(JAVAHOME)/bin/javac
JAVAH = $(JAVAHOME)/bin/javah
JAR = $(JAVAHOME)/bin/jar

JINCLUDES = \
-I$(JAVAHOME)/include \
-I$(JAVAHOME)/include/linux \

JLIBRARIES = \
-L$(JAVAHOME)/jre/lib/i386 -ljawt -lnsl -ldl -lfl
