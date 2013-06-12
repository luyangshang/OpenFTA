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
 
 SccsId : @(#)drawingarea.h	1.1 08/22/96

 This module implements the Drawing Area widget using the
 Motif toolkit.

****************************************************************/

#ifndef drawingarea_h
#define drawingarea_h


#include "tree_pic.h"
#include "tree_pic_forest.h"

/*--------------------------------------------------------------
 Routine : DrawingAreaPostItTextCb
 Purpose : get text (description for tree) from dialog.
---------------------------------------------------------------*/
extern void 
  DrawingAreaPostItTextCb(
    TREE_PIC_FOREST*, 
    char*);

/*--------------------------------------------------------------
 Routine : DrawingAreaReadTextCb
 Purpose : get text (description for an intermediate event) from dialog,
           update event.
---------------------------------------------------------------*/
extern void
  DrawingAreaReadTextCb(
  TREE_PIC_FOREST*,
  char*,
  char*);

/*--------------------------------------------------------------
 Routine : DrawingAreaGateOkCb
 Purpose : get ID (for a gate) from the prompt dialog,
           update item.
---------------------------------------------------------------*/
extern void DrawingAreaGateOkCb(
    TREE_PIC_FOREST*,
    char*,
	int);


/*--------------------------------------------------------------
 Routine : DrawingAreaTxTreeOkCb
 Purpose : get ID (for a transfer tree) from the prompt dialog.
---------------------------------------------------------------*/
extern void
  DrawingAreaTxTreeOkCb(
    TREE_PIC_FOREST*,
    char*);


/*--------------------------------------------------------------
 Routine : edit_selected_item 
 Purpose : 
---------------------------------------------------------------*/
extern void
  edit_selected_item(
    TREE_PIC_FOREST *tree_pic_forest );

#endif
