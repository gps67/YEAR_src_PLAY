#ifndef flt80_2048_pair_H
#define flt80_2048_pair_H

#include "dset_api.h"
#include "flt80_2048.h"
#include "stat_calc.h"

typedef long double flt80;
// typedef double flt80;

/*!
	two parallel arrays, val and count, used to collect graph data
	over the specified range

	The dset_api base allows it to receive data with merge_floats().
	Each merge_point_a() drops a value onto a slot, which accumulates
	the val, and increments the count.

	The x-axis is usually time, but could have any stretch, and
	uses float for generality. There are some alias functions,
	such as t_lo() which is effectively x_lo
*/
struct flt80_2048_pair : public dset_api
{
	struct flt80_2048 val;
	struct flt80_2048 count;

	static const int N2048 = flt80_2048::N2048;

	int N() const { return val.N(); }

	stat_calc calc1;	// build variance

	// now add some graph scalings

	flt80 x_lo;		//! left edge
	flt80 x_step;		//! seconds per array[idx] step

	//! x_hi is just past the end of the array
	flt80 x_hi() const
	{
		return x_lo + x_step * N();
	}

	//! t_lo is x_lo 
	time_t t_lo() const
	{
		return (time_t) x_lo;
	}
	//! t_hi is x_hi
	time_t t_hi() const
	{
		return (time_t) x_hi();
	}

	~flt80_2048_pair() {}
	flt80_2048_pair();
	// flt80_2048_pair( flt80 _x_lo, flt80 _x_hi, int _n );

	void init_1 ( time_t t_lo, time_t t_hi, int _n );
	void init_0 ( flt80 _x_lo, flt80 _x_hi, int _n = N2048 );

	/*!
		find the x slot for time value, or -1 if out of range
	*/
	int idx_of_xval( flt80 xval )
	{
		int idx = (int)floor(( xval - x_lo ) / x_step );
		if( ( idx < 0 ) ||( idx >= N()) )
		{
			if(1) e_print("idx %d out of range [0..[%d, setting to -1\n",
				idx, N() );
			idx = -1;
		}
		return idx;
	}

	/*!
		find the time value for the slot (left not mid point)
	*/
	flt80 xval_of_idx( int idx )
	{
		/// if( (idx < 0) || (idx >= N() ) {...}
		flt80 v = idx;
		v *= x_step;
		v += x_lo;
		return v;
	}

	void merge_point_a( flt80 xval, flt80 yval, flt80 cnt = 1.0 );

 virtual // dset_api
	bool merge_floats(
		mem_line_spec * spec,
		time_t t0,
		int n,
		float * flts
	);

	/*
		now it becomes more dubious ...
	*/

	//! allow caller to force count higher - remember to subtract later !
	void count_init_1()
	{
		count.init_0( count.N(), 1.0 );
	}
	void count_add_1()
	{
		count.each_add(  1.0 );
	}
	void count_sub_1()
	{
		count.each_add( -1.0 );
	}

	// normalise each point so that count = 1 (or 0 )
	void plain_avg();
	void first_avg(int off=1);
	void smooth(int off=1);
	flt80 smudge(int wing=1);
	flt80 smudge3();
	
	void find_hi_lo( flt80 & y_hi, flt80 & y_lo );
	void calc_vary( flt80 & y_avg, flt80 & y_vary );

};

#endif
