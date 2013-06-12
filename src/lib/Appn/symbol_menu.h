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
 Module : Symbol Menu
 Author : FSC Limited
 Date   : 29/11/95
 
 SccsId :@(#)symbol_menu.h	1.1 08/22/96

 Origin :
   FTA project,
   FSC Limited Private Venture, Level 2 Prototype.
 
 Purpose :
   This module implements the basic FTA symbols in a symbol menu
   presentation.
 
****************************************************************/

#ifndef symbol_menu_h
#define symbol_menu_h

#include "item.h"
#include "tree_pic_forest.h"

/*--------------------------------------------------------------
 Routine : set_symbol_type
 Purpose : set type of symbol ( from type of button is symbolo menu )
---------------------------------------------------------------*/
extern BOOL set_symbol_type( ITEM *symbol, int i );


/*--------------------------------------------------------------
 Routine : set_symbols_sens
 Purpose : set sensitivity of symbols in the symbols men
---------------------------------------------------------------*/
extern void
  set_symbols_sens(
    TREE_PIC_FOREST *tree_pic_forest,
    ITEM            *item);




#endif
