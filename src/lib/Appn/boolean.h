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
 Module : boolean
 Author : FSC Limited
 Date   : 13/8/96
 
 SccsId : @(#)boolean.h	1.1 08/22/96
 
 Origin : FTA project,
          FSC Limited Private Venture.
 
 This module defines the BOOL type that needs to be defined for
 modules that do not pick up the Boolean definition exported
 by X.
***************************************************************/

#ifndef boolean_h
#define boolean_h

/*
  typedef enum boolean { FALSE, TRUE } BOOL;

  work after the #defines of FALSE, TRUE in <X11/Intrinsic.h>
  but the following is acceptable.
*/
/* #ifndef FALSE */
/* typedef enum boolean { FALSE, TRUE } BOOL; */
/*  */
/* #else */
typedef int BOOL;
#define FALSE 0
#define TRUE  1
 
/* #endif *//* ndef FALSE */

#endif /* ndef boolean_h */
