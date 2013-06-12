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
 Module : Bits
 Author : FSC Limited
 Date   : 19/12/94

 This module provides routines for manipulating arrays of bits

 SccsId : @(#)bits.h	1.1 08/22/96
 
 The bits are stored in an array of a suitable single-byte type.
 In this implementation 'unsigned char' is used. In the creation
 function 'BitCreate', n signifies the number of BITS. For up to
 8 bits it will create 1 byte. For 9-16 bits, 2 bytes etc.

 This is true for any system for which unsigned char has an 8 bit
 representation.

 Use of BitSet and BitGet should be fully portable. The actual bit
 patterns may not be, so be careful if you want to use it for a
 hardware application (e.g., bit 0 may or may not be RH LSB).
 
****************************************************************/

#ifndef BITS_H
#define BITS_H

/*--------------------------------------------------------------
 Definitions of macros required to cater in software for the
 ability to cross alignment boundaries for large numbers of
 bits in an array ( large > 8 )
---------------------------------------------------------------*/
#define BITS_N          8 /* 8 bits to a byte. Valid for Sun */
#define BITS_ALL_SET  255 /* 0xff */
#define BITS_BYTE_VAL 256
#define BITS_LONG      32

#define BITS_BYTES( n ) ((n + BITS_N - 1) / BITS_N)

/*--------------------------------------------------------------
 Define visible BitArray type and the types it is dependent upon
---------------------------------------------------------------*/
typedef unsigned char BIN_TYPE;

typedef struct _BitArray {
    int            n;       /* number of bits   */
    BIN_TYPE      *a;       /* pointer to array */
} BitArray;


/*--------------------------------------------------------------
 Routine : BitCreate
 Purpose : Create an array of n bits

 This routine allocates memory. It is the callers responsibility 
 to free memory with BitDestroy
--------------------------------------------------------------*/
extern BitArray *     /* RET - the new bit array */
  BitCreate( int n ); /* IN  - number of bits    */

/*
 * name:
 *   BitDestroy
 * summary:
 *   free all memory associated with bit array b
 */
extern void
  BitDestroy( BitArray *b );  /* IN  - bit array to destroy */


/*
 * name:
 *   BitSet
 * summary:
 *   Set a bit in a bit array
 * error:
 *   on out of range  error, return -1
 *   on success, return value set
 */
extern int                /* RET - status info          */
  BitSet(BitArray *b,     /* IN  - bit array            */
         int       pos,   /* IN  - index into bit array */
         int       val);  /* IN  - value to set bit     */

/*
 * name:
 *   BitSetAll
 * summary:
 *   Set all the a bits in a bit array to val
 * method:
 *   explicit
 */
extern int                   /* RET - status info          */
  BitSetAll(BitArray *b,     /* IN  - bit array            */
            int       val);  /* IN  - value to set bits    */

/*
 * name:
 *   BitSetInt
 * summary:
 *   Set bit array to binary representation of val
 */
extern int                   /* RET - status info          */
  BitSetInt(BitArray *b,     /* IN  - bit array            */
            int       val);  /* IN  - value to set array   */

/*
 * name:
 *   BitSetString
 * summary:
 *   Set bit array to binary value given in val (eg "1010")
 */
extern int                      /* RET - status info          */
  BitSetString(BitArray *b,     /* IN  - bit array            */
               char *    val);  /* IN  - value to set array   */

/*
 * name:
 *   BitGet
 * summary:
 *   get value of a bit in a bit array
 * error:
 *   on out of range  error, return -1
 */
extern int               /* RET - value of bit         */
  BitGet(BitArray *b,    /* IN  - bit array            */
         int      pos);  /* IN  - index into bit array */

/*
 * name:
 *   BitEquals
 * summary:
 *   test for equality between two bit arrays.
 * note:
 *   the shorter is assumed to be padded with 0's
 */
extern int                                  /* RET - TRUE or FALSE  */
  BitEquals( BitArray *b1, BitArray *b2 );  /* IN  - two bit arrays */

/*
 * name:
 *   BitVal
 * summary:
 *   calculate value of bit array as a long integer
 * method:
 */
extern unsigned long      /* RET - value of bit array as integer */
  BitVal( BitArray *b );  /* IN  - bit array                     */

/*
 * name:
 *   BitComp
 * summary:
 *   compare bit arrays :
 *     b1 > b2 : return  1
 *     b1 < b2 : return -1
 *     b1 = b2 : return  0
 */
extern int
  BitComp( BitArray *b1, BitArray *b2 ); /* IN   - two bit arrays */

/*
 * name:
 *   BitAND
 * summary:
 *   create the AND of two bit arrays
 * note:
 *   the result is the length of the shorter
 */
extern BitArray *                       /* RET  - the AND        */
  BitAND( BitArray *b1, BitArray *b2 ); /* IN   - two bit arrays */

/*
 * name:
 *   BitOR
 * summary:
 *   create the OR of two bit arrays
 * note:
 *   the result is the length of the shorter
 */
extern BitArray *                        /* RET  - the AND        */
  BitOR( BitArray *b1, BitArray *b2 );   /* IN   - two bit arrays */

/*
 * name:
 *   BitNOT
 * summary:
 *   Create the NOT of a bit array
 */
extern BitArray *        /* RET - the NOT     */
  BitNOT( BitArray *b ); /* IN  - a bit array */

/*
 * name:
 *   BitCount
 * summary:
 *   count number of bits set in b
 */
extern int                 /* RET - number of bits set */
  BitCount( BitArray *b ); /* IN  - a bit array */

/*
 * name:
 *   BitClone
 * summary:
 *   create a copy of a bit array
 */
extern BitArray *          /* RET - the copy    */
  BitClone( BitArray *b ); /* IN  - a bit array */

/*
 * name:
 *   BitCopy
 * summary:
 *   copy b2 into b1
 * pre:
 *   b1 is big enough
 * post:
 *   b1 = b2 padded with 0 if necessary
 */
extern BitArray *          /* RET - b1 (the copy)          */
  BitCopy( BitArray *b1,   /* IN  - bit array to copy to   */
           BitArray *b2 ); /* IN  - bit array to copy from */

/*
 * name:
 *   BitString
 * summary:
 *   create a string representation of the bit array, e.g. "1001"
 * note:
 *   Your responsibility to free the string
 */
extern char *                /* RET - string representation */
  BitString( BitArray *b );  /* IN  - bit array             */

/*
 * name:
 *   BitPrint
 * summary:
 *   print a bit array to standard output
 */
extern void
  BitPrint( BitArray *b );  /* IN  - a bit array */

#endif
