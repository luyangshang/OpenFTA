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
 Module : Tree
 Author : FSC Limited
 Date   : 29/11/95
 
 SccsId :@(#)treeP.h	1.1 08/22/96
 
****************************************************************/
 
#ifndef treeP_h
#define treeP_h

#include "item.h"
#include "NormalisedBooleanExpressions.h"
#include "boolean.h"
#include "levelP.h"

typedef struct tree {
  char   *name;                /* name of tree file     */
  char   *path;                /* path to the tree file     */
  char   *database;            /* name of database file */
  ITEM   *top_item;
  ITEM   *process_top_item;
  LEVEL  *top_level;
  float   min_x;
  float   max_x, max_y;
  int     last_item;
  int     max_level;
  Expr    mcs_expr;             /* cut sets                 */
  Group  *mcs_end;              /* last cut set in list     */
  int     max_order;            /* max order of cut sets    */
  int     num_mcs;              /* number of cut sets       */
  int     num_bas;              /* number of basic events   */

  BOOL    edited;               /* flag - tree has been edited      */
  BOOL    expanded;             /* flag - tree is expanded          */
  BOOL    mcs_exists;           /* flag - mcs have been generated   */
  BOOL    mcs_tree_exists;      /* flag - mcs tree has been created */
  BOOL    probs_exists;         /* flag - probs analysis done       */
  BOOL    monte_exists;         /* flag - M.C. analysis done        */
  BOOL    timed;                /* flag - mcs timing has been done  */

  unsigned int num_lambda_events; /* count of the number of lambda based events in the tree */

  char *post_it_note;
} TREE;

#endif
