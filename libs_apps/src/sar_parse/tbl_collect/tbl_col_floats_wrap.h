#ifndef tbl_col_floats_wrap_H
#define tbl_col_floats_wrap_H

#include "tbl_col_floats_base.h"

class tbl_collect;
#include  "tbl_collect.h"
class dset_api;

/*!
	tbl_collect OWNS the data, but the parser is easier to write if
	you have borrowed pointers to its columns.

	This _wrap class holds the borrowed pointer and provides access
	to its ptr() function, so that scanf can get an address. It also
	makes the source cleaner using "." instead of "->"

	Currently, this isnt using refcounting.
*/
class tbl_col_floats_wrap : public GRP_sar_data
{
	tbl_col_floats_base * floats;
public:

	/*!
		destructor - DONT DELETE the data
	*/
	~tbl_col_floats_wrap()
	{
		floats = NULL;
	}

	/*!
		EG sscanf("%f", attr5.ptr() )
	*/
	float * ptr() {
		return floats->ptr();
	}

	/*!
		constructor - request that tbl create a new column,
		but tbl retains ownership of the column and its data
	*/

	tbl_col_floats_wrap(
		tbl_collect * tbl,
		const char * opt,
		const char * dev,
		const char * attr
	) {
		floats = tbl->create_col( opt, dev, attr );
	}


	/*!
		USED WHERE ?
	*/
	mem_line_spec & spec ()
	{
		return floats->spec;
	}
	

/*
	client should not need these as theyre done by tbl
	--------------------------------------------------
	void set_t0( int _t0 )
	{
		floats->set_t0( _t0 );
	}
	void set_dt( int dt )
	{
		floats->set_dt( dt );
	}
	int N() {
		return floats->N();
	}
	void back_fill( int n, float val ) {
		floats-> back_fill( n, val );
	}
	float * base() {
		return floats->base();
	}
	bool prep_next() {
		return floats->prep_next();
	}
*/

/* new construction TBA
	void setup_into_tbl(
		tbl_collect * tbl,
		float init_val = 0.00124
	);
	void setup_opt_dev_attr(
		tbl_collect * tbl,
		const char * opt,
		const char * dev,
		const char * name
	);

	bool send_to_dset( dset_api * dset );	done by TBL
*/

};
#endif

