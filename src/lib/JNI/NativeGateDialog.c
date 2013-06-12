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

#include "FTAGUI_GateDialog.h"
#include "NativeGateDialog.h"
#include "TopLevelData.h"
#include "drawingarea.h"

static JNIEnv *javaEnv;
static jobject javaGateDialogObj;

static jmethodID postWarningMethod;


JNIEXPORT void JNICALL
Java_FTAGUI_GateDialog_nativeInitialiseGateDialog(JNIEnv *env, jobject obj) {

	jclass cls;

	cls = (*env)->GetObjectClass(env, obj);

	postWarningMethod = (*env)->GetMethodID(env, cls, "postWarning", "(Ljava/lang/String;Ljava/lang/String;)V");
	if(postWarningMethod == 0) {
			printf("DEBUG: failed to get post warning java method ID\n");
			return;
	}

}


JNIEXPORT jint JNICALL Java_FTAGUI_GateDialog_nativeGetType(JNIEnv *env, jobject obj) {

	return(tree_pict->selected_item->type);

}


JNIEXPORT jstring JNICALL Java_FTAGUI_GateDialog_nativeGetID(JNIEnv *env, jobject obj) {
	if(tree_pict->selected_item->id) {
		return((*env)->NewStringUTF(env, tree_pict->selected_item->id));
	} else {
		return((*env)->NewStringUTF(env, ""));
	}
}


JNIEXPORT void JNICALL Java_FTAGUI_GateDialog_nativeOKButtonAction
(JNIEnv *env, jobject obj, jstring jid, jint jtype) {

	const char *id = (*env)->GetStringUTFChars(env, jid, 0);

	javaEnv = env;
	javaGateDialogObj = obj;

	DrawingAreaGateOkCb(&tree_pic_forest, (char*)id, jtype);

	(*env)->ReleaseStringUTFChars(env, jid, id);
}


void GateDialogPostWarning(const char *warning, const char *title){

	(*javaEnv)->CallVoidMethod(javaEnv, javaGateDialogObj, postWarningMethod, 
			(*javaEnv)->NewStringUTF(javaEnv, warning), (*javaEnv)->NewStringUTF(javaEnv, title));
}


void GateDialogPostError(const char *warning, const char *title){
	GateDialogPostWarning(warning, title);
}
