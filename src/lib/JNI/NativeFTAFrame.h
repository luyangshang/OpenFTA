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

#ifndef HAVE_NATIVEFTA_H
#define HAVE_NATIVEFTA_H

#define FTA_BLACK 0
#define FTA_GREEN 1

#include "boolean.h"
#include "tree_pic.h"

extern BOOL FTAFrameAskQuestion(const char *question, const char *title);
extern void FTAFramePostWarning(const char *warning, const char *title);
extern void FTAFramePostError(const char *warning, const char *title);
extern void FTAFramePostScrolledText(const char *title, const char *fileName);
extern void FTAFramePrintTextFile(const char *fileName);
extern void FTAFrameInvokeCutSetsDialog();
extern void FTAFrameInvokeNumericalProbabilityDialog();
extern void FTAFrameInvokeMonteCarloDialog();
extern void FTAFrameLoadPEDFile(const char *file);
extern void FTAFrameNewPEDFile();

extern void FTAFrameSetSensitive(int pos, BOOL on);

extern void FTAFrameSetWindowTitle(TREE_PIC *tp);

#endif
