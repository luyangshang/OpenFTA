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
 Module : basic
 Author : FSC Limited
 Date   : 13/8/96
 
 SccsId : @(#)basic.h	1.1 08/22/96
 
 Origin : FTA project,
          FSC Limited Private Venture.
 
 This module defines the EVENT structure for the Basic events
 used in the Monte Carlo simulation.
***************************************************************/

#ifndef Basic_h
#define Basic_h

#include "boolean.h"
#include "NormalisedBooleanExpressions.h"

#define MAX_SYMBOL_ID_LENGTH  13

typedef struct bas {
  int         type;
  char        id[ MAX_SYMBOL_ID_LENGTH ];
  float       prob;
  BOOL        val;
  Expr        expr;
  char        *database;
  struct bas  *next;
  struct bas  *prev;
} EVENT;

#endif





