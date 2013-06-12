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

#include "FTAGUI_TreeTextDialog.h"
#include "TopLevelData.h"
#include "drawingarea.h"

JNIEXPORT jstring JNICALL Java_FTAGUI_TreeTextDialog_nativeGetDesc(JNIEnv *env, jobject obj) {
	if(tree_pict->tree->post_it_note) {
		return((*env)->NewStringUTF(env, tree_pict->tree->post_it_note));
	} else {
		return((*env)->NewStringUTF(env, ""));
	}
}


JNIEXPORT void JNICALL Java_FTAGUI_TreeTextDialog_nativeOKButtonAction
(JNIEnv *env, jobject obj, jstring jdesc) {

	const char *desc = (*env)->GetStringUTFChars(env, jdesc, 0);

	DrawingAreaPostItTextCb(&tree_pic_forest, (char*)desc);

	(*env)->ReleaseStringUTFChars(env, jdesc, desc);
}
