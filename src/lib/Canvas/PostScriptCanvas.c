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
 Module : PostscriptCanvas
 Author : FSC Limited
 Date   : 5/8/96
 
 SccsId : @(#)PostScriptCanvas.c	1.5 12/16/96
 
 Origin : FTA project,
          FSC Limited Private Venture.
 
 This module implements the Postscript canvas. In reality the 
 Canvas is a file of Postscript commands, but that need not 
 concern the user of the Canvas, to whom it behaves like any 
 other Canvas.
 
 A PostScript file is generated that, so far as possible, produces 
 a page identical to that seen on the screen.
 
 Note that the input coordinates are 'X' style (origin at the top left)
 whereas the output is PostScript (origin at bottom left). The macros
 X and Y convert between these coordinates.
 
 The original method scaled each individual object, and this facility
 is still present. However it is easier to draw the page in the
 units given, but scale the whole page using the PostScript "scale"
 command. This allows text to be drawn at a fixed size.
 
 The current method fits the whole tree on one page, reducing as much
 as necessary. Scale factors > 1.0 are not used. Portrait or Landscape
 are used as appropriate. To do this the max. and min. x and y
 coordinates are needed. This calculation is written into the
 X and Y macros. (So whenever a coordinate is calculated it is checked
 against the current max and min values). Thus the scale and translation
 are not known until all the PostScript drawing instructions have been
 written. For this reason the drawing instructions  are written into a
 PostScript function. Then the scale and translation factors are
 calculated and the PostScript instructions written, then the
 function is called.
 
 Encapsulated PostScript
 
 In order to comply with EPS format, appropriate %% comments are
 included at the top and bottom of the file. NOTE that the bounding
 box is no longer specified at the end of the file as Microsoft
 products do not recognise this format. The Bounding Box is in 
 DEFAULT PostScript coordinates. The line thickness must be taken 
 into account. A new dictionary is created to contain the definition 
 of doprint.

***************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "PostScriptCanvas.h"
#include "PostScriptCanvasP.h"
#include "CanvasP.h" 
#include "util.h"

#include "FileUtilities.h"

#include "MemoryUtil.h"

/* #include "Printers.h" */

#include "AssertUtil.h"
ASSERTFILE( __FILE__ )

#undef DRAW_BORDER 
/* #define DRAW_BORDER */

/* float ps_scale = 0.5; /* default value - actual value calculated in plot.c */

#define PSFONT      11   /* PostScript font size              */
#define LINETHICK   1    /* Default PostScript line thickness */

/*
 *  define macros to convert to PostScript coordinates
 */
#define PI        3.1415926535
#define X_SCALE   1.0     /* ps_scale            */
#define Y_SCALE   1.0     /* ps_scale            */

#define A4_WIDTH   594     /* 72 *  8.25   inches */
#define A4_HEIGHT  841     /* 72 * 11.6875 inches */

/* 
   For A2 printing:
   A2_WIDTH = A3_HEIGHT = A4_WIDTH * 2
   A2_HEIGHT = A3_WIDTH * 2 = A4_HEIGHT * 2

   For A3 printing:
   A3_WIDTH = A4_HEIGHT
   A3_HEIGHT = A4_WIDTH * 2

   For A5 printing:
   A5_WIDTH = A4_HEIGHT / 2
   A5_HEIGHT = A4_WIDTH 
*/

/* #define PAPERWIDTH   A4_WIDTH */
/* #define PAPERHEIGHT  A4_HEIGHT */

static int PAPERWIDTH = A4_WIDTH; /* A4 is default size */
static int PAPERHEIGHT = A4_HEIGHT;

#define XR( x )   ( (float)( (x) * X_SCALE) )               /* relative x */
#define YR( y )   ( (float)( (y) * Y_SCALE) )               /* relative y */

#undef UPSIDEDOWN
#ifdef UPSIDEDOWN
/* upside-down mode settings
 * (text does not work upside-down)
 */
    #define X( x )    ( (float)( PAPERWIDTH - XR(x) ) )         /* x coord    */
    #define Y( y )    ( (float)( YR(y) ) )                   /* y coord    */
    #define A( a )    ( (float)( (a) + 180) )                /* angle      */
#else
/* right way up mode settings */
    #define X( x )    ( xcheck( XR(x) ) )                   /* x coord    */
    #define Y( y )    ( ycheck( PAPERHEIGHT - YR(y) ) )        /* y coord    */
    #define A( a )    ( (float)( a ) )                      /* angle      */
#endif

/*
 * variables to hold max and min values of X and Y
 */
#define LARGE_REAL 100000.0
static float PSmaxX, PSminX, PSmaxY, PSminY;

/*---------------------------------------------------------------
 Routine : xcheck
 Purpose : Check the x value against the recorded maximum and 
 minimum set the recorded max and min values accordingly if the
 value entered requires them to change.
---------------------------------------------------------------*/
float
  xcheck( float xval )
{

    if ( xval > PSmaxX ) {
        PSmaxX = xval;
    } else if ( xval < PSminX ) {
        PSminX = xval;
    }
    return xval;

} /* xcheck */

/*---------------------------------------------------------------
 Routine : ycheck
 Purpose : Check the y value against the recorded maximum and 
 minimum set the recorded max and min values accordingly if the
 value entered requires them to change.
---------------------------------------------------------------*/
float
  ycheck( float yval )
{

    if ( yval > PSmaxY ) {
        PSmaxY = yval;
    } else if ( yval < PSminY ) {
        PSminY = yval;
    }
    return yval;

} /* ycheck */

/*---------------------------------------------------------------
 Routine : PostScriptCanvasCreate
 Purpose : Creates a Postscript Canvas. 

 Create canvas
 Print header to file
---------------------------------------------------------------*/
Canvas
  PostScriptCanvasCreate(
    FILE *file,            /* IN  - file for PostScript */
    const char *filename)  /* IN  - name of this file   */
{
    Canvas                canvas;
/*     unsigned long        foreground, background; */
    time_t file_date;

 /* initialise the min/max variables */
    PSmaxX = -LARGE_REAL; PSminX = LARGE_REAL;
    PSmaxY = -LARGE_REAL; PSminY = LARGE_REAL;

    canvas = CanvasCreate();
    canvas->type = POST_SCRIPT_CANVAS;
    canvas->Canvas_params.post_script.file = file;
    canvas->Canvas_params.post_script.filename = filename;

    file_date = date_of( canvas->Canvas_params.post_script.filename );

    /* Microsoft products insist that the BoundingBox comment is found
       in the header and cannot be deferred to the trailer. To perform
       this we have to pad out the area into which the BoundingBox
       values will be added, and hope that they will never overflow
       the padded area. If they underflow, an extra newline is left in
       the file, which is acceptable.
    */
    fprintf(file, "%%!PS-Adobe-3.0 EPSF-3.0\n"
        "%%%%BoundingBox:                                \n"
                  "%%%%Creator: Formal-FTA Print Tree\n"
                  "%%%%Title: (%s)\n"
                  "%%%%CreationDate: %s\n"
                  "%%%%EndComments\n"
                  "%%\n"
                  "1 dict begin\t%% Define new dictionary =>\n"
                  "%%-------- Define Procedures ---------\n"
                  "/doprint {\n",
                  canvas->Canvas_params.post_script.filename,
                  ctime( &file_date ) );

    return canvas;

} /* PostScriptCanvasCreate */

/*---------------------------------------------------------------
 Routine : PostScriptCanvasClose
 Purpose : Close the Postscript Canvas. This causes the final
 Postscript command to be sent to the file and closes the file.

 Print "showpage" command to file
 Close the canvas
---------------------------------------------------------------*/
void
  PostScriptCanvasClose(
    Canvas canvas)   /* IN  - PostScript canvas */
{
    float Hscale, Wscale, scale;  /* scale factors                        */
    float rot;                    /* rotation angle (degrees)             */
    float trans_x, trans_y;       /* translations in x, y directions      */
    float W, H;                   /* width and height subtracting margins */
    float width, height;          /* extent of actual tree (PS units)     */
    float DEFminX, DEFminY;       /* bl bounding box, default coordinates */
    float DEFmaxX, DEFmaxY;       /* tr bounding box, default coordinates */
    float line_thickness;         /* line thickness, default coordinates  */

    #define mT 50                 /* top margin               */
    #define mB 20                 /* bottom margin            */
    #define mL 20                 /* left margin              */
    #define mR 20                 /* right margin             */

    
#ifdef DRAW_BORDER
 /* draw in the margins */
    fprintf(canvas->Canvas_params.post_script.file,
        "newpath\t\t%% Draw border =>\n"
        "%5.3f %5.3f moveto\n"
        "%5.3f %5.3f lineto\n"
        "%5.3f %5.3f lineto\n"
        "%5.3f %5.3f lineto\n"
        "closepath\n"
        "stroke\n\n", PSminX - 10.0 , PSminY - 10.0,
                    PSminX - 10.0, PSmaxY + 10.0,
                    PSmaxX + 10.0, PSmaxY + 10.0,
                    PSmaxX + 10.0, PSminY - 10.0);
#endif

  switch ( printers_paper_size() )
  {
    case A3:
      PAPERWIDTH = A4_HEIGHT;
      PAPERHEIGHT = A4_WIDTH * 2;
      break;

    default:
    case A4:
      PAPERWIDTH = A4_WIDTH;
      PAPERHEIGHT = A4_HEIGHT;
      break;
  }

 /*
  * calculate the scale, translations and rotations
  */
  /*    if ( ((PSmaxY - PSminY) > (PSmaxX - PSminX)) ||    /* longer than wide    */
  /*       ((PSmaxX - PSminX) < PAPERWIDTH  - mL - mR) ) {  /* will fit no scaling */
  if ( printers_paper_orientation() == PORTRAIT )
    {
     /*
      * PORTRAIT
      */
        W = PAPERWIDTH  - mL - mR;
        H = PAPERHEIGHT - mT - mB;
        Hscale = MIN( H / (PSmaxY - PSminY), 1.0);
        Wscale = MIN( W / (PSmaxX - PSminX), 1.0);

        if (Hscale <= Wscale) {                   /* 'long' tree */
            scale = Hscale;
        } else {                                  /* 'wide' tree */
            scale = Wscale;
        }

        width   = (PSmaxX - PSminX) * scale;
        height  = (PSmaxY - PSminY) * scale;
        trans_x = (PAPERWIDTH - width)/2.0 - PSminX * scale;
        trans_y = mB + H - height - PSminY * scale;
        rot     = 0;

     /* calculate bounding box (in default coordinates) */
        DEFminX = mL + (W - width)/2.0;
        DEFminY = mB + H - height;
        DEFmaxX = DEFminX + width;
        DEFmaxY = DEFminY + height;

    } else {
     /*
      * LANDSCAPE
      */
        W = PAPERHEIGHT - mL - mR;
        H = PAPERWIDTH  - mT - mB;
        Hscale = MIN( H / (PSmaxY - PSminY), 1.0);
        Wscale = MIN( W / (PSmaxX - PSminX), 1.0);

        if (Hscale <= Wscale) {                   /* 'long' tree */
            scale = Hscale;
        } else {                                  /* 'wide' tree */
            scale = Wscale;
        }

        width   = (PSmaxX - PSminX) * scale;
        height  = (PSmaxY - PSminY) * scale;
        trans_x = mB + H - height - PSminY * scale;
        trans_y = (PAPERHEIGHT + width)/2.0 + PSminX * scale;
        rot     = -90;

     /* calculate bounding box (in default coordinates) */
        DEFminX = mB + H - height;
        DEFminY = mR + (W - width)/2.0;
        DEFmaxX = DEFminX + height;
        DEFmaxY = DEFminY + width;
    }

    line_thickness = LINETHICK * scale;

 /* print final section of file
  */

    fprintf(canvas->Canvas_params.post_script.file,
        "} def\t%% End doprint definition\n\n"
        "%5.3f %5.3f translate\n"
        "%5.3f rotate\n"
        "%5.3f %5.3f scale\n\n"
        "doprint\t%% Perform doprint\n"
        "showpage\t%% Draw picture on page\n"
        "end\t%% of dictionary\n\n"
        "%%%%EOF\n",
        trans_x, trans_y, rot, scale, scale );
     /* 0, 0, PAPERWIDTH, PAPERHEIGHT); */

    rewind( canvas->Canvas_params.post_script.file );
    fprintf(canvas->Canvas_params.post_script.file, 
        "%%!PS-Adobe-3.0 EPSF-3.0\n"
        "%%%%BoundingBox: %5.3f %5.3f %5.3f %5.3f\n",
        DEFminX - line_thickness, DEFminY - line_thickness,
        DEFmaxX + line_thickness, DEFmaxY + line_thickness);

 /* close the file */
    fclose(canvas->Canvas_params.post_script.file);

} /* PostScriptCanvasClose */

/*---------------------------------------------------------------
 Routine : PostScriptCanvasDestroy
 Purpose : Destroys the Postscript canvas (i.e., the file containing
 the Postscript).

 Free all memory associated with PostScript canvas
 Remove the canvas
---------------------------------------------------------------*/
void
  PostScriptCanvasDestroy(
    Canvas canvas)  /* IN  - PostScript canvas */
{
    remove(canvas->Canvas_params.post_script.filename);

} /* PostScriptCanvasDestroy */

/*---------------------------------------------------------------
 Routine : PostScriptCanvasSetLineWidth
 Purpose : Sets the drawing line width of the Postscript canvas.
---------------------------------------------------------------*/
void
  PostScriptCanvasSetLineWidth(
    Canvas         canvas,
    const unsigned line_width)
{
    fprintf(
        canvas->Canvas_params.post_script.file,
        "%d setlinewidth\t%% Set line width =>\n", line_width);

} /* PostScriptCanvasSetLineWidth */

/*---------------------------------------------------------------
 Routine : PostScriptText
 Purpose : Convert string to PostScript format

 Replace:  (  -> \(
           )  -> \)
           \  -> \\
---------------------------------------------------------------*/
static char *
  PostScriptText(
    char *text)
{
    char *ps;     /* PostScript string */
    char *s, *t;
    int   n;

    if (text == NULL) {
        return NULL;
    }

 /* count number of characters to change */
    for(n = 0, s = text; (s = strpbrk(s, "()\\")) != NULL; n++, s++)
        ;

    if (n == 0) {
        return strsave(text);
    }

 /* get memory */
/*     if ( (ps = (char *)malloc( (strlen(text)+n+1) * sizeof(char) )) == NULL )  */
    if ( 
      !fNewMemory( 
        (void *)&ps, 
        ( ( strlen(text) + n + 1 ) * sizeof(char) ) ) )
    {
        return NULL;
    }

 /* build new string */
    *ps = '\0';
    for(s = text; (t = strpbrk(s, "()\\")) != NULL; s = t+1) {
        strncat(ps, s, t-s);
        strncat(ps, "\\", 1);
        strncat(ps, t, 1);
    }
    strcat(ps, s);

    return ps;

} /* PostScriptText */

/*---------------------------------------------------------------
 Routine : PostScriptCanvasWriteText
 Purpose : Writes the supplied text at the supplied coordinates
 to the Postscript canvas.
---------------------------------------------------------------*/
void
  PostScriptCanvasWriteText(
    Canvas             canvas,
    const char        *text,
    const CanvasCoord  start)
{
    char *pstext = PostScriptText((char *)text);

    if (pstext != NULL) {

     /* move to position */
        fprintf(canvas->Canvas_params.post_script.file,
                    "%5.3f %5.3f moveto\t%% Write text =>\n",
                X(start.x), Y(start.y));

     /* show text */
        fprintf(canvas->Canvas_params.post_script.file,
                "(%s) show\n\n", pstext);

        FreeMemory(pstext);
    }

} /* PostScriptCanvasWriteText */

/*---------------------------------------------------------------
 Routine : PostScriptCanvasSetFont
 Purpose : Sets the Font of the Postscript canvas.
---------------------------------------------------------------*/
void
  PostScriptCanvasSetFont(
    Canvas      canvas,
    const char *font_string)
{
    fprintf(
        canvas->Canvas_params.post_script.file, 
        "/%s findfont\n", 
        font_string);
    /* Must set a scale for the font else won't work */
    fprintf(
        canvas->Canvas_params.post_script.file, 
        "%d scalefont\n", 
        PSFONT);
    fprintf(
        canvas->Canvas_params.post_script.file, 
        "setfont\n\n");

} /* PostScriptCanvasSetFont */

/*---------------------------------------------------------------
 Routine : PostScriptCanvasDrawLine
 Purpose : Draws the described line to the Postscript canvas.
---------------------------------------------------------------*/
void
  PostScriptCanvasDrawLine(
    Canvas            canvas,
    const CanvasCoord start,
    const CanvasCoord end)
{
    fprintf(
        canvas->Canvas_params.post_script.file, 
        "newpath\t\t%% Draw line =>\n");
    fprintf(
        canvas->Canvas_params.post_script.file,
        "%5.3f %5.3f moveto\n",
        X(start.x), Y(start.y));
    fprintf(
        canvas->Canvas_params.post_script.file,
        "%5.3f %5.3f lineto\n", X(end.x), Y(end.y));
    fprintf(canvas->Canvas_params.post_script.file, "stroke\n\n");

} /* PostScriptCanvasDrawLine */

/*---------------------------------------------------------------
 Routine : PostScriptCanvasDrawRectangle
 Purpose : Draws the described Rectangle to the Postscript canvas.
---------------------------------------------------------------*/
void
  PostScriptCanvasDrawRectangle(
    Canvas            canvas, /* IN - PS canvas     */
    const CanvasCoord point,  /* IN - top l. corner */
    const unsigned    width,  /* IN - rect. width   */
    const unsigned    height) /* IN - rect. height  */
{
    fprintf(
        canvas->Canvas_params.post_script.file, 
        "newpath\t\t%% Draw rectangle =>\n");
    fprintf(
        canvas->Canvas_params.post_script.file,
        "%5.3f %5.3f moveto\n",
        X(point.x), Y(point.y));
    fprintf(
        canvas->Canvas_params.post_script.file,
        "%5.3f %5.3f lineto\n", X(point.x + width), Y(point.y));
    fprintf(
        canvas->Canvas_params.post_script.file,
        "%5.3f %5.3f lineto\n", X(point.x + width), Y(point.y + height));
    fprintf(
        canvas->Canvas_params.post_script.file,
        "%5.3f %5.3f lineto\n", X(point.x), Y(point.y + height));
    fprintf(
        canvas->Canvas_params.post_script.file, "closepath\n");
    fprintf(canvas->Canvas_params.post_script.file, "stroke\n\n");

} /* PostScriptCanvasDrawRectangle */

/*---------------------------------------------------------------
 Routine : PostScriptCanvasDrawPolyline
 Purpose : Draws the described Polyline to the Postscript canvas.
---------------------------------------------------------------*/
void
  PostScriptCanvasDrawPolyline(
    Canvas canvas,
    const  unsigned num_points,
    const  CanvasCoord *points)
{
    int                i;

    fprintf(
        canvas->Canvas_params.post_script.file, 
        "newpath\t\t%% Draw polyline =>\n");
    fprintf(
        canvas->Canvas_params.post_script.file,
        "%5.3f %5.3f moveto\n",
        X(points[0].x), Y(points[0].y));
    for (i = 1; i < num_points; i++) {
        fprintf(
            canvas->Canvas_params.post_script.file,
            "%5.3f %5.3f lineto\n",
            X(points[i].x), Y(points[i].y));
    }
    fprintf(canvas->Canvas_params.post_script.file, "stroke\n\n");

} /* PostScriptCanvasDrawPolyline */

/*---------------------------------------------------------------
 Routine : PostScriptCanvasDrawArc
 Purpose : Draws the described Arc to the Postscript canvas.
    
 PostScript arcs are arcs of circles. You get an elipse
 by scaling a circle. But if you draw a shape at a given
 scale the line thickness is scaled too. We want normal
 lines. So you have to scale, then create the path, then
 change the scale back to normal before drawing. We draw a
 circle of the correct width, and then scale in the vertical
 direction.
 
 we generate these instructions :

 newpath
 gsave
    x y                               translate    % to centre of circle
    width*cos(start) width*sin(start) moveto       % to 1st point of arc
    1  height/width                   scale        % scale vertically
    0 0 r start end                   arc          % create path
    1  width/height                   scale        % scale back normal
    stroke                                         % draw it
 grestore
---------------------------------------------------------------*/
void
  PostScriptCanvasDrawArc(
    Canvas      canvas,       /* IN  - PostScript canvas */
    CanvasCoord point,        /* IN  - centre of circle  */
    unsigned    width,        /* IN  - width of elipse   */
    unsigned    height,       /* IN  - height of elipse  */
    int         start_angle,  /* IN  - start angle       */
    int         end_angle)    /* IN  - end angle         */
{
    float radius           = width/2.0;             /* radius unscaled circle */
    float centre_x         = point.x + width/2.0;   /* x coord centre         */
    float centre_y         = point.y + height/2.0;  /* y coord centre         */
    float vscale           = (float)YR(height) /    /* vertical scale factor  */
                                    XR(width);
    double start_angle_rad = (double)               /* start angle in radians */
                             A(start_angle) *
                             3.1415926/180.0;
    double end_angle_rad   = (double)               /* end angle in radians   */
                             A(end_angle) *
                             3.1415926/180.0;

    float start_x = radius * cos(start_angle_rad);
    float start_y = radius * sin(start_angle_rad);
    float end_x   = radius * cos(end_angle_rad);
    float end_y   = radius * sin(end_angle_rad);

 /* check the x and y values w.r.t. max and min values */
    X(centre_x + start_x); X(centre_x + end_x);
    Y(centre_y + start_y); Y(centre_y + end_y);

    if ( (start_angle < 0) && (end_angle > 0) ) {
        X(centre_x + radius);
    }

    if ( (start_angle < 180) && (end_angle > 180) ) {
        X(centre_x - radius);
    }

    if ( (start_angle < 90) && (end_angle > 90) ) {
        Y(centre_y + radius);
    }

    if ( (start_angle < 270) && (end_angle > 270) ) {
        Y(centre_y - radius);
    }

 /* create the arc */
    fprintf( 
      canvas->Canvas_params.post_script.file, 
      "newpath\t\t%% Draw arc =>\n");
    fprintf( canvas->Canvas_params.post_script.file, "gsave\n");

    fprintf( canvas->Canvas_params.post_script.file,
             "   %5.3f %5.3f translate\n", X(centre_x), Y(centre_y));

    fprintf( canvas->Canvas_params.post_script.file,
             "   1 %5.3f scale\n", vscale);

    fprintf( canvas->Canvas_params.post_script.file, "   %5.3f %5.3f moveto\n",
             XR(start_x),
             YR(start_y) );

    fprintf( canvas->Canvas_params.post_script.file,
             "   0 0 %5.3f %5.3f %5.3f arc\n",
             XR(radius),
             A(start_angle),
             A(end_angle));

    fprintf( canvas->Canvas_params.post_script.file, "   1 %5.3f scale\n",
             1.0/vscale);

    fprintf( canvas->Canvas_params.post_script.file, "   stroke\n");
    fprintf( canvas->Canvas_params.post_script.file, "grestore\n\n");

} /* PostScriptCanvasDrawArc */


/* these two functions are a horrible hack to allow
 * font heights and text widths identical to those
 * on the screen to be obtained
 */

/*---------------------------------------------------------------
 Routine : PostScriptCanvasTextWidth
 Purpose : Returns the text width.
---------------------------------------------------------------*/
unsigned
  PostScriptCanvasTextWidth(
    const Canvas canvas, 
    const char *text)
{
    return XTextWidth(canvas->Canvas_params.post_script.font, text,
                      strlen(text));

} /* PostScriptCanvasTextWidth */

/*---------------------------------------------------------------
 Routine : PostScriptCanvasFontHeight
 Purpose : Returns the Font Height
---------------------------------------------------------------*/
unsigned
  PostScriptCanvasFontHeight(
    const Canvas canvas)
{
    return
        (canvas->Canvas_params.post_script.font->ascent +
         canvas->Canvas_params.post_script.font->descent);

} /* PostScriptCanvasFontHeight */

