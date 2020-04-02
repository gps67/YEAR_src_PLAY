/*
	ARMv6 required for unaligned access LDR
	otherwise it rotates within the word!
*/

#include <stdio.h>

typedef char      i8;
typedef int       i32;
typedef long long i64;

typedef unsigned char      u8;
typedef unsigned int       u32;
typedef unsigned long long u64;

#if 0
typedef unsigned i8  u8;
typedef unsigned i32 u32;
typedef unsigned i64 u64;
#endif

#include "ASM_byte_swap.h"
#include <byteswap.h>

void show_byte( const u8 val )
{
	printf("%c", val & 0xFF );
}

int show_word( u32 val )
{
	ASM_byte_swap_in_situ(val);
	show_byte( val >> 24 );
	show_byte( val >> 16 );
	show_byte( val >> 8 );
	show_byte( val );
	printf("\n" );
	return 0;
}

int show_word_64( u64 val )
{
	// ASM_byte_swap_in_situ_64(val);
	val = bswap_64(val);
	show_byte( val >> (8*7) );
	show_byte( val >> (8*6) );
	show_byte( val >> (8*5) );
	show_byte( val >> (8*4) );

	show_byte( val >> (8*3) );
	show_byte( val >> (8*2) );
	show_byte( val >> (8*1) );
	show_byte( val );
	printf("\n" );
	return 0;
}

u32 fetch32( u32 * ptr )
{
	return * ptr;
}

u64 fetch_64( u64 * ptr )
{
	return * ptr;
}

int main(int argc, char ** argv )
{
	const u8 * str = (const u8 *) "@abcdefghijklm";
	const u8 * str_1 = str + 1;
	const u8 * str_2 = str + 2;
	u32 * p1 = (u32 *) str_1;
	u32 * p2 = (u32 *) str_2;
	u32 w1 = fetch32( p1 );
	u32 w2 = fetch32( p2 );

	show_word( w1 );
	show_word( w2 );

	u64 * q1 = (u64*) p1;
	u64 * q2 = (u64*) p2;
	u64 w3 = fetch_64( q1 );
	u64 w4 = fetch_64( q2 );

	show_word_64( w3 );
	show_word_64( w4 );

	return 0;
}
