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
 
 SccsId : @(#)file_pd.c	1.15 01/28/97
 
 Origin : FTA project,
          FSC Limited Private Venture.
 
 This module implements the pulldown menu for the File option of
 the menu bar.
***************************************************************/

#include <stdlib.h>
#include <string.h>

#include "file_pd.h" 
#include "treeUtil.h"
#include "tree_pic.h"

#include "display_tree.h"
#include "symbol_menu.h"
#include "fta.h"
#include "util.h"
#include "PED_shell.h"
#include "NativeFTAFrame.h"
#include "NativePEDFrame.h"
#include "Dialogs.h"
#include "FileUtilities.h"
 
#include "AssertUtil.h"

ASSERTFILE(__FILE__)

/* Define Query Texts */

#define NEW_TREE_QUESTION \
"WARNING: Tree not saved.\n\n\
The tree has changed since it was last saved.\n\
Creating a new tree will lose any changes that you made.\n\n\
Create new tree and lose any changes?"

#define LOAD_TREE_QUESTION \
"WARNING: Tree not saved.\n\n\
The tree has changed since it was last saved.\n\
Opening a saved tree will lose any changes that you made.\n\n\
Open saved tree and lose any changes?"

#define TREE_WARNING_QUESTION \
"WARNING: Tree not saved.\n\n\
The tree has changed since it was last saved.\n\
If you exit without saving, your changes will be lost.\n\n\
Exit and lose any changes?"

#define DB_WARNING_QUESTION \
"WARNING: Database not saved.\n\n\
The database that is open has changed since it was last saved.\n\
You must save the changes or close the database by creating a\n\
new database before you can exit the tree"

#define TREE_FORMAT_ERROR "ERROR: incorrect format in fault tree file"
#define FILE_OPEN_ERROR   "ERROR: error opening file"


/*
 * forward declarations
 */

/* static void  */
/*   create_new_tree_cb( */
/*     TREE_PIC_FOREST*); */



/*---------------------------------------------------------------
 Routine : file_load
 Purpose : Action to be taken on selecting the File->Load option.
---------------------------------------------------------------*/
void
  file_load(
    TREE_PIC_FOREST *tree_pic_forest,
	char *fname)
{
    if (tree_pic_forest->tree_pic->tree->edited) {
		if(FTAFrameAskQuestion(LOAD_TREE_QUESTION, FTA_OPEN_TITLE)) {
			load_tree_cb(tree_pic_forest, fname);
		}
    } else {
      load_tree_cb(tree_pic_forest, fname);
    }

} /* file_load */


/*---------------------------------------------------------------
 Routine : load_tree_cb
 Purpose : Callback to load new tree
---------------------------------------------------------------*/
int
  load_tree_cb(
    TREE_PIC_FOREST *tree_pic_forest,
    char *fname )
{
    TREE            *tree = tree_pic_forest->tree_pic->tree;
    int             load_err = 0;

	ASSERT( ( tree_pic_forest != NULL ) && ( tree != NULL ) && ( fname != NULL ) );

	/* load tree from file */
    load_err = load_tree(tree, fname);   
    TreePicSetSelectedItem(tree_pic_forest->tree_pic, NULL);  
  
    if ( load_err == TREE_FILE_ERROR) { 
		FTAFramePostWarning(FILE_OPEN_ERROR, FTA_OPEN_TITLE);  
		create_new_tree_cb(tree_pic_forest);
		update_tree(tree_pic_forest->tree_pic);
      return FILE_ERROR;  
    }  
  
    if ( load_err == TREE_ERROR) {
        FTAFramePostWarning(TREE_FORMAT_ERROR, FTA_OPEN_TITLE "Tree");  
        create_new_tree_cb(tree_pic_forest); 
        update_tree(tree_pic_forest->tree_pic);
        return FILE_ERROR;  
    }  

	/* set new file name in tree */
	if(tree->name) {
		strfree( tree->name );
	}
    if(tree->path) {
        strfree( tree->path );
	}
    tree_pic_forest->tree_pic->tree->name = filename_from_pathname(fname);
    tree_pic_forest->tree_pic->tree->path = path_from_pathname(fname);

	update_tree(tree_pic_forest->tree_pic);
    
	ASSERT( tree_pic_forest != NULL );
    
    if ( !ped_shell_exists( tree->database ) ) 
    {
      if ( !ped_shell_any_exist() ) 
      {
        ped_shell_create( ); 
		if(tree->database) {
			/* only load database if one is specified */
		    FTAFrameLoadPEDFile(tree->database);
		}
		/*         ped_shell_open( tree->database ); */
      } 
      else 
      {
        FTAFrameNewPEDFile();
		if(tree->database) {
			/* only load database if one is specified */
			FTAFrameLoadPEDFile(tree->database);
		}
/*         ped_shell_open( tree->database ); */

      } /* no shells exist */

    } /* no shell exists for database */

	ped_shell_add_modify_callback(tree->database,
								database_modified_callback,
								(void *)tree_pic_forest );

	FTAFrameSetWindowTitle( tree_pic_forest->tree_pic );

    update_prim_events_tree( tree_pic_forest->tree_pic->tree );
    make_all_levels(tree_pic_forest->tree_pic->tree);

	return FILE_OK;

} /* load_tree_cb */



/*---------------------------------------------------------------
 Routine : create_new_tree_cb
 Purpose : Callback function to create new tree.
 Space is allocated to hold the tree
---------------------------------------------------------------*/
void create_new_tree_cb(
     TREE_PIC_FOREST        *tree_pic_forest)
{
    /* Destroy old tree and plant a new one */
    replant_tree(tree_pic_forest->tree_pic->tree);

    TreePicSetSelectedItem(tree_pic_forest->tree_pic, NULL);
    TreePicSetSelectedItem(tree_pic_forest->global_tree_pic, NULL);

 /* Disassociate database */
    if(tree_pic_forest->tree_pic->tree->database) {
		strfree( tree_pic_forest->tree_pic->tree->database );
        tree_pic_forest->tree_pic->tree->database = NULL;
	}

	FTAFrameSetWindowTitle( tree_pic_forest->tree_pic );

} /* create_new_tree_cb */




/*---------------------------------------------------------------
 Routine : database_modified_callback
 Purpose: To perform all the actions required to the tree whenever the
 database changes.
 This change may be as a result to the event data, a change of database
 or an asscciation change.
---------------------------------------------------------------*/
void
  database_modified_callback(
    void *client_data )
{
	TREE_PIC_FOREST *tree_pic_forest = (TREE_PIC_FOREST *)client_data;

	update_subtree(
		tree_pic_forest->tree_pic->tree->top_item,
		tree_pic_forest->tree_pic->tree->database );

} /* database_modified_callback */


/*---------------------------------------------------------------
 Routine : file_new
 Purpose : Action to be taken on selecting the File->New option.
---------------------------------------------------------------*/
void
  file_new(
    TREE_PIC_FOREST *tree_pic_forest)
{
    if (tree_pic_forest->tree_pic->tree->edited) {
		if(FTAFrameAskQuestion(NEW_TREE_QUESTION, FTA_NEW_TITLE)) {
			create_new_tree_cb(tree_pic_forest);
		}
    } else {
      create_new_tree_cb(tree_pic_forest);
    }

} /* file_new */



/*---------------------------------------------------------------
 Routine : file_close
 Purpose : Action to be taken on selecting to exit the program.
---------------------------------------------------------------*/
int
  file_close(
    TREE_PIC_FOREST *tree_pic_forest)
{
    if (tree_pic_forest->tree_pic->tree->edited) {
		if(FTAFrameAskQuestion(TREE_WARNING_QUESTION, FTA_NEW_TITLE)) {
			return 0; /*0 => False (don't save)*/
		} else {
			return 1; /*1 => True (needs saving)*/
		}
    } else {
      return 0;
    }

} /* file_close */

