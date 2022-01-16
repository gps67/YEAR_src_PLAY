
#include "scan_to_nl_P.h"
#include "p0p2.h"



/*!
	scan_word("unsigned") wont match unsigned99
	
	If the match fails, FALSE is returned, and LEX.P is unchanged.

	trailing gaps are untouched

	hmm "word" can be "<=>" or "/*!" or "//!" 
	a2 is the cset similar to word that would be a longer word,
	for word = "unsigned" a2 = IDENT_c2
	for word = "<<"
*/
bool scan_to_nl_P::scan_word_a2( const u8 * word, const cset_bit_map & a2 )
{
	// single letter test
	if( *P != *word ) return false;
	// keep roll-back point
	u8 * P0 = P;
	while( *word ) {
		if( *P != * word) {
			P = P0; // roll back // nb rename P0 P-Y_X0
			return false;
		}
		P ++;
		word ++;
	}
	if( peek_a1( a2 ) ) {
		/*
			"unsigned" has been found
			reject "unsigned9" because 9 is valid a2 (so not RW)
			leave as found
		*/
		P = P0;
		return false;
	}
	return true;
}

/*!
	returns false if neither found
*/
bool	scan_to_nl_P::scan_spaces_and_tabs_fn()
{
	if( !( (*P == ' ') || (*P == '\t'))) return false;
	while( (*P == ' ') || (*P == '\t')) {
		P++;
	}
	return true;
}


/*!
	scan over identifier
*/
bool scan_to_nl_P::scan_a1_a2_star_fn(
	p0p2 & zone,
	const cset_bit_map & a1,
	const cset_bit_map & a2
)
{
	zone.p0 = P;
	if(!scan_a1( a1 ) ) return false;
	if( !scan_a1_plus( a2 ) )
	{
		return false;
	}
	zone.p2 = P;
	return true;
}

/*!
	return true if its a HEX digit
*/
bool scan_to_nl_P::peek_digit_hex()
{
	int c = *P; // u8 to int
	if(( '0' <= c ) && ( c <= '9' )) {
		return true;
	}
	if(( 'a' <= c ) && ( c <= 'f' )) {
		return true;
	}
	if(( 'A' <= c ) && ( c <= 'F' )) {
		return true;
	}
	return false;
}

/*!
	scan over a hex digit, and get its value
*/
bool scan_to_nl_P::scan_digit_hex( int & dgt )
{
	int c = *P; // u8 to int
	if(( '0' <= c ) && ( c <= '9' )) {
		dgt = c - '0';
		P++;
		return true;
	}
	if(( 'a' <= c ) && ( c <= 'f' )) {
		dgt = c - 'a' + 10;
		P++;
		return true;
	}
	if(( 'A' <= c ) && ( c <= 'F' )) {
		dgt = c - 'A' + 10;
		P++;
		return true;
	}
	return false;
}

/*!
	scan over a hex digit string, and get its value
*/
bool scan_to_nl_P::scan_digits_hex( int & value )
{
	// call might have done // if( !peek_digit_hex()) return false;
	const u8 * P0 = P;
	int val = 0;
	int dgt = 0;
	while( scan_digit_hex(dgt)) {
		val <<= 4;
		val += dgt;
	}
	if( P0 == P ) return false;
	value = val;
	return true;
}



