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
 
 SccsId : @(#)bits.c	1.2 09/26/96
 
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

#include <stdio.h>
#include <stdlib.h>

/* #include <time.h> */

#include <string.h>

#include "bits.h"

#include "MemoryUtil.h"

static BIN_TYPE BIT[BITS_N] = { 1, 2, 4, 8, 16, 32, 64, 128 };

/**************************************************************************/

/*
 * name:
 *   BitCreate
 * summary:
 *   create an array of n bits
 * method:
 *   calculate number of bytes needed
 *   malloc them
 *   set them all to zero
 */
BitArray *          /* RET - the new bit array */
  BitCreate( int n )  /* IN  - number of bits    */
{
/*   BitArray *b = (BitArray *)malloc( sizeof(BitArray) ); */
  BitArray *b;
BIN_TYPE *a;
  int size = BITS_BYTES( n );

/*   if (b == NULL)  */
  if ( !fNewMemory( (void *)&b, sizeof( BitArray ) ) ) 
  {
    printf("\n*** BitCreate (1) : malloc failed ***\n");
    exit(1);
  }

  if (size > 0) 
  {
/*     if ( (b->a = (BIN_TYPE *)malloc( size * sizeof(BIN_TYPE) )) == NULL ) */
    if ( !fNewMemory( (void *)&(a), ( size * sizeof( BIN_TYPE ) ) ) ) 
    {
      printf("\n*** BitCreate (2) : malloc failed ***\n");
      exit(1);
    }
    b->a = a;
  }

  b->n = n;
  while( --size >= 0 ) 
  {
    (b->a)[size] = 0;
  }

  return b;
}

/**************************************************************************/

/*
 * name:
 *   BitDestroy
 * summary:
 *   free all memory associated with bit array b
 * method:
 *   explicit
 */
void
  BitDestroy( BitArray *b )   /* IN  - bit array to destroy */
{
    FreeMemory(b->a);
    FreeMemory(b);
}

/**************************************************************************/

/*
 * name:
 *   BitSet
 * summary:
 *   Set a bit in a bit array
 * method:
 *   explicit
 */
int                     /* RET - status info          */
  BitSet(BitArray *b,     /* IN  - bit array            */
       int       pos,   /* IN  - index into bit array */
       int       val)   /* IN  - value to set bit     */
{
    if (pos < 0 || pos > b->n - 1) {
        printf("BitSet: pos = %d\n", pos);
        return -1;
    }

    if ( val != 0 ) {
        (b->a)[pos / BITS_N] |= BIT[ pos % BITS_N ];    /* set bit = 1 */
        return 1;
    } else {
        (b->a)[pos / BITS_N] &= ~BIT[ pos % BITS_N ];   /* set bit = 0 */
        return 0;
    }
}

/**************************************************************************/

/*
 * name:
 *   BitSetAll
 * summary:
 *   Set all the a bits in a bit array to val
 * method:
 *   explicit
 */
int                        /* RET - status info          */
  BitSetAll(BitArray *b,     /* IN  - bit array            */
            int       val)   /* IN  - value to set bits    */
{
    int i = BITS_BYTES(b->n);

    while (--i >= 0) {
        (b->a)[i] = (val == 0 ? 0 : BITS_ALL_SET);
    }
    return 1;
}

/**************************************************************************/

/*
 * name:
 *   pow2
 * summary:
 *   calculate powers of 2
 * method:
 *   explicit
 */
static int      /* RET - nth power of 2 */
  pow2( int n )   /* IN  - n              */
{
    int x = 1;
    while (--n >= 0) {
        x = x * 2;
    }
    return x;
}

/**************************************************************************/

/*
 * name:
 *   BitSetInt
 * summary:
 *   Set bit array to binary representation of val
 * method:
 *   explicit
 */
int                        /* RET - status info          */
  BitSetInt(BitArray *b,     /* IN  - bit array            */
            int       val)   /* IN  - value to set array   */
{
    int i;

    BitSetAll(b, 0);

    if ( val > pow2(b->n) - 1 ) return -1;

    for(i = 0; val > 0; val /= 2, i++) {
        if ( val % 2 ) {
            BitSet(b, i, 1);
        }
    }
    return 1;
}

/**************************************************************************/

/*
 * name:
 *   BitSetString
 * summary:
 *   Set bit array to binary value given in val (eg "1010")
 * method:
 *   explicit
 */
int                           /* RET - status info          */
  BitSetString(BitArray *b,     /* IN  - bit array            */
               char *    val)   /* IN  - value to set array   */
{
    int i = 0, n = strlen(val);

    BitSetAll(b, 0);

    if (n > b->n) return -1;

    while( --n >= 0 ) {
        if( val[n] == '1' ) {
            BitSet(b, i++, 1);
        } else if ( val[n] == '0' ) {
            i++;
        } else {
            BitSetAll(b, 0);
            return -1;
        }
    }
    return 1;
}

/**************************************************************************/

/*
 * name:
 *   BitGet
 * summary:
 *   get value of a bit in a bit array
 * method:
 *   explicit
 */
int                    /* RET - value of bit         */
  BitGet(BitArray *b,    /* IN  - bit array            */
         int      pos)   /* IN  - index into bit array */
{
    if (pos > b->n - 1) {
        return -1;
    } else {
        return ( ((b->a)[pos / BITS_N] & BIT[ pos % BITS_N ]) != 0 );
    }
}

/**************************************************************************/

/*
 * name:
 *   BitEquals
 * summary:
 *   test for equality between two bit arrays.
 * method:
 *   the shorter is assumed to be padded with 0's
 */
int                                       /* RET - TRUE or FALSE  */
  BitEquals( BitArray *b1, BitArray *b2 )   /* IN  - two bit arrays */
{
    int s1 = BITS_BYTES(b1->n);
    int s2 = BITS_BYTES(b2->n);
    int i = 0;

    if ( s1 > s2 ) return BitEquals(b2, b1);

    while( i < s1 ) {
        if ( (b1->a)[i] != (b2->a)[i] ) return 0;
        i++;
    }

    while( i < s2 ) {
        if ( (b2->a)[i] != 0) return 0;
        i++;
    }

    return 1;
}

/**************************************************************************/

/*
 * name:
 *   BitVal
 * summary:
 *   calculate value of bit array as a long integer
 * method:
 */
unsigned long           /* RET - value of bit array as integer */
  BitVal( BitArray *b )   /* IN  - bit array                     */
{
    unsigned long val = 0;
    int i = BITS_BYTES(b->n);

    if ( b->n > BITS_LONG ) {
        printf("*** BitVal : overflow\n");
    }

    while (--i >= 0) {
        val = val * BITS_BYTE_VAL + (int)(b->a)[i];
    }

    return val;
}

/**************************************************************************/

/*
 * name:
 *   BitComp
 * summary:
 *   compare bit arrays :
 *     b1 > b2 : return  1
 *     b1 < b2 : return -1
 *     b1 = b2 : return  0
 * method:
 *   find highest non-zero bytes in b1, b2
 *   if different, this tells you the answer.
 *   if the same, compare corresponding bytes,
 *   starting with the most significant. As soon
 *   you find unequal bytes, you know which number is bigger.
 */
int
  BitComp( BitArray *b1, BitArray *b2 )  /* IN   - two bit arrays */
#if 1
{
    int i1, i2;
 
 /* find highest non-zero byte in b1 */
    for( i1 = BITS_BYTES(b1->n) - 1; i1 >= 0 && b1->a[i1] == 0; i1-- )
        ;

 /* find highest non-zero byte in b2 */
    for( i2 = BITS_BYTES(b2->n) - 1; i2 >= 0 && b2->a[i2] == 0; i2-- )
        ;

 /* compare */
    if (i1 > i2) {
        return 1;
    } else if (i1 < i2) {
        return -1;
    }

 /* i1 == i2 : compare bytes in descending order */
    while (i1 >= 0) {
        if ( (b1->a)[i1] > (b2->a)[i1] ) {
            return 1;
        } else if ( (b1->a)[i1] < (b2->a)[i1] ) {
            return -1;
        }
        i1--;
    }

 /* all bytes the same */
    return 0;
}

#else
/*
 *  old method - only works for up to 32 bits !
 */
{
printf("BitComp old method\n");
    int i1, i2;

    if ( (i1 = BitVal(b1)) > (i2 = BitVal(b2)) ) {
        return 1;
    } else if ( i1 < i2 ) {
        return -1;
    } else {
        return 0;
    }
}
#endif

/**************************************************************************/

/*
 * name:
 *   BitAND
 * summary:
 *   create the AND of two bit arrays
 * method:
 *   the result is the length of the shorter
 */
BitArray *                            /* RET  - the AND        */
  BitAND( BitArray *b1, BitArray *b2 )  /* IN   - two bit arrays */
{
    BitArray *b;
    int i;

    if ( b1->n > b2->n ) return BitAND(b2, b1);

    b = BitCreate(b1->n);
    i = BITS_BYTES(b1->n);

    while(--i >= 0 ) {
        (b->a)[i] = (b1->a)[i] & (b2->a)[i];
    }

    return b;
}

/**************************************************************************/

/*
 * name:
 *   BitOR
 * summary:
 *   create the OR of two bit arrays
 * method:
 *   the result is the length of the shorter
 */
BitArray *                             /* RET  - the AND        */
  BitOR( BitArray *b1, BitArray *b2 )    /* IN   - two bit arrays */
{
    BitArray *b;
    int i;

    if ( b1->n > b2->n ) return BitOR(b2, b1);

    b = BitCreate(b1->n);
    i = BITS_BYTES(b1->n);

    while(--i >= 0 ) {
        (b->a)[i] = (b1->a)[i] | (b2->a)[i];
    }

    return b;
}

/**************************************************************************/

/*
 * name:
 *   BitNOT
 * summary:
 *   Create the NOT of a bit array
 * method:
 *   explicit
 */
BitArray *             /* RET - the NOT     */
  BitNOT( BitArray *b )  /* IN  - a bit array */
{
    BitArray *nb;
    int i;

    nb = BitCreate(b->n);
    i = BITS_BYTES(b->n);

    while(--i >= 0 ) {
        (nb->a)[i] = ~(b->a)[i];
    }

    return nb;
}

/**************************************************************************/

/*
 * name:
 *   BitCount
 * summary:
 *   count number of bits set in b
 * method:
 *   explicit
 */
int                      /* RET - number of bits set */
  BitCount( BitArray *b )  /* IN  - a bit array */
{
    int i = 0, n = 0;

    while (i < b->n) {
        if  ( BitGet(b, i++) ) {
            n++;
        }
    }
    return n;
}

/**************************************************************************/

/*
 * name:
 *   BitClone
 * summary:
 *   create a copy of a bit array
 * method:
 *   explicit
 */
BitArray *               /* RET - the copy    */
  BitClone( BitArray *b )  /* IN  - a bit array */
{
    BitArray *nb;
    int i;

    nb = BitCreate(b->n);
    i = BITS_BYTES(b->n);

    while(--i >= 0 ) {
        (nb->a)[i] = (b->a)[i];
    }

    return nb;
}

/**************************************************************************/

/*
 * name:
 *   BitCopy
 * summary:
 *   copy b2 into b1
 * method:
 *   explicit
 */
BitArray *               /* RET - b1 (the copy)          */
  BitCopy( BitArray *b1,   /* IN  - bit array to copy to   */
           BitArray *b2 )  /* IN  - bit array to copy from */
{
    int s1 = BITS_BYTES(b1->n);
    int s2 = BITS_BYTES(b2->n);
    int i = 0;

    while( i < s1 ) {
        (b1->a)[i] = (b2->a)[i];
        i++;
    }

    while( i < s2 ) {
        (b1->a)[i] = 0;
        i++;
    }

    return b2;
}

/**************************************************************************/

/*
 * name:
 *   BitString
 * summary:
 *   create a string representation of the bit array, e.g. "1001"
 * method:
 *   the zeroth byte is the least significant and goes at the right
 */
char *                     /* RET - string representation */
  BitString( BitArray *b )   /* IN  - bit array             */
{
/*   char *bs = (char *)malloc(b->n + 1); */
  char *bs;
  int i;

/*   if (bs == NULL)  */
  if ( !fNewMemory( (void *)&bs, ( b->n + 1 ) ) ) 
  {
    printf("\n*** BitString: malloc failed ***\n");
    exit(1);
  }

  for (i = 0; i < b->n; i++) 
  {
    bs[b->n - 1 - i] = BitGet(b, i) ? '1' : '0';
  }
  bs[i] = '\0';
  return bs;
}


/**************************************************************************/

#if 0
/*
 * name:
 *   BitFromString
 * summary:
 *   create a bit array from a string, e.g. "1001"
 * method:
 *   the zeroth byte is the least significant and goes at the right
 * note:
 *   NOT TESTED - BitSetString used instead
 */
BitArray *                 /* RET - bit array             */
  BitFromString( char *s )   /* IN  - string representation */
{
    int len = strlen(s);
    BitArray *b = BitCreate(len);
    int i = 0;

    while (*s != '\0') {
        BitSet(b, i++, (*s++ == '1' ? 1 : 0) );
    } 
}
#endif

/**************************************************************************/

/*
 * name:
 *   BitPrint
 * summary:
 *   print a bit array to standard output
 * method:
 *   explicit
 */
void
  BitPrint( BitArray *b )   /* IN  - a bit array */
{
    char *bs = BitString(b);
    printf("%s\n", bs );
    FreeMemory(bs);
}

#ifdef TEST
/*
 *    MAIN program to test the module
 */
main()
{
    BitArray *b1 = BitCreate(10);
    BitArray *b2 = BitCreate(10);
    BitArray *b3, *b4;

    BitSet(b1, 0, 1);
    BitSet(b1, 1, 1);
    BitSet(b1, 2, 1);
    BitSet(b1, 3, 1);
    BitSet(b1, 4, 1);

    BitSet(b2, 0, 1);
    BitSet(b2, 2, 1);
    BitSet(b2, 4, 1);
    BitSet(b2, 6, 1);
    BitSet(b2, 8, 1);

    BitPrint(b1);
    BitPrint(b2);

    b3 = BitAND(b1, b2);
    BitPrint(b3);

    b3 = BitOR(b1, b2);
    BitPrint(b3);

    b4 = BitClone(b1);
    BitPrint(b4);

    BitCopy(b4, b3);
    BitPrint(b4);

    printf("%d %d\n", BitEquals(b1, b2), BitEquals(b3, b4) );

    b4 = BitNOT(b3);
    BitPrint(b4);

    BitSetAll(b4, 1);
    BitPrint(b4);

    BitSetAll(b4, 0);
    BitPrint(b4);

    printf("%d %d %d %d\n", BitVal(b1), BitVal(b2), BitVal(b3), BitVal(b4));

    BitSetInt(b4, 100);
    BitPrint(b4);

    BitSetString(b4, "1001010");
    BitPrint(b4);

    BitDestroy(b1);
    BitDestroy(b2);
    BitDestroy(b3);
    BitDestroy(b4);
}
#endif
