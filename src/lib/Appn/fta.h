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
 Module : fta
 Author : FSC Limited
 Date   : 13/8/96
 
 SccsId : @(#)fta.h	1.2 09/30/96
 
 Origin : FTA project,
          FSC Limited Private Venture.
 
 This module defines the constant terms used in the FTA.
***************************************************************/

#ifndef Fta_h
#define Fta_h

#ifndef NULL
#define NULL 0
#endif

#define BASE_10 10

#define FTA_STRING_BUF_LENGTH		256

#define FTA_APPN_TITLE "Formal-FTA"

#define UNAVAILABLE_WARNING \
"This menu option is not currently supported\n\
but will be available in a future version of FTA"

#define NULL_TREE_NAME               "Untitled" 
#define NULL_DB_NAME                 "Untitled" 

#define NO_ITEM_SELECTED_WARNING	"No Tree item currently selected"
#define NO_DB_ITEM_SELECTED_WARNING	"No Database item currently selected"
#define SCALE 			        100

#define MCS_SUFFIX                      "mcs"
/* The Report files are not temporary. They should persist and be
   associated with a tree, but shall only be applicable to a tree
   until that tree is edited. It shall be possible to view/print
   a report from within FTA for as long as the report is valid.
*/
#define VALIDITY_REPORT_TEMPFILE        "vrptmp"
#define VALIDITY_REPORT_SUFFIX          "vrp"
#define MCS_REPORT_TEMPFILE             "mcstmp"
#define MCS_REPORT_SUFFIX               "crp"
#define MONTE_CARLO_REPORT_TEMPFILE     "monte_carlo_tmp"
#define MONTE_CARLO_REPORT_SUFFIX       "mrp"
#define PROBS_REPORT_TEMPFILE           "probs_tmp"
#define PROBS_REPORT_SUFFIX             "prp"

#endif

