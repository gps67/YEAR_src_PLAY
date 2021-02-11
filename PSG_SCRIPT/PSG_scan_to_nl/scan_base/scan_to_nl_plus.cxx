#include "scan_to_nl_plus.h"

#include "str1.h" // atoi atof of p0p2 ...

//	bool scan_dd( int & dd );
//	bool scan_int( int & i );
//	bool scan_double( double & dbl );


/*!
*/
bool scan_to_nl_plus::scan_hex_digits_expect( int & digits )
{
	if( scan_hex_digits( digits ) ) return true;
	report_FAIL( "expected hex digits number" );
	return false;
}


/*!
	scan 2 digits (possibly within a longer string od digits?)
*/
bool scan_to_nl_plus::scan_dd( int & dd )
{
	here_pos P0;
	here_start( P0 );
	u8 c1 = P[0];
	u8 c2 = P[1];
	if(scan_char( ' ' )) {
		c1 = '0';
	} else if(scan_a1( cset_09 )) {
		// c1 = c1;
	} else return FALSE;
	if(!scan_a1( cset_09 )) {
		goto fail;
	}
	dd = (c1 - '0') * 10 + (c2 - '0');
	return TRUE;
fail:
	here_back( P0 );
	return FALSE;
}

// scan for a returned decimal int
bool scan_to_nl_plus::scan_int( int & i )
{
	// zone only needed if int arith not done here
	// when bothered - do digit math
	// ?? what if int becomes float ??
	p0p2 zone;
	p0p2_start( zone );
//	while(scan_char( ' ' )) ;
	if(!scan_a1_plus( cset_09 )) return FALSE;
	// what if float trailing
	p0p2_stop( zone );
	str1 s( zone) ;
	// str1 s = zone;
	i = atoi( (STR0) s.str );
	return TRUE;
}

// scan for a returned decimal int
bool scan_to_nl_plus::scan_double( double & dbl )
{
	here_pos P0;
	here_start( P0 );
	// zone only needed if int arith not done here
	// when bothered - do digit math
	// ?? what if int becomes float ??
	p0p2 zone;
	p0p2_start( zone );
	scan_spaces_and_tabs(); // OPTIONAL
	bool found = false;
	bool negative = false;
	if(scan_char( '-' )) {
		negative = true;
		scan_spaces_and_tabs();
	}

	if(scan_a1_plus( cset_09 )) {
		found = true;
	}
	if(scan_char( '.' )) {
		// OK
	}
	if(scan_a1_plus( cset_09 )) {
		found = true;
	}

	// I WOULD NOT USE SPACE WITHING -.1e-5 // scan_plain_white_space();
	if(scan_char( 'e' )) {
		scan_char( '-' ); 
		scan_a1_plus( cset_09 );
		// OK
	}

	// look for trailing illegals
	if(scan_char( '.' )) {
		found = false;
	}

	if(!found) {
		here_back( P0 );
		return false;
	}

	p0p2_stop( zone );
	str1 s( zone) ;
	dbl = atof( (STR0) s.str );
	return TRUE;
}

/*!
	scan 2 digits, then optional gap
*/
bool scan_to_nl_plus::lex_dd( int & dd )
{
	if( !scan_dd( dd ) ) return false;
	GAP_after_LEX();
	return true;
}

// scan for a returned decimal int
bool scan_to_nl_plus::lex_int( int & i )
{
	if( !scan_int( i ) ) return false;
	GAP_after_LEX();
	return TRUE;
}

// scan for a returned decimal int
bool scan_to_nl_plus::lex_double( double & dbl )
{
	if( !scan_double( dbl ) ) return false;
	GAP_after_LEX();
	return TRUE;
}

bool scan_to_nl_plus::lex_ident( p0p2 & ident )
{
	if(!peek_a1( cset_ident_a1 ) ) return false;
	scan_ident( ident );
	GAP_after_LEX();
	return true;
}
