
#include "flt80_2048_pair.h"
#include "stat_calc.h"


/*!
	constructor - you later init it properly
*/
flt80_2048_pair::flt80_2048_pair()
: val( 0 )
, count( 0 )
{
}

/*
	big mystery - passing TWO flt80 on stack,
	the second parameter became ZERO, ONLY in INLINE.
	?? Alignment on stack ? Hence time_t prototype too
*/

/*!
	late init - specifying the X axis range and grain
*/
void flt80_2048_pair::init_0 ( flt80 _x_lo, flt80 _x_hi, int _n )
{
	assert( _n <= N2048 );
	assert( _n > 0 );
	x_lo = _x_lo;
	x_step = (_x_hi - _x_lo);
	x_step /= _n;
	val.init_0( _n, 0.0 );
	count.init_0( _n, 0.0 );
	calc1.init();
}

/*!
	late init - specifying the X axis range and grain (time_t units)
*/
void flt80_2048_pair::init_1 ( time_t t_lo, time_t t_hi, int _n )
{
	flt80 x_lo = t_lo;
	flt80 x_hi = t_hi;
	init_0 ( x_lo, x_hi, _n );
}

/*!
	merge a point into the array

	This workd for a single data-val, also for an already accumulated
	multi-count.
*/
void flt80_2048_pair::merge_point_a( flt80 xval, flt80 yval, flt80 cnt )
{
	int xidx = idx_of_xval( xval );

	// out of range got converted to  -1
	if( xidx < 0 ) {
		e_print("merge_point_a(%6.3f,%6.3f) xidx<0\n",
			(float) xval, (float) yval 
		);
		return;
	}

	// e_print( "yval %f\n", (float) yval );

	val[ xidx ] += yval;
	count[ xidx ] += cnt;

	// LURK this is all wrong
	if( cnt > 0.6 ) calc1.incl( yval );
}

/*!
	dset_api virtual - add segment of data to the array
*/
bool flt80_2048_pair::merge_floats(
	mem_line_spec * spec,
	time_t t0,
	int n,
	float * flts
){
	// t2 is the right of the segment
	int dt = spec ->dt;
	time_t t2 = t0 + dt * n;

	int i1 = 0; // i1 is the left index 
	int i2 = n; // i2 is the index just off right

	// trim down i1
	if( t0 < t_lo() )
	{
		i1 = ( t_lo() - t0 ) / dt;
	}

	// trim down i2, if the segment exceeds the array
	if( t_hi() < t2 )
	{
		i2 = (t_hi() - t0 ) / dt; // excludes t_hi()
		assert( i2 <= n );
	}

	// load the data into (a part of) the array
	for( int i=i1; i<i2; i++ )
	{
		time_t tx = t0 + (dt*i);
		flt80 xval = tx;
		flt80 yval = flts[i];
		flt80 cnt  = 1.0;	// count = factor
		merge_point_a( xval, yval, cnt );
	}

	return TRUE;
}

/*!
	find value range
*/
void flt80_2048_pair::find_hi_lo( flt80 & y_hi, flt80 & y_lo )
{
	/*
		This might not be the correct func to call.

		What if the array isnt full, val[i] might be 0

		Has the count been normalised to 1 ?

	*/
	val.find_hi_lo( y_hi, y_lo );
}

/*!
	AVOID: need x*x of each value BEFORE it was added to arry
*/
void flt80_2048_pair::calc_vary( flt80 & y_avg, flt80 & y_vary )
{
	calc1.calc_vary( y_avg, y_vary );

	e_print("N=%6.3f AVG=%6.3f VAR=%6.3f\n",
		(float) calc1.count,
		(float) y_avg,
		(float) y_vary
	);
}


/*!
	normalise each point so that count = 1 (or 0 )

	normalise and smudge doesnt quite look like I expect.
	Its not so bad when the area if fully covered.

	normalising the count reduces the benefit of count-weighting
*/
void flt80_2048_pair::plain_avg()
{
	int n = N();
	for( int i=0; i< n; i++ )
	{
		if( count[i] > 0.55 ) {
			val[i] /= count[i];
			count[i] = 1;
		} else {
			val[i] = 0;
			count[i] = 0;
		}
	}
}

/*!
	HMMM
*/
void flt80_2048_pair::first_avg(int off )
{
	smooth(off);
	plain_avg();
}

/*!
	HMMM
*/
flt80 flt80_2048_pair::smudge(int wing )
{
	val.smudge( wing );
	return count.smudge( wing );
}

/*!
	HMMM
*/
flt80 flt80_2048_pair::smudge3()
{
	val.smudge3();
	return count.smudge3();
}

/*!
*/
void flt80_2048_pair::smooth(int off )
{
	val.add_right(off);
	count.add_right(off);
	val.add_left(off);
	count.add_left(off);
}

