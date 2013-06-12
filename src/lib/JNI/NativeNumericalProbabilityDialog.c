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

#include <jni.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <time.h>

#include "AssertUtil.h"

ASSERTFILE(__FILE__)

#include "FTAGUI_NumericalProbabilityDialog.h"
#include "NativeNumericalProbabilityDialog.h"
#include "TopLevelData.h"
#include "analysis_pd.h"
#include "CutSetsCommon.h"
#include "treeUtil.h"
#include "event_list.h"
#include "ProbabilityAnalysis.h"
#include "StringUtilities.h"
#include "util.h"

#define MAX_DISPLAY_STRING_LENGTH 100


static JNIEnv *javaEnv;
static jobject generateNumericalProbabilityObj;

static int    numbas;                      /* number of basic vals              */
static char  *mcs_file = NULL;             /* file with mcs                     */
static char  use_file_details[MAX_DISPLAY_STRING_LENGTH] ; /* details of mcs file               */
static int    num_mcs;                     /* munber of cut sets in mcs_file    */
static int    order;                       /* order of cut sets in mcs_file     */
static time_t date;                        /* ???                               */
static Expr   e;

static jmethodID postScrolledTextMethod;
static jmethodID postWarningMethod;
static jmethodID checkForInterruptMethod;
static jmethodID closeWorkingDialogMethod;
static jmethodID setProgressBarMaxMethod;
static jmethodID progressBarIncMethod;

JNIEXPORT jfloat JNICALL Java_FTAGUI_NumericalProbabilityDialog_nativeGetTimeEstimate
(JNIEnv *env, jobject obj, jint jmax_order, jint jmin_term, jint jmax_term)
{
	return probs_estimate(tree_pict->tree, jmax_order, jmin_term, jmax_term);
}


JNIEXPORT jstring JNICALL Java_FTAGUI_NumericalProbabilityDialog_nativeGetFileName(JNIEnv *env, jobject obj)
{
	if(mcs_file) {
		return (*env)->NewStringUTF(env, mcs_file);
	} else {
		return (*env)->NewStringUTF(env, "");
	}
}



JNIEXPORT jstring JNICALL Java_FTAGUI_NumericalProbabilityDialog_nativeGetMCSFileInfo(JNIEnv *env, jobject obj)
{
	return (*env)->NewStringUTF(env, use_file_details);
}



JNIEXPORT jint JNICALL Java_FTAGUI_NumericalProbabilityDialog_nativeGetNumCutSets(JNIEnv *env, jobject obj)
{
	return(num_mcs);
}



JNIEXPORT jint JNICALL Java_FTAGUI_NumericalProbabilityDialog_nativeNumBasicEvents(JNIEnv *env, jobject obj)
{
	ASSERT(tree_pict->tree->num_bas > 0 );
	return(tree_pict->tree->num_bas);
}



JNIEXPORT jboolean JNICALL Java_FTAGUI_NumericalProbabilityDialog_nativeLambdaEventsExist
(JNIEnv *env, jobject obj) {
	if(tree_pict->tree->num_lambda_events > 0) {
		return JNI_TRUE;
	} else {
		return JNI_FALSE;
	}
}


JNIEXPORT jboolean JNICALL Java_FTAGUI_NumericalProbabilityDialog_nativeGetMCSFile(JNIEnv *env, jobject obj)
{
    char *tree_filename;
	/* look for a file containing cut sets */
    tree_filename = SU_Join(tree_pict->tree->path, tree_pict->tree->name);

    /* does old mcs_file need freeing? */
	if(mcs_file) {
        strfree(mcs_file);
        mcs_file = NULL;
	}

    file_get_mcs( 
		tree_filename, 
		&mcs_file, &num_mcs, &order, &date, &e);

    strfree(tree_filename);

	sprintf(use_file_details, "(%d Minimal cut sets, Max order: %d,  Date: %s",
                 num_mcs, order, ctime(&date));
	use_file_details[strlen(use_file_details) - 1] = ')';
	
	if(mcs_file) {

		expand_tree(tree_pict->tree); 
		
	    /* does old mcs expression need freeing? */
		if(tree_pict->tree->mcs_expr) {
            ExprDestroy(tree_pict->tree->mcs_expr);
		}

		tree_pict->tree->mcs_expr  = e; 
		tree_pict->tree->max_order = order; 
		tree_pict->tree->num_mcs   = num_mcs; 
  
		tree_pict->tree->num_lambda_events = 0; 
		count_lambda_events( 
			tree_pict->tree, 
			tree_pict->tree->top_item ); 
		
		return JNI_TRUE;
	} else {
		return JNI_FALSE;
	}

}



void GenerateNumericalProbabilityPostScrolledText(const char *title, const char *fileName){
	(*javaEnv)->CallVoidMethod(javaEnv, generateNumericalProbabilityObj, postScrolledTextMethod, 
		(*javaEnv)->NewStringUTF(javaEnv, title), (*javaEnv)->NewStringUTF(javaEnv, fileName));
}

void GenerateNumericalProbabilityPostWarning(const char *title, const char *warning){
	(*javaEnv)->CallVoidMethod(javaEnv, generateNumericalProbabilityObj, postWarningMethod, 
		(*javaEnv)->NewStringUTF(javaEnv, title), (*javaEnv)->NewStringUTF(javaEnv, warning));
}

BOOL GenerateNumericalProbabilityCheckForInterrupt() {

	if((*javaEnv)->CallBooleanMethod(javaEnv, generateNumericalProbabilityObj, checkForInterruptMethod)){
		return TRUE;
	} else {
		return FALSE;
	}
}


void GenerateNumericalProbabilityCloseWorkingDialog() {

	(*javaEnv)->CallVoidMethod(javaEnv, generateNumericalProbabilityObj, closeWorkingDialogMethod);

}

void GenerateNumericalProbabilitySetProgressBarMax(int max) {
	jint jmax;
	jmax = max;
	(*javaEnv)->CallVoidMethod(javaEnv, generateNumericalProbabilityObj, setProgressBarMaxMethod, jmax);
}

void GenerateNumericalProbabilityProgressBarInc() {
	(*javaEnv)->CallVoidMethod(javaEnv, generateNumericalProbabilityObj, progressBarIncMethod);
}

JNIEXPORT void JNICALL Java_FTAGUI_GenerateNumericalProbability_nativeOKButtonAction
	(JNIEnv *env, jobject obj, jint chosenOrder, jfloat unitTime, jint chosenTerms)
{
	generateNumericalProbabilityObj = obj;
	javaEnv = env;

	ProbsWidOkCb(tree_pict, chosenOrder, unitTime, chosenTerms);

    /* Close the busy dialog */ 
    GenerateNumericalProbabilityCloseWorkingDialog();
}



JNIEXPORT void JNICALL
Java_FTAGUI_GenerateNumericalProbability_nativeInitialiseGenerateNumericalProbability(JNIEnv *env, jobject obj) {

	jclass cls;

	cls = (*env)->GetObjectClass(env, obj);

	postScrolledTextMethod = (*env)->GetMethodID(env, cls, "postScrolledText", "(Ljava/lang/String;Ljava/lang/String;)V");
	if(postScrolledTextMethod == 0) {
			printf("DEBUG: failed to get post scrolled text java method ID\n");
			return;
	}
	postWarningMethod = (*env)->GetMethodID(env, cls, "postWarning", "(Ljava/lang/String;Ljava/lang/String;)V");
	if(postWarningMethod == 0) {
			printf("DEBUG: failed to get post warning java method ID\n");
			return;
	}
	checkForInterruptMethod = (*env)->GetMethodID(env, cls, "checkForInterrupt", "()Z");
	if(checkForInterruptMethod == 0) {
			printf("DEBUG: failed to get check for interrupt java method ID\n");
			return;
	}
	closeWorkingDialogMethod = (*env)->GetMethodID(env, cls, "closeWorkingDialog", "()V");
	if(closeWorkingDialogMethod == 0) {
			printf("DEBUG: failed to get close working dialog java method ID\n");
			return;
	}
	setProgressBarMaxMethod = (*env)->GetMethodID(env, cls, "setProgressBarMax", "(I)V");
	if(setProgressBarMaxMethod == 0) {
			printf("DEBUG: failed to get set progress bar max java method ID\n");
			return;
	}
	progressBarIncMethod = (*env)->GetMethodID(env, cls, "progressBarInc", "()V");
	if(progressBarIncMethod == 0) {
			printf("DEBUG: failed to get progress bar inc java method ID\n");
			return;
	}
}
