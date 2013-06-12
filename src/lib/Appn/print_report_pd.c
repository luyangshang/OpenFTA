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

/****************************************************************t
 Module : Print_report_pd
 Author : FSC Limited
 Date   : 29/11/95
 
 SccsId :@(#)print_report_pd.c	1.6 02 Jul 1997
 
 Origin : FTA project, FSC Limited Private Venture, Level 2 Prototype.
 
 Purpose: This module implements the pulldown menus for the Print option of
          the Report pulldown on the menu bar.
 
****************************************************************/

#include <stdlib.h>

#include "print_report_pd.h"
#include "Dialogs.h"
#include "tree_pic_forest.h"

#include "fta.h"
#include "FileUtilities.h"
#include "StringUtilities.h"
#include "NativeFTAFrame.h"
#include "util.h"

#define SYSTEM_CALL_ERROR \
"System call failure\n\
Call FTA Support for guidance"

#define MCS_NOT_GENERATED_ERROR \
"Minimal cut sets not generated\n\
Select Analysis->Qualitative (Combo)"

#define PROB_REPORT_NOT_GENERATED_ERROR \
"Probability report not generated\n\
Select Analysis->Numerical Probability"

#define MC_REPORT_NOT_GENERATED_ERROR \
"Monte-Carlo report not generated\n\
Select Analysis->Monte Carlo Simulation"



/*--------------------------------------------------------------
 Routine : print_menu_cb
 Purpose : Invoked when an item is selected in the print menu.
---------------------------------------------------------------*/
void print_menu_cb(
    TREE_PIC_FOREST *tree_pic_forest,
    int item_no
	)
{
    char *report_filename;
	char *tree_filename;

    tree_filename = SU_Join(tree_pic_forest->tree_pic->tree->path, tree_pic_forest->tree_pic->tree->name);
	
    switch  ( item_no ) {
    case PrintQualitative : /* Qualitative report */

        if (tree_pic_forest->tree_pic->tree->mcs_exists) {

            report_filename =
              (char *)generate_filename(
                tree_filename,
                MCS_REPORT_SUFFIX );

		    FTAFramePrintTextFile(report_filename);
		    strfree( report_filename );

        } else {
            FTAFramePostError(MCS_NOT_GENERATED_ERROR, FTA_ERROR_TITLE);
        }
        break;

    case PrintProbability : /* Probabilities Report */

        if (tree_pic_forest->tree_pic->tree->probs_exists) {
            report_filename =
              (char *)generate_filename(
                tree_filename,
                PROBS_REPORT_SUFFIX );

		    FTAFramePrintTextFile(report_filename);
		    strfree( report_filename );

        } else {
            FTAFramePostError(PROB_REPORT_NOT_GENERATED_ERROR, FTA_ERROR_TITLE);
        }
        break;

    case PrintMonteCarlo : /* Monte-Carlo Report */
        if (tree_pic_forest->tree_pic->tree->monte_exists) {
            report_filename =
              (char *)generate_filename(
                tree_filename,
                MONTE_CARLO_REPORT_SUFFIX );

			FTAFramePrintTextFile(report_filename);
		    strfree( report_filename );

         } else {
            FTAFramePostError(MC_REPORT_NOT_GENERATED_ERROR, FTA_ERROR_TITLE);
        }
        break;

    default:
        FTAFramePostError(UNAVAILABLE_WARNING, FTA_ERROR_TITLE);
        break;
    }

    strfree(tree_filename);

}

