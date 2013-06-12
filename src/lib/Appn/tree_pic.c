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
 Module : Tree Picture
 Author : FSC Limited
 Date   : 22/12/95
 
 SccsId :@(#)tree_pic.c	1.6 12/16/96
 
 This module creates and manipulates a tree picture.
 The tree picture contains information about the fault tree, the canvas
 area which holds the tree as well as the drawing attributes.
 
****************************************************************/

#include <stdlib.h>

#include "tree_pic.h"
#include "treeUtil.h"

#include "util.h"

#include "MemoryUtil.h"
#include "AssertUtil.h" 
 
ASSERTFILE( __FILE__ ) 

#define CANVAS_MARGIN_WIDTH                5

#define DEFAULT_SYMBOL_TEXT_FONT        "-*-helvetica-medium-r-normal-*-100-*"

/*--------------------------------------------------------------
 Routine : TreePicGetScale
 Purpose : Returns the scale used in the given tree picture.
---------------------------------------------------------------*/
int
TreePicGetScale(TREE_PIC *tree_pic)
{
    return tree_pic->scale;
}


/*--------------------------------------------------------------
 Routine : TreePicFocus
 Purpose : Places the item in the top centre of the canvas for the
           given tree pictures.
---------------------------------------------------------------*/
void
TreePicFocus(TREE_PIC *tree_pic, ITEM *item)
{
    if (item == NULL) {
        return;
    }

    /* set_scrollbars(tree_pic->scrolled_window,
                   item->da_x[tree_pic->set_no],
                   item->da_y[tree_pic->set_no]); */

}



/*--------------------------------------------------------------
 Routine : TreePicSetSelectedItem
 Purpose : Function to set the selected item for the given tree picture.
---------------------------------------------------------------*/
void
TreePicSetSelectedItem(TREE_PIC        *tree_pic,
                       ITEM     *item)
{
    if (tree_pic != NULL) {
        tree_pic->selected_item = item;
    }
}



/*--------------------------------------------------------------
 Routine : TreePicGetSelectedItem
 Purpose :  Function to get the selected item for the given tree picture.
---------------------------------------------------------------*/
ITEM *
TreePicGetSelectedItem(TREE_PIC *tree_pic)
{
    return tree_pic->selected_item;
}



/*--------------------------------------------------------------
 Routine : TreePicGetMaxSymbWidth
 Purpose :
---------------------------------------------------------------*/
float
TreePicGetMaxSymbWidth(TREE_PIC *tree_pic)
{
    int                symbol_type;
    float        max_symbol_width = 0.0;

    for ( symbol_type=SYMB_BASIC; symbol_type<SYMB_INVALID; symbol_type++ ) {
        if (max_symbol_width < tree_pic->scaled_symbols[symbol_type]->width) {
            max_symbol_width = tree_pic->scaled_symbols[symbol_type]->width;
        }
    }
    return max_symbol_width;
}



/*--------------------------------------------------------------
 Routine : TreePicGetMaxSymbDepth
 Purpose :
---------------------------------------------------------------*/
float
TreePicGetMaxSymbDepth(TREE_PIC *tree_pic)
{
    int                symbol_type;
    float        max_symbol_depth = 0.0;

    for ( symbol_type=SYMB_BASIC; symbol_type<SYMB_INVALID; symbol_type++ ) {
        if (max_symbol_depth < tree_pic->scaled_symbols[symbol_type]->depth) {
            max_symbol_depth = tree_pic->scaled_symbols[symbol_type]->depth;
        }
    }
    return max_symbol_depth;
}

