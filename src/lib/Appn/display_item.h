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
 Module : display_item
 Author : FSC Limited
 Date   : 24/7/96
 
 SccsId : @(#)display_item.h	1.1 08/22/96
 
 Origin : FTA project,
          FSC Limited Private Venture.
 
 This module provides routines to display an Item on the display
 Canvas.
***************************************************************/

#ifndef display_item_h
#define display_item_h

#include "boolean.h"
#include "item.h"
#include "tree_pic.h"

/* Minimum scale size allowed for displaying text */
#define MIN_SCALE_TO_FIT_TEXT           100

/*---------------------------------------------------------------
 Routine : draw_item_all
 Purpose : Draws the given tree item and its associated text.

 The description text is only displayed for items which are not gates,
 since it is not required for gates.
---------------------------------------------------------------*/
extern void 
  draw_item_all(
    TREE_PIC *tree_pic, 
    ITEM *ip);

/*---------------------------------------------------------------
 Routine : translate_item_type
 Purpose : Converts the integer "type" to SYMBOL_TYPE.

 This conversion is used to allow manipulation of items using their "symbol
 definition".
---------------------------------------------------------------*/
extern SYMBOL_TYPE 
  translate_item_type(
    int type);

/*---------------------------------------------------------------
 Routine : is_point_in_item
 Purpose : Checks if the point is in the boundaries of the item.

 The point is passed to this function as a set of x and y 
 coordinates.
---------------------------------------------------------------*/
extern BOOL 
  is_point_in_item(
    TREE_PIC *tree_pic, 
    ITEM *ip, int selected_x, int selected_y);

/*---------------------------------------------------------------
 Routine : draw_selected_item
 Purpose : This routine deselects any highlighted item, and 
 highlights the supplied item.
---------------------------------------------------------------*/
extern void
  draw_selected_item(
    TREE_PIC *tree_pic, 
    ITEM *selected_item);

#endif

