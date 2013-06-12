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
 Module : Symbol Text
 Author : FSC Limited
 Date   : 29/11/95
 
 SccsId :@(#)symbol_text.h	1.3 12/17/96
 
 Purpose :
   This module manipulates the text associated with a symbol, i.e. the
   ID and description of a symbol. 
****************************************************************/

#ifndef symbol_h
#define symbol_h

#include "symbol.h"
#include "Canvas.h"

#ifdef CPP

const unsigned short Max_Number_Of_Symbol_Text_Lines = 10;

/*--------------------------------------------------------------
 This type references the text associated with a symbol.
---------------------------------------------------------------*/
    typedef struct text{
    int     nlines;
    char    *text[ Max_Number_Of_Symbol_Text_Lines ];
}TEXT;

#else

#define Max_Number_Of_Symbol_Text_Lines 10
typedef struct text{
    int     nlines;
    char    *text[ Max_Number_Of_Symbol_Text_Lines ];
}TEXT;

#endif /* CPP */
 
/*--------------------------------------------------------------
 Routine : create_text
 Purpose : Divides the given text string to lengths that fit in the symbol.
           The divided text is stored in a TEXT structure.
           Space is allocated to hold the divided text.
           The divided text is retured.
---------------------------------------------------------------*/
extern TEXT *
  create_text(
    Canvas canvas, 
    char *text, 
    SYMBOL *symbol);

/*--------------------------------------------------------------
 Routine : delete_text
 Purpose : Deletes the divided text.
           All space allocated to hold the divided text is returned.
---------------------------------------------------------------*/
extern void
  delete_text(
    TEXT *text);

/*--------------------------------------------------------------
 Routine : draw_symbol_text
 Purpose : Draws the text in the centre of the symbol.
           The point is the top centre of the symbol.
---------------------------------------------------------------*/
extern void
  draw_symbol_text(
    Canvas canvas, 
    TEXT *text,
    SYMBOL *symbol, 
    CanvasCoord symbol_centre);

/*--------------------------------------------------------------
 Routine : draw_id
 Purpose : Draws the id to the right hand side above the symbol.
           The point is the top centre of the symbol.
---------------------------------------------------------------*/
extern void
  draw_id(
    Canvas canvas,
    CanvasCoord symbol_centre, 
    char *id);

/*--------------------------------------------------------------
 Routine : create_text_as_is
 Purpose : This routine is provided for the draw_post_it_note routine
           which needs to preserve the layout of the text within the
           symbol, and not change it as at present.
---------------------------------------------------------------*/
extern TEXT *
  create_text_as_is(
    Canvas  canvas,
    char   *text,
    SYMBOL *symbol );

/*--------------------------------------------------------------
 Routine : draw_symbol_text_as_is
 Purpose :
---------------------------------------------------------------*/
extern void
  draw_symbol_text_as_is(
    Canvas canvas, 
    TEXT *text,
    SYMBOL *symbol, 
    CanvasCoord symbol_centre);

#endif
