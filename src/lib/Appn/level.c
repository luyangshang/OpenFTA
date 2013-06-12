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
 Module : level
 Author : FSC Limited
 Date   : 24/7/96
 
 SccsId : @(#)level.c	1.3 11/08/96
 
 Origin : FTA project,
          FSC Limited Private Venture, Level 2 Prototype.
 
 This module creates and manipulates the LEVEL structure for the
 given fault tree.
***************************************************************/

#include <stdlib.h>

#include "util.h"
#include "level.h"
#include "treeUtil.h"

#include "MemoryUtil.h"

/*---------------------------------------------------------------
 Routine : set_tree_level
 Purpose : Recursively sets the level of the item and all items 
 in the tree below it.
 Returns the maximum level number.
---------------------------------------------------------------*/
int
  set_tree_level(
    ITEM *ip, 
    int level)
{
        ITEM *child;
    int max, tmp;

    ip->level = level;
    max = level;
    child = ip->child;
    if (child != NULL && ( child->type == COND_NOT_ANAL ||
                           child->type == COND_ANAL ||
                           child->type == TRANSOUT ) ) {
        child->level = level;
        child = child->right_sib;
    }
    while (child != NULL) {
        tmp = set_tree_level(child, level+1);
        max = MAX(max, tmp);
        child = child->right_sib;
    }
    return max;

} /* set_tree_level */

/*---------------------------------------------------------------
 Routine : make_level
 Purpose : Create and initialise a LEVEL structure
---------------------------------------------------------------*/
LEVEL *
  make_level(
    ITEM *ip, 
    int lvl)
{
  LEVEL *lp;
/*  LEVEL *tlp; */

  if (ip == NULL) return NULL;
/*   if ((lp = (LEVEL *)malloc(sizeof(LEVEL))) == NULL) */
  if ( !fNewMemory( (void *)&lp, sizeof(LEVEL) ) )
  {
    printf("\n*** make_level : malloc failed ***\n");
    exit(1);
  }
  lp->level = lvl;
  lp->size = 0;
  lp->iptr = NULL;
  lp->next = NULL;
  return lp;

} /* make_level */

/*---------------------------------------------------------------
 Routine : join_level
 Purpose : Join items of a given level in list
---------------------------------------------------------------*/
void
  join_level(
    ITEM *ip, 
    LEVEL *lp, 
    int lvl, 
    ITEM *tll)
{
        ITEM *tip, *child;
        static ITEM *top_level_list;

    top_level_list = tll;
        if (ip == NULL) return;
        if (ip->level == lvl) {
                lp->size++;
                if (lp->iptr == NULL) {
                        lp->iptr = ip;
        }
                ip->lptr = NULL;
    /** ip->prev = NULL; **/
                if (top_level_list == NULL) {
                        top_level_list = ip;
                } else {
                        tip = top_level_list;
                        while (tip->lptr != NULL) tip = tip->lptr;
                        tip->lptr = ip;
        /** ip->prev = tip; **/
                }
        }
        child = ip->child;
        while (child != NULL) {
                join_level(child, lp, lvl, top_level_list);
                child = child->right_sib;
        }

} /* join_level */


/*---------------------------------------------------------------
 Routine : delete_level
 Purpose : Delete a level (NOT the items in it!)
---------------------------------------------------------------*/
void
  delete_level(
    LEVEL *lp)
{
        LEVEL *next;

        if (lp == NULL) return;
        next = lp->next;
        if ( lp != NULL ) FreeMemory(lp);
        delete_level(next);

} /* delete_level */



/*---------------------------------------------------------------
 Routine : find_level
 Purpose : Find which level a y-coordinate is in.
 Use level structure and drawing-area coords of the items
---------------------------------------------------------------*/
LEVEL *
  find_level(
    TREE_PIC *tree_pic, 
    LEVEL *lp, 
    int selected_y)
{
    float        level_max_y;

    while (lp != NULL) {

     /* need to cope with variable vertical spacing */
        if (lp->next == NULL) {
            level_max_y = (float)lp->iptr->da_y[tree_pic->set_no] +
                          VERT_SPACING * TreePicGetScale(tree_pic);
        } else {
            level_max_y = (float)lp->next->iptr->da_y[tree_pic->set_no];
        }

                if (selected_y < level_max_y) {
                    return lp;
                }

        lp = lp->next;
    }
    return NULL;

} /* find_level */


