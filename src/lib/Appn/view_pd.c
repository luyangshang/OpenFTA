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
 Module : View
 Author : FSC Limited
 Date   : 29/11/95
 
 SccsId :@(#)view_pd.c	1.5 11/13/96
 
 This module implements the pulldown menus for the View option of 
 the menu bar. 
 
****************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>


#include "view_pd.h"
#include "fta.h"
#include "tree_pic.h"
#include "display_tree.h"
#include "tree_pic_forest.h"
#include "FileUtilities.h"
#include "util.h"
#include "Dialogs.h"
#include "NativeFTAFrame.h"
#include "StringUtilities.h"


#define TREE_NOT_VALID_WARNING \
"This tree is not semantically valid\n\
Consequently analysis cannot be performed\n\
Select 'Analysis->Validate' to generate an error report"

#define MCS_NOT_GENERATED_WARNING \
"Minimal cut sets not generated\n\
Select Analysis->Qualitative (m.c.s.)"

#define MCS_TREE_NOT_GENERATED_WARNING \
"Simplified tree not generated\n\
Select Analysis->Simplified Tree"

#define PROB_REPORT_NOT_GENERATED_WARNING \
"Probability report not generated\n\
Select Analysis->Numerical Probability"

#define MC_REPORT_NOT_GENERATED_WARNING \
"Monte-Carlo report not generated\n\
Select Analysis->Monte Carlo Simulation"


/*--------------------------------------------------------------
 Routine :view_menu_cb
 Purpose : Callback invoked when an item is selected in the view menu.
---------------------------------------------------------------*/ 

void view_menu_cb(TREE_PIC_FOREST *tree_pic_forest, int item_no)
{
	char                       *report_filename;
	char                       *tree_filename;

    tree_filename = SU_Join(tree_pic_forest->tree_pic->tree->path, tree_pic_forest->tree_pic->tree->name);
	
    switch ( item_no ) {
		case ViewQualitative : /* Qualitative Report */
			if (tree_pic_forest->tree_pic->tree->mcs_exists) {
				report_filename =
					(char *)generate_filename(
					tree_filename,
					MCS_REPORT_SUFFIX );
				FTAFramePostScrolledText(
					FTA_REPORT_TITLE, 
					report_filename);
				strfree( report_filename );
			} else {
				FTAFramePostError(MCS_NOT_GENERATED_WARNING, FTA_ERROR_TITLE);
			}
			break;

		case ViewProbability : /* Probability Report */
			if (tree_pic_forest->tree_pic->tree->probs_exists) {
				report_filename =
					(char *)generate_filename(
					tree_filename,
					PROBS_REPORT_SUFFIX );
				FTAFramePostScrolledText(
					FTA_REPORT_TITLE, 
					report_filename);
				strfree( report_filename );
			} else {
                FTAFramePostError(PROB_REPORT_NOT_GENERATED_WARNING, FTA_ERROR_TITLE);
			}
			break;

		case ViewMonteCarlo : /* Monte-Carlo Report */
			if (tree_pic_forest->tree_pic->tree->monte_exists) {
                
				report_filename =
					(char *)generate_filename(
					tree_filename,
					MONTE_CARLO_REPORT_SUFFIX );
				FTAFramePostScrolledText(
					FTA_REPORT_TITLE, report_filename);
				strfree( report_filename );
			} else {
				FTAFramePostError(MC_REPORT_NOT_GENERATED_WARNING, FTA_ERROR_TITLE );
            }
            break;

		default :
			FTAFramePostWarning(UNAVAILABLE_WARNING, FTA_INFO_TITLE);
			return;
			break;
    }

    free(tree_filename);

}


