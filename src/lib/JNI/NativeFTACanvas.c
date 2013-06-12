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

#include "FTAGUI_FTACanvas.h"
#include "NativeFTACanvas.h"
#include "TopLevelData.h"

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
#include "drawingarea.h"
#include "file_pd.h"
#include "FileUtilities.h"
#include "MemoryUtil.h"

static jmethodID fillRectangleMethod;
static jmethodID drawRectangleMethod;
static jmethodID clearArcMethod;
static jmethodID fillArcMethod;
static jmethodID drawArcMethod;
static jmethodID fillPolylineMethod;
static jmethodID drawLineMethod;
static jmethodID drawStringMethod;
static jmethodID setPenColourMethod;
static jmethodID setPenWidthMethod;
static jmethodID getTextWidthMethod;
static jmethodID getFontHeightMethod;
static jmethodID editTreeTextMethod;
static jmethodID editIntermediateEventMethod;
static jmethodID editGateMethod;
static jmethodID editTransferTreeMethod;
static jmethodID selectPrimaryEventMethod;

static JNIEnv *javaEnv;
static jobject javaFTACanvasObj;

/* FTACanvas Native Methods */

JNIEXPORT void JNICALL
Java_FTAGUI_FTACanvas_nativeInitialiseFTACanvas(JNIEnv *env, jobject obj) {

	jclass cls;

	cls = (*env)->GetObjectClass(env, obj);

	fillRectangleMethod = (*env)->GetMethodID(env, cls, "fillRectangle", "(IIIII)V");
	if(fillRectangleMethod == 0) {
			printf("DEBUG: failed to get fill rectangle java method ID\n");
			return;
	}

	drawRectangleMethod = (*env)->GetMethodID(env, cls, "drawRectangle", "(IIII)V");
	if(drawRectangleMethod == 0) {
			printf("DEBUG: failed to get draw rectangle java method ID\n");
			return;
	}

	clearArcMethod = (*env)->GetMethodID(env, cls, "clearArc", "(IIIIII)V");
	if(clearArcMethod == 0) {
			printf("DEBUG: failed to get clear arc java method ID\n");
			return;
	}
	
	fillArcMethod = (*env)->GetMethodID(env, cls, "fillArc", "(IIIIIII)V");
	if(fillArcMethod == 0) {
			printf("DEBUG: failed to get fill arc java method ID\n");
			return;
	}

	drawArcMethod = (*env)->GetMethodID(env, cls, "drawArc", "(IIIIII)V");
	if(drawArcMethod == 0) {
			printf("DEBUG: failed to get draw arc java method ID\n");
			return;
	}

	fillPolylineMethod = (*env)->GetMethodID(env, cls, "fillPolyline", "([I[III)V");
	if(fillPolylineMethod == 0) {
			printf("DEBUG: failed to get fill polyline java method ID\n");
			return;
	}

	drawLineMethod = (*env)->GetMethodID(env, cls, "drawLine", "(IIII)V");
	if(drawLineMethod == 0) {
			printf("DEBUG: failed to get draw line java method ID\n");
			return;
	}

	drawStringMethod = (*env)->GetMethodID(env, cls, "drawString", "(Ljava/lang/String;II)V");
	if(drawStringMethod == 0) {
			printf("DEBUG: failed to get draw string java method ID\n");
			return;
	}

	setPenColourMethod = (*env)->GetMethodID(env, cls, "setPenColour", "(I)V");
	if(setPenColourMethod == 0) {
			printf("DEBUG: failed to get set pen colour java method ID\n");
			return;
	}

	setPenWidthMethod = (*env)->GetMethodID(env, cls, "setPenWidth", "(I)V");
	if(setPenWidthMethod == 0) {
			printf("DEBUG: failed to get set pen width java method ID\n");
			return;
	}

	getTextWidthMethod = (*env)->GetMethodID(env, cls, "getTextWidth", "(Ljava/lang/String;)I");
	if(getTextWidthMethod == 0) {
			printf("DEBUG: failed to get get text width java method ID\n");
			return;
	}

	getFontHeightMethod = (*env)->GetMethodID(env, cls, "getFontHeight", "()I");
	if(getFontHeightMethod == 0) {
			printf("DEBUG: failed to get get text height java method ID\n");
			return;
	}

	editTreeTextMethod = (*env)->GetMethodID(env, cls, "editTreeText", "()V");
	if(editTreeTextMethod == 0) {
			printf("DEBUG: failed to get edit tree text java method ID\n");
			return;
	}

	editIntermediateEventMethod = (*env)->GetMethodID(env, cls, "editIntermediateEvent", "()V");
	if(editIntermediateEventMethod == 0) {
			printf("DEBUG: failed to get edit intermediate event java method ID\n");
			return;
	}

	editGateMethod = (*env)->GetMethodID(env, cls, "editGate", "()V");
	if(editGateMethod == 0) {
			printf("DEBUG: failed to get edit gate java method ID\n");
			return;
	}
	
	editTransferTreeMethod = (*env)->GetMethodID(env, cls, "editTransferTree", "()V");
	if(editTransferTreeMethod == 0) {
			printf("DEBUG: failed to get edit transfer tree java method ID\n");
			return;
	}
	selectPrimaryEventMethod = (*env)->GetMethodID(env, cls, "selectPrimaryEvent", "(Ljava/lang/String;)V");
	if(selectPrimaryEventMethod == 0) {
			printf("DEBUG: failed to get select primary event java method ID\n");
			return;
	}
}

JNIEXPORT void JNICALL
Java_FTAGUI_FTACanvas_nativeDrawTree(JNIEnv *env, jobject obj) {
	javaEnv = env;
	javaFTACanvasObj = obj;

	tree_draw(tree_pict, NULL);

}


void FTACanvasDrawRect(int x, int y, int width, int height){
	jint jx;
	jint jy;
	jint jwidth;
	jint jheight;

	jx = x;
	jy = y;
	jwidth = width;
	jheight = height;

	(*javaEnv)->CallVoidMethod(javaEnv, javaFTACanvasObj, drawRectangleMethod, jx, jy, jwidth, jheight);
}


void FTACanvasFillRect(int x, int y, int width, int height, int type){
	jint jx;
	jint jy;
	jint jwidth;
	jint jheight;
	jint jtype;

	jx = x;
	jy = y;
	jwidth = width;
	jheight = height;
	jtype = type;

	(*javaEnv)->CallVoidMethod(javaEnv, javaFTACanvasObj, fillRectangleMethod, jx, jy, jwidth, jheight, jtype);
}


void FTACanvasDrawLine(int x1, int y1, int x2, int y2){
	jint jx1;
	jint jy1;
	jint jx2;
	jint jy2;

	jx1 = x1;
	jy1 = y1;
	jx2 = x2;
	jy2 = y2;

	(*javaEnv)->CallVoidMethod(javaEnv, javaFTACanvasObj, drawLineMethod, jx1, jy1, jx2, jy2);
}


void FTACanvasFillPolyline(const CanvasCoord *p, int length, int type){
	int i;
	jintArray jx;
	jintArray jy;
	jint jlength;
	jint jtype;
	int temp;

	jx = (*javaEnv)->NewIntArray(javaEnv, length);
	jy = (*javaEnv)->NewIntArray(javaEnv, length);
	for(i = 0; i<length; i++) {
		temp = (int)p[i].x;
		(*javaEnv)->SetIntArrayRegion(javaEnv, jx, i, 1, &(temp));
		temp = (int)p[i].y;
		(*javaEnv)->SetIntArrayRegion(javaEnv, jy, i, 1, &(temp));
	}

	jlength = length;
	jtype = type;

	(*javaEnv)->CallVoidMethod(javaEnv, javaFTACanvasObj, fillPolylineMethod, jx, jy, jlength, jtype);
}


void FTACanvasClearArc(int x, int y, int width, int height, int start_angle, int end_angle){

	jint jx, jy, jwidth, jheight, jstart_angle, jend_angle;

	jx = x;
	jy = y;
	jwidth = width;
	jheight = height;
	jstart_angle = start_angle;
	jend_angle = end_angle;

	(*javaEnv)->CallVoidMethod(javaEnv, javaFTACanvasObj, clearArcMethod,
			jx, jy, jwidth, jheight, jstart_angle, jend_angle);
}


void FTACanvasFillArc(int x, int y, int width, int height, int start_angle, int end_angle, int symbol_type){

	jint jx, jy, jwidth, jheight, jstart_angle, jend_angle, jtype;

	jx = x;
	jy = y;
	jwidth = width;
	jheight = height;
	jstart_angle = start_angle;
	jend_angle = end_angle;
	jtype = symbol_type;

	(*javaEnv)->CallVoidMethod(javaEnv, javaFTACanvasObj, fillArcMethod,
			jx, jy, jwidth, jheight, jstart_angle, jend_angle, jtype);
}


void FTACanvasDrawArc(int x, int y, int width, int height, int start_angle, int end_angle){

	jint jx, jy, jwidth, jheight, jstart_angle, jend_angle;

	jx = x;
	jy = y;
	jwidth = width;
	jheight = height;
	jstart_angle = start_angle;
	jend_angle = end_angle;

	(*javaEnv)->CallVoidMethod(javaEnv, javaFTACanvasObj, drawArcMethod,
			jx, jy, jwidth, jheight, jstart_angle, jend_angle);
}


void FTACanvasWriteText(int x, int y, const char* text){

	jint jx, jy;

	jx = x;
	jy = y;

	(*javaEnv)->CallVoidMethod(javaEnv, javaFTACanvasObj, drawStringMethod,
			(*javaEnv)->NewStringUTF(javaEnv, text), jx, jy);

}


void FTACanvasSetPenWidth(unsigned width){
	jint jwidth;

	jwidth = width;

	(*javaEnv)->CallVoidMethod(javaEnv, javaFTACanvasObj, setPenWidthMethod, jwidth);
}


void FTACanvasSetPenColour(unsigned long colour){
	jint jcolour;

	jcolour = colour;

	(*javaEnv)->CallVoidMethod(javaEnv, javaFTACanvasObj, setPenColourMethod, jcolour);
}


int FTACanvasTextWidth(const char* text){

	jint jwidth;
	int width;

	jwidth =  (*javaEnv)->CallIntMethod(javaEnv, javaFTACanvasObj, getTextWidthMethod, 
			(*javaEnv)->NewStringUTF(javaEnv, text));
	width = jwidth;
	return width;
}

int FTACanvasFontHeight(){
	jint jheight;
	int height;

	jheight= (*javaEnv)->CallIntMethod(javaEnv, javaFTACanvasObj, getFontHeightMethod);
	height = jheight;
	return height;
}


void FTACanvasSelectPrimaryEvent(const char* id){

	(*javaEnv)->CallVoidMethod(javaEnv, javaFTACanvasObj, selectPrimaryEventMethod, 
			(*javaEnv)->NewStringUTF(javaEnv, id));
}


JNIEXPORT jboolean JNICALL Java_FTAGUI_FTACanvas_nativeSelectItem(JNIEnv *env, jobject obj, jint jx, jint jy){
	
	int x,y;
	ITEM *old_selected;

	x = jx;
	y = jy;

	old_selected = tree_pict->selected_item;

	tree_pict->selected_item = find_item(tree_pict, tree.top_level, x, y);

	if(old_selected != tree_pict->selected_item) {
		return JNI_TRUE;
	} else {
		return JNI_FALSE;
	}
}


JNIEXPORT jint JNICALL Java_FTAGUI_FTACanvas_nativeGetMaxWidth(JNIEnv *env, jobject obj){
	jint maxWidth;

	maxWidth = getMaxWidth();
	return maxWidth;
}


JNIEXPORT jint JNICALL Java_FTAGUI_FTACanvas_nativeGetMaxDepth(JNIEnv *env, jobject obj){
	jint maxDepth;

	maxDepth = getMaxDepth();
	return maxDepth;
}


JNIEXPORT void JNICALL Java_FTAGUI_FTACanvas_nativeEditSelected(JNIEnv *env, jobject obj){
	javaEnv = env;
	javaFTACanvasObj = obj;

	edit_selected_item(&tree_pic_forest);

}


void FTACanvasEditTreeText() {

	(*javaEnv)->CallVoidMethod(javaEnv, javaFTACanvasObj, editTreeTextMethod);

}


void FTACanvasEditIntermediateEvent() {

	(*javaEnv)->CallVoidMethod(javaEnv, javaFTACanvasObj, editIntermediateEventMethod);

}


void FTACanvasEditGate() {

	(*javaEnv)->CallVoidMethod(javaEnv, javaFTACanvasObj, editGateMethod);

}


void FTACanvasEditTransferTree() {

	(*javaEnv)->CallVoidMethod(javaEnv, javaFTACanvasObj, editTransferTreeMethod);

}

