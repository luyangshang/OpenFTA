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
 
 SccsId : @(#)edit_pd.h	1.4 11/12/96
 
 Origin : FTA project,
          FSC Limited Private Venture.
 
 This module implements the pulldown menu for the Edit option of
 the menu bar.
***************************************************************/

#ifndef edit_pd_h
#define edit_pd_h

#include "tree_pic.h"
#include "primary_event.h"
#include "tree_pic_forest.h"


/*--------------------------------------------------------------
 Routine : EditLinkAction
 Purpose : To link the selected item on the tree to the selected
 event in the database.
 
 This action can be taken as a result of the user performing a
 menu selection, or from a toolbar button.
---------------------------------------------------------------*/
extern void EditLinkAction(TREE_PIC *tree_pic, PRIMARY_EVENT *event);



/*--------------------------------------------------------------
 Routine : EditUndoAction
 Purpose : To undo the last edit.

 This action can be taken as a result of the user performing a
 menu selection.
---------------------------------------------------------------*/
extern void EditUndoAction(TREE_PIC_FOREST *tree_pic_forest);


/*--------------------------------------------------------------
 Routine : EditCutAction
 Purpose : To cut a selected portion of the tree to the copy
 buffer.

 This action can be taken as a result of the user performing a
 menu selection, or from a toolbar button.
---------------------------------------------------------------*/
extern void EditCutAction(TREE_PIC_FOREST *tree_pic_forest);


/*--------------------------------------------------------------
 Routine : EditCopyAction
 Purpose : To copy a selected portion of the tree to the copy
 buffer.

 This action can be taken as a result of the user performing a
 menu selection, or from a toolbar button.
---------------------------------------------------------------*/
extern void EditCopyAction(TREE_PIC_FOREST *tree_pic_forest);


/*--------------------------------------------------------------
 Routine : EditPasteAction
 Purpose : To paste the tree in the copy buffer to the selected
 item on the tree.

 This action can be taken as a result of the user performing a
 menu selection, or from a toolbar button.
---------------------------------------------------------------*/
extern void EditPasteAction(TREE_PIC_FOREST *tree_pic_forest);

/*--------------------------------------------------------------
 Routine : EditShiftLeftAction
 Purpose : To shift the selected item to the left.

 This action can be taken as a result of the user performing a
 menu selection.
---------------------------------------------------------------*/
extern void EditShiftLeftAction(TREE_PIC_FOREST *tree_pic_forest);



/*--------------------------------------------------------------
 Routine : EditShiftRightAction
 Purpose : To shift the selected item to the right.

 This action can be taken as a result of the user performing a
 menu selection.
---------------------------------------------------------------*/
extern void EditShiftRightAction(TREE_PIC_FOREST *tree_pic_forest);


/*--------------------------------------------------------------
 Routine : EditDeleteAction
 Purpose : To delete the selected item (and its children) fom the tree.

 This action can be taken as a result of the user performing a
 menu selection, or from a toolbar button.
---------------------------------------------------------------*/
extern void EditDeleteAction(TREE_PIC_FOREST *tree_pic_forest);


/*--------------------------------------------------------------
 Routine : EditSelectedAction
 Purpose : To edit the info associated with the selected tree
		   entity.

 This action can be taken as a result of the user performing a
 menu selection, or via double clicking on the tree.
---------------------------------------------------------------*/
extern void EditPasteAction(TREE_PIC_FOREST *tree_pic_forest);


/*--------------------------------------------------------------
 Routine : EditSelectedAction
 Purpose : To edit the info associated with the selected tree
		   entity.

 This action can be taken as a result of the user performing a
 menu selection, or via double clicking on the tree.
---------------------------------------------------------------*/
extern void EditSelectedAction(TREE_PIC_FOREST *tree_pic_forest);

#endif
