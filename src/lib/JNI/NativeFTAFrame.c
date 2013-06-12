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

#include "FTAGUI_FTAFrame.h"
#include "NativeFTAFrame.h"
#include "TopLevelData.h"

#include "analysis_pd.h"
#include "view_pd.h"
#include "print_report_pd.h"
#include "treeP.h"
#include "util.h"
#include "treeUtil.h"
#include "tree_pic.h"
#include "tree_pic_forest.h"
#include "symbol.h"
#include "symbol_defs.h"
#include "Canvas.h"
#include "CanvasP.h"
#include "display_tree.h"
#include "symbol_menu.h"
#include "edit_pd.h"
#include "file_pd.h"
#include "FileUtilities.h"
#include "MemoryUtil.h"
#include "PED_shell.h"

TREE tree;
TREE_PIC *tree_pict;
TREE_PIC_FOREST tree_pic_forest;


static jmethodID activateButtonsMethod;
static jmethodID setButtonMethod;
static jmethodID setAnalysisAndReportMenuItemMethod;
static jmethodID askQuestionMethod;
static jmethodID postWarningMethod;
static jmethodID postScrolledTextMethod;
static jmethodID printTextFileMethod;
static jmethodID setWindowTitleMethod;
static jmethodID invokeCutSetsDialogMethod;
static jmethodID invokeNumericalProbabilityDialogMethod;
static jmethodID invokeMonteCarloDialogMethod;
static jmethodID loadPEDFileMethod;
static jmethodID newPEDFileMethod;

static JNIEnv *javaEnv;
static jobject javaFTAFrameObj;

/* FTAFrame Native Methods */

JNIEXPORT void JNICALL
Java_FTAGUI_FTAFrame_nativeInitialiseFTAFrame(JNIEnv *env, jobject obj) {

	int                symbol_type;
	jclass cls;

	javaFTAFrameObj = obj;

	tree_new(&tree);
	tree.database = NULL;
	replant_tree(&tree);

	tree_pict = (TREE_PIC*)malloc(sizeof(TREE_PIC));

    tree_pict->tree = &tree;
    tree_pict->selected_item = NULL;
	tree_pict->scale = 100;
/*	tree_pict->canvas = (Canvas)malloc(sizeof(Canvas_rec));*/
	tree_pict->canvas = CanvasCreate();
	tree_pict->canvas->type = JAVA_CANVAS;
	tree_pict->set_no = 0;

	update_tree(tree_pict);

	tree_pic_forest.tree_pic = tree_pict;
	tree_pic_forest.global_tree_pic = NULL;

	
	/* Initialise scaled_symbols */
    for ( 
      symbol_type = SYMB_BASIC; 
      symbol_type < SYMB_INVALID;
      symbol_type++ ) 
    {
      tree_pict->scaled_symbols[symbol_type] =
        scale_symbol(symbol_defs[symbol_type], (float)tree_pict->scale);
    }

	cls = (*env)->GetObjectClass(env, obj);
	activateButtonsMethod = (*env)->GetMethodID(env, cls, "activateButtons", "()V");
	if(activateButtonsMethod == 0) {
			printf("DEBUG: failed to get activate buttons java method ID\n");
			return;
	}
	setButtonMethod = (*env)->GetMethodID(env, cls, "setButton", "(IZ)V");
	if(setButtonMethod == 0) {
			printf("DEBUG: failed to get set button java method ID\n");
			return;
	}
	setAnalysisAndReportMenuItemMethod = (*env)->GetMethodID(env, cls, "setAnalysisAndReportMenuItem", "(IZ)V");
	if(setAnalysisAndReportMenuItemMethod == 0) {
			printf("DEBUG: failed to get set analysis and report menu item java method ID\n");
			return;
	}
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
	postScrolledTextMethod = (*env)->GetMethodID(env, cls, "postScrolledText", "(Ljava/lang/String;Ljava/lang/String;)V");
	if(postScrolledTextMethod == 0) {
			printf("DEBUG: failed to get post scrolled text java method ID\n");
			return;
	}
	printTextFileMethod = (*env)->GetMethodID(env, cls, "printTextFile", "(Ljava/lang/String;)V");
	if(printTextFileMethod == 0) {
			printf("DEBUG: failed to get print text file java method ID\n");
			return;
	}
	setWindowTitleMethod = (*env)->GetMethodID(env, cls, "setWindowTitle", "(Ljava/lang/String;Ljava/lang/String;)V");
	if(setWindowTitleMethod == 0) {
			printf("DEBUG: failed to get set window title java method ID\n");
			return;
	}
	invokeCutSetsDialogMethod = (*env)->GetMethodID(env, cls, "invokeCutSetsDialog", "()V");
	if(invokeCutSetsDialogMethod == 0) {
			printf("DEBUG: failed to get invoke cut sets dialog java method ID\n");
			return;
	}
	invokeNumericalProbabilityDialogMethod = (*env)->GetMethodID(env, cls, "invokeNumericalProbabilityDialog", "()V");
	if(invokeNumericalProbabilityDialogMethod == 0) {
			printf("DEBUG: failed to get invoke numerical probability dialog java method ID\n");
			return;
	}
	invokeMonteCarloDialogMethod = (*env)->GetMethodID(env, cls, "invokeMonteCarloDialog", "()V");
	if(invokeMonteCarloDialogMethod == 0) {
			printf("DEBUG: failed to get invoke monte carlo dialog java method ID\n");
			return;
	}
	loadPEDFileMethod = (*env)->GetMethodID(env, cls, "loadPEDFile", "(Ljava/lang/String;)V");
	if(loadPEDFileMethod == 0) {
			printf("DEBUG: failed to get load PED file java method ID\n");
			return;
	}
	newPEDFileMethod = (*env)->GetMethodID(env, cls, "newPEDFile", "()V");
	if(newPEDFileMethod == 0) {
			printf("DEBUG: failed to get new PED file java method ID\n");
			return;
	}
}


JNIEXPORT jboolean JNICALL
Java_FTAGUI_FTAFrame_nativeLoadFTAFile(JNIEnv *env, jobject obj, jstring jfileName) {

	const char *fileName = (*env)->GetStringUTFChars(env, jfileName, 0);

	javaEnv = env;
	javaFTAFrameObj = obj;

#ifdef DEBUG
    printf("Num Allocated Memory Blocks Before load_tree_cb = %d\n", numAllocatedBlocks());
#endif

	file_load(&tree_pic_forest, (char*)fileName);

#ifdef DEBUG
    printf("Num Allocated Memory Blocks After load_tree_cb = %d\n", numAllocatedBlocks());
#endif

	(*env)->ReleaseStringUTFChars(env, jfileName, fileName);

	return(JNI_TRUE);
}


JNIEXPORT jboolean JNICALL Java_FTAGUI_FTAFrame_nativeFileExists(JNIEnv *env, jobject obj){
	if(tree_pict->tree->name) {
		return JNI_TRUE;
	} else {
		return JNI_FALSE;
	}
}


JNIEXPORT jboolean JNICALL Java_FTAGUI_FTAFrame_nativeSaveFTAFile(JNIEnv *env, jobject obj){
    char *filename;
 
#ifdef DEBUG
    printf("Num Allocated Memory Blocks Before save_tree_to_file = %d\n", numAllocatedBlocks());
#endif
	
	filename = (char*) malloc((strlen(tree_pict->tree->name) + strlen(tree_pict->tree->path) + 1) * sizeof(char));

    if(filename) {
        strcpy(filename, tree_pict->tree->path);
        strcat(filename, tree_pict->tree->name);
        save_tree_to_file(tree_pict, filename);
        free(filename);
    }

#ifdef DEBUG
    printf("Num Allocated Memory Blocks After save_tree_to_file = %d\n", numAllocatedBlocks());
#endif

	return(JNI_TRUE);
}


JNIEXPORT jboolean JNICALL Java_FTAGUI_FTAFrame_nativeSaveAsFTAFile(JNIEnv *env, jobject obj, jstring jfileName){

	const char *fileName = (*env)->GetStringUTFChars(env, jfileName, 0);

	javaEnv = env;
	javaFTAFrameObj = obj;
	
#ifdef DEBUG
    printf("Num Allocated Memory Blocks Before save_tree_to_file = %d\n", numAllocatedBlocks());
#endif

	if(save_tree_to_file(tree_pict, (char*)fileName) == TREE_OK) {
        FTAFrameSetWindowTitle( tree_pic_forest.tree_pic );
	}

	(*env)->ReleaseStringUTFChars(env, jfileName, fileName);

#ifdef DEBUG
	printf("Num Allocated Memory Blocks After save_tree_to_file = %d\n", numAllocatedBlocks());
#endif

	return(JNI_TRUE);
}


JNIEXPORT jboolean JNICALL Java_FTAGUI_FTAFrame_nativeCloseFile(JNIEnv *env, jobject obj){
	javaEnv = env;
	javaFTAFrameObj = obj;

	if (file_close(&tree_pic_forest) == 1) {
		return JNI_TRUE;
	} else {
		return JNI_FALSE;
	}
}



/*
 * Class:     FTAFrame
 * Method:    nativeDatabaseAction
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_FTAGUI_FTAFrame_nativeDatabaseAction(JNIEnv *env, jobject obj){

    char *ped_shell_db = NULL;

	javaEnv = env;
	javaFTAFrameObj = obj;

#ifdef DEBUG
    printf("Num Allocated Memory Blocks Before nativeDatabaseAction = %d\n", numAllocatedBlocks());
#endif

    if ( tree_pic_forest.tree_pic->tree->database != NULL )
    {
        strfree( tree_pic_forest.tree_pic->tree->database );
        tree_pic_forest.tree_pic->tree->database = NULL;
    }
    ped_shell_db = ped_shell_first();
    if ( ped_shell_db != NULL )
	{ 
	    tree_pic_forest.tree_pic->tree->database =
	    strsave( ped_shell_db );
	}
    else
	{
	    tree_pic_forest.tree_pic->tree->database = ped_shell_db;
	}

    database_modified_callback( &tree_pic_forest );
    tree_edited( tree_pic_forest.tree_pic->tree );

 	FTAFrameSetWindowTitle( tree_pic_forest.tree_pic );

#ifdef DEBUG
    printf("Num Allocated Memory Blocks After nativeDatabaseAction = %d\n", numAllocatedBlocks());
#endif

    return(JNI_TRUE);

}



JNIEXPORT void JNICALL
Java_FTAGUI_FTAFrame_nativeNewFTAFile(JNIEnv *env, jobject obj) {

 	javaEnv = env;
	javaFTAFrameObj = obj;

#ifdef DEBUG
    printf("Num Allocated Memory Blocks Before file_new = %d\n", numAllocatedBlocks());
#endif

	file_new(&tree_pic_forest);

	update_tree(tree_pict);

#ifdef DEBUG
    printf("Num Allocated Memory Blocks After file_new = %d\n", numAllocatedBlocks());
#endif

}

JNIEXPORT jint JNICALL Java_FTAGUI_FTAFrame_nativeGetSelectedItemType(JNIEnv *env, jobject jobj){
	jint type;

	if(tree_pict->selected_item) {
		type = tree_pict->selected_item->type;
	} else {
		type = 0;
	}

	return type;

}



JNIEXPORT void JNICALL Java_FTAGUI_FTAFrame_nativeAddItem(JNIEnv *env, jobject jobj, jint jtype){

	int type;
    ITEM *symbol;

	javaEnv = env;
	javaFTAFrameObj = jobj;

	type = jtype;

	symbol = make_item();
	if ( set_symbol_type( symbol, type ) )
	{
		if ( tree_pict->selected_item == NULL )
		{
			FreeMemory( symbol );

			return;
		}

		if ( !tree_add( 
			symbol,
			tree_pict->selected_item,
			tree_pict ) )
		{
			FreeMemory( symbol );
			return;
		}
	}
}


JNIEXPORT void JNICALL Java_FTAGUI_FTAFrame_nativeUndoAction(JNIEnv *env, jobject obj){
	javaEnv = env;
	javaFTAFrameObj = obj;

	EditUndoAction(&tree_pic_forest);
}


JNIEXPORT void JNICALL Java_FTAGUI_FTAFrame_nativeCutAction(JNIEnv *env, jobject obj){
	javaEnv = env;
	javaFTAFrameObj = obj;

	EditCutAction(&tree_pic_forest);
}


JNIEXPORT void JNICALL Java_FTAGUI_FTAFrame_nativeCopyAction(JNIEnv *env, jobject obj){
	javaEnv = env;
	javaFTAFrameObj = obj;

	EditCopyAction(&tree_pic_forest);
}


JNIEXPORT void JNICALL Java_FTAGUI_FTAFrame_nativePasteAction(JNIEnv *env, jobject obj){
	javaEnv = env;
	javaFTAFrameObj = obj;

	EditPasteAction(&tree_pic_forest);
}


JNIEXPORT void JNICALL Java_FTAGUI_FTAFrame_nativeDeleteAction(JNIEnv *env, jobject obj){
	javaEnv = env;
	javaFTAFrameObj = obj;

	EditDeleteAction(&tree_pic_forest);
}

JNIEXPORT void JNICALL Java_FTAGUI_FTAFrame_nativeShiftLeftAction(JNIEnv *env, jobject obj){
	javaEnv = env;
	javaFTAFrameObj = obj;

	EditShiftLeftAction(&tree_pic_forest);
}


JNIEXPORT void JNICALL Java_FTAGUI_FTAFrame_nativeShiftRightAction(JNIEnv *env, jobject obj){
	javaEnv = env;
	javaFTAFrameObj = obj;

	EditShiftRightAction(&tree_pic_forest);
}


JNIEXPORT void JNICALL Java_FTAGUI_FTAFrame_nativeLinkAction(JNIEnv *env, jobject obj){
	javaEnv = env;
	javaFTAFrameObj = obj;

#ifdef DEBUG
    printf("Num Allocated Memory Blocks Before EditLinkAction = %d\n", numAllocatedBlocks());
#endif

	EditLinkAction(tree_pict, selected_event);

#ifdef DEBUG
	printf("Num Allocated Memory Blocks After EditLinkAction = %d\n", numAllocatedBlocks());
#endif
}



JNIEXPORT void JNICALL Java_FTAGUI_FTAFrame_nativeSetSensitivity(JNIEnv *env, jobject obj){
	javaEnv = env;
	javaFTAFrameObj = obj;

	set_symbols_sens(&tree_pic_forest, TreePicGetSelectedItem(tree_pic_forest.tree_pic));
}



JNIEXPORT void JNICALL Java_FTAGUI_FTAFrame_nativeSetAnalysisAndReportMenu(JNIEnv *env, jobject obj){

	printf("Native FTAFrame: Checking for report files\n");

	if(tree_pict->tree->mcs_exists) {
		(*env)->CallVoidMethod(env, obj, setAnalysisAndReportMenuItemMethod, ViewQualitative, JNI_TRUE);	
	} else {
		(*env)->CallVoidMethod(env, obj, setAnalysisAndReportMenuItemMethod, ViewQualitative, JNI_FALSE);
	}

	if(tree_pict->tree->probs_exists) {
		(*env)->CallVoidMethod(env, obj, setAnalysisAndReportMenuItemMethod, ViewProbability, JNI_TRUE);	
	} else {
		(*env)->CallVoidMethod(env, obj, setAnalysisAndReportMenuItemMethod, ViewProbability, JNI_FALSE);
	}

	if(tree_pict->tree->monte_exists) {
		(*env)->CallVoidMethod(env, obj, setAnalysisAndReportMenuItemMethod, ViewMonteCarlo, JNI_TRUE);
	} else {
		(*env)->CallVoidMethod(env, obj, setAnalysisAndReportMenuItemMethod, ViewMonteCarlo, JNI_FALSE);
	}
}

JNIEXPORT void JNICALL Java_FTAGUI_FTAFrame_nativeAnalysisMenu(JNIEnv *env, jobject obj, jint jmenu_item) {
	javaEnv = env;
	javaFTAFrameObj = obj;

#ifdef DEBUG
    printf("Num Allocated Memory Blocks Before analysis_menu_cb = %d\n", numAllocatedBlocks());
#endif

	analysis_menu_cb(&tree_pic_forest, jmenu_item);

#ifdef DEBUG
    printf("Num Allocated Memory Blocks After analysis_menu_cb = %d\n", numAllocatedBlocks());
#endif
}


JNIEXPORT void JNICALL Java_FTAGUI_FTAFrame_nativeViewMenu(JNIEnv *env, jobject obj, jint jmenu_item) {
	javaEnv = env;
	javaFTAFrameObj = obj;

#ifdef DEBUG
    printf("Num Allocated Memory Blocks Before view_menu_cb = %d\n", numAllocatedBlocks());
#endif

	view_menu_cb(&tree_pic_forest, jmenu_item);

#ifdef DEBUG
	printf("Num Allocated Memory Blocks After view_menu_cb = %d\n", numAllocatedBlocks());
#endif
}


JNIEXPORT void JNICALL Java_FTAGUI_FTAFrame_nativePrintMenu(JNIEnv *env, jobject obj, jint jmenu_item) {
	javaEnv = env;
	javaFTAFrameObj = obj;

#ifdef DEBUG
    printf("Num Allocated Memory Blocks Before print_menu_cb = %d\n", numAllocatedBlocks());
#endif

	print_menu_cb(&tree_pic_forest, jmenu_item);

#ifdef DEBUG
	printf("Num Allocated Memory Blocks After print_menu_cb = %d\n", numAllocatedBlocks());
#endif
}


void FTAFrameSetSensitive(int pos, BOOL on) {
	jint jpos;
	jboolean jon;

	jpos = pos;
	jon = on;
	
	(*javaEnv)->CallVoidMethod(javaEnv, javaFTAFrameObj, setButtonMethod, jpos, jon);
}


BOOL FTAFrameAskQuestion(const char *question, const char *title){

	BOOL result;
	jboolean jresult;

	jresult = (*javaEnv)->CallBooleanMethod(javaEnv, javaFTAFrameObj, askQuestionMethod,
			(*javaEnv)->NewStringUTF(javaEnv, question), (*javaEnv)->NewStringUTF(javaEnv, title));

	result = jresult;
	return(result);
}

void FTAFramePostWarning(const char *warning, const char *title){

	(*javaEnv)->CallVoidMethod(javaEnv, javaFTAFrameObj, postWarningMethod, 
			(*javaEnv)->NewStringUTF(javaEnv, warning), (*javaEnv)->NewStringUTF(javaEnv, title));
}


void FTAFramePostError(const char *warning, const char *title){
	FTAFramePostWarning(warning, title);
}


void FTAFramePostScrolledText(const char *title, const char *fileName){
	(*javaEnv)->CallVoidMethod(javaEnv, javaFTAFrameObj, postScrolledTextMethod, 
		(*javaEnv)->NewStringUTF(javaEnv, title), (*javaEnv)->NewStringUTF(javaEnv, fileName));
}

void FTAFramePrintTextFile(const char *fileName){
	(*javaEnv)->CallVoidMethod(javaEnv, javaFTAFrameObj, printTextFileMethod, 
		(*javaEnv)->NewStringUTF(javaEnv, fileName));
}

void FTAFrameInvokeCutSetsDialog(){
	(*javaEnv)->CallVoidMethod(javaEnv, javaFTAFrameObj, invokeCutSetsDialogMethod);
}


void FTAFrameInvokeNumericalProbabilityDialog(){
	(*javaEnv)->CallVoidMethod(javaEnv, javaFTAFrameObj, invokeNumericalProbabilityDialogMethod);
}


void FTAFrameInvokeMonteCarloDialog(){
	(*javaEnv)->CallVoidMethod(javaEnv, javaFTAFrameObj, invokeMonteCarloDialogMethod);
}


void FTAFrameNewPEDFile(){
	(*javaEnv)->CallVoidMethod(javaEnv, javaFTAFrameObj, newPEDFileMethod);
}


void FTAFrameLoadPEDFile(const char *file){
	(*javaEnv)->CallVoidMethod(javaEnv, javaFTAFrameObj, loadPEDFileMethod, (*javaEnv)->NewStringUTF(javaEnv, file));
}


void FTAFrameSetWindowTitle(TREE_PIC *tp){
	char     title[150];
	char    *tree_name, *db_name;

	if ( tp->tree->name != NULL ) {
		if ( (int)strlen( tp->tree->name ) > 1 ) {
			tree_name = (char *)filename_from_pathname( tp->tree->name );
		} else {
			tree_name = strsave( NULL_TREE_NAME );
		}
	} else {
		tree_name = strsave( NULL_TREE_NAME );
	}

	if ( tp->tree->database != NULL ) {
		if ( (int)strlen( tp->tree->database ) > 1 ) {
			db_name = (char *)filename_from_pathname( tp->tree->database );
		} else {
			db_name = strsave( NULL_DB_NAME );
		}
	} else {
		db_name = strsave( NULL_DB_NAME );
	}
      
	sprintf(title, "Tree: %-50s     Database: %-50s", tree_name, db_name);

	(*javaEnv)->CallVoidMethod(javaEnv, javaFTAFrameObj, setWindowTitleMethod, 
			(*javaEnv)->NewStringUTF(javaEnv, tree_name), 
			(*javaEnv)->NewStringUTF(javaEnv, db_name));

	if ( tree_name != NULL ) {  
		strfree( tree_name );    
		tree_name = NULL;  
	}  

	if ( db_name != NULL ) {  
		strfree( db_name );    
		db_name = NULL;  
	}  
}


