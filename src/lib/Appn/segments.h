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
 Module : Segments
 Author : FSC Limited
 Date   : 29/11/95
 
 SccsId :@(#)segments.h	1.1 08/22/96

Origin : FTA project,
         FSC Limited Private Venture, Level 2 Prototype.

 Purpose: This module draws the different segments used to build the
          twelve FTA basic symbols. 
****************************************************************/

#ifndef Segments_h
#define Segments_h

#include "Canvas.h"


typedef struct point{
	float	x, y;
}FTA_POINT;


typedef struct line{
	FTA_POINT	start, end;
}LINE;


/*--------------------------------------------------------------
 This type provides access to the individual components of a polyline.
 The polyline is multiple connected lines and is defined by an array of
 points and the total number of points.
---------------------------------------------------------------*/
typedef struct polyline{
	int	num_points;
	FTA_POINT	*points;
}POLYLINE;


/*--------------------------------------------------------------
 This type provides access to the individual components of an arc.
 The arc is defined by drawing from the start_angle, relative to a
 three o'clock position, to end_angle, within the bounding rectangle
 specified by the parameters point, width and height.

 Point is the top left corner of the rectangle, as per the X convention
 The angles are specified in degrees.
---------------------------------------------------------------*/ 
typedef struct arc{
	FTA_POINT	point;
	float	width, height;
	int	start_angle, end_angle;
}ARC;


typedef struct rectangle{
	FTA_POINT	point;
	float	width, height;
}RECTANGLE;


typedef enum segment_type{
	LINE_SEG, FILL_POLYLINE_SEG, POLYLINE_SEG, CLEAR_ARC_SEG, FILL_ARC_SEG, ARC_SEG, FILL_RECTANGLE_SEG, RECTANGLE_SEG
}SEGMENT_TYPE;


typedef struct segment{
	struct segment	*next;
	SEGMENT_TYPE	type;
	void		*seg;
}SEGMENT;


/*--------------------------------------------------------------
 Routine : clear_arc
 Purpose : Clears an ARC segment on the canvas using the background colour.
---------------------------------------------------------------*/
void
clear_arc(Canvas canvas, ARC *arc, CanvasCoord symbol_centre);


/*--------------------------------------------------------------
 Routine : fill_arc
 Purpose : Draws a filled ARC segment on the canvas.
           Uses the symbol type to determine the colour.
---------------------------------------------------------------*/
extern void 
  fill_arc(
    Canvas canvas, 
    ARC *arc, 
    CanvasCoord symbol_centre,
	int symbol_type);


/*--------------------------------------------------------------
 Routine : draw_arc
 Purpose : Draws an ARC segment on the canvas.
---------------------------------------------------------------*/
extern void 
  draw_arc(
    Canvas canvas, 
    ARC *arc, 
    CanvasCoord symbol_centre);


/*--------------------------------------------------------------
 Routine : fill_rectangle 
 Purpose : Draws a filled rectangle on the canvas.
           Uses the symbol type to determine the colour.
---------------------------------------------------------------*/
extern void 
  fill_rectangle(
    Canvas canvas, 
    RECTANGLE *rect, 
    CanvasCoord symbol_centre,
	int symbol_type);


/*--------------------------------------------------------------
 Routine : draw_rectangle 
 Purpose : Draws a rectangle on the canvas.
---------------------------------------------------------------*/
extern void 
  draw_rectangle(
    Canvas canvas, 
    RECTANGLE *rect, 
    CanvasCoord symbol_centre);

/*--------------------------------------------------------------
 Routine : fill_polyline
 Purpose : Draws a filled polyline on the canvas.
           using the symbol type to determine the colour.
---------------------------------------------------------------*/
void
fill_polyline(Canvas canvas, POLYLINE *poly, CanvasCoord symbol_centre, int symbol_type);

/*--------------------------------------------------------------
 Routine : draw_polyline
 Purpose : Draws a polyline on the canvas.
---------------------------------------------------------------*/
extern void 
  draw_polyline(
    Canvas canvas, 
    POLYLINE *poly, 
    CanvasCoord symbol_centre);

/*--------------------------------------------------------------
 Routine : draw_line
 Purpose : Draws a line on the canvas.
---------------------------------------------------------------*/
extern void 
  draw_line(
    Canvas canvas, 
    LINE *line, 
    CanvasCoord symbol_centre);

#endif

