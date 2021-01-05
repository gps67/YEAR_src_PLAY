/*
 * With i32 you can overflow at 21 thousand pounds  * 1000
 * So integer per_thou is not a good idea. This uses double
 * then truncation. That MIGHT cause errors.
 *
 * per_cent ACTS ON amnt_t, so YOU will have to create temp vars
 * You also have to convert for divisions?
 */

#include "per_cent.h"
#include "math.h"	// rint
#include "stdio.h"	// stdio

/*!
	constructor
*/
per_cent_t ::
per_cent_t()
: rate_str("100")
{
	rate = 100.0;
}

/*!
	multiply val (pence) by percent, round to nearest pence
*/
void	per_cent_t::times( amnt_t & val ) {
	// LURK rounding error
	flt80 dbl = val.pence;
	dbl = dbl * rate;
	val.set( dbl );
}

/*!
	set percentage value from a doube
*/
bool	per_cent_t::set( double new_rate ) {
	// I never call this - its better to use a str
	// now called by JS converting "0" to (int) 0
	rate = new_rate;
	char s[30];
	sprintf( s, "%.2f", rate );
	rate_str.set( s );
	rate = new_rate;
	// e_print( "per_cent::set( %2.2f ) '%s'\n", (float) rate, s );
	return true;
}

/*!
	set percentage value from a string
*/
bool	per_cent_t::set( const str0 & val ) {
	rate = 0.0;
	rate_str.set( val );
	if( rate_str.is_same_as("0.0") ) return TRUE; // exact "0" str
	if( rate_str.is_same_as("0") ) return TRUE; // exact "0" str
	if( rate_str.is_same_as("-") ) return TRUE; // exact "-" str
	if( rate_str.is_same_as("") ) return TRUE; // exact "-" str
	return val.as_double( rate );
	return TRUE;
}

/*
str0 per_cent_t::get_temp_str()
{
	return rate_str.get();
}
*/

/*!
	compare
*/
IS_DIFF_t per_cent_t::cmp( per_cent_t * val2 )
{
	if( ! val2 ) return IS_MORE;
	if ( rate < val2->rate ) return IS_LESS;
	if ( rate == val2->rate ) return IS_SAME;
	return IS_MORE;
}


/*!
	From A recalc b and c

	A = b + c
	117 = 100 + 17
*/
void per_cent_t::calc_abc_from_A( amnt_t A, amnt_t & b, amnt_t & c )
{
	flt80 V17 = rate;
	flt80 V100 = 100;
	flt80 V117 = V100 + V17;
	flt80 V_100_from_117 = V100/V117;
	flt80  V_17_from_117 = V17/V117;

	b.curr = A.curr;
	c.curr = A.curr;
	flt80 A_117 = A.pence;
	flt80 B_100 = V_100_from_117 * A_117;
	flt80 C_17  =  V_17_from_117 * A_117;
	b.set_pence( B_100 );
	c.set_pence( C_17 );
}


/*!
	From B recalc a and c

	a = B + c
	117 = 100 + 17
*/
void per_cent_t::calc_abc_from_B( amnt_t & a, amnt_t B, amnt_t & c )
{
	flt80 V17 = rate;
	flt80 V100 = 100;
	flt80 V117 = V100 + V17;
	flt80 V_117_from_100 = V117/V100;
	flt80  V_17_from_100 = V17/V100;

	a.curr = B.curr;
	c.curr = B.curr;
	flt80 B_100 = B.pence;
	flt80 A_117 = V_117_from_100 * B_100;
	flt80 C_17  =  V_17_from_100 * B_100;
	a.set( A_117 );
	c.set( C_17 );
}


/*!
	From C recalc a and b

	a = b + C
	117 = 100 + 17
*/
void per_cent_t::calc_abc_from_C( amnt_t & a, amnt_t & b, amnt_t C )
{
	flt80 V17 = rate;
	flt80 V100 = 100;
	flt80 V117 = V100 + V17;
	flt80 V_100_from_17 = V100/V17;
	flt80 V_117_from_17 = V117/V17;

	a.curr = C.curr;
	b.curr = C.curr;
	flt80 C_17  = C.pence;
	flt80 B_100 = V_100_from_17 * C_17;
	flt80 A_117 = V_117_from_17 * C_17;
	a.set( A_117 );
	b.set( B_100 );
}

