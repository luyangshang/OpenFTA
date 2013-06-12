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
 Module : level
 Author : FSC Limited
 Date   : 24/7/96
 
 SccsId : @(#)level.h	1.1 08/22/96
 
 Origin : FTA project,
          FSC Limited Private Venture, Level 2 Prototype.
 
 This module creates and manipulates the LEVEL structure for the
 given fault tree.
***************************************************************/

#ifndef _LEVEL_H
#define _LEVEL_H

#include "item.h"
#include "levelP.h"
#include "tree_pic.h"

/*---------------------------------------------------------------
 Routine : set_tree_level
 Purpose : Recursively sets the level of the item and all items 
 in the tree below it.
 Returns the maximum level number.
---------------------------------------------------------------*/
extern int 
  set_tree_level(
    ITEM *ip, int level);


/*---------------------------------------------------------------
 Routine : make_level
 Purpose : Makes a level structure and returns a pointer to it.
 Space is allocated to hold the level.
---------------------------------------------------------------*/
extern LEVEL *
  make_level(
    ITEM *ip, int lvl);


/*---------------------------------------------------------------
 Routine : join_level
 Purpose : Builds the tree level structures.
---------------------------------------------------------------*/
extern void 
  join_level(
    ITEM *ip, 
    LEVEL *lp, 
    int lvl, 
    ITEM *top_level_list);


/*---------------------------------------------------------------
 Routine : delete_level
 Purpose : Recursively deletes the given LEVEL structure and all 
 that leads from it.
 All space allocated to hold the levels is returned.
---------------------------------------------------------------*/
extern void 
  delete_level(
    LEVEL *lp);


/*---------------------------------------------------------------
 Routine : find_level
 Purpose : Finds the level selected in the tree, e.g. selection 
 on the drawing area.
 The y coordinates of the selection is passed to this function 
---------------------------------------------------------------*/
extern LEVEL *
  find_level(
    TREE_PIC *tree_pic, 
    LEVEL *lp, 
    int selected_y);


#endif

