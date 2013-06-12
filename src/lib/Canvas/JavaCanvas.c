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
 Module : JavaCanvas
 Author : FSC Limited
 Date   : 20/11/98
 
 SccsId : @(#)JavaCanvas.c	1.1 11/20/96
 
 Origin : FTA project,
          FSC Limited Private Venture.
 
 This module implements the canvas dependent on the Java 
 AWT and Swing class libraries.
***************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include "JavaCanvasP.h"
#include "CanvasP.h" 

#include "MemoryUtil.h"

#include "NativeFTACanvas.h"




/*---------------------------------------------------------------
 Routine : JavaCanvasClear
 Purpose : Clears the Java canvas area.
---------------------------------------------------------------*/
void
  JavaCanvasClear(
    Canvas canvas)
{

    

} /* JavaCanvasClear */



/*---------------------------------------------------------------
 Routine : JavaCanvasSetPenColour
 Purpose : Set the foreground (pen) Colour
---------------------------------------------------------------*/
void
  JavaCanvasSetPenColour(
    Canvas canvas, 
    unsigned long colour)
{
 
	FTACanvasSetPenColour(colour);

} /* JavaCanvasSetPenColour */



/*---------------------------------------------------------------
 Routine : JavaCanvasSetPaperColour
 Purpose : Set the background (paper) colour
---------------------------------------------------------------*/
void
  JavaCanvasSetPaperColour(
    Canvas canvas, 
    unsigned long colour)
{
 

} /* JavaCanvasSetPaperColour */





/*---------------------------------------------------------------
 Routine : JavaCanvasInvertPen
 Purpose : Inverts the pen colour to the existing pen or paper 
 colour depending if the pen is on, i.e. if on the pen colour is 
 set to the paper colour. This is usefull for deleting symbols 
 from the canvas area.
---------------------------------------------------------------*/
void
  JavaCanvasInvertPen(
    Canvas canvas)
{
 

} /* JavaCanvasInvertPen */



/*---------------------------------------------------------------
 Routine : JavaCanvasIsPenOn
 Purpose : Checks if pen is on.
---------------------------------------------------------------*/
BOOL
  JavaCanvasIsPenOn(
    const Canvas canvas)
{
 
    return TRUE;

} /* JavaCanvasIsPenOn */



/*---------------------------------------------------------------
 Routine : JavaCanvasSetLineWidth
 Purpose : Sets the line width for the Java canvas area.
---------------------------------------------------------------*/
void
  JavaCanvasSetLineWidth(
    Canvas canvas, 
    const unsigned line_width)
{
	FTACanvasSetPenWidth(line_width);
} /* JavaCanvasSetLineWidth */



/*---------------------------------------------------------------
 Routine : JavaCanvasWriteText
 Purpose : Writes the given text from the specified start point on
 the Java canvas area.
---------------------------------------------------------------*/
void
  JavaCanvasWriteText(
    Canvas canvas, 
    const char *text, 
    const CanvasCoord start)
{
	FTACanvasWriteText(start.x, start.y, text);
} /* JavaCanvasWriteText */



/*---------------------------------------------------------------
 Routine : JavaCanvasSetFont
 Purpose : Sets the font style for the Java canvas area.
---------------------------------------------------------------*/
void
  JavaCanvasSetFont(
    Canvas canvas, 
    const char *font_string)
{
 
} /* JavaCanvasSetFont */



/*---------------------------------------------------------------
 Routine : JavaCanvasDrawLine
 Purpose : Draws a line from the specified start point to the end
 point on the Java canvas area.
---------------------------------------------------------------*/
void
  JavaCanvasDrawLine(
    Canvas canvas, 
    const CanvasCoord start, 
    const CanvasCoord end)
{
	FTACanvasDrawLine(start.x, start.y, end.x, end.y);
} /* JavaCanvasDrawLine */



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
	int symbol_type)
{
	FTACanvasFillRect(point.x, point.y, width, height, symbol_type);

} /* JavaCanvasFillRectangle */


/*---------------------------------------------------------------
 Routine : JavaCanvasDrawRectangle
 Purpose : Draws a rectangle from the specified point with the 
 specified width and height on the Java canvas area.
---------------------------------------------------------------*/
void
  JavaCanvasDrawRectangle(
    Canvas canvas, 
    const CanvasCoord point,
    const unsigned width, 
    const unsigned height)
{
	FTACanvasDrawRect(point.x, point.y, width, height);

} /* JavaCanvasDrawRectangle */


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
	int symbol_type)
{
	FTACanvasFillPolyline(points, num_points, symbol_type);
} /* JavaCanvasFillPolyline */


/*---------------------------------------------------------------
 Routine : JavaCanvasDrawPolyline
 Purpose : Draws a polyline on the Java canvas area. The polyline is
 multiple connected lines and is defined by an array of points and
 the number of points.
---------------------------------------------------------------*/
void
  JavaCanvasDrawPolyline(
    Canvas canvas, 
    const unsigned num_points, 
    const CanvasCoord *points)
{
	unsigned int i;

	if (num_points > 1) {
		for(i = 0; i < (num_points-1); i++) {
			FTACanvasDrawLine(points[i].x, points[i].y, points[i+1].x, points[i+1].y);
		}
	}
} /* JavaCanvasDrawPolyline */



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
    const int end_angle)
{
	FTACanvasClearArc(point.x, point.y, width, height, start_angle, end_angle);

} /* JavaCanvasClearArc */



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
	int symbol_type)
{
	FTACanvasFillArc(point.x, point.y, width, height, start_angle, end_angle, symbol_type);

} /* JavaCanvasFillArc */




/*---------------------------------------------------------------
 Routine : JavaCanvasDrawArc
 Purpose : Draws an arc starting from the start_angle, relative to
 a three o'clock position, to end_angle, within the bounding 
 rectangle specified by the * parameters point, width and height.
 
 The angles are specified in degrees.
 For example to draw a circle or ellipse specify a start_angle of 
 zero and an end_angle of 360.
---------------------------------------------------------------*/
void
  JavaCanvasDrawArc(
    Canvas canvas, 
    const CanvasCoord point,
    const unsigned width, 
    const unsigned height,
    const int start_angle, 
    const int end_angle)
{
	FTACanvasDrawArc(point.x, point.y, width, height, start_angle, end_angle);

} /* JavaCanvasDrawArc */



/*---------------------------------------------------------------
 Routine : JavaCanvasLineWidth
 Purpose : Returns the line width of the Java canvas.
---------------------------------------------------------------*/
unsigned
  JavaCanvasLineWidth(
    const Canvas canvas)
{

    return 1;

} /* JavaCanvasLineWidth */



/*---------------------------------------------------------------
 Routine : JavaCanvasTextWidth
 Purpose : Returns the text width of the Java canvas.
---------------------------------------------------------------*/
unsigned
  JavaCanvasTextWidth(
    const Canvas canvas, 
    const char *text)
{

	return FTACanvasTextWidth(text);

} /* JavaCanvasTextWidth */



/*---------------------------------------------------------------
 Routine : JavaCanvasFontHeight
 Purpose : Returns the font height of the Java canvas.
---------------------------------------------------------------*/
unsigned
  JavaCanvasFontHeight(
    const Canvas canvas)
{

	return FTACanvasFontHeight();

} /* JavaCanvasFontHeight */

