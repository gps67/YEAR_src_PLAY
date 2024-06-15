/*
	ARMv6 required for unaligned access LDR
	otherwise it rotates within the word!

	This loads a 32 bit word from aligned and from odd address
	prints out the u32_WORDS as ABCD or as DCBA

	IT also tests the SWAPB _32 and _64
	by printing out FILE_TEXT_BYTES "@abcdefghijklm" more than 8 shifted
	
./test_align.exec
show_word_32         0x64636261 abcd
show_word_32         0x65646362 bcde
show_word_64 0x6867666564636261 abcdefgh
show_word_64 0x6968676665646362 bcdefghi
(END)

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

#define USE_ASM 0 // 1

int show_word( u32 val )
{
	printf("show_word_32         0x%08X ", val ); // u32 is default on linux
#if USE_ASM
	ASM_byte_swap_in_situ_32(val);
#else
	val = bswap_32(val);
#endif
	show_byte( val >> 24 );
	show_byte( val >> 16 );
	show_byte( val >> 8 );
	show_byte( val );
	printf("\n" );
	return 0;
}

int show_word_64( u64 val )
{
	printf("show_word_64 0x%016LX ", val ); // u64 needs % l
#if USE_ASM
	ASM_byte_swap_in_situ_64(val);
#else
	val = bswap_64(val);
#endif
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
