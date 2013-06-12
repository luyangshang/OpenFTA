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

#include "FTAGUI_MonteCarloDialog.h"
#include "FTAGUI_GenerateMonteCarlo.h"
#include "NativeMonteCarloDialog.h"
#include "TopLevelData.h"
#include "analysis_pd.h"
#include "treeUtil.h"
#include "event_list.h"
#include "MonteCarloSimulation.h"


#define MAX_DISPLAY_STRING_LENGTH 100


static JNIEnv *javaEnv;
static jobject generateMonteCarloObj;

static jmethodID postScrolledTextMethod;
static jmethodID checkForInterruptMethod;
static jmethodID closeWorkingDialogMethod;
static jmethodID setProgressBarMaxMethod;
static jmethodID progressBarIncMethod;

JNIEXPORT void JNICALL Java_FTAGUI_MonteCarloDialog_nativeGetTreeInfo
(JNIEnv *env, jobject obj) {
	expand_tree(tree_pict->tree); 
}


JNIEXPORT jboolean JNICALL Java_FTAGUI_MonteCarloDialog_nativeLambdaEventsExist
(JNIEnv *env, jobject obj) {
	if(tree_pict->tree->num_lambda_events > 0) {
		return JNI_TRUE;
	} else {
		return JNI_FALSE;
	}
}


JNIEXPORT jint JNICALL Java_FTAGUI_MonteCarloDialog_nativeNumBasicEvents
(JNIEnv *env, jobject obj) {
	ASSERT(tree_pict->tree->num_bas > 0 );
	return(tree_pict->tree->num_bas);
}


JNIEXPORT jfloat JNICALL Java_FTAGUI_MonteCarloDialog_nativeGetTimeEstimate
(JNIEnv *env, jobject obj, jint jnumSims) {
	javaEnv = env;
	generateMonteCarloObj = obj;

	return monte_estimate(tree_pict->tree, jnumSims);
}



void GenerateMonteCarloPostScrolledText(const char *title, const char *fileName){
	(*javaEnv)->CallVoidMethod(javaEnv, generateMonteCarloObj, postScrolledTextMethod, 
		(*javaEnv)->NewStringUTF(javaEnv, title), (*javaEnv)->NewStringUTF(javaEnv, fileName));
}



BOOL GenerateMonteCarloCheckForInterrupt() {
	if((*javaEnv)->CallBooleanMethod(javaEnv, generateMonteCarloObj, checkForInterruptMethod)){
		return TRUE;
	} else {
		return FALSE;
	}
}


void GenerateMonteCarloCloseWorkingDialog() {
	(*javaEnv)->CallVoidMethod(javaEnv, generateMonteCarloObj, closeWorkingDialogMethod);
}

void GenerateMonteCarloSetProgressBarMax(int max) {
	jint jmax;
	jmax = max;
	(*javaEnv)->CallVoidMethod(javaEnv, generateMonteCarloObj, setProgressBarMaxMethod, jmax);
}

void GenerateMonteCarloProgressBarInc() {
	(*javaEnv)->CallVoidMethod(javaEnv, generateMonteCarloObj, progressBarIncMethod);
}


JNIEXPORT void JNICALL Java_FTAGUI_GenerateMonteCarlo_nativeOKButtonAction
(JNIEnv *env, jobject obj, jint jnumSims, jfloat junitTime) {
	javaEnv = env;
	generateMonteCarloObj = obj;

	MonteWidOkCb(tree_pict, jnumSims, junitTime);
 
  	/* Close the busy dialog */
    GenerateMonteCarloCloseWorkingDialog();
}


JNIEXPORT void JNICALL Java_FTAGUI_GenerateMonteCarlo_nativeInitialiseGenerateMonteCarlo
(JNIEnv *env, jobject obj) {
	jclass cls;

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
