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
 
 SccsId :@(#)segments.c	1.2 09/26/96
 
Origin : FTA project,
         FSC Limited Private Venture, Level 2 Prototype.
 
 Purpose: This module draws the different segments used to build the
          twelve FTA basic symbols.
****************************************************************/

#include <stdlib.h>
#include <stdio.h>

#include "Canvas.h"

#include "segments.h"
/* #include "fta.h" /* Imports NULL */ 

#include "MemoryUtil.h"


/*--------------------------------------------------------------
 Routine : clear_arc
 Purpose : Clears an ARC segment on the canvas using the background colour.
---------------------------------------------------------------*/
void
clear_arc(Canvas canvas, ARC *arc, CanvasCoord symbol_centre)
{
    CanvasCoord                point;

    point.x = symbol_centre.x + (unsigned)arc->point.x;
    point.y = symbol_centre.y + (unsigned)arc->point.y;
    CanvasClearArc(
      canvas, 
      point, 
      (int)arc->width, 
      (int)arc->height,
      arc->start_angle, 
      arc->end_angle);
}



/*--------------------------------------------------------------
 Routine : fill_arc
 Purpose : Draws a filled ARC segment on the canvas.
           using the symbol type to determine the colour.
---------------------------------------------------------------*/
void
fill_arc(Canvas canvas, ARC *arc, CanvasCoord symbol_centre, int symbol_type)
{
    CanvasCoord                point;

    point.x = symbol_centre.x + (unsigned)arc->point.x;
    point.y = symbol_centre.y + (unsigned)arc->point.y;
    CanvasFillArc(
      canvas, 
      point, 
      (int)arc->width, 
      (int)arc->height,
      arc->start_angle, 
      arc->end_angle,
	  symbol_type);
}


/*--------------------------------------------------------------
 Routine : draw_arc
 Purpose : Draws an ARC segment on the canvas.
---------------------------------------------------------------*/
void
draw_arc(Canvas canvas, ARC *arc, CanvasCoord symbol_centre)
{
    CanvasCoord                point;

    point.x = symbol_centre.x + (unsigned)arc->point.x;
    point.y = symbol_centre.y + (unsigned)arc->point.y;
    CanvasDrawArc(
      canvas, 
      point, 
      (int)arc->width, 
      (int)arc->height,
      arc->start_angle, 
      arc->end_angle);
}


/*--------------------------------------------------------------
 Routine : fill_rectangle
 Purpose : Draws a filled rectangle on the canvas.
           using the symbol type to determine the colour.
---------------------------------------------------------------*/
void
fill_rectangle(Canvas canvas, RECTANGLE *rect, CanvasCoord symbol_centre, int symbol_type)
{
    CanvasCoord                point;

    point.x = symbol_centre.x + (unsigned)rect->point.x;
    point.y = symbol_centre.y + (unsigned)rect->point.y;
    CanvasFillRectangle(
      canvas, 
      point, 
      (int)rect->width, 
      (int)rect->height,
	  symbol_type);
}


/*--------------------------------------------------------------
 Routine : draw_rectangle
 Purpose : Draws a rectangle on the canvas.
---------------------------------------------------------------*/
void
draw_rectangle(Canvas canvas, RECTANGLE *rect, CanvasCoord symbol_centre)
{
    CanvasCoord                point;

    point.x = symbol_centre.x + (unsigned)rect->point.x;
    point.y = symbol_centre.y + (unsigned)rect->point.y;
    CanvasDrawRectangle(
      canvas, 
      point, 
      (int)rect->width, 
      (int)rect->height);
}


/*--------------------------------------------------------------
 Routine : fill_polyline
 Purpose : Draws a filled polyline on the canvas.
           using the symbol type to determine the colour.
---------------------------------------------------------------*/
void
fill_polyline(Canvas canvas, POLYLINE *poly, CanvasCoord symbol_centre, int symbol_type)
{
    CanvasCoord                *points;
    int                        i;

    if ( !fNewMemory( (void *)&points,
                ( poly->num_points * sizeof( CanvasCoord ) ) ) )
    {
        printf("\n*** fill_polyline : malloc failed ***\n");
        exit(1);
    }
    for (i = 0; i < poly->num_points; i++) {
        points[i].x = (unsigned)poly->points[i].x + symbol_centre.x;
        points[i].y = (unsigned)poly->points[i].y + symbol_centre.y;
    }
    CanvasFillPolyline(canvas, poly->num_points, points, symbol_type);
    FreeMemory(points);
}


/*--------------------------------------------------------------
 Routine : draw_polyline
 Purpose : Draws a polyline on the canvas.
---------------------------------------------------------------*/
void
draw_polyline(Canvas canvas, POLYLINE *poly, CanvasCoord symbol_centre)
{
    CanvasCoord                *points;
    int                        i;

/*     if ((points = */
/*             (CanvasCoord *) */
/*                 malloc(poly->num_points*sizeof(CanvasCoord))) == NULL) { */
    if ( !fNewMemory( (void *)&points,
                ( poly->num_points * sizeof( CanvasCoord ) ) ) )
    {
        printf("\n*** draw_polyline : malloc failed ***\n");
        exit(1);
    }
    for (i = 0; i < poly->num_points; i++) {
        points[i].x = (unsigned)poly->points[i].x + symbol_centre.x;
        points[i].y = (unsigned)poly->points[i].y + symbol_centre.y;
    }
    CanvasDrawPolyline(canvas, poly->num_points, points);
    FreeMemory(points);
}

/*--------------------------------------------------------------
 Routine : draw_line
 Purpose : Draws a line on the canvas.
---------------------------------------------------------------*/
void
draw_line(Canvas canvas, LINE *line, CanvasCoord symbol_centre)
{
    CanvasCoord                start, end;

    start.x = symbol_centre.x + (unsigned)line->start.x;
    start.y = symbol_centre.y + (unsigned)line->start.y;
    end.x = symbol_centre.x + (unsigned)line->end.x;
    end.y = symbol_centre.y + (unsigned)line->end.y;
    CanvasDrawLine(canvas, start, end);
}





