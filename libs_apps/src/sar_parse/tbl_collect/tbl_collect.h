#ifndef tbl_collect_H
#define tbl_collect_H
/*
	tbl_collect is several cols-of-tbl_col_floats
	each col has a spec which is NOT YET RESOLVED
	That helps because SQL might not be there and because dt not known
*/

#include <stdio.h>
#include "mem_line_spec.h"

class tbl_collect;

#include "tbl_col_floats_base.h"
#include "dyn_array.h"

class dset_api;

/*!
	Collect/hold a table of columns of dt separated floats
*/
class tbl_collect : public GRP_sar_parse
{
 public:
	//! most columns share similar spec1 (this host, this cmd)
	mem_line_spec spec1;
	//! most columns share similar spec1 (this host, this cmd)
	time_t time0;

	//! The collection of columns (each pointer must be at a fixed address)
	dyn_array<tbl_col_floats_base *> data;
	//! ncol == data.N() // but maybe a bit faster
	int ncol;

	tbl_collect(
		const char * site_name,
		const char * cmd_name
	);

	//! tbl_collect DOES own the data - sar_parser_temps does NOT
	~tbl_collect();

	tbl_col_floats_base * create_col(
		const char * opt,
		const char * dev,
		const char * attr
	);

	bool get_space( int extra_rows );


	bool prep_next_row();
	bool undo_prep_next_row();

	void set_time0(time_t t0);
	void set_t0_in_all_cols(int t0);
	void set_dt_in_all_cols(int dt);
	void set_host_in_all_cols(const char * host);

	bool send_col_to_dset( int col, dset_api * dset ); 
	bool send_to_dset( dset_api * dset ); 

	void zxxx_prints(FILE * f); // debug dump current row
	void zxxx_dumps( FILE * f );
};

#endif
