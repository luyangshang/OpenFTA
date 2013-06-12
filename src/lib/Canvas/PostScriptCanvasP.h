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
 Module : PostscriptCanvasP
 Author : FSC Limited
 Date   : 5/8/96
 
 SccsId : @(#)PostScriptCanvasP.h	1.1 08/22/96
 
 Origin : FTA project,
          FSC Limited Private Venture.
 
 This module defines the private routines of the Postscript 
 canvas. 

 The Postscript commands conform to the Encapsulated PostScript
 Standard.
***************************************************************/

#ifndef _PostScriptP_H
#define _PostScriptP_H


#include <stdio.h>
#include <X11/Intrinsic.h>

#include "PostScriptCanvas.h"


typedef struct _PostScript_rec {
            FILE                *file;
            const char                *filename;
            XFontStruct         *font;     /* a horrible hack to allow
                                            * the postscript code to call
                                            * CanvasTextWidth and
                                            * CanvasFontHeight
                                            */
        } PostScript_rec;

/*---------------------------------------------------------------
 Routine : PostScriptCanvasDestroy
 Purpose : Destroys the Postscript canvas (i.e., the file containing
 the Postscript).
---------------------------------------------------------------*/
extern void                
  PostScriptCanvasDestroy(
    Canvas canvas);

/*---------------------------------------------------------------
 Routine : PostScriptCanvasSetLineWidth
 Purpose : Sets the drawing line width of the Postscript canvas.
---------------------------------------------------------------*/
extern void                
  PostScriptCanvasSetLineWidth(
    Canvas canvas, 
    const unsigned line_width);

/*---------------------------------------------------------------
 Routine : PostScriptCanvasWriteText
 Purpose : Writes the supplied text at the supplied coordinates
 to the Postscript canvas.
---------------------------------------------------------------*/
extern void                
  PostScriptCanvasWriteText(
    Canvas canvas, 
    const char *text,
    const CanvasCoord start);

/*---------------------------------------------------------------
 Routine : PostScriptCanvasSetFont
 Purpose : Sets the Font of the Postscript canvas.
---------------------------------------------------------------*/
extern void                
  PostScriptCanvasSetFont(
    Canvas canvas, 
    const char *font_string);

/*---------------------------------------------------------------
 Routine : PostScriptCanvasDrawLine
 Purpose : Draws the described line to the Postscript canvas.
---------------------------------------------------------------*/
extern void                
  PostScriptCanvasDrawLine(
    Canvas canvas,
    const CanvasCoord start, 
    const CanvasCoord end);

/*---------------------------------------------------------------
 Routine : PostScriptCanvasDrawRectangle
 Purpose : Draws the described Rectangle to the Postscript canvas.
---------------------------------------------------------------*/
extern void                
  PostScriptCanvasDrawRectangle(
    Canvas canvas, 
    const CanvasCoord point,
    const unsigned width, 
    const unsigned height);

/*---------------------------------------------------------------
 Routine : PostScriptCanvasDrawPolyline
 Purpose : Draws the described Polyline to the Postscript canvas.
---------------------------------------------------------------*/
extern void                
  PostScriptCanvasDrawPolyline(
    Canvas canvas, 
    const unsigned num_points,
    const CanvasCoord *points);

/*---------------------------------------------------------------
 Routine : PostScriptCanvasDrawArc
 Purpose : Draws the described Arc to the Postscript canvas.
---------------------------------------------------------------*/
extern void                
  PostScriptCanvasDrawArc(
    Canvas canvas, 
    CanvasCoord point,
    unsigned width, 
    unsigned height,
    int start_angle, 
    int end_angle);


#endif
