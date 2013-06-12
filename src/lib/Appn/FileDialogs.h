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
 
 SccsId : @(#)FileDialogs.h	1.1 08/22/96
 
 Origin : FTA project,
          FSC Limited Private Venture, Level 2 Prototype.
 
 This module provides the functionality for the standard file
 open/saving.
***************************************************************/

#ifndef FILE_SEL_H
#define FILE_SEL_H

#include "boolean.h"

#define FILE_OK 0
#define FILE_ERROR 1

#define FORCE_OVERWRITE TRUE

#define FD_FTA 1
#define FD_PED 2


/*---------------------------------------------------------------
 Routine : save_to_file
 Purpose : Routine called to save data to a file.
 This routine enforces the functionality required for file
 selection through dialogs.
---------------------------------------------------------------*/
extern void
  save_to_file(
    char     *filename,     /* in: Filename string. May be NULL */
    char     *appln_title,  /* in: APPN_TITLE */
    char     *title,        /* in: String to be displayed in dialog boxes */
    char     *filter,       
      /* in: Filter to be applied in file selection box.
             This filter is also used to force the suffix of the file
             that is to be saved. i.e., if filter is "*.b", only
             "*.b" files can be saved. */
    void     *data_type,    
      /* in: Pointer to type of data to be saved to file. */
    int      (*save_data_procedure)(void *, char *), 
      /* in: Procedure used to save data of type data_type to file.
             Filename may be that supplied or selected within this thread.
             Takes pointer to data_type and filename string */ 
    BOOL     force_overwrite,
      /* in: Force the given filename to be overwritten without
             user interaction */
	int      called_from_fta
	  /* in: Messy. Need to know whether routine called from FTA or PED.
			 Need to send the question dialog request to correct java objeect */
	);


/*---------------------------------------------------------------
 Routine : load_from_file
 Purpose : Routine called to load data from a file.
 This routine enforces the functionality required for file
 selection through dialogs.
---------------------------------------------------------------*/
extern void
  load_from_file(
    char     *filename,     /* in: Filename string. May be NULL */ 
    char     *appln_title,  /* in: APPN_TITLE */
    char     *title,        /* in: String to be displayed in dialog boxes */
    char     *filter,
      /* in: Filter to be applied in file selection box.
             This filter is also used to force the suffix of the file
             that can be loaded. i.e., if filter is "*.b", only
             "*.b" files can be loaded. */
    char     *existing_data,
      /* in: String containing a warning to be displayed if existing data
             will be overwriten by loading from a file.
             A non-NULL value causes a query dialog to be displayed before
             the data can be loaded.
             May be NULL. */
    void     *data_type,  
      /* in: Pointer to type of data to be loaded from file. */
    int      (*read_data_procedure)(void *, char *),
      /* in: Procedure used to load data of type data_type from file.
             Filename may be that supplied or selected within this thread.
             Takes pointer to data_type and filename string */
	int      called_from_fta
	  /* in: Messy. Need to know whether routine called from FTA or PED.
			 Need to send the question dialog request to correct java objeect */
	);
#endif		 
