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
 
 SccsId :@(#)symbol.c	1.3 11/22/96
 
 Purpose: This module defines and manipulates the SYMBOL structure.
****************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include "symbol.h"
/* #include "fta.h" */
#include "symbol_defs.h"

#include "MemoryUtil.h"

#include "AssertUtil.h"

ASSERTFILE( __FILE__ )


/*--------------------------------------------------------------
 Routine : scale_segment
 Purpose : Scales the segment.
           Creates and returns the "scaled" segment.
           Space is allocated to hold the scaled segment.
---------------------------------------------------------------*/
static SEGMENT *scale_segment(SEGMENT *seg_list, float scale);

/*--------------------------------------------------------------
 Routine : scale_arc
 Purpose : Scales the arc segment.
           Creates and returns the "scaled" arc segment.
           Space is allocated to hold the scaled arc segment.
---------------------------------------------------------------*/
static ARC *scale_arc(ARC *arc, float scale);

/*--------------------------------------------------------------
 Routine : scale_rectangle
 Purpose : Scales the rectangle segment.
           Creates and returns the "scaled" rectangle segment.
           Space is allocated to hold the scaled rectangle segment.
---------------------------------------------------------------*/
static RECTANGLE *scale_rectangle(RECTANGLE *rect, float scale);

/*--------------------------------------------------------------
 Routine : scale_polyline
 Purpose : Scales the polyline segment.
           Creates and returns the "scaled" polyline segment.
           Space is allocated to hold the scaled polyline segment.
---------------------------------------------------------------*/
static POLYLINE *scale_polyline(POLYLINE *poly, float scale);

/*--------------------------------------------------------------
 Routine : scale_line
 Purpose : Scales the line segment.
           Creates and returns the "scaled" line segment.
           Space is allocated to hold the scaled line segment.
---------------------------------------------------------------*/
static LINE *scale_line(LINE *line, float scale);


/*--------------------------------------------------------------
 Routine : scale_segment
 Purpose : Scales the segment.
           Creates and returns the "scaled" segment.
           Space is allocated to hold the scaled segment.
---------------------------------------------------------------*/
static SEGMENT *scale_segment(SEGMENT *seg_list, float scale);



/*--------------------------------------------------------------
 Routine : scale_symbol
 Purpose : Scales the symbol.
           Creates and returns the "scaled" symbol.
           Space is allocated to hold the scaled symbol.
---------------------------------------------------------------*/
SYMBOL *
scale_symbol(SYMBOL *symbol, float scale)
{
    SYMBOL        *new_symbol;
    SEGMENT        *new_seg, *seg_list;

    if ( !fNewMemory( (void *)&new_symbol, sizeof( SYMBOL ) ) )
    {
      printf("\n*** scale_symbol : malloc failed ***\n");
      exit(1);
    }
    new_symbol->type = symbol->type;
    new_symbol->depth = symbol->depth * (float)scale;
    new_symbol->width = symbol->width * (float)scale;
    new_symbol->textdepth = symbol->textdepth * (float)scale;
    new_symbol->textwidth = symbol->textwidth * (float)scale;
	new_symbol->text_lines_allowed = symbol->text_lines_allowed;
    seg_list = symbol->segment_list;
    new_symbol->segment_list = scale_segment(seg_list, scale);
    new_seg = new_symbol->segment_list;
    seg_list = seg_list->next;
    while (seg_list != NULL) {
        new_seg->next = scale_segment(seg_list, scale);
        new_seg = new_seg->next;
        seg_list = seg_list->next;
    }
    return new_symbol;
}

/*--------------------------------------------------------------
 Routine : draw_symbol
 Purpose : Draws the symbol.
---------------------------------------------------------------*/
void
draw_symbol(Canvas canvas, CanvasCoord symbol_centre, SYMBOL *symbol)
{
    SEGMENT     *seg_list;
    ARC         *arc;
    RECTANGLE   *rect;
    POLYLINE    *poly;
    LINE        *line;

    ASSERT( symbol != NULL );

    seg_list = symbol->segment_list;
    while (seg_list != NULL) {
        /* Identify and draw the segment type */
        switch (seg_list->type) {
        case CLEAR_ARC_SEG :
            arc =  (ARC *)(seg_list->seg);
            clear_arc(canvas, arc, symbol_centre);
            break;
        case FILL_ARC_SEG :
            arc =  (ARC *)(seg_list->seg);
            fill_arc(canvas, arc, symbol_centre, symbol->type);
            break;
        case ARC_SEG :
            arc =  (ARC *)(seg_list->seg);
            draw_arc(canvas, arc, symbol_centre);
            break;
        case RECTANGLE_SEG :
            rect =  (RECTANGLE *)(seg_list->seg);
            draw_rectangle(canvas, rect, symbol_centre);
            break;
        case FILL_RECTANGLE_SEG :
            rect =  (RECTANGLE *)(seg_list->seg);
            fill_rectangle(canvas, rect, symbol_centre, symbol->type);
            break;
		case FILL_POLYLINE_SEG :
            poly = (POLYLINE *)(seg_list->seg);
            fill_polyline(canvas, poly, symbol_centre, symbol->type);
            break;
		case POLYLINE_SEG :
            poly =  (POLYLINE *)(seg_list->seg);
            draw_polyline(canvas, poly, symbol_centre);
            break;
        case LINE_SEG :
            line =  (LINE *)(seg_list->seg);
            draw_line(canvas, line, symbol_centre);
            break;
        default:
            printf("\n*** draw_symbol : Invalid segment type ***\n\n");
            exit(1);
        }
        seg_list = seg_list->next;
    }
}


/*--------------------------------------------------------------
 Routine : draw_symbol_small
 Purpose :
---------------------------------------------------------------*/
void
draw_symbol_small(Canvas canvas, CanvasCoord symbol_centre, SYMBOL *symbol)
{
/*     SEGMENT        *seg_list; */
/*     ARC                *arc; */
/*     RECTANGLE        *rect; */
/*     POLYLINE        *poly; */
/*     LINE        *line; */
    SYMBOL *small_symbol;

    ASSERT( symbol != NULL );

    small_symbol = scale_symbol(symbol, 1.1);
    symbol_centre.y += (symbol->depth - small_symbol->depth)/2.0;
    draw_symbol(canvas, symbol_centre, small_symbol);
    delete_symbol(small_symbol);
}




/*--------------------------------------------------------------
 Routine : delete_symbol
 Purpose : Deletes the symbol.
           All space allocated to hold the symbol is returned.
---------------------------------------------------------------*/
void
delete_symbol(SYMBOL *symbol)
{
    SEGMENT        *seg_list, *seg_list_save;

    ASSERT ( symbol != NULL );

    seg_list = symbol->segment_list;
    while (seg_list != NULL) {

      switch (seg_list->type) {
      case CLEAR_ARC_SEG :
      case FILL_ARC_SEG :
      case ARC_SEG :
        break;
      case FILL_RECTANGLE_SEG :
      case RECTANGLE_SEG :
        break;
      case FILL_POLYLINE_SEG :
      case POLYLINE_SEG :
        {
          POLYLINE *poly = (POLYLINE *)(seg_list->seg);

          FreeMemory( poly->points ); 
          break;
        }
      case LINE_SEG :
        break;
      default:
        printf("\n*** delete_symbol : Invalid symbol type ***\n\n");
        exit(1);
      }
      FreeMemory(seg_list->seg); 
      seg_list_save = seg_list;
      seg_list = seg_list->next;
      FreeMemory(seg_list_save); 
    }
    FreeMemory(symbol); 

} /* delete_symbol */



/*--------------------------------------------------------------
 Routine : scale_segment
 Purpose : Scales the segment.
           Creates and returns the "scaled" segment.
           Space is allocated to hold the scaled segment.
---------------------------------------------------------------*/
static SEGMENT *
scale_segment(SEGMENT *seg_list, float scale)
{
    SEGMENT        *new_seg;

/*     ASSERTMSG(      */
/*       fValidPointer( seg_list, sizeof( SEGMENT ) ),     */
/*       "Segment list ptr not valid" );     */

/*     if ((new_seg = (SEGMENT *)malloc(sizeof(SEGMENT))) == NULL) { */
/*         printf("\n*** scale_segment : malloc failed ***\n"); */
/*         exit(1); */
/*     } */
    if ( !fNewMemory( (void *)&new_seg, sizeof( SEGMENT ) ) )
    {
      printf("\n*** scale_segment : malloc failed ***\n");
      exit(1);
    }
    new_seg->next = NULL;
    new_seg->type = seg_list->type;
/*     new_seg->seg = seg_list->seg;  */
    switch (seg_list->type) {
    case CLEAR_ARC_SEG :
    case FILL_ARC_SEG :
    case ARC_SEG :
        new_seg->seg = (void *)scale_arc(seg_list->seg, scale);
        break;
    case RECTANGLE_SEG :
	case FILL_RECTANGLE_SEG:
        new_seg->seg = (void *)scale_rectangle(seg_list->seg, scale);
        break;
    case FILL_POLYLINE_SEG :
    case POLYLINE_SEG :
        new_seg->seg = (void *)scale_polyline(seg_list->seg, scale);
        break;
    case LINE_SEG :
        new_seg->seg = (void *)scale_line(seg_list->seg, scale);
        break;
    default:
        printf("\n*** scale_segment : Invalid symbol type ***\n\n");
        exit(1);
    }
    return new_seg;
}


/*--------------------------------------------------------------
 Routine : scale_polyline
 Purpose : Scales the polyline segment.
           Creates and returns the "scaled" polyline segment.
           Space is allocated to hold the scaled polyline segment.
---------------------------------------------------------------*/
static POLYLINE *
scale_polyline(POLYLINE *poly, float scale)
{
    POLYLINE        *new_poly;
    int                i;
    FTA_POINT           *points;

/*     if ((new_poly = (POLYLINE *)malloc(sizeof(POLYLINE))) == NULL) { */
/*         printf("\n*** scale_polyline : malloc failed ***\n"); */
/*         exit(1); */
/*     } */
    if ( !fNewMemory( (void *)&new_poly, sizeof( POLYLINE ) ) )
    {
      printf("\n*** scale_polyline : malloc failed ***\n");
      exit(1);
    }
/*     if ((new_poly->points =    */
/*                 (POINT *)malloc(poly->num_points*sizeof(POINT))) == NULL) {    */
/*         printf("\n*** scale_polyline : malloc failed ***\n");    */
/*         exit(1);    */
/*     }    */
    if (  
      !fNewMemory(  
        (void *)&points,  
        ( poly->num_points * sizeof( FTA_POINT ) ) ) ) 
    {  
      printf("\n*** scale_polyline : malloc failed ***\n");  
      exit(1);  
    }  
    new_poly->points = points;
    new_poly->num_points = poly->num_points;
    for (i = 0; i < poly->num_points; i++) {
        new_poly->points[i].x = poly->points[i].x * scale;
        new_poly->points[i].y = poly->points[i].y * scale;
    }

    return new_poly;
}

/*--------------------------------------------------------------
 Routine : scale_line
 Purpose : Scales the line segment.
           Creates and returns the "scaled" line segment.
           Space is allocated to hold the scaled line segment.
---------------------------------------------------------------*/
static LINE *
scale_line(LINE *line, float scale)
{
    LINE        *new_line;

/*     if ((new_line = (LINE *)malloc(sizeof(LINE))) == NULL) { */
/*         printf("\n*** scale_line : malloc failed ***\n"); */
/*         exit(1); */
/*     } */
    if ( !fNewMemory( (void *)&new_line, sizeof( LINE ) ) )
    {
      printf("\n*** scale_line : malloc failed ***\n");
      exit(1);
    }
    new_line->start.x = line->start.x * scale;
    new_line->start.y = line->start.y * scale;
    new_line->end.x = line->end.x * scale;
    new_line->end.y = line->end.y * scale;
    return new_line;
}

/*--------------------------------------------------------------
 Routine : scale_arc
 Purpose : Scales the arc segment.
           Creates and returns the "scaled" arc segment.
           Space is allocated to hold the scaled arc segment.
---------------------------------------------------------------*/
static ARC *
scale_arc(ARC *arc, float scale)
{
    ARC                *new_arc;

/*     if ((new_arc = (ARC *)malloc(sizeof(ARC))) == NULL) { */
/*         printf("\n*** scale_arc : malloc failed ***\n"); */
/*         exit(1); */
/*     } */
    if ( !fNewMemory( (void *)&new_arc, sizeof( ARC ) ) )
    {
      printf("\n*** scale_arc : malloc failed ***\n");
      exit(1);
    }
    new_arc->point.x = arc->point.x * scale;
    new_arc->point.y = arc->point.y * scale;
    new_arc->width = arc->width * scale;
    new_arc->height = arc->height * scale;
    new_arc->start_angle = arc->start_angle;
    new_arc->end_angle = arc->end_angle;
    return new_arc;
}

/*--------------------------------------------------------------
 Routine : scale_rectangle
 Purpose : Scales the rectangle segment.
           Creates and returns the "scaled" rectangle segment.
           Space is allocated to hold the scaled rectangle segment.
---------------------------------------------------------------*/
static RECTANGLE *
scale_rectangle(RECTANGLE *rect, float scale)
{
    RECTANGLE        *new_rect;

/*     if ((new_rect = (RECTANGLE *)malloc(sizeof(RECTANGLE))) == NULL) { */
/*         printf("\n*** scale_rectangle : malloc failed ***\n"); */
/*         exit(1); */
/*     } */
    if ( !fNewMemory( (void *)&new_rect, sizeof( RECTANGLE ) ) )
    {
      printf("\n*** scale_rectangle : malloc failed ***\n");
      exit(1);
    }
    new_rect->point.x = rect->point.x * scale;
    new_rect->point.y = rect->point.y * scale;
    new_rect->width = rect->width * scale;
    new_rect->height = rect->height * scale;
    return new_rect;
}



/*--------------------------------------------------------------
 Routine : is_point_in_symbol
 Purpose : Checks if the difference of two point is in the boundaries of the symbol.
           The difference is passed to this function as a set of x and y values
           (x_diff and y_diff).
---------------------------------------------------------------*/
BOOL
is_point_in_symbol(SYMBOL *symbol, int x_diff, int y_diff)
{
    return (abs(x_diff) <= (int)(symbol->width/2.0) &&
        y_diff >= 0 &&
        y_diff <= symbol->depth);
}
