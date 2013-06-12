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
 Module : Drawing Area
 Author : FSC Limited
 Date   : 29/11/95

 SccsId : @(#)drawingarea.c	1.9 02/04/97
 
 This module implements the Drawing Area widget using the
 Motif toolkit.
 
****************************************************************/

#include <stdlib.h>
#include <string.h>

#include "NativeFTACanvas.h"
#include "NativeGateDialog.h"

#include "PED_shell.h"
#include "drawingarea.h"
#include "AssertUtil.h"
#include "StringUtilities.h"
#include "util.h"
#include "Dialogs.h"
#include "FileUtilities.h"

ASSERTFILE(__FILE__)

#define DRAWING_AREA_GATE_TITLE  FTA_APPN_TITLE " : Gate"
#define DRAWING_AREA_INTERMEDIATE_EVENT_TITLE   FTA_APPN_TITLE " : Intermediate Event"
#define DRAWING_AREA_TREE_ENTRY_TITLE FTA_APPN_TITLE " : Tree Text"
#define DRAWING_AREA_TRANSFER_TREE_TITLE   FTA_APPN_TITLE " : Transfer Tree"
#define DRAWING_AREA_ID_MSG             "Id"
#define DRAWING_AREA_ILLEGAL_ID_WARNING "Illegal ID !"
#define DRAWING_AREA_INCORRECT_TYPE_QUESTION \
"The type of this primary event does not\n\
correspond to the selected symbol type.\n\
Do you want to change the selected symbol type ?"

#define INHIBIT_RULES_EXCEEDED_ERROR \
"The selected gate cannot be changed into\n\
an inhibit gate as it has too many inputs"


/*--------------------------------------------------------------
 Routine : DrawingAreaPostItTextCb
 Purpose : get text (description for tree) from dialog,
---------------------------------------------------------------*/
void DrawingAreaPostItTextCb(
  TREE_PIC_FOREST *tree_pic_forest,
  char     *desc )
{
  /* Populate tree with description */
  if ( tree_pic_forest->tree_pic->tree->post_it_note != NULL ) {
    free( tree_pic_forest->tree_pic->tree->post_it_note );
    tree_pic_forest->tree_pic->tree->post_it_note = NULL;
  }
  if ( ( desc != NULL ) && ( SU_Is_Equal( desc, "\0", FALSE ) != TRUE ) )
  {
    tree_pic_forest->tree_pic->tree->post_it_note = strsave( desc );
  }
  else
  {
    tree_pic_forest->tree_pic->tree->post_it_note = NULL;
  }
  tree_pic_forest->tree_pic->tree->edited = TRUE;

} /* DrawingAreaPostItTextCb */



/*--------------------------------------------------------------
 Routine : DrawingAreaReadTextCb
 Purpose : get text (description for an intermediate event) from dialog,
           update event.
---------------------------------------------------------------*/
void DrawingAreaReadTextCb(
  TREE_PIC_FOREST *tree_pic_forest,
  char     *id,
  char     *desc )
{
    ITEM            *selected_item;
    
	selected_item = TreePicGetSelectedItem(tree_pic_forest->tree_pic);
    if (id != NULL) {
        strcpy(selected_item->id, id);
    } else {
        strcpy(selected_item->id, "");
    }
    item_new_description(selected_item, desc);
    tree_pic_forest->tree_pic->tree->edited = TRUE;
}



/*--------------------------------------------------------------
 Routine : DrawingAreaGateOkCb
 Purpose : get ID (for a gate) from the prompt dialog,
           update item.
---------------------------------------------------------------*/
void
  DrawingAreaGateOkCb(
    TREE_PIC_FOREST *tree_pic_forest,
    char *id,
	int  type
	)
{
    ITEM                *selected_item;

    selected_item = TreePicGetSelectedItem(tree_pic_forest->tree_pic);

    if ( ( type == INHIBIT ) && 
         ( ( selected_item->num_children > 2 ) ||
           ( ( selected_item->num_children == 2 ) &&
             ( !is_condition( selected_item->child ) ) ) ) )
    {
     
		GateDialogPostError(INHIBIT_RULES_EXCEEDED_ERROR, FTA_ERROR_TITLE );
	}
 
    else
    {

      if ( strcmp( selected_item->id, id ) != 0 ) 
      {
        tree_pic_forest->tree_pic->tree->edited = TRUE;
      }

      if ( id == NULL || id == "") 
      {
        strcpy(selected_item->id, "");
      } 
      else 
      {
        strcpy(selected_item->id, id);
      }

      if ( selected_item->type != type ) 
      {
        tree_pic_forest->tree_pic->tree->edited = TRUE;
      }

      selected_item->type = type;
    }

} /* DrawingAreaGateOkCb */


/*--------------------------------------------------------------
 Routine : DrawingAreaTxTreeOkCb
 Purpose : get ID (for a transfer tree) from the prompt dialog.
---------------------------------------------------------------*/
void
  DrawingAreaTxTreeOkCb(
    TREE_PIC_FOREST     *tree_pic_forest,
    char    *txTree )
{
    ITEM                *selected_item;
    char *str = NULL;

    selected_item = TreePicGetSelectedItem(tree_pic_forest->tree_pic);

    if ( txTree == NULL ) {
      strcpy(selected_item->id, "");
    } 
    else 
    {
      str = filename_from_pathname( txTree );
      strcpy(selected_item->id, str );
      strfree( str );
      /* add tree filename suffix */
      /* This is a fudge that will need to be corrected when the
         method of handling Transfer-In/Out trees is addressed.
         MPA 4/11/96 */
      strcat( selected_item->id, ".fta" );   
    }

    tree_pic_forest->tree_pic->tree->edited = TRUE;

} /* DrawingAreaTxTreeOkCb */



/*--------------------------------------------------------------
 Routine : edit_selected_item
 Purpose :
---------------------------------------------------------------*/

void edit_selected_item(TREE_PIC_FOREST *tree_pic_forest)
{
	ITEM            *selected_item;

	ASSERT( tree_pic_forest != NULL );

	selected_item = TreePicGetSelectedItem(tree_pic_forest->tree_pic);

	if ( selected_item ) {
		if ( is_gate( selected_item ) ) {
 
			/* display change gate id dialog */
			FTACanvasEditGate();

		} else if ( is_transfer( selected_item ) ) {
			
			/* display transfer widget */
			FTACanvasEditTransferTree();

		} else if ( is_primary( selected_item ) ) {

			printf("Primary event\n");

			if ( !ped_shell_exists( tree_pic_forest->tree_pic->tree->database ) ) {
				if ( !ped_shell_any_exist() ) {
 
					ped_shell_open( tree_pic_forest->tree_pic->tree->database );

					/* From the shell open for this database get the details for 
					   the id selected in the event list. This may be different
					   from the existing id for the selected item. 
					   Populate tree symbol with event description and id
					*/

					FTACanvasSelectPrimaryEvent(selected_item->id);

					/*ped_shell_select_primary_event(
						selected_item->id,
						tree_pic_forest->tree_pic->tree->database ); */
				} else {
					ped_shell_open(tree_pic_forest->tree_pic->tree->database );

					FTACanvasSelectPrimaryEvent(selected_item->id);
				 /* ped_shell_select_primary_event(
						selected_item->id,
						tree_pic_forest->tree_pic->tree->database ); */
				} /* no shells exist */

			} else {
				/* Shell exists for database */
				/* From the shell open for this database get the details for 
				   the id selected in the event list. This may be different
				   from the existing id for the selected item. 
				   Populate tree symbol with event description and id
				*/

				
			    FTACanvasSelectPrimaryEvent(selected_item->id);

				/* ped_shell_select_primary_event(
					selected_item->id,
					tree_pic_forest->tree_pic->tree->database ); */
			}
     
		} else { /* not a gate, a transfer nor a primary event */

			/* display intermediate event dialog */
			FTACanvasEditIntermediateEvent();

		} /* if selected item is a gate, ... */

	} /* if item selected */
	else {

		/* display tree info */
		FTACanvasEditTreeText();

	}

} /* edit_selected_item */
