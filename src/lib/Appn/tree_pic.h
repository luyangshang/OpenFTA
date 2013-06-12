///////////////////////////////////////////////////////////////////////////////
// 
// OpenFTA - Fault Tree Analysis
// Copyright (C) 2005 FSC Limited
// 
// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by the Free
// Software Foundation; either version 2 of the License, or (at your 
// option) any later version.
// 
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or 
// FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for 
// more details.
//
// You should have received a copy of the GNU General Public License along 
// with this program; if not, write to the Free Software Foundation, Inc., 
// 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
//
// To contact FSC, please send an email to support@fsc.co.uk or write to 
// FSC Ltd., Cardiff Business Technology Centre, Senghenydd Road, Cardiff,
// CF24 4AY.
//
///////////////////////////////////////////////////////////////////////////////

/****************************************************************
 Module : Tree Picture
 Author : FSC Limited
 Date   : 22/12/95
 
 SccsId :@(#)tree_pic.h	1.1 08/22/96
 
 This module creates and manipulates a tree picture.
 The tree picture contains information about the fault tree, the canvas
 area which holds the tree as well as the drawing attributes. 

****************************************************************/

#ifndef Tree_pic_h
#define Tree_pic_h

#include "symbol.h"
#include "item.h"
#include "treeP.h"
#include "Canvas.h"


typedef void *TreePicCanvas;
typedef void *TreePicFont;


typedef struct tree_pic{
	BOOL        scrolled_window;
    Canvas      canvas;
    TreePicCanvas   da;
    TREE       *tree;
    ITEM       *selected_item;
    int         da_max_x, da_max_y;
    int         left_offset;
    int         screen_width;
    int         set_no;
    int         scale;
    SYMBOL     *scaled_symbols[SYMB_INVALID];
}TREE_PIC;



/*--------------------------------------------------------------
 Routine : TreePicGetScale
 Purpose : Returns the scale used in the given tree picture.
---------------------------------------------------------------*/
extern int
  TreePicGetScale(
    TREE_PIC *tree_pic);



/*--------------------------------------------------------------
 Routine : TreePicFocus
 Purpose : Places the item in the top centre of the canvas for the
           given tree pictures.
---------------------------------------------------------------*/
extern void
  TreePicFocus(
    TREE_PIC *tree_pic, 
    ITEM *item);



/*--------------------------------------------------------------
 Routine : TreePicSetSelectedItem
 Purpose : Function to set the selected item for the given tree picture.
---------------------------------------------------------------*/
extern void
  TreePicSetSelectedItem(
    TREE_PIC *tree_pic, 
    ITEM *item);



/*--------------------------------------------------------------
 Routine : TreePicGetSelectedItem
 Purpose :  Function to get the selected item for the given tree picture.
---------------------------------------------------------------*/
extern ITEM *
  TreePicGetSelectedItem(
    TREE_PIC *tree_pic);



/*--------------------------------------------------------------
 Routine : TreePicGetMaxSymbWidth
 Purpose : This function finds and returns the maximum symbol
           width of the scaled symbols.
---------------------------------------------------------------*/
extern float
  TreePicGetMaxSymbWidth(
    TREE_PIC *tree_pic);



/*--------------------------------------------------------------
 Routine : TreePicGetMaxSymbDepth
 Purpose : This function finds and returns the maximum symbol
           depth of the scaled symbols.
---------------------------------------------------------------*/
extern float
  TreePicGetMaxSymbDepth(
    TREE_PIC *tree_pic);


/*--------------------------------------------------------------
 Routine : TreePicSetLabel
 Purpose : Sets the label for a treepic
---------------------------------------------------------------*/
extern void
  TreePicSetLabel(
    TREE_PIC *tree_pic,
    char     *label);


#endif
