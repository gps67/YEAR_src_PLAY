#ifndef SCAN_NL_PLUS_H
#define SCAN_NL_PLUS_H 

#include "scan_to_nl_base.h"

/*
	here_pos3 should be a base class of scan_nl
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


