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

/****************************************************************
 Module : Print_report_pd
 Author : FSC Limited
 Date   : 29/11/95
 
 SccsId : @(#)print_report_pd.h	1.1 22 Aug 1996

 Origin : FTA project, FSC Limited Private Venture, Level 2 Prototype.
 
 Purpose: This module implements the pulldown menus for the Print option of
          the Report pulldown on the menu bar.
 
****************************************************************/

#ifndef print_report_pd_h
#define print_report_pd_h

#include "tree_pic_forest.h"

#define PrintQualitative  0 
#define PrintProbability  1 
#define PrintMonteCarlo   2 


/*--------------------------------------------------------------
 Routine : print_menu_cb
 Purpose : Invoked when an item is selected in the print menu.
---------------------------------------------------------------*/
extern void print_menu_cb(TREE_PIC_FOREST *tree_pic_forest, int item_no);

#endif
