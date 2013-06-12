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

#ifndef HAVE_NATIVEFTACANVAS_H
#define HAVE_NATIVEFTACANVAS_H

#include "boolean.h"
#include "tree_pic.h"

extern void FTACanvasFillRect(int x, int y, int width, int height, int symbol_type);
extern void FTACanvasDrawRect(int x, int y, int width, int height);
extern void FTACanvasDrawLine(int x1, int y1, int x2, int y2);
extern void FTACanvasClearArc(int x, int y, int width, int height, int start_angle, int end_angle);
extern void FTACanvasDrawArc(int x, int y, int width, int height, int start_angle, int end_angle);
extern void FTACanvasFillArc(int x, int y, int width, int height, int start_angle, int end_angle, int symbol_type);
extern void FTACanvasFillPolyline(const CanvasCoord *p, int length, int type);

extern void FTACanvasSetPenColour(unsigned long colour);
extern void FTACanvasSetPenWidth(unsigned width);
extern void FTACanvasWriteText(int x, int y, const char* text);
extern int FTACanvasTextWidth(const char* text);
extern int FTACanvasFontHeight();
extern void FTACanvasSelectPrimaryEvent(const char* id);

extern void FTACanvasEditTreeText();
extern void FTACanvasEditIntermediateEvent();
extern void FTACanvasEditGate();
extern void FTACanvasEditTransferTree();

#endif
