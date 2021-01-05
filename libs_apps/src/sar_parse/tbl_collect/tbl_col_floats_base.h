#ifndef FLOATS_BASE_H
#define FLOATS_BASE_H

#include <stdio.h>

#include "mem_line_spec.h"

#include "misc.h"
#include "dyn_array.h"

class tbl_collect;
class dset_api;

/*!
	tbl_col_floats_base is the core data holder of the parser, to
	build a segment of a single line (one column of a table).

	The time series data is floats separated by dt starting at t0.
	You should align T0 to some proper value.
*/
class tbl_col_floats_base : public GRP_sar_data
{
	dyn_array<float> ary;
public:
	mem_line_spec	spec;
	time_t t0;

	/*
		constructor - copy spec1 but change opt,dev,attrname
	*/
	tbl_col_floats_base(
		mem_line_spec * spec1,
		const char * opt,
		const char * dev,
		const char * attr
	);

	/*!
		constructor - init zero - should not be used
	*/
	tbl_col_floats_base()
	{
//		line_id = -1;
		ary.get_space(10);
	}

	/*!
		destructor - default
	*/
	~tbl_col_floats_base()
	{
	}

	/*!
		T0 is the time the command started, with the first
		data item at dt seconds later. This is not set in the
		constructor, because it probably isnt known then.

		An unaligned T0 can cause strange crashes when merging segments.
	*/
	void set_t0( int _t0 )
	{
		t0 = _t0;
		realign_t0();
	}

	/*!
		Its normal to realign T0 to the nearest dt.
	*/
	void realign_t0( int grain = 0 )
	{
		if( grain < 1 )
			grain = spec.dt;
		if( grain < 1 )
			grain = 10;
		t0 = ( ( t0+(grain/2) ) / grain ) * grain; // presume int truncation
	}

	/*!
		dt is the delta-time seconds between readings,
	*/
	void set_dt( int dt )
	{
		spec.dt = dt;
	}

	/*!
		The number of items in the array
	*/
	int N()
	{
		return ary.N();
	}

	/*!
		Sometimes devices 'appear' hours after the command
		started (SAR doesnt report completely idle devices),
		so back-fill all values with zero or val
	*/
	bool back_fill( int n, float val )
	{
		return ary.extend_to_fill( n, val );
	}

	/*!
		dyn_array<float> adds headers but contains
		a simple standard C array. Get the base address
	*/
	float * base()
	{
		return ary.ary_base();
	}

	/*!
		Get the pointer to the LAST item in the array.
	*/
	float * ptr_last()
	{
		return ary.gap_addr() -1 ; // pointer arith -4bytes
	}

	/*
		scanf uses a ptr to an existing float, instead of
		append(newitem)
	*/
	float * ptr() 
	{
		return ptr_last();
	}

	/*!
		early realloc
	*/
	bool get_space( int extra_rows )
	{
		return ary.get_space( extra_rows );
	}

	/*!
		prepare the array for the next reading by extending the
		array by one (which should init 0.0)
	*/
	bool prep_next()
	{
		/*
			ary.extend() calls constructr which inits to zero
			This means calling prep_next BEFORE using any
			That means doing so for ADDED columns ...
		*/
		bool ok = TRUE;
		if(! ary.extend_by(1) )ok = FALSE;	// keep data written into gap
		// DONE BY INIT // *ptr() = 0;		// pre-fill gap
		return ok;
	}

	/*!
		undo a prep_next() which is not needed.

		The parser typically calls prep_next() before calling
		sscanf(), which confirms the valid data line, or detects
		that there is no data, and then calls undo_prep_next()
	*/
	bool undo_prep_next()
	{
		return (ary.extend_by(-1) );
	}

	void setup_opt_dev_attr(
		mem_line_spec * spec1,
		const char * opt,
		const char * dev,
		const char * attr
	);

	void setup_opt_dev_attr(
		tbl_collect * tbl,
		const char * opt,
		const char * dev,
		const char * attr
	);

	bool send_to_dset( dset_api * dset );
};
#endif

