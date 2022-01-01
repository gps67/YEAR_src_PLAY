#include "HT_table.h"
#include "stbl_spelling.h"

// str2_reused is necessary, but not overtly here
// it is done by ht1.cxx (I hope) (deprec?)
// it is done by HT_table_base.cxx (I hope)
// which stores ONE LOCKED (I wish) str2_reused per HT
// 
// #include "str2_reused.h"

#include "buffer1.h"

//#include <string.h> // memcpy

using namespace STBL;

// one POOL to hold them all // KEY == VAL // 
// global static SPELLING // ANYSTR - but held // to trim() before
// since HT is static, users dont need to wrestle with template<KEY_t,VAL_t>
// that is all in this .cxx .o // needs str2 is obj_ref holder of str1
// 
static HT_table<str2,str2> STBL_Spelling_Pool;


/*
	TODO REWORK:

		this moves to HT_table_base
		ALL tables with str2 keys (of the main_mem persuasion)
		use a common pool of str2 keys (why - hash splits enough!)

		the first call lookup( const char * k ) finds k
		in the local STBL (local vars, elem_pool, etc),
		and upgrades to str2 * key (somehow, maybe HT_PAIR)
		If not found in local DRY (repeat), then hop to
		the central string pool, where a self_deleting_str2
		is created (zero_ref_count kept by POOL, but temp,
		but one_in_list, but on_dispose--remove_from_dry)

		SO: when creating a HT - name the central pool
		SO: its the HT that allocate the str2_key from const char *
		
		
	LURK THREADS

		the table and also str2_reused must both be locked

		a simple spinning lock is OK as its usually V.fast
		and occasionally waiting for malloc.

		that needs to be done in the HT_table_base class
		or in this level function

	LURK NUL
		not sure what happens in sub-classes

 //	LURK overwrite
 //		_poke p0p2 overwrites the pointed string!
 //
 //		in an mmap file, that will cause (copy on write)
 //		or write-on-close

		I'd like to make this class better for p0p2
		as it is clearly how a parser might work over words
		(most of which are less than 8 bytes!)

	POSS ROM str0 - dont malloc
	POSS ROM store - use own area ...  (disk rom chenges everything)

	LURK cset
		you must be consistent to make u8 * s the same
		so 8859 or utf8 but not the same (different spellings)
		
*/

str2 * STBL::Spelling_Pool_lookup_add( const u8 * s )
{
	if(!s) s = (const u8 *) "";

//	str2_reused s2_head;
//	str2 * s2 = & s2_head;
//	s2_head.set_fixed( s );

	str2 * s2_ret = STBL_Spelling_Pool.lookup( (const char *) s );
	if( s2_ret ) return s2_ret;

	INFO("'%s'", s);

	s2_ret = new str2( s ); // NEW not borrowed FAKE str2
	return STBL_Spelling_Pool.add( s2_ret, s2_ret ); // KEY VAL
}

// #define NUL 0
#include "CR_LF_NUL.h"

/*!
	... ditto .. but poke a NUL byte at the end of p0p2 (and undo)
*/
str2 * STBL::Spelling_Pool_lookup_add_poke( const p0p2 & s )
{
//	const char * P0 = (char * ) s.p0;
	char * P2 = (char * ) s.p2;
	const char C2 = * P2;
	*P2 = NUL;
	str2 * s2 = STBL::Spelling_Pool_lookup_add( s.p0 );
	*P2 = C2;
	return s2;
}

/*!
	... ditto ... but readonly p0p2 buffer, so copy into a buffer1
*/
str2 * STBL::Spelling_Pool_lookup_add( const p0p2 & s )
{
	buffer1 buf;
	buf.set( s );
	// DONE by set() // buf.trailing_nul();
	return STBL::Spelling_Pool_lookup_add( buf );

//	// I like the idea of not using malloc for buffer1
//	u8 buff[ len + 1 ];
//	memcpy( buff, s.p0, len );
//	buff[len] = NUL;
//
//	str2 * s2 = STBL::Spelling_Pool_lookup_add( buff );
//	return s2;
}


str2 * STBL::Spelling_Pool_lookup_add( buffer1 & buf )
{
	buf.trailing_nul();
	return STBL::Spelling_Pool_lookup_add( buf.buff );
}


/* TODO
	//! A single PTR // 
	scan_to_nl_P {

		u8 * P;

	} // thats it // THEN the rest are the WORD and PHRASE found

	static {	// SEGEMNT // .data .bss .cache // prealloced .cache

		cset_bitmap
		 cset_bit_map_cset %s // = cset_AZaz09_ // a2

		cset_bit_map
		 cset_ident_a1;
		 cset_ident_a2;
		 cset_ident_a3;

        	cset_expr == "a2 = a1 + PLUS" cset_bit_map cset_ident_a2;
        	cset_expr "a2 = a1 + PLUS" {
			cset_bit_map
			 cset_ident_a2; // BIND PROVIDE_HERE CACHED("a2")
			& cset_ident_a1; // BIND nearby VAR ("a1") AMP("&")
			 PLUS;
			 ret_VAL; "PLUS" // CSET [0-9] _ $ 
			 ret_VAR; "PLUS"
		}
        	cset_expr "a3" {
			a3 = a1 + a2 + lookup( CSET, "PLUS" )
			// STEPS AZ az 09 // STEP UDEF // GLOBAL EFFECT
		}
        	cset_expr "a1" {
			a1 = // SUM OF STEPS
			AZ az 
			a2 = a1 + // STEPS into a1 // EVAL on INIT // DONE
			// PRELOAD runs stores loads STEPS and CACHE
			STEP 09 _ 
			OPTION_%s ? STEP $ 
			OPTION_%s ? STEP % - _ UTF8_GYLPH
			cset_ident_UTF8_GYLPH
			// over permissive stay within soft rules // CIDENT_GLYPH
			// CIDENT_GLYPH += [128+SP+1 - 255] 
			// KEEP UPPER CTRL for NBSP and SCRIPTING use
			// STEP [128 - 128+SP] 
			// STEP [128+SP+1 - 255]
			// OPTION FOR  8 BIT TRANSPARENT // all GLYPH or UDEF
			// [128 - 255] // UTF8_GLYPH // upto downstream up_down
			// CIDENT_GLYPH += [SP+1 - 255] // mostly everything is a GLYPH
			// CIDENT_EDGE_ += [NUL SP] [SP+1 - 255] // mostly everything is a GLYPH

			
		}
		// OK Module has static SEGMENT and SESS MMAP STO ROM 
		// OK Module also has PROC and its CODE_TEXT_TOKENSED_EVAL
		// OK Module rund GEN COMPILE RUN GEN COMPILE DEBUG
	}

	MATCH
		a1_a2_star // builtin CONST becomes ROM or STO // splice in

		// splice_in // easy way to AUTO_ADD_EVERYWHERE
		// splice_in // a1_a2_star += UTF8_GLYPH // in CIDENT :-)
		// filter pre_filter // GLYPH_SUBSET no_gaps_no_funny_stuff
		// filter FAIL_on_MATCH UNICODE is GAPPY
		
		not_a3 // _next _within "DETECT/REJECT/USAGE/CIDENT"
		a3 {
			// various GAPPY utf8 chars
			// maybe the UPPER_CTRL bank [128+NUL - 128+SP]
		}

		BENCH AUTO_GEN DETECT CONFLICT "CIDENT" "a3"
		a3 += UTF8_GAPPY
		UPPER_CTRL += [128+NUL - 128+SP]
		UTF8_GAPPY += [128+NUL - 128+SP]
		UTF8_GAPPY += UPPER_CTRL // [NUL SP] 
	
	TRIP
		$0 == CMD0 == PAGE_ID == CODE_POINT
		$1 == EVAL_PAGE GET $1 // LO // lhs
		$2 == EVAL_PAGE GET $2 // HI // rhs

		CMD0 {
			[LO HI]
		}
		LO {
			128+NUL
		}
		HI {
			128+SP
		}

		GEN {
			NUL == NUL
			SP == SP
			128 == 128

			u8_BYTE = EXPR BASE + ITEM // group_base // 

			u8 LO = EVAL INIT "128" + "NUL"
			u8 HI = EVAL INIT "128" + "SP"

			PSG MATCH_and_STEP_API {
				// MATCH27 is set_all_bits_in_range
				bool MATCH27 ( u8 lo, u8 hi ) {
					BITMAP256 %s cset_bitmap // u8_idx
					BITMAP256 set_all_bits_in_range( lo, hi );
				}
				// MATCHXX incl / excl / ...
				// get set reset
				// or and inv xor 
				// exprs actions well known CSET_SUBSETS
				// CIDENT varies with APP
				// OPTION PARSE IDENT // year-mm-dd_not_minus
				// sample "VALUE"

			}

			LEX MATCH_STEP {
			 literals c1
			 literals c1 c2
			 literals c1 c2 c3
			 literals c3 c2 c1 OP // OPCODE + BYTEB // >> 16 c2 c3
			 // pack multi byte into int c3 // c2 c1 // >> 16

			}

			CSET knows about [LO HI] RANGE + RANGE + ITEM

			LEX knows about CSET_STEP a1 a2_star a1_a2_star

			PSG knows about LEX

		}


	EXPR
		 component_bitfield_IDX field
		 FIELD
		  a3 = a2 + PLUS 
		 PLUS
		  // SCRIPT creates PLUS by mentioning it
		  // SCRIPT declares TYPE("PLUS") as "_t" // "%s_t" // AKA implied
		  // MATCH "PLUS" type STR0 latin_but_in_utf8 // ENCODED LIST
		  // MATCH clicks with LEX_USAGE_MAGNET
		  // REGEX multibyte utf8 SUB_LEX Layout
		  // CSET NUL
		  // CTRL NUL
		  // CSET BACKSPACE TAB CR LF SP 
		  // CTRL Field_sep_ Record_sep STREAM_CTRL EDGE
		  // CSET CTRL
		  // CSET GLYPH
		  // CSET COMPOSE TTY terminfo codes CSR XY
		  // # CSET REGEX MATCH P0P2 P 

		PARSER
		  // // # create_ITEM_by_mention
		  // // # create ../LIBR/ITEM #
		  // // # alias ./LIBR/ITEM # not preferred but DIALECT
		  // // # AUTO_VAR ITEM == ../LIBR/%s/item 
		  // // # "item" becomed "%s" EXPR EG "%s_%s %s _t
		  // // # GEN ITEM # as_a_var # varname #
		  // // # BENCH connects "ITEM" as "SAMPLE" "VALUE" "DIALECT"
		  // // # %s # CIDENT # CIDENT_PLUS #

		 CIDENT
		  // // # %s # // parse_CIDENT // permit utf8_anysrt

		 CSET utf8_anystr

			latin1_in_utf8 _but not in unicode
			permits CIDENT_anystr // consistent USAGE CSET _PROFILE

	SCRIPT

		CSET utf8_anystr latin1_values_binary_encoded_as_tf8 // not u

		Somewhere there is a phrase CSET_CIDENT_a1_a2 // _and_so_on
		You can rewrite that to anything, eg URL decoded TEXT_in_CSET

		TEXT CSET LINES of ARGV with CMNT PAGE LAYOUT

		INDENTED TEXT 

		GEN PARSE

		EVAL ARGV

		 Layout INDENTED_LINES_of_CODE import MODULE

	
	GEN ASM

		PARSE SCRIPT build EXPR_TREE Module DATA
		DATA is PSG of CODE parsed from SCRIPT
		ASM runs DATA = { TOKENISED SCRIPT ARGS }

	PSG missing_CMD0

		PAGE == "PAGE" // Layout // DTP == "Indented_Lines_of_WORDS"

		or rather it DOES come with CMD0 == "CODE_POINT" // _HERE

		BENCH recognises gossip text layout added to DATA_SET

		LAYOUT {
		 PAGE // CMD0 // PAGE_ID == "AUTO_GEN(PAGE_ID)"
		  SUBSET SCRIPT
		  SUBSET LINES of WORDS
		  SUBSET INDENTED TEXT
		  SUBSET CURLY_TEXT
		}



*/
