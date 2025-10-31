#ifndef amnt_t_H
#define amnt_t_H

#include "ints.h"
#include "GLOBAL.H"
#include <stdlib.h>
#include <stdio.h> // sprintf
#include <math.h>
#include "dgb_fail.h"
#include "buffer1.h"

/*
 	floats have problems with rounding errors
	ints have a max range of += 2 x 1024 * 1024 * 1024 pence
	Thats about 21,474,836 pounds

	and with % calcs of 1000
	you can avoid exceeding 21 thousand pounds without thinking

	curr_t is unused and will remain so,
	all transactions MUST be converted to the base currency BEFORE entry.
	If your base changes, you need two sets of accounts.
	There might be way of merging two lines from two currencies,
	but that will use a FIXED convertion rate.

	SO i64 pence
*/

/*!
	UNUSED - currency type is ... UK .. EURO ...
*/
struct curr_t : public GRP_lib_base
{
	i32	flag;
	curr_t() {
		flag = 0;
	}

	bool operator!=( const curr_t & val2 )
	{
		return (flag != val2.flag );
	}
	bool operator==( const curr_t & val2 )
	{
		return (flag == val2.flag );
	}
};

/*!
	amnt_t as pence in currency.

	Integer pence does not have rounding errors, but it has
	a 32 bit ceiling of +- 21 Million pounds.
*/
struct amnt_t : public GRP_lib_base
{
	curr_t	curr;
//	i32	pence; // 21 million though 
	i64	pence; // 4G * 21 million pounds (counting in pence)

	/*!
		constructor - default zero
	*/
	amnt_t()
	{
		pence = 0;
	}

	//! copy assignment
	void set( const amnt_t & v2 )
	{
		pence = v2.pence;
	}

	bool is_zero() const
	{
		return pence == 0;
	}

	bool is_non_zero() const
	{
		return !is_zero();
	}

	bool is_negative() const
	{
		return pence < 0;
	}

	/*!
		constructor - from string value "1.23"
	*/
	amnt_t( str0 s );

	//!	set( pounds )
	void set( double dbl_pounds );

	/*!
		assign from string value 1.23
	*/
	bool set( str0 s );

	void set_pence( int p );

//	void set_pence( flt80 p );

	void set_pence( double p );


	void set_pounds( int p );

	void set_pounds( double p );

//	void set_pounds( flt80 p );

	double get_pounds_double() const;

	void set_zero();

/*!
	convert to string value
*/

	// WAS BUG // CSV printed commas in numbers

	str0 get_temp_str() const;

	str0 get_nocurr_str() const;

	str0 get_nocurr_str( buffer1 & buff ) const;

	str0 get_csv_str( buffer1 & buff ) const;

	void round_nearest();

	void round_down();

	void round_for_CH1();

	int pounds_rounded_down() const;

	int pounds_rounded_nearest() const;

	void get_nocurr_nosign_nopence_str( buffer1 & buff ) const;

	str0 get_nocurr_nosign_str( buffer1 & buff ) const;
	str0 get_nocurr_nosign_nocomma_str( buffer1 & buff ) const;

	// rename put_byte or print, get should return buff's contents as val (no clear() used) ???
	void get_curr_str( buffer1 & buff ) const;

	str0 get_curr_str() const;

/*!
	compare this with parameter

	read result from left to right A1.cmp(a2) --> A1 IS_LESS not a2
*/
	IS_DIFF_t cmp( const amnt_t & a2 ) const;

	bool add( const amnt_t & val );

	bool sub( const amnt_t & val );

	void negate();

	// the calculator is so stupid
	// it puts 0.25 ratios into currency 25p
	// but that seems OK for now
	// add parameter typed flt80

	bool mult( const amnt_t & val );

	bool divide( const amnt_t & val );

};
#endif
