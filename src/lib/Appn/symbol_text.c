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
 Module : Symbol Text
 Author : FSC Limited
 Date   : 29/11/95

 SccsId :@(#)symbol_text.c	1.6 11/22/96

 Purpose :
   This module manipulates the text associated with a symbol, i.e. the
   ID and description of a symbol.

****************************************************************/

#include <stdlib.h>
#include <stdio.h>

#include <ctype.h>
#include <string.h>

#include "boolean.h"
#include "symbol_text.h"
#include "MathMacros.h"

#include "StringUtilities.h"

#include "MemoryUtil.h"

/* #include "AssertUtil.h"  */
/*   */
/* ASSERTFILE(   __FILE__   )  */

/*--------------------------------------------------------------
 Routine : pre_process_text
 Purpose : Processes the specified text so that all words are
           separated by '\0' characters.
           The processed text is stored in 'buffer'.
           Returns the number of characters in 'buffer'.
---------------------------------------------------------------*/
static unsigned
     pre_process_text( char *buffer, char *text );

/*--------------------------------------------------------------
 Routine : create_line
 Purpose : Modifies the supplied text buffer to create a line of symbol text.
           The supplied text must be pre-processed so that all words
           are separated by '\0' characters.
           The created line will fit inside the specified width of
           symbol unless the first word is too large, in which case
           the line will consist of just this word.
           Returns the number of characters in the created line.
---------------------------------------------------------------*/
static unsigned
create_line( 
            Canvas canvas, unsigned symbol_width,
            char *text, char *buffer_end );


/*--------------------------------------------------------------
 Routine : create_text_as_is
 Purpose : This routine is provided for the draw_post_it_note routine
           which needs to preserve the layout of the text within the
           symbol, and not change it as at present.
---------------------------------------------------------------*/
TEXT *
create_text_as_is( 
                  Canvas canvas,
                  char *text,
                  SYMBOL *symbol )
{
  TEXT *divided_text;
  char *remaining_text, *buffer_end;
  unsigned line_length, line_number;
  char *anchor;

  if ( !fNewMemory( ( void * ) &remaining_text, ( strlen( text ) + 1 ) ) )
  {
    exit( 1 );
  }

  anchor = remaining_text;

  remaining_text = strcpy( remaining_text, text );
  remaining_text[ strlen( remaining_text ) ] = '\0';

/* Losing one character descriptions. They may be useless but... */
/*   buffer_end = strlen(   remaining_text   ) + remaining_text - 1;  */
  buffer_end = strlen( remaining_text ) + remaining_text;

  if ( !fNewMemory( ( void * ) &divided_text, sizeof( TEXT ) ) )
  {
    printf( "\n*** create_text : malloc failed ***\n" );
    exit( 1 );
  }

  line_length = 0;
  line_number = 0;
  while ( remaining_text < buffer_end )
  {
    char *pTextLine;

/*     divided_text->text[  line_number  ] = remaining_text;  */
    if ( SU_Number_Of_Char( '\n', remaining_text, FALSE ) != 0 )
    {
      line_length +=
        ( SU_Location_Of( '\n', remaining_text, FALSE, TRUE ) + 1 );
      remaining_text[ line_length - 1 ] = '\0';
      fNewMemory( ( void * ) &pTextLine, ( strlen( remaining_text ) + 1 ) );
      strcpy( pTextLine, remaining_text );
      divided_text->text[ line_number ] = pTextLine;

      remaining_text += line_length;
      line_length = 0;
    }
    else
    {
      line_length +=
        ( strlen( remaining_text ) );
      fNewMemory( ( void * ) &pTextLine, ( strlen( remaining_text ) + 1 ) );
      strcpy( pTextLine, remaining_text );
      divided_text->text[ line_number ] = pTextLine;

      remaining_text += line_length;
      line_length = 0;
    }
    line_number++;
  }
  divided_text->nlines = line_number - 1;

  FreeMemory( anchor );

  return divided_text;

} /* create_text_as_is */

/*--------------------------------------------------------------
 Routine : create_text
 Purpose : Divides the given text string to lengths that fit in the symbol.
           The divided text is stored in a TEXT structure.
           Space is allocated to hold the divided text.
           The divided text is retured.
---------------------------------------------------------------*/
TEXT *        /* RET - the divided text */
create_text( 
            Canvas canvas,        /* IN  - canvas           */
            char *text,        /* IN  - raw text         */
            SYMBOL *symbol )        /* IN  - symbol           */
{
  TEXT *divided_text;
  char *buffer, *remaining_text, *buffer_end;
  unsigned line_length, line_number;
  unsigned short i;

  if ( text == NULL )
  {
    text = "\0";
  }

  /* First copy the text to a buffer removing duplicate white space
     and delimiting words with '\0' characters */
  if ( !fNewMemory( ( void * ) &buffer, ( strlen( text ) + 1 ) ) )
  {
    printf( "\n*** create_text : malloc failed ***\n" );
    exit( 1 );
  }

  buffer_end = pre_process_text( buffer, text ) + buffer;

  remaining_text = buffer;

  /* Now build an array of text lines */
  if ( !fNewMemory( ( void * ) &divided_text, sizeof( TEXT ) ) )
  {
    printf( "\n*** create_text : malloc failed ***\n" );
    exit( 1 );
  }
  for ( i = 0; i < Max_Number_Of_Symbol_Text_Lines; i++ )
    divided_text->text[ i ] = NULL;

  if ( remaining_text == buffer_end )
  {        /* NULL string */
    divided_text->nlines = 0;
    divided_text->text[ divided_text->nlines ] = remaining_text;
  }
  else
  {
    for ( line_number = 0; remaining_text < buffer_end; line_number++ )
    {
      char *pTextLine;
      if ( line_number > ( Max_Number_Of_Symbol_Text_Lines - 1 ) )
      {
/*         printf(   */
/*           "create_text : line_number exceeds " */
/*             "symbol->text_lines_allowed\n"  ); */

        break;
      }
/*       divided_text->text[  line_number  ] = remaining_text; */
      line_length =
        create_line( 
                    canvas,
                    ( unsigned ) ( symbol->textwidth ),
                    remaining_text,
                    buffer_end );

      fNewMemory( ( void * ) &pTextLine, ( strlen( remaining_text ) + 1 ) );
      strcpy( pTextLine, remaining_text );
      divided_text->text[ line_number ] = pTextLine;

      remaining_text += line_length;
    }
    divided_text->nlines = line_number - 1;
  }

  FreeMemory( buffer );

  return divided_text;
}

/*--------------------------------------------------------------
 Routine : pre_process_text
 Purpose : Processes the specified text so that all words are
           separated by '\0' characters.
           The processed text is stored in 'buffer'.
           Returns the number of characters in 'buffer'.
---------------------------------------------------------------*/
static unsigned
pre_process_text( 
                 char *buffer,
                 char *text )
{
  unsigned i;
  unsigned buffer_count = 0;
  BOOL word_found = FALSE;

  for ( i = 0; text[ i ] != '\0'; i++ )
  {
    if ( !isspace( text[ i ] ) )
    {
      buffer[ buffer_count++ ] = text[ i ];
      word_found = TRUE;
    }
    else
      if ( word_found )
      {
        buffer[ buffer_count++ ] = '\0';
        word_found = FALSE;
      }
  }
  buffer[ buffer_count ] = text[ i ];
  return buffer_count;
}

/*--------------------------------------------------------------
 Routine : create_line
 Purpose : Modifies the supplied text buffer to create a line of symbol text.
           The supplied text must be pre-processed so that all words
           are separated by '\0' characters.
           The created line will fit inside the specified width of
           symbol unless the first word is too large, in which case
           the line will consist of just this word.
           Returns the number of characters in the created line.
---------------------------------------------------------------*/
static unsigned
create_line( 
            Canvas canvas,
            unsigned symbol_width,
            char *text,
            char *buffer_end )
{
  char *next_word;

  /* First check if the word is too large for the symbol */
  /* This is because the words in text are separated by '\0' */
  if ( CanvasTextWidth( canvas, text ) > symbol_width )
  {
/*        printf(  "Word too long for symbol\n"  );  */
    return strlen( text ) + 1;
  }

  /* Now build up a line by replacing '\0' characters with spaces */
  next_word = text;
  while ( ( next_word < buffer_end ) &&
         ( CanvasTextWidth( canvas, text ) <= symbol_width ) )
  {
    next_word += strlen( next_word ) + 1;
    *( next_word - 1 ) = ' ';
  }

  /* Replace the most recent space to mark the string end */
  *( next_word - 1 ) = '\0';

  return next_word - text;
}

/*--------------------------------------------------------------
 Routine : AddContinuation
 Purpose : To add the "..." continuation to strings that require
 truncation.
 ToString is an allocated string.
---------------------------------------------------------------*/
static char *
  AddContinuation(
    char *ToString )
{
  char *pNewTextLine;
  char *pLastSpace;
  const char *continuation = "...";

  fNewMemory( 
             ( void * ) &pNewTextLine,
             ( strlen( ToString ) + strlen( continuation ) + 1 ) );
  strcpy( pNewTextLine, ToString );
  FreeMemory( ToString );
  pLastSpace = strrchr( pNewTextLine, ' ' );
  if ( pLastSpace != NULL )
  {
    strcpy( pLastSpace + 1, continuation );
  }
  else
  {
    if ( strlen( pNewTextLine ) > strlen( continuation ) )
    {
      char *pStr = pNewTextLine;

      strcpy( 
             &pStr[ strlen( pNewTextLine ) - strlen( continuation ) ],
             continuation );
    }
    else
    {
      strcat( pNewTextLine, continuation );
    }
  }
  return pNewTextLine;

} /* AddContinuation */

/*--------------------------------------------------------------
 Routine : draw_symbol_text
 Purpose : Draws the text in the centre of the symbol.
           The point is the top centre of the symbol.
---------------------------------------------------------------*/
void
draw_symbol_text( Canvas canvas, TEXT *text,
                 SYMBOL *symbol, CanvasCoord symbol_centre )
{
  CanvasCoord start;
  unsigned fontheight;
  int  textheight, linewidth;
  float symb_horzcntr, symb_vertcntr, symbol_cornerx;
  int  i;

  fontheight = CanvasFontHeight( canvas );
/*     textheight = fontheight * (  text->nlines+1  ); */
  textheight =
    fontheight * MIN( text->nlines + 1, symbol->text_lines_allowed );
  symb_vertcntr = ( symbol->depth - ( float ) textheight ) / 2.0;
  symbol_cornerx = ( float ) symbol_centre.x - ( symbol->width / 2.0 );
  start.y = symbol_centre.y + ( unsigned ) symb_vertcntr + fontheight;
  if ( text->nlines >= 0 )
    for ( i = 0; i <= text->nlines; i++ )
    {
      linewidth = CanvasTextWidth( canvas, text->text[ i ] );
      symb_horzcntr = ( symbol->width - ( float ) linewidth ) / 2.0;
      start.x = ( unsigned ) ( symbol_cornerx + symb_horzcntr );

      /* If lines > number allowed in symbol, add continuation
         indication */
      if ( i < ( symbol->text_lines_allowed - 1 ) )
      {
        CanvasWriteText( canvas, text->text[ i ], start );
      }
      else
      {
        /* If this is not the last line ... */
        if ( text->text[ i + 1 ] != NULL )
        {
          text->text[ i ] = AddContinuation( text->text[ i ] ); 
        }

        CanvasWriteText( canvas, text->text[ i ], start );
        break;
      }
      start.y += fontheight;
    }
}

/*--------------------------------------------------------------
 Routine : draw_symbol_text_as_is
 Purpose :
---------------------------------------------------------------*/
void
draw_symbol_text_as_is( 
                       Canvas canvas,
                       TEXT *text,
                       SYMBOL *symbol,
                       CanvasCoord symbol_centre )
{
  CanvasCoord start;
  unsigned fontheight;
  int  textheight, linewidth;
  float symb_horzcntr, symb_vertcntr, symbol_cornerx;
  int  i;

  fontheight = CanvasFontHeight( canvas );
  textheight = fontheight * ( text->nlines + 1 );
/*     symb_vertcntr = (  symbol->depth - (  float  )textheight  ) / 2.0; */
  symb_vertcntr = ( symbol->depth / 5.0 );        /* 5 lines */
  symbol_cornerx = ( float ) symbol_centre.x - ( symbol->width / 2.0 );
  start.y = symbol_centre.y + ( unsigned ) symb_vertcntr + fontheight;
  if ( text->nlines >= 0 )
    for ( i = 0; i <= text->nlines; i++ )
    {
      linewidth = CanvasTextWidth( canvas, text->text[ i ] );
/*             symb_horzcntr = (  symbol->width - (  float  )linewidth  ) / 2.0; */
      symb_horzcntr = ( symbol->width / 40.0 );        /* 40 columns */
      start.x = ( unsigned ) ( symbol_cornerx + symb_horzcntr );
      CanvasWriteText( canvas, text->text[ i ], start );
      start.y += fontheight;
    }
} /* draw_symbol_text_as_is */

/*--------------------------------------------------------------
 Routine : draw_id
 Purpose : Draws the id to the right hand side above the symbol.
           The point is the top centre of the symbol.
---------------------------------------------------------------*/
void
draw_id( Canvas canvas, CanvasCoord symbol_centre, char *id )
{
  CanvasCoord start;
  unsigned fontheight;

  fontheight = CanvasFontHeight( canvas );
  start.x = symbol_centre.x + fontheight / 2;
  start.y = symbol_centre.y - fontheight;
  CanvasWriteText( canvas, id, start );
}

/*--------------------------------------------------------------
 Routine : delete_text
 Purpose : Deletes the divided text.
           All space allocated to hold the divided text is returned.
---------------------------------------------------------------*/
void
delete_text( TEXT *divided_text )
{
  unsigned short i;

  for ( i = 0; i <= divided_text->nlines; i++ )
    FreeMemory( divided_text->text[ i ] );

  FreeMemory( divided_text );
}
