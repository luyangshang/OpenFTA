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

/***************************************************************
 Module : display_tree
 Author : FSC Limited
 Date   : 13/8/96
 
 SccsId : @(#)display_tree.h	1.1 08/22/96
 
 Origin : FTA project,
          FSC Limited Private Venture.
 
 This module provides routines to display a tree.
***************************************************************/

#ifndef display_tree_h
#define display_tree_h

#include "tree_pic.h"
#include "item.h"
#include "level.h"

/*
 * This constant determines the border width of the tree in the
 * relative coordinate system.
 */
#define BORDER_WIDTH 0.2


/*
 * This constant determines the distance between the top of a symbol and
 * the end of the connecting line leading from the symbol.
 * This is set in the relative coordinate system.
 */
#define SYMB_LINE_SPACE (VERT_SPACING - CONNECT_LENGTH)  /* ORIGINAL 1.25 */

/*---------------------------------------------------------------
 Routine : scale_tree
 Purpose : Scales the tree coordinates from the relative 
 coordinate system to the drawing area coordinate system.

 If the scale is set to 0 the tree would appear as a dot.
---------------------------------------------------------------*/
extern void 
  scale_tree(
    TREE_PIC *tree_pic, 
    int scale);



/*---------------------------------------------------------------
 Routine : draw_tree
 Purpose : Draws the tree with respect to the given scale.
---------------------------------------------------------------*/
extern void 
  draw_tree(
    TREE_PIC *tree_pic, 
    int scale);



/*---------------------------------------------------------------
 Routine : draw_post_it_note
 Purpose : This routine draws the descriptive post it note on the 
 Tree Canvas.
---------------------------------------------------------------*/
extern void
  draw_post_it_note(
    TREE_PIC *tree_pic );



/*---------------------------------------------------------------
 Routine : connect_condition
 Purpose : Draw line connecting a gate to its condition.
---------------------------------------------------------------*/
extern void
  connect_condition(
    TREE_PIC *tree_pic, 
    ITEM *gate, 
    ITEM *cond);



/*---------------------------------------------------------------
 Routine : connect_transout
 Purpose : This routine connects a transout symbol with the 
 supplied top item. 
---------------------------------------------------------------*/
extern void
  connect_transout(
    TREE_PIC *tree_pic, 
    ITEM *top, 
    ITEM *transout);



/*---------------------------------------------------------------
 Routine : find_item
 Purpose : Finds the item clicked on in the drawing area.
 If the item is not found NULL is returned.
---------------------------------------------------------------*/
extern ITEM *
  find_item(
    TREE_PIC *tree_pic, 
    LEVEL *lp, 
    int selected_x, 
    int selected_y);




#endif
