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
 
 SccsId : @(#)JavaCanvas.h	1.1 08/22/96
 
 Origin : FTA project,
          FSC Limited Private Venture.
 
 This module implements the canvas dependent on Java
***************************************************************/

#ifndef _XCanvas_H
#define _XCanvas_H

#include "fta.h" 
#include "boolean.h"

#include "Canvas.h"


/*---------------------------------------------------------------
 Routine : JavaCanvasClear
 Purpose : Clears the Java canvas area.
---------------------------------------------------------------*/
extern void	
  JavaCanvasClear(
    Canvas canvas);

/*---------------------------------------------------------------
 Routine : JavaCanvasSetPenColour
 Purpose : Set the foreground (pen) Colour
---------------------------------------------------------------*/
extern void	
  JavaCanvasSetPenColour(
    Canvas canvas, 
    unsigned long colour);

/*---------------------------------------------------------------
 Routine : JavaCanvasSetPaperColour
 Purpose : Set the background (paper) colour
---------------------------------------------------------------*/
extern void	
  JavaCanvasSetPaperColour(
    Canvas canvas, 
    unsigned long colour);

/*---------------------------------------------------------------
 Routine : JavaCanvasInvertPen
 Purpose : Inverts the pen colour to the existing pen or paper 
 colour depending if the pen is on, i.e. if on the pen colour is 
 set to the paper colour. This is usefull for deleting symbols 
 from the canvas area.
---------------------------------------------------------------*/
extern void	
  JavaCanvasInvertPen(Canvas canvas);

/*---------------------------------------------------------------
 Routine : XCanvasIsPenOn
 Purpose : Checks if pen is on.
---------------------------------------------------------------*/
extern BOOL	
  JavaCanvasIsPenOn(
    const Canvas canvas);


#endif
