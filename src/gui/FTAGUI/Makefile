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


CLASSES = \
AlphaNumericField.class \
AppLocation.class \
BrowserLauncher.class \
CutSetsDialog.class \
FloatField.class \
OpenFTA.class \
FTACanvas.class \
FTAConstants.class \
FTAFileFilter.class \
FTAFrame.class \
FTAList.class \
FTAUtilities.class \
GateDialog.class \
GenerateMonteCarlo.class \
IntegerField.class \
IntermediateEventDialog.class \
JarResources.class \
MonteCarloDialog.class \
NumericalProbabilityDialog.class \
PEDConstants.class \
PEDFrame.class \
Preferences.class \
PrintPreferences.class \
PrintPreviewCanvas.class \
PrintUtilities.class \
RestrictedDocument.class \
RestrictedLengthAlphaNumericField.class \
RestrictedLengthExclusiveField.class \
RestrictedNumberDocument.class \
RestrictLengthField.class \
ScrolledTextFrame.class \
SetZoomDialog.class \
StringUtilities.class \
TextPrinter.class \
TextPrint.class \
TransferTreeDialog.class \
TreeOverviewFrame.class \
TreeTextDialog.class \
WorkingDialog.class

HEADERS = $(CLASSES:.class=.h)

LIBNAME = FTAGUI.jar



# Define targets
# Define in order to be presented in MakeTool
#

all:    $(CLASSES) $(HEADERS)
	cd .. ; \
	$(JAR) $(JARFLAGS) $(LIBNAME) FTAGUI/*.class

clean:
	-/bin/rm -r *.class *.h ../$(LIBNAME)


veryclean:
	make clean


spotless:
	make veryclean
	-/bin/rm -r *% *~ *.old

