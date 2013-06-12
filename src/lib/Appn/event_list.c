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
 Module : Event list
 Author : FSC Limited
 Date   : April 1992
 
 SccsId : @(#)event_list.c	1.4 11/08/96
 
 This module maintains a single-instance primary event list. The
 list is used in the Monte Carlo simulation to record the events
 that are simulated to have failed. The list is used for Algebraic
 Cut Sets to build the expressions for each event which are then
 logically reduced.

****************************************************************/

/* #include <malloc.h> */
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "event_list.h"
#include "treeUtil.h"
#include "util.h"
#include "bits.h"
#include "primary_event.h"
#include "PED_shell.h"

#include "MemoryUtil.h"
#include "StringUtilities.h"

#include "NativeFTAFrame.h"

/* #include "Assert.h" */
/*  */
/* ASSERTFILE( __FILE__ ) */

/*--------------------------------------------------------------- 
 #define this to use set_basic_vals2 - the 'fast' version
 otherwise use set_basic_vals
 The current tested implementation presumes that this macro is
 undefined. MPA 24/7/96
---------------------------------------------------------------*/
#undef SET_BASIC_VALS2

/*---------------------------------------------------------------
 Static variables global to event_list.c
---------------------------------------------------------------*/

static EVENT *first_bas = NULL;  /* first event in basic event list */
static EVENT *last_bas  = NULL;  /* last  event in basic event list */
static int num_bas      = -1;    /* number of basic events in list  */


/*---------------------------------------------------------------
 Routine : make_event
 Purpose : Make a basic event structure
---------------------------------------------------------------*/
static EVENT *
  make_event()
{
  EVENT *bp;

/*   if ((bp = (EVENT *)malloc(sizeof(EVENT))) == NULL)  */
  if ( !fNewMemory( (void *)&bp, sizeof( EVENT ) ) )
  {
    printf("\n*** make_event : malloc failed ***\n");
    exit(1);
  }
  bp->val = FALSE;
  bp->next = NULL;
  return(bp);

} /* make_event */



/*---------------------------------------------------------------
 Routine : reset_basic_vals
 Purpose : To reset the basic event list values to FALSE, and 
 return a count of the number of basic events.

---------------------------------------------------------------*/
int
  reset_basic_vals()
{
    EVENT *bp;
    int n=0;

    bp = first_bas;
    while (bp != NULL){
        n++;
        bp->val = FALSE;
        bp = bp->next;
    }
    return n;
}


/*---------------------------------------------------------------
 Routine : rec_expand_tree
 Purpose : To expand the supplied sub-tree, including any 
 transferred-in trees.
 
 This routine works recursively on a tree of items.

 The tree parameter is redundant.
---------------------------------------------------------------*/
static void
  rec_expand_tree( TREE *tree, ITEM *ip, char* current_ped_name )
{
    TREE expnd_tree;
    FILE *fp;
    ITEM *process_child;
    EVENT *tbp;
    char *dbname;
    char *transin_file;
    char *transin_ped;

    char *ignored;

    if (ip == NULL) {
        printf("\n*** rec_expand_tree : NULL item pointer ***\n");
    }
    switch(ip->type) {
    case BASIC:
    case UNDEVELOP:
    case EXTERNAL:
    case COND_ANAL:

     /* if event is already in list, point to it and return
      * else find its place in the list
      */
        tbp = first_bas;
        while (tbp != NULL) {
            int cmp = strcmp(tbp->id, ip->id); 
            if (cmp == 0){ 
                ip->event = tbp;
                return;
            } else if (cmp > 0) { 
                break;             /* tbp is the first one greater than ip */
            }
            tbp = tbp->next;
        }

     /* event not in list - create new EVENT */
        ip->event = make_event();
        strcpy(ip->event->id, ip->id);
        ip->event->type = ip->type;
        ip->event->database = SU_Copy(current_ped_name);
        ip->event->next = NULL;
        ip->event->prev = NULL;

     /* add in alphabetic position (immediately before tbp) */
        if (first_bas == NULL) {           /* empty list */
            first_bas = ip->event;
            last_bas  = ip->event;
        } else if (tbp == NULL) {          /* add at end        */
            last_bas->next = ip->event;
            ip->event->prev = last_bas;
            last_bas  = ip->event;
        } else if (tbp->prev == NULL) {    /* add at start      */
            ip->event->next = first_bas;
            first_bas->prev = ip->event;
            first_bas = ip->event;
        } else {                           /* add in the middle */
            tbp->prev->next = ip->event;
            ip->event->prev = tbp->prev;
            tbp->prev = ip->event;
            ip->event->next = tbp;
        }
        return;

    case COND_NOT_ANAL:
        return;

    case OR:
    case XOR:
        process_child = ip->process_child;
        while (process_child != NULL){
            rec_expand_tree(tree, process_child, current_ped_name);
            process_child = process_child->process_sibling; 
        }
        return;

    case AND:
    case INHIBIT:
    case PRIORITY_AND:
        process_child = ip->process_child;
        while (process_child != NULL){
            rec_expand_tree(tree, process_child, current_ped_name);
            process_child = process_child->process_sibling; 
        }
        return;

    case TRANSIN:
        expnd_tree.top_item = NULL;
        expnd_tree.process_top_item = NULL;

        transin_file = SU_Join(tree->path, ip->id);

        /* open file */
        if ((fp = fopen(transin_file, "r")) == NULL){
            printf("\n*** Error loading tree %s ***\n\n", ip->id);
            exit(1);
        }

	    free(transin_file);

     /* read database name */
        dbname = load_dbname(fp);

        ignored = load_post_it_note( fp );
        FreeMemory( ignored );

     /* recursively load the tree */
        expnd_tree.top_item = load_item(fp, NULL, dbname);

     /* close file */
        fclose(fp);
            
     /* generate processing pointers */
        expnd_tree.process_top_item =
            generate_processing_pointers(expnd_tree.top_item);

     /* add tree as child of the TRANSIN item */
        ip->process_child = expnd_tree.process_top_item;
/*         ip->num_children = 1; */
        expnd_tree.top_item->parent = ip;

        if(dbname) {
          transin_ped = SU_Join(tree->path, dbname);
        } else {
          transin_ped = NULL;
        }

        if(tree->path) {
	         expnd_tree.path = SU_Copy(tree->path);
        } else {
           expnd_tree.path = NULL;
        }

	      /* create space for and load ped that goes with Transfer-In tree */
        if(transin_ped) {
	         ped_shell_create();
	         FTAFrameLoadPEDFile(transin_ped);
        }

     /* recursively expand the transfered-in tree */
        rec_expand_tree(&expnd_tree, expnd_tree.process_top_item, transin_ped);
     /* ip->ep = expnd_tree.process_top_item->ep; */
        return;
		free(transin_ped);
		free(dbname);

    case TRANSOUT:
      printf("event_list: Transfer out found\n");
      return;
      break;

    default:
        printf("\n*** rec_expand_tree : Invalid item type %d ***\n\n",
          ip->type );
        exit(1);
    }

} /* rec_expand_tree */


/*---------------------------------------------------------------
 Routine : rec_del_bas
 Purpose : Free an item in the basic event list
 
 This routine works recursively on a linked list of events.
---------------------------------------------------------------*/
static void
  rec_del_bas( 
    EVENT *bp )
{
    if (bp == NULL) return;
    if (bp->next != NULL) rec_del_bas(bp->next);
	FreeMemory(bp->database);
    FreeMemory(bp);
} /* rec_del_bas */




/*---------------------------------------------------------------
 Routine : del_bas
 Purpose : Delete the basic event list
---------------------------------------------------------------*/
void
  del_bas()
{
    if (first_bas != NULL) rec_del_bas(first_bas);
    first_bas = NULL;
    last_bas = NULL;

/*     printf("del_bas() : basic event list deleted\n"); */

} /* del_bas */



/*---------------------------------------------------------------
 Routine : rec_unexpand_tree
 Purpose : Reset tree to state before cut sets were generated:

 In this context reset means
   1) set expressions to NULL
   2) discard transfer-in trees
 
 This routine works recursively on a tree of items.
---------------------------------------------------------------*/
static void
  rec_unexpand_tree( ITEM *ip )
{
  ITEM *process_child;

  if (ip == NULL) return;

  switch(ip->type){

    case BASIC:
    case UNDEVELOP:
    case EXTERNAL:
    case COND_NOT_ANAL:
    case COND_ANAL:
      ip->event = NULL;
      return;

    case TRANSIN:
      process_child = ip->process_child;
      if (process_child != NULL) delete_subtree(process_child);
      ip->process_child = NULL;
      ip->num_children = 0;
      ip->event = NULL;
      return;

    case OR:
    case XOR:
    case AND:
    case INHIBIT:
    case PRIORITY_AND:
      process_child = ip->process_child;
      while (process_child != NULL){
        rec_unexpand_tree(process_child);
        process_child = process_child->process_sibling; 
      }
      ip->event = NULL;
      return;

    case INTERMEDIATE:
      printf("What do I do with an INTERMEDIATE event type?\n");
      return;

    case TRANSOUT:
      printf("What do I do with a TRANSOUT event type?\n");
      return;

    default:
      printf("\n*** rec_unexpand_tree: Invalid item type ***\n");
      exit(1);
  }
} /* rec_unexpand_tree */



/*---------------------------------------------------------------
 Routine : expand_tree
 Purpose : To expand the given tree to include any transferred-in
 trees, and to build the list of basic events that the complete tree
 depends upon (many ITEMs may point to the same EVENT structure)

 IF ( tree has not been expanded ) THEN
   delete existing basic events list
   generate processing pointers
   expand tree
   reset basic vals, and set num_bas
 ENDIF
---------------------------------------------------------------*/
void
  expand_tree( TREE *tree )
{
/* printf("expand_tree()\n");  */

    if ( !tree->expanded ) {
        del_bas();

        tree->process_top_item = 
          generate_processing_pointers(tree->top_item);
        rec_expand_tree(tree, tree->process_top_item, tree->database);
/* print_process_item( tree->process_top_item ); */
        tree->expanded = TRUE;

        tree->num_bas = reset_basic_vals();
        num_bas = tree->num_bas;               /* set global variable */
    }
} /* expand_tree */



/*---------------------------------------------------------------
 Routine : unexpand_tree
 Purpose : Reset tree to state before it was expanded
---------------------------------------------------------------*/
void
  unexpand_tree( TREE *tree )
{
    if (tree->process_top_item != NULL){
        rec_unexpand_tree(tree->process_top_item);
    }
} /* unexpand_tree */



/*---------------------------------------------------------------
 Routine : initialise_exprs
 Purpose : To initialise the basic event list for algebraic cut sets

 expr field is initalised with a one-group expression:
      1st event : 10000...
      2nd event : 01000...
      3rd event : 00100...
      etc
---------------------------------------------------------------*/
void
  initialise_exprs()
{
    EVENT *bp;
    Group *g;
    int n = num_bas;
    int done;

    for (bp = first_bas; bp != NULL; bp = bp->next) {
#if 0
		/* do not free memory here */
        if (bp->expr) {
            ExprDestroy(bp->expr);
        }
#endif
        bp->expr = ExprCreate();
        g = GroupCreateN(num_bas);
        BitSet(g->b, --n, 1);
        bp->expr = ExprORGroup(bp->expr, g, &done);

/*         printf("%d  ", n); ExprPrint(bp->expr); */
    }

} /* initialise_exprs */


/*---------------------------------------------------------------
 Routine : free_exprs
 Purpose : Frees the memory taken up by the basic event list 
           expressions for algebraic cut sets
---------------------------------------------------------------*/
void
  free_exprs()
{
    EVENT *bp;
 
    for (bp = first_bas; bp != NULL; bp = bp->next) {
        if (bp->expr) {
            ExprDestroy(bp->expr);
			bp->expr = NULL;
        }
    }
} /* free_exprs */


/*---------------------------------------------------------------
 Routine : BitPara
 Purpose : To create a paragraph containing the list of id's of 
 the basic events given in BitArray

 Steps simultaneously through the BitArray and the BASLIST
---------------------------------------------------------------*/
char **
  BitPara( BitArray *b, int n )
{
    EVENT   *bp;
    char **p = NULL;
    int    i = b->n - 1;  /* backwards! */

    for (bp=first_bas; ( bp != NULL ) && ( i >= 0 ); bp=bp->next) {
        if ( BitGet(b, i) ) {
            char s[80];
            sprintf(s, " %s", bp->id);
            p = ParaGrow(p, s, n);
        }
        i--;
    }
    return p;

} /* BitPara */


/*---------------------------------------------------------------
 Routine : set_bas_prob
 Purpose : To extract event probabilities for the events in the
 event list from the primary events database.
---------------------------------------------------------------*/
void
  set_bas_prob(
    float unit_time )
{
    EVENT     *bp;
    PRIMARY_EVENT   *component;

    bp = first_bas;
    while (bp != NULL) {
      if ( ped_shell_primary_event_exists( bp->id, bp->database ) ) {
        component =
          (PRIMARY_EVENT *)ped_shell_get_primary_event(bp->id, bp->database );

        if ( component->lambda == 'L' ) {
          /* Probability of failure within N unit_time is given as
             1 - e^(-lambda * N)
             Lewis p88
          */ 
          bp->prob = 1.0 - exp( unit_time * (-1.0 * component->prob ) );
        } else {
            bp->prob = component->prob;
        }
      } else {
        bp->prob = 0.0;
      }
      bp = bp->next;
    }

} /* set_bas_prob */


/*---------------------------------------------------------------
 Routine : FailString
 Purpose : To create string containing list of id's of basic events
 given in BitArray
---------------------------------------------------------------*/
static char *
  FailString( BitArray *b )
{
    EVENT  *bp;
    char *s = NULL;
    int   i = b->n - 1; /* backwards! */

    for (bp=first_bas; bp != NULL && i >= 0; bp=bp->next) {
        if ( BitGet(b, i) ) {
            s = strgrow(s, " ");
            s = strgrow(s, bp->id);
        }
        i--;
    }
    return s;

} /* FailString */


/*---------------------------------------------------------------
 Routine : BasicString
 Purpose : To create a string containing the id of a basic event
---------------------------------------------------------------*/
char *
  BasicString( 
    int n,    /* IN  - number of basic events */
    int i )   /* IN  - event index            */
{
    char *fs;
    BitArray *b = BitCreate(n);
    BitSet(b, n-i-1, 1);           /* backwards! */
    fs = FailString(b);
    BitDestroy(b);
    return fs;

} /* BasicString */


/*---------------------------------------------------------------
 Routine : get_probs
 Purpose : To return the array of probabilities of basic events
---------------------------------------------------------------*/
void
  get_probs( float *probs )  /* OUT - array of probabilities         */
{
    EVENT       *bp;
    int        i = 0;

    for (bp = first_bas; bp != NULL; bp = bp->next) {
        probs[i++] = bp->prob;
    }
} /* get_probs */


/*---------------------------------------------------------------
 Routine : set_basic_vals
 Purpose : To set the values of the identified events in the list.

 The value for a bit set (1) is TRUE, for a bit not set (0) is 
 FALSE

 z contains array of values to be set, e.g. [1,5,6] -> 0100011
---------------------------------------------------------------*/
int
  set_basic_vals( int *z )
{
  EVENT *bp;
  int bits_set = 0;
  int n = 0;

  bp = first_bas;
  while (bp != NULL){
    if (n == z[bits_set]) {
      bp->val = TRUE;
      bits_set++;
    } else {
      bp->val = FALSE;
    }
    n++;
    bp = bp->next;
  }
  return n;

} /* set_basic_vals */


/*---------------------------------------------------------------
 Routine : calc_bas_parms
 Purpose : To calculate  
   1) prob of at least one basic event occuring
   2) array of relative probabilities of being the "first" event,
      given that there is at least one failure event.
   3) index into the basic events

 1) P(A+B+C+...) = P( !(!A.!B.!C. ...) ) = (for independent basic events)
                      1 - (1-P(A)) * (1-P(B)) * (1-P(C)) ... )
 
 2) Note that "first" means simply first TRUE event in the list.
    No time order is implied.

    In list A, B, C, ... P(A) = a etc and probs are independent.
       
    P(A is first) = a
    P(B is first) = (1-a)b
    P(C is first) = (1-a)(1-b)c
    ...
 
    The total of these is P(at least one event) as calculated in 1).
    so we divide all the probs by this value.
 
 3) This is just an array of pointers built up as we traverse the list.
---------------------------------------------------------------*/
void
  calc_bas_parms( 
    double *bas_prob, /* OUT - p. at least one basic event failure */
    double *rel,      /* OUT - array of relative probabilities
                               of being the first in the list */
    EVENT **index)    /* OUT - index to basic events */
{
    EVENT       *bp;
    double      p0 = 1.0;      /* prob of zero failures */
    int        i = 0;

    for (bp = first_bas; bp != NULL; bp = bp->next) {
        rel[i] = p0 * bp->prob;
        p0 *= (1.0 - bp->prob);
        index[i++] = bp;
    }
    *bas_prob = 1.0 - p0;

 /* divide all probs by the total */
    if ( *bas_prob > 0.0 ) {
        while (--i >= 0) {
            rel[i] /= *bas_prob; 
        }
    }

} /* calc_bas_parms */
