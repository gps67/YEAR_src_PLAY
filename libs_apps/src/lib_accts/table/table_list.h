#ifndef table_list_H
#define table_list_H

/*
	a list of tables
	? with auto_guess col_spec ? (instead of schema.cxx)
*/

#include "table.h"
#include "obj_list.h"
#include "ht2.h"

namespace ACCTS {

	class data_set;
	class table;

//! a list and hash-table of data-tables
/*!
	The two lists are synchronised,
	but you can remove a table from the list,
	leaving it in the hash-map, so that it isnt saved.
	Though it would be better to have flags on each table.
*/
class table_list : public obj_ref {
 public:
	data_set		* dset;
	obj_list<table>		tbl_list;
	ht_tables		tbl_map;	// hash table
	table_list() {}

	table_list( data_set * _dset ) {
		dset = _dset;
	}


	table * find_add( str0 name ); // new table has no schema yet
	table * find_add( str2 * name );
	table * find_add_hidden( str2 * name );
	table * get( str2 * name ); // NULL
	table * get( str0 name ); // NULL
	void hide_name( str2 * name );
};

}; // NAMESPACE

#endif
