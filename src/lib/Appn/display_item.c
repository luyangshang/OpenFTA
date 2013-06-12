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
 Module : display_item
 Author : FSC Limited
 Date   : 24/7/96
 
 SccsId : @(#)display_item.c	1.11 12/16/96
 
 Origin : FTA project,
          FSC Limited Private Venture.
 
 This module provides routines to display an Item on the display
 Canvas.
***************************************************************/

#include "display_item.h"
#include <signal.h>
#include <string.h>
#include "treeUtil.h"
#include "tree_pic.h"
#include "symbol_text.h"
#include "symbol.h"
#include "display_tree.h"
#include "Canvas.h"
#include "JavaCanvas.h"

#include "primary_event.h"
#include "PED_shell.h"
//#include "buttondefs.h"
//#include "ButtonActions.h"

#include "StringUtilities.h"

#include "NativeFTAFrame.h"


/* #include "Assert.h" */
/*  */
/* ASSERTFILE( __FILE__ ) */

/*
 * The two following constants determine the line width in pixels. This is
 * required for highlighting and unhighlighting the items.
 */
#define DEFAULT_LINE_WIDTH                1
#define HIGHLIGHT_LINE_WIDTH                3


/*---------------------------------------------------------------
 Routine : draw_connecting_lines
 Purpose : Draws the lines connected to the given item.
---------------------------------------------------------------*/
static void 
  draw_connecting_lines(
    TREE_PIC *tree_pic, 
    ITEM *ip);


/*---------------------------------------------------------------
 Routine : draw_text
 Purpose : Divides the given text string to lengths that fit in 
 the symbol, draws it in the symbol and finally deletes the 
 divided text.
 The point is the top centre of the symbol.
---------------------------------------------------------------*/
static void 
  draw_text(
    Canvas canvas, 
    CanvasCoord symbol_centre,
    char *text, 
    SYMBOL *symbol);


/*---------------------------------------------------------------
 Routine : draw_item
 Purpose : Draws the given item.
---------------------------------------------------------------*/
static void 
  draw_item(
    TREE_PIC *tree_pic, 
    ITEM *ip);

/*---------------------------------------------------------------
 Routine : draw_probability
 Purpose : Draws the probability of the given item next to it on
 the Canvas.
---------------------------------------------------------------*/
static void
  draw_probability(
    TREE_PIC *tree_pic,
    ITEM *ip, 
    CanvasCoord        point )
{
    /* Write probability next to primary event symbol. MPA 20/6/96 */
    if ( is_primary( ip ) )
    {
      CanvasCoord         start;
      unsigned            fontheight;
      PRIMARY_EVENT       *pe;
      char                string_buf[ 15 ];
 
      if ( TreePicGetScale(tree_pic) >= MIN_SCALE_TO_FIT_TEXT ) {
        pe = ped_shell_get_primary_event(
          ip->id,
          tree_pic->tree->database );

        if ( pe != NULL ) {
          fontheight = CanvasFontHeight(tree_pic->canvas);
          start.x = point.x + ( 1.5 * fontheight );
          start.y = point.y + ( 0.05 * fontheight );
          sprintf( string_buf, "%15g", pe->prob );
          CanvasWriteText(
            tree_pic->canvas, 
            strrchr( string_buf, ' ' ) + 1, 
            start);
        }
      }
    }

} /* draw_probability */


/*---------------------------------------------------------------
 Routine : draw_item_all
 Purpose : Draws the given tree item and its associated text.
 
 The description text is only displayed for items which are not gates,
 since it is not required for gates.
---------------------------------------------------------------*/
void
  draw_item_all(
    TREE_PIC *tree_pic, 
    ITEM *ip)
{
    CanvasCoord                point;
    SYMBOL_TYPE                symb_type;

    if (ip == NULL) return;
    point.x = ip->da_x[tree_pic->set_no];
    point.y = ip->da_y[tree_pic->set_no];
    symb_type = translate_item_type(ip->type);
#if 0
    draw_symbol(tree_pic->canvas, point, tree_pic->scaled_symbols[symb_type]);
#endif

    draw_item(tree_pic, ip);

if ( tree_pic->set_no != GLOBAL_VIEW_SET_NO )
    {
    if (TreePicGetScale(tree_pic) >= MIN_SCALE_TO_FIT_TEXT) {
      draw_id(tree_pic->canvas, point, ip->id);

     /* display description for non-gate items */
      if ( !is_gate(ip)) {
	  if ( ( ip->text != NULL ) && ( SU_Is_Null( ip->text ) == FALSE ) )
	      { 
		  if ( !is_primary( ip ) ) 
		      draw_text(
				tree_pic->canvas, point, ip->text,
				tree_pic->scaled_symbols[symb_type]);
	      }
      }
      if ( ( is_primary( ip ) &&
	     ( ped_shell_primary_event_exists( 
					      ip->id, 
					      tree_pic->tree->database ) ) ) )
	  {
	      PRIMARY_EVENT *component;

	      draw_probability( tree_pic, ip, point );

	      component = 
		  (PRIMARY_EVENT *)ped_shell_get_primary_event(
							       ip->id, 
							       tree_pic->tree->database );
	      if ( ( component->desc != NULL ) && 
		   ( SU_Is_Null( component->desc ) == FALSE ) )
		  draw_text(
			    tree_pic->canvas, point, component->desc,
			    tree_pic->scaled_symbols[symb_type]);

	  }

    }
    }

} /* draw_item_all */

/*---------------------------------------------------------------
 Routine : draw_item
 Purpose : Draws the given item.
---------------------------------------------------------------*/
static void
  draw_item(
    TREE_PIC *tree_pic, 
    ITEM *ip)
{
    CanvasCoord                point;
    SYMBOL_TYPE                symb_type;

    if (ip == NULL) return;
    point.x = ip->da_x[tree_pic->set_no];
    point.y = ip->da_y[tree_pic->set_no];
    symb_type = translate_item_type(ip->type);
    if (is_primary(ip) && is_dormant(ip) ) {
        draw_symbol_small(tree_pic->canvas, point,
            tree_pic->scaled_symbols[symb_type]);
    }
    draw_symbol(tree_pic->canvas, point, tree_pic->scaled_symbols[symb_type]);

} /* draw_item */

/*---------------------------------------------------------------
 Routine : highlight_item
 Purpose : Highlights the selected item on the Canvas.
 Handles the ghosting of the Edit menu concerned with highlighting
 a selected item.
---------------------------------------------------------------*/
static void
  highlight_item(
    TREE_PIC *tree_pic, 
    ITEM *ip)
{
    if ( ip != NULL ) {
		CanvasSetLineWidth(tree_pic->canvas, HIGHLIGHT_LINE_WIDTH);
		JavaCanvasSetPenColour(tree_pic->canvas, FTA_GREEN);
		draw_item_all(tree_pic, ip);
		JavaCanvasSetPenColour(tree_pic->canvas, FTA_BLACK);
		CanvasSetLineWidth(tree_pic->canvas, DEFAULT_LINE_WIDTH);
	}
} /* highlight_item */

/*---------------------------------------------------------------
 Routine : unhighlight_item
 Purpose : Unhighlight a highlighted item.
 BEGIN unhighlight_item
    draw over highlighted item in the background colour
    re-draw it normal width
    re-draw connecting lines
 END unhighlight_item
---------------------------------------------------------------*/
static void
  unhighlight_item(
    TREE_PIC *tree_pic, 
    ITEM *ip)
{
    if ( ip != NULL ) { 
		CanvasSetLineWidth(tree_pic->canvas, HIGHLIGHT_LINE_WIDTH);

		/* Delete the item by setting the pen colour to the background
		* colour and then drawing the item, i.e. drawing the item in
		* the background colour in effect deletes the item.
		*/
		/* XCanvasInvertPen(tree_pic->canvas); */
		draw_item(tree_pic, ip);

	  /* Reset the pen colour and redraw the deleted item with the
	   * default line width
	  */
	 CanvasSetLineWidth(tree_pic->canvas, DEFAULT_LINE_WIDTH);
	 /* XCanvasInvertPen(tree_pic->canvas); */
		draw_item(tree_pic, ip);

		/* Redraw the connecting lines since the thicker line width would
		 * delete the edges of these lines
		 */
		draw_connecting_lines(tree_pic, ip);

    } 
} /* unhighlight_item */

/*---------------------------------------------------------------
 Routine : draw_selected_item
 Purpose : This routine deselects any highlighted item, and 
 highlights the supplied item.

 Unhighlight any currently selected item
 Set new selected item
 Highlight new selected item
---------------------------------------------------------------*/
void
  draw_selected_item(
    TREE_PIC *tree_pic, 
    ITEM *selected_item)
{
  /* unhighlight_item(tree_pic, TreePicGetSelectedItem(tree_pic));
  TreePicSetSelectedItem(tree_pic, selected_item); */
  highlight_item(tree_pic, selected_item);

#ifdef XRT
/* This would have been better in the relevent highlight/unhighlight
   routines, but caused the icons to flicker too much.
   MPA 4/11/96 */
  if ( TreePicGetSelectedItem(tree_pic) == NULL )
  {
    XtSetSensitive(Toolbars[FtaToolbar] [ToolbarCut], False );  
    XtSetSensitive(Toolbars[FtaToolbar] [ToolbarCopy], False );  
    XtSetSensitive(Toolbars[FtaToolbar] [ToolbarLink], False );  
  }
  else
  {
    XtSetSensitive(Toolbars[FtaToolbar] [ToolbarCut], True );  
    XtSetSensitive(Toolbars[FtaToolbar] [ToolbarCopy], True );  
    if ( is_primary( selected_item ) )
	{
	    if ( (char *)ped_shell_first() == NULL )
		{
		    if ( tree_pic->tree->database == NULL )
			{
			    XtSetSensitive(
					   Toolbars[FtaToolbar] [ToolbarLink],
					   True );
			    return;
			}
		    else
			{
			    XtSetSensitive(
					   Toolbars[FtaToolbar] [ToolbarLink], 
					   False );
			    return;
			}
		}
	    else
		{
		    if ( tree_pic->tree->database == NULL )
			{
			    XtSetSensitive(
					   Toolbars[FtaToolbar] [ToolbarLink],
					   False );
			    return;
			}
		}
	    if ( strcmp( 
			tree_pic->tree->database,
			(char *)ped_shell_first() ) == 0 ) 
		XtSetSensitive(Toolbars[FtaToolbar] [ToolbarLink], True ); 
	    else
		XtSetSensitive(Toolbars[FtaToolbar] [ToolbarLink], False );
	}
    else
      XtSetSensitive(Toolbars[FtaToolbar] [ToolbarLink], False );
  }
#endif /* XRT */

} /* draw_selected_item */

/*---------------------------------------------------------------
 Routine : draw_connecting_lines
 Purpose : Draws the lines connected to the given item.
---------------------------------------------------------------*/
static void
  draw_connecting_lines(
    TREE_PIC *tree_pic, 
    ITEM *ip)
{
    CanvasCoord                start, end;
    SYMBOL_TYPE                symb_type;
/*     ITEM *child = NULL, *parent = NULL; */

    if (ip == NULL) return;
    start.x = ip->da_x[tree_pic->set_no];
    start.y = ip->da_y[tree_pic->set_no];
    end.x = start.x;
    symb_type = translate_item_type(ip->type);

    if (ip != tree_pic->tree->top_item) {

     /* draw the line that joins to the item from its parent */
        if (ip->type != COND_NOT_ANAL &&
            ip->type != COND_ANAL &&
            ip->type != TRANSOUT) {

                end.y = start.y - (unsigned)(CONNECT_LENGTH *
                        (float)TreePicGetScale(tree_pic));

         /* adjust for dormant events */
            if (is_primary(ip) && is_dormant(ip) ) {
                start.y -= tree_pic->scaled_symbols[symb_type]->depth * 0.05;
            }

                CanvasDrawLine(tree_pic->canvas, start, end);
        }
    }

    if ( (ip->num_children > 1) ||
         ( (ip->num_children == 1) && (ip->child->type != COND_NOT_ANAL &&
                                       ip->child->type != COND_ANAL     &&
                                       ip->child->type != TRANSOUT) ) )    {

        /* draw the line that leads from the item to its children */
            start.y += (unsigned)tree_pic->scaled_symbols[symb_type]->depth;
            end.y = ip->da_y[tree_pic->set_no] +
            (unsigned)(SYMB_LINE_SPACE * (float)TreePicGetScale(tree_pic));
        CanvasDrawLine(tree_pic->canvas, start, end);
    }

 /* deal with condition */
    if (ip->type == COND_NOT_ANAL || ip->type == COND_ANAL ) {

        connect_condition(tree_pic, ip->parent, ip);
    }
    if (ip->child != NULL && (ip->child->type == COND_NOT_ANAL ||
                              ip->child->type == COND_ANAL) ) {
        connect_condition(tree_pic, ip, ip->child);
    }

 /* deal with transfer-out */
    if (ip->child != NULL && ip->child->type == TRANSOUT) {

        connect_transout(tree_pic, ip, ip->child);
    }

} /* draw_connecting_lines */

/*---------------------------------------------------------------
 Routine : translate_item_type
 Purpose : Converts the integer "type" to SYMBOL_TYPE.
 
 This conversion is used to allow manipulation of items using their "symbol
 definition".
---------------------------------------------------------------*/
SYMBOL_TYPE
  translate_item_type(
    int type)
{
    switch(type) {
    case BASIC:
            return SYMB_BASIC;
            break;
    case OR:
            return SYMB_OR;
            break;
    case AND:
            return SYMB_AND;
            break;
    case INTERMEDIATE:
            return SYMB_INTERMEDIATE;
            break;
    case UNDEVELOP:
            return SYMB_UNDEV;
            break;
    case EXTERNAL:
            return SYMB_EXTERN;
            break;
    case TRANSIN:
            return SYMB_TRANS_IN;
            break;
    case PRIORITY_AND:
            return SYMB_PRIORITY_AND;
            break;
    case INHIBIT:
            return SYMB_INHIBIT;
            break;
    case XOR:
            return SYMB_EXCL_OR;
            break;
    case COND_NOT_ANAL:
    case COND_ANAL:
            return SYMB_CONDITION;
            break;
    case TRANSOUT:
        return SYMB_TRANS_OUT;
        break;
    default:
            printf("\n*** translate_item_type : Error translating type ***\n\n");
        raise(SIGABRT);
         /* exit(1); */
    }

    return SYMB_INVALID;

} /* translate_item_type */

/*---------------------------------------------------------------
 Routine : draw_text
 Purpose : Divides the given text string to lengths that fit in 
 the symbol, draws it in the symbol and finally deletes the 
 divided text.
 The point is the top centre of the symbol.
---------------------------------------------------------------*/
static void
  draw_text(
    Canvas       canvas,
    CanvasCoord  symbol_centre,
    char        *text,
    SYMBOL      *symbol)
{
    TEXT        *divided_text;

 /* Divide the text string to lengths that fit in the symbol */
    divided_text = create_text(canvas, text, symbol);
    draw_symbol_text(canvas, divided_text, symbol, symbol_centre);
    delete_text(divided_text);

} /* draw_text */

/*---------------------------------------------------------------
 Routine : is_point_in_item
 Purpose : Checks if the point is in the boundaries of the item.
 
 The point is passed to this function as a set of x and y 
 coordinates.
---------------------------------------------------------------*/
BOOL
  is_point_in_item(
    TREE_PIC *tree_pic, 
    ITEM *ip, 
    int selected_x, 
    int selected_y)
{
    int x_diff, y_diff;
    SYMBOL_TYPE symb_type;

    x_diff = selected_x - ip->da_x[tree_pic->set_no];
    y_diff = selected_y - ip->da_y[tree_pic->set_no];
    symb_type = translate_item_type(ip->type);
    return is_point_in_symbol(
        tree_pic->scaled_symbols[symb_type], x_diff, y_diff);

} /* is_point_in_item */

