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

 SccsId :@(#)symbol_menu.c	1.7 01/28/97

 Origin :
   FTA project,
   FSC Limited Private Venture, Level 2 Prototype.

 Purpose :
   This module implements the basic FTA symbols in a symbol menu
   presentation.

****************************************************************/

/* #include <malloc.h> */
#include <stdlib.h>

#include "symbol_menu.h"
#include "symbol.h"
#include "item.h"

#include "NativeFTAFrame.h"


#include "MemoryUtil.h"

#define GATE_NOT_SUPPORTED_MSG \
"This symbol type is not supported in this version of FTA\n\
but will be available in a future release."




/*--------------------------------------------------------------
 Routine : set_symbol_type
 Purpose : set type of symbol ( from type of button is symbolo menu )
---------------------------------------------------------------*/
BOOL set_symbol_type( ITEM *symbol, int i )
{
  switch ( i )
  {
      case BASIC_POS:
      symbol->type = BASIC;
      break;
    case UNDEVELOP_POS:
      symbol->type = UNDEVELOP;
      break;
    case AND_POS:
      symbol->type = AND;
      break;
    case PRIORITY_AND_POS:
      symbol->type = PRIORITY_AND;
      break;
    case TRANSIN_POS:
      symbol->type = TRANSIN;
      break;
    case COND_POS:
      symbol->type = COND_NOT_ANAL;
      break;
    case EXTERNAL_POS:
      symbol->type = EXTERNAL;
      break;
    case INTERMEDIATE_POS:
      symbol->type = INTERMEDIATE;
      break;
    case OR_POS:
      symbol->type = OR;
      break;
    case XOR_POS:
      symbol->type = XOR;
      break;
    case INHIBIT_POS:
      symbol->type = INHIBIT;
      break;
    case TRANSOUT_POS:
      symbol->type = TRANSOUT;
      break;
    default:
      return FALSE;
  }
  return TRUE;
}



/*--------------------------------------------------------------
 Routine : set_symbols_sens
 Purpose : set sensitivity of symbols in the symbols men
---------------------------------------------------------------*/
void
set_symbols_sens( 
		 TREE_PIC_FOREST *tree_pic_forest,
		 ITEM *item )	/* selected item */
{
	if((item != NULL) &&
       (item == tree_pic_forest->tree_pic->tree->top_item) &&
       ((item->child == NULL) || (item->child->type != TRANSOUT)))
	{
		FTAFrameSetSensitive(TRANSOUT_POS, TRUE );
	} else {
		FTAFrameSetSensitive(TRANSOUT_POS, FALSE );
	}

	if ( item != NULL ) {
	    if ( is_gate( item ) ) {

			/* any gate */
			FTAFrameSetSensitive(INTERMEDIATE_POS, TRUE );
			FTAFrameSetSensitive(BASIC_POS, TRUE );
			FTAFrameSetSensitive(UNDEVELOP_POS, TRUE );
			FTAFrameSetSensitive(EXTERNAL_POS, TRUE );
			FTAFrameSetSensitive(AND_POS, FALSE );
			FTAFrameSetSensitive(OR_POS, FALSE );
			FTAFrameSetSensitive(PRIORITY_AND_POS, FALSE );
			FTAFrameSetSensitive(XOR_POS, FALSE );
			FTAFrameSetSensitive(INHIBIT_POS, FALSE );

			if ( item->child != NULL &&
				( item->child->type == COND_NOT_ANAL ||
				  item->child->type == COND_ANAL ) )
			{

				/* already has a condition */
				FTAFrameSetSensitive(COND_POS, FALSE );
			} else {
				FTAFrameSetSensitive(COND_POS, TRUE );
			}
				
			FTAFrameSetSensitive(TRANSIN_POS, TRUE );

			if ( item->type == INHIBIT )
			{
				/* Inhibit gate, only one condition, one event */
				if ( ( item->num_children >= 2 ) ) 
				{
					/* No more symbols allowed */
					FTAFrameSetSensitive(INTERMEDIATE_POS, FALSE );
					FTAFrameSetSensitive(BASIC_POS, FALSE );
					FTAFrameSetSensitive(UNDEVELOP_POS, FALSE );
					FTAFrameSetSensitive(EXTERNAL_POS, FALSE );
					FTAFrameSetSensitive(TRANSIN_POS, FALSE );
					FTAFrameSetSensitive(COND_POS, FALSE );
				}
				else
				{
					if ( ( item->num_children == 1 ) )
					{
						if ( is_condition( item->child ) )
						{
							FTAFrameSetSensitive(COND_POS, FALSE );
						}
						else
						{
							FTAFrameSetSensitive(INTERMEDIATE_POS, FALSE );
							FTAFrameSetSensitive(BASIC_POS, FALSE );
							FTAFrameSetSensitive(UNDEVELOP_POS, FALSE );
							FTAFrameSetSensitive(EXTERNAL_POS, FALSE );
							FTAFrameSetSensitive(COND_POS, TRUE );
							FTAFrameSetSensitive(TRANSIN_POS, FALSE );
						}
					}
				}
			}
			return;

		}
		else if ( item->type == INTERMEDIATE &&
				( item->num_children == 0 ||
				( item->num_children == 1 &&
				item->child->type == TRANSOUT ) ) )
		{

			/* intermediate event with no child */
			FTAFrameSetSensitive(INTERMEDIATE_POS, TRUE );
			FTAFrameSetSensitive(BASIC_POS, TRUE );
			FTAFrameSetSensitive(UNDEVELOP_POS, TRUE );
			FTAFrameSetSensitive(EXTERNAL_POS, TRUE );
			FTAFrameSetSensitive(AND_POS, TRUE );
			FTAFrameSetSensitive(OR_POS, TRUE );
			FTAFrameSetSensitive(PRIORITY_AND_POS, TRUE );
			FTAFrameSetSensitive(XOR_POS, TRUE );
			FTAFrameSetSensitive(INHIBIT_POS, TRUE );
			FTAFrameSetSensitive(COND_POS, FALSE );
			FTAFrameSetSensitive(TRANSIN_POS, TRUE );
			return;
		}
	}

	/* NULL, primary, condition, transfer-in, intermediate-with-child */
	FTAFrameSetSensitive(INTERMEDIATE_POS, FALSE );
	FTAFrameSetSensitive(BASIC_POS, FALSE );
	FTAFrameSetSensitive(UNDEVELOP_POS, FALSE );
	FTAFrameSetSensitive(EXTERNAL_POS, FALSE );
	FTAFrameSetSensitive(AND_POS, FALSE );
	FTAFrameSetSensitive(OR_POS, FALSE );
	FTAFrameSetSensitive(PRIORITY_AND_POS, FALSE );
	FTAFrameSetSensitive(XOR_POS, FALSE );
	FTAFrameSetSensitive(INHIBIT_POS, FALSE );
	FTAFrameSetSensitive(COND_POS, FALSE );
	FTAFrameSetSensitive(TRANSIN_POS, FALSE );
}
