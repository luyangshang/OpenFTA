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
 Module : JavaCanvasP
 Author : FSC Limited
 Date   : 20/11/98
 
 SccsId : @(#)XCanvasP.h	1.1 08/22/96
 
 Origin : FTA project,
          FSC Limited Private Venture.
 
 This module defines the private Java canvas routines dependent on 
 the X/Motif libraries, e.g. DrawingArea and Pixmaps.
***************************************************************/

#ifndef _JavaCanvasP_H
#define _JavaCanvasP_H

#include "JavaCanvas.h"

/*
 * The types of Canvas.
 */

typedef struct _JavaCanvas_rec {

            unsigned long        pen_colour;
            unsigned long        paper_colour;
        } JavaCanvas_rec;



/*---------------------------------------------------------------
 Routine : JavaCanvasSetLineWidth
 Purpose : Sets the line width for the Java canvas area.
---------------------------------------------------------------*/
extern void        
  JavaCanvasSetLineWidth(
    Canvas canvas, 
    const unsigned line_width);

/*---------------------------------------------------------------
 Routine : JavaCanvasWriteText
 Purpose : Writes the given text from the specified start point on
 the Java canvas area.
---------------------------------------------------------------*/
extern void        
  JavaCanvasWriteText(
    Canvas canvas,
    const char *text, 
    const CanvasCoord start);

/*---------------------------------------------------------------
 Routine : javaCanvasSetFont
 Purpose : Sets the font style for the Java canvas area.
---------------------------------------------------------------*/
extern void        
  JavaCanvasSetFont(Canvas canvas, 
    const char *font_string);

/*---------------------------------------------------------------
 Routine : JavaCanvasDrawLine
 Purpose : Draws a line from the specified start point to the end
 point on the Java canvas area.
---------------------------------------------------------------*/
extern void        
  JavaCanvasDrawLine(
    Canvas canvas,
    const CanvasCoord start, 
    const CanvasCoord end);


/*---------------------------------------------------------------
 Routine : JavaCanvasFillRectangle
 Purpose : Draws a filled rectangle from the specified point with the 
 specified width and height on the Java canvas area.
 Uses the symbol type to determine the colour.
---------------------------------------------------------------*/
void
  JavaCanvasFillRectangle(
    Canvas canvas, 
    const CanvasCoord point,
    const unsigned width, 
    const unsigned height,
	int symbol_type);


/*---------------------------------------------------------------
 Routine : JavaCanvasDrawRectangle
 Purpose : Draws a rectangle from the specified point with the 
 specified width and height on the Java canvas area.
---------------------------------------------------------------*/
extern void        
  JavaCanvasDrawRectangle(
    Canvas canvas, 
    const CanvasCoord point,
    const unsigned width, 
    const unsigned height);


/*---------------------------------------------------------------
 Routine : JavaCanvasFillPolyline
 Purpose : Draws a polyline on the Java canvas area. The polyline is
 multiple connected lines and is defined by an array of points and
 the number of points.
 Uses the symbol type to determine the colour.
---------------------------------------------------------------*/
void
  JavaCanvasFillPolyline(
    Canvas canvas, 
    const unsigned num_points, 
    const CanvasCoord *points,
	int symbol_type);


/*---------------------------------------------------------------
 Routine : JavaCanvasDrawPolyline
 Purpose : Draws a polyline on the Java canvas area. The polyline is
 multiple connected lines and is defined by an array of points and
 the number of points.
---------------------------------------------------------------*/
extern void        
  JavaCanvasDrawPolyline(
    Canvas canvas,
    const unsigned num_points, 
    const CanvasCoord *points);


/*---------------------------------------------------------------
 Routine : JavaCanvasClearArc
 Purpose : Clears an arc using the backgrounf colour, 
 starting from the start_angle, relative to
 a three o'clock position, to end_angle, within the bounding 
 rectangle specified by the * parameters point, width and height.
 
 The angles are specified in degrees.
 For example to draw a circle or ellipse specify a start_angle of 
 zero and an end_angle of 360.
---------------------------------------------------------------*/
void
  JavaCanvasClearArc(
    Canvas canvas, 
    const CanvasCoord point,
    const unsigned width, 
    const unsigned height,
    const int start_angle, 
    const int end_angle);


/*---------------------------------------------------------------
 Routine : JavaCanvasFillArc
 Purpose : Draws a filled arc starting from the start_angle, relative to
 a three o'clock position, to end_angle, within the bounding 
 rectangle specified by the * parameters point, width and height.
 
 The angles are specified in degrees.
 For example to draw a circle or ellipse specify a start_angle of 
 zero and an end_angle of 360.
 Uses the symbol type to determine the colour.
---------------------------------------------------------------*/
void
  JavaCanvasFillArc(
    Canvas canvas, 
    const CanvasCoord point,
    const unsigned width, 
    const unsigned height,
    const int start_angle, 
    const int end_angle,
	int symbol_type);


/*---------------------------------------------------------------
 Routine : JavaCanvasDrawArc
 Purpose : Draws an arc starting from the start_angle, relative to
 a three o'clock position, to end_angle, within the bounding 
 rectangle specified by the * parameters point, width and height.

 The angles are specified in degrees.
 For example to draw a circle or ellipse specify a start_angle of 
 zero and an end_angle of 360.
---------------------------------------------------------------*/
extern void        
  JavaCanvasDrawArc(
    Canvas canvas, 
    const CanvasCoord point,
    const unsigned width, 
    const unsigned height,
    const int start_angle, 
    const int end_angle);

/*---------------------------------------------------------------
 Routine : JavaCanvasLineWidth
 Purpose : Returns the line width of the Java canvas.
---------------------------------------------------------------*/
extern unsigned        
  JavaCanvasLineWidth(
    const Canvas canvas);

/*---------------------------------------------------------------
 Routine : JavaCanvasTextWidth
 Purpose : Returns the text width of the Java canvas.
---------------------------------------------------------------*/
extern unsigned        
  JavaCanvasTextWidth(
    const Canvas canvas, 
    const char *text);

/*---------------------------------------------------------------
 Routine : JavaCanvasFontHeight
 Purpose : Returns the font height of the Java canvas.
---------------------------------------------------------------*/
extern unsigned        
  JavaCanvasFontHeight(
    const Canvas canvas);

#endif
