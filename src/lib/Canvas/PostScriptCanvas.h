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
 
 SccsId : @(#)PostScriptCanvas.h	1.1 08/22/96
 
 Origin : FTA project,
          FSC Limited Private Venture.
 
 This module implements the Postscript canvas. In reality the 
 Canvas is a file of Postscript commands, but that need not 
 concern the user of the Canvas, to whom it behaves like any 
 other Canvas.

 The Postscript commands conform to the Encapsulated PostScript
 Standard.
***************************************************************/
 
#ifndef _PostScript_H
#define _PostScript_H

#include <stdio.h>
#include "Canvas.h"


/*extern float                ps_scale; */

/*---------------------------------------------------------------
 Routine : PostScriptCanvasCreate
 Purpose : Creates a Postscript Canvas. 
---------------------------------------------------------------*/
extern Canvas                
  PostScriptCanvasCreate(
    FILE *file, 
    const char *filename);

/*---------------------------------------------------------------
 Routine : PostScriptCanvasClose
 Purpose : Close the Postscript Canvas. This causes the final
 Postscript command to be sent to the file and closes the file.
---------------------------------------------------------------*/
extern void                
  PostScriptCanvasClose(
    Canvas canvas);

/*---------------------------------------------------------------
 Routine : PostScriptCanvasTextWidth
 Purpose : Returns the text width.
---------------------------------------------------------------*/
extern unsigned
  PostScriptCanvasTextWidth(
    const Canvas canvas, 
    const char *text);

/*---------------------------------------------------------------
 Routine : PostScriptCanvasFontHeight
 Purpose : Returns the Font Height
---------------------------------------------------------------*/
extern unsigned
  PostScriptCanvasFontHeight(
    const Canvas canvas);

#endif

