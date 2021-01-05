#ifndef tbl_iter_H
#define tbl_iter_H

#include "obj_hold.h"
#include "dset.h"
#include "tbl_col_spec_plus.h"
#include "dgb_fail.h"
#include "tbl_col_spec_date_rel.h"

namespace ACCTS {
/*
	Design change - again
	A more things unravel, I cant escape making the fields hardcoded
	(with a dynamic 'any-others' list). As a feeler, and patch whilst
	that isnt so, the table/rows are loaded into these structures.
	In future the IO_CSV will load into structures, where row_inst
	is derived from/points to the fixed structure. Each field
	will be registered with a get/set function (etc)

	Interestingly JAVA reflection could build the get/set list,
	python could do something, and C++ would require a CORE fixed list.
*/

struct tbl_iter : public GRP_lib_accts
{
	table * tbl;
	int r;
	int nr;
	row_inst * row;

	tbl_iter()
	{
		tbl = NULL;
		row = NULL;
		r = -1;
		nr = 0;
	}
	tbl_iter( table * _tbl )
	{
		tbl = NULL;
		row = NULL;
		r = -1;
		nr = 0;
		set_tbl( _tbl );
	}

	void set_tbl( table * _tbl )
	{
		tbl = _tbl;
		restart();
	}
	bool finished()
	{
		return (r >= nr );
	}
	bool get_row()
	{
		r++;
		if( r>=nr )
		{
			row = NULL;
			return false;
		}
		if(tbl)
			row = tbl->rows_list[r];
		return true;
	}
	void restart()
	{
		r = -1;
		row = NULL;
		nr = 0;
		if(tbl)
			nr = tbl->rows_list.N();
	}

	bool stay()
	{
		return get_row();
	}
	void step()
	{
	}
	// for( tbl_iter iter( tbl, row ); iter.stay(); iter.step() )
	// 
};

}; // NAMESPACE

#endif
