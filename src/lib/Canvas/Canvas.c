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
 
 SccsId : @(#)Canvas.c	1.2 09/26/96
 
 Origin : FTA project,
          FSC Limited Private Venture.
 
 This module implements the canvas.
***************************************************************/

#include <stdlib.h>
#include <stdio.h>

#include "CanvasP.h"
#include "Canvas.h"

#include "MemoryUtil.h"

/*---------------------------------------------------------------
 Routine : CanvasCreate
 Purpose : Creates a canvas object. Allocates memory to hold the
 Canvas object.
---------------------------------------------------------------*/
Canvas
  CanvasCreate()
{
    Canvas c;

/*     if ( (c = (Canvas)malloc(sizeof(Canvas_rec))) == NULL )  */
    if ( !fNewMemory( (void **)&c, sizeof(Canvas_rec) ) ) 
    {
        printf("\n*** CanvasCreate : malloc failed ***\n");
        exit(1);
    }
    return c;

} /* CanvasCreate */

/*---------------------------------------------------------------
 Routine : CanvasDestroy
 Purpose : Destroys a canvas area.
 
 All space allocated to hold the canvas is released.
---------------------------------------------------------------*/
void
  CanvasDestroy(Canvas canvas)
{
    switch (canvas->type) {
        case X_CANVAS:
            /* XCanvasDestroy(canvas); */
            break;
        case POST_SCRIPT_CANVAS:
            /* PostScriptCanvasDestroy(canvas); */
            break;
         case JAVA_CANVAS:
            /* JavaCanvasDestroy(canvas); */
            break;
		 default:
            /* No action */
            break;
    }
    FreeMemory(canvas);

} /* CanvasDestroy */

/*---------------------------------------------------------------
 Routine : CanvasSetLineWidth
 Purpose : Sets the line width for the canvas area.
---------------------------------------------------------------*/
void
  CanvasSetLineWidth(
    Canvas canvas, 
    const unsigned line_width)
{
    switch (canvas->type) {
        case X_CANVAS:
            /* XCanvasSetLineWidth(canvas, line_width); */
            break;
        case POST_SCRIPT_CANVAS:
            /* PostScriptCanvasSetLineWidth(canvas, line_width); */
            break;
        case JAVA_CANVAS:
            JavaCanvasSetLineWidth(canvas, line_width);
            break;
		default:
            /* No action */
            break;
    }
} /* CanvasSetLineWidth */

/*---------------------------------------------------------------
 Routine : CanvasWriteText
 Purpose : Writes the given text from the specified start point
 on the canvas area.
---------------------------------------------------------------*/
void
  CanvasWriteText(
    Canvas canvas, 
    const char *text, 
    const CanvasCoord start)
{
    switch (canvas->type) {
        case X_CANVAS:
            /* XCanvasWriteText(canvas, text, start); */
            break;
        case POST_SCRIPT_CANVAS:
            /* PostScriptCanvasWriteText(canvas, text, start); */
            break;
        case JAVA_CANVAS:
            JavaCanvasWriteText(canvas, text, start);
            break;
        default:
            /* No action */
            break;
    }
} /* CanvasWriteText */

/*---------------------------------------------------------------
 Routine : CanvasSetFont
 Purpose : Sets the font style for the canvas area.
---------------------------------------------------------------*/
void
  CanvasSetFont(
    Canvas canvas, 
    const char *font_string)
{
    switch (canvas->type) {
        case X_CANVAS:
            /* XCanvasSetFont(canvas, font_string); */
            break;
        case POST_SCRIPT_CANVAS:
            /* PostScriptCanvasSetFont(canvas, font_string); */
            break;
		case JAVA_CANVAS:
            JavaCanvasSetFont(canvas, font_string);
            break;
		default:
            /* No action */
            break;
    }
} /* CanvasSetFont */

/*---------------------------------------------------------------
 Routine : CanvasDrawLine
 Purpose : Draws a line from the specified start point to the end
 point on the canvas area.
---------------------------------------------------------------*/
void
  CanvasDrawLine(
    Canvas canvas, 
    const CanvasCoord start, 
    const CanvasCoord end)
{
    switch (canvas->type) {
        case X_CANVAS:
            /* XCanvasDrawLine(canvas, start, end); */
            break;
        case POST_SCRIPT_CANVAS:
            /* PostScriptCanvasDrawLine(canvas, start, end); */
            break;
        case JAVA_CANVAS:
            JavaCanvasDrawLine(canvas, start, end);
            break;
        default:
            /* No action */
            break;
    }
}  /* CanvasDrawLine */


/*---------------------------------------------------------------
 Routine : CanvasFILLRectangle
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
	int symbol_type)
{
    switch (canvas->type) {
        case X_CANVAS:
            /* XCanvasDrawRectangle(canvas, point, width, height); */
            break;
         case POST_SCRIPT_CANVAS:
            /* PostScriptCanvasDrawRectangle(canvas, point, width, height); */
            break;
		 case JAVA_CANVAS:
            JavaCanvasFillRectangle(canvas, point, width, height, symbol_type);
            break;
         default:
            /* No action */
            break;
    }
} /* CanvasFillRectangle */


/*---------------------------------------------------------------
 Routine : CanvasDrawRectangle
 Purpose : Draws a rectangle from the specified point with the
 specified width and height on the canvas area.
---------------------------------------------------------------*/
void
  CanvasDrawRectangle(
    Canvas canvas, 
    const CanvasCoord point,
    const unsigned width, 
    const unsigned height)
{
    switch (canvas->type) {
        case X_CANVAS:
            /* XCanvasDrawRectangle(canvas, point, width, height); */
            break;
         case POST_SCRIPT_CANVAS:
            /* PostScriptCanvasDrawRectangle(canvas, point, width, height); */
            break;
		 case JAVA_CANVAS:
            JavaCanvasDrawRectangle(canvas, point, width, height);
            break;
         default:
            /* No action */
            break;
    }
} /* CanvasDrawRectangle */


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
	int symbol_type)
{
    switch (canvas->type) {
        case X_CANVAS:
            /* XCanvasDrawPolyline(canvas, num_points, points); */
            break;
        case POST_SCRIPT_CANVAS:
            /* PostScriptCanvasDrawPolyline(canvas, num_points, points); */
            break;
        case JAVA_CANVAS:
            JavaCanvasFillPolyline(canvas, num_points, points, symbol_type);
            break;
        default:
            /* No action */
            break;
    }
} /* CanvasFillPolyline */


/*---------------------------------------------------------------
 Routine : CanvasDrawPolyline
 Purpose : Draws a polyline on the canvas area. The polyline is
 multiple connected lines and is defined by an array of points
 and the number of points.
---------------------------------------------------------------*/
void
  CanvasDrawPolyline(
    Canvas canvas, 
    const unsigned num_points, 
    const CanvasCoord *points)
{
    switch (canvas->type) {
        case X_CANVAS:
            /* XCanvasDrawPolyline(canvas, num_points, points); */
            break;
        case POST_SCRIPT_CANVAS:
            /* PostScriptCanvasDrawPolyline(canvas, num_points, points); */
            break;
        case JAVA_CANVAS:
            JavaCanvasDrawPolyline(canvas, num_points, points);
            break;
        default:
            /* No action */
            break;
    }
} /* CanvasDrawPolyline */


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
    const int end_angle)
{
    switch (canvas->type) {
        case X_CANVAS:
            /* XCanvasDrawArc(
                canvas, point, width, height, start_angle, end_angle); */
            break;
        case POST_SCRIPT_CANVAS:
            /* PostScriptCanvasDrawArc(
                canvas, point, width, height, start_angle, end_angle); */
            break;
        case JAVA_CANVAS:
            JavaCanvasClearArc(
                canvas, point, width, height, start_angle, end_angle);
            break;
        default:
            /* No action */
            break;
    }
} /* CanvasClearArc */


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
	int symbol_type)
{
    switch (canvas->type) {
        case X_CANVAS:
            /* XCanvasDrawArc(
                canvas, point, width, height, start_angle, end_angle); */
            break;
        case POST_SCRIPT_CANVAS:
            /* PostScriptCanvasDrawArc(
                canvas, point, width, height, start_angle, end_angle); */
            break;
        case JAVA_CANVAS:
            JavaCanvasFillArc(
                canvas, point, width, height, start_angle, end_angle, symbol_type);
            break;
        default:
            /* No action */
            break;
    }
} /* CanvasDrawArc */


/*---------------------------------------------------------------
 Routine : CanvasDrawArc
 Purpose : Draws an arc starting from the start_angle, relative
 to a three o'clock position, to end_angle, within the bounding
 rectangle specified by the parameters point, width and height.
 The angles are specified in degrees.
 For example to draw a circle or ellipse specify a start_angle of zero
 and an end_angle of 360.
---------------------------------------------------------------*/
void
  CanvasDrawArc(
    Canvas canvas, 
    const CanvasCoord point,
    const unsigned width, 
    const unsigned height,
    const int start_angle, 
    const int end_angle)
{
    switch (canvas->type) {
        case X_CANVAS:
            /* XCanvasDrawArc(
                canvas, point, width, height, start_angle, end_angle); */
            break;
        case POST_SCRIPT_CANVAS:
            /* PostScriptCanvasDrawArc(
                canvas, point, width, height, start_angle, end_angle); */
            break;
        case JAVA_CANVAS:
            JavaCanvasDrawArc(
                canvas, point, width, height, start_angle, end_angle);
            break;
        default:
            /* No action */
            break;
    }
} /* CanvasDrawArc */

/*---------------------------------------------------------------
 Routine : CanvasLineWidth
 Purpose : Returns the line width of the canvas.
---------------------------------------------------------------*/
unsigned
  CanvasLineWidth(
    const Canvas canvas)
{
    unsigned        line_width;

    switch (canvas->type) {
        case X_CANVAS:
            /* line_width = XCanvasLineWidth(canvas); */
            break;
        case POST_SCRIPT_CANVAS:
            break;
        case JAVA_CANVAS:
             line_width = JavaCanvasLineWidth(canvas);
            break;
        default:
            /* No action */
            break;
    }
    return line_width;

} /* CanvasLineWidth */

/*---------------------------------------------------------------
 Routine : CanvasTextWidth
 Purpose : Returns the text width of the canvas.
---------------------------------------------------------------*/
unsigned
  CanvasTextWidth(
    const Canvas canvas, 
    const char *text)
{
    unsigned        text_width;

    switch (canvas->type) {
        case X_CANVAS:
            /* text_width = XCanvasTextWidth(canvas, text); */
            break;
        case POST_SCRIPT_CANVAS:

         /* TEMPORARY HACK - DGW */
/*             printf("CanvasTextWidth : TEMPORARY HACK - DGW\n"); */
            /* text_width = PostScriptCanvasTextWidth(canvas, text); */

            break;
       case JAVA_CANVAS:
            text_width = JavaCanvasTextWidth(canvas, text);
            break;
        default:
            /* No action */
            break;
    }
    return text_width;

} /* CanvasTextWidth */

/*---------------------------------------------------------------
 Routine : CanvasFontHeight
 Purpose : Returns the font height of the canvas.
---------------------------------------------------------------*/
unsigned
  CanvasFontHeight(const Canvas canvas)
{
    unsigned        font_height;

    switch (canvas->type) {
        case X_CANVAS:
            /* font_height = XCanvasFontHeight(canvas); */
            break;
        case POST_SCRIPT_CANVAS:

         /* TEMPORARY HACK - DGW */
/*             printf("CanvasFontHeight : TEMPORARY HACK - DGW\n"); */
            /* font_height = PostScriptCanvasFontHeight(canvas); */

            break;
        case JAVA_CANVAS:
            font_height = JavaCanvasFontHeight(canvas);
            break;
        default:
            /* No action */
            break;
    }
    return font_height;

} /* CanvasFontHeight */

