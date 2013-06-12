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
 Module : File Dialogs
 Author : FSC Limited
 Date   : 5/8/96
 
 SccsId : @(#)FileDialogs.c	1.8 02/04/97
 
 Origin : FTA project,
          FSC Limited Private Venture.
 
 This module provides the functionality for the standard file
 open/saving.
***************************************************************/

#include <stdlib.h>
#include <stdio.h>

#ifndef WIN32
#include <unistd.h>
#endif

#include "FileDialogs.h"
#include "FileUtilities.h"
#include "fta.h"
#include "Dialogs.h"
#include "util.h"
#include "NativeFTAFrame.h"
#include "NativePEDFrame.h"

#include "StringUtilities.h"

#include "MemoryUtil.h"

/* #include "WorkingDialog.h" */

/* #include "Assert.h" */
/*  */
/* ASSERTFILE(__FILE__) */

#define charset XmSTRING_DEFAULT_CHARSET

#define FILE_EXISTS_QUESTION \
"WARNING: File already exists.\n\n\
Overwrite the file ?"

#define FILENAME_ERROR "ERROR: invalid file name"
#define FILE_OPEN_ERROR "ERROR: error opening file "
#define SAVE_ERROR "ERROR: File not saved - "

/* #define FILE_EXISTS 0 */

/* Define the structure of the client data passed between
   instantiating procedure and the callback instances */
typedef struct _fstruct {
  char *filename;
  char *title;
  char *filter;
  void *data_type;  
  int (*action_procedure)(void *, char *);  
  char *file_error_string;
  char *file_dialog_title;
  int  mode;
} fstruct;



/*---------------------------------------------------------------
 Routine : end_thread_cb
 Purpose : To perform the actions required at the end of a thread.

 Frees any allocated memory.
---------------------------------------------------------------*/
static void
  end_thread_cb(
    fstruct *client_data )
{
  strfree( client_data->file_dialog_title ); 
  strfree( client_data->file_error_string ); 
  FreeMemory( client_data );

  return;
} /* end_thread_cb */


/*---------------------------------------------------------------
 Routine : filename_error
 Purpose : Routine called when a filename error is detected. Posts
 and error dialog.
---------------------------------------------------------------*/
static void
  filename_error(
    char *filename,
    fstruct *client_data )
{
	printf("FileDialogs: filename_error\n");

	free( filename ); 

	FTAFramePostError(FILENAME_ERROR, client_data->file_dialog_title ); 

	end_thread_cb(client_data);

} /* filename_error */


/*---------------------------------------------------------------
 Routine : select_filename_and_act_cb
 Purpose : Routine that allows selection of a file and the subsequent
 read/write of data from/to that file.

 Post selection dialog with callback on 'OK' to perform
 the correct action on the data.
---------------------------------------------------------------*/
static void
  select_filename_and_act_cb(
    fstruct *client_data)
{

  // This needs to be filled in, no time pre-Xmas

} /* select_filename_and_act_cb */



/*---------------------------------------------------------------
 Routine : file_open_error
 Purpose : This routine is called if a file open error is detected.
---------------------------------------------------------------*/
static void file_open_error(fstruct *client_data, int called_from )
{
	char *error_message;

	error_message = strsave( client_data->file_error_string );
	error_message = 
		strgrow( error_message, filename_from_pathname( client_data->filename ) );

	client_data->file_dialog_title = 
		strgrow( client_data->file_dialog_title, "Open " );
	client_data->file_dialog_title = 
		strgrow( client_data->file_dialog_title, client_data->title );

	printf("FileDialogs: file_open_error\n");

	if(called_from == FD_FTA) {
		/* routine called from FTA code */
		FTAFramePostError(error_message, client_data->file_dialog_title);
	} else if(called_from == FD_PED) {
		/* routine called from PED code */
		PEDFramePostError(error_message, client_data->file_dialog_title);
	}

} /* file_open_error */
    


/*---------------------------------------------------------------
 Routine : load_file_cb
 Purpose : This routine performs the action of reading the
 selected file to obtain the required data. The format of the
 file, and the type of the data is only known to the registered
 action procedure.

 If the filename presented is NULL, the file selection dialog
 is posted to allow the selection of a filename.

 If any errors occur the relevent error dialogs are posted. 
---------------------------------------------------------------*/
static void load_file_cb(fstruct *client_data, int called_from)
{

	if ( client_data->filename == NULL ) {
		select_filename_and_act_cb(client_data);
	} else {
		if ( !file_exists( client_data->filename ) ) {
			/* file given, does not exist */

			file_open_error( client_data, called_from );

		} else {

			if ( (*(client_data->action_procedure))(
					client_data->data_type, client_data->filename) ==
				FILE_ERROR ) 
			{
				file_open_error( client_data, called_from );
			}

			end_thread_cb(client_data);

		} /* end if file exists */
	}
} /* load_file_cb */



/*---------------------------------------------------------------
 Routine : load_from_file
 Purpose : Routine called to load data from a file.
 This routine enforces the functionality required for file
 selection through dialogs.
---------------------------------------------------------------*/
void load_from_file(
    char   *filename, 
    char   *appln_title,
    char   *title,
    char   *filter,
    char   *existing_data,
    void   *data_type,
    int    (*read_data_procedure)(void *, char *),
	int     called_from)
{
	fstruct *client_data;

	if ( !fNewMemory( (void *)&client_data, sizeof( fstruct ) ) )
	{
		exit( 1 );
	}

	/* Set up the client data for use in callback */
	client_data->action_procedure = read_data_procedure;
	client_data->title = title;
	client_data->filter = filter;
	client_data->data_type = data_type;  
	client_data->file_error_string = strsave(FILE_OPEN_ERROR);
	client_data->file_dialog_title  = strsave(appln_title);
	client_data->mode = R_OK;

	client_data->filename = filename;

	if ( existing_data != NULL ) {  

		if(called_from == FD_FTA) {
			/* routine called from FTA code */
			if(FTAFrameAskQuestion(existing_data, FTA_QUESTION_TITLE)) {
				load_file_cb(client_data, called_from);
			}
		} else if(called_from == FD_PED){
			/* routine called from PED code */
			if(PEDFrameAskQuestion(existing_data, FTA_QUESTION_TITLE)) {
				load_file_cb(client_data, called_from);
			}
		}
	} else {

		load_file_cb(client_data, called_from);
	}

} /* load_from_file */


/*---------------------------------------------------------------
 Routine : overwrite_file_cb
 Purpose : This routine is called if the user presses the OK button
 on the dialog querying the overwriting of an existing file.

 It attempts to use the registered action procedure, and if it
 returns with an error, posts an error dialog.

 Otherwise it calls the end thread procedure to free up any
 allocated memory that is no longer needed.
---------------------------------------------------------------*/
static void
  overwrite_file_cb(fstruct *client_data, int called_from)
{
	char *s;

	if ( (*client_data->action_procedure)( 
		client_data->data_type, 
		client_data->filename ) == FILE_ERROR ) 
	{
		s = filename_from_pathname( client_data->filename );
		client_data->file_error_string =
		(char *)strgrow(
			client_data->file_error_string,
			s );

	
		if(called_from == FD_FTA) {
			/* routine called from FTA code */
			FTAFramePostError(client_data->file_error_string, FTA_SAVE_TITLE);
		} else if(called_from == FD_PED) {
			/* routine called from FTA code */
			PEDFramePostError(client_data->file_error_string, FTA_SAVE_TITLE);
		}

		strfree( s );

	}; 

	end_thread_cb(client_data);

} /* overwrite_file_cb */



/*---------------------------------------------------------------
 Routine : save_to_file
 Purpose : Routine called to save data to a file.
 This routine enforces the functionality required for file
 selection through dialogs.

 Sets up client data to be passed between routines in the
 save file thread.
 if filename given
 then
   if file exists
   then
     query if file is to be overwritten
       (overwrite if so)
   end if
 else
   if given but doesn't exist
   then
     save the data to the file
   end if
 end if
---------------------------------------------------------------*/
void
  save_to_file(
    char     *filename,
    char     *appln_title,
    char     *title,
    char     *filter,
    void     *data_type,  
    int      (*save_data_procedure)(void *, char *),
    BOOL     force_overwrite,
	int     called_from
    )
{

	fstruct *client_data;
	char *s;

	if ( !fNewMemory( (void *)&client_data, sizeof( fstruct ) ) )
	{
		exit( 1 );
	}

	/* Set up the client data for use in callback */
	client_data->action_procedure = save_data_procedure;
	client_data->title = title;
	client_data->filter = filter;
	client_data->data_type = data_type;  
	client_data->file_error_string = strsave(SAVE_ERROR);
	client_data->file_dialog_title  = strsave(appln_title);
	client_data->mode = W_OK;

	if ( !(filename == NULL) ) {

		/* Set up the filename in client data for use in callback */
		client_data->filename = filename; 

		/* if file exists and not forced overwrite then */
		if ( file_exists( filename ) && !( force_overwrite ) ) {

			/* File exists, query overwrite */
			s = filename_from_pathname( filename );
			client_data->file_dialog_title =
			(char *)strgrow(
				client_data->file_dialog_title,
				s );

			if(called_from == FD_FTA) {
				/* routine called from FTA code */
				if(FTAFrameAskQuestion(FILE_EXISTS_QUESTION, client_data->file_dialog_title)) {
					overwrite_file_cb((fstruct *)client_data, FD_FTA);
				}
			} else if(called_from == FD_PED) {
				/* routine called from FTA code */
				if(PEDFrameAskQuestion(FILE_EXISTS_QUESTION, client_data->file_dialog_title)) {
					overwrite_file_cb((fstruct *)client_data, FD_PED);
				}
			}


			/* Don't free client_data as the callback still needs it */
			strfree( s ); 

		} else { /* file given, does not exist, or is forced overwrite */

			if ( (*save_data_procedure)(data_type, filename) == FILE_ERROR ) {

				s = filename_from_pathname( client_data->filename );
				client_data->file_error_string =
				(char *)strgrow(
					client_data->file_error_string,
					s );

 				if(called_from == FD_FTA) {
					/* routine called from FTA code */
					FTAFramePostError(client_data->file_error_string, client_data->file_dialog_title);
				} else if(called_from == FD_PED) {
					/* routine called from FTA code */
					PEDFramePostError(client_data->file_error_string, client_data->file_dialog_title);
				}
				

				strfree( s ); 
 
				end_thread_cb(client_data);
			}

		} /* end if file exists */

	} else { /* NULL filename */

		/* Get selection of filename and save using that name */
		select_filename_and_act_cb(client_data);

	}

} /* save_to_file */

