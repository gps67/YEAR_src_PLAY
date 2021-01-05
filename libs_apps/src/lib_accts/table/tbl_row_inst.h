#ifndef table_row_inst_H
#define table_row_inst_H

#include "str2.h"
#include "obj_hold.h"
#include "obj_list.h"
#include "obj_ref_js.h"
#include "table.h"

namespace ACCTS {

//! a row instance
/*!

	The row_inst holds the cell values, and the row_sort weight,
	but it doesnt hold a pointer to the table.

	It might be possible to completely replace row_inst with
	dedicated C-structs, using col_spec's to figure out the data
	access.

	TODO: remove the dupliate of the key

	TODO: remove is_group_row
*/
class row_inst : public obj_ref_js
{

	//! the key field is repeated - why - it must be a str2
/*
	obj_hold<str2>		x9_key;
*/

 protected:
	friend class col_spec; // but sub-classes cant access key
	friend class col_spec_str2_key; // but sub-classes cant access key
	friend class table;
	friend class sort_spec_one;

	//! a list of cell values
	obj_list<obj_ref>	cells;

	//! the sort-weight of this row in the table
	int			row_sort;

//	//! is this row a leaf, or a group naode (in its own table)
//	bool			is_group_row; // col_spec treats differently

	table * debug_table;

 public:

	int get_row_sort()
	{
		return row_sort;
	}

	void xx_set_key( str2 * k )
	{
/*
		x9_key = k;
*/
	}

	/*!
		the key of this row

		This is unofficial, its not a part of the basic structure,
		but it is nice to be able to print the identifying name
		of the row.

		This is a grungy knowledge that the first field is
		the key, and that its a str2.

	*/
	str0 debug_row_name();

	str0 xx_key_str()
	{
		return "/key_str/";
/*
		if(!x9_key) return "NULL-KEY!";
		return x9_key->get();
*/
	}

	row_inst( table * tbl, int row_pos )
	{
		debug_table = tbl;
		row_sort = row_pos;	// set by csv reader ?
//		is_group_row = FALSE;
		row_flags = 0;
	}

	void prints();
//	{
		//e_print( "%s", (STR0) debug_row_name() );
	//}

	row_inst * row_clone();

	/*!
		tbl controls the generation of key - desc
		(as used in combo items) but the row can hold
		the values for reuse
	*/
	obj_hold<str2>	cache_key_desc;

	/*!
		row_flags could hold things like: is_leaf, is_group,
		display fgbg, ...
	*/
	u32 row_flags;
};

}; // NAMESPACE
#endif
