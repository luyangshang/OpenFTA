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
 Module : event_list
 Author : FSC Limited
 Date   : 13/8/96
 
 SccsId : @(#)event_list.h	1.1 08/22/96
 
 Origin : FTA project,
          FSC Limited Private Venture.
 
 This module maintains a single-instance primary event list. The
 list is used in the Monte Carlo simulation to record the events
 that are simulated to have failed. The list is used for Algebraic
 Cut Sets to build the expressions for each event which are then
 logically reduced.

 This module is a mish-mash of ideas and implementations. It may
 assist in the determination of minimum cut sets, but cut_sets.[ch]
 exists to perform the real cut set related routines. This module
 is in need of thorough review.
 MPA 23/7/96

***************************************************************/

#ifndef mcs_h
#define mcs_h

#include "bits.h"
#include "basic.h"
#include "NormalisedBooleanExpressions.h"
#include "treeUtil.h"

/*---------------------------------------------------------------
 Routine : unexpand_tree
 Purpose : Resets tree to state before cut sets were generated
 All event expressions are set to NULL and any transfered in trees
 are discarded.
---------------------------------------------------------------*/
extern void
  unexpand_tree(
    TREE *tree );

/*---------------------------------------------------------------
 Routine : del_bas
 Purpose : This routine deletes the basic event list.
---------------------------------------------------------------*/
extern void
  del_bas();


/*---------------------------------------------------------------
 Routine : expand_tree
 Purpose : Include transfered-in trees
 Build the BASLIST (many ITEMs may point to the same EVENT structure)
---------------------------------------------------------------*/
extern void
  expand_tree(
    TREE *tree );


/*---------------------------------------------------------------
 Routine : reset_basic_vals
 Purpose : zero (and count) list of basic vals
---------------------------------------------------------------*/
extern int
  reset_basic_vals();


/*---------------------------------------------------------------
 Routine : initialise_exprs
 Purpose : Initialise the basic event list for algebraic cut sets
---------------------------------------------------------------*/
extern void
  initialise_exprs();


/*---------------------------------------------------------------
 Routine : free_exprs
 Purpose : Frees the memory taken up by the basic event list 
           expressions for algebraic cut sets
---------------------------------------------------------------*/
extern void
  free_exprs();


/*---------------------------------------------------------------
 Routine : BitPara
 Purpose : Create paragraph containing list of id's of basic events
 given in BitArray
---------------------------------------------------------------*/
extern char **
  BitPara(
    BitArray *b, 
    int n );


/*---------------------------------------------------------------
 Routine : set_bas_prob
 Purpose : Search events database for event probabilities,
 and enter them in the EVENT list
---------------------------------------------------------------*/
extern void
  set_bas_prob(
    float unit_time );


/*---------------------------------------------------------------
 Routine : get_probs
 Purpose : Return array of probabilities of basic events
---------------------------------------------------------------*/
extern void
  get_probs( 
    float *probs ); /* OUT - array of probabilities */



/*---------------------------------------------------------------
 Routine : BasicString
 Purpose : Create string containing id of a basic event
---------------------------------------------------------------*/
extern char *
  BasicString( 
    int n,    /* IN  - number of basic events */
    int i );  /* IN  - event index            */


/*---------------------------------------------------------------
 Routine : set_basic_vals
 Purpose : To set the values of the identified events in the list.
---------------------------------------------------------------*/
extern int
  set_basic_vals( int *z );


/*---------------------------------------------------------------
 Routine : calc_bas_parms
 Purpose : Calculate : prob of at least one basic event failing
                       array of relative probabilities
                       index into the basic events
---------------------------------------------------------------*/
extern void
  calc_bas_parms(
    double *bas_prob, /* OUT - p. at least one basic event failure */
    double *rel,      /* OUT - array of relative probabilities
                      *       of being the first in the list
                      */
    EVENT  **index); /* OUT - index to basic events               */


#endif
