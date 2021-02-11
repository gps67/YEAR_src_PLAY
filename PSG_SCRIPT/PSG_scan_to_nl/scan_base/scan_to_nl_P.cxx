
#include "scan_to_nl_P.h"
#include "p0p2.h"


/*
	statics are just like "extern" things
	define them in the .CXX corresponding to the .H
*/
int scan_to_nl_P::cset_inited_a = 0; // init defined as = 0;
cset_bit_map scan_to_nl_P::cset_09;
cset_bit_map scan_to_nl_P::cset_09_af_AF;
cset_bit_map scan_to_nl_P::cset_az;
cset_bit_map scan_to_nl_P::cset_AZ;
cset_bit_map scan_to_nl_P::cset_AZaz;
cset_bit_map scan_to_nl_P::cset_AZaz_;
cset_bit_map scan_to_nl_P::cset_AZaz09;
cset_bit_map scan_to_nl_P::cset_AZaz09_;
cset_bit_map scan_to_nl_P::cset_line;
cset_bit_map scan_to_nl_P::cset_ident_a1;
cset_bit_map scan_to_nl_P::cset_ident_a2;


/*!
*/
void scan_to_nl_P::init_csets(void)
{
	// maybe add force_init_csets() which sets cset_inited_a = 0;
	if( cset_inited_a ) return;
	cset_inited_a = 1;

	e_print("##### C init_csets() ###\n");

	// could be done by init constructor!
	cset_az.set_null();
	cset_AZ.set_null();
	cset_09.set_null();
	cset_AZaz.set_null();
	cset_AZaz_.set_null();
	cset_AZaz09.set_null();
	cset_line.set_null();

	cset_az.set_range( 'a', 'z' );
	cset_AZ.set_range( 'A', 'Z' );
	cset_09.set_range( '0', '9' );

	cset_AZaz   |= cset_az;
	cset_AZaz   |= cset_AZ;
	cset_AZaz09 |= cset_AZaz;
	cset_AZaz09 |= cset_09;

	cset_line.set_range( 0, 255 );
	cset_line.reset_bit( 0 ); // not allowed
	cset_line.reset_bit( '\n' ); // not allowed
	cset_line.reset_bit( '\r' ); // not part of line text

	cset_AZaz_ = cset_AZaz;
	cset_AZaz_.set_bit( '_' );

	cset_AZaz09_ = cset_AZaz09;
	cset_AZaz09_.set_bit( '_' );

	cset_09_af_AF.set_null();
	cset_line.set_range( '0', '9' );
	cset_line.set_range( 'a', 'f' );
	cset_line.set_range( 'A', 'F' );

	// default identifier is C
	cset_ident_a1 = cset_AZaz;
	cset_ident_a2 = cset_AZaz09_;
}


/*!
	scan_word("unsigned") wont match unsigned99
	
	If the match fails, FALSE is returned, and LEX.P is unchanged.

	trailing gaps are untouched
*/
bool scan_to_nl_P::scan_word( const u8 * word, const cset_bit_map & a2 )
{
	if( *P != *word ) return false;
	u8 * P0 = P;
	while( *word ) {
		if( *P != * word) return false;
		P ++;
		word ++;
		
	}
	if( peek_a1( a2 ) ) {
		/*
			reject "unsigned99" because 9 is in a2
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
bool scan_to_nl_P::peek_hex_digit()
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
	scan over a hex digit
*/
bool scan_to_nl_P::scan_hex_digit()
{
	int c = *P; // u8 to int
	if(( '0' <= c ) && ( c <= '9' )) {
		P++;
		return true;
	}
	if(( 'a' <= c ) && ( c <= 'f' )) {
		P++;
		return true;
	}
	if(( 'A' <= c ) && ( c <= 'F' )) {
		P++;
		return true;
	}
	return false;
}

/*!
	scan over a hex digit, and get its value
*/
bool scan_to_nl_P::scan_hex_digit( int & dgt )
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
bool scan_to_nl_P::scan_hex_digits( int & value )
{
	// call might have done // if( !peek_hex_digit()) return false;
	const u8 * P0 = P;
	int val = 0;
	int dgt = 0;
	while( scan_hex_digit(dgt)) {
		val <<= 4;
		val += dgt;
	}
	if( P0 == P ) return false;
	value = val;
	return true;
}



