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
 Module : undo
 Author : D.G Williams
          FSC Limited 
 Date   : 12/6/95
 
 SccsId :@(#)undo.h	1.1 08/22/96

 Implements undo option for edit menu

  keep a static variable of type Undo to record the last edit.
  provide functions to record the last edit and to do the undo.

  The undo() function applies only to options in the EDIT menu, and to
  adding a new item from the symbol menu. It restores the tree
  (including the copy buffer) to the state before the last edit action.
  Only the last edit action may be undone. Selecting Undo again must
  return the tree to its original state.

  After an edit (in edit.c, symbol_menu.c, or here) record_edit() must be
  called, with the correct arguments.

  The following kinds of edits are distinguished, with data:

 type    : CUT
 item    : parent of cut item
 subtree : the old contents of the COPY buffer
 sibling : sibling of cut item
 [NOTE   : the cut subtree is stored in the COPY buffer]

 type    : UNDO_CUT
 item    : the restored item
 subtree : -
 sibling : -

 type    : DELETE                     (a delete or an undo-add)
 item    : parent of deleted item
 subtree : deleted subtree
 sibling : sibling of deleted item

 type    : ADD                        (a paste, new symbol, or undo-delete)
 item    : the added item
 subtree : -
 sibling : -

 type    : CHANGE
 item    : the changed item
 subtree : copy as it was before
 sibling : -

 type    : SHIFTR                        (a shift-right or undo-shift-left)
 item    : shifted item
 subtree : -
 sibling : -

 type    : SHIFTL                        (a shift-left or undo-shift-right)
 item    : shifted item
 subtree : -
 sibling : -

 type    : COPY
 item    : copied item
 subtree : old copy buffer
 sibling :

 type    : UNDO_COPY
 item    : copied item
 subtree : restored copy buffer (will already contain this)
 sibling : -
 
****************************************************************/

#ifndef undo_h
#define undo_h

#include "item.h"
#include "treeP.h"

/*--------------------------------------------------------------
 type to represent type of edit operation
---------------------------------------------------------------*/
typedef enum edit_type {
    CUT, UNDO_CUT, DELETE, 
    ADD, CHANGE, SHIFTR, SHIFTL, 
    COPY, UNDO_COPY, NONE
} EDIT_TYPE;


/*--------------------------------------------------------------
 Routine : record_edit
 Purpose : record edit in undo buffer
---------------------------------------------------------------*/
extern void
  record_edit(
    TREE       *tree,        /* tree                            */
    EDIT_TYPE   type,        /* type of last edit operation     */
    ITEM       *item,        /* item associated with edit       */
    ITEM       *subtree,     /* sub-tree associated with edit   */
    ITEM       *sibling);    /* sibling of item                 */



#endif


/*--------------------------------------------------------------
 Routine : undo
 Purpose : undo last edit operation

  switch (last operation)
  CUT :
     insert COPY buffer in tree
     put UNDO buffer back in COPY buffer
     set last edit to UNDO_CUT
  UNDO_CUT :
     record COPY buffer in UNDO buffer
     record item in COPY buffer
     delete item
     set last edit to CUT
  DELETE :
     insert UNDO buffer in tree
     set last edit to ADD
  ADD  :
     delete the added subtree (put in UNDO buffer)
     set last edit to DELETE
  CHANGE :
     record fields of item in tmp
     copy to item from undo buffer item
     copy from tmp to undo buffer
     set last edit to CHANGE
  SHIFTR :
     do a shift left
     set last edit to SHIFTL
  SHIFTL :
     do a shift right
     set last edit to SHIFTR
  COPY   :
     copy from UNDO buffer to COPY buffer
     set last edit to UNDO_COPY
  UNDO_COPY :
     copy item to COPY buffer
---------------------------------------------------------------*/
extern void
  undo();
