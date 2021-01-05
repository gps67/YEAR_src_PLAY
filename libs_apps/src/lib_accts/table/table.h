#ifndef table_H
#define table_H

// PESKY - these must be in namespace
namespace ACCTS { class col_spec; };

//	pre-draft

/*
 * 	A tbl is a hash table of rows, with various sorted indexes
 * 	tbl also helps with the CSV load and save
*/

#include "obj_hold.h"
#include "obj_list.h"
#include "obj_ref_js.h"
#include "str2.h"
#if 1
#include "HT_table.h"
// #include "ht2.h" // ht_specs
#else
#include "ht2.h"
#endif
// #include <stdio.h>
#include "ints.h"	// N_ARY

// #include "io_csv_accts.h"
// #include "tbl_col_spec.h"
// #include "tbl_row_inst.h"
// #include "tbl_cell_base.h"

namespace ACCTS {

class io_csv_accts;

#define REGISTER_FIELD_NAMES(init, ary) \
	register_field_names( init, N_ARY(ary), ary )

class data_set; // circular includes for this

class col_spec_amnt; // return type check

/*!
	table stores the data and info about it

	A table is a list of rows, where each row has an internal
	structure described by the list of col_spec's.
*/
class table : public obj_ref_js {
 public:

	//! uplink to the sibling tables
	data_set *		dset;

	//! unique identifier for the table
	obj_hold<str2>		name;

	/*!
		the key field
	*/
	obj_hold<col_spec>	key_spec;

	/*!
		The "dotdot" col within this table that groups leaves
		The group col is only used for self grouping tables,
		its the column within each row that names the parent key.

		EG The 'CAT' table has a ""dotdot" column.
		Previously it could be any name.
	*/
	obj_hold<col_spec>	group_spec;

	/*!
		The column which is describes this row.
		This selector is used to expand 'key' into
		'key-desc' in group headings.
	*/
	obj_hold<col_spec>	desc_spec;

	//! an array of 'type-spec' of each column
	obj_list<col_spec>	col_specs;

	//! the column that is the preferred tree_column
	obj_hold<col_spec>	col_spec_tree;

	//! the column that is the preferred sort column (usually same as tree)
	obj_hold<col_spec>	col_spec_sort_1;

	//! the column that is the second preferred sort column
	obj_hold<col_spec>	col_spec_sort_2;

	//! hash table of the col_specs by name
	HT_specs		col_map;	// hash table

	//! array of row data instances
	obj_list<row_inst>	rows_list;

	//! hash table of rows by key field
#if 1
	HT_row			rows_map;
#else
	ht_row			rows_map;
#endif

	//! numbering the rows allows sorting by input sequence
	int			row_sort_counter;

	//! used during input load
	bool			csv_line1_seen;

	//! reports can be tables but are hidden
	bool			is_hidden;

// METHODS //

	//! create a table called name, added to dset
	table( str2 * name, data_set * _dset );

	//! add a new column to the table - get spec from type str
	col_spec * add_type_col_before( str0 type, str0 name, str0 before );

	//! add a new column to the table - get spec from type str
	col_spec * add_type_col_after( str0 type, str0 name, str0 after );

	//! add a new column to the table - get spec from type str
	col_spec * add_type_col( str0 type, str0 name );

	//! add a new column to the table - guess spec
	void add_col( str0 name );

	//! add a new column to the table
	void add_col( col_spec * spec );

	//! insert a new col_spec and NULL data, adjust other data
	void ins_col( int pos, col_spec * spec );

	//! interpret type string, else guess - PRIVATE
	col_spec * mk_col_spec( str0 type, str2 * name );

	//! fetch the tables name (a private field)
	str2 * get_name() { return name; }

	//! the tables name for debugging messages
	str0 get_name_str() { if(!name) return "ANON"; else return name->sget(); }

	//! check that argv[] (loosely) matches the field names.
	bool check_field_names( int argc, char ** argv );

	//! add a new row_inst to the table from CSV
	bool csv_line_in( io_csv_accts * csv );

	//! find col_spec called key (or NULL)
	col_spec * get_col_spec( str2 * key );
	//! find col_spec called key (or NULL)
	col_spec * get_col_spec( str0 key );

	//! throw if not found
	col_spec * must_get_col_spec( str0 key );

	//! throw if not found
	col_spec_amnt * must_get_col_spec_amnt( str0 key );

	//! find a row from its key (or NULL)
	row_inst * find_row( str2 * key );
	//! find a row from unallocated str
	row_inst * find_row( str0 key );

	bool complain_row_not_found( str0 key );
	row_inst * find_row_or_FAIL( str0 key );

	//! find a row from its key, creating one if absent
	row_inst * find_row_add_fake( str2 * key ); 
	//! find_row_add_fake, with fewer mallocs
	row_inst * find_row_add_fake( str0 key ); 

 protected:
	//! add to list and index, getting key from row
	bool add_row( row_inst * row );

	//! fill in any blank values with ...
//	void fill_row( row_inst * row );

 public:

	//! get the key FIELD as str2
	str2 * get_key_of_row( row_inst * row );

	//! get the desc FIELD as str2 (or NULL)
	str2 * get_desc_of_row( row_inst * row );

	//! set the description field (used for VAT_ACCT)
	bool set_desc_of_row( row_inst * row, str0 s );

	//! get the key_and_desc as str2
	str2 * get_key_desc_of_row( row_inst * row );

	//! get the row that is the local-group-parent of this row (or NULL)
	row_inst * get_local_group_of_row( row_inst * row );

	void delete_row( row_inst * r );

	void del_row( str0 row_key );
	void del_row( str2 * row_key );

	void prints();

	table * new_clone( str2 * name2 );
	table * new_clone( str0   name2 );

	void sort_by_col_spec(
		col_spec * spec0,
		col_spec * spec1 = NULL,
		col_spec * spec2 = NULL,
		col_spec * spec3 = NULL
	);
	void sort_by_col_names(
		str0 name0,
		str0 name1 = (STR0) NULL,
		str0 name2 = (STR0) NULL,
		str0 name3 = (STR0) NULL
	);

 virtual
	~table();
};

// extern table * new_acct_tbl2( data_set * dset );

}; // NAMESPACE
#endif
