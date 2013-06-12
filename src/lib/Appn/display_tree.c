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
 Module : display_tree
 Author : FSC Limited
 Date   : 13/8/96
 
 SccsId : @(#)display_tree.c	1.8 12/16/96
 
 Origin : FTA project,
          FSC Limited Private Venture.
 
 This module provides routines to display a tree.
***************************************************************/

#include <stdlib.h>
#include <math.h>

#include "display_tree.h"
#include "display_item.h"
#include "boolean.h"
#include "util.h"
#include "treeUtil.h"  /* Imports HORIZ/VERT layout */
#include "Canvas.h"
#include "symbol_text.h"
#include "fta.h"
#include "FileUtilities.h"

#include "MemoryUtil.h"
#include "AssertUtil.h"

ASSERTFILE( __FILE__ )


/* used to store the canvas size */
static int maxWidth;
static int maxDepth;

/*
 * Recursively evaluates the drawing_area coordinates of the items in level
 * order, using the level structure.
 */
static void 
  eval_da_coords(
    TREE_PIC *tree_pic, 
    LEVEL *lp, 
    int scale);


/*
 * Draw the tree with respect to the given scale, using level order.
 * To draw the entire tree use tree->top_level for the LEVEL argument.
 */
static void 
  draw_tree_level(
    TREE_PIC *tree_pic, 
    LEVEL *lp, 
    int scale);


/*
 * Draws the connecting lines from the item to its children.
 * The sym_line_space argument is the distance between the top of a symbol
 * and the end of the connecting line leading from the symbol, evaluated
 * in the drawing area coordinate system.
 */
static void 
  draw_leading_lines(
    TREE_PIC *tree_pic, 
    ITEM *ip, 
    int sym_line_space);


/*
 * Checks if the point is within the boundaries of the tree.
 * The point is passed to this function as a set of x and y coordinates.
 */
static BOOL 
  is_point_in_tree(
    TREE_PIC *tree_pic, 
    int selected_x, 
    int selected_y);



/*---------------------------------------------------------------
 Routine : draw_post_it_note
 Purpose : This routine draws the descriptive post it note on the 
 Tree Canvas.
---------------------------------------------------------------*/
void
  draw_post_it_note(
    TREE_PIC *tree_pic )
{
  CanvasCoord symbol_centre;
  char *s;
  TEXT        *divided_text; 
  static RECTANGLE post_it_rect = 
    { {-0.750f, 0.0f }, 1.5f, 1.2f }; 
  static SEGMENT post_it_seg = { NULL, RECTANGLE_SEG, &post_it_rect }; 
  static SYMBOL post_it_symbol = 
    { SYMB_INTERMEDIATE, 
      1.5f, 1.2f, 
      &post_it_seg, 
      1.3f, 1.2f, 
      Max_Number_Of_Symbol_Text_Lines }; 
  SYMBOL *scaled_symbol;
 
  if ( TreePicGetScale( tree_pic ) > 99 ) {
    scaled_symbol =
      scale_symbol( 
        &post_it_symbol, 
        (float)TreePicGetScale( tree_pic ) ); 
    
    symbol_centre.x = 1.0 * (float)TreePicGetScale( tree_pic ); 
    symbol_centre.y = 0.2 * (float)TreePicGetScale( tree_pic ); 
  
/*     draw_symbol( */
/*       tree_pic->canvas, */
/*       symbol_centre, */
/*       scaled_symbol );  */

    if ( TreePicGetScale(tree_pic) >= MIN_SCALE_TO_FIT_TEXT ) 
    {
      char *pStr;
/*         s = strsave( tree_pic->tree->post_it_note ); */
      s = strsave( "Tree: " );
      if ( tree_pic->tree->name == NULL )
        s = strgrow( s, NULL_TREE_NAME );
      else
      {
        pStr = filename_from_pathname( tree_pic->tree->name );
        s = strgrow( s, pStr );
        strfree( pStr );
      }
      s = strgrow( s, "\nDatabase: " );
      if ( tree_pic->tree->database == NULL )
        s = strgrow( s, NULL_DB_NAME );
      else
      {
        pStr = filename_from_pathname( tree_pic->tree->database );
        s = strgrow( s, pStr );
        strfree( pStr );
      }
      s = strgrow( s, "\n" );
      if ( tree_pic->tree->post_it_note != NULL ) 
      {
        s = strgrow( s, tree_pic->tree->post_it_note );
      }

      divided_text =  
        create_text_as_is( 
          tree_pic->canvas,  
          s,
          scaled_symbol ); 
  
      draw_symbol_text_as_is( 
        tree_pic->canvas, 
        divided_text, 
        scaled_symbol, 
        symbol_centre ); 

      if ( divided_text != NULL )
        delete_text( divided_text ); 
      strfree( s );
    }
    /* Free the scaled_symbols memory */
    delete_symbol( scaled_symbol );
  }

} /* draw_post_it_note */



/*---------------------------------------------------------------
 Routine : scale_tree
 Purpose : Scales the tree coordinates from the relative 
 coordinate system to the drawing area coordinate system.
 
 If the scale is set to 0 the tree would appear as a dot.
---------------------------------------------------------------*/
void
  scale_tree(
    TREE_PIC *tree_pic, 
    int scale)
{
    unsigned int sw_width;    /* width of scrolled window */

 /* get width of scrolled window */

	sw_width = 1000;

 /* calculate left offset */
    tree_pic->left_offset = 
      MAX( 
        scale * HORIZ_SPACING, 
        (int)sw_width/2 - (int)(scale * tree_pic->tree->top_item->x) );

 /* tree_pic->screen_width/2 + (int)TreePicGetMaxSymbWidth(tree_pic)/2; */

 /* calculate da_max_x, da_max_y */
    tree_pic->da_max_x = 
        scale * (fabs(tree_pic->tree->min_x) + tree_pic->tree->max_x) +
        tree_pic->left_offset;
    tree_pic->da_max_y = tree_pic->tree->max_y * (float)scale +
    BORDER_WIDTH * (float)scale;

 /* evaluate drawing area coordinates of all items */
	maxDepth = 0;
	maxWidth = 0;
    eval_da_coords(tree_pic, tree_pic->tree->top_level, scale);

} /* scale_tree */



/*---------------------------------------------------------------
 Routine : draw_tree
 Purpose : Draws the tree with respect to the given scale.
---------------------------------------------------------------*/
void
  draw_tree(
    TREE_PIC *tree_pic, 
    int scale)
{

    draw_post_it_note( tree_pic ); 

ASSERT( fValidPointer( tree_pic->tree->top_item, sizeof( ITEM ) ) );

    update_subtree( tree_pic->tree->top_item, tree_pic->tree->database );
    draw_tree_level(tree_pic, tree_pic->tree->top_level, scale); 

    draw_selected_item(tree_pic, TreePicGetSelectedItem(tree_pic));

} /* draw_tree */




/*---------------------------------------------------------------
 Routine : eval_da_coords
 Purpose : Recursively evaluates the drawing_area coordinates of 
 the items in level order, using the level structure.
---------------------------------------------------------------*/
static void
  eval_da_coords(
    TREE_PIC *tree_pic, 
    LEVEL *lp, 
    int scale)
{
    ITEM *lip;
    LEVEL *next;
	int currentWidth, currentDepth;

    if (lp == NULL) return;
    lip = lp->iptr;
    while (lip != NULL) 
    {
      /* da_x[ Set Num ] =
           ( ( tree min x + item x ) * scale ) + left offset
         Not left offset * scale ?
      */
        lip->da_x[tree_pic->set_no] =
            (int) ((fabs(tree_pic->tree->min_x) + lip->x) * (float)scale) +
            tree_pic->left_offset;

      /* da_y[ Set num ] =
           ( ( item y * scale ) + ( BORDER_WIDTH * scale ) )
         tree min y == 0?
      */
        lip->da_y[tree_pic->set_no] =
            (int) ((lip->y*(float)scale) + (BORDER_WIDTH*(float)scale));

		/* calculate max width/depth */
		currentWidth = lip->da_x[tree_pic->set_no] + 100;
		if(currentWidth > maxWidth) {
			maxWidth = currentWidth;
		}
		currentDepth = lip->da_y[tree_pic->set_no] + 100;
		if(currentDepth > maxDepth) {
			maxDepth = currentDepth;
		}
 
		lip = lip->lptr;
 	
	}
    next = lp->next;
    eval_da_coords(tree_pic, next, scale);

} /* eval_da_coords */



/*---------------------------------------------------------------
 Routine : draw_tree_level
 Purpose : Draw the tree with respect to the given scale, using 
 level order.
 To draw the entire tree use tree->top_level for the LEVEL argument.
---------------------------------------------------------------*/
static void
  draw_tree_level(
    TREE_PIC *tree_pic, 
    LEVEL *lp, 
    int scale)
{
    ITEM    *lip;
    int                sym_line_space;

	while ( lp != NULL ) 
	{ 
	/*     if (lp == NULL) return;  */
	/* sym_line_space = (int)(SYMB_LINE_SPACE * (float)scale); */

	/* need to cope with variable vertical spacing	*/
		if (lp->next == NULL) {
			sym_line_space = (int)(SYMB_LINE_SPACE * (float)scale);
	    } else {
		    float vert_spacing = lp->next->iptr->y - lp->iptr->y;
			sym_line_space = (int)((vert_spacing - CONNECT_LENGTH) * (float)scale);
	    }

		lip = lp->iptr;
		while (lip != NULL) {
            draw_item_all(tree_pic, lip);
            draw_leading_lines(tree_pic, lip, sym_line_space);
            lip = lip->lptr;
		}

		/* do next level */
		/*     next = lp->next;  */
		/*     draw_tree_level(tree_pic, next, scale);  */
		lp = lp->next; 
	} 
} /* draw_tree_level */



/*---------------------------------------------------------------
 Routine : connect_condition
 Purpose : Draw line connecting a gate to its condition.
---------------------------------------------------------------*/
void
  connect_condition(
    TREE_PIC *tree_pic, 
    ITEM *gate, 
    ITEM *cond)
{
    CanvasCoord     start, end;
    SYMBOL_TYPE     symb_type;

    symb_type = translate_item_type(gate->type);
    start.x = gate->da_x[tree_pic->set_no] +
              tree_pic->scaled_symbols[symb_type]->width/2.0;
    start.y = gate->da_y[tree_pic->set_no] +
              tree_pic->scaled_symbols[symb_type]->depth/2.0;

    symb_type = translate_item_type(cond->type);
    end.x = cond->da_x[tree_pic->set_no] -
              tree_pic->scaled_symbols[symb_type]->width/2.0;
    end.y = start.y; /* cond->da_y[tree_pic->set_no] +
                        tree_pic->scaled_symbols[symb_type]->depth/2.0; */

    CanvasDrawLine(tree_pic->canvas, start, end);

} /* connect_condition */

/*---------------------------------------------------------------
 Routine : connect_transout
 Purpose : This routine connects a transout symbol with the 
 supplied top item. 
---------------------------------------------------------------*/
void
  connect_transout(
    TREE_PIC *tree_pic, 
    ITEM *top, 
    ITEM *transout)
{
    CanvasCoord     start, corner, end;
/*     SYMBOL_TYPE     symb_type; */

    start.x = top->da_x[tree_pic->set_no];
    start.y = top->da_y[tree_pic->set_no];

    corner.x = start.x;
    corner.y = transout->da_y[tree_pic->set_no] +
                   TRANS_OUT_BIT * tree_pic->scale;

    CanvasDrawLine(tree_pic->canvas, start, corner);

    end.x = transout->da_x[tree_pic->set_no] - 
              tree_pic->scaled_symbols[SYMB_TRANS_OUT]->width/2.0; 
    end.y = corner.y;

    CanvasDrawLine(tree_pic->canvas, corner, end);

} /* connect_transout */




/*---------------------------------------------------------------
 Routine : draw_leading_lines
 Purpose : Draws the connecting lines from the item to its children.

 The sym_line_space argument is the distance between the top of a symbol
 and the end of the connecting line leading from the symbol, evaluated
 in the drawing area coordinate system.
---------------------------------------------------------------*/
static void
  draw_leading_lines(
    TREE_PIC *tree_pic, 
    ITEM *ip, 
    int sym_line_space)
{
    ITEM                     *child;
    SYMBOL_TYPE                symb_type, child_type;
    CanvasCoord                start, end;
    int                        first_child_da_x, last_child_da_x;

    /* Check for transfer in symbols since the qualitative
     * (combo) algorithm sets num_children to 1
     */
    if (ip->num_children > 0 && ip->type != TRANSIN) {

            symb_type = translate_item_type(ip->type);
            child = ip->child;

     /* deal with condition, if present */
        if (child->type == COND_NOT_ANAL || child->type == COND_ANAL) {
            connect_condition(tree_pic, ip, child);
            child = child->right_sib;

     /* deal with transfer-out, if present */
        } else if (child->type == TRANSOUT) {
            connect_transout(tree_pic, ip, child);
            child = child->right_sib;
        }

            if (child != NULL) {
                first_child_da_x = child->da_x[tree_pic->set_no];
        } else {
                first_child_da_x = (int)NULL;
        }

     /* draw vertical lines to each child */
            while (child != NULL) {
                start.x = child->da_x[tree_pic->set_no];
                start.y = child->da_y[tree_pic->set_no];
                end.x = start.x;
                end.y = ip->da_y[tree_pic->set_no] + sym_line_space;

         /* adjust for dormant events */
            if (is_primary(child) && is_dormant(child) ) {
                child_type = translate_item_type(child->type);
                start.y -= tree_pic->scaled_symbols[child_type]->depth * 0.05;
            }

                CanvasDrawLine(tree_pic->canvas, start, end);
                last_child_da_x = child->da_x[tree_pic->set_no];
                child = child->right_sib;
            }

        if (first_child_da_x != (int)NULL) {

         /* draw vertical line down from the item */
            start.x = ip->da_x[tree_pic->set_no];
            start.y = ip->da_y[tree_pic->set_no] +
            (unsigned)tree_pic->scaled_symbols[symb_type]->depth;
            end.x = start.x;
            end.y = ip->da_y[tree_pic->set_no] + sym_line_space;
            CanvasDrawLine(tree_pic->canvas, start, end);

         /* draw horizontal line to connect the vertical lines */
                start.x = first_child_da_x;
                start.y = ip->da_y[tree_pic->set_no] + sym_line_space;
                end.x = last_child_da_x;
                end.y = start.y;
                CanvasDrawLine(tree_pic->canvas, start, end);
        }
    }

} /* draw_leading_lines */



/*---------------------------------------------------------------
 Routine : find_item
 Purpose : Finds the item clicked on in the drawing area.
 If the item is not found NULL is returned.
---------------------------------------------------------------*/
ITEM *
  find_item(
    TREE_PIC *tree_pic, 
    LEVEL *lp, 
    int selected_x, 
    int selected_y)
{
    LEVEL        *level;
    ITEM        *lip;

    if (lp == NULL) return NULL;
    if (is_point_in_tree(tree_pic, selected_x, selected_y)) {
            level = find_level(tree_pic, lp, selected_y);
            if (level == NULL) return NULL;
            lip = level->iptr;
            while (lip != NULL) {
                if (is_point_in_item(tree_pic, lip, selected_x, selected_y)) {
                        return lip;
                }
                lip = lip->lptr;
            }
    }
    return NULL;

} /* find_item */



/*---------------------------------------------------------------
 Routine : is_point_in_tree
 Purpose : Checks if the point is within the boundaries of the tree.
 The point is passed to this function as a set of x and y 
 coordinates.
---------------------------------------------------------------*/
static BOOL
  is_point_in_tree(
    TREE_PIC *tree_pic, 
    int selected_x, 
    int selected_y)
{
/*
    return (
        selected_x >= tree_pic->screen_width/2 &&
        selected_x <= tree_pic->da_max_x+
        (int)(TreePicGetMaxSymbWidth(tree_pic)/2.0) &&
        selected_y >= (int)(BORDER_WIDTH*(float)TreePicGetScale(tree_pic)) &&
        selected_y <= tree_pic->da_max_y+
        (int)(TreePicGetMaxSymbDepth(tree_pic)));
*/

    return( selected_x <= tree_pic->da_max_x &&
            selected_y <= tree_pic->da_max_y );

} /* is_point_in_tree */


int getMaxWidth() {
	return maxWidth;
}

int getMaxDepth() {
	return maxDepth;
}
