#ifndef SCAN_NL_XPOS_H
#define SCAN_NL_XPOS_H 

#include "ints.h" // u8

/*!
	simplest CSR
*/
struct here_pos
{
	u8 * P;
};


/*!
	3 word XPOS in LINE
*/
struct here_xpos
{
	u8 * P;
	u8 * P_X0;
	int Y;
};

// rename here_xpos sd

/*
	here_xpos should be a base class of scan_nl

	These are going to be renamed XPOS

		XPOS_P1
		XPOS_OFFS
		XPOS_Y
		XPOS_XPOS

		FILE_ZONE = P0P2_TEXT CST == CSET

		// PARSE DETCT // P0P2_TEXT %s_%s // P0P2 == P0P2 // CSET == TEXT.CSET //

		// DIAG { DIAG } {
		// # DTP is drawing a DIAG # or two # CODE_POINT #
		// }

			{ u8 * P } // CMNT

			MACRO P = PTR = BASE + OFFS

			BASE == "XPOS_P1.BASE" // page_zero // item_zero // as_id_zero //
			u8 * ZERO == BASE // signal accumulates at CODE_POINT // define ZERO BASE // define BASE PAGE0 // 
			u8 * ZERO == PAGE0 // signal accumulates at CODE_POINT // define ZERO BASE // define BASE PAGE0 // 

			OFFS == Parsed_TEXT_XPOS xpos of item in TEXT // ALIAS XPOS ITEM // SIGN_USAGE // OPCODE DECODES TEXT csr is at XPOS // ITEM_DESC = "ITEM"

			BENCH VAR_NAME "%s" ITEM DESC XPOS CSR P0 P2 nbytes OFFS offs_%s // AUTO_GEN offs_%s //

			P0P2 == "{ BASE_ADDR ADDR_P2 }"
			STO == "{ BASE nbytes }"

			-- INVISIBLLE PARAMETER -- HOW --
			-- added here ALLOC nbytes in HEAP as SESS.HERE -- CODE_POINT --

				P0P2 == "{ BASE_ADDR ADDR_P2 }"

				STO == "{ BASE nbytes }"
				
				ZONE = P0P2 

				P0P2 += P2 == .next.P0 // see DIAG

				SESS == "CTXT.SESS" // AKA "CTXT_SESS" // AKA "ALIAS" // of SESS

					ld sess = "MANGLED_ALIAS SESS"

				ALLOC { BASE = MALLOC nbytes INIT NULLS // CMNT }

				EXPR P0 = BASE
				EXPR P2 = BASE + nbytes

				P0P2 P0 P2 // CMNT // CODE_POINT // :-)
		// CODE //
				CODE { KNOWS } // thinks it might be
				CODE { GROWS } // BENCH //

				convert 
					STO_t 
						STO == "{ BASE nbytes }" CMNT == "CMNT"
					EVAL
						STO.ARGV += { u8 * BASE } 
						STO.ARGV += { int nbytes }
						CMNT == CODE_POINT.VAR_EA_LEX("CMNT")
						CMNT == LINK_ABOVE "CMNT" // simple MATCH STR0 // NAME TOKEN MACRO ITEM STO NBYTES P0P2 MMAP API // 
						NAME_POOL{ item = lookup "CMNT" } // flat_match_CMNT_bind_var_SESS_this_line_ITEM ITEM_CMNT
						KEY == { item = lookup "CMNT" } // CMNT // two meanings overlap boost both CMNT as OWN SAMPLE VALUE
					P0P2
						CTOR == "P0P2 == %s" // BENCH FILLS OUT REST // EG "%s" requires "VAL" or { val eval("VAL") } // EXPR //
						EXPR = CTOR

					STO
						CTOR == $0 // SELF INIT //
						EXPR == CTOR // search // { repair search } //

						-pick-
						-or-
						{ BASE == P_X0 } // P_X0 == P0P2.P0

							I thihnk I need to reword p0p2_t P0P2



*/


/*
	here_pos is for when here_xpos is too much

	LEXER when it does not cross EOLN 
	OPTION int OFFS xpos_of_P.as_u12_OFFS _XPOS
	AUTO_BITS upgrade u12 to u24 to u48_payload _as_i48

	BOLN INDENT ARGV CMNT TEXT_of_FILE

	TOKENISER gets it to u8_u8_PAIR or u32_u32_PAIR // DECODE u8_u8

	Parser uses BOLN + byte_pos 

		u8 * BOLN;
		u8   OFFS;

		struct here_xpos
		{
			u8 * P;
			u8 * P_X0;
			int Y;
		};

		BOLN = EXPR = P_X0
		BOLN_Y = Y
		PTR = ADDR = P_X0 + OFFS // OFFS = u8_payload_BYTE_i8

		XPOS == BOLN u8_OFFS // OPTION SUBSET MATCH TOK

		POINT_to_POINT 
		bridge between 2 points
		BOLN to XPOS

		VECT == XYZ_STEP
		VECT == i8_OFFS
		VECT == u8_OFFS
		VECT == OFFS // i8 u8 i16 u16 i32 u32 i64 u64 f32 f64 // u48
		EXPR == { ADDR = BASE + OFFS }
		EXPR == { OFFS = OFFS_of_field_in_struct }
		EXPR == { OFFS = x1248_ } // u8 u16 u32 u64 // u48 // INT_UNS
		{ WORD_t WORD[IDX_t idx] // WORD_t == "EIGHT_t" // "FOUR_t" }

		BOLN = EITHER here_xpos or PTR_P { u8 * PTR }
		{ u8 * %s } BOLN

		DETOKENISER needs to use OFFS of FIELD in STRUCT
		DETOKENISER needs to use IDX from ALLOC

			ALLOC RET_VAL RET_TYPE AUTO_SCALE cpu_var_WORDS
			ALLOC { idx = N ++ } // no comment on axiom
			// data_payload = i48_payload // OPTIONS u48 i48 
			// data_payload = u8_BYTE_B // BITS 8-15 //
			// data_payload = BIT_FIELD of i64_WORD 

			ALLOC { bitfield = idx } // SPEC += u8_in_u64 _in_mem
			ALLOC { upto_i48 } // BIT_SHIFT // 
			// no_data_payload

			ALLOC IDX
			ALLOC OFFS

			ALLOC SIGN
			 INT
			 UNS

			BIT_FIELD
			 SIGN = INT
			 nbits = 48
			 nbits_left = ZERO // CT_ABSENT
			 nbits_here = nbits = 48
			 nbits_right = 16
			 
			ROTATE_STATE_PLAIN
			 LOHI_MEM
			 DCBA_WORD
			  DCBA_WORD_64

			{ u48_payload = u64_WORD >> 16 }
			  mk_u64 u48_payload u16_payload // 

			   mk_u64
			    u48_payload
			    u16_payload 

			   _JOIN
			   _PAIR %s %s
			   _SPLIT

			   _BYTE_B
			   _WORD_DECL _u64 _u32 _u16 _u8
			   _WORD_DECL _i64 _i32 _i16 _i8
			   _WORD_DECL _f32 _f64

			   truncate cast CPU 





*/

#endif

