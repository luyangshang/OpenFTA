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
##      @(#)Makefile	1.5,    97/07/02, 16:59:24
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


OBJS = \
Canvas.o \
JavaCanvas.o 


LIBNAME = $(BUILDIR)/libcanvas.a


# External dependencies
#
include ${TOP}/Appn/Exports
include ${TOP}/Utilities/Exports


# Internal dependencies
#
include ${TOP}/Canvas/Exports


# Define targets
# Define in order to be presented in MakeTool
#

all:    $(OBJS)
	if ( test ! -d $(BUILDIR) ) then \
          mkdir $(BUILDIR); \
        fi
	$(AR) $(ARFLAGS) $(LIBNAME) $(OBJS)

clean:
	-/bin/rm -r $(OBJS) $(LIBNAME) $(BUILDIR) core


veryclean:
	make clean


spotless:
	make veryclean
	-/bin/rm -r *% *~ *.old


# Define dependencies
#
Canvas.o:          $(CANVASP_H) $(CANVAS_H) $(MEMORY_H)
JavaCanvas.o:   $(JAVACANVASP_H) $(CANVASP_H)
PostScriptCanvas.o:      $(POSTSCRIPT_H) $(POSTSCRIPTP_H) \
  $(CANVASP_H) $(UTIL_H) $(FILE_UTILS_H) $(MEMORY_H) $(ASSERT_H) $(PRINTERS_H)

