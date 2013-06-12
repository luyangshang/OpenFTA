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
 Module : file_pd
 Author : FSC Limited
 Date   : 13/8/96
 
 SccsId : @(#)file_pd.h	1.4 12/16/96
 
 Origin : FTA project,
          FSC Limited Private Venture.
 
 This module implements the pulldown menu for the File option of
 the menu bar.
***************************************************************/

#ifndef file_h
#define file_h

#include "tree_pic_forest.h"

#define FILE_OK 0
#define FILE_ERROR 1



/*---------------------------------------------------------------
 Routine : file_load
 Purpose : Action to be taken on selecting the File->Load option.
---------------------------------------------------------------*/
extern void
  file_load(
    TREE_PIC_FOREST *tree_pic_forest,
	char *fname);

/*---------------------------------------------------------------
 Routine : load_tree_cb
 Purpose : Callback to load new tree
---------------------------------------------------------------*/
extern int
  load_tree_cb(
    TREE_PIC_FOREST *tree_pic_forest,
    char *fname );

/*---------------------------------------------------------------
 Routine : create_new_tree_cb
 Purpose : Callback function to create new tree.
 Space is allocated to hold the tree
---------------------------------------------------------------*/
extern void create_new_tree_cb(
     TREE_PIC_FOREST        *tree_pic_forest);


/*---------------------------------------------------------------
 Routine : database_modified_callback
 Purpose: To perform all the actions required to the tree whenever the
 database changes.
 This change may be as a result to the event data, a change of database
 or an asscciation change.
---------------------------------------------------------------*/
extern void
  database_modified_callback(
    void *client_data );

/*---------------------------------------------------------------
 Routine : file_new_cb
 Purpose: To perform all the actions required to the tree whenever creating
 new tree.
---------------------------------------------------------------*/
extern void file_new(
    TREE_PIC_FOREST *tree_pic_forest);


/*---------------------------------------------------------------
 Routine : file_close_cb
 Purpose: To perform all the actions required to the tree whenever exiting the
 application
---------------------------------------------------------------*/
extern int file_close(
    TREE_PIC_FOREST *tree_pic_forest);


#endif
