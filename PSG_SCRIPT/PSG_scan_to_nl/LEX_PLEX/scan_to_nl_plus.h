#ifndef SCAN_NL_PLUS_H
#define SCAN_NL_PLUS_H 

#include "scan_to_nl_xpos.h"
#include "scan_to_nl_base.h" // XPOS comes before base as added_item
// BASE_CLASS_t ITEM += XPOS_VAR { P_BASE_PAGE_ZERO /* VTL */ P_Y0 Y P }
// here_t here = ALLOC struct here_xpos; // CODE_AVAIL AUTO_VAR "here" //
// Match here_t _t struct { P P_X0 Y } 
// Rename here_t struct { P P_X0 Y } 
// struct here_t { P P_X0 Y } here; // inline DECL and start using "here_t" //
// struct here_t { P P_X0 Y } here; // _t "here_t" // _t decodes it's arg
// CODE_POINT _t MATCH EA=="_t"
// CODE_POINT _t MATCH MODULE SCRIPT line XPOS "_t" // AUTO_KNOW UNION _t
// _t is a BENCH_TOOL that groks SCRIPT_VAR("_t") SCOPE=AUTO_SCOPE SESS
// "%s_t" // here here_t // 
// var_name == "here"
// type_name == %s_t
// // eval name literally %%s_t "%s" %s // all same %s

// 

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
//	bool scan_digits_hex( int & digits );
	bool scan_digits_hex_expect( int & digits );


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


