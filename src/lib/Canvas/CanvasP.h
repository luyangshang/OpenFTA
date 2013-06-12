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
 Module : CanvasP
 Author : FSC Limited
 Date   : 5/8/96
 
 SccsId : @(#)CanvasP.h	1.1 08/22/96
 
 Origin : FTA project,
          FSC Limited Private Venture.
 
 This module implements the Private description of the canvas object. 
 The canvas can be either an X, Java or a Postscript canvas, but this 
 is of no relevance to the user of the Canvas.
***************************************************************/

#ifndef _CanvasP_H
#define _CanvasP_H

/* #include "XCanvasP.h" 
#include "PostScriptCanvasP.h" */

#include "JavaCanvasP.h"


/*
 * The types of Canvas.
 */

typedef enum {
		X_CANVAS,
		POST_SCRIPT_CANVAS,
		JAVA_CANVAS
	} Canvas_type;


typedef struct _Canvas_rec {
	    Canvas_type		type;
	    union {
/*		XCanvas_rec	x_canvas; */
/*		PostScript_rec	post_script; */
		JavaCanvas_rec java_canvas;
		
		} Canvas_params;
	} Canvas_rec;

#endif
