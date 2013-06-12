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
 Module : Symbol
 Author : FSC Limited
 Date   : 07/12/95
 
 SccsId :@(#)symbol.h	1.2 11/22/96
 
 Purpose: This module defines and manipulates the SYMBOL structure. 
****************************************************************/

#ifndef Symbol_h
#define Symbol_h

#include "boolean.h"
#include "segments.h"
#include "Canvas.h"

typedef enum symbol_type{
	SYMB_BASIC = 0,
	SYMB_UNDEV = 1,
	SYMB_INTERMEDIATE = 2, 
	SYMB_AND = 3, 
	SYMB_OR = 4,
	SYMB_PRIORITY_AND = 5, 
	SYMB_TRANS_IN = 6,
	SYMB_CONDITION = 7,
	SYMB_EXTERN = 8,
	SYMB_EXCL_OR = 9, 
	SYMB_INHIBIT= 10, 
	SYMB_TRANS_OUT = 11, 
	SYMB_INVALID = 12
}SYMBOL_TYPE;

#ifdef TWO_COL_MODE
    #define PIXMAP_SIZE         64
    #define MAX_NUM_PIXMAPS     14
    #define NUM_PIXMAPS         12
    #define SCALE_FOURTY        40
    #define SCALE_FIFTY         50
    #define SCALE_SIXTY_SIX     66
    #define SCALE_HUNDRED       100

    #define BASIC_POS           0
    #define UNDEVELOP_POS       1
    #define AND_POS             2
    #define PRIORITY_AND_POS    3
    #define TRANSIN_POS         4
    #define COND_POS            5
    #define EXTERNAL_POS        6
    #define INTERMEDIATE_POS    7
    #define OR_POS              8
    #define XOR_POS             9
    #define INHIBIT_POS         10
    #define TRANSOUT_POS        11

#else
    #define PIXMAP_SIZE         64 / 2
    #define MAX_NUM_PIXMAPS     12
    #define NUM_PIXMAPS         12
    #define SCALE_FOURTY        40 / 2
    #define SCALE_FIFTY         50 / 2
    #define SCALE_SIXTY_SIX     66 / 2
    #define SCALE_HUNDRED       100 / 2

    #define INTERMEDIATE_POS    0
    #define BASIC_POS           1
    #define UNDEVELOP_POS       2
    #define EXTERNAL_POS        3
    #define COND_POS            4
    #define AND_POS             5
    #define OR_POS              6
    #define PRIORITY_AND_POS    7
    #define XOR_POS             8
    #define INHIBIT_POS         9
    #define TRANSIN_POS         10
    #define TRANSOUT_POS        11

#endif


/*--------------------------------------------------------------
 This type provides access to the individual components of a symbol.
---------------------------------------------------------------*/
typedef struct symbol {
	SYMBOL_TYPE	type;
	float		width, depth;
	SEGMENT		*segment_list;
	float		textwidth;
        float           textdepth;
        unsigned short  text_lines_allowed;
} SYMBOL;


/*--------------------------------------------------------------
 Routine : scale_symbol 
 Purpose : Scales the symbol.
           Creates and returns the "scaled" symbol.
           Space is allocated to hold the scaled symbol.
---------------------------------------------------------------*/
extern SYMBOL *
  scale_symbol(
    SYMBOL *symbol, 
    float scale);



/*--------------------------------------------------------------
 Routine : draw_symbol
 Purpose : Draws the symbol.
---------------------------------------------------------------*/
extern void 
  draw_symbol(
    Canvas canvas, 
    CanvasCoord symbol_centre, 
    SYMBOL *symbol);



/*--------------------------------------------------------------
 Routine : delete_symbol
 Purpose : Deletes the symbol.
           All space allocated to hold the symbol is returned.
---------------------------------------------------------------*/
extern void 
  delete_symbol(
    SYMBOL *symbol);


/*--------------------------------------------------------------
 Routine : draw_symbol_small
 Purpose :
---------------------------------------------------------------*/
extern void
  draw_symbol_small(
    Canvas canvas, 
    CanvasCoord symbol_centre, 
    SYMBOL *symbol);



/*--------------------------------------------------------------
 Routine : is_point_in_symbol
 Purpose : Checks if the difference of two point is in the boundaries of the symbol.
           The difference is passed to this function as a set of x and y values
           (x_diff and y_diff).
---------------------------------------------------------------*/
extern BOOL 
  is_point_in_symbol(
    SYMBOL *symbol, 
    int x_diff, 
    int y_diff);


#endif
