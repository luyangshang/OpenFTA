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

#include "TopLevelData.h"
#include "FTAGUI_PEDFrame.h"
#include "NativePEDFrame.h"

#include "primary_events_database.h"
#include "primary_event.h"
#include "PED_shell.h"
#include "FileUtilities.h"
#include "fta.h"
#include "util.h"

#define JAVA_NONE          0
#define JAVA_BASIC         1
#define JAVA_EXTERNAL      2
#define JAVA_UNDEVELOPED   3
#define JAVA_COND_NOT_ANAL 4
#define JAVA_COND_ANAL     5

PRIMARY_EVENTS_DATABASE *current_database;
PRIMARY_EVENT *selected_event;

static jmethodID askQuestionMethod;
static jmethodID postWarningMethod;
static jmethodID addListElementMethod;
static jmethodID clearListMethod;
static jmethodID setWindowTitleMethod;

static JNIEnv *javaEnv;
static jobject javaPEDFrameObj;

JNIEXPORT void JNICALL Java_FTAGUI_PEDFrame_nativeInitialisePEDFrame(JNIEnv *env, jobject obj){

	jclass cls;

	javaEnv = env;
	javaPEDFrameObj = obj;

	current_database = ped_shell_create();

	cls = (*env)->GetObjectClass(env, obj);

	askQuestionMethod = (*env)->GetMethodID(env, cls, "askQuestion", "(Ljava/lang/String;Ljava/lang/String;)Z");
	if(askQuestionMethod == 0) {
			printf("DEBUG: failed to get ask question java method ID\n");
			return;
	}

	postWarningMethod = (*env)->GetMethodID(env, cls, "postWarning", "(Ljava/lang/String;Ljava/lang/String;)V");
	if(postWarningMethod == 0) {
			printf("DEBUG: failed to get post warning java method ID\n");
			return;
	}

	addListElementMethod = (*env)->GetMethodID(env, cls, "addListElement", "(Ljava/lang/String;)V");
	if(addListElementMethod == 0) {
			printf("DEBUG: failed to get add list element java method ID\n");
			return;
	}

	clearListMethod = (*env)->GetMethodID(env, cls, "clearList", "()V");
	if(clearListMethod == 0) {
			printf("DEBUG: failed to get clear list java method ID\n");
			return;
	}
	setWindowTitleMethod = (*env)->GetMethodID(env, cls, "setWindowTitle", "(Ljava/lang/String;)V");
	if(setWindowTitleMethod == 0) {
			printf("DEBUG: failed to get set window title java method ID\n");
			return;
	}
}

JNIEXPORT void JNICALL Java_FTAGUI_PEDFrame_nativeApplyEvent(JNIEnv *env, jobject obj, 
							jstring jid, jfloat jprob, jstring jdesc, 
							jboolean jconstFailure, jboolean jdormant,
							jint jtype, jint joriginalType){

	PrimaryEventType original_type;
	PRIMARY_EVENT new_event;
	const char *temp_str;

	javaPEDFrameObj = obj;
	javaEnv = env;
		
	temp_str = (*env)->GetStringUTFChars(env, jid, 0);
	strcpy(new_event.id, temp_str);
	(*env)->ReleaseStringUTFChars(env, jid, temp_str);

	new_event.type = jtype;
	new_event.dormant = (jdormant ? (char)DORMANT : (char)FALSE);
	new_event.lambda = (jconstFailure ? (char)TRUE : (char)FALSE);
	
	new_event.desc = (char *)(*env)->GetStringUTFChars(env, jdesc, 0);

	new_event.prob = jprob;

	new_event.next = NULL;

	original_type = joriginalType;

	verify_event(current_database, &new_event, original_type);

}


/*
 * Class:     PEDFrame
 * Method:    nativeDeleteEvent
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_FTAGUI_PEDFrame_nativeDeleteEvent(JNIEnv *env, jobject obj, jstring jid) {
	const char *temp_str;

	temp_str = (*env)->GetStringUTFChars(env, jid, 0);

	delete_event(current_database, temp_str);

	(*env)->ReleaseStringUTFChars(env, jid, temp_str);

}


BOOL PEDFrameAskQuestion(const char *question, const char *title){

	BOOL result;
	jboolean jresult;

	jresult = (*javaEnv)->CallBooleanMethod(javaEnv, javaPEDFrameObj, askQuestionMethod,
			(*javaEnv)->NewStringUTF(javaEnv, question), (*javaEnv)->NewStringUTF(javaEnv, title));

	result = jresult;
	return(result);
}



void PEDFramePostWarning(const char *warning, const char *title){

	(*javaEnv)->CallVoidMethod(javaEnv, javaPEDFrameObj, postWarningMethod, 
			(*javaEnv)->NewStringUTF(javaEnv, warning), (*javaEnv)->NewStringUTF(javaEnv, title));
}


void PEDFramePostError(const char *warning, const char *title){
	PEDFramePostWarning(warning, title);
}


JNIEXPORT jboolean JNICALL Java_FTAGUI_PEDFrame_nativeGetConstFailure(JNIEnv *env, jobject obj){
	if((BOOL)selected_event->lambda) {
		return(JNI_TRUE);
	} else {
		return(JNI_FALSE);
	}
}

JNIEXPORT jstring JNICALL Java_FTAGUI_PEDFrame_nativeGetDesc(JNIEnv *env, jobject obj){
	if(selected_event->desc) {
		return((*env)->NewStringUTF(env, selected_event->desc));
	} else {
		return((*env)->NewStringUTF(env, ""));
	}
}


JNIEXPORT jboolean JNICALL Java_FTAGUI_PEDFrame_nativeGetDormant(JNIEnv *env, jobject obj){
	if((BOOL)selected_event->dormant) {
		return(JNI_TRUE);
	} else {
		return(JNI_FALSE);
	}
}


JNIEXPORT jfloat JNICALL Java_FTAGUI_PEDFrame_nativeGetProb(JNIEnv *env, jobject obj){
	jfloat jprob;
	
	jprob = selected_event->prob;
	return(jprob);
}


JNIEXPORT jint JNICALL Java_FTAGUI_PEDFrame_nativeGetType(JNIEnv *env, jobject obj){
	jint jtype;

	jtype = selected_event->type;
	return(jtype);
}


JNIEXPORT void JNICALL Java_FTAGUI_PEDFrame_nativeSetSelectedEvent(JNIEnv *env , jobject obj, jstring jevent){
	char *event;
	List pel ;

	if(jevent) {
	
		/* find event */
		event = (char *)(*env)->GetStringUTFChars(env, jevent, 0);
		pel = current_database->primary_events_list;

	    while (pel != NULL) {
      
			if (strcmp(((PRIMARY_EVENT *)Head_Of(pel))->id, event) == 0 ) {
				selected_event = (PRIMARY_EVENT *)Head_Of(pel);
				break;
			}
			pel = Tail_Of(pel);
		}
		(*env)->ReleaseStringUTFChars(env, jevent, event);	
	} else {
		selected_event = NULL;
	}
}


JNIEXPORT jboolean JNICALL Java_FTAGUI_PEDFrame_nativeEventSelected(JNIEnv *env, jobject obj) {
	if(selected_event) {
		return(JNI_TRUE);
	} else {
		return(JNI_FALSE);
	}
}


char *PEDFrameGetSelectedId() {
	
	if(selected_event) {
		return(selected_event->id);
	} else {
		return(NULL);
	}
}


PRIMARY_EVENTS_DATABASE *NativePEDCurrentPEDState() {
	return current_database;
}


JNIEXPORT void JNICALL Java_FTAGUI_PEDFrame_populateList(JNIEnv *env, jobject obj) {
	char* element;
	List pel;

	javaPEDFrameObj = obj;
	javaEnv = env;

	(*javaEnv)->CallVoidMethod(javaEnv, javaPEDFrameObj, clearListMethod);

	pel = current_database->primary_events_list;
    while ( pel != NULL ) {
		if(( (PRIMARY_EVENT *)Head_Of( pel ) )->id) {
			element = ( (PRIMARY_EVENT *)Head_Of( pel ) )->id;
			(*javaEnv)->CallVoidMethod(javaEnv, javaPEDFrameObj, addListElementMethod, 
									   (*javaEnv)->NewStringUTF(javaEnv, element));
		}
		pel = Tail_Of(pel); 
	}
}

JNIEXPORT jboolean JNICALL
Java_FTAGUI_PEDFrame_nativeLoadPEDFile(JNIEnv *env, jobject obj, jstring jfileName) {

	const char *fileName = (*env)->GetStringUTFChars(env, jfileName, 0);

	javaEnv = env;
	javaPEDFrameObj = obj;
	
	if ( fileName != NULL )
	{

	primary_events_database_open_cb((char*)fileName);

	(*env)->ReleaseStringUTFChars(env, jfileName, fileName);

	return(JNI_TRUE);
	}
	else
	{
	return(JNI_FALSE);
	}

}



JNIEXPORT jboolean JNICALL Java_FTAGUI_PEDFrame_nativeFileExists(JNIEnv *env, jobject obj){
	if(current_database->current_primary_events_database) {
		return JNI_TRUE;
	} else {
		return JNI_FALSE;
	}
}


JNIEXPORT jboolean JNICALL Java_FTAGUI_PEDFrame_nativeSavePEDFile(JNIEnv *env, jobject obj){
	primary_events_database_save_to_file(current_database, current_database->current_primary_events_database);

	return(JNI_TRUE);
}


JNIEXPORT jboolean JNICALL Java_FTAGUI_PEDFrame_nativeSaveAsPEDFile(JNIEnv *env, jobject obj, jstring jfileName){

	const char *fileName = (*env)->GetStringUTFChars(env, jfileName, 0);

	javaEnv = env;
	javaPEDFrameObj = obj;
	
	if(primary_events_database_save_to_file(current_database, (char*)fileName)) {;
        PEDFrameSetWindowTitle(current_database->current_primary_events_database);
	}

	(*env)->ReleaseStringUTFChars(env, jfileName, fileName);

	return(JNI_TRUE);
}


JNIEXPORT jboolean JNICALL Java_FTAGUI_PEDFrame_nativeNewPEDFile(JNIEnv *env, jobject obj, jboolean jconfirm) {
	jboolean result = JNI_FALSE;
	BOOL confirm;

	javaEnv = env;
	javaPEDFrameObj = obj;
    
	if(jconfirm) {
		confirm = TRUE;
	} else {
		confirm = FALSE;
	}

	if(PEDFrameNewPEDFile(confirm)) {
		result = JNI_TRUE;
	}

	return result;
}


BOOL PEDFrameNewPEDFile(BOOL confirm) {

	BOOL result = FALSE;
   
	if(primary_events_database_new_cb(current_database, confirm)){
		if(current_database = ped_shell_create()) {
	        PEDFrameSetWindowTitle(current_database->current_primary_events_database);
		}
	    result = TRUE;
	}

	return result;
}


void PEDFrameSetWindowTitle(char *database_name){

	char* db_name;

	if ( database_name != NULL ) {
		if ( (int)strlen( database_name ) > 1 ) {
			db_name = (char *)filename_from_pathname( database_name );
		} else {
			db_name = strsave( NULL_DB_NAME );
		}
	} else {
		db_name = strsave( NULL_DB_NAME );
	}
      
	(*javaEnv)->CallVoidMethod(javaEnv, javaPEDFrameObj, setWindowTitleMethod, 
		 (*javaEnv)->NewStringUTF(javaEnv, db_name));

	if ( db_name != NULL ) {  
		strfree( db_name );    
		db_name = NULL;  
	}  
}


PRIMARY_EVENTS_DATABASE *native_ped_frame_get_current_database(){
    return current_database;
}
