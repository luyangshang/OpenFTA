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
 Module : Canvas
 Author : FSC Limited
 Date   : 5/8/96
 
 SccsId : @(#)Canvas.h	1.1 08/22/96
 
 Origin : FTA project,
          FSC Limited Private Venture.
 
 This module implements the canvas.
***************************************************************/

#ifndef _Canvas_H
#define _Canvas_H


typedef struct _CanvasCoord {
            unsigned                x;
            unsigned                y;
        } CanvasCoord;


typedef struct _Canvas_rec *Canvas;

/*---------------------------------------------------------------
 Routine : CanvasCreate
 Purpose : Creates a canvas object. Allocates memory to hold the
 Canvas object.
---------------------------------------------------------------*/
extern Canvas         
  CanvasCreate(); 

/*---------------------------------------------------------------
 Routine : CanvasDestroy
 Purpose : Destroys a canvas area.

 All space allocated to hold the canvas is released.
---------------------------------------------------------------*/
extern void        
  CanvasDestroy(Canvas canvas);

/*---------------------------------------------------------------
 Routine : CanvasSetLineWidth
 Purpose : Sets the line width for the canvas area.
---------------------------------------------------------------*/
extern void        
  CanvasSetLineWidth(Canvas canvas, const unsigned width);

/*---------------------------------------------------------------
 Routine : CanvasWriteText
 Purpose : Writes the given text from the specified start point 
 on the canvas area.
---------------------------------------------------------------*/
extern void        
  CanvasWriteText(
    Canvas canvas,
    const char *text, 
    const CanvasCoord start);

/*---------------------------------------------------------------
 Routine : CanvasSetFont
 Purpose : Sets the font style for the canvas area.
---------------------------------------------------------------*/
extern void        
  CanvasSetFont(
    Canvas canvas, 
    const char *font_string);

/*---------------------------------------------------------------
 Routine : CanvasDrawLine
 Purpose : Draws a line from the specified start point to the end 
 point on the canvas area.
---------------------------------------------------------------*/
extern void        
  CanvasDrawLine(
    Canvas canvas,
    const CanvasCoord start, 
    const CanvasCoord end);


/*---------------------------------------------------------------
 Routine : CanvasFillRectangle
 Purpose : Draws a filled rectangle from the specified point with the
 specified width and height on the canvas area.
           Uses symbol type to determine the colour
 ---------------------------------------------------------------*/
void
  CanvasFillRectangle(
    Canvas canvas, 
    const CanvasCoord point,
    const unsigned width, 
    const unsigned height,
	int symbol_type);


/*---------------------------------------------------------------
 Routine : CanvasDrawRectangle
 Purpose : Draws a rectangle from the specified point with the 
 specified width and height on the canvas area.
---------------------------------------------------------------*/
extern void        
  CanvasDrawRectangle(
    Canvas canvas, 
    const CanvasCoord point,
    const unsigned width, 
    const unsigned height);


/*---------------------------------------------------------------
 Routine : CanvasFillPolyline
 Purpose : Draws a polyline on the canvas area. The polyline is
 multiple connected lines and is defined by an array of points
 and the number of points.
 Uses symbol type to determine the colour
---------------------------------------------------------------*/
void
  CanvasFillPolyline(
    Canvas canvas, 
    const unsigned num_points, 
    const CanvasCoord *points,
	int symbol_type);


/*---------------------------------------------------------------
 Routine : CanvasDrawPolyline
 Purpose : Draws a polyline on the canvas area. The polyline is 
 multiple connected lines and is defined by an array of points 
 and the number of points.
---------------------------------------------------------------*/
extern void        
  CanvasDrawPolyline(
    Canvas canvas,
    const unsigned num_points, 
    const CanvasCoord *points);


/*---------------------------------------------------------------
 Routine : CanvasClearArc
 Purpose : Clears an arc using the background colour,
 starting from the start_angle, relative
 to a three o'clock position, to end_angle, within the bounding
 rectangle specified by the parameters point, width and height.
 The angles are specified in degrees.
 For example to draw a circle or ellipse specify a start_angle of zero
 and an end_angle of 360.
---------------------------------------------------------------*/
void
  CanvasClearArc(
    Canvas canvas, 
    const CanvasCoord point,
    const unsigned width, 
    const unsigned height,
    const int start_angle, 
    const int end_angle);


/*---------------------------------------------------------------
 Routine : CanvasFillArc
 Purpose : Draws a filled arc starting from the start_angle, relative
 to a three o'clock position, to end_angle, within the bounding
 rectangle specified by the parameters point, width and height.
 The angles are specified in degrees.
 For example to draw a circle or ellipse specify a start_angle of zero
 and an end_angle of 360.
 Uses symbol type to determine the colour
---------------------------------------------------------------*/
void
  CanvasFillArc(
    Canvas canvas, 
    const CanvasCoord point,
    const unsigned width, 
    const unsigned height,
    const int start_angle, 
    const int end_angle,
	int symbol_type);


/*---------------------------------------------------------------
 Routine : CanvasDrawArc
 Purpose : Draws an arc starting from the start_angle, relative 
 to a three o'clock position, to end_angle, within the bounding 
 rectangle specified by the parameters point, width and height.
 The angles are specified in degrees.
 For example to draw a circle or ellipse specify a start_angle of zero
 and an end_angle of 360.
---------------------------------------------------------------*/
extern void        
  CanvasDrawArc(
    Canvas canvas, 
    const CanvasCoord point,
    const unsigned width, 
    const unsigned height,
    const int start_angle, 
    const int end_angle);

/*---------------------------------------------------------------
 Routine : CanvasLineWidth
 Purpose : Returns the line width of the canvas.
---------------------------------------------------------------*/
extern unsigned        
  CanvasLineWidth(const Canvas canvas);

/*---------------------------------------------------------------
 Routine : CanvasTextWidth
 Purpose : Returns the text width of the canvas.
---------------------------------------------------------------*/
extern unsigned        
  CanvasTextWidth(
    const Canvas canvas, 
    const char *text);

/*---------------------------------------------------------------
 Routine : CanvasFontHeight
 Purpose : Returns the font height of the canvas.
---------------------------------------------------------------*/
extern unsigned        
  CanvasFontHeight(const Canvas canvas);

#endif
