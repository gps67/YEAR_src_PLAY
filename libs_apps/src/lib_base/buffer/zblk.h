#ifndef zblk_H
#define zblk_H

/*
	zblk: compress a single mem block and hold the result
	grow buffer stretches to the largest ever needed

	know size from t0 t2 line.dt + type
	but prefer general multi-part processor?
	(particularly with list of blocks through SQL filter)

	best guess TEXT becomes ZLIB becomes CRYPTO_ZLIB_TEXT

		LIBR ZLIB += ZLIB
		LIBR ZLIB += LZO
		LIBR ZLIB += BZ2
		LIBR ZLIB += UDEF

		ZLIB_FACTOR_of_LZ0 [0 9 ] COMPRESS_FACTOR_GUESS

			COUNT every FRAME nbytes_in nbytes_out nbytes_used
			COUNT every FRAME
			 nbytes_in
			 nbytes_out
			 nbytes_used

			COUNT every FRAME
			 TPOS when FRAME SENT // DIALECT PAST TENSE //
			 XPOS in SESS in LIBR in UDEF STO CPU CODE BATCH_PANEL

			FRAME occupies one XPOS_EDGE __TICK_TOCK_
			FRAME LIST occupies upto FRAME_EDGE
			LAZY_EVAL learn sample DATA with LAZY_EVAL_FLAG_MODE_api
			LAZY_EVAL_api

	LAZY_EVAL_api

		We simulate a MINI_MACHINE with SPEC CODE and DATA
		CODE can be in LIBR and SESS and UDEF
		CODE_COPY is a reflection of CODE used AS_IF HERE_THIS in SESS

	DATA_CODE_api

		builtin LIBR provides API += LIBR with SESS 
		builtin LIBR provides API += LIBR with SESS with XPOS
		builtin LIBR provides API += LIBR LIBR TREE
		builtin LIBR provides API += LIBR with SESS BENCH

		DATA_CODE += Module_LIBR // PRE compiled
		DATA_CODE += Module_UDEF // NOW SPEC_ITEM_EXPR _DIALECT

		DATA_TEXT += CLIENT_READER_of_TEXT

			CLIENT = CALLER
			READER += import_LIBR _API

		SPEC_DECL += SPEC_DECL_TREE
	
	DATA_SPEC_DECL_and_CODE

		nbytes DATA
		SPEC_DECL_t SPEC_DECL; // MATCH %s_t %s // LHS == RHS //
		CODE_t CODE;

		XPOS_t XPOS_in_CODE // XPOS has AXIOM FILE XPOS XPOS_in_FILE
		DECL XPOS_in_CODE == XPOS_in_FILE // any i64_EA_EXPR

	A B KIT

		HEAP_ITEM

		 ITEM_SPEC ITEM
		 HEAP_SPEC HEAP

	// SWITCH _SPEC to _t // PICK // DRAW //

	// A B KIT // TILE of anove DECL as A_B_KIT // AVAR_and_XPOS

		DECODE_i64() { // DSL // BITFIELD DECL //
		 BYTE_A
		 BYTE_B
		 i16_PAYLOAD_GET()
		 i32_PAYLOAD_GET()

		 	EACH of above AUTO GEN all of the _t XPOS and DATA
			EACH adds SPEC from XPOS
			EACH adds DATA from WORD_in_CPU
			EACH adds DATA from WORD_in_MEM

		 i64_PAYLOAD_GET_EA_EXPR()

		 	i64_t CPU_WORD = _i64_MEM_WORD_at_EA // API //
			EA = EA_EXPR // SCRIPT meets LIBR // OPTIMISE but KEEP SAFE
			SAFE READ // usually YES // within MODE USER SESS //
			SAFE READ DENY // often NO // within MODE USER SESS //
			SAFE WRITE // USAGE RWX after all API PERMITS and PROOFS
			SAFE WRITE DENY // USAGE ROM_DATA OPTIMISE KNOWN ITKEN

		array[ IDX_32 ] of i64_WORD_in_MEM
		
			"{ i64_LOHI_in_MEM array_of_i64_WORD }"

				MENTION "{ ... }"

		CTRL_PANEL_for_DYNAMIC_ARRAY // when STATIC ROM complain PLENTY

			OPTION ROM is fixed after PRE_LOAD and SETUP ADJUST

			ADJUST += "{ ALLOC idx = N ++ }" // N idx ALLOC_SCRIPT

			SCRIPT from MENTION of ALLOC
			 SESS
			  ITEM // NEW CREATED HERE // OLD STILL HERE //

			OPTION BOUNCE P0P2 of ARRAY // array_IDX_of_EIGHT // i32_IDX
			OPTION i32_from_i32 within SESS with LIBR and UDEF

				UDEF is MACRO in SCRIPT to give NOUN action
				MACRO uses TOKEN from SPELLING
				OPTION DIALEXT EA = EA_XPOS_in_SCRIPT

		OPTION DIALECT EA = EA_XPOS_in_SCRIPT

			We run our MINI_WORLD within i8

				GROW i1 i2 i3 i4 i5 i6 i7 i8
				GROW i8 i16 i32
				GROW i12 i24 i48

		OPTION i48 _64_0_BITFIELDS // the entire WORD // i16_t

		OPTION i48 __48_0 // LO_SIDE //  i64_WORD << 16 >> 16
		OPTION i48 _64_16 // HI_SIDE i48_PAYLOAD = i64_WORD >> 16

		OPTION i48 __48_0 i48_PAYLOAD = i64_WORD << 16 >> 16
		OPTION i48 _64_16 i48_PAYLOAD = i64_WORD >> 16

				i5_in_i8_in_CPU_WORD

				 PICK i5 i8_UPTO_in_CPU_WORD

				 CPU_WORD_t _i5 = get_BITFIELD_i5

				 // _t comes with a LIBR of api DIALECT UDEF
				 // _t comes with promise to flatten to i32
				 // EA_EXPR and MINI_MACHINE will relocate on GROW
				 // a different EA_64 to be RUN in its place
				 // FILTER //
				 // LHS FILTER RHS //
				 // RHS FILTER LHS // SWITCH due to DIALECT

	GROW IDX_i8
	GROW i8_IDX

		PICK i5 i8_UPTO_in_CPU_WORD
		PICK i5 i16_UPTO_in_CPU_WORD
		PICK i5 i32_UPTO_in_CPU_WORD
		PICK i5 i48_UPTO_in_CPU_WORD


			We run our MINI_WORLD within i24

				this is a faster builtin

			We run our MINI_WORLD within i32

				i32_HALF
				i32_HIGH_HALF
				i32_BITFIELDS // that dont cross the 32_bit line
				i32_BITFIELDS // that do   cross the 32_bit line

			MATCH recognises BYTE_B when described in ALIAS

			64 [                0 [ // _IN_CPU_WORD
			64 [                0 [ // NEXT_i64
			64 [       16[  8[  0 [ // BYTE_B BITFIELD_16_8

		array[ IDX_32 ] of i64_WORD_in_MEM
		i64_WORD_in_MEM
		i64_WORD_in_CPU

			both import other at equal rank
			fail if conflict
			warn and continue




		 i32_i32_in_MEM_t
		 i32_i32_in_MEM = i64_WORD_in_MEM_at_BASE_OFFS_and_SPEC

		 	IMPORT BITFIELDS
			 LEXICON += BYTE_B // TOKEN of LIBR imported to PROVIDE
			 LEXICON += BYTE_A // AUTO IMPORT from MENTION of _DECL_

			MENTION "BYTE_B" and a MINI_WORLD appears with FSM TABLES
			 TABLES += FSM_TABLES
			 FSM_TABLES += TABLES 

			 EACH of above BRING PROVIDE

			  import LIBR += FEATURE // WE ARE FEATURE // LIBR IS //
			  import LIBR += FEATURE // WE ARE FEATURE // UDEF IS //

			 SAFE SWITCH the outer MINI_MACHINE is ROM

			  ROM 


		 	 MATCH EA_EXPR
			  value can be i8_i8_i16_i32
			  value can be i32_i32
			  value can be i64_as_i32_i32
			  value can be _UDEF_

		 i16_PAYLOAD_SET(i16)
		 i16_PAYLOAD_GET(i16 & RET_VAR)
		 i16_PAYLOAD_GET(i16 & INTO_VAR)

		 	i16_PAYLOAD = i32_WORD >> 16 # CPU_64_directly does that
			i32_WORD is the name of the parameter in the world above
			i32_BITFIELD includes all down to i8_in_CPU_WORD _HALF
			i32_HALF is the plain i64 but as truncated i32
			i32_HIGH_HALF is the i64_SHIFTTED_DOWN_32 _or_MEM

		i16 i16_PAYLOAD_GET()


		}

*/

#include <zlib.h>
#include "misc.h"
#include "blk1.h"

/*!
	A zblk is a blk1 which does compression/decompression.

	Maybe the two should be separated.
*/
struct zblk : public GRP_lib_base
{
	blk1	buff;
	int	srclen;
	zblk();
	~zblk();
	void clear() { buff.clear(); }
	bool compress( const char *src, uLong len);
	bool expand( blk1 & outbuff, uLong expected_len);

	bool test();

};

#endif
