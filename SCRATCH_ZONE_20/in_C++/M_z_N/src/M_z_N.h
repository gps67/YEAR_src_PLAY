#ifndef M_z_N_H
#define M_z_N_H

// need NULL defined in
#include "stdlib.h"
#include "dgb.h" // INFO used


// for simplicity we limit ourselves to i32_idx inside u32_array
// we still hope for u8_idx

typedef int IDX_int; // POS

struct L_M_z_N_R_32 {

	char * PTR_to_ARRAY;

	IDX_int L;
	IDX_int M;
	IDX_int N;
	IDX_int R;

	/*!
		 NBIT increases by 1 
	*/
	int NBIT; // 2 3 4 5 6 7 8 9 10 ..
	int NBIT_idx_MAX; // = 0xFF; // 
	int M_MIN;
//	int NBIT_BITFIELD; // 2 6 8 12 16 24 32 48 64
	int NBIT_WORD; // 8 16 24 32 48 64 // BYTES = BITS / 8 remainder ZERO
	int bytes_per_item; // 1 2 4 8 

static const	IDX_int Z = 0;

	L_M_z_N_R_32( )
	: PTR_to_ARRAY(NULL)
	, L(0), M(0), N(0), R(0)
	, NBIT(2)
	, NBIT_idx_MAX(0x03) // b0011 2 bit
	, NBIT_WORD(2)
	, bytes_per_item(4)
	{
	}

	L_M_z_N_R_32( int bytes_per_word )
	: PTR_to_ARRAY(NULL)
	, L(0), M(0), N(0), R(0)
	, NBIT(0)
	, NBIT_idx_MAX(0x00) // b0000 0 bit // SINGLE_BIT has { M1 ZERO } // u0
	, NBIT_WORD(0)
	, bytes_per_item(bytes_per_word)
	{
		// BOOT_STRAP the match from 0 BIT jump via get_space to 5 BIT
		// WRAP 5 BIT in u8 // 
		/*
			STEP
				u0	1

				There is no DATA, no choice always KNOWN

				u1	2

				Single u1 BIT // VALUES "{ M1 ZERO }"

				If you use MINUS VALUE[0] means "ZERO" // 0
				If you use MINUS VALUE[1] means "M1" // -1

				If DONT use MINUS VALUE[0] means "ZERO" // 0
				If DONT use MINUS VALUE[1] means "01" // +1

				ZERO can mean NULL // ALIAS FALSE // falsy
				NON_ZERO can mean NOT_FALSE -aka- TRUE

				OPTION

					ON
					OFF 	ZERO is false OFF

				OPTION 

				(0)	[0]
				(-1)	[1]

				OPTION_REWRAP

					ONE
					TWO

				OPTION
					YES
					NO	ZERO is false

				u2	4

					PICK_1_of_4

					 decent
					 choice
					 between
					 options

					REGISTER_ABCD

					 JUMP_TABLE
					 item_type: EIGHT_t u32_u32

					REGISTER_ABCD

					 JUMP_TABLE
					 item_type: WORD_PAIR_u32_u32

					COMPILER_renames_ABCD_as_CODE_POINT_CACHED
					 rename example BITFIELD moves PAYLOAD
					 rename example ABCD alias into R_u5

					 CPU has 8 REGISTERS

					 	486
						CPU_SPILL holds 16K of EIGHT

					 CPU has 16 REGISTERS

					 	ARM32
					 	AMD64
						CPU_SPILL holds 16K of EIGHT

					 CPU has 32 REGISTERS

					 	ARM64
						CPU_SPILL holds 16K of EIGHT

					 MMAP is available on all CPU's we use

					 	486
						AMD_64
						x86_32
						x86_64
						ARM_32
						ARM_64

					 MMAP can be simulated as CACHE

					 	DATA is in ROM
					 	DATA is in STO
					 	DATA is in DB_NBYTES_BLOB

							VALUE passed around
							DB TRAY of SESS

						ESP32 can load all MMAP in
						ESP32 can load few MMAP in
						ESP32 can cache activity MMAP 

						Compiler has provided ROM source
						Compile uses CODE_POINT a lot
						XPOS == "{ FILE OFFS }"
						XPOS_as_line_number_CALC

						 fill in all the exprs
						 doing so pre-loads some
						 run script pre-loads more
						 build samples table
						 build local lexicon dialect
						 build compiled ASM SCRIPT

						 compile into JavaScript AVAR
						 mention creates AVAR
						 AVAR knows CODE_POINT of DECL
						 XPOS = "{ FILE OFFS }"
						 FILE = ITEM_POOL_of_FILE[IDX]




					 MINI_MACHINE has MMAP_u16_registers

					  EIGHT[u16] // 64K * 8 // 512 K

					 MINI_MACHINE
					 HEAP_ADDR

					  u8_u8_u16_u32_EA_EXPR in SESS
					  // SESS == RUNNING_MINI_MACHINE

					  u8_u24 in SESS.SEGMENT_ONE _TWO
					  // ITEM == HEAP[u8].ITEM[u24] 
					  // ITEM_t == EIGHT_t // KNOW over CODE_POINT
					  // KNOW has to PARSE what it knows
					  // to find out, to KNOW, cached_MMAP
					  // EIGHT[u8_u24] // ONE item one_EIGHT
					  // EIGHT[u8_u24] // ONE item WORD_PAIR
					  // FOUR[u8_u24] // ONE ITEM one WORD
					  // TWO[u8_u24] // ONE_TWO walked past
					  // ONE[u8_u24] // ONE_BYTE EA_OFFS


				u2_PICK 

				[b00]
				[b01]
				[b10]
				[b11]

				There is no PLUS other than ZERO

				u3	8
				u4	16 // 0x0F // TABLE_ONE[u4_idx] // [16]
				u5	32
				u6	64
				u7	128
				u8	256

				In every case, u8 can only hold 255, not 256
				An extra bit would be the easiest
				relay_repeat_KNOW _is_EMPTY
				relay_repeat_KNOW _is_never_EMPTY
				relay_repeat_KNOW ZERO_means_NULL
				relay_repeat_KNOW ZERO_MEANS_FULL
				relay_repeat_KNOW ZERO_MEANS_IDX0
				relay_repeat_KNOW _is_never_MINUS
				relay_repeat_KNOW _is_some_MINUS
				relay_repeat_KNOW _is_many_MINUS

				we can ALLOC [0  [0xFF[FULL
				#define FULL = 0xFF + 1
				# FILTER rewrite values as preset vars
				xFF is the nearest AVAR for this value
				xFF is NBIT_idx_MAX // ALIAS LHS RHS 

				u8 is ALIAS MACRO u8 { u8 } // 0xFF //

					BIT_FIELD_u8_NAME // but shifted

				u15 is u15_WORD_PLUS // u1_not_MINUS

				u9 is ALIAS MACRO u9 { u16 }
				u10 is ALIAS MACRO u10 { u16 }
				u11 is ALIAS MACRO u11 { u16 }
				u12 is ALIAS MACRO u12 { u16 }
				u13 is ALIAS MACRO u13 { u16 }
				u14 is ALIAS MACRO u14 { u16 }
				u15 is ALIAS MACRO u15 { u16 }
				u16 is ALIAS MACRO u16_t { u16 }

				u17 is ALIAS MACRO u32_t { u32 }
				u24 is ALIAS MACRO u32_t { u32 }
				u28 is ALIAS MACRO u32_t { u32 }
				u30 is ALIAS MACRO u32_t { u32 }
				u31 is ALIAS MACRO u32_t { u32 }
				u32 is ALIAS MACRO u32_t { u32 }
				u32 is ALIAS MACRO u32_t { u32_with_ZERO_MEANS_FULL
				special addressing mode, for u32 not u64
				ARM32 cant cope with 4G of memory need,
				ARM32 might have 128 MB or 2G (phone 32G)
				ARM32 uses many pools of HEAP_ITEM[u32_idx]

				#define u10 u16 // typedef u16 u10
				typedef u16 u10

				#define u10 u16



		*/
		INFO("CTOR()"); // COMPILER carries over DB_CODE_SPEC
	}

	IDX_int X_of_V( IDX_int V ) { return V - L; }
	IDX_int V_of_X( IDX_int X ) { return X + L; }

	char * EA_of_X( IDX_int X ) {
		return PTR_to_ARRAY + X * bytes_per_item;
	}

	char * EA_of_V( IDX_int V ) {
		return EA_of_X( X_of_V( V ) );
	}

	IDX_int MAX_of_NBIT( int NBIT ) {
		return 2 ^ NBIT - 1;
	}

	bool ALLOC_PLUS( IDX_int & V );
	bool ALLOC_MINUS( IDX_int & V );
	
	bool get_plus_space_for( int add );
	bool increase_NBIT_WORD_to_hold( int NBITS );
	bool increase_NBIT_to_cover( IDX_int N2 );
	
	bool grow_array();
	bool slide_MN_left_some();
	bool slide_left_to(IDX_int LM_gap); // gap left

	bool grow_array_left() { return grow_array_on_right( false ); }
	bool grow_array_right() { return grow_array_on_right( true ); }
	bool grow_array_on_right( bool on_right );


};

#endif
