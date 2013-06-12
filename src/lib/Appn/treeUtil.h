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
 Module : Tree
 Author : FSC Limited
 Date   : 24/7/96

 SccsId: @(#)treeUtil.h	1.3 11/12/96

 Origin : FTA project,
          FSC Limited Private Venture, Level 2 Prototype.

 This module implements a tree. The tree contains details of the 
 item and level structure of the tree.

 This header file defines the public view of a tree.

 Trees and Items display a confusing degree of metamorphosis
 from one to the other. Items should exist as nodes within trees
 but here trees are made up of connected items. Hence item.h
 contains *tree routines that might be expected to be exported
 from here.
***************************************************************/

#ifndef TREE_H
#define TREE_H

#include <stdio.h>
#include "boolean.h"
#include "item.h"
#include "treeP.h"
#include "tree_pic.h"

/*---------------------------------------------------------------
 These macros are defined to assist tree layout and are described
 in the terms of the relative coordinate system
---------------------------------------------------------------*/
#define HORIZ_SPACING   1.4  /* space between items on the same level of tree */
#define VERT_SPACING    1.0  /* space between levels of tree                  */
#define CONNECT_LENGTH 0.25  /* height connecting line going up from a child  */
#define COND_SPACING    1.0  /* space between gate and condition (to right)   */
#define TRANS_OUT_RIGHT 1.5  /* space from top event and transfer-out (right) */
#define TRANS_OUT_UP    0.5  /* space from top event and transfer-out (up)    */
#define TRANS_OUT_BIT   0.09 /* distance from top of transfer-out to connect. */

/*---------------------------------------------------------------
 Define the maximum order allowed for a tree. The order is the
 number of events in a cut set
---------------------------------------------------------------*/
#define MAX_ORDER_SIZE 32767

/*---------------------------------------------------------------
 Added 7/3/96 MPA - Define Error codes for tree file access
---------------------------------------------------------------*/
#define TREE_OK 0
#define TREE_FILE_ERROR 1
#define TREE_ERROR 2


/*---------------------------------------------------------------
 Routine : tree_new
 Purpose : To reinitialise all tree data to a state consistent 
 with a new tree
---------------------------------------------------------------*/
extern void
  tree_new(
    TREE *tree);


/*---------------------------------------------------------------
 Routine : make_all_levels
 Purpose : To make the level structure for the tree. The level 
 structure is required for the automatic layout of the tree on
 the drawing area.
---------------------------------------------------------------*/
extern void
  make_all_levels(
    TREE *tree);


/*---------------------------------------------------------------
 Routine : delete_tree
 Purpose : To delete the tree from memory, and reset the tree data 
 structures to the new tree state.
---------------------------------------------------------------*/
extern void
  delete_tree(
    TREE *tree);


/*---------------------------------------------------------------
 Routine : replant_tree
 Purpose : To re-initialise an existing tree and all the structures
 related to a tree.
---------------------------------------------------------------*/
extern void
  replant_tree(
    TREE *tree);


/*---------------------------------------------------------------
 Routine : tree_draw
 Purpose : To draw the tree on the related canvas with the specified
 item at the top centre of the viewport.
---------------------------------------------------------------*/
extern BOOL
  tree_draw( 
    TREE_PIC *tree_pic,    /* tree to draw     */
    ITEM     *item );      /* item to focus on */



/*---------------------------------------------------------------
 Routine : resize_da
 Purpose : To resize the drawing area widge to fit the tree.
---------------------------------------------------------------*/
extern BOOL
  resize_da( 
    TREE_PIC *tree_pic);



/*---------------------------------------------------------------
 Routine : tree_add
 Purpose : Adds the addition tree below the to_item
---------------------------------------------------------------*/
extern BOOL
  tree_add(
    ITEM *addition,
    ITEM *to_item,
    TREE_PIC *tree_pic );


/*---------------------------------------------------------------
 Routine : add_child
 Purpose : Add an item to the tree as rightmost child of parent.

 The exceptions to the rule of adding as the rightmost child of a 
 parent are Conditions and Transfer-Out items. These are added as
 leftmost child of the parent.

 The routine presumes that both parent and child exist.
---------------------------------------------------------------*/
extern void
  add_child(
    TREE *tree, 
    ITEM *parent, 
    ITEM *child);



/*---------------------------------------------------------------
 Routine : tree_edited
 Purpose : To set the flags to indicated that the tree has been
 edited
---------------------------------------------------------------*/
extern void
  tree_edited(
    TREE *tree);


/*---------------------------------------------------------------
 Routine : load_tree
 Purpose : To load the tree data from the tree file

 TREE_FILE_ERROR or TREE_ERROR is returned if tree cannot be loaded
---------------------------------------------------------------*/
extern int
  load_tree(
    TREE *tree, 
    char *filename);


/*---------------------------------------------------------------
 Routine : load_dbname
 Purpose : To load the name of the associated database from tree 
 file (first line in file). The database name is defined in the 
 file as an absolute pathname.
---------------------------------------------------------------*/
extern char *
  load_dbname( 
    FILE *fp );


/*---------------------------------------------------------------
 Routine : load_post_it_note
 Purpose : To load the descriptive text for the tree from the
 tree file.
---------------------------------------------------------------*/
extern char *
  load_post_it_note(
    FILE *fp );


/*---------------------------------------------------------------
 Routine : update_prim_events_tree
 Purpose : To update primary events in the tree with data from 
 database
---------------------------------------------------------------*/
extern void
  update_prim_events_tree(
    TREE *tree);


/*---------------------------------------------------------------
 Routine : delete_child
 Purpose : To delete item (and its subtree) from tree
---------------------------------------------------------------*/
extern void
  delete_child(
    TREE *tree, 
    ITEM *ip);



/*---------------------------------------------------------------
 Routine : insert_child
 Purpose : To insert the item to the tree as specified child of 
 parent
---------------------------------------------------------------*/
extern void
  insert_child(
    TREE *tree, 
    ITEM *parent, 
    ITEM *child, 
    ITEM *sibling);


/*---------------------------------------------------------------
 Routine : get_paste_item
 Purpose : To return the copy of item (and its whole subtree)
 from the copy buffer 
---------------------------------------------------------------*/
extern ITEM *
  get_paste_item(
    TREE *tree);


/*---------------------------------------------------------------
 Routine : copy_to_buffer
 Purpose : To put a copy of item (and it's whole subtree) in the 
 copy buffer
---------------------------------------------------------------*/
extern void
  copy_to_buffer(
    TREE *tree, 
    ITEM *item);


/*---------------------------------------------------------------
 Routine : shift_item_left
 Purpose : To swap item with its left-hand sibling
---------------------------------------------------------------*/
extern void
  shift_item_left(
    TREE *tree, 
    ITEM *item);


/*---------------------------------------------------------------
 Routine : shift_item_right
 Purpose : To swap item with its right-hand sibling
---------------------------------------------------------------*/
extern void
  shift_item_right(
    TREE *tree, 
    ITEM *item);


/*---------------------------------------------------------------
 Routine : update_tree
 Purpose : To update the tree, calculating position of all items.
---------------------------------------------------------------*/
extern BOOL update_tree(TREE_PIC *tree_pic);

/*---------------------------------------------------------------
 Routine : getMaxWidth
 Purpose : returns the max width of the canvas
---------------------------------------------------------------*/
extern int getMaxWidth(); 


/*---------------------------------------------------------------
 Routine : getMaxDepth
 Purpose : returns the max depth of the canvas
---------------------------------------------------------------*/
extern int getMaxDepth(); 

/*---------------------------------------------------------------
 Routine : validate_tree
 Purpose : To check tree for validity (for analysis).

 The routine returns an indication of whether the tree is valid or
 invalid.
---------------------------------------------------------------*/
extern BOOL
  validate_tree(
    TREE *tree, 
    ITEM *ip, 
    FILE *report_file);

/*---------------------------------------------------------------
 Routine : save_tree_to_file
 Purpose : To save the tree data to the specified tree file

 TREE_FILE_ERROR or TREE_ERROR is returned if tree cannot be saved.
---------------------------------------------------------------*/
extern int
  save_tree_to_file(
    void *tree_data, 
    char *filename);


/*---------------------------------------------------------------
 Routine : count_lambda_events
 Purpose : To count the events identified in the tree that have
 been declared with probabilities that are calculated per unit
 time. e.g., Chance of failure Once per Blue Moon

 If any lambda events are found, the tree is updated with the
 count, and the Quantative and Monte Carlo Analyses will prompt
 for the number of unit times to be used in their calculations.
---------------------------------------------------------------*/
extern void
  count_lambda_events(
    TREE *tree,
    ITEM *ip );


/*---------------------------------------------------------------
 Routine : eval_tree
 Purpose : To evaluate the tree from the supplied item. Evaluation
 in this context means to determine whether the tree decomposes down
 to be TRUE or FALSE. (recursive)

 This evaluation is achieved by recursively traversing down the tree
 finding the truth of each subtree within the tree.

 e.g., A or B is true if A or B is true
       C and D is true if C and D are true.
---------------------------------------------------------------*/
extern BOOL
  eval_tree(
    ITEM *ip);


#endif
