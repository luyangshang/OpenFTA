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
 
 SccsId : @(#)tree.c	1.11 01/28/97
 
 Origin : FTA project,
          FSC Limited Private Venture, Level 2 Prototype.
 
 This module implements a tree. The tree contains details of the
 item and level structure of the tree.
 
 Trees and Items display a confusing degree of metamorphosis
 from one to the other. Items should exist as nodes within trees
 but here trees are made up of connected items. Hence item.h
 contains *tree routines that might be expected to be exported
 from here.
***************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "treeUtil.h"
#include "level.h"
#include "event_list.h"
#include "undo.h"
#include "display_tree.h"
#include "MathMacros.h"
#include "symbol_defs.h"
#include "util.h"
#include "CutSetsCommon.h"
#include "FileUtilities.h"
#include "fta.h"
#include "NativeFTAFrame.h"
#include "Dialogs.h"
#include "PED_shell.h"
#include "StringUtilities.h"
#include "FileUtilities.h"

#include "MemoryUtil.h"
#include "AssertUtil.h"

ASSERTFILE( __FILE__ )

/*---------------------------------------------------------------
 Define the String macros used within this module
---------------------------------------------------------------*/

#define GATE_TO_GATE_WARNING \
"Gate parent must not be another gate"
 
#define INTER_CHILD_WARNING \
"Intermediate event is allowed only one child"
 
#define CHILD_NOT_ALLOWED_WARNING \
"Child not allowed for this symbol type"

#define CONDITION_NOT_ALLOWED_WARNING \
"Condition can not be added to this symbol"

#define ONLY_ONE_CONDITION_WARNING \
"Gate may only have one condition"
 
#define CANT_CUT_TOP_ITEM_WARNING \
"Top item may not be deleted"
 
#define TRANSFER_OUT_WARNING \
"Transfer-out symbol may occur only at top of tree"

#define NO_ID_ERR \
"Item without id located"

#define GATE_NUM_CHILD_ERR \
"Gate must possess at least 2 children"

#define GATE_INHIBIT_ERR \
"Inhibit gate must have only a conditional event and a primary event"

#define INTER_NO_CHILD_ERR \
"Intermediate event must possess a child"

#define TRANSIN_NOT_EXISTS_ERR \
"Transfer-In tree does not exist"

#define PRIM_EVENT_NOT_IN_DB_WARNING \
"Primary event does not exist in the Database"

/*---------------------------------------------------------------
 Define Subtree constants
---------------------------------------------------------------*/
/*
 * do something about MAX_LEVEL - DGW
 Q: What? MPA 24/7/96
 A:
 */
/* #define MAX_LEVEL     50 */
static int MAX_LEVEL;

#define LARGE         10000
#define NORMAL_HEIGHT VERT_SPACING
#define MIXED_HEIGHT  (VERT_SPACING + 0.4)

/*---------------------------------------------------------------
 structure to return shape of a subtree
---------------------------------------------------------------*/
typedef struct shape_ {
    int    n;              /* number of levels       */
    float *ext_left;       /* extend left from root  */
    float *ext_right;      /* extent right from root */
} Shape;



/*---------------------------------------------------------------
 Define static variables global to this module
---------------------------------------------------------------*/
static ITEM *copy_buffer = NULL;
/* static float height[ MAX_LEVEL ]; */



/*---------------------------------------------------------------
 Define forward references to routines declared and used locally
 in this module
---------------------------------------------------------------*/


static void 
  print_validity_error(
    ITEM *item, 
    char *severity,
    char *error, 
    FILE *file);


static void 
  eval_rel_coords3(
    TREE *tree);


static void
  ShiftSubtree(
    ITEM *item, 
    float offset);


static Shape *
  ComputePositions(
    ITEM *item, 
    int level,
float *height );


static int
  save_tree(
    TREE_PIC *tree_pic, 
    char *filename);


static void
  save_dbname(
    FILE *fp, 
    char *dbname);


/*---------------------------------------------------------------
 Routine : tree_new
 Purpose : To reinitialise all tree data to a state consistent
 with a new tree
---------------------------------------------------------------*/
void
  tree_new(
    TREE *tree)
{
  ITEM *ip;

/* printf("tree: tree_new()\n"); */

  /* Initialise the tree to known values. These define a tree with
     no items, no cut sets and which is un-named and un-edited */
  tree->top_item = NULL;
  tree->last_item = 0;
  tree->process_top_item = NULL;
  tree->top_level = NULL;
  tree->mcs_expr = NULL;
  tree->mcs_end = NULL;
  tree->max_order = 0;
  tree->max_level = 0;
  tree->edited = FALSE;
  tree->expanded = FALSE;
  tree->mcs_tree_exists = FALSE;
  tree->mcs_exists = FALSE;
  tree->probs_exists = FALSE;
  tree->monte_exists = FALSE;
  tree->timed = FALSE;
  if(tree->name) {
    free(tree->name);
    tree->name = NULL;
  }
  if(tree->path) {
	free(tree->path);
    tree->path = NULL;
  }
  
  tree->post_it_note = NULL;
  tree->num_lambda_events = 0;

  /* Don't reset database, as on 'New' the database shall be be retained
     as a basis for the next tree */

  /* Start the new tree with an Intermediate event */
  ip = make_item(); 
  ip->type = INTERMEDIATE; 
/*        ip->level = 0;*/
  tree->top_item = ip; 

  /* Reset level information */
  make_all_levels(tree); 

} /* tree_new */




/*---------------------------------------------------------------
 Routine : make_all_levels
 Purpose : To make the level structure for the tree. The level
 structure is required for the automatic layout of the tree on
 the drawing area.
---------------------------------------------------------------*/
void
make_all_levels(TREE *tree)
{
    LEVEL *lp;
    LEVEL *last_lp = NULL; 
    int i;

    delete_level( tree->top_level );

    tree->max_level = set_tree_level(tree->top_item, 0);

    for (i = 0; i <= tree->max_level; i++) {
        lp = make_level(tree->top_item, i);
        if (i == 0) {
            tree->top_level = lp;
        } else {  
          if ( last_lp != NULL )
            last_lp->next = lp;  
          else
            printf("make_all_levels: last_lp used before set\n");
        }
        last_lp = lp;  

        join_level(tree->top_item, lp, i, NULL);
    }

} /* make_all_levels */



/*---------------------------------------------------------------
 Routine : delete_tree
 Purpose : To delete the tree from memory, and reset the tree data
 structures to the new tree state.
---------------------------------------------------------------*/
void
delete_tree(TREE *tree)
{
        unexpand_tree(tree);

        delete_subtree(tree->top_item); 
        tree->top_item = NULL;
        delete_level(tree->top_level);
        tree->top_level = NULL;

        /* delete top level tree info */
        if(tree->name) {
			FreeMemory(tree->name);
			tree->name = NULL;
		}
        if(tree->path) {
			FreeMemory(tree->path);
			tree->path = NULL;
		}
        if(tree->database) {
			FreeMemory(tree->database);
			tree->database = NULL;
		}
        if(tree->mcs_expr) {
			ExprDestroy(tree->mcs_expr);
			tree->mcs_expr = NULL;
		}
        if(tree->post_it_note) {
			FreeMemory(tree->post_it_note);
			tree->post_it_note = NULL;
		}

} /* delete_tree */



/*---------------------------------------------------------------
 Routine : replant_tree
 Purpose : To re-initialise an existing tree and all the structures
 related to a tree.

 delete old tree
 delete basic event list
 make new tree
 reset undo buffer
---------------------------------------------------------------*/
void
  replant_tree(
    TREE *tree )
{
  /* Delete the tree. This unexpands the tree, uses delete_subtree to
     recursively delete the tree, and deletes the levels. */
  delete_tree( tree );
  del_bas();

  /* Reinitialise the tree pointed to */
  tree_new( tree );

  /* Reset the Undo buffer. Once the tree has been deleted the Undo
     buffer makes no sense */
  record_edit( NULL, NONE, NULL, NULL, NULL );

} /* replant_tree */



/*---------------------------------------------------------------
 Routine : update_tree
 Purpose : To update the tree, calculating position of all items.
---------------------------------------------------------------*/
BOOL update_tree(TREE_PIC *tree_pic)
{
    eval_rel_coords3(tree_pic->tree);

    scale_tree(tree_pic, TreePicGetScale(tree_pic));

    if (tree_pic->scrolled_window) {
        if ( !resize_da(tree_pic) )
          return FALSE;
    }
	return TRUE;
}

/*---------------------------------------------------------------
 Routine : tree_draw
 Purpose : To draw the tree on the related canvas with the specified
 item at the top centre of the viewport.

 calculate positions of all items
 draw the tree with draw_tree()
---------------------------------------------------------------*/
BOOL
tree_draw( TREE_PIC *tree_pic,    /* tree to draw     */
           ITEM     *item )       /* item to focus on */
{

/* This was (item != NULL & tree_pic->scrolled_window != NULL)
   Should it have been changed ?? MPA 10/4/96 */
    if ( ( item != NULL ) && ( tree_pic->scrolled_window) ){
        TreePicFocus(tree_pic, item);
    }

    draw_tree(tree_pic, TreePicGetScale(tree_pic));

/*   } */
/*   else */
/*   { */
/*     dlg_post_warning( */
/*       tree_pic->widget, */
/*       "Warning: Tree Size exceeded.\n" */
/*         "Tree cannot be drawn. Consider splitting tree into\n" */
/*         "Transfer-In subtrees.", */
/*       FTA_APPN_TITLE " : Tree Level Exceeded" ); */
/*  */
/*   } */

return TRUE;
} /* tree_draw */

/*---------------------------------------------------------------
 Routine : resize_da
 Purpose : To resize the drawing area widge to fit the tree.
---------------------------------------------------------------*/
BOOL
resize_da( TREE_PIC *tree_pic)
{

    return TRUE;
} /* resize_da */




/*---------------------------------------------------------------
 Local routines
---------------------------------------------------------------*/

/*---------------------------------------------------------------
 Routine : print_validity_error
 Purpose : To print the tree validity error to the supplied file.
---------------------------------------------------------------*/
void
print_validity_error(ITEM *item, char *severity, char *error, FILE *file)
{
    fprintf(
      file, 
      "%-21s : ID> %-*s : %s\n", 
      severity, 
      MAX_SYMBOL_ID_LENGTH + 1, 
      item->id, 
      error);

} /* print_validity_error */


/*---------------------------------------------------------------
 Routine : ShiftSubtree
 Purpose : To recursively apply an offset to the x coordinate of
 an item and all it's children.

 The offset can be positive (right) or negative (shift left)
---------------------------------------------------------------*/
void
ShiftSubtree(ITEM *item, float offset)
{
    ITEM *child;

 /* shift this item */
    item->x += offset;

 /* recursively shift children */
    for (child = item->child; child != NULL; child = child->right_sib) {
        ShiftSubtree(child, offset);
    }
} /* ShiftSubtree */



/*---------------------------------------------------------------
 Routine : ComputePosition
 Purpose : To recursively calculate position of the supplied item 
 and its subtree in relative coordinate system

 FOR each child DO
   recursively do subtree
   place to right of previous subtrees, as close as possible
 END DO
 place this item
---------------------------------------------------------------*/
Shape *
ComputePositions(ITEM *item, int level,
float *height )
{
    ITEM   *first_child;
    ITEM   *child;
    SYMBOL *gate_symbol;
    Shape  *shape;     /* shape of this subtree    */
    Shape  *cshape;    /* shape of a child subtree */
    float   overlap, tmp;
    int i;
    float *extent;

 /* create a Shape */
    if ( !fNewMemory( (void *)&shape, sizeof(Shape) ) )
    {
      exit( 1 );
    }
    shape->n = 1;
    if ( 
      !fNewMemory( 
        (void *)&extent, 
        ( ( MAX_LEVEL - level ) * sizeof(float) ) ) )
    {
      exit( 1 );
    }
    shape->ext_left = extent;

    if ( 
      !fNewMemory( 
        (void *)&extent, 
        ( ( MAX_LEVEL - level ) * sizeof(float) ) ) )
    {
      exit( 1 );
    }
    shape->ext_right = extent;

    for (i = 0; i < MAX_LEVEL - level; i++) {
        shape->ext_left[i] = shape->ext_right[i] = 0.0;
    }

 /* set vertical position */
    item->y = height[level];

    first_child = item->child;

 /* skip any condition or transfer-out (deal with it at the end) */
    if ( first_child != NULL && ( first_child->type == COND_NOT_ANAL ||
                                  first_child->type == COND_ANAL ||
                                  first_child->type == TRANSOUT ) ) {

        first_child = first_child->right_sib;
    }

    if ( first_child == NULL ) {

     /* no children - set up for this item */
        item->x = 0;
        shape->n = 1;
        shape->ext_left[0]  = 0;
        shape->ext_right[0] = 0;

    } else {

     /* recursively place children
      * add child as close to previous children as it will go
      */
        for (child = first_child; child != NULL; child = child->right_sib) {
            cshape = ComputePositions(child, level+1, height);

         /* work out maximum overlap between the current tree (shape)
          * and child's tree (cshape) - include the required spacing
          */
            overlap = -LARGE;
            for (i = 0; (i < cshape->n) && (i < shape->n - 1); i++) {
                tmp = shape->ext_right[i+1] + HORIZ_SPACING
                      - cshape->ext_left[i];

             /* space subtrees a bit further */
                if (i > 0) {
                    tmp += HORIZ_SPACING/2.0;
                }

                overlap = MAX(overlap, tmp);
            }

         /* adjust positions */
            if (overlap > 0) {

             /* shift current subtree right */
                ShiftSubtree(child, overlap);

            } else if (overlap < 0) {

             /* shift previous subtrees right */
                ITEM *itm;
                for (itm = first_child; itm != child; itm = itm->right_sib) {
                    ShiftSubtree(itm, -overlap);
                }
            }

         /* update shape */
            for (i = 0; i < cshape->n; i++) {

             /* update left extent */
                if ( i < shape->n - 1) {
                    if (overlap < 0) {
                        shape->ext_left[i+1] = shape->ext_left[i+1] - overlap;
                    }
                } else {
                    shape->n++;
                    if (overlap <= 0) {
                        shape->ext_left[i+1] = cshape->ext_left[i];
                    } else {
                        shape->ext_left[i+1] = cshape->ext_left[i] + overlap;
                    }
                }

             /* update right extent */
                if (overlap > 0) {
                    shape->ext_right[i+1] = cshape->ext_right[i] + overlap;
                } else {
                    shape->ext_right[i+1] = cshape->ext_right[i];
                }
            }

            FreeMemory(cshape->ext_right);
            FreeMemory(cshape->ext_left);
            FreeMemory(cshape);
        }

     /* place this item above children */
        if ( is_gate(item->child) ) {

         /* if child is gate - place directly over the gate */
            item->x = item->child->x;

        } else {

         /* else - place this item at center of its children */
            item->x = ( shape->ext_left[1] + shape->ext_right[1] ) / 2.0;
        }

        shape->ext_left[0]  = item->x;
        shape->ext_right[0] = item->x;

    }
/* Fix Transfer out drawing problem on new tree. MPA 1/4/96 */
    /* deal with any transfer-out */
    if ( item->child != NULL && item->child->type == TRANSOUT ) {  
  
/* printf("Child is a transfer out\n");  */

	    item->child->x = item->x + TRANS_OUT_RIGHT;  
        item->child->y = item->y - TRANS_OUT_UP;  
        shape->ext_right[0] += TRANS_OUT_RIGHT;  
		
/* if first gate has a conditioning event attached, then we need to move the top event and transfer out
   across so that they are in line */

		if(first_child && is_gate(first_child) && first_child->child &&
			(first_child->child->type == COND_NOT_ANAL 
			 || first_child->child->type == COND_ANAL)) {

			 shape->ext_right[0] -= (COND_SPACING/2);

             item->child->x = item->child->x - (COND_SPACING/2);
			 item->x = item->x - (COND_SPACING/2);
		} 
    }  

 /* deal with gates */
    if (is_gate(item) && item->child != NULL) {

     /* deal with any condition */
        if ( item->child->type == COND_NOT_ANAL ||
             item->child->type == COND_ANAL ) {

            item->child->x = item->x + COND_SPACING;
            item->child->y = item->y;

         /* adjust for particular gate */
            switch (item->type) {
            case AND :
                gate_symbol = &and_symbol;
                break;
            case PRIORITY_AND :
                gate_symbol = &priority_and_symbol;
                break;
            case OR :
                gate_symbol = &or_symbol;
                break;
            case XOR :
                gate_symbol = &excl_or_symbol;
                break;
            case INHIBIT :
                gate_symbol = &inhibit_symbol;
                break;
            default:
                break;
            }
            item->child->y -= (condition_symbol.depth - gate_symbol->depth)/2.0;

            shape->ext_right[0] += COND_SPACING;
        }

    }

    return shape;

} /* ComputePositions */



/*---------------------------------------------------------------
 Routine : eval_rel_coords3
 Purpose : To evaluate the coordinates of all items in relative 
 coordinate system

 set array of heights of levels
 call ComputePositions() for top item
---------------------------------------------------------------*/
void
eval_rel_coords3(TREE *tree)
{
    Shape *shape;
    LEVEL *lp;
    ITEM  *ip;
    BOOL   gate;
    int i;
/* float height[ tree->max_level ]; */
float *height;

MAX_LEVEL = tree->max_level + 1;

fNewMemory( (void *)&height, ( MAX_LEVEL + 1 ) * sizeof( float ) );
 
 /* if there is a transfer-out symbol, leave extra space at the top */
    if (tree->top_item->child != NULL &&
        tree->top_item->child->type == TRANSOUT) {

/* printf("Tree has transfer out symbol\n"); */
        height[0] = TRANS_OUT_UP;
    } else {
        height[0] = 0.0;
    }

 /* use level structure to set heights
  * levels consisting of all events or all gates are NORMAL_HEIGHT
  * levels with mixed events and gates are MIXED_HEIGHT
  */
    for (i=1, lp = tree->top_level; lp != NULL; lp = lp->next, i++) {

     /* set this level NORMAL_HEIGHT */
        height[i] = height[i-1] + NORMAL_HEIGHT;

     /* record type of first item in level */
        gate = is_gate(lp->iptr) ? TRUE : FALSE;

     /* look for a different type - if found, set this level MIXED_HEIGHT,
      * but a condition means the PREVIOUS level must be spaced further
      */
        for (ip = lp->iptr; ip != NULL; ip = ip->lptr) {
            if (ip->type == COND_NOT_ANAL || ip->type == COND_ANAL) {
                if (height[i-1] - height[i-2] < MIXED_HEIGHT) {
                    height[i-1] += MIXED_HEIGHT - NORMAL_HEIGHT;
                    height[i]   += MIXED_HEIGHT - NORMAL_HEIGHT;
                }
            } else {
                if ( (gate && !is_gate(ip)) || (!gate && is_gate(ip) ) ){
                    height[i] = height[i-1] + MIXED_HEIGHT;
                 /* break; */
                }
            }
        }
    }

 /* recursively compute positions, starting with top item */
    shape = ComputePositions(tree->top_item, 0, height );

 /* set max and min values */
    tree->min_x = LARGE;
    tree->max_x = 0;

    for (i = 0; i < shape->n; i++) {
        tree->min_x = MIN(tree->min_x, shape->ext_left[i]);
        tree->max_x = MAX(tree->max_x, shape->ext_right[i]);
    }
    tree->max_x += HORIZ_SPACING;
    tree->max_y = height[shape->n] + 0.4;

 /* free stuff */
    FreeMemory(shape->ext_right);
    FreeMemory(shape->ext_left);
    FreeMemory(shape);
    FreeMemory(height);

} /* eval_rel_coords3 */




/*---------------------------------------------------------------
 Routine : tree_add
 Purpose : Adds the addition tree below the to_item

 Replaces duplicate code from symbol_menu_cb and edit_menu_cb
---------------------------------------------------------------*/
BOOL
  tree_add(
    ITEM *addition,
    ITEM *to_item,
    TREE_PIC *tree_pic )
{
  if ( addition != NULL ) {

    if ( ( addition->type == COND_NOT_ANAL ) ||
         ( addition->type == COND_ANAL ) )
    {
      if ( !is_gate(to_item) ||
           ( ( to_item->type == INHIBIT ) && 
             ( to_item->num_children >= 2 ) ||
             ( ( to_item->num_children == 1 ) &&
               ( is_condition( to_item->child ) ) ) ) )
      {
			FTAFramePostWarning(CONDITION_NOT_ALLOWED_WARNING, FTA_ERROR_TITLE);
			return FALSE;
      } else {
        if (to_item->child != NULL &&
            ( to_item->child->type == COND_NOT_ANAL ||
              to_item->child->type == COND_ANAL ) ) {
			FTAFramePostWarning(ONLY_ONE_CONDITION_WARNING, FTA_ERROR_TITLE);
			return FALSE;
        }
      }
    }
  }
 
  if (!is_gate(to_item) &&
      to_item->type != INTERMEDIATE) {
		FTAFramePostWarning(CHILD_NOT_ALLOWED_WARNING, FTA_ERROR_TITLE);
		return FALSE;
  }
 
  if (is_gate(to_item)) {
    if (is_gate(addition)) {
		FTAFramePostWarning(GATE_TO_GATE_WARNING, FTA_ERROR_TITLE);
		return FALSE;
    }
    if (addition->type == TRANSOUT) {
		FTAFramePostWarning(TRANSFER_OUT_WARNING, FTA_ERROR_TITLE);
		return FALSE;
    }
  }
 
  if (to_item->type == INTERMEDIATE) {
    if (addition->type == TRANSOUT) {
      if (to_item !=
        tree_pic->tree->top_item) {
			FTAFramePostWarning(TRANSFER_OUT_WARNING, FTA_ERROR_TITLE);
			return FALSE;
      } else {
        if (to_item->child != NULL &&
          to_item->child->type == TRANSOUT) {
          /* no warning necessary - ignore request */
          return FALSE;
        }
      }
    } else {
 
      if ( ( to_item->num_children == 1 &&
        to_item->child->type != TRANSOUT) ||
        to_item->num_children == 2 ) {
			FTAFramePostWarning(INTER_CHILD_WARNING, FTA_ERROR_TITLE);
			return FALSE;
      }
    }
  } 
  add_child(tree_pic->tree,
    to_item,
    addition);
 
  update_tree(tree_pic);

  record_edit(tree_pic->tree,
    ADD,
    addition,
    NULL,
    NULL);


  return TRUE;

} /* tree_add */




	/*---------------------------------------------------------------
 Routine : add_child
 Purpose : Add an item to the tree as rightmost child of parent.
 
 The exceptions to the rule of adding as the rightmost child of a
 parent are Conditions and Transfer-Out items. These are added as
 leftmost child of the parent.
 
 The routine presumes that both parent and child exist.
---------------------------------------------------------------*/
void
  add_child(TREE *tree, ITEM *parent, ITEM *child)
{
  ITEM *tmp;

  child->parent = parent;
  if ( parent == NULL ) {
    child->level = 0;
  } else {
    child->level = parent->level+1;
    parent->num_children++;

    if (child->type == COND_NOT_ANAL ||
        child->type == COND_ANAL ||
        child->type == TRANSOUT) {

        child->level = parent->level;

     /* add as left-most child */
        tmp = parent->child;
        parent->child = child;
        child->right_sib = tmp;
        child->left_sib = NULL;
        if (tmp != NULL) {
            tmp->left_sib = child;
        }
    
    } else {
    
     /* add as right-most child */
        child->right_sib = NULL;
        tmp = parent->child;
        if (tmp == NULL) {
            parent->child = child;
            child->left_sib = NULL;
        } else {
            while (tmp->right_sib != NULL) {
                tmp = tmp->right_sib;
            }
            tmp->right_sib = child;
            child->left_sib = tmp;
        }
    }
  }
 /* remake level structure */
    make_all_levels(tree); 
    tree_edited(tree); 
} /* add_child */



/*---------------------------------------------------------------
 Routine : tree_edited
 Purpose : To set the flags to indicated that the tree has been
 edited
---------------------------------------------------------------*/
void
tree_edited(TREE *tree)
{
    tree->edited = TRUE;
    tree->expanded = FALSE;
    tree->mcs_tree_exists = TRUE;
    tree->mcs_exists = FALSE;
    tree->probs_exists = FALSE;
    tree->monte_exists = FALSE;
    tree->timed = FALSE;

} /* tree_edited */



/*---------------------------------------------------------------
 Routine : load_tree
 Purpose : To load the tree data from the tree file
 
 TREE_FILE_ERROR or TREE_ERROR is returned if tree cannot be loaded
---------------------------------------------------------------*/
int
load_tree(TREE *tree, char *filename)
{
  FILE  *fp;
  char  *dbname;
  char  *absolute_dbname;

 /* open file (if error, return) */
  if ((fp = fopen(filename, "r")) == NULL) {
    return TREE_FILE_ERROR;           /* error opening file */
    }

  /* Re-initialise tree, level structure and undo buffer */
  replant_tree( tree );

 /* Read name of database. Loaded at client of this routine. */
  dbname = load_dbname(fp);

  /* if database exists then create full path to it */
  if(dbname) {
    absolute_dbname = path_from_pathname(filename);
    if(!absolute_dbname) {
      return TREE_FILE_ERROR;    
    } 
      
/*	if(absolute_dbname = realloc(absolute_dbname, strlen(absolute_dbname) + strlen(dbname) + 1)) { */

	if(fResizeMemory(&absolute_dbname, strlen(absolute_dbname) + strlen(dbname) + 1)) {
      strcat(absolute_dbname, dbname); 
    } else {
      return TREE_FILE_ERROR;    
    } 

    strfree( dbname );
  
  } else {
    absolute_dbname = NULL;
  } 

  /* strsave returns '\0' if dbname is NULL */
  if(tree->database) {
	  strfree( tree->database );
  }

  tree->database = absolute_dbname;
 

  if ( tree->post_it_note != NULL ) { 
    FreeMemory( tree->post_it_note ); 
  } 
  tree->post_it_note = load_post_it_note( fp );

 /* read tree */

  delete_subtree( tree->top_item );
/*  FreeMemory( tree->top_item ); */
  tree->top_item = load_item(fp, NULL, dbname);

 /* close file and check for error */
  fclose(fp);
  if (!tree->top_item) {
    printf("tree: ERROR READING FILE\n");
      replant_tree( tree );
    return TREE_ERROR;           /* tree format error */
  }

 /* Make the level structure for the tree */
  make_all_levels(tree); 

  /* Set up tree indicators to show if valid reports exists for
     mcs, probs and monte-carlo */
  {
    char  *mcs_file;
    int    num_mcs;
    int    order;
    time_t date;
    Expr   e;
    char  *report_filename;

    file_get_mcs(
      filename, /* tree->name has not yet been set */
      &mcs_file,
      &num_mcs, &order, &date, &e);

    if ( mcs_file != NULL ) {
      tree->mcs_exists = TRUE;
      tree->mcs_expr = e;
      tree->max_order = order;
      strfree(mcs_file);
    } /* if mcs file exists */

    report_filename =
      (char *)generate_filename(
        filename,
        PROBS_REPORT_SUFFIX );

    date = date_of( report_filename );

    if ( date != 0 ) {
      if ( date > date_of( filename ) ) {
        tree->probs_exists = TRUE;
      }
    }
    strfree( report_filename );

    report_filename =
      (char *)generate_filename(
        filename,
        MONTE_CARLO_REPORT_SUFFIX );

    date = date_of( report_filename );

    if ( date != 0 ) {
      if ( date > date_of( filename ) ) {
        tree->monte_exists = TRUE;
      }
    }
    strfree( report_filename );

  }

  return TREE_OK;

} /* load_tree */



/*---------------------------------------------------------------
 Routine : load_dbname
 Purpose : To load the name of the associated database from tree
 file (first line in file). The database name is defined in the
 file as an absolute pathname.
---------------------------------------------------------------*/
char *
load_dbname( FILE *fp )
{
	char temp_dbname[256];
	char *end_string;
    char *dbname;

	//fNewMemory( (void *)&dbname, 100*sizeof(char) );
	/* dbname = (char*)malloc(100*sizeof(char)); */


	if (!fgets(temp_dbname, 256, fp)) {
    /* if (fscanf(fp, "%s\n", dbname) != 1) { */
        printf("tree: ERROR READING FILE\n");
        return NULL_DB_NAME;
    }

	/* remove the \n if it exists */
	if(end_string = strchr(temp_dbname, '\n')) {
		*end_string = '\0';
	}

	/* create dbname and copy temp_dbname */
    if (strcmp(temp_dbname, "NULL") == 0) {
        return NULL;
    } else {
		dbname = SU_Copy(temp_dbname);
        return dbname;
    }

} /* load_dbname */




/*---------------------------------------------------------------
 Routine : load_post_it_note
 Purpose : To load the descriptive text for the tree from the
 tree file.
---------------------------------------------------------------*/
char *
  load_post_it_note(
    FILE *fp )
{

  char *post_it_note = NULL;
  int i; 
  int length;
  int children;
  char c;
  char ignored[10];

  if (fscanf(fp, "%c %s %d\n", &c, &ignored, &children) != EOF ) {

    if ( fscanf(fp, "%d", &length) != EOF ) {
      if ( !fNewMemory( (void *)&post_it_note, ( length + 1 ) ) ) 
      {
        printf("\n*** load_post_it_note : malloc failed ***\n");
        exit(1);
      }
      post_it_note[ 0 ] = '\0';

      if ( length ) {
        if (fgetc(fp) != ' ') {
          printf("ERROR READING FIELD SEPARATOR FROM FILE\n");
          return NULL;
        }
        if ( fscanf( fp, "%c", &c ) != EOF ) {
          i = 0;
          do {
            post_it_note[ i ] = c;
            i++;
          } while ( ( i < length ) &&
                    ( fscanf( fp, "%c", &c ) != EOF ) );
          post_it_note[ i ] = '\0';
        }
      }
    }
  } 
  
  return post_it_note;

} /* load_post_it_note */



/*---------------------------------------------------------------
 Routine : update_prim_events_tree
 Purpose : To update primary events in the tree with data from
 database
---------------------------------------------------------------*/
void
update_prim_events_tree(TREE *tree)
{
    update_subtree(tree->top_item, tree->database );

} /* update_prim_events_tree */




/*---------------------------------------------------------------
 Routine : delete_child
 Purpose : To delete item (and its subtree) from tree
---------------------------------------------------------------*/
void
delete_child(TREE *tree, ITEM *ip)
{
    ITEM  *parent;

    if (ip == NULL) return;

  ASSERT( fValidPointer( ip, sizeof( ITEM ) ) );

    if (ip == tree->top_item) {
FreeMemory( tree->top_item );
            tree->top_item = NULL;
    }

 /* update parent-child links */
    parent = ip->parent;
    if (parent != NULL) {
            parent->num_children--;
            if (parent->child == ip) {
                parent->child = ip->right_sib;
            if (ip->right_sib != NULL) {
                ip->right_sib->left_sib = NULL;
            }
        } else {
                ip->left_sib->right_sib = ip->right_sib;
            if (ip->right_sib != NULL) {
                ip->right_sib->left_sib = ip->left_sib;
            }
        }
    }

 /* delete the item */
    delete_subtree(ip);

 /* delete level structure */
    delete_level(tree->top_level);
    tree->top_level = NULL;

 /* remake level structure */
    make_all_levels(tree);

    tree->last_item = 0;
    tree_edited(tree);

} /* delete_child */



/*---------------------------------------------------------------
 Routine : insert_child
 Purpose : To insert the item to the tree as specified child of
 parent
---------------------------------------------------------------*/
void
insert_child(TREE *tree, ITEM *parent, ITEM *child, ITEM *right_sib)
{
    ITEM *tmp;

    child->parent = parent;
    parent->num_children++;
    if (child->type == COND_NOT_ANAL ||
        child->type == COND_ANAL ||
        child->type == TRANSOUT ) {

        child->level = parent->level;
    } else {
        child->level = parent->level+1;
    }

 /* add before right_sib */
    tmp = parent->child;
    if (tmp == NULL) {
        parent->child = child;
        child->right_sib = NULL;
        child->left_sib = NULL;
    } else if (tmp == right_sib) {
        parent->child = child;
        child->right_sib = right_sib;
        right_sib->left_sib = child;
        child->left_sib = NULL;
    } else {
        while (tmp->right_sib != right_sib) {
            tmp = tmp->right_sib;
        }
        tmp->right_sib = child;
        child->left_sib = tmp;
        child->right_sib = right_sib;
        if (right_sib != NULL) {
            right_sib->left_sib = child;
        }
    }

 /* remake level structure */
    make_all_levels(tree);
    tree_edited(tree);

} /* insert_child */



/*---------------------------------------------------------------
 Routine : get_paste_item
 Purpose : To return the copy of item (and its whole subtree)
 from the copy buffer
---------------------------------------------------------------*/
ITEM *
get_paste_item(TREE *tree)
{
    ITEM *item = copy_subtree(copy_buffer);
    return item;

} /* get_paste_item */



/*---------------------------------------------------------------
 Routine : copy_to_buffer
 Purpose : To put a copy of item (and it's whole subtree) in the
 copy buffer
---------------------------------------------------------------*/
void
copy_to_buffer(TREE *tree, ITEM *item)
{
    ITEM *tmp;

    if (copy_buffer != NULL) {
        delete_subtree(copy_buffer);
    }
    copy_buffer = copy_subtree(item);

 /* if copied tree contains a transfer-out - delete it */
    if ( copy_buffer != NULL &&
         copy_buffer->child != NULL &&
         copy_buffer->child->type == TRANSOUT ) {
        copy_buffer->num_children--;
        tmp = copy_buffer->child;
        copy_buffer->child = copy_buffer->child->right_sib;
        copy_buffer->child->left_sib = NULL;
        free(tmp);
    }

	/* the tree isn't editted if a section of it is copied */
    /* tree_edited(tree);*/

} /* copy_to_buffer */



/*---------------------------------------------------------------
 Routine : shift_item_left
 Purpose : To swap item with its left-hand sibling
---------------------------------------------------------------*/
void
shift_item_left(TREE *tree, ITEM *item)
{
    ITEM *parent = item->parent, *tmp;

    if (parent->child == item) {
        return;
    } else if (parent->child->right_sib == item) {
        parent->child->right_sib = item->right_sib;
        if (item->right_sib != NULL) {
            item->right_sib->left_sib = parent->child;
        }
        item->right_sib = parent->child;
        parent->child->left_sib = item;
        parent->child = item;
        item->left_sib = NULL;
    } else {
        for (tmp = parent->child; tmp->right_sib->right_sib != item;
             tmp = tmp->right_sib)
            ;
        tmp->right_sib->right_sib = item->right_sib;
        if (item->right_sib != NULL) {
            item->right_sib->left_sib = tmp->right_sib;
        }
        item->right_sib = tmp->right_sib;
        tmp->right_sib->left_sib = item;
        tmp->right_sib = item;
        item->left_sib = tmp;
    }

 /* remake the level structure for the tree */
    make_all_levels(tree);
    tree_edited(tree);

} /* shift_item_left */



/*---------------------------------------------------------------
 Routine : shift_item_right
 Purpose : To swap item with its right-hand sibling
---------------------------------------------------------------*/
void
shift_item_right(TREE *tree, ITEM *item)
{
    if (item->right_sib != NULL) {
        shift_item_left(tree, item->right_sib);
    }

} /* shift_item_right */



/*---------------------------------------------------------------
 Routine : validate_tree
 Purpose : To check tree for validity (for analysis).
 
 The routine returns an indication of whether the tree is valid or
 invalid.
---------------------------------------------------------------*/
BOOL
  validate_tree(
    TREE *tree, 
    ITEM *ip, 
    FILE *report_file)
{
  TREE expnd_tree;
  FILE *fp;
  ITEM *child;
  BOOL error = FALSE;
  char *ignored;
  char *transin_file;
  char *transin_ped;

  /* if item has no id then */
  if ( !strlen(ip->id) ) {
    /* if item is a primary or a transfer in then */
    if ( is_primary(ip) || ip->type == TRANSIN ) {
      error = TRUE;
      print_validity_error(ip, FTA_ERROR_TITLE, NO_ID_ERR, report_file);
    } 
    else {
      print_validity_error(ip, FTA_WARNING_TITLE, NO_ID_ERR, report_file);
    }
  } /* if no id */

  if ( is_gate(ip) ) 
  {
    if ( ip->type != INHIBIT )
    {
	if ( ip->num_children < 2 )
	    {
		error = TRUE;
		print_validity_error(
				     ip, 
				     FTA_ERROR_TITLE, 
				     GATE_NUM_CHILD_ERR, 
				     report_file);
	    } /* if gate with < 2 children */
    } 
    else
	{
	    if ( ( ip->num_children != 2 ) || 
		 ( ( ip->num_children == 2 ) &&
		   !( is_condition( ip->child ) ||
		      is_condition( ip->child->right_sib )
		      ) 
		   )
		 )
	      
		{
		    error = TRUE;
		    print_validity_error(
					 ip, 
					 FTA_ERROR_TITLE, 
					 GATE_INHIBIT_ERR, 
					 report_file);
		}
	}

    child = ip->child;
    while (child != NULL) {
      error = !validate_tree(tree, child, report_file) || error;
      child = child->right_sib;        
    }

  } else if ( ip->type == INTERMEDIATE ) {
    if ( !ip->num_children ) {
      error = TRUE;
      print_validity_error(
        ip, 
        FTA_ERROR_TITLE, 
        INTER_NO_CHILD_ERR, 
        report_file);
    } else {
      if ( ip->child->type != TRANSOUT )
        error = !validate_tree(tree, ip->child, report_file) || error;
      else
        error = 
          !validate_tree(tree, ip->child->right_sib, report_file) || error;
    }
  } else if ( ip->type == TRANSIN ) {

    expnd_tree.top_item = NULL;
    expnd_tree.process_top_item = NULL;
    expnd_tree.top_level = NULL;
	expnd_tree.name = NULL;
//	expnd_tree.path = NULL;
  if(tree->path) {
	  expnd_tree.path = SU_Copy(tree->path);
  } else {
    expnd_tree.path = NULL;
  }
  expnd_tree.database = NULL;
    expnd_tree.mcs_expr = NULL;
    expnd_tree.post_it_note = NULL;

	transin_file = SU_Join(tree->path, ip->id);

//    if ((fp = fopen(ip->id, "r")) == NULL  ){
    if ((fp = fopen(transin_file, "r")) == NULL  ){
	  error = TRUE;
      print_validity_error(
        ip, FTA_ERROR_TITLE,TRANSIN_NOT_EXISTS_ERR, report_file);
    } else {
      /* discard database name */ 
      char *dbname = load_dbname(fp);  

      ignored = load_post_it_note( fp );
      FreeMemory( ignored );

      expnd_tree.top_item = load_item(fp, NULL, dbname ); 
      expnd_tree.database = dbname;
      fclose(fp);

	  /* create space for and load ped that goes with Transfer-In tree */
      if(dbname) {
	      transin_ped = SU_Join(tree->path, dbname);

        if (!ped_shell_exists(transin_ped)) {
        	  ped_shell_create();
            FTAFrameLoadPEDFile(transin_ped);
        }
	      strfree(transin_ped);
      }

      error = 
        !validate_tree(
          &expnd_tree, 
          expnd_tree.top_item,
          report_file) || error;
      if(dbname) {

         /* no not delete database as it might be required by an expanded tree */
         /* ped_shell_delete_expanded(); */
      }
      delete_tree(&expnd_tree);
    }

	strfree(transin_file);
	
  } else if ( is_primary(ip) ) {
    if ( !ped_shell_primary_event_exists( ip->id, tree->database ) ) {

/* If the item is not found in the database, is it an error or a
   warning. If it is an error the tree will be invalidated, even though
   the cut sets can be generated successfully if this is the only
   type of error.
   It would be better to return an error enumeration rather than just
   TRUE/FALSE.
   MPA 4/6/96
*/
      error = TRUE; 
      print_validity_error(
        ip, FTA_ERROR_TITLE, PRIM_EVENT_NOT_IN_DB_WARNING, report_file);
    }
  }
  return !error;

} /* validate_tree */



/*---------------------------------------------------------------
 Routine : save_tree_to_file
 Purpose : To save the tree data to the specified tree file
 
 TREE_FILE_ERROR or TREE_ERROR is returned if tree cannot be saved.

 Deletes the obsolesced report files relating to the tree.

 This routine is used by the general purpose file selection
 dialog stream to save the tree to a file.
---------------------------------------------------------------*/
int
  save_tree_to_file(
    void *tree_data,
    char *filename)
{
  TREE_PIC *tree_pic = (TREE_PIC *)tree_data;
  char *report_filename;

  if ( save_tree( tree_pic, filename ) == TREE_OK ) {

    /* delete old name and path */
	if(tree_pic->tree->name) {
      strfree(tree_pic->tree->name);
	}
    /* delete old name and path */
	if(tree_pic->tree->path) {
      strfree(tree_pic->tree->path);
	}	  

    tree_pic->tree->name = filename_from_pathname(filename);
    tree_pic->tree->path = path_from_pathname(filename);
    tree_pic->tree->edited = FALSE;
    tree_pic->tree->expanded = FALSE;
    tree_pic->tree->mcs_exists = FALSE;
    tree_pic->tree->probs_exists = FALSE;
    tree_pic->tree->monte_exists = FALSE;
 
    /* Delete any report files etc as they are now obsolete */
    report_filename =
      (char *)generate_filename(
        tree_pic->tree->name,
        VALIDITY_REPORT_SUFFIX );

    remove( report_filename );
    strfree( report_filename );
    
    report_filename =
      (char *)generate_filename(
        tree_pic->tree->name,
        MCS_REPORT_SUFFIX );

    remove( report_filename );
    strfree( report_filename );
    
    report_filename =
      (char *)generate_filename(
        tree_pic->tree->name,
        MCS_SUFFIX );

    remove( report_filename );
    strfree( report_filename );
    
    report_filename =
      (char *)generate_filename(
        tree_pic->tree->name,
        MONTE_CARLO_REPORT_SUFFIX );

    remove( report_filename );
    strfree( report_filename );
    
    report_filename =
      (char *)generate_filename(
        tree_pic->tree->name,
        PROBS_REPORT_SUFFIX );

    remove( report_filename );
    strfree( report_filename );
 
    return TREE_OK;
 
  } else {
    return TREE_FILE_ERROR;
  }

} /* save_tree_to_file */



/*---------------------------------------------------------------
 Routine : save_tree
 Purpose : To save a tree to a supplied file. This is the routine
 that actually knows the structure of a tree and how it is saved
 to a file.
---------------------------------------------------------------*/
int
save_tree(TREE_PIC *tree_pic, char *filename)
{
  FILE *fp;
  int num_chars;
  char *file_path;
  char *end_file_path;
  char *end_db_path;
  char *rel_database;
  int i;
  int path_len;;

  if ((fp = fopen(filename, "w")) == NULL) return TREE_FILE_ERROR; 
  
  if(tree_pic->tree->database) {
    file_path = path_from_pathname(filename);
    num_chars = SU_Num_Same_Chars(tree_pic->tree->database, file_path);
    end_db_path = tree_pic->tree->database + num_chars;
    end_file_path = file_path + num_chars;
    num_chars = num_folders_in_pathname(end_file_path);
  
    path_len = num_chars*3 + strlen(end_db_path) + 1;

    fNewMemory( (void *)&rel_database, path_len ); 

	/* rel_database = (char*)malloc(path_len); */
    if(rel_database) {
        for(i=0; i<num_chars; i++) {
          rel_database[i] = '.';
          rel_database[i+1] = '.';
          rel_database[i+2] = file_separator();
        }

        for(i=num_chars*3; i<path_len; i++) {
          rel_database[i] = end_db_path[i-(num_chars*3)];
        }

    } else {
      return TREE_FILE_ERROR;
    }
  
  } else {
    rel_database = NULL;
  }

  /* Save associated database name to tree file */
  save_dbname(fp, rel_database); 

  if(rel_database) {
	  strfree(rel_database);
  }

  if ( tree_pic->tree->post_it_note != NULL ) {
    fprintf(fp, "S NULL 0\n");
    if ( (int)strlen( tree_pic->tree->post_it_note ) > 0 ) {
      fprintf(
        fp, 
        "%d %s", 
        strlen( tree_pic->tree->post_it_note ),
        tree_pic->tree->post_it_note );
    }
    else {
      fprintf(
        fp,
        "%d", 0 );
    }
    fputc('\n', fp);
  }
  else {
    fprintf(fp, "S NULL 0\n0\n");
  }
  /* Save all the items of the tree */
  save_item(fp, tree_pic->tree->top_item);
  fclose(fp);
  return TREE_OK;

} /* save_tree */        


/*---------------------------------------------------------------
 Routine : save_dbname
 Purpose : To save the database pathname found in tree to the file supplied.

 If no name exists, writes "NULL" to file.
---------------------------------------------------------------*/
void
save_dbname(FILE *fp, char* dbname)
{

    if (dbname == NULL) {
        fprintf(fp, "NULL\n");
    } else {
        fprintf(fp, "%s\n", dbname);
    }
} /* save_dbname */


/*---------------------------------------------------------------
 Routine : count_lambda_events
 Purpose : To count the events identified in the tree that have
 been declared with probabilities that are calculated per unit
 time. e.g., Chance of failure Once per Blue Moon
 
 If any lambda events are found, the tree is updated with the
 count, and the Quantative and Monte Carlo Analyses will prompt
 for the number of unit times to be used in their calculations.
---------------------------------------------------------------*/
void
  count_lambda_events(
    TREE *tree,
    ITEM *ip )
{
  ITEM *child;
  PRIMARY_EVENT *component;
/*   int i; */
 
  if (ip == NULL) return;
  child = ip->child;
  while (child != NULL){
    if ( is_primary( child ) ) {
      if ( child->id != NULL ) {
        component = 
          (PRIMARY_EVENT *)ped_shell_get_primary_event( 
            child->id, 
            tree->database );
        if ( component != NULL ) {
          if ( component->lambda != '\0' ) 
            tree->num_lambda_events++; 
        }
      }
    }
    count_lambda_events(tree, child);
    child = child->right_sib;
  }
 
} /* count_lambda_events */


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
BOOL
eval_tree(ITEM *ip)
{
    ITEM      *process_child;

    if (ip == NULL) {
        printf("\n*** tree: eval_tree : NULL item pointer ***\n");
        exit(1);
    }

    switch(ip->type){

    case BASIC:
    case UNDEVELOP:
    case EXTERNAL:
    case COND_ANAL:
            return ip->event->val;

    case COND_NOT_ANAL:
            return TRUE;

    case OR:
    case XOR:
            process_child = ip->process_child;

         /* if first child is an analysed condition,
          * AND it with other children
          */
            if (process_child->type == COND_ANAL) {
                if (!eval_tree(process_child)) return FALSE;
                process_child = process_child->process_sibling;
            }

         /* if first child is a non-analysed condition,
          * ignore it.
          */
            if (process_child->type == COND_NOT_ANAL) {
                process_child = process_child->process_sibling;
            }

         /* do OR of other children */
            while (process_child != NULL){
                if (eval_tree(process_child)) return TRUE;
                process_child = process_child->process_sibling;
            }
            return FALSE;

    case AND:
    case INHIBIT:
    case PRIORITY_AND:
            process_child = ip->process_child;

         /* if first child is a non-analysed condition,
          * ignore it.
          */
            if (process_child->type == COND_NOT_ANAL) {
                process_child = process_child->process_sibling;
            }

         /* do AND of other children */
            while (process_child != NULL){
                if (!eval_tree(process_child)) return FALSE;
                process_child = process_child->process_sibling;
            }
            return TRUE;

    case TRANSIN:
    case TRANSOUT:
            return eval_tree(ip->process_child);

    default:
            printf("\n*** tree: eval_tree : Invalid item type ***\n\n");
            exit(1);
    }
    return FALSE; /* Added as lint showed that this routine could just
                     drop out */

} /* eval_tree */
