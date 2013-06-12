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
 Module : analysis_pd
 Author : FSC Limited
 Date   : 13/8/96
 
 SccsId : @(#)analysis_pd.h	1.1 08/22/96
 
 Origin : FTA project,
          FSC Limited Private Venture.
 
 This module implements the pulldown menu for the Analysis option of
 the menu bar.
***************************************************************/

#ifndef ANALYSIS_H
#define ANALYSIS_H

#include "tree_pic_forest.h"

extern void analysis_menu_cb(
    TREE_PIC_FOREST *tree_pic_forest,
    int              item_no
);

/*---------------------------------------------------------------
 Routine : CutSetWidAlgebraicOkCb
 Purpose : The callback routine used for the OK button of the
 Algebraic Cut Set dialog.
---------------------------------------------------------------*/
extern void CutSetWidAlgebraicOkCb(
	TREE_PIC *tree_pic,
    int      order 
);


/*---------------------------------------------------------------
 Routine : ProbsWidOkCb
 Purpose : The callback routine used for the OK button of the
 Numerical Probabilities dialog.
---------------------------------------------------------------*/
extern void
  ProbsWidOkCb(
	TREE_PIC *tree_pic,
    int chosen_order,
    float unit_time,
    int chosen_terms );


/*---------------------------------------------------------------
 Routine : MonteWidOkCb
 Purpose : The callback routine used for the OK button of the
 Monte Carlo simulations dialog.
---------------------------------------------------------------*/
extern void
  MonteWidOkCb(
 	TREE_PIC *tree_pic,
	int      simulations,
    float    unit_time );


#endif
