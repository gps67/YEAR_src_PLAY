#include "amnt_t.h"

// #include "ints.h"
// #include "GLOBAL.H"
// #include <stdlib.h>
// #include <stdio.h> // sprintf
// #include <math.h>
// #include "dgb_fail.h"
// #include "buffer1.h"

/*
 	floats have problems with rounding errors
	ints have a max range of += 2 x 1024 * 1024 * 1024 pence
	Thats about 21,474,836 pounds

	curr_t is unused and will remain so,
	all transactions MUST be converted to the base currency BEFORE entry.
	If your base changes, you need two sets of accounts.
	There might be way of merging two lines from two currencies,
	but that will use a FIXED convertion rate.
*/

#if 0
	// INLINE see .h

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
#endif

	/*!
		constructor - from string value "1.23"
	*/
	amnt_t::amnt_t( str0 s )
	{
		pence = 0;
		set( s );
	}

	//!	set( pounds )
	void amnt_t::set( double dbl )
	{
		set_pounds( dbl );
	}

	/*!
		assign from string value 1.23
	*/
	bool amnt_t::set( str0 s )
	{
		double d = atof( s );
		d *= 100;
		set_pence( d );
		return TRUE;		// LURK BUG ZERO
	}

	void amnt_t::set_pence( int p )
	{
		pence = p;
	}

	void amnt_t::set_pence( double p )
	{
#ifdef WIN32
		pence = i32( p );
#else
		pence = i32( rint( p ) );
#endif
	}

	void amnt_t::set_pounds( int p )
	{
		pence = p * 100;
	}

	void amnt_t::set_pounds( double p )
	{
		set_pence( p * 100.0 );
	}

	double amnt_t::get_pounds_double() const
	{
		return pence / 100.0;
	}

//	//! copy assignment
//	void set( const amnt_t & v2 )
//	{
//		pence = v2.pence;
//	}

	void amnt_t::set_zero()
	{
		pence = 0;
	}

/*!
	convert to string value
*/

	str0 amnt_t::get_temp_str() const
	{
		char asap[20];
		double d = pence;
		d = d / 100.0;
		sprintf( asap, "%.2f", d );
		return &asap[0];
	}

	str0 amnt_t::get_nocurr_str() const
	{
		static buffer1 buff;
		buff.clear();
		get_nocurr_str( buff );
		return buff;
	}

	str0 amnt_t::get_csv_str( buffer1 & buff ) const
	{
	//	buff.clear(); // rename print or add_str
		if( is_negative() )
		{
			buff.put_byte( '-' );
		}
		get_nocurr_nosign_nocomma_str( buff );
		return buff;
	}

	str0 amnt_t::get_nocurr_str( buffer1 & buff ) const
	{
	
		if( is_negative() )
		{
			// FAILED - converted char to int to buffer
			// which did not much
			// buff.put( '-' );
			buff.put_byte( '-' );
		}
		get_nocurr_nosign_str( buff );
		return buff;
	}

	void amnt_t::round_nearest()
	{
		pence = 100 * pounds_rounded_nearest();
	}

	void amnt_t::round_down()
	{
		pence = 100 * pounds_rounded_down();
	}

	void amnt_t::round_for_CH1()
	{
		round_down();
	}

	int amnt_t::pounds_rounded_down() const
	{
		int p = pence;
		int f = 1;
		if( p < 0 ) {
			p = 99 - p; // DOWN
			f = - 1;
		}
		i32 pnd = p  / 100 ;
		return f * pnd;
	}

	int amnt_t::pounds_rounded_nearest() const
	{
		int f = 1;
		int p = pence;
		if( p < 0 ) {
			p = - p;
			f = - 1;
		}
		i32 pnd = (p + 50)  / 100 ;
		return f * pnd;
	}

	void amnt_t::get_nocurr_nosign_nopence_str( buffer1 & buff ) const
	{
		i32 pnd = pounds_rounded_nearest();
		int p = pnd * 100;
		if( p < 0 ) 
		{
			p = - p;
			// SIGN ALREADY DONE // buff.put_byte( "-" );
			// eg (negative)
		}
		i32 l = p / 100;
		p = p % 100;

		i32 th = l / 1000;
		l = l % 1000;

		i32 mil = th / 1000;
		th = th % 1000;

		if( mil )
		{
			buff.print( "%d,%3.3d,%3.3d", mil, th, l );
		} else if( th )
		{
			buff.print( "%d,%3.3d", th, l );
		} else
		{
			buff.print( "%d", l );
		}
	}

	str0 amnt_t::get_nocurr_nosign_nocomma_str( buffer1 & buff ) const
	{
		i32 p = pence;
		if( p < 0 ) 
		{
			p = - p;
			// SIGN ALREADY DONE // buff.put_byte( "-" );
			// eg (negative)
		}
		i32 l = p / 100;
		p = p % 100;

		i32 th = l / 1000;
		l = l % 1000;

		i32 mil = th / 1000;
		th = th % 1000;

		if( mil )
		{
			buff.print( "%d%3.3d%3.3d.%2.2d", mil, th, l, p );
		} else if( th )
		{
			buff.print( "%d%3.3d.%2.2d", th, l, p );
		} else
		{
			buff.print( "%d.%2.2d", l, p );
		}
		return buff;
	}

	str0 amnt_t::get_nocurr_nosign_str( buffer1 & buff ) const
	{
		i32 p = pence;
		if( p < 0 ) 
		{
			p = - p;
			// SIGN ALREADY DONE // buff.put_byte( "-" );
			// eg (negative)
		}
		i32 l = p / 100;
		p = p % 100;

		i32 th = l / 1000;
		l = l % 1000;

		i32 mil = th / 1000;
		th = th % 1000;

		if( mil )
		{
			buff.print( "%d,%3.3d,%3.3d.%2.2d", mil, th, l, p );
		} else if( th )
		{
			buff.print( "%d,%3.3d.%2.2d", th, l, p );
		} else
		{
			buff.print( "%d.%2.2d", l, p );
		}
		return buff;
	}

	// rename put_byte or print, get should return buff's contents as val (no clear() used) ???
	void amnt_t::get_curr_str( buffer1 & buff ) const
	{
		// this generates a multipbyte warning, its not utf8
		static const uchar GBP = 0xA0;
		buff.put_byte( GBP );
		buff.put_byte( ' ' );
		get_nocurr_str( buff );
	}

	str0 amnt_t::get_curr_str() const
	{
		static buffer1 buff;
		buff.clear();		// ???
		get_curr_str( buff );
		return buff;
	}

/*!
	compare this with parameter

	read result from left to right A1.cmp(a2) --> A1 IS_LESS not a2
*/
	IS_DIFF_t amnt_t::cmp( const amnt_t & a2 ) const
	{
		// possible overflow with subtractions
		if( pence == a2.pence ) return IS_SAME;
		if( pence <  a2.pence ) return IS_LESS;
		return IS_MORE;
	}

	bool amnt_t::add( const amnt_t & val )
	{
		if( curr != val.curr )
		{
			return FALSE_dgb_fail( "Different Currencies" );
		}
		pence += val.pence;
		return true; // curr matched
	}

	bool amnt_t::sub( const amnt_t & val )
	{
		if( curr != val.curr )
		{
			return FALSE_dgb_fail( "Different Currencies" );
		}
		pence -= val.pence;
		return true; // curr matched
	}

	void amnt_t::negate()
	{
		pence = - pence;
	}

	// the calculator is so stupid
	// it puts 0.25 ratios into currency 25p
	// but that seems OK for now
	// add parameter typed flt80

	bool amnt_t::mult( const amnt_t & val )
	{
		if( curr != val.curr )
		{
			return FALSE_dgb_fail( "Different Currencies" );
		}
		double dbl = pence * val.pence;
		dbl /= 100.0;
		set_pence( dbl );
		return true; // curr matched
	}

	bool amnt_t::divide( const amnt_t & val )
	{
		if( curr != val.curr )
		{
			return FALSE_dgb_fail( "Different Currencies" );
		}
		double dbl = (100.0 * pence) / val.pence; // non zero !
		set_pence( dbl );
		return true; // curr matched
	}

