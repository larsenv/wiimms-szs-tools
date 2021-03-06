
/***************************************************************************
 *                                                                         *
 *                     _____     ____                                      *
 *                    |  __ \   / __ \   _     _ _____                     *
 *                    | |  \ \ / /  \_\ | |   | |  _  \                    *
 *                    | |   \ \| |      | |   | | |_| |                    *
 *                    | |   | || |      | |   | |  ___/                    *
 *                    | |   / /| |   __ | |   | |  _  \                    *
 *                    | |__/ / \ \__/ / | |___| | |_| |                    *
 *                    |_____/   \____/  |_____|_|_____/                    *
 *                                                                         *
 *                       Wiimms source code library                        *
 *                                                                         *
 ***************************************************************************
 *                                                                         *
 *        Copyright (c) 2012-2020 by Dirk Clemens <wiimm@wiimm.de>         *
 *                                                                         *
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   See file gpl-2.0.txt or http://www.gnu.org/licenses/gpl-2.0.txt       *
 *                                                                         *
 ***************************************************************************/

#define _GNU_SOURCE 1

#include <string.h>
#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <errno.h>
#include <sys/time.h>
#include <fcntl.h>

#include "dclib-basics.h"
#include "dclib-debug.h"
#include "dclib-utf8.h"

//
///////////////////////////////////////////////////////////////////////////////
///////////////				bits			///////////////
///////////////////////////////////////////////////////////////////////////////

uint Count1Bits ( cvp addr, uint size )
{
    DASSERT( CHAR_BIT == 8 );

    uint count = 0;
    const uchar *ptr = addr;
    while ( size-- > 0 )
	count += TableBitCount[*ptr++];
    return count;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

int FindLowest0BitLE ( cvp addr, uint size )
{
    DASSERT( CHAR_BIT == 8 );

    const uchar *ptr = addr;
    uint idx;
    for ( idx = 0; idx < size; idx++ )
    {
	const int val = TableLowest0Bit[*ptr++];
	if ( val >= 0 )
	    return 8*idx + val;
    }
    return -1;
}

///////////////////////////////////////////////////////////////////////////////

int FindLowest1BitLE ( cvp addr, uint size )
{
    DASSERT( CHAR_BIT == 8 );

    const uchar *ptr = addr;
    uint idx;
    for ( idx = 0; idx < size; idx++ )
    {
	const int val = TableLowest1Bit[*ptr++];
	if ( val >= 0 )
	    return 8*idx + val;
    }
    return -1;
}

///////////////////////////////////////////////////////////////////////////////

int FindHighest0BitLE ( cvp addr, uint size )
{
    DASSERT( CHAR_BIT == 8 );

    const uchar *ptr = addr + size;
    while ( size-- > 0 )
    {
	const int val = TableHighest0Bit[*--ptr];
	if ( val >= 0 )
	    return 8*size + val;
    }
    return -1;
}

///////////////////////////////////////////////////////////////////////////////

int FindHighest1BitLE ( cvp addr, uint size )
{
    DASSERT( CHAR_BIT == 8 );

    const uchar *ptr = addr + size;
    while ( size-- > 0 )
    {
	const int val = TableHighest1Bit[*--ptr];
	if ( val >= 0 )
	    return 8*size + val;
    }
    return -1;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

u32 GetAlign32 ( u32 data )
{
    const int index = FindLowest1Bit(&data,sizeof(data));
    return index < 0 ? 0 : 1 << index;
}

///////////////////////////////////////////////////////////////////////////////

u64 GetAlign64 ( u64 data )
{
    const int index = FindLowest1Bit(&data,sizeof(data));
    return index < 0 ? 0 : 1llu << index;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

int FindLowest0BitBE ( cvp addr, uint size )
{
    DASSERT( CHAR_BIT == 8 );

    const uchar *ptr = addr + size;
    uint idx;
    for ( idx = 0; idx < size; idx++ )
    {
	const int val = TableLowest0Bit[*--ptr];
	if ( val >= 0 )
	    return 8*idx + val;
    }
    return -1;
}

///////////////////////////////////////////////////////////////////////////////

int FindLowest1BitBE ( cvp addr, uint size )
{
    DASSERT( CHAR_BIT == 8 );

    const uchar *ptr = addr + size;
    uint idx;
    for ( idx = 0; idx < size; idx++ )
    {
	const int val = TableLowest1Bit[*--ptr];
	if ( val >= 0 )
	    return 8*idx + val;
    }
    return -1;
}

///////////////////////////////////////////////////////////////////////////////

int FindHighest0BitBE ( cvp addr, uint size )
{
    DASSERT( CHAR_BIT == 8 );

    const uchar *ptr = addr;
    while ( size-- > 0 )
    {
	const int val = TableHighest0Bit[*ptr++];
	if ( val >= 0 )
	    return 8*size + val;
    }
    return -1;
}

///////////////////////////////////////////////////////////////////////////////

int FindHighest1BitBE ( cvp addr, uint size )
{
    DASSERT( CHAR_BIT == 8 );

    const uchar *ptr = addr;
    while ( size-- > 0 )
    {
	const int val = TableHighest1Bit[*ptr++];
	if ( val >= 0 )
	    return 8*size + val;
    }
    return -1;
}

//
///////////////////////////////////////////////////////////////////////////////
///////////////			bit manipulation		///////////////
///////////////////////////////////////////////////////////////////////////////

bool TestSetBit ( void *addr, uint bit_num )
{
    const u8 mask = 1 << ( bit_num & 7 );
    u8 *ptr = ((u8*)addr) + ( bit_num >> 3 );
    const bool result = ( *ptr & mask ) != 0;
    *ptr |= mask;
    return result;
}

///////////////////////////////////////////////////////////////////////////////

bool TestClearBit  ( void *addr, uint bit_num )
{
    const u8 mask = 1 << ( bit_num & 7 );
    u8 *ptr = ((u8*)addr) + ( bit_num >> 3 );
    const bool result = ( *ptr & mask ) != 0;
    *ptr &= ~mask;
    return result;
}

///////////////////////////////////////////////////////////////////////////////

bool TestInvertBit ( void *addr, uint bit_num )
{
    const u8 mask = 1 << ( bit_num & 7 );
    u8 *ptr = ((u8*)addr) + ( bit_num >> 3 );
    const bool result = ( *ptr & mask ) != 0;
    *ptr ^= mask;
    return result;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void SetBits ( void *addr, uint beg_bitnum, uint end_bitnum )
{
    static uchar tab1[] = { 0xff, 0xfe, 0xfc, 0xf8, 0xf0, 0xe0, 0xc0, 0x80 };
    static uchar tab2[] = { 0x01, 0x03, 0x07, 0x0f, 0x1f, 0x3f, 0x7f, 0xff };

    if ( beg_bitnum < end_bitnum )
    {
	const uint lo8 =   beg_bitnum >> 3;
	const uint hi8 = --end_bitnum >> 3;

	if ( lo8 < hi8 )
	{

	    uchar * dest = (uchar*)addr + lo8;
	    *dest++ |= tab1[ beg_bitnum & 7 ];

	    uchar * end = (uchar*)addr + hi8;
	    *end |= tab2[ end_bitnum & 7 ];

	    while ( dest < end )
		*dest++ = 0xff;
	}
	else
	{
	    uchar * dest = (uchar*)addr + lo8;
	    *dest |= tab1[ beg_bitnum & 7 ] & tab2[ end_bitnum & 7 ];
	}
    }
}

///////////////////////////////////////////////////////////////////////////////

void ClearBits ( void *addr, uint beg_bitnum, uint end_bitnum )
{
    static uchar tab1[] = { 0x00, 0x01, 0x03, 0x07, 0x0f, 0x1f, 0x3f, 0x7f };
    static uchar tab2[] = { 0xfe, 0xfc, 0xf8, 0xf0, 0xe0, 0xc0, 0x80, 0x00 };

    if ( beg_bitnum < end_bitnum )
    {
	const uint lo8 =   beg_bitnum >> 3;
	const uint hi8 = --end_bitnum  >> 3;

	if ( lo8 < hi8 )
	{

	    uchar * dest = (uchar*)addr + lo8;
	    *dest++ &= tab1[ beg_bitnum & 7 ];

	    uchar * end = (uchar*)addr + hi8;
	    *end &= tab2[ end_bitnum & 7 ];

	    while ( dest < end )
		*dest++ = 0;
	}
	else
	{
	    uchar * dest = (uchar*)addr + lo8;
	    *dest &= tab1[ beg_bitnum & 7 ] | tab2[ end_bitnum & 7 ];
	}
    }
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void InvertBits ( void *addr, uint beg_bitnum, uint end_bitnum )
{
    static uchar tab1[] = { 0xff, 0xfe, 0xfc, 0xf8, 0xf0, 0xe0, 0xc0, 0x80 };
    static uchar tab2[] = { 0x01, 0x03, 0x07, 0x0f, 0x1f, 0x3f, 0x7f, 0xff };

    if ( beg_bitnum < end_bitnum )
    {
	const uint lo8 =   beg_bitnum >> 3;
	const uint hi8 = --end_bitnum >> 3;

	if ( lo8 < hi8 )
	{

	    uchar * dest = (uchar*)addr + lo8;
	    *dest++ ^= tab1[ beg_bitnum & 7 ];

	    uchar * end = (uchar*)addr + hi8;
	    *end ^= tab2[ end_bitnum & 7 ];

	    while ( dest < end )
		*dest++ ^= 0xff;
	}
	else
	{
	    uchar * dest = (uchar*)addr + lo8;
	    *dest ^= tab1[ beg_bitnum & 7 ] & tab2[ end_bitnum & 7 ];
	}
    }
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#undef GET_NEXT
#define GET_NEXT ch = *src++; \
		if ( ++min_idx > max_idx ) goto term; \
		if ( min_idx == max_idx ) ch &= max_mask

int PrintBitfieldEx
(
    // returns the number of printed characters, or -1 if buffer is to small

    char	*buf,		// buffer
    uint	buf_size,	// size of 'buf'
    cvp		bit_field,	// address of bit field
    uint	min_idx,	// minimal index (inclusive) to check
    uint	max_idx,	// maximal index (inclusive) to check
    bool	use_ranges,	// true: create ranges 'a:b'
    int		mult,		// each printed index is first multiplyed by 'mult'
    int		add		// ... and then added to 'add'.
)
{
    DASSERT(buf);
    DASSERT(buf_size);
    DASSERT(bit_field);

    char *dest = buf;
    char *end  = buf + buf_size;
    char *sep = "";

    const u8 *src = (u8*)bit_field + (min_idx >> 3);
    u8 ch = *src++; // always preload the current byte
    if ( min_idx & 7 )
	ch &= ~((1<<(min_idx&7))-1);
    min_idx >>= 3;
    const u8 max_mask = (2<<(max_idx&7))-1;
    max_idx >>= 3;
    noPRINT("SETUP[%zx] %02x, %u..%u, max_mask=%02x\n",
	src-(u8*)bit_field-1, ch, min_idx, max_idx, max_mask );

    while ( min_idx <= max_idx )
    {
	//--- first skip bytes with value 0

	if (!ch)
	    do { GET_NEXT; } while (!ch);


	//-- now find the first set bit

	DASSERT(ch);
	noPRINT("CHECK[%zx] %02x, %u..%u\n", src-(u8*)bit_field-1, ch, min_idx, max_idx);
	u8 mask = 1;
	uint bit_idx = 0;
	while ( !(ch & mask) )
	    mask <<= 1, bit_idx++;

	uint first = 8*min_idx + bit_idx;
	noPRINT("FOUND-BEG[%zx.%u=%u] %02x, %u..%u\n",
		src-(u8*)bit_field-1, bit_idx, first, ch, min_idx, max_idx );


	//-- now find the first bit not set

	while ( ch & mask )
	    mask <<= 1, bit_idx++;

	if ( bit_idx == 8 )
	{
	    do { GET_NEXT; } while (ch == 0xff);

	    mask = 1;
	    bit_idx = 0;
	    while ( ch & mask )
		mask <<= 1, bit_idx++;
	}

	uint last = 8*min_idx + bit_idx - 1;
	noPRINT("FOUND-END[%zx.%u=%u] %02x, %u..%u\n",
		src-(u8*)bit_field-1, bit_idx, last, ch, min_idx, max_idx );

	if ( first == last )
	    dest = snprintfE(dest,end,"%s%d",sep,first*mult+add);
	else if ( use_ranges )
	    dest = snprintfE(dest,end,"%s%d:%d",
			sep,
			first * mult + add,
			last * mult + add );
	else
	    while ( first <= last )
	    {
		dest = snprintfE(dest,end,"%s%d",sep,first++*mult+add);
		sep = ",";
	    }

	if ( dest >= end-1 )
	{
	    *dest = 0;
	    return -1;
	}

	sep = ",";
	ch &= ~((1<<bit_idx)-1);
    }

 term:
    *dest = 0;
    return dest - buf;
}


//
///////////////////////////////////////////////////////////////////////////////
///////////////		low level endian conversions		///////////////
///////////////////////////////////////////////////////////////////////////////

void write_be16 ( void * be_data_ptr, u16 data )
{
    DASSERT(be_data_ptr);
    u8 * dest = be_data_ptr;
    *dest++ = data >> 8;
    *dest   = data;
}

void write_be24 ( void * be_data_ptr, u32 data )
{
    DASSERT(be_data_ptr);
    u8 * dest = be_data_ptr + 2;
    *dest-- = data; data >>= 8;
    *dest-- = data;
    *dest   = data >> 8;
}

void write_be32 ( void * be_data_ptr, u32 data )
{
    DASSERT(be_data_ptr);
    u8 * dest = be_data_ptr + 3;
    *dest-- = data; data >>= 8;
    *dest-- = data; data >>= 8;
    *dest-- = data;
    *dest   = data >> 8;
}

void write_be40 ( void * be_data_ptr, u64 data )
{
    DASSERT(be_data_ptr);
    *(u8*)be_data_ptr = data>>32;
    write_be32( (u8*)be_data_ptr+1, data );
}

void write_be48 ( void * be_data_ptr, u64 data )
{
    DASSERT(be_data_ptr);
    write_be16( be_data_ptr, data>>32 );
    write_be32( (u8*)be_data_ptr+2, data );
}

void write_be56 ( void * be_data_ptr, u64 data )
{
    DASSERT(be_data_ptr);
    write_be24( be_data_ptr, data>>32 );
    write_be32( (u8*)be_data_ptr+3, data );
}

void write_be64 ( void * be_data_ptr, u64 data )
{
    DASSERT(be_data_ptr);
    write_be32( be_data_ptr, data >> 32 );
    write_be32( (u8*)be_data_ptr+4, data );
}

void write_bef4 ( void * be_data_ptr, float data )
{
    // assume: local system supports IEEE 754

    write_be32(be_data_ptr,*(u32*)&data);
}

void write_bef8 ( void * be_data_ptr, double data )
{
    // assume: local system supports IEEE 754

    write_be64(be_data_ptr,*(u64*)&data);
}

///////////////////////////////////////////////////////////////////////////////

void write_le16 ( void * le_data_ptr, u16 data )
{
    DASSERT(le_data_ptr);
    u8 * dest = le_data_ptr;
    *dest++ = data;
    *dest   = data >> 8;
}

void write_le24 ( void * le_data_ptr, u32 data )
{
    DASSERT(le_data_ptr);
    u8 * dest = le_data_ptr;
    *dest++ = data; data >>= 8;
    *dest++ = data;
    *dest   = data >> 8;
}

void write_le32 ( void * le_data_ptr, u32 data )
{
    DASSERT(le_data_ptr);
    u8 * dest = le_data_ptr;
    *dest++ = data; data >>= 8;
    *dest++ = data; data >>= 8;
    *dest++ = data;
    *dest   = data >> 8;
}

void write_le40 ( void * le_data_ptr, u64 data )
{
    DASSERT(le_data_ptr);
    *(u8*)le_data_ptr = (u8)data;
    write_le32( (u8*)le_data_ptr+1, data >> 8 );
}

void write_le48 ( void * le_data_ptr, u64 data )
{
    DASSERT(le_data_ptr);
    write_le16( le_data_ptr, data );
    write_le32( (u8*)le_data_ptr+2, data >> 16 );
}

void write_le56 ( void * le_data_ptr, u64 data )
{
    DASSERT(le_data_ptr);
    write_le32( le_data_ptr, data );
    write_le24( (u8*)le_data_ptr+4, data >> 32 );
}

void write_le64 ( void * le_data_ptr, u64 data )
{
    DASSERT(le_data_ptr);
    write_le32( le_data_ptr, data );
    write_le32( (u8*)le_data_ptr+4, data >> 32 );
}

void write_lef4 ( void * le_data_ptr, float data )
{
    // assume: local system supports IEEE 754

    write_le32(le_data_ptr,*(u32*)&data);
}

void write_lef8 ( void * le_data_ptr, double data )
{
    // assume: local system supports IEEE 754

    write_le64(le_data_ptr,*(u64*)&data);
}

///////////////////////////////////////////////////////////////////////////////
// convert lists

void be16n ( u16 * dest, const u16 * src, int n )
{
    DASSERT( dest );
    DASSERT( n >= 0 );
    DASSERT( src || !n );

    while ( n-- > 0 )
	*dest++ = be16(src++);
}

void be32n ( u32 * dest, const u32 * src, int n )
{
    DASSERT( dest );
    DASSERT( n >= 0 );
    DASSERT( src || !n );

    while ( n-- > 0 )
	*dest++ = be32(src++);
}

void bef4n ( float32 * dest, const float32 * src, int n )
{
    DASSERT( dest );
    DASSERT( n >= 0 );
    DASSERT( src || !n );

    while ( n-- > 0 )
	*dest++ = bef4(src++);
}

void write_be16n ( u16 * dest, const u16 * src, int n )
{
    DASSERT( dest );
    DASSERT( n >= 0 );
    DASSERT( src || !n );

    while ( n-- > 0 )
	write_be16(dest++,*src++);
}

void write_be32n ( u32 * dest, const u32 * src, int n )
{
    DASSERT( dest );
    DASSERT( n >= 0 );
    DASSERT( src || !n );

    while ( n-- > 0 )
	write_be32(dest++,*src++);
}

void write_bef4n ( float32 * dest, const float32 * src, int n )
{
    DASSERT( dest );
    DASSERT( n >= 0 );
    DASSERT( src || !n );

    while ( n-- > 0 )
	write_bef4(dest++,*src++);
}

///////////////////////////////////////////////////////////////////////////////
// convert lists

void le16n ( u16 * dest, const u16 * src, int n )
{
    DASSERT( dest );
    DASSERT( n >= 0 );
    DASSERT( src || !n );

    while ( n-- > 0 )
	*dest++ = le16(src++);
}

void lef4n ( float32 * dest, const float32 * src, int n )
{
    DASSERT( dest );
    DASSERT( n >= 0 );
    DASSERT( src || !n );

    while ( n-- > 0 )
	*dest++ = lef4(src++);
}

void write_le16n ( u16 * dest, const u16 * src, int n )
{
    DASSERT( dest );
    DASSERT( n >= 0 );
    DASSERT( src || !n );

    while ( n-- > 0 )
	write_le16(dest++,*src++);
}

void write_lef4n ( float32 * dest, const float32 * src, int n )
{
    DASSERT( dest );
    DASSERT( n >= 0 );
    DASSERT( src || !n );

    while ( n-- > 0 )
	write_lef4(dest++,*src++);
}

///////////////////////////////////////////////////////////////////////////////

const endian_func_t be_func =
{
    {0xfe,0xff}, true, false, DC_BIG_ENDIAN,
    be16, be24, be32, be40, be48, be56, be64, bef4, bef8,
    write_be16, write_be24, write_be32,
    write_be40, write_be48, write_be56, write_be64,
    write_bef4, write_bef8
};

const endian_func_t le_func =
{
    {0xff,0xfe}, false, true, DC_LITTLE_ENDIAN,
    le16, le24, le32, le40, le48, le56, le64, lef4, lef8,
    write_le16, write_le24, write_le32,
    write_le40, write_le48, write_le56, write_le64,
    write_lef4, write_lef8
};

const endian_func_t * GetEndianFunc ( const void * byte_order_mark )
{
    DASSERT(byte_order_mark);
    const u8 * bom = byte_order_mark;
    return bom[0] == 0xfe && bom[1] == 0xff
		? &be_func
		: bom[0] == 0xff && bom[1] == 0xfe
			? &le_func
			: 0;
}

///////////////////////////////////////////////////////////////////////////////

uint GetTextBOMLen ( const void * data, uint data_size )
{
    static const u8 be_bom[] = { 0xef, 0xbb, 0xbf };
    static const u8 le_bom[] = { 0xef, 0xbf, 0xbe };

    if ( data_size >= 3 && ( !memcmp(data,be_bom,3) || !memcmp(data,le_bom,3) ))
	return 3;

    if ( data_size >= 2 && ( *(u16*)data == 0xfeff || *(u16*)data == 0xfffe ))
	return 2;

    return 0;
}

//
///////////////////////////////////////////////////////////////////////////////
///////////////			check index			///////////////
///////////////////////////////////////////////////////////////////////////////

int CheckIndex1 ( int max, int index )
{
    DASSERT( max >= 0 );

    if ( index >= 0 )
	return index <= max ? index : max;

    index += max;
    return index >= 0 ? index : 0;
}

///////////////////////////////////////////////////////////////////////////////

int CheckIndex1End ( int max, int index )
{
    DASSERT( max >= 0 );
    if ( index > 0 )
	return index <= max ? index : max;

    index += max;
    return index >= 0 ? index : 0;
}

///////////////////////////////////////////////////////////////////////////////

int CheckIndex2 ( int max, int * p_begin, int * p_end )
{
    DASSERT( max >= 0 );
    DASSERT(p_begin);
    DASSERT(p_end);

    int begin = *p_begin;
    if ( begin < 0 )
    {
	begin += max;
	if ( begin < 0 )
	    begin = 0;
    }
    else if ( begin > max )
	begin = max;

    int end = *p_end;
    if ( end < 0 )
    {
	end += max;
	// don't check for 'end<0' here, because 'end<begin' is checked later
    }
    else if ( end > max )
	end = max;

    if ( end < begin )
	end = begin;

    *p_begin = begin;
    *p_end   = end;
    return end - begin;
}

///////////////////////////////////////////////////////////////////////////////

int CheckIndex2End ( int max, int * p_begin, int * p_end )
{
    DASSERT( max >= 0 );
    DASSERT(p_begin);
    DASSERT(p_end);

    int begin = *p_begin;
    if ( begin <= 0 )
    {
	begin += max;
	if ( begin < 0 )
	    begin = 0;
    }
    else if ( begin > max )
	begin = max;

    int end = *p_end;
    if ( end <= 0 )
    {
	end += max;
	// don't check for 'end<0' here, because 'end<begin' is checked later
    }
    else if ( end > max )
	end = max;

    if ( end < begin )
	end = begin;

    *p_begin = begin;
    *p_end   = end;
    return end - begin;
}

//
///////////////////////////////////////////////////////////////////////////////
///////////////			encoding/decoding		///////////////
///////////////////////////////////////////////////////////////////////////////

ccp GetEncodingName ( EncodeMode_t em )
{
    static const char tab[][7] =
    {
	"OFF",
	"ANSI",
	"UTF8",
	"BASE64",
	"URL64",
	"STAR64",
	"XML64",
	"JSON",
    };

    return (uint)em < sizeof(tab)/sizeof(*tab) ? tab[em] : PrintCircBuf("%d",em);
}

///////////////////////////////////////////////////////////////////////////////

char * PrintEscapedString
(
    // returns 'buf'

    char	*buf,		// valid destination buffer
    uint	buf_size,	// size of 'buf', >= 10
    ccp		source,		// NULL string to print
    int		len,		// length of string. if -1, str is null terminated
    CharMode_t	char_mode,	// modes, bit field (CHMD_*)
    char	quote,		// NULL or quotation char, that must be quoted
    uint	*dest_len	// not NULL: Store length of result here
)
{
    DASSERT(buf);
    DASSERT(buf_size>=10);

    const CharMode_t utf8	= char_mode & CHMD_UTF8;
    const CharMode_t allow_e	= char_mode & CHMD_ESC;

    char *dest = buf;
    char *dest_end = dest + buf_size - 4;

    if (!source)
	source = "";
    ccp str = source;
    ccp end = len < 0 ? 0 : str + len;

    while ( dest < dest_end && ( !end || str < end ) )
    {
	const u8 ch = (u8)*str++;
	switch (ch)
	{
	    case 0:
		if (!end)
		{
		    str--;
		    goto eos;
		}
		*dest++ = '\\';
		*dest++ = 'x';
		*dest++ = '0';
		*dest++ = '0';
		break;

	    case '\\': *dest++ = '\\'; *dest++ = '\\'; break;
	    case '\a': *dest++ = '\\'; *dest++ = 'a'; break;
	    case '\b': *dest++ = '\\'; *dest++ = 'b'; break;
	    case '\f': *dest++ = '\\'; *dest++ = 'f'; break;
	    case '\n': *dest++ = '\\'; *dest++ = 'n'; break;
	    case '\r': *dest++ = '\\'; *dest++ = 'r'; break;
	    case '\t': *dest++ = '\\'; *dest++ = 't'; break;
	    case '\v': *dest++ = '\\'; *dest++ = 'v'; break;

	    case '\033':
		*dest++ = '\\';
		if (allow_e)
		    *dest++ = 'e';
		else
		{
		    *dest++ = 'x';
		    *dest++ = '1';
		    *dest++ = 'B';
		}
		break;

	    default:
		if ( ch == quote )
		{
		    *dest++ = '\\';
		    *dest++ = quote;
		}
		else if ( ch < ' ' || !utf8 && (ch&0x7f) < ' ' || (ch&0x7f) == 0x7f )
		{
		    *dest++ = '\\';
		    *dest++ = 'x';
		    *dest++ = HiDigits[ch>>4];
		    *dest++ = HiDigits[ch&15];
		}
		else
		    *dest++ = ch;
	}
    }
 eos:
    *dest = 0;
    if (dest_len)
	*dest_len = dest - buf;
    return buf;
}

///////////////////////////////////////////////////////////////////////////////

uint ScanEscapedString
(
    // returns the number of valid bytes in 'buf' (NULL term not counted)

    char	*buf,		// valid destination buffer, maybe source
    uint	buf_size,	// size of 'buf'
    ccp		source,		// string to scan
    int		len,		// length of string. if -1, str is null terminated
    bool	utf8,		// true: source and output is UTF-8
    int		quote,		// 0:none, -1:auto, >0: quotation char
    uint	*scanned_len	// not NULL: Store number of scanned 'source' bytes here
)
{
    DASSERT(buf);
    DASSERT(buf_size>3);
    DASSERT(source);

    char *dest = buf;
    char *dest_end = dest + buf_size - ( utf8 ? 4 : 1 );

    ccp src = source;
    ccp src_end = src + ( len < 0 ? strlen(src) : len );

    bool have_quote = quote > 0;
    if ( quote == -1 && src < src_end && ( *src == '"' || *src == '\'' ))
    {
	quote = *src++;
	have_quote = true;
    }

    while ( dest < dest_end && src < src_end )
    {
	uint code;
	if ( *src == '\\' )
	    src = ScanEscape(&code,src+1,src_end);
	else
	{
	    if (utf8)
		code = ScanUTF8AnsiCharE(&src,src_end);
	    else
		code = *src++;

	    if ( code == quote && have_quote )
		break;
	}

	if (utf8)
	    dest = PrintUTF8Char(dest,code);
	else
	    *dest++ = code;
    }

    if (scanned_len)
	*scanned_len = src - source;

    *dest = 0;
    return dest - buf;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

u32 CalcCRC32 ( u32 crc, cvp buf, uint size )
{
 #if HAVE_PRINT0
    static bool done = false;
    if ( !done && TRACE_FILE )
    {
	done = true;
	fprintf(TRACE_FILE,"\nstatic const u32 TableCRC32[0x100] =\n{\n");
	uint i;
	for ( i = 0; i < 0x100; i += 8 )
	    fprintf(TRACE_FILE," %#010x,%#010x,%#010x,%#010x, %#010x,%#010x,%#010x,%#010x,\n",
		TableCRC32[i+0],
		TableCRC32[i+1],
		TableCRC32[i+2],
		TableCRC32[i+3],
		TableCRC32[i+4],
		TableCRC32[i+5],
		TableCRC32[i+6],
		TableCRC32[i+7] );
	fprintf(TRACE_FILE,"};\n");
    }
 #endif

    crc = ~crc;
    const u8 *p = buf;
    while ( size-- )
	crc = TableCRC32[ (crc ^ *p++) & 0xff ] ^ crc >> 8;

    return ~crc;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

u64 DecodeU64
(
    ccp		source,			// string to decode
    int		len,			// length of string. if -1, str is null terminated
    uint	base,			// number of possible digits (2..36):
    uint	*scanned_len		// not NULL: Store number of scanned 'str' bytes here
)
{
    DASSERT(source);
    DASSERT( base <= 36 );

    u64 num = 0;
    ccp src = source;
    ccp end = len < 0 ? 0 : src + len;

    while ( !end || src < end )
    {
	u8 digit = TableNumbers[(u8)*src];
	if ( digit >= base )
	    break;
	num = num * base + digit;
	src++;
    }

    if (scanned_len)
	*scanned_len = src - source;
    return num;
}

///////////////////////////////////////////////////////////////////////////////

ccp EncodeU64
(
    char	* buf,		// result buffer (size depends on base)
				// If NULL, a local circulary static buffer is used
    size_t	buf_size,	// size of 'buf', ignored if buf==NULL
    u64		num,		// number to encaode
    uint	base		// number of possible digits (2..64):
)
{
    DASSERT( base >= 2 );
    DASSERT( base <= 64 );
    if ( base < 2 )
	base = 2;
    else if ( base > 64 )
	base = 64;

    char digits[65], *dest = digits + sizeof(digits);
    *--dest = 0;

    while ( num )
    {
	*--dest = LoDigits[ num % base ];
	num /= base;
    }

    if (!buf)
	buf = GetCircBuf ( buf_size = digits + sizeof(digits)-1 - dest );

    StringCopyS(buf,buf_size,dest);
    return buf;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// Default tables for DecodeBase64() and EncodeBase64(), if no table is defined.

ccp TableDecode64default = TableDecode64;
ccp TableEncode64default = TableEncode64;

///////////////////////////////////////////////////////////////////////////////

uint DecodeBase64
(
    // returns the number of valid bytes in 'buf'

    char	*buf,			// valid destination buffer
    uint	buf_size,		// size of 'buf', >= 3
    ccp		source,			// NULL or string to decode
    int		len,			// length of string. if -1, str is null terminated
    const char	decode64[256],		// decoding table; if NULL: use TableDecode64default
    bool	allow_white_spaces,	// true: skip white spaces
    uint	*scanned_len		// not NULL: Store number of scanned 'str' bytes here
)
{
    DASSERT(buf);
    DASSERT(buf_size>=3);
    u8 *dest = (u8*)buf;
    u8 *dest_end = dest + ( scanned_len ? buf_size / 3 * 3 : buf_size );

    if (!source)
	source = "";
    ccp src = source;
    ccp end = src + ( len < 0 ? strlen(src) : len );
    int ch1, ch2;

    if (!decode64)
	decode64 = TableDecode64default;

    while ( dest < dest_end && src < end )
    {
	ch1 = decode64[(u8)*src++];
	if (allow_white_spaces)
	    while ( ch1 == DECODE_SPACE && src < end )
		ch1 = decode64[(u8)*src++];
	if ( ch1 < 0 )
	{
	    src--;
	    break;
	}
	if ( src == end )
	    break;

	ch2 = decode64[(u8)*src++];
	if (allow_white_spaces)
	    while ( ch2 == DECODE_SPACE && src < end )
		ch2 = decode64[(u8)*src++];
	if ( ch2 < 0 )
	{
	    src--;
	    break;
	}

	*dest++ = ch1 << 2 | ch2 >> 4;
	if ( src == end || dest == dest_end )
	    break;

	ch1 = decode64[(u8)*src++];
	if (allow_white_spaces)
	    while ( ch1 == DECODE_SPACE && src < end )
		ch1 = decode64[(u8)*src++];
	if ( ch1 < 0 )
	{
	    src--;
	    break;
	}

	*dest++ = ch2 << 4 | ch1 >> 2;
	if ( src == end || dest == dest_end )
	    break;

	ch2 = decode64[(u8)*src++];
	if (allow_white_spaces)
	    while ( ch2 == DECODE_SPACE && src < end )
		ch2 = decode64[(u8)*src++];
	if ( ch2 < 0 )
	{
	    src--;
	    break;
	}
	*dest++ = ch1 << 6 | ch2;
    }

    if (scanned_len)
    {
	while ( src < end && decode64[(u8)*src] == DECODE_FILLER )
	    src++;
	*scanned_len = src - source;
    }
    return dest - (u8*)buf;
}

///////////////////////////////////////////////////////////////////////////////

mem_t DecodeBase64Circ
(
    // Returns a buffer alloced by GetCircBuf()
    // with valid pointer and null terminated.
    // If result is too large (>CIRC_BUF_MAX_ALLOC) then (0,0) is returned.

    ccp		source,			// NULL or string to decode
    int		len,			// length of string. if -1, str is null terminated
    const char	decode64[256]		// decoding table; if NULL: use TableDecode64default
)
{
    char buf[CIRC_BUF_MAX_ALLOC+10];
    mem_t mem;
    mem.len = DecodeBase64(buf,sizeof(buf),source,len,decode64,false,0);
    if ( mem.len < CIRC_BUF_MAX_ALLOC )
	mem.ptr = CopyCircBuf0(buf,mem.len);
    else
    {
	mem.ptr = 0;
	mem.len = 0;
    }
    return mem;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

uint EncodeBase64
(
    // returns the number of scanned bytes of 'source'

    char	*buf,			// valid destination buffer
    uint	buf_size,		// size of 'buf', >= 4
    const void	*source,		// NULL or data to encode
    int		source_len,		// length of 'source'; if <0: use strlen(source)
    const char	encode64[64+1],		// encoding table; if NULL: use TableEncode64default
    bool	use_filler,		// use filler for aligned output

    ccp		next_line,		// not NULL: use this string as new line sep
    uint	bytes_per_line		// >0: use 'next_line' every # input bytes
					// will be rounded down to multiple of 3
)
{
    DASSERT(buf);
    DASSERT(buf_size>3);
    DASSERT(source||!source_len);

    noPRINT("EncodeBase64(sz=%u,len=%d,fil=%d)\n",buf_size,source_len,use_filler);

    if (!next_line)
	bytes_per_line = ~0;
    else if (!bytes_per_line)
	bytes_per_line = 19;
    else
	bytes_per_line /= 3;

    char *dest = buf;
    char *dest_end = buf + buf_size - 4;

    if (!source)
	source = "";
    const u8 *src = source;
    const u8 *src_end = src + ( source_len < 0 ? strlen(source) : source_len );

    if (!encode64)
	encode64 = TableEncode64default;

    uint n_tupel = 0;

    while ( src < src_end && dest < dest_end )
    {
	if ( ++n_tupel > bytes_per_line && next_line )
	{
	    char *next = StringCopyE(dest,(char*)buf+buf_size,next_line);
	    if ( next >= dest_end )
		break;
	    dest = next;
	    n_tupel = 1;
	}

	//----- start: no pending bits

	u8 ch1 = *src++;
	*dest++ = encode64 [ ch1 >> 2 ];
	ch1 &= 0x03;
	DASSERT( (ch1 & ~0x03) == 0 );
	if ( src == src_end )
	{
	    *dest++ = encode64[ch1<<4];
	    if (use_filler)
	    {
		*dest++ = encode64[64];
		*dest++ = encode64[64];
	    }
	    break;
	}

	//----- 2 pending bits in 'ch1'

	u8 ch2 = *src++;
	*dest++ = encode64 [ ch1 << 4 | ch2 >> 4 ];
	ch2 &= 0x0f;
	DASSERT( (ch2 & ~0x0f) == 0 );
	if ( src == src_end )
	{
	    *dest++ = encode64[ch2<<2];
	    if (use_filler)
		*dest++ = encode64[64];
	    break;
	}

	//----- 4 pending bits in 'ch2'

	ch1 = *src++;
	*dest++ = encode64 [ ch2 << 2 | ch1 >> 6 ];

	//----- 6 pending bits in 'ch1'

	*dest++ = encode64[ch1&0x3f];
    }

    *dest = 0;

    return src - (const u8*)source;
}

///////////////////////////////////////////////////////////////////////////////

mem_t EncodeBase64Circ
(
    // Returns a buffer alloced by GetCircBuf()
    // with valid pointer and null terminated.
    // If result is too large (>CIRC_BUF_MAX_ALLOC) then (0,0) is returned.

    const void	*source,		// NULL or data to encode
    int		source_len,		// length of 'source'; if <0: use strlen(source)
    const char	encode64[64+1]		// encoding table; if NULL: use TableEncode64default
)
{
    char buf[CIRC_BUF_MAX_ALLOC+10];
    EncodeBase64(buf,sizeof(buf),source,source_len,encode64,false,0,0);
    mem_t mem;
    mem.len = strlen(buf);
    if ( mem.len < CIRC_BUF_MAX_ALLOC )
	mem.ptr = CopyCircBuf(buf,mem.len+1);
    else
    {
	mem.ptr = 0;
	mem.len = 0;
    }
    return mem;
}

///////////////////////////////////////////////////////////////////////////////


uint EncodeBase64ml // ml: multi line
(
    // returns the number of scanned bytes of 'source'

    char	*buf,			// valid destination buffer
    uint	buf_size,		// size of 'buf', >= 4
    const void	*source,		// NULL or data to encode
    int		source_len,		// length of 'source'; if <0: use strlen(source)
    const char	encode64[64+1],		// encoding table; if NULL: use TableEncode64default
    bool	use_filler,		// use filler for aligned output

    int		indent,			// indention of output
    ccp		prefix,			// NULL or prefix before encoded data
    ccp		eol,			// line terminator, if NULL then use NL
    int		bytes_per_line		// create a new line every # input bytes
					// will be rounded down to multiple of 3
)
{
    indent = NormalizeIndent(indent);
    if (!eol)
	eol = "\n";

    char linesep[200];
    int pre_pos;
    snprintf(linesep,sizeof(linesep),"%s%n%*s%s",
		eol,
		&pre_pos,
		indent, "",
		prefix ? prefix : ""
		);

    char *end = buf + buf_size;
    char *dest = StringCopyE(buf,end,linesep+pre_pos);
    const uint scanned = EncodeBase64( dest, end-dest, source, source_len,
				encode64, use_filler, linesep, bytes_per_line );
    dest = buf + strlen(buf);
    StringCopyE(dest,end,eol);
    return scanned;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

uint EncodeJSON
(
    // returns the number of valid bytes in 'buf'. Result is NULL-terminated.

    char	*buf,			// valid destination buffer
    uint	buf_size,		// size of 'buf', >2 and 2 bytes longer than needed
    const void	*source,		// NULL or data to encode
    int		source_len		// length of 'source'; if <0: use strlen(source)
)
{
    DASSERT(buf);
    DASSERT(buf_size>2);

    char *dest = buf;
    char *dest_end = dest + buf_size - 2;

    if (!source)
	source = "";
    ccp str = source;
    ccp end = str + ( source_len < 0 ? strlen(str) : source_len );

    while ( dest < dest_end && str < end )
    {
	const u8 ch = (u8)*str++;
	switch (ch)
	{
	    case '"':  *dest++ = '\\'; *dest++ = '"'; break;
	    case '\\': *dest++ = '\\'; *dest++ = '\\'; break;
	    case '\b': *dest++ = '\\'; *dest++ = 'b'; break;
	    case '\f': *dest++ = '\\'; *dest++ = 'f'; break;
	    case '\n': *dest++ = '\\'; *dest++ = 'n'; break;
	    case '\r': *dest++ = '\\'; *dest++ = 'r'; break;
	    case '\t': *dest++ = '\\'; *dest++ = 't'; break;

	    default:
		if ( ch >= ' ' )
		    *dest++ = ch;
		else if ( dest + 5 > dest_end )
		    goto eos;
		else
		{
		    *dest++ = '\\';
		    *dest++ = 'u';
		    *dest++ = '0';
		    *dest++ = '0';
		    *dest++ = HiDigits[ch>>4];
		    *dest++ = HiDigits[ch&15];
		}
		break;
	}
    }

 eos:
    *dest = 0;
    return dest - buf;
}

//-----------------------------------------------------------------------------

mem_t EncodeJSONCirc
(
    // Returns a buffer alloced by GetCircBuf()
    // with valid pointer and null terminated.
    // If result is too large (>CIRC_BUF_MAX_ALLOC) then (0,0) is returned.

    const void	*source,		// NULL or data to encode
    int		source_len		// length of 'source'; if <0: use strlen(source)
)
{
    char buf[CIRC_BUF_MAX_ALLOC+10];
    mem_t mem;
    mem.len = EncodeJSON(buf,sizeof(buf),source,source_len);
    if ( mem.len < CIRC_BUF_MAX_ALLOC )
	mem.ptr = CopyCircBuf(buf,mem.len+1);
    else
    {
	mem.ptr = 0;
	mem.len = 0;
    }
    return mem;
}

//-----------------------------------------------------------------------------

mem_t QuoteJSONCirc
(
    // Returns a buffer alloced by GetCircBuf()
    // with valid pointer and null terminated.
    // If result is too large (>CIRC_BUF_MAX_ALLOC) then (0,0) is returned.

    const void	*source,		// NULL or data to encode
    int		source_len,		// length of 'source'; if <0: use strlen(source)
    int		null_if			// return 'null' without quotes ...
					//	<=0: never
					//	>=1: if source == NULL
					//	>=2: if source_len == 0
)
{
    if ( source_len < 0 )
	source_len = source ? strlen(source) : 0;

    mem_t mem;
    if ( null_if > 0 && !source || null_if > 1 && !source_len )
    {
	mem.ptr = "null";
	mem.len = 4;
    }
    else
    {
	char buf[CIRC_BUF_MAX_ALLOC+10];
	mem.len = EncodeJSON(buf+1,sizeof(buf)-2,source,source_len) + 2;
	if ( mem.len < CIRC_BUF_MAX_ALLOC - 2 )
	{
	    buf[0] = buf[mem.len-1] = '"';
	    buf[mem.len] = 0;
	    mem.ptr = CopyCircBuf(buf,mem.len+1);
	}
	else
	{
	    mem.ptr = 0;
	    mem.len = 0;
	}
    }
    return mem;
}

//-----------------------------------------------------------------------------

mem_t DecodeJSONCirc
(
    // Returns a buffer alloced by GetCircBuf()
    // with valid pointer and null terminated.
    // If result is too large (>CIRC_BUF_MAX_ALLOC) then (0,0) is returned.

    ccp		source,		// NULL or string to decode
    int		source_len,	// length of 'source'. If -1, str is NULL terminated
    int		quote		// 0:none, -1:auto, >0: quotation char
)
{
    char buf[CIRC_BUF_MAX_ALLOC+10];
    mem_t mem;
    mem.len = DecodeJSON(buf,sizeof(buf),source,source_len,quote,0);
    if ( mem.len < CIRC_BUF_MAX_ALLOC )
	mem.ptr = CopyCircBuf(buf,mem.len+1);
    else
    {
	mem.ptr = 0;
	mem.len = 0;
    }
    return mem;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

uint DecodeByMode
(
    // returns the number of valid bytes in 'buf'. Result is NULL-terminated.

    char		*buf,		// valid destination buffer
    uint		buf_size,	// size of 'buf', >= 3
    ccp			source,		// string to decode
    int			slen,		// length of string. if -1, str is null terminated
    EncodeMode_t	emode,		// encoding mode
    uint		*scanned_len	// not NULL: Store number of scanned 'str' bytes here
)
{
    DASSERT(buf);
    DASSERT( buf_size >= 1 );
    DASSERT( source || !slen );
    DASSERT( (uint)emode < ENCODE__N );

    uint len = 0;
    if ( buf_size < 4 )
    {
	char temp[4];
	len = DecodeByMode(temp,sizeof(temp),source,slen,emode,scanned_len);
	memcpy(buf,temp,buf_size);
	goto term;
    }

    DASSERT( buf_size >= 3 );
    switch (emode)
    {
      case ENCODE_STRING:
	len = ScanEscapedString(buf,buf_size,source,slen,false,-1,scanned_len);
	break;

      case ENCODE_UTF8:
	len = ScanEscapedString(buf,buf_size,source,slen,true,-1,scanned_len);
	break;

      case ENCODE_BASE64:
	len = DecodeBase64(buf,buf_size-1,source,slen,TableDecode64,true,scanned_len);
	break;

      case ENCODE_BASE64URL:
      case ENCODE_BASE64STAR:
	len = DecodeBase64(buf,buf_size-1,source,slen,TableDecode64url,true,scanned_len);
	break;

      case ENCODE_BASE64XML:
	len = DecodeBase64(buf,buf_size-1,source,slen,TableDecode64xml,true,scanned_len);
	break;

      case ENCODE_JSON:
	len = DecodeJSON(buf,buf_size,source,slen,-1,scanned_len);
	break;

      default:
	if ( slen < 0 )
	    len = StringCopyS(buf,buf_size,source) - buf;
	else
	    len = StringCopySM(buf,buf_size,source,slen) - buf;
	if (scanned_len)
	    *scanned_len = len;
	break;
    }

    //--- terminate with NULL

 term:;
    if ( len >= buf_size )
	len = buf_size - 1;
    buf[len] = 0;
    return len;
}

///////////////////////////////////////////////////////////////////////////////

mem_t DecodeByModeMem
(
    // Returns the decoded 'source'. Result is NULL-terminated.
    // It points either to 'buf' or is alloced (on buf==NULL or too less space)
    // If alloced (mem.ptr!=buf) => call FreeMem(&mem)

    char		*buf,		// NULL or destination buffer
    uint		buf_size,	// size of 'buf'
    ccp			source,		// string to decode
    int			slen,		// length of string. if -1, str is null terminated
    EncodeMode_t	emode,		// encoding mode
    uint		*scanned_len	// not NULL: Store number of scanned 'str' bytes here
)
{
    DASSERT( source || !slen );
    DASSERT( (uint)emode < ENCODE__N );

    if ( slen < 0 )
	slen = source ? strlen(source) : 0;

    uint need;
    switch (emode)
    {
      case ENCODE_STRING:
      case ENCODE_UTF8:
	need = slen + 5;
	break;

      case ENCODE_BASE64:
      case ENCODE_BASE64URL:
      case ENCODE_BASE64STAR:
      case ENCODE_BASE64XML:
	need = ( 3*slen ) / 4 + 12;
	break;

      //case ENCODE_JSON: // [[json]]
      default:
	need = slen + 3; // at least 3 bytes
	break;
    }

    const bool alloced = !buf || buf_size < need;
    if (alloced)
    {
	buf = MALLOC(need);
	buf_size = need;
    }

    mem_t mem;
    mem.len = DecodeByMode(buf,buf_size,source,slen,emode,scanned_len);
    if ( alloced && mem.len+10 < buf_size )
	buf = REALLOC( buf, mem.len+1 );
    mem.ptr = buf;
    return mem;
}

///////////////////////////////////////////////////////////////////////////////

uint DecodeByModeMemList
(
    // returns the number of scanned strings

    mem_list_t		*res,		// result
    uint		res_mode,	// 0:append, 1:clear, 2:init
    ccp			source,		// string to decode
    int			slen,		// length of string. if -1, str is null terminated
    EncodeMode_t	emode,		// encoding mode
    uint		*scanned_len	// not NULL: Store number of scanned 'str' bytes here
)
{
    DASSERT(res);
    if ( res_mode > 1 )
	InitializeMemList(res);
    else if ( res_mode > 0 )
	ResetMemList(res);

    if (!source)
    {
	if (scanned_len)
	    *scanned_len = 0;
	return 0;
    }

    ccp src = source;
    if ( slen < 0 )
	slen = strlen(src);
    NeedBufMemList(res,slen,0);
    ccp end = src + slen;

    char buf[4000];

    uint count;
    for ( count = 1;; count++ )
    {
	if ( emode != ENCODE_OFF )
	    while ( src < end && (uchar)*src <= ' ' )
		src++;

	if ( src >= end )
	{
	    AppendMemListN(res,&EmptyMem,1,MEMLM_ALL);
	    break;
	}

	if ( *src == ',' )
	    AppendMemListN(res,&EmptyMem,1,MEMLM_ALL);
	else if ( *src == '%' )
	{
	    AppendMemListN(res,&NullMem,1,MEMLM_ALL);
	    src++;
	}
	else if ( emode == ENCODE_OFF )
	{
	    mem_t mem;
	    mem.ptr = src;
	    src = memchr(src,',',end-src);
	    if (!src)
		src = end;
	    mem.len = src - mem.ptr;
	    AppendMemListN(res,&mem,1,MEMLM_ALL);
	}
	else
	{
	    uint scanned;
	    mem_t mem = DecodeByModeMem(buf,sizeof(buf),src,end-src,emode,&scanned);
	    src += scanned;

	    AppendMemListN(res,&mem,1,MEMLM_ALL);
	    if ( mem.ptr != buf )
		FreeString(mem.ptr);
	}

	while ( src < end && (uchar)*src <= ' ' )
	    src++;
	if ( src >= end || *src != ',' )
	    break;
	src++;
    }

    //--- terminate

    if (scanned_len)
	*scanned_len = src - source;
    return count;
}

///////////////////////////////////////////////////////////////////////////////

uint EncodeByMode
(
    // returns the number of valid bytes in 'buf'. Result is NULL-terminated.

    char		*buf,		// valid destination buffer
    uint		buf_size,	// size of 'buf', >= 4
    ccp			source,		// string to encode
    int			slen,		// length of string. if -1, str is null terminated
    EncodeMode_t	emode		// encoding mode
)
{
    DASSERT(buf);
    DASSERT( buf_size >= 3 );
    DASSERT( source || !slen );
    DASSERT( (uint)emode < ENCODE__N );

    noPRINT("EncodeByMode(,%u,%d,%d)\n",buf_size,slen,emode);

    uint len = 0;
    switch (emode)
    {
      case ENCODE_STRING:
	PrintEscapedString(buf,buf_size,source,slen,0,0,&len);
	break;

      case ENCODE_UTF8:
	PrintEscapedString(buf,buf_size,source,slen,CHMD__ALL,0,&len);
	break;

      case ENCODE_BASE64:
	len = EncodeBase64(buf,buf_size,source,slen,TableEncode64,true,0,0);
	len = GetEncodeBase64FillLen(len);
	break;

      case ENCODE_BASE64URL:
	len = EncodeBase64(buf,buf_size,source,slen,TableEncode64url,true,0,0);
	len = GetEncodeBase64FillLen(len);
	break;

      case ENCODE_BASE64STAR:
	len = EncodeBase64(buf,buf_size,source,slen,TableEncode64star,true,0,0);
	len = GetEncodeBase64FillLen(len);
	break;

      case ENCODE_BASE64XML:
	len = EncodeBase64(buf,buf_size,source,slen,TableEncode64xml,true,0,0);
	len = GetEncodeBase64FillLen(len);
	break;

      case ENCODE_JSON:
	len = EncodeJSON(buf,buf_size,source,slen);
	break;

      default:
	if ( slen < 0 )
	    len = StringCopyS(buf,buf_size,source) - buf;
	else
	    len = StringCopySM(buf,buf_size,source,slen) - buf;
	break;
    }

    //--- terminate with NULL

    if ( len >= buf_size )
	len = buf_size - 1;
    buf[len] = 0;
    return len;
}

///////////////////////////////////////////////////////////////////////////////

mem_t EncodeByModeMem
(
    // Returns the encoded 'source'. Result is NULL-terminated.
    // It points either to 'buf' or is alloced (on buf==NULL or too less space)
    // If alloced (mem.ptr!=buf) => call FreeMem(&mem)

    char		*buf,		// NULL or destination buffer
    uint		buf_size,	// size of 'buf'
    ccp			source,		// string to encode
    int			slen,		// length of string. if -1, str is null terminated
    EncodeMode_t	emode		// encoding mode
)
{
    DASSERT( source || slen <= 0 );
    DASSERT( (uint)emode < ENCODE__N );

    if ( slen < 0 )
	slen = source ? strlen(source) : 0;

    uint need;
    switch (emode)
    {
      case ENCODE_STRING:
      case ENCODE_UTF8:
	need = slen * 4 + 5;
	break;

      case ENCODE_BASE64:
      case ENCODE_BASE64URL:
      case ENCODE_BASE64STAR:
      case ENCODE_BASE64XML:
	need = ( 4*slen ) / 3 + 10;
	break;

      case ENCODE_JSON: // [[json]]
	{
	    need = slen + 1;
	    ccp ptr = source;
	    ccp end = ptr + slen;
	    while ( ptr < end )
		if ( (uchar)*ptr++ < ' ' )
		    need += 5;
	}
	break;

      default:
	need = slen + 1;
	break;
    }

    const bool alloced = !buf || buf_size < need;
    if (alloced)
    {
	buf = MALLOC(need);
	buf_size = need;
    }

    mem_t mem;
    mem.len = EncodeByMode(buf,buf_size,source,slen,emode);
    if ( alloced && mem.len+10 < buf_size )
	buf = REALLOC( buf, mem.len+1 );
    mem.ptr = buf;
    return mem;
}

//
///////////////////////////////////////////////////////////////////////////////
///////////////			    time			///////////////
///////////////////////////////////////////////////////////////////////////////

s64 timezone_adjust_sec   = -1;
s64 timezone_adjust_usec  = -1;
int timezone_adjust_isdst = -1;

///////////////////////////////////////////////////////////////////////////////

void SetupTimezone ( bool force )
{
    if ( force || timezone_adjust_sec == -1 )
    {
	timezone_adjust_sec  = GetTimezoneAdjust(GetTimeSec(false));
	timezone_adjust_usec = 1000000ll * timezone_adjust_sec;
	TRACE("TZ: %s,%s, %ld, %d => %lld %lld\n",
		tzname[0], tzname[1], timezone, daylight,
		timezone_adjust_sec, timezone_adjust_usec );
    }
}

///////////////////////////////////////////////////////////////////////////////

int GetTimezoneAdjust ( time_t tim )
{
    struct tm gmt, loc;
    gmtime_r(&tim,&gmt);
    localtime_r(&tim,&loc);
    int delta = ( gmt.tm_hour - loc.tm_hour ) * 3600
	      + ( gmt.tm_min  - loc.tm_min  ) *   60
	      + ( gmt.tm_sec  - loc.tm_sec  );
    if ( gmt.tm_yday != loc.tm_yday )
    {
	if ( gmt.tm_year < loc.tm_year
	    || gmt.tm_year == loc.tm_year && gmt.tm_yday < loc.tm_yday )
	{
	    delta -= 24*3600;
	}
	else
	{
	    delta += 24*3600;
	}
    }
    return delta;
}

///////////////////////////////////////////////////////////////////////////////

struct timeval GetTimeOfDay ( bool localtime )
{
    static uint last_hour = 0;
    struct timeval tval;
    gettimeofday(&tval,NULL);
    if (localtime)
    {
	const uint hour = tval.tv_sec / 3600;
	if ( last_hour != hour )
	{
	    last_hour = hour;
	    SetupTimezone(true);
	}
	tval.tv_sec -= timezone_adjust_sec;
    }
    return tval;
}

///////////////////////////////////////////////////////////////////////////////

DayTime_t GetDayTime ( bool localtime )
{
    struct timeval tval = GetTimeOfDay(localtime);
    div_t d;

    DayTime_t dt;
    dt.time	= tval.tv_sec;
    d		= div(tval.tv_sec,86400);
    dt.day	= d.quot;
    d		= div(d.rem,3600);
    dt.hour	= d.quot;
    d		= div(d.rem,60);
    dt.min	= d.quot;
    dt.sec	= d.rem;
    dt.usec	= tval.tv_usec;

    return dt;
}

///////////////////////////////////////////////////////////////////////////////

u_sec_t GetTimeSec ( bool localtime )
{
    struct timeval tval = GetTimeOfDay(localtime);
    return tval.tv_sec;
}

///////////////////////////////////////////////////////////////////////////////

u_msec_t GetTimeMSec ( bool localtime )
{
    struct timeval tval = GetTimeOfDay(localtime);
    return (u64)(tval.tv_sec) * 1000ll + tval.tv_usec/1000;
}

///////////////////////////////////////////////////////////////////////////////

u_usec_t GetTimeUSec ( bool localtime )
{
    struct timeval tval = GetTimeOfDay(localtime);
    return (u64)(tval.tv_sec) * 1000000ll + tval.tv_usec;
}

//
///////////////////////////////////////////////////////////////////////////////
///////////////			    timer			///////////////
///////////////////////////////////////////////////////////////////////////////

static time_t time_base = 0;

///////////////////////////////////////////////////////////////////////////////

u_msec_t GetTimerMSec()
{
    struct timeval tval;
    gettimeofday(&tval,NULL);
    if (!time_base)
	time_base = tval.tv_sec;

    return (u_msec_t)( tval.tv_sec - time_base ) * 1000 + tval.tv_usec/1000;
}

///////////////////////////////////////////////////////////////////////////////

u_usec_t GetTimerUSec()
{
    struct timeval tval;
    gettimeofday(&tval,NULL);
    if (!time_base)
	time_base = tval.tv_sec;

    return (u_usec_t)( tval.tv_sec - time_base ) * 1000000 + tval.tv_usec;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

ccp PrintTimeByFormat
(
    // returns temporary buffer by GetCircBuf();

    ccp			format,		// format string for strftime()
    time_t		tim		// seconds since epoch; 0 is replaced by time()
)
{
    if (!tim)
	tim = time(0);

    char buf[100];
    struct tm *tm = localtime(&tim);
    const uint len = strftime(buf,sizeof(buf),format,tm);
    return CopyCircBuf(buf,len+1);
}

//-----------------------------------------------------------------------------

ccp PrintTimeByFormatUTC
(
    // returns temporary buffer by GetCircBuf();

    ccp			format,		// format string for strftime()
    time_t		tim		// seconds since epoch; 0 is replaced by time()
)
{
    if (!tim)
	tim = time(0);

    char buf[100];
    struct tm *tm = gmtime(&tim);
    const uint len = strftime(buf,sizeof(buf),format,tm);
    return CopyCircBuf(buf,len+1);
}

///////////////////////////////////////////////////////////////////////////////

ccp PrintUsecByFormat
(
    // returns temporary buffer by GetCircBuf();

    ccp			format,		// format string for strftime()
					// 1-6 '@' in row replaced by digits of 'usec'
    time_t		tim,		// seconds since epoch
    uint		usec		// micro second of second
)
{
    char buf[100];
    struct tm *tm = localtime(&tim);
    const uint len = strftime(buf,sizeof(buf),format,tm);
    char *at = strchr(buf,'@');
    if (at)
    {
	char ubuf[8];
	snprintf(ubuf,sizeof(ubuf),"%06u",usec);
	ccp src = ubuf;
	while ( *at == '@' && *src )
	    *at++ = *src++;
    }

    return CopyCircBuf(buf,len+1);
}

//-----------------------------------------------------------------------------

ccp PrintUsecByFormatUTC
(
    // returns temporary buffer by GetCircBuf();

    ccp			format,		// format string for strftime()
					// 1-6 '@' in row replaced by digits of 'usec'
    time_t		tim,		// seconds since epoch
    uint		usec		// micro second of second
)
{
    char buf[100];
    struct tm *tm = gmtime(&tim);
    const uint len = strftime(buf,sizeof(buf),format,tm);
    char *at = strchr(buf,'@');
    if (at)
    {
	char ubuf[8];
	snprintf(ubuf,sizeof(ubuf),"%06u",usec);
	ccp src = ubuf;
	while ( *at == '@' && *src )
	    *at++ = *src++;
    }

    return CopyCircBuf(buf,len+1);
}

///////////////////////////////////////////////////////////////////////////////

ccp PrintTimevalByFormat
(
    // returns temporary buffer by GetCircBuf();

    ccp			format,		// format string for strftime()
    const struct timeval *tv		// time to print, if NULL use gettimeofday()
)
{
    struct timeval temp;
    if (!tv)
    {
	gettimeofday(&temp,0);
	tv = &temp;
    }

    return PrintUsecByFormat(format,tv->tv_sec,tv->tv_usec);
}

//-----------------------------------------------------------------------------

ccp PrintTimevalByFormatUTC
(
    // returns temporary buffer by GetCircBuf();

    ccp			format,		// format string for strftime()
    const struct timeval *tv		// time to print, if NULL use gettimeofday()
)
{
    struct timeval temp;
    if (!tv)
    {
	gettimeofday(&temp,0);
	tv = &temp;
    }

    return PrintUsecByFormatUTC(format,tv->tv_sec,tv->tv_usec);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

ccp PrintTimeSec
(
    char		* buf,		// result buffer (>19 bytes are good)
					// If NULL, a local circulary static buffer is used
    size_t		buf_size,	// size of 'buf', ignored if buf==NULL
    uint		sec		// seconds (=time) to print
)
{
    if (!buf)
	buf = GetCircBuf( buf_size = 20 );

    time_t time = sec;
    struct tm *tm = localtime(&time);
    strftime(buf,buf_size,"%F %T",tm);
    return buf;
}

///////////////////////////////////////////////////////////////////////////////

ccp PrintTimeMSec
(
    char		* buf,		// result buffer (>23 bytes are good)
					// If NULL, a local circulary static buffer is used
    size_t		buf_size,	// size of 'buf', ignored if buf==NULL
    u_msec_t		msec,		// milliseconds to print
    uint		fraction	// number of digits (0-3) to print as fraction
)
{
    if (!buf)
	buf = GetCircBuf( buf_size = 24 );

    time_t time = msec / 1000;
    struct tm *tm = localtime(&time);
    uint len = strftime(buf,buf_size,"%F %T",tm);

    if ( fraction && len + 4 < buf_size )
    {
	len += snprintf(buf+len, buf_size-len, ".%03llu", msec % 1000 );
	const uint pos = len - 3 + ( fraction < 3 ? fraction : 3 );
	if ( pos < buf_size )
	    buf[pos] = 0;
    }

    return buf;
}

///////////////////////////////////////////////////////////////////////////////

ccp PrintTimeUSec
(
    char		* buf,		// result buffer (>26 bytes are good)
					// If NULL, a local circulary static buffer is used
    size_t		buf_size,	// size of 'buf', ignored if buf==NULL
    u_usec_t		usec,		// microseconds to print
    uint		fraction	// number of digits (0-6) to print as fraction
)
{
    if (!buf)
	buf = GetCircBuf( buf_size = 28 );

    time_t time = usec / 1000000;
    struct tm *tm = localtime(&time);
    uint len = strftime(buf,buf_size,"%F %T",tm);

    if ( fraction && len + 7 < buf_size )
    {
	len += snprintf(buf+len, buf_size-len, ".%06llu", usec % 1000000 );
	const uint pos = len - 6 + ( fraction < 6 ? fraction : 6 );
	if ( pos < buf_size )
	    buf[pos] = 0;
    }

    return buf;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

ccp PrintTimerMSec
(
    char		* buf,		// result buffer (>12 bytes are good)
					// If NULL, a local circulary static buffer is used
    size_t		buf_size,	// size of 'buf', ignored if buf==NULL
    u32			msec,		// milliseconds to print
    uint		fraction	// number of digits (0-3) to print as fraction
)
{
    if (!buf)
	buf = GetCircBuf( buf_size = 16 );

    if (fraction)
    {
	const uint len = snprintf(buf,buf_size,"%02d:%02d:%02d.%03d",
		msec/3600000, msec/60000%60, msec/1000%60, msec%1000 );
	if ( len > 3 && fraction < 3 )
	{
	    const uint pos = len - 3 + fraction;
	    if ( pos < buf_size )
		buf[pos] = 0;
	}
    }
    else
	snprintf(buf,buf_size,"%02d:%02d:%02d",
	    msec/3600000, msec/60000%60, msec/1000%60 );

    ccp ptr = buf;
    int colon_counter = 0;
    while ( *ptr == '0' || *ptr == ':' && !colon_counter++ )
	ptr++;
    return *ptr == ':' ? ptr-1 : ptr;
}

///////////////////////////////////////////////////////////////////////////////

ccp PrintTimerUSec
(
    char		* buf,		// result buffer (>16 bytes are good)
					// If NULL, a local circulary static buffer is used
    size_t		buf_size,	// size of 'buf', ignored if buf==NULL
    s_usec_t		usec,		// microseconds to print
    uint		fraction	// number of digits (0-3) to print as fraction
)
{
    if ( !buf || buf_size < 4 )
	buf = GetCircBuf( buf_size = 20 );

    const bool minus = usec < 0;
    if (minus)
	usec = -usec;

    u32 sec = (u64)usec / 1000000;

    if (fraction)
    {
	const uint len = snprintf(buf+1,buf_size-1,"%02d:%02d:%02d.%06lld",
	    sec/3600, sec/60%60, sec%60, (u64)usec%1000000 );
	if ( len > 6 && fraction < 6 )
	{
	    const uint pos = len - 6 + fraction;
	    if ( pos < buf_size )
		buf[pos] = 0;
	}
    }
    else
	snprintf(buf+1,buf_size-1,"%02d:%02d:%02d",
	    sec/3600, sec/60%60, sec%60 );

    char *ptr = buf+1;
    while ( *ptr == '0' || *ptr == ':' )
	ptr++;
    if ( *ptr == '.' )
	ptr--;
    if (minus)
	*--ptr = '-';
    return ptr;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

ccp PrintTimer3
(
    char		* buf,		// result buffer (>3 bytes)
					// If NULL, a local circulary static buffer is used
    size_t		buf_size,	// size of 'buf', ignored if buf==NULL
    u64			sec,		// seconds to print
    int			usec,		// 0...999999: usec fraction,
					// otherwise suppress ms/us output
    bool		aligned		// true: print aligned 3 character output
)
{
    if (!buf)
	buf = GetCircBuf( buf_size = 4 );

    if ( !sec && usec >= 0 && usec <= 999999 )
    {
	if (!usec)
	    StringCopyS(buf,buf_size,aligned?"  0":"0");
	else if ( usec < 100 )
	    snprintf(buf,buf_size,aligned?"%2uu":"%uu",usec);
	else if ( usec < 1000 )
	    snprintf(buf,buf_size,".%ui",usec/100);
	else if ( usec < 100000 )
	    snprintf(buf,buf_size,aligned?"%2ui":"%ui",usec/1000);
	else
	    snprintf(buf,buf_size,".%us",usec/100000);
    }
    else if ( sec < 100 )
	snprintf(buf,buf_size,aligned?"%2llus":"%llus",sec);
    else if ( sec < 6000 )
	snprintf(buf,buf_size,aligned?"%2llum":"%llum",sec/60);
    else if ( sec < 360000 )
	snprintf(buf,buf_size,aligned?"%2lluh":"%lluh",sec/3600);
    else
    {
	const u64 days = sec / (24*3600);
	if ( days < 100 )
	    snprintf(buf,buf_size,aligned?"%2llud":"%llud",days);
	else if ( days < 700 )
	    snprintf(buf,buf_size,aligned?"%2lluw":"%lluw",days/7);
	else
	{
	    const u32 years = days / 365;
	    if ( years < 100 )
		snprintf(buf,buf_size,aligned?"%2uy":"%uy",years);
	    else
		buf = "***";
	}
    }

    return buf;
}

///////////////////////////////////////////////////////////////////////////////

ccp PrintTimer4
(
    char		* buf,		// result buffer (>4 bytes)
					// If NULL, a local circulary static buffer is used
    size_t		buf_size,	// size of 'buf', ignored if buf==NULL
    u64			sec,		// seconds to print
    int			usec,		// 0...999999: usec fraction,
					// otherwise suppress ms/us output
    bool		aligned		// true: print aligned 4 character output
)
{
    if (!buf)
	buf = GetCircBuf( buf_size = 5 );

    if ( sec < 10 && usec >= 0 && usec <= 999999 )
    {
	if (sec)
	    snprintf(buf,buf_size,"%llu.%us",sec,usec/100000);
	else if (!usec)
	    StringCopyS(buf,buf_size,aligned?"   0":"0");
	else if ( usec < 1000 )
	    snprintf(buf,buf_size,aligned?"%3uu":"%uu",usec);
	else if ( usec < 10000 )
	    snprintf(buf,buf_size,"%u.%ui",usec/1000,usec/100%10);
	else
	    snprintf(buf,buf_size,aligned?"%3ui":"%ui",usec/1000);
    }
    else if ( sec < 600 )
	snprintf(buf,buf_size,aligned?"%3llus":"%llus",sec);
    else if ( sec < 36000 )
	snprintf(buf,buf_size,aligned?"%3llum":"%llum",sec/60);
    else if ( sec < 168*3600 )
	snprintf(buf,buf_size,aligned?"%3lluh":"%lluh",sec/3600);
    else
    {
	const u64 days = sec / (24*3600);
	if ( days < 365 )
	    snprintf(buf,buf_size,aligned?"%3llud":"%llud",days);
	else if ( days < 10*365 )
	    snprintf(buf,buf_size,aligned?"%3lluw":"%lluw",days/7);
	else
	{
	    const u32 years = days / 365;
	    if ( years < 1000 )
		snprintf(buf,buf_size,aligned?"%3uy":"%uy",years);
	    else
		buf = "****";
	}
    }

    return buf;
}

///////////////////////////////////////////////////////////////////////////////

ccp PrintTimer6
(
    char		* buf,		// result buffer (>6 bytes)
					// If NULL, a local circulary static buffer is used
    size_t		buf_size,	// size of 'buf', ignored if buf==NULL
    u64			sec,		// seconds to print
    int			usec,		// 0...999999: usec fraction,
					//    otherwise suppress ms/us output
    bool		aligned		// true: print aligned 6 character output
)
{
    if (!buf)
	buf = GetCircBuf( buf_size = 7 );

    if (aligned)
    {
	if ( sec < 10 && usec >= 0 && usec <= 999999 )
	{
	    usec += 1000000 * sec;
	    if (!usec)
		StringCopyS(buf,buf_size,"     0");
	    else if ( usec < 10000 )
		snprintf(buf,buf_size,"%4uus",usec);
	    else
		snprintf(buf,buf_size,"%4ums",usec/1000);
	}
	else if ( sec < 100 )
	{
	    if ( usec >= 0 && usec <= 999999 )
		snprintf(buf,buf_size,"%2llu.%02us",sec,usec/10000);
	    else
		snprintf(buf,buf_size,"%5llus",sec);
	}
	else if ( sec < 100*60 )
	    snprintf(buf,buf_size,"%2llum%02llus",sec/60,sec%60);
	else if ( sec < 100*3600 )
	    snprintf(buf,buf_size,"%2lluh%02llum",sec/3600,sec/60%60);
	else
	{
	    const u64 days = sec / (24*3600);
	    if ( days < 100 )
		snprintf(buf,buf_size,"%2llud%02lluh",days,sec/3600%24);
	    else if ( days < 7000 )
		snprintf(buf,buf_size,"%3lluw%llud",days/7,days%7);
	    else
	    {
		const u32 years = days / 365;
		if ( years < 100 )
		    snprintf(buf,buf_size,"%2uy%02lluw",years,days%365/7);
		else if ( years < 100000 )
		    snprintf(buf,buf_size,"%5uy",years);
		else
		    buf = "******";
	    }
	}
    }
    else
    {
	if ( sec < 10 )
	{
	    usec += 1000000 * sec;
	    if (!usec)
		StringCopyS(buf,buf_size,"0");
	    else if ( usec < 10000 )
		snprintf(buf,buf_size,"%uus",usec);
	    else
		snprintf(buf,buf_size,"%ums",usec/1000);
	}
	else if ( sec < 100 )
	{
	    if ( usec >= 0 && usec <= 999999 )
	    {
		const uint val2 = usec/10000;
		if (val2)
		    snprintf(buf,buf_size,"%llu.%02us",sec,val2);
		else
		    snprintf(buf,buf_size,"%llus",sec);
	    }
	    else
		snprintf(buf,buf_size,"%llus",sec);
	}
	else if ( sec < 100*60 )
	{
	    const uint val2 = sec%60;
	    if (val2)
		snprintf(buf,buf_size,"%llum%02us",sec/60,val2);
	    else
		snprintf(buf,buf_size,"%llum",sec/60);
	}
	else if ( sec < 100*3600 )
	{
	    const uint val2 = sec/60%60;
	    if (val2)
		snprintf(buf,buf_size,"%lluh%02um",sec/3600,val2);
	    else
		snprintf(buf,buf_size,"%lluh",sec/3600);
	}
	else
	{
	    const u64 days = sec / (24*3600);
	    if ( days < 100 )
	    {
		const uint val2 = sec/3600%24;
		if (val2)
		    snprintf(buf,buf_size,"%llud%02uh",days,val2);
		else
		    snprintf(buf,buf_size,"%llud",days);
	    }
	    else if ( days < 7000 )
	    {
		const uint val2 = days%7;
		if (val2)
		    snprintf(buf,buf_size,"%lluw%ud",days/7,val2);
		else
		    snprintf(buf,buf_size,"%lluw",days/7);
	    }
	    else
	    {
		const u32 years = days / 365;
		if ( years < 100 )
		{
		    const uint val2 = days%365/7;
		    if (val2)
			snprintf(buf,buf_size,"%uy%02uw",years,val2);
		    else
			snprintf(buf,buf_size,"%uy",years);
		}
		else if ( years < 100000 )
		    snprintf(buf,buf_size,"%uy",years);
		else
		    buf = "******";
	    }
	}
    }

    return buf;
}

///////////////////////////////////////////////////////////////////////////////

ccp PrintTimerUSec4s
(
    char		* buf,		// result buffer (>4 bytes)
					// If NULL, a local circulary static buffer is used
    size_t		buf_size,	// size of 'buf', ignored if buf==NULL
    s_usec_t		usec,		// microseconds to print
    sizeform_mode_t	mode		// support of DC_SFORM_ALIGN, DC_SFORM_PLUS
)
{
    if ( !buf || buf_size < 2 )
	buf = GetCircBuf( buf_size = 5 );

    if (!usec)
	StringCopyS(buf,buf_size, mode & DC_SFORM_ALIGN ? "   0" : "0" );
    else
    {
	char sign;
	if ( usec < 0 )
	{
	    usec = -usec;
	    sign = '-';
	}
	else
	    sign = mode & DC_SFORM_PLUS ? '+' : ' ';

	PrintTimerUSec3(buf+1,buf_size-1,usec,mode&DC_SFORM_ALIGN);
	*buf = ' ';

	char *ptr = buf+1;
	while ( *ptr == ' ' )
	    ptr++;
	ptr[-1] = sign;
    }
    return buf;
}

///////////////////////////////////////////////////////////////////////////////

ccp PrintTimerUSec7s
(
    char		* buf,		// result buffer (>7 bytes)
					// If NULL, a local circulary static buffer is used
    size_t		buf_size,	// size of 'buf', ignored if buf==NULL
    s_usec_t		usec,		// microseconds to print
    sizeform_mode_t	mode		// support of DC_SFORM_ALIGN, DC_SFORM_PLUS
)
{
    if ( !buf || buf_size < 2 )
	buf = GetCircBuf( buf_size = 8 );

    if (!usec)
	StringCopyS(buf,buf_size, mode & DC_SFORM_ALIGN ? "      0" : "0" );
    else
    {
	char sign;
	if ( usec < 0 )
	{
	    usec = -usec;
	    sign = '-';
	}
	else
	    sign = mode & DC_SFORM_PLUS ? '+' : ' ';

	PrintTimerUSec6(buf+1,buf_size-1,usec, mode & DC_SFORM_ALIGN );
	*buf = ' ';

	char *ptr = buf+1;
	while ( *ptr == ' ' )
	    ptr++;
	ptr[-1] = sign;
    }
    return buf;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#if 0
u64 GetXTime ( xtime_t * xtime )
{
    struct timeval tval;
    gettimeofday(&tval,0);
    if (!time_base)
	time_base = tval.tv_sec;
    if (!rel_usec_base)
	rel_usec_base = (u64)( tval.tv_sec - time_base ) * 1000000 + tval.tv_usec;

    const ull total_usec = 1000000ull * tval.tv_sec + tval.tv_usec;

    if (xtime)
    {
	xtime->time       = tval.tv_sec;
	xtime->usec       = tval.tv_usec;
	xtime->total_usec = total_usec;
	xtime->rel_usec   = total_usec - rel_usec_base;
    }
    return total_usec;
}
#endif

//
///////////////////////////////////////////////////////////////////////////////
///////////////			scan date & time		///////////////
///////////////////////////////////////////////////////////////////////////////

char * ScanInterval
(
    time_t	*res_time,	// not NULL: store seconds
    u32		*res_usec,	// not NULL: store micro seconds of second
    ccp		source		// source text
)
{
    uint days = 0, sec = 0, usec = 0;
    bool day_possible = true;
    uint colon_count = 0;
    char *src = source ? (char*)source : "";

    while ( *src > 0 && *src <= ' ' )
	src++;

    for(;;)
    {
	if ( *src < '0' || *src >='9' )
	    break;
	ulong num = strtoul(src,&src,10);
	if ( *src == 'd' && day_possible )
	{
	    src++;
	    days = num;
	    day_possible = false;
	}
	else if ( *src == ':' && colon_count < 2 )
	{
	    src++;
	    colon_count++;
	    sec = ( sec + num ) * 60;
	}
	else
	{
	    sec += num;
	    if ( *src == '.' )
	    {
		src++;
		uint i;
		for ( i = 0; i < 6; i++ )
		{
		    usec *= 10;
		    if ( *src >= '0' && *src <= '9' )
			usec += *src++ - '0';
		}
		while ( *src >= '0' && *src <= '9' )
		    src++;
	    }
	    else if ( colon_count == 1 )
		sec *= 60;
	}
    }

    if (res_time)
	*res_time = days * 86400 + sec;
    if (res_usec)
	*res_usec = usec;
    return src;
}

///////////////////////////////////////////////////////////////////////////////

char * ScanInterval64
(
    u64		*res_usec,	// not NULL: store total micro seconds
    ccp		source		// source text
)
{
    time_t time;
    u32 usec;
    char *src = ScanInterval(&time,&usec,source);
    if (res_usec)
	*res_usec = time * 1000000ull + usec;
    return src;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

char * ScanDateTime
(
    time_t	*res_time,	// not NULL: store seconds here
    u32		*res_usec,	// not NULL: store useconds of second here
    ccp		source,		// source text
    bool	allow_delta	// true: allow +|- interval
)
{
    //--- scan date

    time_t total_time = 0;
    int total_usec = 0;
    ccp src = source ? source : "";
    while ( *src > 0 && *src <= ' ' )
	src++;

    struct tm tm;
    memset(&tm,0,sizeof(tm));
    src = strptime(src,"%Y-%m-%d",&tm);

    noPRINT(" %4u-%02u-%02u %2u:%02u:%02u |%s|\n",
	tm.tm_year+1900, tm.tm_mon+1, tm.tm_mday,
	tm.tm_hour, tm.tm_min, tm.tm_sec, src );

    if (src)
	total_time = mktime(&tm);
    else
	src = source ? source : "";
    noPRINT_TIME(total_time,"DATE");


    //--- scan time

    time_t time;
    u32 usec;
    src = ScanInterval(&time,&usec,src);
    total_time += time;
    total_usec += usec;
    noPRINT_TIME(total_time,"TIME");


    //--- scan timezone

    ccp src2 = src;
    while ( *src2 > 0 && *src2 <= ' ' )
	src2++;
    if ( ( src2[0] == '-' || src2[0] == '+' )
	&& src2[1] >= '0' && src2[1] <= '9'
	&& src2[2] >= '0' && src2[2] <= '9'
	&& src2[3] >= '0' && src2[3] <= '9'
	&& src2[4] >= '0' && src2[4] <= '9'
	&& ( src2[5] < '0' || src2[5] > '9' ))
    {
	int delta = 36000 * src2[1]
		  +  3600 * src2[2]
		  +   600 * src2[3]
		  +    60 * src2[4]
		  - 40260 * '0';
	if ( src2[0] == '-' )
	    delta = -delta;

	noPRINT("GOT TIMEZONE: %.5s => %d\n",src2,delta);
	total_time -= delta + timezone;
	src = src2 + 5;
    }
    else
    {
	PRINT("timezone=%ld\n",timezone);
	struct tm *tm = localtime(&total_time);
	if (tm->tm_isdst)
	    total_time -= 3600;
    }
    noPRINT_TIME(total_time,"ZONE");


    //--- scan delta

    if (allow_delta)
    {
	ccp src2 = src;
	while ( *src2 > 0 && *src2 <= ' ' )
	    src2++;
	bool minus = *src2 == '-';
	if ( minus || *src2 == '+' )
	{
	    src = ScanInterval(&time,&usec,src2+1);
	    if (minus)
	    {
		total_time -= time;
		total_usec -= usec;
	    }
	    else
	    {
		total_time += time;
		total_usec += usec;
	    }
	}
    }

    if ( total_usec < 0 )
    {
	total_time -= ( 999999 - total_usec ) / 1000000;
	total_usec  = 1000000 - (-total_usec) % 1000000;
    }
    else if ( total_usec >= 1000000 )
    {
	total_time += total_usec / 1000000;
	total_usec %= 1000000;
    }

    if (res_time)
	*res_time = total_time;
    if (res_usec)
	*res_usec = total_usec;
    return (char*)src;
}

///////////////////////////////////////////////////////////////////////////////

char * ScanDateTime64
(
    u64		*res_usec,	// not NULL: store total micro seconds
    ccp		source,		// source text
    bool	allow_delta	// true: allow +|- interval
)
{
    time_t time;
    u32 usec;
    char *src = ScanDateTime(&time,&usec,source,allow_delta);
    if (res_usec)
	*res_usec = time * 1000000ull + usec;
    return src;
}

//
///////////////////////////////////////////////////////////////////////////////
///////////////			Since 2001			///////////////
///////////////////////////////////////////////////////////////////////////////
// hour support

ccp hour2text
(
	int hour,	// hour to print
	int fw,		// wanted field width assuming separators are 1 char
	ccp sep_date,	// NULL or separator within date
			// if NULL: use '-'
	ccp sep_dt	// NULL or separator of between date and time
			// if NULL: use ' '
)
{
    time_t tim = hour2time(hour);
    struct tm tm;
    gmtime_r(&tim,&tm);

    if (!sep_date)
	sep_date = "-";

    if (!sep_dt)
	sep_dt = " ";

    return fw >= 13
	? PrintCircBuf("%04d%s%02u%s%02u%s%02u",
		tm.tm_year + 1900, sep_date,
		tm.tm_mon  +    1, sep_date,
		tm.tm_mday, sep_dt,
		tm.tm_hour
		)
	: fw >= 11
	? PrintCircBuf("%02d%s%02u%s%02u%s%02u",
		tm.tm_year % 100, sep_date,
		tm.tm_mon  +   1, sep_date,
		tm.tm_mday, sep_dt,
		tm.tm_hour
		)
	: fw >= 10
	? PrintCircBuf("%2u.%s%02u:xx",
		tm.tm_mday, sep_dt,
		tm.tm_hour
		)
	: PrintCircBuf("%02u%s%02u%s%02u",
		tm.tm_mon  +   1, sep_date,
		tm.tm_mday, sep_dt,
		tm.tm_hour
		);
}

///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
// day support

ccp day2text
(
	int day,	// day to print
	int fw,		// wanted field width assuming separators are 1 char
	ccp sep_date,	// NULL or separator within date
			// if NULL: use '-'
	ccp sep_dt	// ignored
)
{
    time_t tim = day2time(day);
    struct tm tm;
    gmtime_r(&tim,&tm);

    if (!sep_date)
	sep_date = "-";

    return fw >= 10
	? PrintCircBuf("%04d%s%02u%s%02u",
		tm.tm_year + 1900, sep_date,
		tm.tm_mon  +    1, sep_date,
		tm.tm_mday )
	: PrintCircBuf("%02d%s%02u%s%02u",
		tm.tm_year % 100, sep_date,
		tm.tm_mon  +   1, sep_date,
		tm.tm_mday );
}

///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
// week support

ccp week2text
(
	int week,	// week to print
	int fw,		// wanted field width assuming separators are 1 char
	ccp sep_date,	// NULL or separator within date
			// if NULL: use 'w'
	ccp sep_dt	// ignored
)
{
    time_t tim = week2time(week) + 4*86400;
    struct tm tm;
    gmtime_r(&tim,&tm);

    return PrintCircBuf("%04d%s%02u",
		tm.tm_year + 1900,
		sep_date ? sep_date : "w",
		tm.tm_yday / 7 + 1 );
}

///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
// month support

int time2month ( time_t tim )
{
    struct tm tm;
    gmtime_r(&tim,&tm);
    return ( tm.tm_mon + 12 * tm.tm_year ) - 101*12;
}

///////////////////////////////////////////////////////////////////////////

time_t month2time ( int month )
{
    SetupTimezone(false);
    struct tm tm = {0};

    tm.tm_isdst	= -1;
    tm.tm_year	= month / 12 + 101;
    tm.tm_mon	= month % 12;
    tm.tm_mday	= 1;

    int delta = 43200-timezone_adjust_sec;
    if ( delta < 0 )
    {
	delta += 86400;
	tm.tm_mday++;
    }

    tm.tm_hour	= delta / 3600;
    tm.tm_min	= delta / 60 % 60;
    return mktime(&tm) / 86400 * 86400;
}

///////////////////////////////////////////////////////////////////////////

ccp month2text
(
	int month,	// month to print
	int fw,		// wanted field width assuming separators are 1 char
	ccp sep_date,	// NULL or separator within date
			// if NULL: use '-'
	ccp sep_dt	// ignored
)
{
    month += 2001*12;
    return PrintCircBuf("%4d%s%02u",
		month/12,
		sep_date ? sep_date : "-",
		month%12 + 1 );
}

///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
// quarter support

int time2quarter ( time_t tim )
{
    struct tm tm;
    gmtime_r(&tim,&tm);
    return ( tm.tm_mon/3 + 4 * tm.tm_year ) - 101*4;
}

///////////////////////////////////////////////////////////////////////////

time_t quarter2time ( int quarter )
{
    SetupTimezone(false);
    struct tm tm = {0};

    tm.tm_isdst	= -1;
    tm.tm_year	= quarter / 4 + 101;
    tm.tm_mon	= quarter % 4 * 3;
    tm.tm_mday	= 1;

    int delta = 43200-timezone_adjust_sec;
    if ( delta < 0 )
    {
	delta += 86400;
	tm.tm_mday++;
    }

    tm.tm_hour	= delta / 3600;
    tm.tm_min	= delta / 60 % 60;
    return mktime(&tm) / 86400 * 86400;
}

///////////////////////////////////////////////////////////////////////////

ccp quarter2text
(
	int quarter,	// quarter to print
	int fw,		// wanted field width assuming separators are 1 char
	ccp sep_date,	// NULL or separator within date
			// if NULL: use 'q'
	ccp sep_dt	// ignored
)
{
    quarter += 2001*4;
    return PrintCircBuf("%4d%s%u",
		quarter/4,
		sep_date ? sep_date : "q",
		quarter%4 + 1 );
}

///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
// year support

int time2year ( time_t tim )
{
    struct tm tm;
    gmtime_r(&tim,&tm);
    return tm.tm_year + 1900;
}

///////////////////////////////////////////////////////////////////////////

time_t year2time ( int year )
{
    SetupTimezone(false);
    struct tm tm = {0};

    tm.tm_isdst	= -1;
    tm.tm_year	= year - 1900;
    tm.tm_mday	= 1;

    int delta = 43200-timezone_adjust_sec;
    if ( delta < 0 )
    {
	delta += 86400;
	tm.tm_mday++;
    }

    tm.tm_hour	= delta / 3600;
    tm.tm_min	= delta / 60 % 60;
    return mktime(&tm) / 86400 * 86400;
}

//
///////////////////////////////////////////////////////////////////////////////
///////////////			urandom support			///////////////
///////////////////////////////////////////////////////////////////////////////

int urandom_available = 0; // <0:not, =0:not tested, >0:ok
bool use_urandom_for_myrandom = true;

uint ReadFromUrandom ( void *dest, uint size )
{
    static int fd = -1;

    if ( fd == -1 )
    {
	if ( urandom_available < 0 )
	    return 0;
     #ifdef O_CLOEXEC
	fd = open("/dev/urandom",O_CLOEXEC|O_NONBLOCK);
     #else
	fd = open("/dev/urandom",O_NONBLOCK);
     #endif
	if ( fd == -1 )
	{
	    urandom_available = -1;
	    return 0;
	}
     #ifndef O_CLOEXEC
	fcntl(fd,F_SETFD,FD_CLOEXEC);
     #endif
	urandom_available = 1;
    }

    u8 *buf = dest;
    while ( size > 0 )
    {
	ssize_t stat = read(fd,buf,size);
	if ( stat < 0 )
	{
	    urandom_available = -1;
	    close(fd);
	    return 0;
	}
	buf  += stat;
	size -= stat;
    }

    return buf - (u8*)dest;
}

//
///////////////////////////////////////////////////////////////////////////////
///////////////			random numbers			///////////////
///////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------
// Algorithm: The Art of Computer Programming, part II, section 3.6
// by Donald E. Knuth
//------------------------------------------------------------------

const u32 RANDOM32_C_ADD = 2 * 197731421; // 2 Primzahlen
const u32 RANDOM32_COUNT_BASE = 4294967; // Primzahl == ~UINT_MAX32/1000;

static int random32_a_index = -1;	// Index in die a-Tabelle
static u32 random32_count = 1;		// Abw�rts-Z�hler bis zum Wechsel von a,c
static u32 random32_a,
	   random32_c,
	   random32_X;			// Die letzten Werte

static u32 random32_a_tab[] = // Init-Tabelle
{
    0xbb40e62d, 0x3dc8f2f5, 0xdc024635, 0x7a5b6c8d,
    0x583feb95, 0x91e06dbd, 0xa7ec03f5, 0
};

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

u32 MyRandom ( u32 max )
{
    if (use_urandom_for_myrandom)
    {
	u32 res32;
	if (ReadFromUrandom(&res32,sizeof(res32)))
	{
	    if (!max)
		return res32;

	    u64 res64 = (u64)max * res32;
	    return res64 >> 32;
	}
	use_urandom_for_myrandom = false;
    }

    if (!--random32_count)
    {
	// Neue Berechnung von random32_a und random32_c f�llig

	if ( random32_a_index < 0 )
	{
	    // allererste Initialisierung auf Zeitbasis

	    MySeedByTime();
	}
	else
	{
	    random32_c += RANDOM32_C_ADD;
	    random32_a = random32_a_tab[++random32_a_index];
	    if (!random32_a)
	    {
		random32_a_index = 0;
		random32_a = random32_a_tab[0];
	    }

	    random32_count = RANDOM32_COUNT_BASE;
	}
    }

    // Jetzt erfolgt die eigentliche Berechnung

    random32_X = random32_a * random32_X + random32_c;

    if (!max)
	return random32_X;

    u64 res = (u64)max * random32_X;
    return res >> 32;
}

///////////////////////////////////////////////////////////////////////////////

u64 MySeed ( u64 base )
{
    uint a_tab_len = 0;
    while (random32_a_tab[a_tab_len])
	a_tab_len++;
    const u32 base32 = base / a_tab_len;

    random32_a_index	= (int)( base % a_tab_len );
    random32_a		= random32_a_tab[random32_a_index];
    random32_c		= ( base32 & 15 ) * RANDOM32_C_ADD + 1;
    random32_X		= base32 ^ ( base >> 32 );
    random32_count	= RANDOM32_COUNT_BASE;

    return base;
}

///////////////////////////////////////////////////////////////////////////////

u64 MySeedByTime()
{
 #if WIN_SZS_LIB

    u64 time;
    QueryPerformanceCounter((LARGE_INTEGER*)&time);
    return MySeed(time);

 #else

    struct timeval tval;
    gettimeofday(&tval,NULL);
    return MySeed( tval.tv_sec ^ tval.tv_usec );

 #endif
}

///////////////////////////////////////////////////////////////////////////////

void MyRandomFill ( void * buf, size_t size )
{
    size_t xsize = size / sizeof(u32);
    if (xsize)
    {
	size -= xsize * sizeof(u32);
	u32 * ptr = buf;
	while ( xsize-- > 0 )
	    *ptr++ = MyRandom(0);
	buf = ptr;
    }

    u8 * ptr = buf;
    while ( size-- > 0 )
	*ptr++ = MyRandom(0);
}

//
///////////////////////////////////////////////////////////////////////////////
///////////////			UUID				///////////////
///////////////////////////////////////////////////////////////////////////////

void CreateUUID ( uuid_buf_t dest )
{
    DASSERT(dest);
    const uint stat = ReadFromUrandom(dest,sizeof(uuid_buf_t));
    if (!stat)
	MyRandomFill(dest,sizeof(uuid_buf_t));

    dest[6] = dest[6] & 0x0f | 0x40;
    dest[8] = dest[8] & 0x3f | 0x80;
}

///////////////////////////////////////////////////////////////////////////////

uint CreateTextUUID ( char *buf, uint bufsize )
{
    uuid_buf_t uuid;
    CreateUUID(uuid);
    return PrintUUID(buf,bufsize,uuid);
}

///////////////////////////////////////////////////////////////////////////////

uint PrintUUID ( char *buf, uint bufsize, uuid_buf_t uuid )
{
    return snprintf(buf,bufsize,
	"%02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-%02x%02x%02x%02x%02x%02x",
	uuid[0],uuid[1],uuid[2],uuid[3],
	uuid[4],uuid[5], uuid[6],uuid[7],  uuid[8],uuid[9],
	uuid[10],uuid[11],uuid[12],uuid[13],uuid[14],uuid[15] );
}

///////////////////////////////////////////////////////////////////////////////

char * ScanUUID ( uuid_buf_t uuid, ccp source )
{
    memset(uuid,0,sizeof(uuid_buf_t));

    uint i;
    ccp src = source;
    for ( i = 0; i < sizeof(uuid_buf_t); i++ )
    {
	if ( *src == '-' )
	    src++;

	uint num;
	ccp end = ScanNumber(&num,src,0,16,2);
	if ( end != src+2 )
	    return (char*)source;
	uuid[i] = num;
	src = end;
    }
    return (char*)src;
}

//
///////////////////////////////////////////////////////////////////////////////
///////////////			gcd(), lcm()			///////////////
///////////////////////////////////////////////////////////////////////////////

uint gcd ( uint n1, uint n2 ) // greatest common divisor, german: ggt
{
    if ( !n1 || !n2 )
	return 0;

    if ( n1 > n2 )
    {
	uint temp = n1;
	n1 = n2;
	n2 = temp;
    }

    while ( n1 != 1 )
    {
	DASSERT ( n1 > 1 );
	DASSERT ( n2 >= n1 );

	uint temp = n2 % n1;
	if ( !temp )
	    break;
	n2 = n1;
	n1 = temp;
    }
    return n1;
}

///////////////////////////////////////////////////////////////////////////////

u32 gcd32 ( u32 n1, u32 n2 ) // greatest common divisor, german: ggt
{
    if ( !n1 || !n2 )
	return 0;

    if ( n1 > n2 )
    {
	u32 temp = n1;
	n1 = n2;
	n2 = temp;
    }

    while ( n1 != 1 )
    {
	DASSERT ( n1 > 1 );
	DASSERT ( n2 > n1 );

	u32 temp = n2 % n1;
	if ( !temp )
	    break;
	n2 = n1;
	n1 = temp;
    }
    return n1;
}

///////////////////////////////////////////////////////////////////////////////

u64 gcd64 ( u64 n1, u64 n2 ) // greatest common divisor, german: ggt
{
    if ( !n1 || !n2 )
	return 0;

    if ( n1 > n2 )
    {
	u64 temp = n1;
	n1 = n2;
	n2 = temp;
    }

    while ( n1 != 1 )
    {
	DASSERT ( n1 > 1 );
	DASSERT ( n2 > n1 );

	u64 temp = n2 % n1;
	if ( !temp )
	    break;
	n2 = n1;
	n1 = temp;
    }
    return n1;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

uint lcm ( uint n1, uint n2 ) // lowest common multiple, german: kgv
{
    uint g = gcd(n1,n2);
    if ( g > 0 )
    {
	n1 /= g;
	g = n1 * n2;
	if ( g < n1 || g < n2 ) // overflow?
	    g = 0;
    }
    return g;
}

///////////////////////////////////////////////////////////////////////////////

u32 lcm32 ( u32 n1, u32 n2 ) // lowest common multiple, german: kgv
{
    u32 g = gcd32(n1,n2);
    if ( g > 0 )
    {
	n1 /= g;
	g = n1 * n2;
	if ( g < n1 || g < n2 ) // overflow?
	    g = 0;
    }
    return g;
}

///////////////////////////////////////////////////////////////////////////////

u64 lcm64 ( u64 n1, u64 n2 ) // lowest common multiple, german: kgv
{
    u64 g = gcd64(n1,n2);
    if ( g > 0 )
    {
	n1 /= g;
	g = n1 * n2;
	if ( g < n1 || g < n2 ) // overflow?
	    g = 0;
    }
    return g;
}

//
///////////////////////////////////////////////////////////////////////////////
///////////////			    scan number			///////////////
///////////////////////////////////////////////////////////////////////////////

#define NN NUMBER_NULL
#define NC NUMBER_CONTROL
#define NL NUMBER_LINE
#define NS NUMBER_SPACE
#define NT NUMBER_TIE
#define NP NUMBER_SEPARATE
#define NX NUMBER_OTHER

const char DigitTable[] =
{
    NN, NC, NC, NC,  NC, NC, NC, NC,  NC, NS, NL, NC,  NS, NL, NC, NC,
    NC, NC, NC, NC,  NC, NC, NC, NC,  NC, NC, NC, NC,  NC, NC, NC, NC,
    NS, NX, NX, NX,  NX, NX, NX, NX,  NX, NX, NX, NX,  NP, NT, NT, NX,	//  !"#$%&' ()*+,-./
     0,  1,  2,  3,   4,  5,  6,  7,   8,  9, NT, NP,  NX, NX, NX, NX,	// 01234567 89:;<=>?

    NX, 10, 11, 12,  13, 14, 15, 16,  17, 18, 19, 20,  21, 22, 23, 24,	// @ABCDEFG HIJKLMNO
    25, 26, 27, 28,  29, 30, 31, 32,  33, 34, 35, NX,  NX, NX, NX, NX,	// PQRSTUVW XYZ[\]^_
    NX, 10, 11, 12,  13, 14, 15, 16,  17, 18, 19, 20,  21, 22, 23, 24,	// `abcdefg hijklmno
    25, 26, 27, 28,  29, 30, 31, 32,  33, 34, 35, NX,  NX, NX, NX, NX,	// pqrstuvw xyz{|}~

    NX, NX, NX, NX,  NX, NX, NX, NX,  NX, NX, NX, NX,  NX, NX, NX, NX,
    NX, NX, NX, NX,  NX, NX, NX, NX,  NX, NX, NX, NX,  NX, NX, NX, NX,
    NX, NX, NX, NX,  NX, NX, NX, NX,  NX, NX, NX, NX,  NX, NX, NX, NX,
    NX, NX, NX, NX,  NX, NX, NX, NX,  NX, NX, NX, NX,  NX, NX, NX, NX,

    NX, NX, NX, NX,  NX, NX, NX, NX,  NX, NX, NX, NX,  NX, NX, NX, NX,
    NX, NX, NX, NX,  NX, NX, NX, NX,  NX, NX, NX, NX,  NX, NX, NX, NX,
    NX, NX, NX, NX,  NX, NX, NX, NX,  NX, NX, NX, NX,  NX, NX, NX, NX,
    NX, NX, NX, NX,  NX, NX, NX, NX,  NX, NX, NX, NX,  NX, NX, NX, NX,
};

#undef NN
#undef NC
#undef NL
#undef NS
#undef NT
#undef NP
#undef NX

///////////////////////////////////////////////////////////////////////////////

u64 ScanDigits
(
    // same as ScanNumber(), but other interface
    // returns the scanned number

    ccp	  *source,	// pointer to source string, modified
    ccp	  end_source,	// NULL or end of 'source'
    uint  intbase,	// integer base, 2..36
    int	  maxchar,	// max number of digits to read
    uint  *count	// not NULL: store number of scanned digits here
)
{
    DASSERT(source);
    DASSERT( intbase >= 2 && intbase <= 36 );

    ccp src = *source;
    u64 num = 0;
    while ( maxchar-- && ( !end_source || src < end_source ) )
    {
	const u8 digit = (u8)DigitTable[(u8)*src];
	if ( digit >= intbase )
	    break;
	num = num * intbase + digit;
	src++;
    }
    if (count)
	*count = src - *source;
    *source = src;
    return num;
}

///////////////////////////////////////////////////////////////////////////////

char * ScanNumber
(
    // same as ScanDigits(), but other interface
    // returns a pointer to the first not used character

    uint  *dest_num,	// store result here, never NULL
    ccp	  src,		// pointer to source string, modified
    ccp	  src_end,	// NULL or end of 'src'
    uint  intbase,	// integer base, 2..36
    int	  maxchar	// max number of digits to read
)
{
    DASSERT(dest_num);
    uint num = 0;
    if (src)
    {
	while ( maxchar-- && ( !src_end || src < src_end ) )
	{
	    const uint n = (uint)DigitTable[*(uchar*)src];
	    if ( n >= (uint)intbase )
		break;
	    num = num * intbase + n;
	    src++;
	}
    }
    *dest_num = num;
    return (char*)src;
}

///////////////////////////////////////////////////////////////////////////////

char * ScanEscape
(
    // returns a pointer to the first not used character

    uint  *dest_code,	// store result here, never NULL
    ccp	  src,		// pointer to source string (behind escape char)
    ccp	  src_end	// NULL or end of 'src'
)
{
    DASSERT(dest_code);
    DASSERT(src);
    if ( !src || ( src_end ? src >= src_end : !*src ) )
    {
	*dest_code = 0;
	return (char*)src;
    }

    uint code = 0;
    switch (*src++)
    {
	case '\\': code = '\\'; break;
	case 'a':  code = '\a'; break;
	case 'b':  code = '\b'; break;
	case 'E':
	case 'e':  code = '\e'; break;
	case 'f':  code = '\f'; break;
	case 'n':  code = '\n'; break;
	case 'r':  code = '\r'; break;
	case 't':  code = '\t'; break;
	case 'v':  code = '\v'; break;

	case 'x':
	    src = ScanNumber(&code,src,src_end,16,2);
	    break;

	case 'u':
	    src = ScanNumber(&code,src,src_end,16,4);
	    break;

	case 'U':
	    src = ScanNumber(&code,src,src_end,16,8);
	    break;

	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	    src = ScanNumber(&code,src-1,src_end,8,3);
	    break;

	default:
	    code = src[-1];
	    break;
    }

    *dest_code = code;
    return (char*)src;
}

///////////////////////////////////////////////////////////////////////////////

uint ScanHexString
(
    // return the number of written bytes

    void  *buf,		// write scanned data here
    uint  buf_size,	// size of buf
    ccp	  *source,	// pointer to source string, modified
    ccp	  end_source,	// NULL or end of 'source'
    bool  allow_tie	// allow chars ' .:-' and TAB,VT as byte separator
)
{
    DASSERT(buf);
    DASSERT(source);

    u8 *dest = buf;
    uint count = 0;
    while ( count < buf_size )
    {
	ccp src = *source;
	if (allow_tie)
	{
	    while ( !end_source || src < end_source )
	    {
		const char res = DigitTable[(u8)*src];
		if ( res != NUMBER_SPACE && res != NUMBER_TIE )
		    break;
		src++;
	    }
	    *source = src;
	}

	while ( !end_source || src < end_source )
	{
	    const char res = DigitTable[(u8)*src];
	    if ( res < 0 || res >= 16 )
		break;
	    src++;
	}

	uint n_digits = src - *source;
	if (!n_digits)
	    break;

	while ( n_digits > 0 )
	{
	    uint scanned;
	    uint num = ScanDigits( source, end_source, 16,
					n_digits & 1 ? 1 : 2, &scanned );
	    if (!scanned)
		return count;

	    n_digits -= scanned;
	    *dest++ = num;
	    count++;
	}
    }

    return count;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

char * ScanS32
(
    // return 'source' on error

    s32		*res_num,		// not NULL: store result (only on success)
    ccp		source,			// NULL or source text
    uint	default_base		// base for numbers without '0x' prefix
					//  0: C like with octal support
					// 10: standard value for decimal numbers
					// 16: standard value for hex numbers
)
{
    if (!source)
	return 0;

    ccp src = source;
    while ( *src > 0 && *src <= ' ' )
	src++;

    const bool minus = *src == '-';
    if ( minus || *src == '+' )
    {
	src++;
	while ( *src > 0 && *src <= ' ' )
	    src++;
    }
    const uint base = src[0] == '0' && ( src[1] == 'x' || src[1] == 'X' )
			? 16 : default_base;
    char *end;
    const s32 num = strtoul( src, &end, base );
    if ( (ccp)end > src )
    {
	if (res_num)
	    *res_num = minus ? -num : num;
	return end;
    }

    return (char*)source;
}

///////////////////////////////////////////////////////////////////////////////

char * ScanS64
(
    // return 'source' on error

    s64		*res_num,		// not NULL: store result (only on success)
    ccp		source,			// NULL or source text
    uint	default_base		// base for numbers without '0x' prefix
					//  0: C like with octal support
					// 10: standard value for decimal numbers
					// 16: standard value for hex numbers
)
{
    if (!source)
	return 0;

    ccp src = source;
    while ( *src > 0 && *src <= ' ' )
	src++;

    const bool minus = *src == '-';
    if ( minus || *src == '+' )
    {
	src++;
	while ( *src > 0 && *src <= ' ' )
	    src++;
    }
    const uint base = src[0] == '0' && ( src[1] == 'x' || src[1] == 'X' )
			? 16 : default_base;
    char *end;
    const s64 num = strtoull( src, &end, base );
    if ( (ccp)end > src )
    {
	if (res_num)
	    *res_num = minus ? -num : num;
	return end;
    }

    return (char*)source;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// strto*() replacements, base ion Scan*() with better base support

long int str2l ( const char *nptr, char **endptr, int base )
{
    s32 res = 0;
    char *end = ScanS32(&res,nptr,base);
    if (endptr)
	*endptr = end;
    return res;
}

///////////////////////////////////////////////////////////////////////////////

long long int str2ll ( const char *nptr, char **endptr, int base )
{
    s64 res = 0;
    char *end = ScanS64(&res,nptr,base);
    if (endptr)
	*endptr = end;
    return res;
}

///////////////////////////////////////////////////////////////////////////////

unsigned long int str2ul ( const char *nptr, char **endptr, int base )
{
    u32 res = 0;
    char *end = ScanU32(&res,nptr,base);
    if (endptr)
	*endptr = end;
    return res;
}

///////////////////////////////////////////////////////////////////////////////

unsigned long long int str2ull ( const char *nptr, char **endptr, int base )
{
    u64 res = 0;
    char *end = ScanU64(&res,nptr,base);
    if (endptr)
	*endptr = end;
    return res;
}

//
///////////////////////////////////////////////////////////////////////////////
///////////////		round float/double (zero LSB)		///////////////
///////////////////////////////////////////////////////////////////////////////

float RoundF2bytes ( float f )
{
    union { float f; u32 u; } x;
    x.f = f;
    if ( x.u & 0x8000 )
    {
	x.u &= 0xffff0000;
	int exp;
	const float frac = frexpf(x.f,&exp);
	x.f = ldexpf(frac+0.00585938,exp);
    }

    x.u &= 0xffff0000;
    return x.f;
}

//-----------------------------------------------------------------------------

float RoundF3bytes ( float f )
{
    union { float f; u32 u; } x;
    x.f = f;
    if ( x.u & 0x80 )
    {
	x.u &= 0xffffff00;
	int exp;
	const float frac = frexpf(x.f,&exp);
	x.f = ldexpf(frac+0.00002289,exp);
    }

    x.u &= 0xffffff00;
    return x.f;
}

//-----------------------------------------------------------------------------

double RoundD6bytes ( double d )
{
    union { double d; u64 u; } x;
    x.d = d;
    if ( x.u & 0x8000 )
    {
	x.u &= 0xffffffffffff0000ull;
	int exp;
	const double frac = frexp(x.d,&exp);
	x.d = ldexp(frac+0.000000000010913936,exp);
    }

    x.u &= 0xffffffffffff0000ull;
    return x.d;
}

//-----------------------------------------------------------------------------

double RoundD7bytes ( double d )
{
    union { double d; u64 u; } x;
    x.d = d;
    if ( x.u & 0x80 )
    {
	x.u &= 0xffffffffffffff00ull;
	int exp;
	const double frac = frexp(x.d,&exp);
	x.d = ldexp(frac+0.000000000000042633,exp);
    }

    x.u &= 0xffffffffffffff00ull;
    return x.d;
}

//
///////////////////////////////////////////////////////////////////////////////
///////////////			print number			///////////////
///////////////////////////////////////////////////////////////////////////////

char * PrintNumberU4
(
    char		*buf,		// result buffer
					// NULL: use a local circulary static buffer
    size_t		buf_size,	// size of 'buf', ignored if buf==NULL
    u64			num,		// number to print
    sizeform_mode_t	mode		// any of DC_SFORM_ALIGN, DC_SFORM_DASH
)
{
    if (!buf)
	buf = GetCircBuf( buf_size = 5 );
    const sizeform_mode_t aligned = mode & DC_SFORM_ALIGN;

    if ( num < 1000000000 ) // 1e9
    {
	u32 n32 = num;
	if ( !n32 && mode & DC_SFORM_DASH )
	    StringCopyS(buf,buf_size, aligned ? "   -" : "-" );
	else if ( n32 < 10000 )
	    snprintf(buf,buf_size, aligned ? "%4u" : "%u", n32 );
	else if ( n32 < 1000000 )
	    snprintf(buf,buf_size, aligned ? "%3uk" : "%uk", n32/1000 );
	else if ( n32 < 10000000 )
	{
	    n32 /= 100000;
	    snprintf(buf,buf_size,"%u.%uM", n32/10, n32%10 );
	}
	else
	    snprintf(buf,buf_size, aligned ? "%3uM" : "%uM", n32/1000000 );
    }
    else
    {
	u32 n32;
	ccp factor = "GTPE";
	if ( num < 1000000000000000ull ) // 1e15
	    n32 = num / 100000000; // 1e8
	else
	{
	    factor += 2;
	    n32 = num / 100000000000000ull; // 1e14
	}

	while (*factor)
	{
	    if ( n32 < 100 )
	    {
		snprintf(buf,buf_size,"%u.%u%c", n32/10, n32%10, *factor );
		break;
	    }
	    else if ( n32 < 10000 )
	    {
		snprintf(buf,buf_size, aligned ? "%3u%c" : "%u%c", n32/10, *factor );
		break;
	    }
	    n32 /= 1000;
	    factor++;
	}
    }
    return buf;
}

///////////////////////////////////////////////////////////////////////////////

char * PrintNumberU5
(
    char		*buf,		// result buffer
					// NULL: use a local circulary static buffer
    size_t		buf_size,	// size of 'buf', ignored if buf==NULL
    u64			num,		// number to print
    sizeform_mode_t	mode		// any of DC_SFORM_ALIGN, DC_SFORM_DASH
)
{
    if (!buf)
	buf = GetCircBuf( buf_size = 6 );
    const sizeform_mode_t aligned = mode & DC_SFORM_ALIGN;

    if ( num < 1000000000 ) // 1e9
    {
	u32 n32 = num;
	if ( !n32 && mode & DC_SFORM_DASH )
	    StringCopyS(buf,buf_size, aligned ? "    -" : "-" );
	else if ( n32 < 100000 )
	    snprintf(buf,buf_size, aligned ? "%5u" : "%u", n32 );
	else if ( n32 < 10000000 )
	    snprintf(buf,buf_size, aligned ? "%4uk" : "%uk", n32/1000 );
	else
	    snprintf(buf,buf_size, aligned ? "%4uM" : "%uM", n32/1000000 );
    }
    else
    {
	u32 n32;
	ccp factor = "MGTPE";
	if ( num < 1000000000000000ull ) // 1e15
	    n32 = num / 1000000; // 1e6
	else
	{
	    factor += 2;
	    n32 = num / 1000000000000ull; // 1e12
	}

	while (*factor)
	{
	    if ( n32 < 10000 )
	    {
		snprintf(buf,buf_size, aligned ? "%4u%c" : "%u%c", n32, *factor );
		break;
	    }
	    n32 /= 1000;
	    factor++;
	}
    }
    return buf;
}

///////////////////////////////////////////////////////////////////////////////

char * PrintNumberU6
(
    char		*buf,		// result buffer
					// NULL: use a local circulary static buffer
    size_t		buf_size,	// size of 'buf', ignored if buf==NULL
    u64			num,		// number to print
    sizeform_mode_t	mode		// any of DC_SFORM_ALIGN, DC_SFORM_DASH
)
{
    if (!buf)
	buf = GetCircBuf( buf_size = 7 );
    const sizeform_mode_t aligned = mode & DC_SFORM_ALIGN;

    if ( num < 1000000000 ) // 1e9
    {
	u32 n32 = num;
	if ( !n32 && mode & DC_SFORM_DASH )
	    StringCopyS(buf,buf_size, aligned ? "     -" : "-" );
	else if ( n32 < 1000000 )
	    snprintf(buf,buf_size, aligned ? "%6u" : "%u", n32 );
	else if ( n32 < 100000000 )
	    snprintf(buf,buf_size, aligned ? "%5uk" : "%uk", n32/1000 );
	else
	    snprintf(buf,buf_size, aligned ? "%5uM" : "%uM", n32/1000000 );
    }
    else
    {
	u32 n32;
	ccp factor = "MGTPE";
	if ( num < 1000000000000000ull ) // 1e15
	    n32 = num / 1000000; // 1e6
	else
	{
	    factor += 2;
	    n32 = num / 1000000000000ull; // 1e12
	}

	while (*factor)
	{
	    if ( n32 < 100000 )
	    {
		snprintf(buf,buf_size, aligned ? "%5u%c" : "%u%c", n32, *factor );
		break;
	    }
	    n32 /= 1000;
	    factor++;
	}
    }
    return buf;
}

///////////////////////////////////////////////////////////////////////////////

char * PrintNumberU7
(
    char		*buf,		// result buffer
					// NULL: use a local circulary static buffer
    size_t		buf_size,	// size of 'buf', ignored if buf==NULL
    u64			num,		// number to print
    sizeform_mode_t	mode		// any of DC_SFORM_ALIGN, DC_SFORM_DASH
)
{
    if (!buf)
	buf = GetCircBuf( buf_size = 8 );
    const sizeform_mode_t aligned = mode & DC_SFORM_ALIGN;

    if ( num < 1000000000 ) // 1e9
    {
	u32 n32 = num;
	if ( !n32 && mode & DC_SFORM_DASH )
	    StringCopyS(buf,buf_size, aligned ? "      -" : "-" );
	else if ( n32 < 10000000 )
	    snprintf(buf,buf_size, aligned ? "%7u" : "%u", n32 );
	else
	    snprintf(buf,buf_size, aligned ? "%6uk" : "%uk", n32/1000 );
    }
    else
    {
	u32 n32;
	ccp factor = "MGTPE";
	if ( num < 1000000000000000ull ) // 1e15
	    n32 = num / 1000000; // 1e6
	else
	{
	    factor += 2;
	    n32 = num / 1000000000000ull; // 1e12
	}

	while (*factor)
	{
	    if ( n32 < 1000000 )
	    {
		snprintf(buf,buf_size, aligned ? "%6u%c" : "%u%c", n32, *factor );
		break;
	    }
	    n32 /= 1000;
	    factor++;
	}
    }
    return buf;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

char * PrintNumberS5
(
    char		*buf,		// result buffer
					// NULL: use a local circulary static buffer
    size_t		buf_size,	// size of 'buf', ignored if buf==NULL
    s64			num,		// number to print
    sizeform_mode_t	mode		// any of DC_SFORM_ALIGN, DC_SFORM_DASH
)
{
    if ( num >= 0 )
	return PrintNumberU5(buf,buf_size,num,mode);

    if (!buf)
	buf = GetCircBuf( buf_size = 6 );
    const sizeform_mode_t aligned = mode & DC_SFORM_ALIGN;

    if ( num > -1000000000 ) // -1e9
    {
	s32 n32 = num;
	if ( n32 > -10000 )
	    snprintf(buf,buf_size, aligned ? "%5d" : "%d", n32 );
	else if ( n32 > -1000000 )
	    snprintf(buf,buf_size, aligned ? "%4dk" : "%dk", n32/1000 );
	else if ( n32 > -10000000 )
	{
	    n32 /= -100000;
	    snprintf(buf,buf_size,"-%u.%uM", n32/10, n32%10 );
	}
	else
	    snprintf(buf,buf_size, aligned ? "%4dM" : "%dM", n32/1000000 );
    }
    else
    {
	s32 n32;
	ccp factor = "GTPE";
	if ( num > -1000000000000000ull ) // -1e15
	    n32 = num / 100000000; // -1e8
	else
	{
	    factor += 2;
	    n32 = num / 100000000000000ull; // -1e14
	}

	while (*factor)
	{
	    if ( n32 > -100 )
	    {
		snprintf(buf,buf_size,"-%u.%u%c", -n32/10, -n32%10, *factor );
		break;
	    }
	    else if ( n32 > -10000 )
	    {
		snprintf(buf,buf_size, aligned ? "%4d%c" : "%d%c", n32/10, *factor );
		break;
	    }
	    n32 /= 1000;
	    factor++;
	}
    }
    return buf;
}

///////////////////////////////////////////////////////////////////////////////

char * PrintNumberS6
(
    char		*buf,		// result buffer
					// NULL: use a local circulary static buffer
    size_t		buf_size,	// size of 'buf', ignored if buf==NULL
    s64			num,		// number to print
    sizeform_mode_t	mode		// any of DC_SFORM_ALIGN, DC_SFORM_DASH
)
{
    if ( num >= 0 )
	return PrintNumberU6(buf,buf_size,num,mode);

    if (!buf)
	buf = GetCircBuf( buf_size = 7 );
    const sizeform_mode_t aligned = mode & DC_SFORM_ALIGN;

    if ( num > -1000000000 ) // -1e9
    {
	s32 n32 = num;
	if ( n32 > -100000 )
	    snprintf(buf,buf_size, aligned ? "%6d" : "%d", n32 );
	else if ( n32 > -10000000 )
	    snprintf(buf,buf_size, aligned ? "%5dk" : "%dk", n32/1000 );
	else
	    snprintf(buf,buf_size, aligned ? "%5dM" : "%dM", n32/1000000 );
    }
    else
    {
	s32 n32;
	ccp factor = "MGTPE";
	if ( num > -1000000000000000ull ) // -1e15
	    n32 = num / 1000000; // -1e6
	else
	{
	    factor += 2;
	    n32 = num / 1000000000000ull; // -1e12
	}

	while (*factor)
	{
	    if ( n32 > -10000 )
	    {
		snprintf(buf,buf_size, aligned ? "%5d%c" : "%d%c", n32, *factor );
		break;
	    }
	    n32 /= 1000;
	    factor++;
	}
    }
    return buf;
}

///////////////////////////////////////////////////////////////////////////////

char * PrintNumberS7
(
    char		*buf,		// result buffer
					// NULL: use a local circulary static buffer
    size_t		buf_size,	// size of 'buf', ignored if buf==NULL
    s64			num,		// number to print
    sizeform_mode_t	mode		// any of DC_SFORM_ALIGN, DC_SFORM_DASH
)
{
    if ( num >= 0 )
	return PrintNumberU7(buf,buf_size,num,mode);

    if (!buf)
	buf = GetCircBuf( buf_size = 8 );
    const sizeform_mode_t aligned = mode & DC_SFORM_ALIGN;

    if ( num > -100000000 ) // -1e8
    {
	s32 n32 = num;
	if ( n32 > -1000000 )
	    snprintf(buf,buf_size, aligned ? "%7d" : "%d", n32 );
	else
	    snprintf(buf,buf_size, aligned ? "%6dk" : "%dk", n32/1000 );
    }
    else
    {
	s32 n32;
	ccp factor = "MGTPE";
	if ( num > -1000000000000000ull ) // -1e15
	    n32 = num / 1000000; // -1e6
	else
	{
	    factor += 2;
	    n32 = num / 1000000000000ull; // -1e12
	}

	while (*factor)
	{
	    if ( n32 > -100000 )
	    {
		snprintf(buf,buf_size, aligned ? "%6d%c" : "%d%c", n32, *factor );
		break;
	    }
	    n32 /= 1000;
	    factor++;
	}
    }
    return buf;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

char * PrintNumberD5
(
    char		*buf,		// result buffer
					// NULL: use a local circulary static buffer
    size_t		buf_size,	// size of 'buf', ignored if buf==NULL
    double		num,		// number to print
    sizeform_mode_t	mode		// any of DC_SFORM_ALIGN
)
{
    if (!buf)
	buf = GetCircBuf( buf_size = 16 );
    const sizeform_mode_t aligned = mode & DC_SFORM_ALIGN;

    double abs = fabs(num);
    if ( abs >= 1e2 )
    {
	if ( num <= 0 )
	{
	    if ( abs <= S64_MAX )
		return PrintNumberS5(buf,buf_size,double2int64(num),mode);
	    return "-****";
	}
	else
	{
	    if ( abs <= U64_MAX )
		return PrintNumberU5(buf,buf_size,double2int64(num),mode);
	}
	snprintf(buf,buf_size,"%5.0e",num);
	return buf;
    }

    if ( num < 0.0 )
    {
	if ( abs < 1e-2 )
	    return aligned ? "   -0" : "-0";

	if ( abs < 10.0 )
	    snprintf(buf,buf_size,"%5.2f",num);
	else
	    snprintf(buf,buf_size,"%5.1f",num);
    }
    else
    {
	if ( num < 1e-3 )
	    return aligned ? "    0" : "0";

	if ( num < 10.0 )
	    snprintf(buf,buf_size,"%5.3f",num);
	else
	    snprintf(buf,buf_size,"%5.2f",num);
    }

    return buf;
}

///////////////////////////////////////////////////////////////////////////////

char * PrintNumberD6
(
    char		*buf,		// result buffer
					// NULL: use a local circulary static buffer
    size_t		buf_size,	// size of 'buf', ignored if buf==NULL
    double		num,		// number to print
    sizeform_mode_t	mode		// any of DC_SFORM_ALIGN
)
{
    if (!buf)
	buf = GetCircBuf( buf_size = 7 );
    const sizeform_mode_t aligned = mode & DC_SFORM_ALIGN;

    double abs = fabs(num);
    if ( abs >= 1e3 )
    {
	if ( num <= 0 )
	{
	    if ( abs <= S64_MAX )
		return PrintNumberS6(buf,buf_size,double2int64(num),mode);
	}
	else
	{
	    if ( abs <= U64_MAX )
		return PrintNumberU6(buf,buf_size,double2int64(num),mode);
	}
	snprintf(buf,buf_size,"%6.0e",num);
	return buf;
    }

    if ( num < 0.0 )
    {
	if ( abs < 1e-3 )
	    return aligned ? "    -0" : "-0";

	if ( abs < 10.0 )
	    snprintf(buf,buf_size,"%6.3f",num);
	else if ( abs < 100.0 )
	    snprintf(buf,buf_size,"%6.2f",num);
	else
	    snprintf(buf,buf_size,"%6.1f",num);
    }
    else
    {
	if ( num < 1e-4 )
	    return aligned ? "     0" : "0";

	if ( num < 10.0 )
	    snprintf(buf,buf_size,"%6.4f",num);
	else if ( num < 100.0 )
	    snprintf(buf,buf_size,"%6.3f",num);
	else
	    snprintf(buf,buf_size,"%6.2f",num);
    }

    return buf;
}

///////////////////////////////////////////////////////////////////////////////

char * PrintNumberD7
(
    char		*buf,		// result buffer
					// NULL: use a local circulary static buffer
    size_t		buf_size,	// size of 'buf', ignored if buf==NULL
    double		num,		// number to print
    sizeform_mode_t	mode		// any of DC_SFORM_ALIGN
)
{
    if (!buf)
	buf = GetCircBuf( buf_size = 18 );
    const sizeform_mode_t aligned = mode & DC_SFORM_ALIGN;

    double abs = fabs(num);
    if ( abs >= 1e4 )
    {
	if ( num <= 0 )
	{
	    if ( abs <= S64_MAX )
		return PrintNumberS7(buf,buf_size,double2int64(num),mode);
	    snprintf(buf,buf_size,"%7.0e",num);
	    return buf;
	}
	else
	{
	    if ( abs <= U64_MAX )
		return PrintNumberU7(buf,buf_size,double2int64(num),mode);
	}
	snprintf(buf,buf_size,"%7.1e",num);
	return buf;
    }

    if ( num < 0.0 )
    {
	if ( abs < 1e-4 )
	    return aligned ? "     -0" : "-0";

	if ( abs < 10.0 )
	    snprintf(buf,buf_size,"%7.4f",num);
	else if ( abs < 100.0 )
	    snprintf(buf,buf_size,"%7.3f",num);
	else if ( abs < 1000.0 )
	    snprintf(buf,buf_size,"%7.2f",num);
	else
	    snprintf(buf,buf_size,"%7.1f",num);
    }
    else
    {
	if ( num < 1e-5 )
	    return aligned ? "      0" : "0";

	if ( num < 10.0 )
	    snprintf(buf,buf_size,"%7.5f",num);
	else if ( num < 100.0 )
	    snprintf(buf,buf_size,"%7.4f",num);
	else if ( num < 1000.0 )
	    snprintf(buf,buf_size,"%7.3f",num);
	else
	    snprintf(buf,buf_size,"%7.2f",num);
    }

    return buf;
}

//
///////////////////////////////////////////////////////////////////////////////
///////////////			print size			///////////////
///////////////////////////////////////////////////////////////////////////////

ccp dc_size_tab_1000[DC_SIZE_N_MODES+1] =
{
    0,		// DC_SIZE_DEFAULT
    0,		// DC_SIZE_AUTO

    "B",	// DC_SIZE_BYTES
    "kB",	// DC_SIZE_K
    "MB",	// DC_SIZE_M
    "GB",	// DC_SIZE_G
    "TB",	// DC_SIZE_T
    "PB",	// DC_SIZE_P
    "EB",	// DC_SIZE_E

    0		// all others
};

//-----------------------------------------------------------------------------

ccp dc_size_tab_1024[DC_SIZE_N_MODES+1] =
{
    0,		// DC_SIZE_DEFAULT
    0,		// DC_SIZE_AUTO

    "B",	// DC_SIZE_BYTES
    "KiB",	// DC_SIZE_K
    "MiB",	// DC_SIZE_M
    "GiB",	// DC_SIZE_G
    "TiB",	// DC_SIZE_T
    "PiB",	// DC_SIZE_P
    "EiB",	// DC_SIZE_E

    0		// all others
};

///////////////////////////////////////////////////////////////////////////////

ccp GetSizeUnit // get a unit for column headers
(
    size_mode_t	mode,		// print mode
    ccp		if_invalid	// output for invalid modes
)
{
    const bool force_1000 = ( mode & DC_SIZE_M_BASE ) == DC_SIZE_F_1000;

    switch ( mode & DC_SIZE_M_MODE )
    {
	//---- SI and IEC units

	case DC_SIZE_DEFAULT:
	case DC_SIZE_AUTO:	return "size";
	case DC_SIZE_BYTES:	return "bytes";

	case DC_SIZE_K:		return force_1000 ? "kB" : "KiB";
	case DC_SIZE_M:		return force_1000 ? "MB" : "MiB";
	case DC_SIZE_G:		return force_1000 ? "GB" : "GiB";
	case DC_SIZE_T:		return force_1000 ? "TB" : "TiB";
	case DC_SIZE_P:		return force_1000 ? "PB" : "PiB";
	case DC_SIZE_E:		return force_1000 ? "EB" : "EiB";
    }

    return if_invalid;
}

///////////////////////////////////////////////////////////////////////////////

int GetSizeFW // get a good value field width
(
    size_mode_t	mode,		// print mode
    int		min_fw		// minimum fw => return max(calc_fw,min_fw);
				// this value is also returned for invalid modes
)
{
    int fw = mode & (DC_SIZE_F_AUTO_UNIT|DC_SIZE_F_NO_UNIT) ? 0 : 4;

    switch ( mode & DC_SIZE_M_MODE )
    {
	case DC_SIZE_DEFAULT:
	case DC_SIZE_AUTO:
	    if ( !(mode & DC_SIZE_F_NO_UNIT) )
		fw = 4;
	    fw += 4;
	    break;

	case DC_SIZE_BYTES:	fw += 10; break;
	case DC_SIZE_K:		fw +=  7; break;
	case DC_SIZE_M:		fw +=  4; break;
	case DC_SIZE_G:		fw +=  3; break;
	case DC_SIZE_T:		fw +=  3; break;
	case DC_SIZE_P:		fw +=  3; break;
	case DC_SIZE_E:		fw +=  3; break;

	default:		fw = min_fw;
    }

    return fw > min_fw ? fw : min_fw;
}

///////////////////////////////////////////////////////////////////////////////
// [[PrintSize]]

char * PrintSize
(
    char		*buf,		// result buffer
					// NULL: use a local circulary static buffer
    size_t		buf_size,	// size of 'buf', ignored if buf==NULL
    u64			size,		// size to print
    sizeform_mode_t	sform_mode,	// output format, bit field
    size_mode_t		mode		// print mode
)
{
    DASSERT( DC_SIZE_N_MODES <= DC_SIZE_M_MODE + 1 );


    if (!buf)
	buf = GetCircBuf( buf_size = 20 );

    const bool force_1000 = ( mode & DC_SIZE_M_BASE ) == DC_SIZE_F_1000;

    // ??? [[2do]] follow 'sform_mode'
    const sizeform_mode_t aligned = sform_mode & DC_SFORM_ALIGN;

    switch ( mode & DC_SIZE_M_MODE )
    {
	//---- SI and IEC units

	case DC_SIZE_BYTES:
	    snprintf(buf,buf_size, aligned ? "%6llu B" : "%llu B", size );
	    break;

	case DC_SIZE_K:
	    if (force_1000)
		snprintf(buf,buf_size, aligned ? "%5llu kB" : "%llu kB",
			( size + KB_SI/2 ) / KB_SI );
	    else
		snprintf(buf,buf_size, aligned ? "%4llu KiB" : "%llu KiB",
			( size + KiB/2 ) / KiB );
	    break;

	case DC_SIZE_M:
	    if (force_1000)
		snprintf(buf,buf_size, aligned ? "%5llu MB" : "%llu MB",
			( size + MB_SI /2 ) / MB_SI  );
	    else
		snprintf(buf,buf_size, aligned ? "%4llu MiB" : "%llu MiB",
			( size + MiB /2 ) / MiB  );
	    break;

	case DC_SIZE_G:
	    if (force_1000)
		snprintf(buf,buf_size, aligned ? "%5llu GB" : "%llu GB",
			( size + GB_SI/2 ) / GB_SI );
	    else
		snprintf(buf,buf_size, aligned ? "%4llu GiB" : "%llu GiB",
			( size + GiB/2 ) / GiB );
	    break;

	case DC_SIZE_T:
	    if (force_1000)
		snprintf(buf,buf_size, aligned ? "%5llu TB" : "%llu TB",
			( size + TB_SI/2 ) / TB_SI );
	    else
		snprintf(buf,buf_size, aligned ? "%4llu TiB" : "%llu TiB",
			( size + TiB/2 ) / TiB );
	    break;

	case DC_SIZE_P:
	    if (force_1000)
		snprintf(buf,buf_size, aligned ? "%5llu PB" : "%llu PB",
			( size + PB_SI/2 ) / PB_SI );
	    else
		snprintf(buf,buf_size, aligned ? "%4llu PiB" : "%llu PiB",
			( size + PiB/2 ) / PiB );
	    break;

	case DC_SIZE_E:
	    if (force_1000)
		snprintf(buf,buf_size, aligned ? "%5llu EB" : "%llu EB",
			( size + EB_SI/2 ) / EB_SI );
	    else
		snprintf(buf,buf_size, aligned ? "%4llu EiB" : "%llu EiB",
			( size + EiB/2 ) / EiB );
	    break;


	//----- default == auto

	default:
	    buf = force_1000
			? PrintSize1000(buf,buf_size,size,sform_mode)
			: PrintSize1024(buf,buf_size,size,sform_mode);
	    if ( !(mode&DC_SIZE_F_NO_UNIT) )
		return buf;
    }

    if ( mode & (DC_SIZE_F_AUTO_UNIT|DC_SIZE_F_NO_UNIT) )
    {
	char * ptr = buf;
	while ( *ptr == ' ' )
	    ptr++;
	while ( *ptr && *ptr != ' ' )
	    ptr++;
	*ptr  = 0;
    }

    return buf;
}

///////////////////////////////////////////////////////////////////////////////
// [[PrintSize1000]]

char * PrintSize1000
(
    char		*buf,		// result buffer
					// NULL: use a local circulary static buffer
    size_t		buf_size,	// size of 'buf', ignored if buf==NULL
    u64			size,		// size to print
    sizeform_mode_t	sform_mode	// output format, bit field
)
{
    if (!buf)
	buf = GetCircBuf( buf_size = 20 );

    u64 num;
    size_mode_t unit;

    u64 mb = (size+MB_SI/2)/MB_SI; // maybe an overflow => extra if
    if ( mb < 10000 && size < EB_SI )
    {
	u64 kb = (size+KB_SI/2)/KB_SI;
	if ( kb < 10 )
	{
	    num  = size;
	    unit = DC_SIZE_BYTES;
	}
	else if ( kb < 10000 )
	{
	    num  = kb;
	    unit = DC_SIZE_K;
	}
	else
	{
	    num  = mb;
	    unit = DC_SIZE_M;
	}
    }
    else
    {
	mb = size / MB_SI; // recalc because of possible overflow
	u64 tb = (mb+MB_SI/2)/MB_SI;
	if ( tb < 10000 )
	{
	    if ( tb < 10 )
	    {
		num  = (mb+KB_SI/2)/KB_SI;
		unit = DC_SIZE_G;
	    }
	    else
	    {
		num  = tb;
		unit = DC_SIZE_T;
	    }
	}
	else
	{
	    u64 pb = (mb+GB_SI/2)/GB_SI;
	    if ( pb < 10000 )
	    {
		num  = pb;
		unit = DC_SIZE_P;
	    }
	    else
	    {
		num  = (mb+TB_SI/2)/TB_SI;
		unit = DC_SIZE_E;
	    }
	}
    }

    if	(  sform_mode & DC_SFORM_INC
	&& num
	&& !( num % 1000 )
	&& dc_size_tab_1000[unit+1]
	)
    {
	unit++;
	num /= 1000;
    }

    char unit_buf[8];
    char *dest = unit_buf;
    if ( !(sform_mode & DC_SFORM_NARROW) )
	*dest++ = ' ';

    if ( sform_mode & DC_SFORM_UNIT1 )
    {
	if ( unit != DC_SIZE_BYTES )
	    *dest++ = dc_size_tab_1000[unit][0];
	else if ( sform_mode & DC_SFORM_ALIGN )
	    *dest++ = ' ';
	else
	    dest = unit_buf; // not any char
	*dest = 0;
    }
    else if ( sform_mode & DC_SFORM_ALIGN )
	snprintf(dest,sizeof(unit_buf)-1,"%-3s",dc_size_tab_1000[unit]);
    else
	StringCopyS(dest,sizeof(unit_buf)-1,dc_size_tab_1000[unit]);

    if ( !num && unit == DC_SIZE_BYTES && sform_mode & DC_SFORM_DASH )
    {
	if ( sform_mode & DC_SFORM_ALIGN )
	    snprintf(buf,buf_size,"   -%s",unit_buf);
	else
	    StringCopyS(buf,buf_size,"-");
    }
    if ( sform_mode & DC_SFORM_ALIGN )
	snprintf(buf,buf_size,"%4llu%s",num,unit_buf);
    else
	snprintf(buf,buf_size,"%llu%s",num,unit_buf);

    return buf;
};

///////////////////////////////////////////////////////////////////////////////
// [[PrintSize1024]]

char * PrintSize1024
(
    char		*buf,		// result buffer
					// NULL: use a local circulary static buffer
    size_t		buf_size,	// size of 'buf', ignored if buf==NULL
    u64			size,		// size to print
    sizeform_mode_t	sform_mode	// output format, bit field
)
{
    if (!buf)
	buf = GetCircBuf( buf_size = 20 );

    u64 num;
    size_mode_t unit;

    u64 mib = (size+MiB/2)/MiB; // maybe an overflow => extra if
    if ( mib < 10000 && size < EiB )
    {
	u64 kib = (size+KiB/2)/KiB;
	if ( kib < 10 )
	{
	    num  = size;
	    unit = DC_SIZE_BYTES;
	}
	else if ( kib < 10000 )
	{
	    num  = kib;
	    unit = DC_SIZE_K;
	}
	else
	{
	    num  = mib;
	    unit = DC_SIZE_M;
	}
    }
    else
    {
	mib = size / MiB; // recalc because of possible overflow
	u64 tib = (mib+MiB/2)/MiB;
	if ( tib < 10000 )
	{
	    if ( tib < 10 )
	    {
		num  = (mib+KiB/2)/KiB;
		unit = DC_SIZE_G;
	    }
	    else
	    {
		num  = tib;
		unit = DC_SIZE_T;
	    }
	}
	else
	{
	    u64 pib = (mib+GiB/2)/GiB;
	    if ( pib < 10000 )
	    {
		num  = pib;
		unit = DC_SIZE_P;
	    }
	    else
	    {
		num  = (mib+TiB/2)/TiB;
		unit = DC_SIZE_E;
	    }
	}
    }

    if	(  sform_mode & DC_SFORM_INC
	&& num
	&& !( num & 0x3ff )
	&& dc_size_tab_1024[unit+1]
	)
    {
	unit++;
	num /= 0x400;
    }

    char unit_buf[8];
    char *dest = unit_buf;
    if ( !(sform_mode & DC_SFORM_NARROW) )
	*dest++ = ' ';

    if ( sform_mode & DC_SFORM_UNIT1 )
    {
	if ( unit != DC_SIZE_BYTES )
	    *dest++ = dc_size_tab_1000[unit][0];
	else if ( sform_mode & DC_SFORM_ALIGN )
	    *dest++ = ' ';
	else
	    dest = unit_buf; // not any char
	*dest = 0;
    }
    else if ( sform_mode & DC_SFORM_ALIGN )
	snprintf(dest,sizeof(unit_buf)-1,"%-3s",dc_size_tab_1024[unit]);
    else
	StringCopyS(dest,sizeof(unit_buf)-1,dc_size_tab_1024[unit]);

    if ( !num && unit == DC_SIZE_BYTES && sform_mode & DC_SFORM_DASH )
    {
	if ( sform_mode & DC_SFORM_ALIGN )
	    snprintf(buf,buf_size,"   -%s",unit_buf);
	else
	    StringCopyS(buf,buf_size,"-");
    }
    else if ( sform_mode & DC_SFORM_ALIGN )
	snprintf(buf,buf_size,"%4llu%s",num,unit_buf);
    else
	snprintf(buf,buf_size,"%llu%s",num,unit_buf);

    return buf;
};

//
///////////////////////////////////////////////////////////////////////////////
///////////////			   scan size			///////////////
///////////////////////////////////////////////////////////////////////////////

u64 (*ScanSizeFactorHook)
(
    char	ch_factor,		// char to analyze
    int		force_base		// if 1000|1024: force multiple of this
) = 0;

//-----------------------------------------------------------------------------

u64 ScanSizeFactor
(
    char	ch_factor,		// char to analyze
    int		force_base		// if 1000|1024: force multiple of this
)
{
    if (ScanSizeFactorHook)
    {
	u64 res = ScanSizeFactorHook(ch_factor,force_base);
	if (res)
	    return res;
    }

    if ( force_base == 1000 )
    {
	switch (ch_factor)
	{
	    case 'b': case 'c': return     1;
	    case 'k': case 'K': return KB_SI;
	    case 'm': case 'M': return MB_SI;
	    case 'g': case 'G': return GB_SI;
	    case 't': case 'T': return TB_SI;
	    case 'p': case 'P': return PB_SI;
	    case 'e': case 'E': return EB_SI;
	}
    }
    else if ( force_base == 1024 )
    {
	switch (ch_factor)
	{
	    case 'b': case 'c': return   1;
	    case 'k': case 'K': return KiB;
	    case 'm': case 'M': return MiB;
	    case 'g': case 'G': return GiB;
	    case 't': case 'T': return TiB;
	    case 'p': case 'P': return PiB;
	    case 'e': case 'E': return EiB;
	}
    }
    else
    {
	switch (ch_factor)
	{
	    case 'b':
	    case 'c': return 1;

	    case 'k': return KB_SI;
	    case 'm': return MB_SI;
	    case 'g': return GB_SI;
	    case 't': return TB_SI;
	    case 'p': return PB_SI;
	    case 'e': return EB_SI;

	    case 'K': return KiB;
	    case 'M': return MiB;
	    case 'G': return GiB;
	    case 'T': return TiB;
	    case 'P': return PiB;
	    case 'E': return EiB;
	}
    }

    return 0;
}

///////////////////////////////////////////////////////////////////////////////

char * ScanSizeTerm
(
    double	*num,			// not NULL: store result
    ccp		source,			// source text
    u64		default_factor,		// use this factor if number hasn't one
    int		force_base		// if 1000|1024: force multiple of this
)
{
    ASSERT(source);

    char * end;
    double d = strtod(source,&end);
    if ( end > source )
    {
	// something was read

	if ( *end == '/' )
	{
	    const double div = strtod(end+1,&end);
	    if ( div > 0.0 )
		d /= div;
	}

	u64 factor = ScanSizeFactor(*end,force_base);
	if (factor)
	    end++;
	else
	    factor = default_factor;

	if (factor)
	    d *= factor;
	else
	    end = (char*)source;
    }

    if (num)
	*num = d;

    return end;
}

///////////////////////////////////////////////////////////////////////////////

char * ScanSize
(
    double	*num,			// not NULL: store result
    ccp		source,			// source text
    u64		default_factor,		// use this factor if number hasn't one
    u64		default_factor_add,	// use this factor for summands
    int		force_base		// if 1000|1024: force multiple of this
)
{
    DASSERT(source);
    TRACE("ScanSize(df=%llx,%llx, base=%u)\n",
			default_factor, default_factor_add, force_base );

    double sum = 0.0;
    bool add = true;
    char * end = 0;
    for(;;)
    {
	double term;
	end = ScanSizeTerm(&term,source,default_factor,force_base);
	if ( end == source )
	    break;

	if (add)
	    sum += term;
	else
	    sum -= term;

	while ( *end > 0 && *end <= ' ' )
	    end++;

	if ( *end == '+' )
	    add = true;
	else if ( *end == '-' )
	    add = false;
	else
	    break;

	source = end+1;
	while ( *source > 0 && *source <= ' ' )
	    source++;

	if ( !*source && default_factor_add )
	{
	    if (add)
		sum += default_factor_add;
	    else
		sum -= default_factor_add;
	    end = (char*)source;
	    break;
	}

	default_factor = default_factor_add;
    }

    if (num)
	*num = sum;

    return end;
}

///////////////////////////////////////////////////////////////////////////////

char * ScanSizeU32
(
    u32		*num,			// not NULL: store result
    ccp		source,			// source text
    u64		default_factor,		// use this factor if number hasn't one
    u64		default_factor_add,	// use this factor for summands
    int		force_base		// if 1000|1024: force multiple of this
)
{
    double d;
    char * end = ScanSize(&d,source,default_factor,default_factor_add,force_base);
    if ( d < 0 || d > ~(u32)0 )
	end = (char*)source;
    else if (num)
	*num = (u32)d;

    return end;
}

///////////////////////////////////////////////////////////////////////////////

char * ScanSizeU64
(
    u64		*num,			// not NULL: store result
    ccp		source,			// source text
    u64		default_factor,		// use this factor if number hasn't one
    u64		default_factor_add,	// use this factor for summands
    int		force_base		// if 1000|1024: force multiple of this
)
{
    double d;
    char * end = ScanSize(&d,source,default_factor,default_factor_add,force_base);
    if ( d < 0 || d > ~(u64)0 )
	end = (char*)source;
    else if (num)
	*num = (u64)d;

    return end;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

char * ScanSizeRange
(
    int		*stat,			// if not NULL: store result
					//	0:none, 1:single, 2:range
    double	*num1,			// not NULL: store 'from' result
    double	*num2,			// not NULL: store 'to' result

    ccp		source,			// source text
    u64		default_factor,		// use this factor if number hasn't one
    u64		default_factor_add,	// use this factor for summands
    int		force_base,		// if 1000|1024: force multiple of this
    double	max_value		// >0: max value for open ranges
)
{
    int count = 0;
    double d1, d2;

    char *end = ScanSize(&d1,source,default_factor,default_factor_add,force_base);
    if ( end == source )
    {
	d1 = d2 = 0.0;
	count = 0;
    }
    else
    {
	count++;
	d2 = d1;
	if ( *end == '#' || *end == ':' )
	{
	    source = end;
	    end = ScanSize(&d2,source+1,default_factor,default_factor_add,force_base);
	    if ( end == source+1 )
	    {
		if ( *source == ':' && max_value > 0.0 )
		{
		    count++;
		    d2 = max_value;
		    if ( *end == '*' )
			end++;
		}
		else
		{
		    end--;
		    d2 = d1;
		}
	    }
	    else
	    {
		count++;
		if ( *source == '#' )
		    d2 += d1-1;
	    }
	    if ( d2 < d1 )
		d2 = d1;
	}
    }

    if (stat)
	*stat = count;
    if (num1)
	*num1 = d1;
    if (num2)
	*num2 = d2;
    return end;
}

///////////////////////////////////////////////////////////////////////////////

char * ScanSizeRangeU32
(
    int		*stat,			// if not NULL: store result
					//	0:none, 1:single, 2:range
    u32		*num1,			// not NULL: store 'from' result
    u32		*num2,			// not NULL: store 'to' result

    ccp		source,			// source text
    u64		default_factor,		// use this factor if number hasn't one
    u64		default_factor_add,	// use this factor for summands
    int		force_base,		// if 1000|1024: force multiple of this
    u32		max_value		// >0: max value for open ranges
)
{
    double d1, d2;
    char * end = ScanSizeRange(stat,&d1,&d2,source,
			default_factor,default_factor_add,force_base,
			max_value>0 ? max_value : U32_MAX );

    if ( d1 < 0 || d1 > U32_MAX || d2 < 0 || d2 > U32_MAX )
	end = (char*)source;
    else
    {
	if (num1)
	    *num1 = (u32)d1;
	if (num2)
	    *num2 = (u32)d2;
    }

    return end;

}

///////////////////////////////////////////////////////////////////////////////

char * ScanSizeRangeU64
(
    int		*stat,			// if not NULL: store result
					//	0:none, 1:single, 2:range
    u64		*num1,			// not NULL: store 'from' result
    u64		*num2,			// not NULL: store 'to' result

    ccp		source,			// source text
    u64		default_factor,		// use this factor if number hasn't one
    u64		default_factor_add,	// use this factor for summands
    int		force_base,		// if 1000|1024: force multiple of this
    u64		max_value		// >0: max value for open ranges
)
{
    double d1, d2;
    char * end = ScanSizeRange(stat,&d1,&d2,source,
			default_factor,default_factor_add,force_base,
			max_value>0 ? max_value : U64_MAX );

    if ( d1 < 0 || d1 > U64_MAX || d2 < 0 || d2 > U64_MAX )
	end = (char*)source;
    else
    {
	if (num1)
	    *num1 = (u64)d1;
	if (num2)
	    *num2 = (u64)d2;
    }

    return end;

}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

char * ScanSizeRangeList
(
    uint	*n_range,		// not NULL: store number of scanned ranges
    double	*num,			// array with '2*max_range' elements
					//	unused elements are filled with 0.0
    uint	max_range,		// max number of allowed ranges

    ccp		source,			// source text
    u64		default_factor,		// use this factor if number hasn't one
    u64		default_factor_add,	// use this factor for summands
    int		force_base,		// if 1000|1024: force multiple of this
    double	max_value		// >0: max value for open ranges
)
{
    DASSERT(num);

    uint i = 0;
    char *ptr = (char*)source;

    while ( i < max_range )
    {
	while ( *ptr > 0 && *ptr <= ' ' || *ptr == ',' )
	    ptr++;

	int stat;
	ptr = ScanSizeRange( &stat, num, num+1, ptr,
			default_factor, default_factor_add, force_base, max_value );
	if (!stat)
	    break;
	num += 2;
	i++;

	char *ptr2 = ptr;
	while ( *ptr2 > 0 && *ptr2 <= ' ' )
	    ptr2++;
	if ( *ptr2 != ',' )
	    break;

	ptr = ptr2 + 1;
    }

    if (n_range)
	*n_range = i;

    while ( i < max_range )
    {
	*num++ = 0.0;
	*num++ = 0.0;
	i++;
    }

    return ptr;
}

///////////////////////////////////////////////////////////////////////////////

char * ScanSizeRangeListU32
(
    uint	*n_range,		// not NULL: store number of scanned ranges
    u32		*num,			// array with '2*max_range' elements
					//	unused elements are filled with 0
    uint	max_range,		// max number of allowed ranges

    ccp		source,			// source text
    u32		default_factor,		// use this factor if number hasn't one
    u32		default_factor_add,	// use this factor for summands
    int		force_base,		// if 1000|1024: force multiple of this
    u32		max_value		// >0: max value for open ranges
)
{
    DASSERT(num);

    const uint max = 100;
    double d_buf[2*max];
    double *d = max_range < max ? d_buf : MALLOC(max_range*2*sizeof(double));

    char *end = ScanSizeRangeList( n_range, d, max_range,
			source, default_factor, default_factor_add,
			force_base, max_value );

    uint i;
    for ( i = 0; i < 2*max_range; i++ )
	num[i] = d[i] < 0.0 ? 0.0 : d[i] < U32_MAX ? d[i] : U32_MAX;

    if ( d != d_buf )
	FREE(d);
    return end;
}

///////////////////////////////////////////////////////////////////////////////

char * ScanSizeRangeListU64
(
    uint	*n_range,		// not NULL: store number of scanned ranges
    u64		*num,			// array with '2*max_range' elements
					//	unused elements are filled with 0
    uint	max_range,		// max number of allowed ranges

    ccp		source,			// source text
    u64		default_factor,		// use this factor if number hasn't one
    u64		default_factor_add,	// use this factor for summands
    int		force_base,		// if 1000|1024: force multiple of this
    u64		max_value		// >0: max value for open ranges
)
{
    DASSERT(num);

    const uint max = 100;
    double d_buf[2*max];
    double *d = max_range < max ? d_buf : MALLOC(max_range*2*sizeof(double));

    char *end = ScanSizeRangeList( n_range, d, max_range,
			source, default_factor, default_factor_add,
			force_base, max_value );

    uint i;
    for ( i = 0; i < 2*max_range; i++ )
	num[i] = d[i] < 0.0 ? 0.0 : d[i] < U64_MAX ? d[i] : U64_MAX;

    if ( d != d_buf )
	FREE(d);
    return end;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

enumError ScanSizeOpt
(
    double	*num,			// not NULL: store result
    ccp		source,			// source text
    u64		default_factor,		// use this factor if number hasn't one
    u64		default_factor_add,	// use this factor for summands
    int		force_base,		// if 1000|1024: force multiple of this

    ccp		opt_name,		// NULL or name of option for error messages
    u64		min,			// minimum allowed value
    u64		max,			// maximum allowed value
    bool	print_err		// true: print error messages
)
{
    double d;
    char * end = ScanSize(&d,source,default_factor,default_factor_add,force_base);

 #ifdef DEBUG
    {
	u64 size = d;
	TRACE("--%s %8.6g ~ %llu ~ %llu GiB ~ %llu GB\n",
		opt_name, d, size, (size+GiB/2)/GiB, (size+500000000)/1000000000 );
    }
 #endif

    enumError err = ERR_OK;

    if ( source == end || *end )
    {
	err = ERR_SYNTAX;
	if (print_err)
	    ERROR0(ERR_SYNTAX,
			"Illegal number for option --%s: %s\n",
			opt_name, source );
    }
    else if ( min > 0 && d < min )
    {
	err = ERR_SYNTAX;
	if (print_err)
	    ERROR0(ERR_SEMANTIC,
			"Value of --%s too small (must not <%llu): %s\n",
			opt_name, min, source );
    }
    else if ( max > 0 && d > max )
    {
	err = ERR_SYNTAX;
	if (print_err)
	    ERROR0(ERR_SEMANTIC,
			"Value of --%s too large (must not >%llu): %s\n",
			opt_name, max, source );
    }

    if ( num && !err )
	*num = d;
    return err;
}

///////////////////////////////////////////////////////////////////////////////

enumError ScanSizeOptU64
(
    u64		*num,			// not NULL: store result
    ccp		source,			// source text
    u64		default_factor,		// use this factor if number hasn't one
    int		force_base,		// if 1000|1024: force multiple of this

    ccp		opt_name,		// NULL or name of option for error messages
    u64		min,			// >0: minimum allowed value
    u64		max,			// >0: maximum allowed value
    u32		multiple,		// >0: result must be multiple
    u32		pow2,			// >0: result must power of '1<<pow2'
    bool	print_err		// true: print error messages
)
{
    if (!max)
	max = ~(u64)0;

    if ( pow2 && !force_base )
    {
	// try base 1024 first without error messages
	u64 val;
	if (!ScanSizeOptU64( &val, source, default_factor, 1024,
				opt_name, min,max, multiple, pow2, false ))
	{
	    if (num)
		*num = val;
	    return ERR_OK;
	}
    }

    double d = 0.0;
    enumError err = ScanSizeOpt(&d,source,default_factor,
				multiple ? multiple : 1,
				force_base,opt_name,min,max,print_err);

    u64 val;
    if ( d < 0.0 )
    {
	val = 0;
	err = ERR_SEMANTIC;
	if (print_err)
	    ERROR0(ERR_SEMANTIC, "--%s: negative values not allowed: %s\n",
			opt_name, source );
    }
    else
	val = d;

    if ( err == ERR_OK && pow2 > 0 )
    {
	int shift_count = 0;
	u64 shift_val = val;
	if (val)
	{
	    while (!(shift_val&1))
	    {
		shift_count++;
		shift_val >>= 1;
	    }
	}

	if ( shift_val != 1 || shift_count/pow2*pow2 != shift_count )
	{
	    err = ERR_SEMANTIC;
	    if (print_err)
		ERROR0(ERR_SYNTAX,
			"--%s: value must be a power of %d but not %llu\n",
			opt_name, 1<<pow2, val );
	}
    }

    if ( err == ERR_OK && multiple > 1 )
    {
	u64 xval = val / multiple * multiple;
	if ( xval != val )
	{
	    if ( min > 0 && xval < min )
		xval += multiple;

	    if (print_err)
		ERROR0(ERR_WARNING,
		    "--%s: value must be a multiple of %u -> use %llu instead of %llu.\n",
		    opt_name, multiple, xval, val );
	    val = xval;
	}
    }

    if ( num && !err )
	*num = val;
    return err;
}

///////////////////////////////////////////////////////////////////////////////

enumError ScanSizeOptU32
(
    u32		*num,			// not NULL: store result
    ccp		source,			// source text
    u64		default_factor,		// use this factor if number hasn't one
    int		force_base,		// if 1000|1024: force multiple of this

    ccp		opt_name,		// NULL or name of option for error messages
    u64		min,			// >0: minimum allowed value
    u64		max,			// >0: maximum allowed value
    u32		multiple,		// >0: result must be multiple
    u32		pow2,			// >0: result must power of '1<<pow2'
    bool	print_err		// true: print error messages
)
{
    if ( !max || max > ~(u32)0 )
	max = ~(u32)0;

    u64 val = 0;
    enumError err = ScanSizeOptU64( &val, source, default_factor, force_base,
				opt_name, min, max, multiple, pow2, print_err );

    if ( num && !err )
	*num = (u32)val;
    return err;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// strto*() like wrappers

u32 GetSizeU32	( ccp src, char **end, int force_base )
{
    u32 num = 0;
    char *ptr = ScanSizeU32(&num,src,1,1,force_base);
    if (end)
	*end = ptr;
    return num;
}

///////////////////////////////////////////////////////////////////////////////

u64 GetSizeU64	( ccp src, char **end, int force_base )
{
    u64 num = 0;
    char *ptr = ScanSizeU64(&num,src,1,1,force_base);
    if (end)
	*end = ptr;
    return num;
}

///////////////////////////////////////////////////////////////////////////////

double GetSizeD	( ccp src, char **end, int force_base )
{
    double num = 0.0;
    char *ptr = ScanSize(&num,src,1,1,force_base);
    if (end)
	*end = ptr;
    return num;
}

//
///////////////////////////////////////////////////////////////////////////////
///////////////			Scan Duration			///////////////
///////////////////////////////////////////////////////////////////////////////

char * ScanSIFactor
(
    // returns end of scanned string

    double	*num,			// not NULL: store result
    ccp		source,			// source text
    double	default_factor		// return this if no factor found
)
{
    DASSERT(source);
    switch (*(uchar*)source++)
    {
	case 'y': default_factor = 1e-24; break;
	case 'z': default_factor = 1e-21; break;
	case 'a': default_factor = 1e-18; break;
	case 'f': default_factor = 1e-15; break;
	case 'p': default_factor = 1e-12; break;
	case 'n': default_factor = 1e-9; break;
	case 0xb5: // � as ANSI
	case 'u': default_factor = 1e-6; break;
	case 'm': default_factor = 1e-3; break;
	case 'c': default_factor = 1e-2; break;
	case 'd': default_factor = 1e-1; break;

	case 'h': default_factor = 1e2; break;
	case 'k': default_factor = 1e3; break;
	case 'M': default_factor = 1e6; break;
	case 'G': default_factor = 1e9; break;
	case 'T': default_factor = 1e12; break;
	case 'P': default_factor = 1e15; break;
	case 'E': default_factor = 1e18; break;
	case 'Z': default_factor = 1e21; break;
	case 'Y': default_factor = 1e24; break;

	case 0xc2:
	    if ( (uchar)*source == 0xb5 )
	    {
		// � as UTF-8
		source++;
		default_factor = 1e-6;
		break;
	    }
	    else
		source--;
	    break;

	default:
	   source--;
    }
    if (num)
	*num = default_factor;
    return (char*)source;
}

///////////////////////////////////////////////////////////////////////////////

double GetDurationFactor ( char ch )
{
    switch (ch)
    {
	case 's': return 1.0; break;
	case 'm': return 60.0; break;
	case 'h': return 3600.0; break;
	case 't':
	case 'd': return 86400.0; break;
	case 'w': return 7*86400.0; break;
	case 'j':
	case 'y': return 365*86400.0; break;
	default:  return 0.0;
    }
}

///////////////////////////////////////////////////////////////////////////////

char * ScanDuration
(
    double		*num,		// not NULL: store result
    ccp			source,		// source text
    double		default_factor,	// default factor if no SI unit found
    ScanDuration_t	mode
)
{
    if (!source)
	return 0;

    ccp src = source;
    if ( mode & SDUMD_SKIP_BLANKS )
	while ( *src == ' ' || *src == '\t' )
	    src++;

    if ( mode & SDUMD_ALLOW_LIST )
    {
	bool valid = false;
	mode &= ~(SDUMD_SKIP_BLANKS|SDUMD_ALLOW_LIST);
	double sum = 0.0;
	bool sub = false;
	for(;;)
	{
	    double val;
	    ccp end = ScanDuration(&val,src,default_factor,mode);
	    if ( src == end )
		break;
	    if (sub)
	    {
		sub = false;
		sum -= val;
	    }
	    else
		sum += val;
	    src = end;
	    valid = true;

	    if ( mode & SDUMD_ALLOW_SPACE_SEP )
		while ( *src == ' ' || *src == '\t' )
		    src++;
	    if ( *src == '+' && mode & SDUMD_ALLOW_PLUS_SEP )
		src++;
	    else if ( *src == '-' && mode & SDUMD_ALLOW_MINUS_SEP )
	    {
		src++;
		sub = true;
	    }
	    mode |= SDUMD_SKIP_BLANKS;
	}
	if (num)
	    *num = sum;
	return (char*)( valid ? src : source );
    }


    //--- single scan

    char *end;
    double val = strtod(src,&end);
    if ( end > src )
    {
	if ( *end == ':' && mode & SDUMD_ALLOW_COLON )
	{
	    // 'H:M' or 'H:M:S'
	    src = end + 1;
	    val *= 3600;
	    double min = strtod(src,&end);
	    if ( end > src )
	    {
		val += 60*min;
		src = end;
		if ( *src == ':' )
		{
		    // 'H:M:S'
		    double sec = strtod(++src,&end);
		    if ( end > src )
		    {
			src = end;
			val += sec;
		    }
		}
	    }
	}
	else
	{
	    if ( *end == '/' && mode & SDUMD_ALLOW_DIV )
	    {
		const double div = strtod(end+1,&end);
		if ( div > 0.0 )
		    val /= div;
	    }
	    src = end;

	    double si_factor;
	    end = ScanSIFactor(&si_factor,src,1.0);

	    double unit = GetDurationFactor(*end);
	    if (unit)
	    {
		val *= unit * si_factor;
		src = end+1;
	    }
	    else
	    {
		unit = GetDurationFactor(*src);
		if (unit)
		{
		    val *= unit;
		    src++;
		}
		else
		{
		    val *= (end>src) ? si_factor : default_factor;
		    src = end;
		}
	    }
	}
	if (num)
	    *num = val;
	return (char*)src;
    }

    if (num)
	*num = 0.0;
    return (char*)source;
}

//
///////////////////////////////////////////////////////////////////////////////
///////////////			PrintIP4*()			///////////////
///////////////////////////////////////////////////////////////////////////////

char * PrintIP4A
(
    // print in A-notation

    char	*buf,		// result buffer
				// NULL: use a local circulary static buffer
    size_t	buf_size,	// size of 'buf', ignored if buf==NULL
    u32		ip4,		// IP4 to print
    s32		port		// 0..0xffff: add ':port'
)
{
    if (!buf)
	buf = GetCircBuf( buf_size = FW_IPV4_A_PORT+1 );

    u8 addr[4];
    write_be32(addr,ip4);

    if ( port >= 0 && port <= 0xffff )
	snprintf(buf,buf_size,"%u.%u:%u",
		addr[0], ip4 & 0xffffff, port );
    else
	snprintf(buf,buf_size,"%u.%u",
		addr[0], ip4 & 0xffffff );
    return buf;
}

///////////////////////////////////////////////////////////////////////////////

char * PrintIP4B
(
    // print in A-notation

    char	*buf,		// result buffer
				// NULL: use a local circulary static buffer
    size_t	buf_size,	// size of 'buf', ignored if buf==NULL
    u32		ip4,		// IP4 to print
    s32		port		// 0..0xffff: add ':port'
)
{
    if (!buf)
	buf = GetCircBuf( buf_size = FW_IPV4_B_PORT+1 );

    u8 addr[4];
    write_be32(addr,ip4);

    if ( port >= 0 && port <= 0xffff )
	snprintf(buf,buf_size,"%u.%u.%u:%u",
		addr[0], addr[1], ip4 & 0xffff, port );
    else
	snprintf(buf,buf_size,"%u.%u.%u",
		addr[0], addr[1], ip4 & 0xffff );
    return buf;
}

///////////////////////////////////////////////////////////////////////////////

char * PrintIP4
(
    char	*buf,		// result buffer
				// NULL: use a local circulary static buffer
    size_t	buf_size,	// size of 'buf', ignored if buf==NULL
    u32		ip4,		// IP4 to print
    s32		port		// 0..0xffff: add ':port'
)
{
    if (!buf)
	buf = GetCircBuf( buf_size = FW_IPV4_C_PORT+1 );

    u8 addr[4];
    write_be32(addr,ip4);

    if ( port >= 0 && port <= 0xffff )
	snprintf(buf,buf_size,"%u.%u.%u.%u:%u",
		addr[0], addr[1], addr[2], addr[3], port );
    else
	snprintf(buf,buf_size,"%u.%u.%u.%u",
		addr[0], addr[1], addr[2], addr[3] );
    return buf;
}

///////////////////////////////////////////////////////////////////////////////

char * PrintLeftIP4
(
    char	*buf,		// result buffer
				// NULL: use a local circulary static buffer
    size_t	buf_size,	// size of 'buf', ignored if buf==NULL
    u32		ip4,		// IP4 to print
    s32		port		// 0..0xffff: add ':port'
)
{
    if (!buf)
	buf = GetCircBuf( buf_size = FW_IPV4_C_PORT+1 );

    u8 addr[4];
    write_be32(addr,ip4);
    uint len, fw;

    if ( port >= 0 && port <= 0xffff )
    {
	fw = 21;
	len = snprintf(buf,buf_size,"%u.%u.%u.%u:%u",
			addr[0], addr[1], addr[2], addr[3], port );
    }
    else
    {
	fw = 15;
	len = snprintf(buf,buf_size,"%u.%u.%u.%u",
			addr[0], addr[1], addr[2], addr[3] );
    }

    char *dest = buf + len;
    char *end  = buf + ( fw < buf_size ? fw : buf_size );
    while ( dest < end )
	*dest++ = ' ';
    *dest = 0;

    return buf;
}

///////////////////////////////////////////////////////////////////////////////

char * PrintRightIP4
(
    char	*buf,		// result buffer
				// NULL: use a local circulary static buffer
    size_t	buf_size,	// size of 'buf', ignored if buf==NULL
    u32		ip4,		// IP4 to print
    s32		port,		// 0..0xffff: add ':port'
    uint	port_mode	// 0: 'ip:port', 1: 'ip :port', 2: 'ip : port'
)
{
    if (!buf)
	buf = GetCircBuf( buf_size = 24 );

    u8 addr[4];
    write_be32(addr,ip4);

    char tempbuf[16];
    snprintf(tempbuf,sizeof(tempbuf),"%u.%u.%u.%u",
			addr[0], addr[1], addr[2], addr[3] );

    if ( port >= 0 && port <= 0xffff )
    {
	if (!port_mode)
	    snprintf(buf,buf_size,"%15s:%-5u",tempbuf,port);
	else if ( port_mode == 1 )
	    snprintf(buf,buf_size,"%15s :%-5u",tempbuf,port);
	else
	    snprintf(buf,buf_size,"%15s : %-5u",tempbuf,port);
    }
    else
	snprintf(buf,buf_size,"%15s",tempbuf);

    return buf;
}

///////////////////////////////////////////////////////////////////////////////

char * PrintAlignedIP4
(
    char	*buf,		// result buffer
				// NULL: use a local circulary static buffer
    size_t	buf_size,	// size of 'buf', ignored if buf==NULL
    u32		ip4,		// IP4 to print
    s32		port		// 0..0xffff: add ':port'
)
{
    if (!buf)
	buf = GetCircBuf( buf_size = FW_IPV4_C_PORT+1 );

    u8 addr[4];
    write_be32(addr,ip4);

    if ( port >= 0 && port <= 0xffff )
	snprintf(buf,buf_size,"%3u.%3u.%3u.%3u:%5u",
		addr[0], addr[1], addr[2], addr[3], port );
    else
	snprintf(buf,buf_size,"%3u.%3u.%3u.%3u",
		addr[0], addr[1], addr[2], addr[3] );
    return buf;
}

//
///////////////////////////////////////////////////////////////////////////////
///////////////				CRC16			///////////////
///////////////////////////////////////////////////////////////////////////////

void CreateCRC16Table ( u16 table[0x100], u16 polynom )
{
    int i, j;
    for ( i = 0; i < 256; i++ )
    {
	u16 val = i << 8;
	for ( j = 0; j < 8; j++ )
	{
	    if ( val & 0x8000 )
		val = val << 1 ^ polynom;
	    else
		val <<= 1;
	}
	table[i] = val;
    }
}

//-----------------------------------------------------------------------------

const u16 * GetCRC16Table ( u16 polynom )
{
    static u16 *table = 0;
    static u16 active_polynom = 0;

    if ( !table || active_polynom != polynom )
    {
	if (!table)
	    table = MALLOC(sizeof(u16)*0x100);
	CreateCRC16Table(table,polynom);
	active_polynom = polynom;
    }
    return table;
}

//-----------------------------------------------------------------------------

u16 CalcCRC16 ( cvp data, uint data_size, u16 polynom, u16 preset )
{
    DASSERT( data || !data_size );

    register u16 crc = preset;
    if ( data_size > 0 )
    {
	const u16 *table = GetCRC16Table(polynom);
	const u8 *ptr = data;
	while ( data_size-- > 0 )
	    crc = table[ ( *ptr++ ^ crc >> 8 ) & 0xff ] ^ crc << 8;
    }
    return crc;
}

//
///////////////////////////////////////////////////////////////////////////////
///////////////			    misc			///////////////
///////////////////////////////////////////////////////////////////////////////

float double2float ( double d )
{
    // reduce precision
    return d;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

ccp PrintNiceID4 ( uint id )
{
    if (!id)
	return "----";

    char *buf = GetCircBuf(5);
    DASSERT(buf);

 #if 0 // the second solution is much faster!

    buf[4]  = 0;
    div_t d = div( (int)( id % (26*26*100) ), 10 );
    buf[3]  = '0' + d.rem;
    d       = div(d.quot,10);
    buf[2]  = '0' + d.rem;
    d       = div(d.quot,26);
    buf[1]  = 'A' + d.rem;
    buf[0]  = 'A' + d.quot;

 #else

    buf[0] = 'A' + id / 2600 % 26;
    buf[1] = 'A' + id /  100 % 26;
    buf[2] = '0' + id /   10 % 10;
    buf[3] = '0' + id        % 10;
    buf[4] = 0;

 #endif

    return buf;
}

///////////////////////////////////////////////////////////////////////////////

char * ScanNiceID4 ( int * res, ccp arg )
{
    DASSERT(res);
    *res = -1;

    if (!arg)
	return 0;

    char ch = tolower((int)arg[0]);
    if ( ch >= 'a' && ch <= 'z' )
    {
	int num = ch - 'a';
	ch = tolower((int)arg[1]);
	if ( ch >= 'a' && ch <= 'z' )
	{
	    num = 26 * num + ch - 'a';
	    ch = arg[2];
	    if ( ch >= '0' && ch <= '9' )
	    {
		num = 10 * num + ch - '0';
		ch = arg[3];
		if ( ch >= '0' && ch <= '9' )
		{
		    *res = 10 * num + ch - '0';
		    return (char*)arg+4;
		}
	    }
	}
    }
    return (char*)arg;
};

//
///////////////////////////////////////////////////////////////////////////////
///////////////				END			///////////////
///////////////////////////////////////////////////////////////////////////////

