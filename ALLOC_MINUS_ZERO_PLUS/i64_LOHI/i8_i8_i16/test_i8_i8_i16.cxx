/*

	YET another attempt at BITFIELD BYTE_FIELD

	SEE LOHI expectations and rhyme with x86_32 and x86_64

	BYTE_A
	BYTE_B
	u16_PAYLOAD	AVAR_in_STO  or RUN GETTERC()

	AVAR_in_STO
		_in_MEM
		_in_FILE
		_in_HEAP
		_in_API

		_in_MMAP { // base class for all of above // SCRIPT INHERIT 

			in_HEAP in_SEGMENT in_FILE

			in_ELF
				ROM - yes
				STO - HOW ?

			in_SCRIPT
				CALL_ELF
				MMAP_NOT_ELF

					Merge all SEGMENTS into SEGMENTS

				OUTER MMAP SEGMENT PROVIDER

					OUTER += MINI_MACHINE "OUTER_MMAP_PROVIDER"
					 VFS += actual_FS
					 DIR += FILE // problems with 
					 // overf amiliarity "DIR" must overlap
					 // overlap IDENT in SCOPE //
					 // simplest SPELLING // score = BASE + FILTERED
					 SEGMENT == MMAP_entire_FILE

					 OUTER HEAP FILE == MMAP_FILE

					  SIMPLE_ROM_HEAPS are SINGLE_SEGMENT
					  RUNTIME_STO_HEAPS are 4 segments then GC
					  _ZONE_0	N_PAGES
					  _ZONE_1	N_PAGES
					  _ZONE_2	N_PAGES
					  _ZONE_3	N_PAGES

					  SESSION grows _0 _1 _2 _3
					  OUTSIDE grows to future_avail_now

					  	N_BYTES is PLENTY 
						OUTSIDE notices activity
						and pre-allocs PLENTY

						but that also changes to
						PLENTY_MORE

						but by now, we have already
						ALLOC SEGMENT as MMAP_REGION

						MMAP_REGION is an MMAP
						MMAP_inside_an_MMAP
			CLASS N_PAGES {
				const PAGE_nbytes == 4096 ;
				int N;	// often i5 32
				u8 * PAGE_ZERO; 
				typedef char PAGE_BYTE[ PAGE_nbytes ];
				// AVAIL u8 * PTR = EA_OBJECT_BASE_as_EA
				// AVAIL contiguous pages of _4096
				// AVAIL ANOTHER AREA
			}

			OUTER_SEGMENT_LIST

			MEASURE grow SPEED
			 BEHAVE as stats_gatherer clock all ALLOC requests

			  u47 clock = SECONDS since TIME_POS_ZERO _PLUS_OFFS_11
			  u47 clock = SECONDS since 1999_PLUS_ONE

		}

*/

#include <stdio.h>

typedef char      i8;
typedef int       i32;
typedef long long i64;

typedef unsigned char      u8;
typedef unsigned int       u32;
typedef unsigned long long u64;

#include "CPU_WORD.h"
#include "i8_i8_i16.h"

#if 0
typedef unsigned i8  u8;
typedef unsigned i32 u32;
typedef unsigned i64 u64;
#endif

// #include "ASM_byte_swap.h"
#include <byteswap.h>

void show_byte( const u8 val )
{
	printf("%c", val & 0xFF );
}

#define USE_ASM 0 // 1

int show_word_32( u32 val, const char * CMNT = 0 )
{
	if(!CMNT) CMNT = "CMNT";
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
	printf("      %s\n", CMNT );
	return 0;
}

int show_word_64( u64 val, const char * CMNT = 0 )
{
	if(!CMNT) CMNT = "CMNT";
//	printf("show_word_64 0x%016LX ", val ); // u64 needs % l
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

	show_byte(' '); // 
	show_byte( val >> (8*3) );
	show_byte( val >> (8*2) );
	show_byte( val >> (8*1) );
	show_byte( val );
	printf(" %s\n", CMNT );
	return 0;
}

u32 fetch_32( u32 * ptr )
{
	return * ptr;
}

u64 fetch_64( u64 * ptr )
{
	return * ptr;
}

int main(int argc, char ** argv )
{
	// LOOKING for ARM32 madness, when unaligned PTR
	// these work correctly on AMD_64
	// 
	// USING ALPHABET in 4 bytes of WORD_32
//	const u8 * str = (const u8 *) "@abcdefghijklm";
	const u8 * str = (const u8 *)  "abcdefghijklm";
	const u8 * str_1 = str + 1;
	const u8 * str_2 = str + 2;
	// str_0 is aligned _1 _2 NOT
	u32 * p0 = (u32 *) str;
	u32 * p1 = (u32 *) str_1;
	u32 * p2 = (u32 *) str_2;
	// p0 p1 p2 are pointers to WORDS - just badly aligned
	u32 w0 = fetch_32( p0 );
	u32 w1 = fetch_32( p1 );
	u32 w2 = fetch_32( p2 );
	// W0 w1 w2 // u32_WORD // test fetch 

	printf("show_text             '%s'\n", str ); // shows at
	show_word_32( w0, "w0 - u32_from_P0 - aligned 8" );
	show_word_32( w1,  "w1 - u32_from_P0+1" );
	show_word_32( w2,  "w2 - u32_from_P0+2" );

	u64 * q0 = (u64*) p1;
	u64 * q1 = (u64*) p1;
	u64 * q2 = (u64*) p2;
	// q1 = p1 // cast ITEM_t from u32_MEM_WORD to u32_u32_MEM_PAIR // u84
	u64 w5 = fetch_64( q0 ); // aligned access OK // it should work 
	u64 w3 = fetch_64( q1 ); // unaligned by 1 // compare with above
	u64 w4 = fetch_64( q2 ); // unaligned by 2 // compare with above

	show_word_64( w5, "w5 is from q0 "  );
	show_word_64( w3, "w6 is from q1 "  );
	show_word_64( w4, "w7 is from q2 "  );

	return 0;
}
