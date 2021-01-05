#ifndef per_cent_H
#define per_cent_H
/*
 * With i32 you can overflow at 21 thousand pounds  * 1000
 * So integer per_thou is not a good idea. This uses double
 * then truncation. That MIGHT cause errors.
 *
 * per_cent ACTS ON amnt_t, so YOU will have to create temp vars
 * You also have to convert for divisions?
 */

#include "str1.h"
#include "amnt_t.h"

/*!
	A percentage factor, held as a double
*/
struct per_cent_t : public GRP_lib_base
{
private:
	double	rate;
public:
	str1	rate_str;	// preserve input spelling

	per_cent_t();

	void	times( amnt_t & val );
	bool	set( double new_rate );
	bool	set( str1 & val );
	bool	set( const str0 & val );
	IS_DIFF_t cmp( per_cent_t * val2 );
	str0 get_temp_str();

	// A == b + c // 110 == 100 + 10
	void calc_abc_from_A( amnt_t   A, amnt_t & b, amnt_t & c );
	void calc_abc_from_B( amnt_t & a, amnt_t   B, amnt_t & c );
	void calc_abc_from_C( amnt_t & a, amnt_t & b, amnt_t   C );

};

#endif
