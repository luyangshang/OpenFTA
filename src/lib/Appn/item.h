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
 Module : Item
 Author : FSC Limited
 Date   : 24/7/96
 
 SccsId : @(#)item.h	1.3 01/28/97

 Origin : FTA project,
          FSC Limited Private Venture, Level 2 Prototype.
 
 This module creates and manipulates the ITEM structure for the 
 given fault tree.

 The items hold all the tree construction data, and hold two
 views of the tree. The items know where they are drawn on the
 canvas.
***************************************************************/

#ifndef _ITEM_H
#define _ITEM_H

#include <stdio.h>
#include "boolean.h" 
#include "basic.h" 

/*---------------------------------------------------------------
 These macros define the types of events known to FTA
---------------------------------------------------------------*/
#define UNKNOWN             '?'
#define BASIC               'B'
#define AND                 'A'
#define OR                  'O'
#define TRANSIN             'I'
#define TRANSOUT            'T'
#define INTERMEDIATE        'M'
#define UNDEVELOP           'U'
#define INHIBIT             'H'
#define PRIORITY_AND        'P'
#define XOR                 'X'
#define EXTERNAL            'E'
#define COND_NOT_ANAL       'N'
#define COND_ANAL           'C'
#define DORMANT             'D'

/*---------------------------------------------------------------
 This macro defines the maximum length in characters that the
 descriptive text of a symbol may take.
---------------------------------------------------------------*/
#define MAX_SYMBOL_DESC_LENGTH             100

/*---------------------------------------------------------------
 These macros define the number of tree pics that are suported, and can
 be displayed. At present, only two are defined, the main tree view
 and the global tree view.
---------------------------------------------------------------*/
#define MAIN_VIEW_SET_NO    0
#define GLOBAL_VIEW_SET_NO  1
#define MAX_SET_NO          GLOBAL_VIEW_SET_NO + 1

/*---------------------------------------------------------------
 This type provides access to the individual components of an 
 item in the tree.
---------------------------------------------------------------*/
typedef struct item{
    int        type;
    int        dormant;
    char       id[MAX_SYMBOL_ID_LENGTH];
    EVENT      *event;

 /* tree structure */
    int            num_children;
    struct item    *parent;
    struct item    *child;
    struct item    *right_sib;
    struct item    *left_sib;
    char           *text;

 /* Information stored for the level structure */
    int             level;
    struct item     *lptr;

 /* Processing pointers */
    struct item     *process_child;     
    struct item     *process_sibling;

 /* Co-ordinates of item */
    float    x, y;       /* relative co-ordinates     */
    int      da_x[MAX_SET_NO], da_y[MAX_SET_NO]; /* drawing area co-ordinates */

} ITEM;


/*---------------------------------------------------------------
 Routine : generate_processing_pointers
 Purpose : Recursively generates a tree of items which is equivalent 
 to the standard tree except that all the intermediate events and 
 transfer out items are discarded.

 Returns a pointer to the first child which is not an intermediate 
 event.

 The items hold the pointers for both views of the tree rather than
 having two separate trees pointing to the common items.
---------------------------------------------------------------*/
extern ITEM *
  generate_processing_pointers(
    ITEM *ip);


/*---------------------------------------------------------------
 Routine : is_gate
 Purpose : To return an indication of whether the supplied item
 has the type of AND, OR, XOR, PRIORITY_AND or INHIBIT
---------------------------------------------------------------*/
extern BOOL 
  is_gate(
    ITEM *item);

/*---------------------------------------------------------------
 Routine : is_primary
 Purpose : To return an indication of whether the supplied item
 has the type of BASIC, UNDEVELOPED, CONDITIONAL or EXTERNAL
---------------------------------------------------------------*/
extern BOOL 
  is_primary(
    ITEM *item);

/*---------------------------------------------------------------
 Routine : is_dormant
 Purpose : To return an indication of whether the supplied item
 is a dormant event 
---------------------------------------------------------------*/
extern BOOL 
  is_dormant(
    ITEM *item);

/*---------------------------------------------------------------
 Routine : is_transfer
 Purpose : To return an indication of whether the supplied item
 has the type of TRANSFER-IN or TRANSFER-OUT
---------------------------------------------------------------*/
extern BOOL 
  is_transfer(
    ITEM *item);

/*---------------------------------------------------------------
 Routine : is_condition
 Purpose : To return an indication of whether the supplied item
 has the type of COND_ANAL or COND_NOT_ANAL
---------------------------------------------------------------*/
extern BOOL 
  is_condition(
    ITEM *item);


/*---------------------------------------------------------------
 Routine : make_item
 Purpose : Makes an item structure and returns a pointer to it.

 Memory is allocated to hold the item.
---------------------------------------------------------------*/
extern ITEM *
  make_item(void);


/*---------------------------------------------------------------
 Routine : delete_subtree
 Purpose : Recursively deletes the given item and items below it.

 All memory allocated to hold the items is returned.
---------------------------------------------------------------*/
extern void 
  delete_subtree(
    ITEM *ip);


/*---------------------------------------------------------------
 Routine : update_subtree
 Purpose : Recursively refreshes the item and all items below it
 so that their information reflects the associated data found in
 the Primary Events Database if the item is a primary event.

 This information includes the description, the type and whether
 the event is dormant.
---------------------------------------------------------------*/
extern void
  update_subtree(
    ITEM *ip,
    char *database );


/*---------------------------------------------------------------
 Routine : item_new_description
 Purpose : To set the description for item to that supplied.
---------------------------------------------------------------*/
extern void
  item_new_description(
    ITEM *item, 
    char *desc);


/*---------------------------------------------------------------
 Routine : load_item
 Purpose : Recursively loads the item and all items in the tree 
 below, building the internal data structures.

 Each item is linked with it's parent and siblings to form a
 tree structure. It would be better to restrict tree building
 to the tree module, but the work involved precludes this at
 this time (25/7/96)

 Memory is allocated to hold the items.

 If an item can not be loaded NULL is returned.
---------------------------------------------------------------*/
extern ITEM *
  load_item(
    FILE *fp, 
    ITEM *parent,
    char *database );



/*---------------------------------------------------------------
 Routine : copy_subtree
 Purpose : Recursively copies the given item and items below it,
 returning a pointer to the copy.

 This copying process allocates memory for each item copied.
---------------------------------------------------------------*/
extern ITEM * 
  copy_subtree(
    ITEM *ip);


/*---------------------------------------------------------------
 Routine : save_item
 Purpose : Recursively saves the given item, and all items below 
 it, to supplied file.
---------------------------------------------------------------*/
extern void 
  save_item(
    FILE *fp, 
    ITEM *ip);


#endif
