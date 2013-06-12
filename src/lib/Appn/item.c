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
 Module : Item
 Author : FSC Limited
 Date   : 24/7/96
 
 SccsId : @(#)item.c	1.7 01/28/97
 
 Origin : FTA project,
          FSC Limited Private Venture, Level 2 Prototype.
 
 
 This module creates and manipulates the ITEM structure for the
 given fault tree.
 
 The items hold all the tree construction data, and hold two
 views of the tree. The items know where they are drawn on the
 canvas.
***************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "item.h"
#include "util.h"
#include "PED_shell.h"

#include "MemoryUtil.h"

#include "AssertUtil.h"

ASSERTFILE(__FILE__)

/*---------------------------------------------------------------
 Static global variable used in the output algorithms to give
 a rough guide to the structure of the tree
---------------------------------------------------------------*/
static int tabs = 0;

/*---------------------------------------------------------------
 Routine : is_gate
 Purpose : To return an indication of whether the supplied item
 has the type of AND, OR, XOR, PRIORITY_AND or INHIBIT
---------------------------------------------------------------*/
BOOL
is_gate(ITEM *item)
{
    return (item->type == AND || item->type == OR || item->type == XOR ||
            item->type == INHIBIT || item->type == PRIORITY_AND);

} /* is_gate */

/*---------------------------------------------------------------
 Routine : is_primary
 Purpose : To return an indication of whether the supplied item
 has the type of BASIC, UNDEVELOPED, CONDITIONAL or EXTERNAL
---------------------------------------------------------------*/
BOOL
is_primary(
  ITEM        *item )
{
    return (item->type == BASIC || item->type == UNDEVELOP ||
            item->type == EXTERNAL || item->type == COND_NOT_ANAL ||
            item->type == COND_ANAL);

} /* is_primary */

/*---------------------------------------------------------------
 Routine : is_dormant
 Purpose : To return an indication of whether the supplied item
 is a dormant event
---------------------------------------------------------------*/
BOOL
is_dormant(
ITEM    *item )
{
    return (item->dormant == DORMANT);

} /* is_dormant */

/*---------------------------------------------------------------
 Routine : is_transfer
 Purpose : To return an indication of whether the supplied item
 has the type of TRANSFER-IN or TRANSFER-OUT
---------------------------------------------------------------*/
BOOL
  is_transfer( ITEM *item )
{
    return ( item->type == TRANSOUT || item->type == TRANSIN );

} /* is_transfer */

BOOL
  is_condition( ITEM *item )
{
    return ( ( item->type == COND_ANAL ) || ( item->type == COND_NOT_ANAL ) );

} /* is_condition

/**************************************************************************/

/************************ Build/Load a tree *********************************/

/*---------------------------------------------------------------
 Routine : make_item
 Purpose : Makes an item structure and returns a pointer to it.
 
 Memory is allocated to hold the item.
---------------------------------------------------------------*/
ITEM *
  make_item(void)
{
  ITEM *ip;
  int i;

/*   if ((ip = (ITEM *)malloc(sizeof(ITEM))) == NULL)  */
  if ( !fNewMemory( (void *)&ip, sizeof( ITEM ) ) ) 
  {
    printf("\n*** make_item : malloc failed ***\n");
    exit(1);
  }
  ip->type = BASIC;
  ip->dormant = '\0';
  strcpy(ip->id, ""); 
  ip->event = NULL;

  ip->num_children = 0;
  ip->parent = NULL;
  ip->child = NULL;
  ip->right_sib = NULL;
  ip->left_sib = NULL;
/*   ip->text = strsave(NULL); */
  ip->text = NULL;
  ip->level = 0;
  ip->lptr = NULL;

  ip->process_child = NULL;
  ip->process_sibling = NULL;
 /**ip->prev = NULL; **/

  ip->x = 0;
  ip->y = 0;
  for (i=0; i<MAX_SET_NO; i++) {
    ip->da_x[i] = 0;
    ip->da_y[i] = 0;
  }

  return ip;

} /* make_item */



/*---------------------------------------------------------------
 Routine : generate_processing_pointers
 Purpose : Recursively generates a tree of items which is equivalent
 to the standard tree except that all the intermediate events and
 transfer out items are discarded.
 
 Returns a pointer to the first child which is not an intermediate
 event.
 
 The items hold the pointers for both views of the tree rather than
 having two separate trees pointing to the common items.
---------------------------------------------------------------*/
ITEM *
generate_processing_pointers(ITEM *ip)
{
    ITEM        *right_sib, *next_pp, *next_sibling, *child;

  ASSERT( fValidPointer( ip, sizeof( ITEM ) ) );

 /* First traverse to right hand side of tree via siblings */
    right_sib = ip->right_sib;
    if ( right_sib ) {
        next_pp = generate_processing_pointers(right_sib);            
    } else {
        next_pp = NULL;
    }

 /* next_pp now references right hand side of processing tree
  * Now process this item
  */
    if ( ip->type == INTERMEDIATE ) {  
     /* Ignore it */
        next_sibling = next_pp;     
        next_pp = generate_processing_pointers(ip->child);  
        next_pp->process_sibling = next_sibling;     
    } else {  
      if ( ip->type == TRANSOUT ) {  
        next_pp = generate_processing_pointers(ip->right_sib);  
      }  
      else {  
        child = ip->child;
        if ( child ) {
         /* Examine the child */
            ip->process_child = generate_processing_pointers(child);
            next_pp = ip;
        } else {
         /* Bottom of tree reached */
            ip->process_child = NULL;
            ip->process_sibling = next_pp;
            next_pp = ip; 
        }
      }  
    }  
    return next_pp;

} /* generate_processing_pointers */



/*---------------------------------------------------------------
 Routine : delete_subtree
 Purpose : Recursively deletes the given item and items below it.
 
 All memory allocated to hold the items is returned.
---------------------------------------------------------------*/
void
delete_subtree(ITEM *ip)
{
  ITEM *child, *tmp;

  if (ip != NULL) { 
  ASSERT( fValidPointer( ip, sizeof( ITEM ) ) );

    child = ip->child;
    while (child != NULL){
      tmp = child->right_sib;
      delete_subtree(child);
      child = tmp;
    }
  }
  if ( ip != NULL ) {
    if ( ip->text != NULL) 
    {
      FreeMemory( ip->text );
      ip->text = NULL;
    }
    FreeMemory(ip); 
  }
  return;

} /* delete_subtree */


/*---------------------------------------------------------------
 Routine : update_subtree
 Purpose : Recursively refreshes the item and all items below it
 so that their information reflects the associated data found in
 the Primary Events Database if the item is a primary event.
 
 This information includes the description, the type and whether
 the event is dormant.
---------------------------------------------------------------*/
void
  update_subtree(
    ITEM *ip,
    char *database )
{
  ITEM *child;

  if (ip != NULL) {

  ASSERT( fValidPointer( ip, sizeof( ITEM ) ) );

    /* update event */
    if ( is_primary(ip) ) {
      if ( ped_shell_primary_event_exists( ip->id, database ) ) {
        PRIMARY_EVENT *component = 
          (PRIMARY_EVENT *)ped_shell_get_primary_event( ip->id, database );
/*         item_new_description(ip, component->desc); */
        ip->type = primary_event_item_type(component->type);
        ip->dormant = component->dormant;
      } else {
    
        /* event not found in database */
/* Don't delete the event 'cos it may be the pointer for the Global tree */
/* which doesn't have a database supplied. */
/* Would be better if Global Tree was a copy, rather than a pointer to the
   real tree, except that if the main tree changed, the global tree would
   not be updated. */
/*         ip->event = NULL; */
/*         item_new_description(ip, NULL); */
/*         printf("update_subtree: event %s not in database\n", ip->id);  */
      }
    }

    /* update children */
    child = ip->child;
    while (child != NULL) {
      update_subtree(child, database );
      child = child->right_sib;        
    }
  }
} /* update_subtree */




/*---------------------------------------------------------------
 Routine : item_new_description
 Purpose : To set the description for item to that supplied.
---------------------------------------------------------------*/
void
item_new_description(ITEM *item, char *desc)
{
  ASSERT( fValidPointer( item, sizeof( ITEM ) ) );

    if ( item->text != NULL ) 
    {
      FreeMemory( item->text );
      item->text = NULL;
    }
if ( desc != NULL ) 
{ 
    item->text = strsave(desc);
} 
} /* item_new_description */



/*---------------------------------------------------------------
 Routine : load_item
 Purpose : Recursively loads the item and all items in the tree
 below, building the internal data structures.
 
 Each item is linked with it's parent and siblings to form a
 tree structure. It would be better to restrict tree building
 to the tree module, but the work involved precludes this at
 this time (25/7/96)
 
 Memory is allocated to hold the items.
 
 If an item can not be loaded NULL is returned.

 Only reads text for intermediate events, then looks for id in the
 database. 
 
 Q: The code contained the following:

 Why not do this ???
 
 ?  IF (intermediate) THEN
 ?     use text from file
 ?  ELSE
 ?     IF (event in database) THEN
 ?        use data from database
 ?     ELSE
 ?        use data from file
 ?     ENDIF
 ?  ENDIF
 A:
---------------------------------------------------------------*/
ITEM *
  load_item(
    FILE *fp, 
    ITEM *parent,
    char *database )
{
  ITEM *ip, *tip;
  char type[3];
  int n;
  int length;
  char *text;
  int c;

  ASSERT( fp != NULL );

  ip = make_item();

  if (fscanf(fp, "%s %s %d\n", type, ip->id, &n) < 3) {
    printf("ERROR READING EVENT TYPE AND ID FROM FILE\n");
    return NULL;
  }

  if ( !strcmp(ip->id, "NULL") ) {
    strcpy(ip->id, "");
  }
  ip->type    = type[0];
  ip->dormant = type[1];  /* will be '\0' if not specified */

 /* Load text for INTERMEDIATE events only */
  if (ip->type == INTERMEDIATE) {
    if (fscanf(fp, "%d", &length) < 1) {
      printf("ERROR READING EVENT DESCRIPTION TEXT LENGTH FROM FILE\n");
      return NULL;
    } else {
      if ( ip->text != NULL ) FreeMemory( ip->text );
      if ( !fNewMemory( (void *)&text, length + 1 ) )
      {
        printf("\n*** load_item : malloc failed ***\n");
        exit(1);
      }
      ip->text = text;

      if (length) {
        if (fgetc(fp) != ' ') {
          printf("ERROR READING FIELD SEPARATOR FROM FILE\n");
          return NULL;
        }

	if (!fgets(ip->text, length+1, fp)) {
	  printf("ERROR READING EVENT DESCRIPTION TEXT FROM FILE\n");
	  return NULL;
	}

      } else {
	ip->text[0] = '\0';
      }

      /*
      if (!fgets(ip->text, length, fp)) {
        printf("ERROR READING EVENT DESCRIPTION TEXT FROM FILE\n");
        return NULL;
      }
      */
      /* flush EOL */
      /*
      do {
	c = fgetc(fp);
      }  while (c == 10 || c == 13);
      ungetc(c, fp);
      */
      
      if (fscanf(fp, "\n") == EOF) {
        printf("ITEM: ERROR READING EVENT TERMINATOR FROM FILE\n");
        return NULL;
      } 

      /*
      if (fgetc(fp) != '\n') {
        printf("ITEM: ERROR READING EVENT TERMINATOR FROM FILE\n");
        return NULL;
      }
      */
    }
  } 
/* End load text */

/* Search database for primary events */
  if ( is_primary(ip) ) {
    PRIMARY_EVENT *component;

     /* if found, use data in database */
    if ( ped_shell_primary_event_exists( ip->id, database ) ) {
      component = 
        (PRIMARY_EVENT *)ped_shell_get_primary_event(ip->id, database );
/*       item_new_description(ip, component->desc); */
      ip->type = primary_event_item_type(component->type);
      ip->dormant = component->dormant;

    }
  }

  ip->event = NULL;
  ip->parent = NULL;
  ip->child = NULL;
  ip->right_sib = NULL;
  ip->left_sib = NULL;

 /* add event to tree - we should really use add_child() to avoid duplication */
 /* add_child can't at present be used as it does more than just adding a
    child item into the tree (though to be strictly true, it doesn't do that
    anyway as the items are linked to make a tree, not inserted into a
    tree structure). It also does not cater for parent == NULL.
    Also, load_item is used recursively so where to do the load_item? 
    MPA 12/4/96 */

  if (parent == NULL) {   
    ip->parent = NULL;   
  } else {   
    ip->parent = parent;   
   
    if (parent->child == NULL) {   
      parent->child = ip;   
    } else {   
      tip = parent->child;   
      while (tip->right_sib != NULL) {   
        tip = tip->right_sib;   
      }   
      tip->right_sib = ip;   
      ip->left_sib = tip;   
    }   
  }   

  if ( ( ip->type == BASIC ) || 
       ( ip->type == TRANSIN ) ||
       ( ip->type == TRANSOUT ) ) {   
    ip->num_children = 0;   
  } else {   
    ip->num_children = n;   
    for (n=1; n<= ip->num_children; n++) {   
      if (!load_item(fp, ip, database)) {  
        return NULL;  
      }  
    }  
  }  

  return ip;

} /* load_item */


/*---------------------------------------------------------------
 Routine : copy_subtree
 Purpose : Recursively copies the given item and items below it,
 returning a pointer to the copy.
 
 This copying process allocates memory for each item copied.
---------------------------------------------------------------*/
ITEM *
copy_subtree(ITEM *ip)
{
    ITEM *item, *child, *ipchild, *tmp;

    if (ip == NULL) {
        return NULL;
    } else {
        item = make_item();

        item->type = ip->type;
        item->dormant = ip->dormant;
     /* item->ref  = ip->ref; */
        strcpy(item->id, ip->id);
        item->num_children = ip->num_children;
        item->event = ip->event;
        /* Copy description for Intermediate events only. All other
           events should get their text from the database. */
        if ( item->type == INTERMEDIATE )
          item_new_description(item, ip->text);
        ipchild = ip->child;
        if (ipchild != NULL) {
            item->child = copy_subtree(ipchild);
            child = item->child;
            child->parent = item;
            ipchild = ipchild->right_sib;
            while (ipchild != NULL){
                tmp = copy_subtree(ipchild);
                child->right_sib = tmp;
                tmp->left_sib = child;
                tmp->parent = item;
                child = tmp;
                ipchild = ipchild->right_sib;
            }
        }


        return item;
    }

} /* copy_subtree */


/************************** Save trees ***************************/

/*---------------------------------------------------------------
 Routine : save_item
 Purpose : Recursively saves the given item, and all items below
 it, to supplied file.
---------------------------------------------------------------*/
void
save_item(
FILE *fp,
ITEM *ip )
{
        ITEM *child;
        char *id;

        if (ip == NULL) return;

        if ( strcmp(ip->id, "") ) {
                id = ip->id;
        } else {
                id = "NULL";
        }

#if 0
        switch(ip->type){
        case BASIC:
                        fprintf(fp, "B ");
                        break;
        case OR:
                        fprintf(fp, "O ");
                        break;
        case XOR:
                        fprintf(fp, "X ");
                        break;
        case AND:
                        fprintf(fp, "A ");
                        break;
        case INHIBIT:
                        fprintf(fp, "H ");
                        break;
        case PRIORITY_AND:
                        fprintf(fp, "P ");
                        break;
        case INTERMEDIATE:
                        fprintf(fp, "M ");
                        break;
        case UNDEVELOP:
                        fprintf(fp, "U ");
                        break;
        case EXTERNAL:
                        fprintf(fp, "E ");
                        break;
        case COND_NOT_ANAL:
        case COND_ANAL:
                        fprintf(fp, "C ");
                        break;
        case TRANSIN:
                        fprintf(fp, "I ");
            break;
    case TRANSOUT:
            fprintf(fp, "T ");
            break;
        default:
                        printf("\n*** save_item: Invalid item type ***\n\n");
                        exit(1);
        }
#endif

    fprintf(fp, "%c", ip->type);             /* type */

    if (ip->dormant) {                       /* dormant flag, if present */
        fprintf(fp, "%c", DORMANT);
    }

    fprintf(fp, " %s ", id);                 /* id */

        fprintf(fp, "%d \n", ip->num_children);  /* number of children */

 /* Save text for INTERMEDIATE events only */
    if (ip->type == INTERMEDIATE) {
      if ( ip->text != NULL )
      {
        fprintf(fp, "%d", strlen(ip->text));
        if (strlen(ip->text)) {
            fprintf(fp, " %s", ip->text);
                }
      }
      else
      {
        fprintf(fp, "%d", 0 );
/*         fprintf(fp, " " ); */
      }
        fputc('\n', fp);
    }

        child = ip->child;
        while (child != NULL){
                save_item(fp, child);
                child = child->right_sib;        
        }

} /* save_item */


