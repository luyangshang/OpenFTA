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
 Module : Util
 Author : FSC Limited
 Date   : 25/11/95

 SccsId :@(#)util.h	1.2 09/26/96

 Provides utility functions 
 
****************************************************************/

#ifndef UTIL_H
#define UTIL_H

#include "MathMacros.h"

/*--------------------------------------------------------------
 Routine : strsave
 Purpose : save a string in new memory
           YOUR reponsibility to free memory (with free( RET ))
---------------------------------------------------------------*/
extern char *            /* RET - copy (in new memory)               */
  strsave( char *s );    /* IN  - a null terminated string (or NULL) */



/*--------------------------------------------------------------
 Routine : strgrow
 Purpose : concatenate a new string to an existing string, using realloc
           YOUR reponsibility to free memory (with free( RET ))
---------------------------------------------------------------*/
extern char *  /* RET - new address of string  s, containing s + r      */
  strgrow( 
    char *s,   /* IN  - a malloced, null terminated  string (or NULL)   */
    char *r ); /* IN  - null terminated string to append to s (or NULL) */


/*--------------------------------------------------------------
 Routine : strfree
 Purpose : Free an allocated string.
---------------------------------------------------------------*/
extern void
  strfree(
    char *pStr );

/*--------------------------------------------------------------
 a paragraph is a null-terminated array of char **'s, each
 pointing to a null terminated string.
---------------------------------------------------------------*/

/*--------------------------------------------------------------
 Routine : ParaCreate
 Purpose : create a paragraph and initialise with a string
---------------------------------------------------------------*/
extern char **
  ParaCreate(
    char *s, 
    int n);

/*--------------------------------------------------------------
 Routine : ParaDestroy
 Purpose : free a paragraph and all its strings
---------------------------------------------------------------*/
extern void
  ParaDestroy( 
    char **p);

/*--------------------------------------------------------------
 Routine : ParaPrint
 Purpose : print a paragraph
---------------------------------------------------------------*/
extern void
  ParaPrint( 
    char **p);


/*--------------------------------------------------------------
 Routine : ParaGrow
 Purpose : concatenate a new string to an existing paragraph, using realloc
---------------------------------------------------------------*/
extern char ** /* RET - new address of paragraph, containing p + s      */
  ParaGrow( 
    char **p,  /* IN  - a malloced, null terminated  string (or NULL)   */
    char  *s,  /* IN  - line length                                     */
    int    n );/* IN  - null terminated string to append to p (or NULL) */


/*--------------------------------------------------------------
 Routine : intcopy
 Purpose : copy an array of integers, b to a
---------------------------------------------------------------*/
extern int *
  intcopy( 
    int *a, 
    int *b, 
    int n );   /* two arrays, number of elements */


/*--------------------------------------------------------------
 Routine : intsave
 Purpose : save an array of integers
---------------------------------------------------------------*/
extern int *
  intsave( 
    int *a, 
    int n );          /* array, number of elements */


/*--------------------------------------------------------------
 Routine : time_string
 Purpose : get current local time as a string
---------------------------------------------------------------*/
extern char *
  time_string( void );


/*--------------------------------------------------------------
 Routine : swap
 Purpose : swap elements of an array of pointers
           uses void** to work with any kind of pointers
---------------------------------------------------------------*/
extern void
  swap(
    void **a, 
    int i, 
    int j);

#endif
