#ifndef ASCII_chars_H
#define ASCII_chars_H

#ifdef NUL
#undef NUL
#endif

// unsure if these cause problems with uchar compare
// but they do fit in the +ve side of signed

static const char ASCII_NUL = 0;
static const char ASCII_BEL = 7;
static const char ASCII_TAB = 9;
static const char ASCII_FF = 12;
static const char ASCII_CR = 13;
static const char ASCII_LF = 10;
static const char ASCII_ESC= 27;
static const char ASCII_SP = 32;

static const char ASCII_BS =  8;
static const char ASCII_DEL = 127;

static const char ASCII_BACK_SLASH = '\\';

static const char ASCII_QUOTE_SINGLE = '\'';
static const char ASCII_QUOTE_DOUBLE = '"';
static const char ASCII_QUOTE_BACK = '`';

static const char ASCII_PAREN_LEFT = '(';
static const char ASCII_PAREN_RIGHT = ')';

static const char ASCII_SQUARE_LEFT = '[';
static const char ASCII_SQUARE_RIGHT = ']';

static const char ASCII_BACKSLASH = '\\';

static const char ASCII_BRACE_LEFT = '{';
static const char ASCII_BRACE_RIGHT = '}';

static const char ASCII_Q1 = ASCII_QUOTE_SINGLE; // my ALIAS for Q1
static const char ASCII_Q2 = ASCII_QUOTE_DOUBLE; // my ALIAS for Q2

/*
// CSR_t Q2 = XPOS_of_Q2 // Q2_lhs_XPOS // "SAMPLE_VALUE" AVAR_holding_SAMPLE_VALUE
// creating AVAR in DIALECT appear in LISTS on BENCH and UI // DIAG // DRAW //
// AVAR = "AVAR" // ASSIGN("AVAR") COMPARE("AVAR") PLUS_EQUAL(LHS,OPCODE,RHS) // 
// OPCODE = PUNCT = "+=" //

	RUNNUNG MINI_MACHINE can replace local REF with SESS OBJ REF

		TOKEN SPELLING found in SRC TREE
		IDX_of_TOKEN is ALL EA this as EA_EXPR usually IDX_EXPR

		USAGE of TOKEN in SCRIPT at XPOS 
			create SESS_VAR("SPELLING") with EA_of_EXPR
			u8_u8_u48_PAYLOAD
			u8_u8_u16_u32_IDX_EXPR

			POOL[u16_IDX] of EIGHT
			 EIGHT
			 	c_addr_t // ITEM_t * PTR ; //

			ITEM_t 
			 OBJ_HEADER
			  .ref_count
			  .VTBL_UDEF // _UDEF VTBL 
			  .flags // could have ref_count 
			  LIST_of_FIELDS_added_to_OBJ
			  ARRAY_of_EIGHT_appended as PAYLOAD

			SUB_SEGMENT_ITEM
			 OPTION
			  nbytes
			 OPTION
			  SEGMENT OFFS N SPEC
			 OPTION
			  ITEM is PAGE amounst SEAM_LESS_PAGE_JOIN
			  NBYTES { N P1 } // P0 = P1 // 
			  P0P2 { P0 P2 }





	// SCRIPTED CAST VAR // XPOS of Q2_lhs_XPOS //
	// Q2_rhs_XPOS // exists as AVAR holder of XPOS from ref_xpos
	// XPOS LIBR FILE LINE XPOS // 
	// XPOS LIBR FILE SEEK // OFFS //

	Q2_XPOS { LIBR FILE SEEK LNO BPOS CPOS XPOS Q2 SEGMENT OFFS }

	CODE_POINT at SEGMENT OFFS 
		NBYTES_SEGMENT
	CODE_POINT
		SESS += SESS
		SESS += CODE_POINT
		SESS += DATA_POINT EA NBYTES P0P2 SEGMENT_OFFS P2_guard
		SESS += STO 
			SESS gets a new STO item as a named field and VAR
			AVAR STO is added to global_scope
		STO += nbytes and adapters 
		STO += per item IDX and SPEC and API 
			MATCH lookup("ALIAS")
	Q2 is a SCRIPT that sees "Q2" as a template parser for "{ Q2 %s Q2 }"
	 VIEW "{ Q2 %s Q2 }"
	 VIEW "{ Q2 SP %s SP Q2 }"
	 AUTO PARSE += permit SP into GAP_PARSER incl NO_GAP ABSENT_GAP
	 AUTO PARSE += MATCH "{ Q2 SP }" and then "{ SP Q2 }"
	 AUTO PARSE += MATCH "{ Q2 BRACE_LEFT NOSP }" and then "{ SP } Q2 }"
	 AUTO PARSE += MATCH "{ Q2 BRACE_LEFT SP }" and then "{ SP } Q2 }"
	 AUTO PARSE += MATCH "{ Q2 BRACE_LEFT GAP }" and BENCH sets BITS AUTO
	 AUTO PARSE += MATCH "{ SP BRACE_RIGHT Q2 }" and SIMILAR_from_PARSER
	 STEP FSM weighted branches # add up step totals # PICK EXPR # COMPILE #
	 STEP FSM run over PREP DATA # write optimised BYTE_CODE #
	 AUTO_PARSE += add_GAPS where SP is
	 AUTO_PARSE += trim_GAPS where LEX_EDGE might also be
	 AUTO PARSE += GEN_CODE for each EXPR compiled to CASM // tiny_c ? //


*/


/*
	too many to mention
	auto add to local diary
	docs suggest man ascii # -k # man -k ASCII #

	ASCII_chars.h
	ASCII_Table_chars.h
	 CTRL { 0 <= SP }
          NUL {
       000   0     00    NUL '\0' (null character)   100   64    40    @
	  }
       001   1     01    SOH (start of heading)      101   65    41    A
       002   2     02    STX (start of text)         102   66    42    B
       003   3     03    ETX (end of text)           103   67    43    C
       004   4     04    EOT (end of transmission)   104   68    44    D
       005   5     05    ENQ (enquiry)               105   69    45    E
       006   6     06    ACK (acknowledge)           106   70    46    F
	 }
	 Glyphs
	 	# dammn need a vt100 manual with ASCII names
		# search?q=ascii+table

*/


// static const char DEL = 127;

#endif
