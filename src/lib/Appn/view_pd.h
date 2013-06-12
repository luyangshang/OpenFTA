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
 Module : View
 Author : FSC Limited
 Date   : 29/11/95
 
 SccsId :@(#)view_pd.h	1.1 08/22/96
 
 This module implements the pulldown menus for the View option of
 the menu bar. 

****************************************************************/

#ifndef view_pd_h
#define view_pd_h

#include "tree_pic_forest.h"

/*********************************************************************
 Define View Pulldown Numbers
 ********************************************************************/
#define ViewFirst        0 
#define ViewLast         2 
#define NumViewItems ViewLast + 1 
#define ViewQualitative  0 
#define ViewProbability  1 
#define ViewMonteCarlo   2 

/*--------------------------------------------------------------
 Routine :view_menu_cb
 Purpose : Callback invoked when an item is selected in the view menu.
---------------------------------------------------------------*/
extern void view_menu_cb(TREE_PIC_FOREST *tree_pic_forest, int item_no);


#endif
