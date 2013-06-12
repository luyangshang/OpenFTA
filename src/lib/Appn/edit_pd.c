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
 Module : edit_pd
 Author : FSC Limited
 Date   : 13/8/96

 SccsId : @(#)edit_pd.c	1.12 01/28/97

 Origin : FTA project,
          FSC Limited Private Venture.

 This module implements the pulldown menu for the Edit option of
 the menu bar.
***************************************************************/

#include <stdlib.h>
#include <string.h>

#include "edit_pd.h"
#include "fta.h"        /* Imports NULL */
#include "Dialogs.h"
#include "symbol.h"
#include "treeUtil.h"
#include "tree_pic.h"
#include "tree_pic_forest.h"
#include "display_tree.h"
#include "PED_shell.h"
#include "primary_event.h"
#include "util.h"
#include "MemoryUtil.h"
#include "undo.h"
#include "AssertUtil.h"

#include "event_list.h"

#include "NativeFTAFrame.h"
#include "NativeFTACanvas.h"


#define FTA_EVENT_TYPES_DIFFER \
"The type of the selected tree event differs from the\n\
selected Database event.\n\n\
Change to the database type?"

#define DELETE_WARNING_QUESTION \
"WARNING: Selected item and any children will be deleted\n\
                   Deleted items can not be recovered\n\n\
Delete anyway?"

#define COPY_BUFFER_EMPTY_ERROR \
    "Copy buffer empty"

#define NO_SHIFT_WARNING \
     "This symbol type may not be shifted"

#define CANT_CUT_TOP_ITEM_WARNING \
     "Top item may not be deleted"

#define CONDITIONING_EVENT_ALREADY_EXISTS \
"The parent gate already has a conditioning event associated with it"

#define CANNOT_ATTACH_CONDITIONING_EVENT \
"Cannot associate a conditioning event with an intermediate event"

typedef struct _event_parcel
{
  TREE_PIC *tree_pic;
  ITEM *selected_item;
  PRIMARY_EVENT *event;
}    EVENT_PARCEL;


/*--------------------------------------------------------------
 Routine : MakeCopy
 Purpose : Make a copy of the tree from the supplied item, to the
 copy buffer.
 Sets the sensitivity of the Paste pulldown and toolbar button.
--------------------------------------------------------------*/
static void
MakeCopy( 
         EDIT_TYPE ForEditType,
         ITEM *OfItem,
         ITEM *AndSibling,
         ITEM *Focus,
         TREE_PIC_FOREST *tree_pic_forest )
{
	ITEM *copy;

	copy_to_buffer( tree_pic_forest->tree_pic->tree, Focus );

	record_edit( 
              tree_pic_forest->tree_pic->tree,
              ForEditType,
              OfItem,
              copy = get_paste_item( tree_pic_forest->tree_pic->tree ),
              AndSibling );

	/* free unnecessary copy */
	delete_subtree( copy );

} /* MakeCopy */


/*---------------------------------------------------------------
 Routine : change_cb
 Purpose : Callback routine for OK button of question dialog if
 when linking a primary event on the tree to a primary event in
 the database, their types are different.
---------------------------------------------------------------*/
static change_cb(EVENT_PARCEL *change)
{
  BOOL perform_change = TRUE;

  /* don't perform change if new event type is conditioning
     and a conditioning event already exists on the parent gate 
	 or if parent is not a gate */
  if(change->selected_item->parent &&
	 (primary_event_item_type( change->event->type ) == COND_NOT_ANAL ||
	  primary_event_item_type( change->event->type ) == COND_ANAL)) {

	if(is_gate(change->selected_item->parent)) {
      /* check to see if first child of gate is a conditioning event */
      /* cannot change selected item, unless it is the existing conditioning event */
      if((change->selected_item->parent->child->type == COND_NOT_ANAL ||
		 change->selected_item->parent->child->type == COND_ANAL) &&
		 change->selected_item != change->selected_item->parent->child) {

		 FTAFramePostWarning(CONDITIONING_EVENT_ALREADY_EXISTS, FTA_WARNING_TITLE);
         perform_change = FALSE;

	  }
	} else {
      /* cannot attach condtioning event to anything other than a gate */

        FTAFramePostWarning(CANNOT_ATTACH_CONDITIONING_EVENT, FTA_WARNING_TITLE);
		perform_change = FALSE;
	}
  }
	
  if(perform_change) {	
    if ( ( change->selected_item->type !=
         primary_event_item_type( change->event->type ) ) ||
        ( change->selected_item->dormant != change->event->dormant ) )
	{
      change->tree_pic->tree->edited = TRUE;
	}

    /* Only change Id if type hasn't changed, or change has been
       accepted. MPA 12/6/96 */
    if ( strcmp( change->selected_item->id, change->event->id ) != 0 )
	{
      strcpy( change->selected_item->id, change->event->id );
      change->tree_pic->tree->edited = TRUE;
	}

    item_new_description( change->selected_item, change->event->desc );
    change->selected_item->type = primary_event_item_type( change->event->type );
    change->selected_item->dormant = change->event->dormant;

    /* remake level structure */
    make_all_levels(change->tree_pic->tree); 
    tree_edited(change->tree_pic->tree); 

  }

  FreeMemory( change );

  return 0;

} /* change_cb */


/*--------------------------------------------------------------
 Routine : EditLinkAction
 Purpose : To link the selected item on the tree to the selected
 event in the database.

 This action can be taken as a result of the user performing a
 menu selection, or from a toolbar button.
---------------------------------------------------------------*/
void EditLinkAction(TREE_PIC *tree_pic, PRIMARY_EVENT *event)
{
  ITEM *selected_item;
  EVENT_PARCEL *change;

      if (  selected_item = tree_pic->selected_item)
      {
        if ( is_primary( selected_item ) )
        {
  
          if ( event->id != NULL )
          {

            if ( !fNewMemory( ( void * ) &change, sizeof( EVENT_PARCEL ) ) )
            {
              exit( 1 );
            }

            change->tree_pic = tree_pic;
            change->selected_item = selected_item;
            change->event = event;

            if ( selected_item->type !=
                primary_event_item_type( event->type ) )
            {
				if(FTAFrameAskQuestion(FTA_EVENT_TYPES_DIFFER, FTA_QUESTION_TITLE)) {
					change_cb(change);
				}

            }
            else
            {
              change_cb(change);

            }
          }        /* event != NULL */
          else
          {
            FTAFramePostWarning(NO_DB_ITEM_SELECTED_WARNING, FTA_WARNING_TITLE);
          }
        }
        else
        {
          FTAFramePostWarning("Primary event NOT selected\n", FTA_WARNING_TITLE);
        }
      }
      else
      {
        FTAFramePostWarning(NO_ITEM_SELECTED_WARNING, FTA_WARNING_TITLE);
        return;
      }

	update_tree(tree_pic);

} /* EditLinkAction */


/*--------------------------------------------------------------
 Routine : EditUndoAction
 Purpose : To undo the last edit.

 This action can be taken as a result of the user performing a
 menu selection.
---------------------------------------------------------------*/
void EditUndoAction(TREE_PIC_FOREST *tree_pic_forest){
	undo();
	update_tree(tree_pic_forest->tree_pic);
	TreePicSetSelectedItem( tree_pic_forest->tree_pic, NULL );
	TreePicSetSelectedItem( tree_pic_forest->global_tree_pic, NULL );
} /* EditUndoAction */


/*--------------------------------------------------------------
 Routine : EditDeleteAction
 Purpose : To delete the selected item (and its children) fom the tree.

 This action can be taken as a result of the user performing a
 menu selection, or from a toolbar button.
---------------------------------------------------------------*/
void EditDeleteAction(TREE_PIC_FOREST *tree_pic_forest)
{
	ITEM *selected_item, *parent;

	if ( ( selected_item = TreePicGetSelectedItem( tree_pic_forest->tree_pic ) ) != NULL ) {

        /* if top item, disallow */
        if ( selected_item == tree_pic_forest->tree_pic->tree->top_item )
        {
          FTAFramePostWarning(CANT_CUT_TOP_ITEM_WARNING, FTA_WARNING_TITLE);
          return;
        }

        parent = selected_item->parent;

        record_edit( tree_pic_forest->tree_pic->tree,
                    DELETE,
                    parent,
                    selected_item,
                    selected_item->right_sib ); 

        /* delete selected item */
        delete_child( tree_pic_forest->tree_pic->tree, selected_item );

		/* Invalidate processing pointers now that an item has been deleted */
		tree_pic_forest->tree_pic->tree->process_top_item = NULL;

        /* select the parent */
        TreePicSetSelectedItem( tree_pic_forest->tree_pic, parent );
        //TreePicSetSelectedItem( tree_pic_forest->global_tree_pic, parent );
        //set_symbols_sens( tree_pic_forest, parent );

	} else {
        FTAFramePostWarning(NO_ITEM_SELECTED_WARNING, FTA_WARNING_TITLE);
	}

	update_tree(tree_pic_forest->tree_pic);

} /* EditDeleteAction */



/*--------------------------------------------------------------
 Routine : EditShiftLeftAction
 Purpose : To shift the selected item to the left.

 This action can be taken as a result of the user performing a
 menu selection.
---------------------------------------------------------------*/
void EditShiftLeftAction(TREE_PIC_FOREST *tree_pic_forest){
	ITEM *selected_item;
	
	if ((selected_item = TreePicGetSelectedItem(tree_pic_forest->tree_pic))) {

		/* do not shift a condition or a transfer-out */
        if (selected_item->type == COND_NOT_ANAL ||
			selected_item->type == COND_ANAL ||
            selected_item->type == TRANSOUT )
		{
			FTAFramePostWarning(NO_SHIFT_WARNING, FTA_WARNING_TITLE);
			return;
        }

        /* do not shift an item past a condition or a transfer-out */
        if (selected_item->left_sib == NULL ||
            selected_item->left_sib->type == COND_NOT_ANAL ||
            selected_item->left_sib->type == COND_ANAL ||
            selected_item->left_sib->type == TRANSOUT )
        {
			/* DO NOTHING */
			return;
        }

        /* shift item and record edit */
        shift_item_left( tree_pic_forest->tree_pic->tree, selected_item );
        record_edit( tree_pic_forest->tree_pic->tree,
                    SHIFTL,
                    selected_item,
                    NULL,
                    NULL );

	} else {
		FTAFramePostWarning(NO_ITEM_SELECTED_WARNING, FTA_WARNING_TITLE);
        return;
    }

	update_tree(tree_pic_forest->tree_pic);

} /* EditShiftLeftAction */



/*--------------------------------------------------------------
 Routine : EditShiftRightAction
 Purpose : To shift the selected item to the right.

 This action can be taken as a result of the user performing a
 menu selection.
---------------------------------------------------------------*/
void EditShiftRightAction(TREE_PIC_FOREST *tree_pic_forest){
	ITEM *selected_item;
 
	if((selected_item = TreePicGetSelectedItem(tree_pic_forest->tree_pic))) {

		/* do not shift a condition or a transfer-out */
        if (selected_item->type == COND_NOT_ANAL ||
			selected_item->type == COND_ANAL ||
            selected_item->type == TRANSOUT )
        {
			FTAFramePostWarning(NO_SHIFT_WARNING, FTA_WARNING_TITLE);
			return;
        }

        if ( selected_item->right_sib == NULL )
        {
			/* DO NOTHING */
			return;
        }

        shift_item_right(tree_pic_forest->tree_pic->tree, selected_item);

        record_edit(tree_pic_forest->tree_pic->tree,
                    SHIFTR,
                    selected_item,
                    NULL,
                    NULL);

	} else {
        FTAFramePostWarning(NO_ITEM_SELECTED_WARNING, FTA_WARNING_TITLE);
        return;
    }

	update_tree(tree_pic_forest->tree_pic);

} /* EditShiftRightAction */



/*--------------------------------------------------------------
 Routine : EditCutAction
 Purpose : To cut a selected portion of the tree to the copy
 buffer.

 This action can be taken as a result of the user performing a
 menu selection, or from a toolbar button.
---------------------------------------------------------------*/
void EditCutAction(TREE_PIC_FOREST *tree_pic_forest)
{
	ITEM *selected_item;
	ITEM *parent;

	if((selected_item = TreePicGetSelectedItem(tree_pic_forest->tree_pic)) != NULL){

		/* if top item, disallow */
		if ( selected_item == tree_pic_forest->tree_pic->tree->top_item )
		{
			FTAFramePostWarning(CANT_CUT_TOP_ITEM_WARNING, FTA_WARNING_TITLE);
			return;
		}

		parent = selected_item->parent;

		MakeCopy(CUT,
				parent,
				selected_item->right_sib,
				selected_item,
				tree_pic_forest );

		/* delete selected item */
		delete_child( tree_pic_forest->tree_pic->tree, selected_item );

		/* Invalidate processing pointers now that an item has been cut out */
		tree_pic_forest->tree_pic->tree->process_top_item = NULL;

		/* select the parent */
		TreePicSetSelectedItem( tree_pic_forest->tree_pic, parent );
		TreePicSetSelectedItem( tree_pic_forest->global_tree_pic, parent );
	} else {
		FTAFramePostWarning(NO_ITEM_SELECTED_WARNING, FTA_WARNING_TITLE);
		return;
	}

	update_tree(tree_pic_forest->tree_pic);

} /* EditCutAction */



/*--------------------------------------------------------------
 Routine : EditCopyAction
 Purpose : To copy a selected portion of the tree to the copy
 buffer.

 This action can be taken as a result of the user performing a
 menu selection, or from a toolbar button.
---------------------------------------------------------------*/
void EditCopyAction(TREE_PIC_FOREST *tree_pic_forest)
{
	ITEM *selected_item;

	if((selected_item = TreePicGetSelectedItem(tree_pic_forest->tree_pic)))
	{
		MakeCopy( 
				COPY,
				selected_item,
				NULL,
				selected_item,
				tree_pic_forest );
	} else {
		FTAFramePostWarning(NO_ITEM_SELECTED_WARNING, FTA_WARNING_TITLE);
		return;
	}

	update_tree(tree_pic_forest->tree_pic);

} /* EditCopyAction */



/*--------------------------------------------------------------
 Routine : EditPasteAction
 Purpose : To paste the tree in the copy buffer to the selected
 item on the tree.

 This action can be taken as a result of the user performing a
 menu selection, or from a toolbar button.
---------------------------------------------------------------*/
void EditPasteAction(TREE_PIC_FOREST *tree_pic_forest)
{
	ITEM *selected_item;

	if((selected_item = TreePicGetSelectedItem(tree_pic_forest->tree_pic))) {
		ITEM *paste;

		/* add item from cut buffer */
		if((paste = get_paste_item(tree_pic_forest->tree_pic->tree)) != NULL) {

			/* Add the tree in the clipboard to the tree at the selected
			item. This call removes the duplication of code between the
			symbol_menu_cb and edit_menu_cb. MPA 12/4/96 */
			if(!tree_add(paste, selected_item, tree_pic_forest->tree_pic)) {
				delete_subtree( paste );
				return;
			}

			/* Make the primary event descriptions match those in the
			database, if they exist. MPA 3/7/96 */
			update_subtree(selected_item,
						tree_pic_forest->tree_pic->tree->database );
		} else {
			FTAFramePostWarning(COPY_BUFFER_EMPTY_ERROR, FTA_ERROR_TITLE);
			return;
		}

	} else {
		FTAFramePostWarning(NO_ITEM_SELECTED_WARNING, FTA_WARNING_TITLE);
		return;
	}

	update_tree(tree_pic_forest->tree_pic);

} /* EditPasteAction */




