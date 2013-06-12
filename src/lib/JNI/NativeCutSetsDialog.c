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

#include "FTAGUI_CutSetsDialog.h"
#include "NativeCutSetsDialog.h"
#include "TopLevelData.h"
#include "analysis_pd.h"
#include "CutSetsCommon.h"
#include "StringUtilities.h"
#include "util.h"

#define MAX_DISPLAY_STRING_LENGTH 100

static JNIEnv *javaEnv;
static jobject generateCutSetsObj;

static int    numbas;                      /* number of basic vals              */
static char  *mcs_file = NULL;             /* file with mcs                     */
static char  use_file_details[MAX_DISPLAY_STRING_LENGTH] ; /* details of mcs file               */
static int    num_mcs;                     /* number of cut sets in mcs_file    */
static int    order;                       /* order of cut sets in mcs_file     */
static time_t date;                        /* ???                               */

static jmethodID postScrolledTextMethod;
static jmethodID checkForInterruptMethod;
static jmethodID closeWorkingDialogMethod;
static jmethodID setProgressBarMaxMethod;
static jmethodID progressBarIncMethod;

JNIEXPORT jint JNICALL Java_FTAGUI_CutSetsDialog_nativeNumBasicEvents(JNIEnv *env, jobject obj){
	ASSERT(tree_pict->tree->num_bas > 0 );
	return(tree_pict->tree->num_bas);
}


JNIEXPORT jstring JNICALL Java_FTAGUI_CutSetsDialog_nativeGetFileName(JNIEnv *env, jobject obj){
	if(mcs_file) {
		return (*env)->NewStringUTF(env, mcs_file);
	} else {
		return (*env)->NewStringUTF(env, "");
	}
}


JNIEXPORT jboolean JNICALL Java_FTAGUI_CutSetsDialog_nativeGetMCSFile(JNIEnv *env, jobject obj){

    char *tree_filename;
    Expr   e;

    tree_filename = SU_Join(tree_pic_forest.tree_pic->tree->path, tree_pic_forest.tree_pic->tree->name);

    if(!tree_filename) {
        return JNI_FALSE;
    }

    /* does old mcs_file need freeing? */
	if(mcs_file) {
        strfree(mcs_file);
        mcs_file = NULL;
	}

	/* look for a file containing cut sets */
	file_get_mcs( 
		tree_filename, 
		&mcs_file, &num_mcs, &order, &date, &e);

    ExprDestroy(e);

    strfree(tree_filename);

	sprintf(use_file_details, "(%d Minimal cut sets, Max order: %d,  Date: %s",
                 num_mcs, order, ctime(&date));
	use_file_details[strlen(use_file_details) - 1] = ')';

	if(mcs_file) {
		return JNI_TRUE;
	} else {
		return JNI_FALSE;
	}

}


JNIEXPORT jstring JNICALL Java_FTAGUI_CutSetsDialog_nativeGetMCSFileInfo(JNIEnv *env, jobject obj){
	return (*env)->NewStringUTF(env, use_file_details);
}



void GenerateCutSetsPostScrolledText(const char *title, const char *fileName){
	(*javaEnv)->CallVoidMethod(javaEnv, generateCutSetsObj, postScrolledTextMethod, 
		(*javaEnv)->NewStringUTF(javaEnv, title), (*javaEnv)->NewStringUTF(javaEnv, fileName));
}



BOOL GenerateCutSetsCheckForInterrupt() {

	if((*javaEnv)->CallBooleanMethod(javaEnv, generateCutSetsObj, checkForInterruptMethod)){
		return TRUE;
	} else {
		return FALSE;
	}
}


void GenerateCutSetsCloseWorkingDialog() {

	(*javaEnv)->CallVoidMethod(javaEnv, generateCutSetsObj, closeWorkingDialogMethod);

}


void GenerateCutSetsSetProgressBarMax(int max) {
	jint jmax;
	jmax = max;
	(*javaEnv)->CallVoidMethod(javaEnv, generateCutSetsObj, setProgressBarMaxMethod, jmax);
}

void GenerateCutSetsProgressBarInc() {
	(*javaEnv)->CallVoidMethod(javaEnv, generateCutSetsObj, progressBarIncMethod);
}


JNIEXPORT void JNICALL Java_FTAGUI_GenerateCutSets_nativeOKButtonAction
(JNIEnv *env, jobject obj , jboolean juseFile, jint jorder){

	int order = jorder;
	
	generateCutSetsObj = obj;
	javaEnv = env;
	CutSetWidAlgebraicOkCb(tree_pict, order);

    /* Close the busy dialog */
    GenerateCutSetsCloseWorkingDialog();
}


JNIEXPORT void JNICALL
Java_FTAGUI_GenerateCutSets_nativeInitialiseGenerateCutSets(JNIEnv *env, jobject obj) {

	jclass cls;

	generateCutSetsObj = obj;

	cls = (*env)->GetObjectClass(env, obj);

	postScrolledTextMethod = (*env)->GetMethodID(env, cls, "postScrolledText", "(Ljava/lang/String;Ljava/lang/String;)V");
	if(postScrolledTextMethod == 0) {
			printf("DEBUG: failed to get post scrolled text java method ID\n");
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
