#include "flt80_2048.h"
#include <math.h>

// typedef unsigned int size_t; // or is it?
#include <time.h> // time_t
#include <assert.h> // time_t

#include "e_print.h" // e_print


/*!
	constructor - init0
*/
flt80_2048::flt80_2048( int _n, flt80 val0 )
{
	init_0( N2048, val0 );
	ary_n = _n;
}

/*!
	init range to val (0.0)
*/
void flt80_2048::init_0 (int _n, flt80 val )
{
	assert( _n <= N2048 );
	assert( _n > 0 );
	ary_n = _n;
	int n = N();
	// over fill anyway
	n = _n;
	for( int i=0; i<n; i++ )
	{
		flts[i] = val;
	}
}

/*!
	smudge by adding (non-) adjacent values together
*/
void flt80_2048::add_left(int adj)
{
	if(!adj) return;
	if( adj<0 ) { add_right(-adj); return; }
	int n = N();
	n-=adj;
	for(int i=0; i<n; i++ )
	{
		flts[i] += flts[i+adj];
		flts[i] /= 2;
	}
}

/*!
	smudge by adding (non-) adjacent values together
*/
void flt80_2048::add_right(int adj)
{
	if(!adj) return;
	if( adj<0 ) { add_left(-adj); return; }
	int n = N();
	n-=adj;
	// non-overlapping right to left
	for(int i=n-1; i>=0; i-- )
	{
		flts[i+adj] += flts[i];
		flts[i+adj] /= 2;
	}
}

/*!
	smudge by .. experimental
*/
flt80 flt80_2048::smudge3()
{
	flt80 L;
	flt80 M;
	flt80 R;
	/*
		overwrites both values so keeps both values
	*/
	if( N() < 2 ) return 1.0;

	L = flts[0];
	M = flts[1];
	R = flts[0];

	if( N() == 2 ) {
		flts[0] = flts[1] = M + R;
		return 2.0;
	}

	for( int i=0; i<N()-1; i++ )
	{
		L = M;			//	1	0	1	2
		M = R;			//	0	1	2	3
		R = flts[i + 1 ];	//	1	2	3	4

		flts[i] = L + M + R;	//	0	1	2	3
	}
	
	flts[ N() - 1 ] = L + M + R;
	return 3.0;
}

/*!
	smudge by .. experimental

	if wing is 2, 5 values are added together: 2 left, mid, 2 right
	and the result NOT divided by 5, but 5 is returned as the xs
	factor for you to divide by (you can repeat and combine).

	The entire 20K is copied into the temp 'plain' array, so that
	our own results are not reused (maybe there should be a second
	parameter).

	The running sum-of-5, means that we only need to add 1 value and
	sub 1 value per step, not add 5 each time.

	At the end-stops, some fixup has to take place, which can lead
	to misleading trend lines, so try to overshoot by 1 hour (10%)
	on both sides, and ignore those ranges.

	Note nested calls, a smudge(20) calls smudge(10) which calls ...
	This gives much better results, particularly at the end ranges,
	and when single values have big steps.
*/
flt80 flt80_2048::smudge(int wing)
{
	e_print("smudge(%d)\n", wing );
	// experimental, optimistation possible
	if( wing < 2 ) return smudge3();

	// caller must divide by xs
	flt80 xs = 1.0;	// factor
	/*
		nested calls, a smudge of 20 calls 10 calls 5 calls 2 calls smudge3
	*/
	xs *= smudge( wing / 2 );

	// copy the entire arrray, to retain the original values
	flt80_2048 plain ( *this );
	// The number of items in the running sum
	int n_add = wing + 1 + wing;
	// not enough points (eg wing == 3000), return as done by sub-calls
	if( N() < n_add ) return xs;
	// caller must divide by xs
	xs *= n_add;

	// left and right endstops, hi is off by 1
	int lo = wing;
	int hi = N() - wing;

	// setup initial sum
	flt80 sum = 0.0;
	for( int i = 0; i < n_add; i++ )
	{
		sum += plain.flts[ i ];
	}

	// left range needs its own _something_
	for( int i = 0; i < lo; i++ )
	{
		flts[i] = sum;
		if(0) for( int j = 0; j < n_add; j++ )
		{
			flts[i] += plain.flts[ 0 + j ];
		}
	}

	// remove leading edge that loop adds in (to make n_add items)
	if( lo < hi ) sum -= plain.flts[ lo + wing ];

	// main sweep - CHECK THIS - one to many already in?
	for( int i = lo; i < hi; i++ )
	{
		// add in the new point - before its used
		sum += plain.flts[ i + wing ];
		flts[ i ] = sum;
		// remove the old point after its used
		sum -= plain.flts[ i - wing ];
	}

	// right range needs its own _something_
	for( int i = hi; i < N(); i++ )
	{
		flts[i] = sum;
		if(0) for( int j = 0; j < n_add; j++ )
		{
			flts[i] += plain.flts[ N() - 1 - j ];
		}
	}
	return xs;
}

/*!
	Return the highest and lowest values
*/
void flt80_2048::find_hi_lo( flt80 & _hi, flt80 & _lo )
{
	flt80 hi = flts[0];
	flt80 lo = flts[0];
	int n = N();
	for( int i=1; i<n; i++ )
	{
		if( flts[i] < lo )
			lo = flts[i];
		else if( flts[i] > hi )
			hi = flts[i];
	}
	_hi = hi;
	_lo = lo;
}

/*!
	add val to each
*/
void flt80_2048::each_add( flt80 adj )
{
	int n = N();
	for( int i=0; i<n; i++ )
	{
		flts[i] += adj;
	}
}

/*!
	multiply each by val
*/
void flt80_2048::each_mult( flt80 adj )
{
	int n = N();
	for( int i=0; i<n; i++ )
	{
		flts[i] *= adj;
	}
}

/*!
	subtract val from each
*/
void flt80_2048::each_sub( flt80 adj )
{
	each_add( - adj );
}

/*!
	divide each by val - if not zero
*/
void flt80_2048::each_div( flt80 adj )
{
	if( fabs( adj ) < 0.0000000001 ) 
	{
		e_print("flt80_2048::each_div( %f )\n", (float) adj );
		return;
	}
	each_mult( ((flt80)1.0)/adj );
}

/*!
	sum of all items in the array
*/
flt80 flt80_2048::calc_sum( )
{
	flt80 sum = 0;
	int n = N();
	for( int i=0; i<n; i++ )
	{
		sum += flts[i];
	}
	return sum;
}

/*!
	sum of all (item-squared)
*/
flt80 flt80_2048::calc_sum_sqr( )
{
	flt80 sum = 0;
	int n = N();
	for( int i=0; i<n; i++ )
	{
		sum += ( flts[i] * flts[i]);
	}
	return sum;
}
