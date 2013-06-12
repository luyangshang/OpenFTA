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
 Module : levelP
 Author : FSC Limited
 Date   : 24/7/96
 
 SccsId : @(#)levelP.h	1.1 08/22/96
 
 Origin : FTA project,
          FSC Limited Private Venture, Level 2 Prototype.
 
 This module provides the private items available for the LEVEL 
 structure for the given fault tree.
***************************************************************/

#ifndef _LEVEL_P_H
#define _LEVEL_P_H

#include "item.h"

/*
 * This type provides access to the individual components of a level in the
 * tree.
 */
typedef struct _level{
	int           level;
	int           size;
	ITEM         *iptr;
	struct _level *next;
} LEVEL;

#endif
