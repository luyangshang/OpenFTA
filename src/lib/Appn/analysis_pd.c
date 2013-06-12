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
 Module : analysis_pd
 Author : FSC Limited
 Date   : 13/8/96
 
 SccsId : @(#)analysis_pd.c	1.8 01/28/97
 
 Origin : FTA project,
          FSC Limited Private Venture.
 
 This module implements the pulldown menu for the Analysis option of
 the menu bar.
***************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include "NativeFTAFrame.h"
#include "NativeCutSetsDialog.h"
#include "NativeNumericalProbabilityDialog.h"
#include "NativeMonteCarloDialog.h"
#include "analysis_pd.h"
#include "tree_pic_forest.h"
#include "event_list.h"
#include "AssertUtil.h"
#include "treeUtil.h"
#include "Dialogs.h"
#include "FileUtilities.h"
#include "util.h"
#include "CutSetsByAlgebraicMethod.h" 
#include "ProbabilityAnalysis.h"
#include "MonteCarloSimulation.h"
#include "StringUtilities.h"



ASSERTFILE( __FILE__ )

#define UNCERT_MENU_STRING          "Uncertainty"
#define SENSVTY_MENU_STRING         "Sensitivity"
#define PHASED_MISSION_MENU_STRING  "Phased Mission"
#define N_BASIC_LABEL_STRING        "There are xxxx basic events in the tree"
#define ESTIMATED_RT_STRING         "Estimated run time : xh xm"

#define TREE_NOT_VALID_WARNING \
"This tree is not semantically valid\n\
Consequently analysis cannot be performed\n\
Select 'Analysis->Validate' to generate an error report"

#define TREE_NOT_LOADED_ERROR       "Tree not loaded"
#define TREE_NOT_SAVED_ERROR        "Tree not saved"
#define MCS_NOT_GENERATED_ERROR \
"No minimal cut sets on file\n\
Select Analysis->Qualitative (m.c.s.)"

#define FILE_NOT_OPENED_ERROR \
"Temporary file could not be opened\n\
Check directory file permissions"

#define CUT_SET_DIALOG_TITLE        FTA_APPN_TITLE " : Minimal Cut Sets"
#define MONTE_CARLO_DIALOG_TITLE    FTA_APPN_TITLE " : Monte Carlo"
#define PROBS_DIALOG_TITLE          FTA_APPN_TITLE " : Probabilities"


/*********************************************************************
 Define Analysis Pulldown Numbers
 ********************************************************************/
#define AnalysisFirst          0
#define AnalysisLast           3
#define NumAnalysisItems AnalysisLast + 1 + ( 3 * Separator )
#define AnalysisValidate       0
#define AnalysisCutSet         1
#define AnalysisProbabilities  2
#define AnalysisMonteCarlo     3


/*---------------------------------------------------------------
 Routine : CutSetWidAlgebraicOkCb
 Purpose : The callback routine used for the OK button of the
 Algebraic Cut Set dialog.
---------------------------------------------------------------*/
void CutSetWidAlgebraicOkCb(
	TREE_PIC *tree_pic,
    int      order )
{
	char *report_filename;
	char *tree_filename;

    tree_filename = SU_Join(tree_pic->tree->path, tree_pic->tree->name);

    report_filename =
		(char *)generate_filename(
			tree_filename,
			MCS_REPORT_SUFFIX );


	if (mcs_algebraic(
		report_filename,
		tree_pic->tree,
		order )) {

		tree_pic->tree->mcs_exists = TRUE;

		/* Called from CutSetsDialog so need to send text back to there */
		GenerateCutSetsPostScrolledText(FTA_REPORT_TITLE, report_filename );

	} 

	strfree(report_filename); 
    strfree( tree_filename );


} /* CutSetWidAlgebraicOkCb */


/*---------------------------------------------------------------
 Routine : ProbsWidOkCb
 Purpose : The callback routine used for the OK button of the
 Numerical Probabilities dialog.
---------------------------------------------------------------*/
void
  ProbsWidOkCb(
	TREE_PIC *tree_pic,
    int chosen_order,
    float unit_time,
    int chosen_terms )
{
	char *report_filename;
	char *tree_filename;

    tree_filename = SU_Join(tree_pic->tree->path, tree_pic->tree->name);

    report_filename =
		(char *)generate_filename(
		tree_filename,
		PROBS_REPORT_SUFFIX );

	if (calculate_probs(
		report_filename,
		tree_pic->tree, 
		chosen_order, 
		chosen_terms, 
		unit_time )) {

		tree_pic->tree->probs_exists = TRUE;

		/* Called from NumericalProbabilityDialog so need to send text back to there */
		GenerateNumericalProbabilityPostScrolledText(FTA_REPORT_TITLE, report_filename );


	} else {
		GenerateNumericalProbabilityCloseWorkingDialog();		
		GenerateNumericalProbabilityPostWarning("Unable to complete Numerical Probability calculation", "Numerical Probability");
	}
  
	strfree( report_filename );
   	strfree( tree_filename );

	remove( PROBS_REPORT_TEMPFILE );

} /* ProbsWidOkCb */



/*---------------------------------------------------------------
 Routine : MonteWidOkCb
 Purpose : The callback routine used for the OK button of the
 Monte Carlo simulations dialog.
---------------------------------------------------------------*/
void MonteWidOkCb(
	TREE_PIC *tree_pic,
	int      simulations,
	float    unit_time )
{
	char *report_filename;
	char *tree_filename;

    tree_filename = SU_Join(tree_pic->tree->path, tree_pic->tree->name);

    report_filename =
		(char *)generate_filename(
		tree_filename,
		MONTE_CARLO_REPORT_SUFFIX );
 

	/* do the Monte-Carlo simulation */
	if (generate_monte_carlo_simulation(
		FALSE,
		NULL,
		report_filename,
		tree_pic->tree, 
		simulations,
		unit_time )) {

		tree_pic->tree->monte_exists = TRUE;

		/* Called from MonteCarloDialog so need to send text back to there */
		GenerateMonteCarloPostScrolledText(FTA_REPORT_TITLE,report_filename );
 

	} else {

	}

	strfree( report_filename );
   	strfree( tree_filename );

} /* MonteWidOkCb */


/*---------------------------------------------------------------
 Routine : analysis_menu_cb
 Purpose : The callback routine for the "Analysis" pulldown.
---------------------------------------------------------------*/
void analysis_menu_cb(
    TREE_PIC_FOREST *tree_pic_forest,
    int              item_no
)
{
	FILE            *temp_file;
	char            *report_filename;
    char            *tree_filename;

	/* If there is not a tree loaded */
	if ( tree_pic_forest->tree_pic->tree->top_item == NULL ) {
		FTAFramePostError(TREE_NOT_LOADED_ERROR, FTA_ERROR_TITLE);
		return;
	} /* if tree not loaded */

	/* If the tree is not saved */
	if ( tree_pic_forest->tree_pic->tree->edited ||
		 !tree_pic_forest->tree_pic->tree->name) {
		FTAFramePostError(TREE_NOT_SAVED_ERROR, FTA_ERROR_TITLE);
		return;
	} /* if tree not saved */

	switch ( item_no ) {

		case AnalysisValidate : /* Validate */
			/* Q: Is VALIDITY_REPORT_TEMPFILE really temporary ? 
			   A: Yes. It is only valid for as long as the tree
                  remains unedited, and is best generated afresh
                  each time. The temp file is deleted at the end of
                  the FTA session, but during that time the validation
                  report could be printed from a separate window onto
                  the directory. */

		    /* The Report files are not temporary. They should persist and be
			   associated with a tree, but shall only be applicable to a tree
		       until that tree is edited. It shall be possible to view/print
        	   a report from within FTA for as long as the report is valid.
		       The temp_files here are only used to hold the VALIDATION 
		       temporary file.  */

            tree_filename = SU_Join(tree_pic_forest->tree_pic->tree->path, tree_pic_forest->tree_pic->tree->name);

			report_filename =
				(char *)generate_filename(
				tree_filename ?
				tree_filename :
				NULL_TREE_NAME,
				VALIDITY_REPORT_SUFFIX );

			if ( ( temp_file = fopen( report_filename, "w" ) ) )
			{ 
				fprintf( 
					temp_file,  
					"VALIDATION REPORT ON %s\n\n",  
					tree_pic_forest->tree_pic->tree->name ? 
					tree_pic_forest->tree_pic->tree->name :  
					NULL_TREE_NAME ); 
				if ( validate_tree( 
					tree_pic_forest->tree_pic->tree,
					tree_pic_forest->tree_pic->tree->top_item,
					temp_file ) ) { 
						fprintf( temp_file, "\nTREE VALID\n" ); 
				} else {
					fprintf( temp_file, "\nTREE INVALID\n" ); 
				} /* if tree valid */

				fclose(temp_file);


				FTAFramePostScrolledText(FTA_REPORT_TITLE, report_filename );

			} else { /* file not opened */
				FTAFramePostError(FILE_NOT_OPENED_ERROR, FTA_ERROR_TITLE );
				/* just in case the stream remains after the error ... */
				fclose(temp_file);  

			} /* if tree exists and tmp file is opened */

			strfree(report_filename);
            strfree(tree_filename);

		break;


		case AnalysisCutSet :

			/* This was handled by the cut_set_menu as a
			   slide right */

			ASSERT( tree_pic_forest != NULL );
			ASSERT( tree_pic_forest->tree_pic != NULL );
			ASSERT( tree_pic_forest->tree_pic->tree != NULL );
			ASSERT( tree_pic_forest->tree_pic->tree->top_item != NULL );

			if ( ( temp_file = fopen( VALIDITY_REPORT_TEMPFILE, "w" ) ) )
			{  
				if ( validate_tree(  
					tree_pic_forest->tree_pic->tree, 
					tree_pic_forest->tree_pic->tree->top_item, 
					temp_file ) ) { 
 

					/* if necessary, expand tree */
					expand_tree(tree_pic_forest->tree_pic->tree);

					/* display mcs dialog */
					FTAFrameInvokeCutSetsDialog();

				} 
				else 
				{ /* tree not valid */
					FTAFramePostError(TREE_NOT_VALID_WARNING, FTA_ERROR_TITLE); 
				} /* if tree valid */
				fclose(temp_file);  
				remove( VALIDITY_REPORT_TEMPFILE ); 
			} 
			else 
			{ /* file not opened */
				FTAFramePostError(FILE_NOT_OPENED_ERROR, FTA_ERROR_TITLE ); 
  
			} /* if tree valid and tmp file is opened */

		break;

		case AnalysisProbabilities : /* Numerical Probability */
			
			if ( ( temp_file = fopen( VALIDITY_REPORT_TEMPFILE, "w" ) ) )
			{ 
				if ( validate_tree(
					tree_pic_forest->tree_pic->tree,
					tree_pic_forest->tree_pic->tree->top_item,
					temp_file ) ) {

					/* display probabilities dialog */
					FTAFrameInvokeNumericalProbabilityDialog();

				} else { /* tree not valid */
 
				FTAFramePostError(
						TREE_NOT_VALID_WARNING,
						FTA_ERROR_TITLE );
				} /* if tree valid */

				fclose(temp_file); 
				remove( VALIDITY_REPORT_TEMPFILE );
 
			} else { /* file not opened */
				FTAFramePostError(
					FILE_NOT_OPENED_ERROR,
					FTA_ERROR_TITLE );

			} /* if tree valid and tmp file is opened */
			
		break;

		case AnalysisMonteCarlo : /* Monte Carlo Simulation */
			if ( ( temp_file = fopen( VALIDITY_REPORT_TEMPFILE, "w" ) ) )
			{ 
				if ( validate_tree(
					tree_pic_forest->tree_pic->tree,
					tree_pic_forest->tree_pic->tree->top_item,
					temp_file ) ) {

					/* display monte-carlo dialog */
 					FTAFrameInvokeMonteCarloDialog();

				} else { /* tree not valid */
					FTAFramePostError(
						TREE_NOT_VALID_WARNING,
						FTA_ERROR_TITLE );
				} /* if tree valid */
        
				fclose(temp_file); 
				remove( VALIDITY_REPORT_TEMPFILE );
			} else { /* file not opened */
				FTAFramePostError(
					FILE_NOT_OPENED_ERROR,
					FTA_ERROR_TITLE );

			} /* if tree valid and tmp file is opened */

		break;

		default :
			FTAFramePostWarning(UNAVAILABLE_WARNING, FTA_INFO_TITLE );
		break;

  } /* switch on button pressed */

} /* analysis_menu_cb */

