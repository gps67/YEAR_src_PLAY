#ifndef flt80_2048_H
#define flt80_2048_H

#include <math.h>

typedef long double flt80;
// typedef double flt80;

// typedef unsigned int size_t; // or is it?
#include <time.h> // time_t
#include <assert.h> // time_t

#include "e_print.h" // g_print
#include "ints.h"

/*!
	flt80_2048 is a fixed size array[2048] of Intel floats.

	Within the fixed size array, you can set int ary_n

	There are some useful functions, to divide each value by V,
	sum-of-all-squared.

	flt80 is an 80 bit float on i386. Its what the FPU/CPU uses, plain
	float and double are convertions. Theres no need for it to be
	exactly 80 bit, (even 32 bit float should be OK), so on different
	CPU's use the most natural form, currently (long double).

	There were a few issues with inline functions not liking more
	than 1 flt80 parameter, and these went away with properly linked
	functions.

	The 2048 fixed range was selected, to allow big dumps into
	a tolerable range, then relay the 2048 onto the screen (715).
	2048 * 10 bytes = 20K each

	Nested averaging can lead to large additions which overflow
	ints almost immediately, floats should be OK.

	smudge() is a vital part of plotting smooth graphs.
*/
struct flt80_2048 : public GRP_sar_plot
{
	static const int N2048 = 2048;
	int ary_n;

	flt80 flts[ N2048 ];
	int N() const { return ary_n; }

	flt80 & operator[](size_t idx )
	{
		return flts[idx];
	}

// 	flt80_2048( const flt80_2048 & orig ); // default copy constructor
	flt80_2048( int _n = N2048, flt80 val0 = 0.0 );
	void init_0 (int _n, flt80 val = 0.0);
	void add_left(int adj);
	void add_right(int adj);
	flt80 smudge(int wing);
	flt80 smudge3();

	void find_hi_lo( flt80 & _hi, flt80 & _lo );
	flt80 calc_sum( );
	flt80 calc_sum_sqr( );
	void each_add( flt80 adj );
	void each_mult( flt80 adj );
	void each_sub( flt80 adj );
	void each_div( flt80 adj ); // silent if div zero
};

#endif
