#include "tbl_col_spec_plus.h"

namespace ACCTS {

bool col_spec::set_BASE( row_inst * row, str0 s )
{
	obj_ref * cell = new_cell_from_str( s );
	set_obj( row, cell );
	// WHY IS A NULL CELL A PROBLM ? // if( ! cell ) return FALSE;
	return TRUE;
}
bool col_spec::set_from_csv( row_inst * row, str0 s )
{
	return set_BASE( row, s );
}
bool col_spec::set_from_edit( row_inst * row, str0 s )
{
	return set_BASE( row, s );
}

// - new_cell_from_str 

obj_ref * col_spec_link::new_cell_from_str( str0 s ) {
/*
	input is a key into a second table - or the same
	output a reference to that row
	(That gets stored as the cell value)
*/
	if( (strcmp("-",s)==0)
	 || (strcmp("",s)==0)
	) {
		return NULL;
	}
	table * tbl2 = get_linked_table();
	if(!tbl2)
	{
		throw "new_cell_from_str() NULL tbl2";
		return NULL;
	}
	if( tbl == tbl2 )
	{
		// throw "new_cell_from_str() tbl == tbl2";
		// return NULL;
	}
	row_inst * r;
	r = tbl2->find_row_add_fake( s );
	return (obj_ref *) r;
}

/*!
	This is used to CHANGE an existing keys value
*/
bool col_spec_str2_key::set_BASE( row_inst * row, str0 s0 )
{
	table * t = tbl;
	if(!t) {
		prints();
		return FALSE;
	}

	if( ! row ) {
		e_print("key set_BASE() NULL row\n");
		prints();
		return FALSE;
	}

	// convert '+' to '#-1234' (KNOW following obj_ref is a str2)
	obj_hold<str2> new_key;
	new_key = (str2 *) new_cell_from_str( s0 );
	if( !new_key ) {
		e_print( "ERROR: null new_key in %s\n", __PRETTY_FUNCTION__ );
		return FALSE;
	}

	// the (str2 *) is not a cast
	// it ensures that the POINTER is copied not the obj_hold
	// which works but I'm avoiding the copy constructor

	obj_hold<str2> old_key; old_key = (str2 *) tbl->get_key_of_row( row );

	if( old_key ) {
		if( old_key->is_same_as( *new_key ) ) return TRUE;
	}

	// dummy debugging section, no vars created
	{
		row_inst * old_row = tbl->rows_map.lookup( old_key );
		if( ! old_row )
		{
			e_print("set_BASE() old_row =- NULL\n" );
			return FALSE;
		}
		if( old_row != row ) {
			e_print("set_BASE() old_row != row\n" );
			return FALSE;
		}
	}

	int LURK_convoluted_access_to_tables_maps;

	row->xx_set_key( new_key );
	set_obj( row, new_key );
	if( ! tbl->rows_map.rename( old_key, new_key )) return FALSE;;
	return TRUE;
}

}; // NAMESPACE
