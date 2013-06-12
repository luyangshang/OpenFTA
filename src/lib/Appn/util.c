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
 
 SccsId :@(#)util.c	1.3 10/29/96
 
 Provides utility functions
 
****************************************************************/
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "util.h"

#include "MemoryUtil.h"

/*--------------------------------------------------------------
 Routine : strsave
 Purpose : save a string in new memory
           YOUR reponsibility to free memory (with free( RET ))
---------------------------------------------------------------*/
char *                 /* RET - copy (in new memory)               */
strsave( char *s )     /* IN  - a null terminated string (or NULL) */
{
    char *t;
    if ( s == NULL ) {
/*         if ( (t  = (char *)malloc( 1 )) == NULL) { */
/*             printf("\n*** strsave : malloc failed ***\n"); */
/*             exit(1); */
/*         } */
if ( fNewMemory( (void *)&t, 1 ) )
        *t = '\0';
else
  exit( 1 );

    } else {
/*         if ( (t = (char *)malloc( strlen(s) + 1 )) == NULL) { */
/*             printf("\n*** strsave : malloc failed ***\n"); */
/*             exit(1); */
/*         } */
if ( fNewMemory( (void *)&t, strlen(s) + 1 ) )
        strcpy(t, s);
else
  exit( 1 );

    }
    return t;
}


/*--------------------------------------------------------------
 Routine : strgrow
 Purpose : concatenate a new string to an existing string, using realloc
           YOUR reponsibility to free memory (with free( RET ))
---------------------------------------------------------------*/
char *              /* RET - new address of string  s, containing s + r      */
strgrow( char *s,   /* IN  - a malloced, null terminated  string (or NULL)   */
         char *r )  /* IN  - null terminated string to append to s (or NULL) */
{
    if ( s == NULL ) {
        return strgrow(strsave(NULL), r);
    }

    if ( r == NULL ) {
        return s;
    }

/*     if ( (s = (char *)realloc( s, strlen(s) + strlen(r) + 1 )) == NULL) { */
/*         printf("\n*** strgrow : realloc failed ***\n"); */
/*         exit(1); */
/*     } */
    if ( fResizeMemory( (void *)&s, strlen(s) + strlen(r) + 1 ) )
        return strcat(s, r);
    else
      exit( 1 );

  return NULL;
}

/*--------------------------------------------------------------
 Routine : strfree
 Purpose : Free an allocated string.
---------------------------------------------------------------*/
void
  strfree(
    char *pStr )
{
  FreeMemory( pStr );
} /* strfree */

/*--------------------------------------------------------------
 a paragraph is a null-terminated array of char **'s, each
 pointing to a null terminated string.
---------------------------------------------------------------*/

/*--------------------------------------------------------------
 Routine : ParaCreate
 Purpose : create a paragraph and initialise with a string
---------------------------------------------------------------*/
char **
ParaCreate(char *s, int n)
{
    char **p;

    if ((int)strlen(s) > n) {
        printf("ParaCreate: string too long\n");
        return NULL;
    }

/*     if ( (p = (char **)malloc( 2 * sizeof(char *))) == NULL ) { */
    if ( !fNewMemory( (void *)&p, ( 2 * sizeof(char *) ) ) ) 
    {
        printf("\n*** ParaCreate : malloc failed ***\n");
        exit(1);
    }

    p[0] = strsave(s);
    p[1] = NULL;

    return p;
}


/*--------------------------------------------------------------
 Routine : ParaDestroy
 Purpose : free a paragraph and all its strings
---------------------------------------------------------------*/ 
void
ParaDestroy( char **p)
{
    char **q;

    for ( q = p; *q != NULL; q++ ) {
        FreeMemory(*q);
    }
    FreeMemory(p);
}


/*--------------------------------------------------------------
 Routine : ParaPrint
 Purpose : print a paragraph
---------------------------------------------------------------*/
void
ParaPrint( char **p)
{
    printf("p = %p:\n", p);
    while ( *p != NULL ) {
        printf("%p: %s\n", *p, *p);
        p++;
    }
    printf("%p:\n", *p);
}


/*--------------------------------------------------------------
 Routine : ParaGrow
 Purpose : concatenate a new string to an existing paragraph, using realloc
---------------------------------------------------------------*/
char **              /* RET - new address of paragraph, containing p + s      */
ParaGrow( char **p,  /* IN  - a malloced, null terminated  string (or NULL)   */
          char  *s,  /* IN  - line length                                     */
          int    n ) /* IN  - null terminated string to append to p (or NULL) */
{
    char **q;
    int    i;

    if ( p == NULL ) {
        return ParaCreate(s, n);
    }

    if ( s == NULL ) {
        return p;
    }

    if ((int)strlen(s) > n) {
        printf("paragrow: string too long\n");
        return p;
    }

    for ( i = 0, q = p; *q != NULL; q++, i++ )
       ;
    q--;

    if ( (int)(strlen(*q) + strlen(s)) > n ) {
/*         if ( (p = (char **)realloc( p, (i+2)*sizeof(char*) )) == NULL) { */
        if ( !fResizeMemory( (void *)&p, (i+2)*sizeof(char*) ) ) {
            printf("\n*** strgrow : paragrow failed ***\n");
            exit(1);
        }
        p[i]   = strsave(s);
        p[i+1] = NULL;

    } else {
        *q = strgrow(*q, s);
    }

    return p;
}


/*--------------------------------------------------------------
 Routine : intcopy
 Purpose : copy an array of integers, b to a
---------------------------------------------------------------*/
int *
intcopy( int *a, int *b, int n )
{
    while (--n >= 0) {
        a[n] = b[n];
    }
    return a;
}


/*--------------------------------------------------------------
 Routine : intsave
 Purpose : save an array of integers
---------------------------------------------------------------*/
int *
intsave( int *a, int n )
{
    int *p;

    if ( a == NULL || n <= 0 ) {
        return NULL;
    }
/*     if ( (p = (int *)malloc( n * sizeof(int) )) == NULL ) { */
    if ( !fNewMemory( (void *)&p, ( n * sizeof(int) ) ) ) 
    {
        printf("\n*** intsave : malloc failed ***\n");
        exit(1);
    }
    return intcopy(p, a, n);
}


/*--------------------------------------------------------------
 Routine : time_string
 Purpose : get current local time as a string
---------------------------------------------------------------*/
char *
time_string (void)
{
    time_t tt = time(&tt);
    char  *t = strsave(ctime(&tt));

    while ( t[strlen(t)-1] == '\n' ) {
        t[strlen(t)-1] = '\0';
    }
    return t;
}

/*--------------------------------------------------------------
 Routine : swap
 Purpose : swap elements of an array of pointers
           uses void** to work with any kind of pointers
---------------------------------------------------------------*/
void
swap(void **a, int i, int j)
{
    void *tmp = a[i];
    a[i] = a[j];
    a[j] = tmp;
}


/*--------------------------------------------------------------
 Routine : main
 Purpose : test function, only gets included if compiled with TEST
           option
---------------------------------------------------------------*/
#ifdef TEST
main()
{
    char **p = NULL;

    p = ParaGrow(p, " one", 20);
    ParaPrint(p);
    p = ParaGrow(p, " two", 20);
    printf("\n");
    ParaPrint(p);
    p = ParaGrow(p, " three", 20);
    printf("\n");
    ParaPrint(p);
    p = ParaGrow(p, " four", 20);
    printf("\n");
    ParaPrint(p);
    p = ParaGrow(p, " five", 20);
    printf("\n");
    ParaPrint(p);
    p = ParaGrow(p, " six", 20);
    printf("\n");
    ParaPrint(p);
    p = ParaGrow(p, " seven", 20);
    printf("\n");
    ParaPrint(p);
    p = ParaGrow(p, " eight", 20);
    printf("\n");
    ParaPrint(p);
    p = ParaGrow(p, " nine", 20);
    printf("\n");
    ParaPrint(p);
    p = ParaGrow(p, " ten", 20);
    printf("\n");
    ParaPrint(p);
    p = ParaGrow(p, " eleven", 20);
    printf("\n");
    ParaPrint(p);
    p = ParaGrow(p, " twelve", 20);
    printf("\n");
    ParaPrint(p);
    p = ParaGrow(p, " thirteen", 20);
    printf("\n");
    ParaPrint(p);
    p = ParaGrow(p, " fourteen", 20);
    printf("\n");
    ParaPrint(p);
    p = ParaGrow(p, " fifteen", 20);
    printf("\n");
    ParaPrint(p);
    p = ParaGrow(p, " sixteen", 20);
    printf("\n");
    ParaPrint(p);
    p = ParaGrow(p, " seventeen", 20);
    printf("\n");
    ParaPrint(p);
    p = ParaGrow(p, " twenty-four thousand", 20);
    printf("\n");
    ParaPrint(p);
    p = ParaGrow(p, " thirty", 20);
    printf("\n");
    ParaPrint(p);
/*
*/

}
#endif
