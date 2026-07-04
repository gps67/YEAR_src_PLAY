#ifndef STO_one_H
#define STO_one_H
	/* 
		module STO_one 
		data_type_t STO_t // item_type in a STO frame

		STO_old // does not exist // could rename from STO
		STO	// ZERO means IMMEDIATE AVAR SELF or DECL
		STO_one	// _one means the REWRITE for STO_HEAP_ITEM
		STO_two // _two means _one _two is active
		STO_3	// IDX = get_IDX // const IDX_3 3 // ALIAS IDX PICK 3
		STO_0F	// reserve 0x0F for CORE // maybe UDEF 8+8 or 4+4+4+4
		STO_FF	// reserve 0x0F for CORE // M1 also takes it down to FA
		STO_FA	// [00 [0F ... ... [FA ... [FF [i256 == NEXT = FF+1

 EXAMPLE TEMPLATE of i8_DECODED
 		We probably only want M1
		We release M1 and it picks M4

		// PAGE uses UNS_IDX eg u2 because PAGE
		// UDEF uses INT_IDX eg i2 because MINUS
		// the HIGH_HALF is also reflected in MINUS HALF
		// MINUS_HALF HALF_HALF HIGH_HALF // PLUS_HALF == HALF_HALF
		// OPTION ZERO_means_NULL
		// OPTION ZERO_means_PLUS 
		// USAGE ZERO is only item, reduce to u0_IDX_SELF // still 0

		1 1	PLUS	P3 // or steal it back as M1
		1 0	PLUS	P2

		0 1	PLUS	P1
		0 0	DATA	ZERO
		1 1	UDEF	M1
		1 0	CORE	M2

 TESSELATING DECODE

 		TODO - draw it, this descriptionis wrong, fixit

		from above,
		remove those values from an i4 map

 HIGH HALF of PAGE of bytes not WORD of BITFIELDS i3

		_111
		_110
		_101
		_100

 HALF_HALF of PAGE of DATA  i3

		_011	M1_fractal
		_010	M2_fractal
		_001
		_000

 HIGH_HALF of PAGE reflected as MINUS

		1111	M1
		1110	M2
		1101
		1100

		1011
		1010
		1001
		1000	M8 M128 binds here when this is i4 (M128 here means M8)

			ALIAS M128 really means "the most minus 1000"
			ALIAS M128_or_most_MINUS "[L [M [Z [N [R" // L //
			_as_a_bitmap
				logic coped without that qualification
				but M123 might be the most_MINUS
				and not used as a BITFIELD mask 
			_not_as_a_bitmap
				M123 might actually be M128
				M123 happens when M1 M2 M3 M4 M5 are used
				but then moved to M128 releasing M1

			-MAP_ALIAS-

			M128	--
			M127	M5
			M126	M4
			M125	M2
			M124	M2
			M123	M1
			M122	NOT_ZERO_ the new M128 == MOST_MINUS

			M5
			M4 M3 M2 M1


		BIG HEAP ALLOC

		 SKIPS first 256 SLOTS // for either // CODE LIBR UDEF
		 Everyone skips so page_one _two _3 gets to 1024 

		 CORE leaves 2 bits for first 4 pages of 256

		 SKIPS first 16 PAGE of 256 SLOTS // 4096 item

		 	00	16 reserved pages
		 	0F	
			7F	The P127 point is unused +1 in i9
			80	The M128 point (here is M128)
			81	The M127 PAGE is in range
			10	The P16 PAGE is above those reserved pages
			EF	The EF limit
			F0	The [F0 [FF [MZ M16 pages
			FA	The M6 reflections are real here
			FF	The M1 PAGE

		 TINY_HEAP uses idx_ZERO as idx_PLUS
		 TINY_HEAP suggests idx_ZERO is SELF HERE THIS ITEM
		 TINY_HEAP suggests idx_ZERO is ONLY // CT_RT converts to i0
		 i0_IDX can have ANY value, eg PICK, can be NULL, also ITEM
		 i0_ITEM means SCOPE is HERE so no extra DECODE STEP needed
		 i0_KNOW may mean const VIA_AVAR
		 i0_KNOW may also mean VIA_AVAR = "AVAR_1234"

		 BIG HEAP alloc can easily jump past the first 64K of items
		 of OFFS of IDX (i19 = 512 K)
		 because those tables can be SPARSE

		 OWN_HEAP has EIGHT[u64_idx] // 512K index of i64_ITEM
		 DECODE_i64 as i8_i8_i16_i16_i16
		 DECODE_i64 as i8_i8_i16_i8_i24
		 DECODE_i64 as i8_i8_i16_i32

		 DECODE_i64 as i16_i16_i16_i16		// i8_i8_OPCODE
		 DECODE_i64 as i16_i16_i16_i8_i8	// BITFIELD DEFAULT
		 DECODE_i64 as         i48_i8_i8	// i48_i16 _PAYLOAD
		 DECODE_i64 as     i32_i16_i8_i8
		 DECODE_i64 as  i24_i8_i16_i8_i8

		 DECODE_i64 as i8_i8_i16_i8_i24
		 DECODE_i64 as i8_i8_i16_i32


		We step back on leave IDX_ZERO and PAGE_ZERO ti user data
		OPTION step back // we use M2

		That many designs of STO, each visiting modules brings dialect
		[0 [8	LIBR
		[8 [16	CORE
		[16 [128 UDEF
		[M128 [0

			CORE does everything you need at ELEMENT AXIOM level
			LIBR does those 8 themes again but UDEF by LIBR
			UDEF does 111 other schemes
			CORE also uses M1 M2 M3 M4
			CORE uses M3 M4 leaving LIBR can use M1 M2
			CORE uses M1 but then lets go for use in LIBR as M1
			for use as FF or as M1

			M128 to ZERO
			P128 to FULL


 	

		 STO_ZERO is not created because reserved
		 STO_ZERO is PICK[IDX_STO_one]
		 STO_one is ATTEMPT_one that does not disturb STO _old

		This is the STUBS of all the modules exports that we use
		Each importing module, could even create a CT_RT_Obj & obj;
		// pick by fn args // C_ANSI_fn_signatures // OTHER
		// obj = new obj( ARGS ) // DIALECT_SHIFT drop the new keyword
		// obj = obj( ARGS ) // api CTOR rules apply // morph_transport
		// obj = obj( ARGS ) // FN = _CTOR_ pick by fn_args
		// ITEM_t & item = lookup( pool, item ) ; // FRAME_var "item"
		// some RULES includes use of NAME_t & name = lookup("anystr");
		// some RULES include use of GCC/ASM/SCRIPT, CT_machine = DIAG
		// some RULES include use of DIAG + ANIM + LEXICON.script["%s"]
		// EG2
		// Parse stuff like { LEXICON.script["%s"] }
		// Match // LEXICON.script["%s"] } BLOB_QUOTED_anystr
		// Use of Name <--> IDX <--> USE // _of_SCRIPT
		// COMPILE_TOKENISE_EXPR ( CTXT, NAME, OPCODE )
		// OPCODE BIND_EA_EXPR // it doesn't have to run, just use id
		// BIND LINK, link = //EA// lookup( name )
		// BIND LINK link = some_other_EA_expr

	*/

	namespace STO {
	 // typedef caddr_t PTR; // DIALECT DROPPING _t

	 class STO_MGR_; // uplink to STO.STR_manager // DIALECT T_ from T_t 
	 class SEGMENT;
	 class mmap_file;

	};

	// the difference between a 1K and 4K STO_FILE in u16 K_limit
	// user allocated 200 KB for DODO_Segment
	// u16 x u10 = u26 of u32 - u6 
	// to MMAP it all, need to step back from u32
	// the 32 bit model will hold development back a bit
	// then it will step into its own MODULE_SPACE
	// which all fits WELL UNDER 64MB x 64_MODULE_SPACES // less some memory
	// to load it all, is to MMAP it all
	//
	// ENTIRE SEGMENT is under UDEF Layout
	// but it is NK_PAGES
	// caddr_t PAGE0;
	// int NK; // MAYBE less than u16, NOT NEGATE HERE, ZERO_is_first
	// and it occurs within container STO_
	// STO_MGR_UPLINK * STO_MGR; // owner of STO HEAP alloc, resize code ...
	
#include "sto_4k.h"

#endif

