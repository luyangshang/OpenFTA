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
display_tree.o \
level.o \
symbol_menu.o \
edit_pd.o \
symbol_text.o \
analysis_pd.o \
event_list.o \
treeUtil.o \
bits.o \
tree_pic.o \
file_pd.o \
undo.o \
util.o \
display_item.o \
segments.o \
view_pd.o \
item.o \
symbol.o \
print_report_pd.o \
FileDialogs.o

LIBNAME = $(BUILDIR)/libappn.a


# Externally visible file dependencies

include ${TOP}/Appn/Exports
include ${TOP}/Analysis/Exports
include ${TOP}/Canvas/Exports
include ${TOP}/PrimaryEvents/Exports
include ${TOP}/Utilities/Exports
include ${TOP}/Widgets/Exports


# Internal dependencies
#
BUTTONDEFS_H=      buttondefs.h
ANALYSIS_PD_H=     analysis_pd.h
#DATABASE_PD_H=     database_pd.h
EVENT_LIST_H=      event_list.h\
  $(BASIC_H) $(TREE_H) $(EXPR_H) $(BITS_H)
LEVEL_H=           level.h\
  $(ITEM_H) $(LEVELP_H) $(TREE_PIC_H)
LEVELP_H=          levelP.h\
  $(ITEM_H)
SEGMENTS_H=        segments.h\
  $(CANVAS_H)
SYMBOL_H=          symbol.h\
  $(BOOLEAN_H) $(SEGMENTS_H) $(CANVAS_H)
SYMBOL_DEFS_H=     symbol_defs.h\
  $(SYMBOL_H) $(SEGMENTS_H) $(FTA_H)
SYMBOL_TEXT_H=     symbol_text.h\
  $(SYMBOL_H) $(CANVAS_H)
UNDO_H=            undo.h\
  $(ITEM_H) $(TREEP_H)
VIEW_PD_H=         view_pd.h


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
all:    $(OBJS)
	if ( test ! -d $(BUILDIR) ) then \
          mkdir $(BUILDIR); \
        fi
	$(AR) $(ARFLAGS) $(LIBNAME) $(OBJS)

clean:
	-/bin/rm -r $(OBJS) $(BUILDIR) $(EXE) core


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
        do (\
                echo Cleansing $$i ...;\
                cd $$i;\
                $(MAKE) spotless FLAGS=$(FLAGS)\
        ); done\


# Object file dependencies
#
analysis_pd.o: $(CUTSET_WID_H) $(PROBSWID_H) $(MONTEWID_H) \
  $(ANALYSIS_PD_H) $(FTA_H) $(TREE_PIC_H) $(MONTE_CARLO_H)\
  $(PROBS_H) $(BASIC_H) $(EVENT_LIST_H) $(ALGEBRAIC_H) $(COMBO_H)\
  $(TREE_PIC_FOREST_H) $(BITS_H) $(UTIL_H) $(DIALOGS_H) $(TREE_H) \
  $(FILE_UTILS_H) $(BTNACT_H) $(MENUBAR_H) \
  $(ASSERT_H)

bits.o: $(BITS_H) $(MEMORY_H)

display_item.o: $(DISPLAY_ITEM_H) $(TREE_H) $(SYMBOL_TEXT_H) \
  $(DISPLAY_TREE_H) $(CANVAS_H) $(SYMBOL_H) $(XCANVAS_H) \
  $(PRIMARY_EVENT_H) $(PED_SHELL_H) $(BTNACT_H) \
  $(STR_UTILS_H) $(TOOLBAR_H)

display_tree.o: $(DISPLAY_TREE_H) $(BOOLEAN_H) $(FTA_H) $(UTIL_H) \
                $(DISPLAY_ITEM_H) $(TREE_H) $(FILE_UTILS_H) $(XCANVAS_H) \
  $(SYMBOL_TEXT_H) $(MEMORY_H) $(ASSERT_H)

edit_pd.o: $(EDIT_PD_H) $(FTA_H) $(SYMBOL_H) $(TREE_H) $(DIALOGS_H)\
  $(DISPLAY_TREE_H) $(TREE_PIC_FOREST_H) $(UNDO_H) \
  $(TREE_PIC_H) $(SYMBOL_MENU_H) $(PED_SHELL_H) $(PRIMARY_EVENT_H) \
  $(BTNACT_H) $(DRAWINGAREA_H) $(UTIL_H) $(MEMORY_H) $(EVENT_LIST_H) \
  $(MENUBAR_H) $(TOOLBAR_H)

event_list.o: $(EVENT_LIST_H) $(TREE_PIC_H) $(TREE_H) $(UTIL_H) $(BITS_H) \
  $(PRIMARY_EVENT_H) $(PED_SHELL_H) $(MEMORY_H)

file_pd.o: $(FILE_PD_H) $(TREE_PIC_H) $(DISPLAY_TREE_H) $(TREE_PIC_FOREST_H) \
  $(FTA_H) $(TREE_H) $(DIALOGS_H) $(PED_SHELL_H) \
  $(FILE_UTILS_H) $(SYMBOL_MENU_H) $(FILE_DIALOGS_H) $(BTNACT_H) $(UTIL_H) \
  $(PLOT_H) $(MENUBAR_H) $(ASSERT_H)

fta.o: $(FTA_H) $(TREE_PIC_H) $(CONFIGURE_H) $(DRAWING_AREA_H) $(MENUBAR_H) \
  $(SYMBOL_MENU_H) $(TREE_PIC_FOREST_H) $(FILE_BASED_HELP_H) $(FILE_PD_H) \
  $(PLOT_H) $(PED_SHELL_H) $(FILE_PD_H) $(EDIT_PD_H) $(TREE_H) $(PRINTERS_H) \
  $(TOOLBAR_H) $(RESOURCE_CONTEXT_H) $(ICON_H) $(PIXMAP_H)

item.o: $(ITEM_H) $(UTIL_H) $(PRIMARY_EVENT_H) $(PED_SHELL_H) $(MEMORY_H) \
  $(ASSERT_H)

level.o: $(LEVEL_H) $(UTIL_H) $(TREE_H) $(MEMORY_H)

segments.o: $(SEGMENTS_H) $(MEMORY_H)

symbol.o: $(SYMBOL_H) $(SYMBOL_DEFS_H) $(MEMORY_H) $(ASSERT_H)

symbol_menu.o: $(SYMBOL_MENU_H) $(TREE_PIC_H) $(DIALOGS_H) $(SYMBOL_H)\
               $(ITEM_H) $(DISPLAY_TREE_H) $(CANVASP_H) $(CANVAS_H)\
               $(PIXMAPXCANVAS_H) $(TREE_H) $(UNDO_H) $(MEMORY_H)

symbol_text.o: $(SYMBOL_TEXT_H) $(BOOLEAN_H) $(MATHMACROS_H) $(STR_UTILS_H) \
  $(MEMORY_H)

treeUtil.o: $(TREE_H) $(FTA_H) $(SYMBOL_DEFS_H) $(TREE_PIC_H) $(ITEM_H) $(UTIL_H)\
  $(LEVEL_H) $(UNDO_H) $(DIALOGS_H) $(PRIMARY_EVENT_H) $(PED_SHELL_H) \
  $(BOOLEAN_H) $(FILE_UTILS_H) $(CUT_SETS_H) $(EVENT_LIST_H) \
  $(DISPLAY_TREE_H) $(XCANVAS_H) $(BTNACT_H) $(MEMORY_H) \
  $(ASSERT_H)

tree_pic.o: $(TREE_PIC_H) $(FTA_H) $(SYMBOL_DEFS_H) $(DRAWING_AREA_H)\
  $(DRAWINGXCANVAS_H) $(CANVAS_H) $(UTIL_H) $(FILE_UTILS_H) $(TREE_H) \
  $(MEMORY_H) $(ASSERT_H)

undo.o: $(UNDO_H) $(ITEM_H) $(TREE_H) $(BTNACT_H) $(TOOLBAR_H)

util.o: $(UTIL_H) $(MEMORY_H)

view_pd.o: $(VIEW_PD_H) $(FTA_H) $(TREE_PIC_H) \
        $(DISPLAY_TREE_H)\
        $(DIALOGS_H) $(TREE_PIC_FOREST_H) $(FILE_UTILS_H) \
  $(BTNACT_H) $(UTIL_H) $(MENUBAR_H)

