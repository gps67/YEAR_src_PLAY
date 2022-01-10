#ifndef SCAN_NL_PLUS_H
#define SCAN_NL_PLUS_H 

#include "scan_to_nl_base.h"

/*
	here_pos3 should be a base class of scan_nl

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
						{ BASE == P0 } // P0 == P0P2.P0

							I thihnk I need to reword p0p2_t P0P2



*/
struct here_pos3
{
	u8 * P;
	u8 * P0;
	int Y;
};

/*
	here_pos is for when here_pos3 is too much
*/
struct here_pos
{
	u8 * P;
};

/*!
	adds here_back and some semi-lex things like digits 
*/
class scan_to_nl_plus : public scan_to_nl_base
{
 public:
	scan_to_nl_plus()
	: scan_to_nl_base()
	{
	}

	scan_to_nl_plus( p0p2 buffer )
	: scan_to_nl_base( buffer )
	{
	}

	bool scan_dd( int & dd );
	bool scan_int( int & i );
	bool scan_double( double & dbl );

// added
//	bool scan_hex_digits( int & digits );
	bool scan_hex_digits_expect( int & digits );


	// useful when lexer rolls back
	void here_start( here_pos & pos ) const
	{
		pos.P = P;
	}
	void here_back( const here_pos & pos )
	{
		P  = pos.P;
	}

	// useful when lexer rolls back
	void here_start( here_pos3 & pos ) const
	{
		pos.P0 = P0;
		pos.P = P;
		pos.Y = Y;
	}
	void here_back( const here_pos3 & pos )
	{
		P0 = pos.P0;
		P  = pos.P;
		Y  = pos.Y;
	}

	// useful when lexer rolls back
//	void here_start( u8 * & P0 ) {  P0 = P; }
//	void here_back( u8 * P0 ) {  P = P0; }

	// useful when lexer gathers chars after it runs (or rolls back)
	void p0p2_start( p0p2 & P0 ) {  P0.p0 = P0.p2 = P ; }
	void p0p2_stop( p0p2 & P0 ) {  P0.p2 = P; }
	void p0p2_back( p0p2 & P0 ) {  P = P0.p0; }

// expect 
// lex 
	virtual bool GAP_after_LEX() = 0;

	bool lex_dd( int & dd );
	bool lex_word( const char * word ) 
	{
		if( !scan_word( word )) return false;
		GAP_after_LEX();
		return true;
	}
	bool lex_double( double & dbl );
	bool lex_int( int & i );
	bool lex_ident( p0p2 & ident );

};


#endif


