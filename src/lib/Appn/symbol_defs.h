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
 Module : Symbol Definitions
 Author : FSC Limited
 Date   : 29/11/95
 
 SccsId :@(#)symbol_defs.h	1.3 11/22/96
 
 Purpose: This module defines each of the twelve basic FTA symbols as a
          combination of segments in a SYMBOL structure e.g. an AND gate can
          be defined as a combination of an arc and a polyline segments.
          The initializing coordinates are set in a relative coordinate system. 
****************************************************************/

#ifndef symbol_defs_h
#define symbol_defs_h

#include "symbol.h"
#include "segments.h"


/*--------------------------------------------------------------
 Definition of a Basic event.
---------------------------------------------------------------*/
static ARC bas_arc1 = { {-0.5, 0.0}, 1.0, 1.0, 0, 360 };
static SEGMENT bas_seg1 = { NULL, ARC_SEG, &bas_arc1 };
static SEGMENT bas_seg2 = { &bas_seg1, FILL_ARC_SEG, &bas_arc1 };
static SYMBOL bas_symbol = 
  { SYMB_BASIC, 1.0f, 1.0f, &bas_seg2, 0.7f, 0.7f, 6 };


/*--------------------------------------------------------------
 Definition of an Undeveloped event.
---------------------------------------------------------------*/
static FTA_POINT undev_points[] =
        { {0.0f, 0.0f}, {-0.65f, 0.4f}, {0.0f, 0.8f}, {0.65f, 0.4f}, {0.0f, 0.0f} };
static POLYLINE undev_poly1 =
        { (int)(sizeof(undev_points)/sizeof(FTA_POINT)), undev_points };
static POLYLINE undev_fill_poly1 =
        { (int)(sizeof(undev_points)/sizeof(FTA_POINT)), undev_points };
static SEGMENT undev_seg1 = { NULL, POLYLINE_SEG, &undev_poly1 };
static SEGMENT undev_seg2 = { &undev_seg1, FILL_POLYLINE_SEG, &undev_fill_poly1 };
static SYMBOL undev_symbol = 
  { SYMB_UNDEV, 1.3f, 0.8f, &undev_seg2, 0.75f, 0.75f, 3 };


/*--------------------------------------------------------------
 Definition of an Intermediate event.
---------------------------------------------------------------*/
static RECTANGLE inter_rect1 = { {-0.52f, 0.0f}, 1.04f, 0.70f };
static SEGMENT inter_seg1 = { NULL, RECTANGLE_SEG, &inter_rect1 };
static SEGMENT inter_seg2 = { &inter_seg1, FILL_RECTANGLE_SEG, &inter_rect1 };
static SYMBOL inter_symbol =
 { SYMB_INTERMEDIATE, 1.04f, 0.70f, &inter_seg2, 0.97f, 0.64f, 5 };



/*--------------------------------------------------------------
 Definition of an AND gate.
---------------------------------------------------------------*/
static ARC and_arc1 = { {-0.2f, 0.0f}, 0.4f, 0.56f, 0, 180 };
static FTA_POINT and_points[] =
        { {-0.2f, 0.28f}, {-0.2f, 0.4f}, {0.2f, 0.4f}, {0.2f, 0.28f} };
static POLYLINE and_poly1 =
        { (int)(sizeof(and_points)/sizeof(FTA_POINT)), and_points };
static SEGMENT and_seg1 = { NULL, POLYLINE_SEG, &and_poly1 };
static SEGMENT and_seg2 = { &and_seg1, ARC_SEG, &and_arc1 };
static SEGMENT and_seg3 = { &and_seg2, FILL_POLYLINE_SEG, &and_poly1 };
static SEGMENT and_seg4 = { &and_seg3, FILL_ARC_SEG, &and_arc1 };
static SYMBOL and_symbol = 
  { SYMB_AND, 0.36f, 0.4f, &and_seg4, 0.4f, 0.4f, 0 };

/*--------------------------------------------------------------
 Definition of a Priority AND gate.
---------------------------------------------------------------*/ 
static FTA_POINT priority_and_points[] = { {-0.2f, 0.4f}, {0.0f, 0.0f}, {0.2f, 0.4f} };
static POLYLINE priority_and_poly1 =
        { (int)(sizeof(priority_and_points)/sizeof(FTA_POINT)),
        priority_and_points };
static SEGMENT priority_and_seg1 =
        { &and_seg2, POLYLINE_SEG, &priority_and_poly1 };
static SEGMENT priority_and_seg2 = 
		{ &priority_and_seg1, FILL_POLYLINE_SEG, &and_poly1 };
static SEGMENT priority_and_seg3 = 
		{ &priority_and_seg2, FILL_ARC_SEG, &and_arc1 };
static SYMBOL priority_and_symbol =
  { SYMB_PRIORITY_AND, 0.36f, 0.4f, &priority_and_seg3, 0.4f, 0.4f, 0 };

/*--------------------------------------------------------------
 Definition of a OR gate.
---------------------------------------------------------------*/
static ARC or_arc1 = { {-0.2f, 0.0f}, 0.4f, 0.8f, 0, 180 };
static ARC or_arc2 = { {-0.2f, 0.35f}, 0.4f, 0.1f, 0, 180 };
static SEGMENT or_seg1 = { NULL, ARC_SEG, &or_arc1 };
static SEGMENT or_seg2 = { &or_seg1, ARC_SEG, &or_arc2 };
static SEGMENT or_seg3 = { &or_seg2, CLEAR_ARC_SEG, &or_arc2 };
static SEGMENT or_seg4 = { &or_seg3, FILL_ARC_SEG, &or_arc1 };
static SYMBOL or_symbol = 
  { SYMB_OR, 0.32f, 0.35f, &or_seg4, 0.4f, 0.4f, 0 };

/*--------------------------------------------------------------
 Definition of a Exclusive OR gate.
---------------------------------------------------------------*/ 
static FTA_POINT excl_or_points[] = { {-0.2f, 0.4f}, {0.0f, 0.0f}, {0.2f, 0.4f} };
static POLYLINE excl_or_poly1 =
        { (int)(sizeof(excl_or_points)/sizeof(FTA_POINT)), excl_or_points };
static SEGMENT excl_or_seg1 =
        { &or_seg2, POLYLINE_SEG, &excl_or_poly1 };
static SEGMENT excl_or_seg2 = 
		{ &excl_or_seg1, CLEAR_ARC_SEG, &or_arc2 };
static SEGMENT excl_or_seg3 = 
		{ &excl_or_seg2, FILL_ARC_SEG, &or_arc1 };
static SYMBOL excl_or_symbol =
  { SYMB_EXCL_OR, 0.32f, 0.35f, &excl_or_seg3, 0.4f, 0.4f, 0 };

/*--------------------------------------------------------------
 Definition of a Transfer in symbol.
---------------------------------------------------------------*/
static FTA_POINT trans_in_points[] =
        { {0.0f, 0.18f}, {-0.21f, 0.51f}, {0.21f, 0.51f}, {0.0f, 0.18f}, {0.0f, 0.0f} };
static POLYLINE trans_in_poly1 =
        { (int)(sizeof(trans_in_points)/sizeof(FTA_POINT)), trans_in_points };
static SEGMENT trans_in_seg1 = { NULL, POLYLINE_SEG, &trans_in_poly1 };
static SEGMENT trans_in_seg2 = { &trans_in_seg1, FILL_POLYLINE_SEG, &trans_in_poly1 };
static SYMBOL trans_in_symbol =
  { SYMB_TRANS_IN, 0.42f, 0.51f, &trans_in_seg2, 0.42f, 0.33f, 0 };


/*--------------------------------------------------------------
 Definition of a Condition event.
---------------------------------------------------------------*/
static ARC condition_arc1 = { {-0.52f, 0.0f}, 1.04f, 0.64f, 0, 360 };
static SEGMENT condition_seg1 = { NULL, ARC_SEG, &condition_arc1 };
static SEGMENT condition_seg2 = {&condition_seg1 , FILL_ARC_SEG, &condition_arc1 };
static SYMBOL condition_symbol =
  { SYMB_CONDITION, 1.04f, 0.64f, &condition_seg2, 1.04f, 0.64f, 2 };

/*--------------------------------------------------------------
 Definition of an External event.
---------------------------------------------------------------*/

static FTA_POINT extern_points[] =
        { {0.0f, 0.0f}, {-0.42f, 0.12f}, {-0.42f, 1.0f}, {0.42f, 1.0f}, {0.42f, 0.12f},
        {0.0f, 0.0f} };
static POLYLINE extern_poly1 =
        { (int)(sizeof(extern_points)/sizeof(FTA_POINT)), extern_points };
static SEGMENT extern_seg1 = { NULL, POLYLINE_SEG, &extern_poly1 };
static SEGMENT extern_seg2 = { &extern_seg1, FILL_POLYLINE_SEG, &extern_poly1 };
static SYMBOL extern_symbol =
  { SYMB_EXTERN, 0.84f, 1.0f, &extern_seg2, 0.80f, 0.97f, 10 };

/*--------------------------------------------------------------
 Definition of an Inhibit gate.
---------------------------------------------------------------*/
static FTA_POINT inhibit_points[] =
        { {0.0f, 0.0f}, {-0.2f, 0.12f}, {-0.2f, 0.4f}, {0.0f, 0.52f}, {0.2f, 0.4f},
        {0.2f, 0.12f}, {0.0f, 0.0f} };
static POLYLINE inhibit_poly1 =
        { (int)(sizeof(inhibit_points)/sizeof(FTA_POINT)), inhibit_points };
static SEGMENT inhibit_seg1 = { NULL, POLYLINE_SEG, &inhibit_poly1 };
static SEGMENT inhibit_seg2 = { &inhibit_seg1, FILL_POLYLINE_SEG, &inhibit_poly1 };
static SYMBOL inhibit_symbol =
  { SYMB_INHIBIT, 0.4f, 0.52f, &inhibit_seg2, 0.4f, 0.26f, 0 };


/*--------------------------------------------------------------
 Definition of a Transfer out event.
---------------------------------------------------------------*/ 
static FTA_POINT trans_out_points[] = 
  { {-0.06f, 0.09f}, {0.0f, 0.0f}, {0.21f, 0.33f}, {-0.21f, 0.33f}, {-0.06f, 0.09f},  
    {-0.21f, 0.09f} };  
static POLYLINE trans_out_poly1 = 
        { (int)(sizeof(trans_out_points)/sizeof(FTA_POINT)), trans_out_points }; 
static SEGMENT trans_out_seg1 = { NULL, POLYLINE_SEG, &trans_out_poly1 }; 
static SEGMENT trans_out_seg2 = { &trans_out_seg1, FILL_POLYLINE_SEG, &trans_out_poly1 }; 
static SYMBOL trans_out_symbol = 
  { SYMB_TRANS_OUT, 0.42f, 0.33f, &trans_out_seg2, 0.42f, 0.33f, 0 }; 
/* static SYMBOL trans_out_symbol;  */


static SYMBOL *symbol_defs[SYMB_INVALID] =
  { &bas_symbol, 
    &undev_symbol, 
    &inter_symbol, 
    &and_symbol, 
    &or_symbol,
    &priority_and_symbol, 
    &trans_in_symbol, 
    &condition_symbol,
    &extern_symbol, 
    &excl_or_symbol, 
    &inhibit_symbol, 
    &trans_out_symbol};

/* extern void */
/*   create_symbol_definitions( void ); */
/*  */
/* extern SYMBOL * */
/*   get_symbol_definition( */
/*     int symbol_type ); */
/*  */
/* extern void */
/*   delete_symbol_definitions( void ); */

#endif 
