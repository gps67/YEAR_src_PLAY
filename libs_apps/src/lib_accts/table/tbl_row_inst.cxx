#include "tbl_row_inst.h"
#include "table.h"
#include "tbl_col_spec.h"

namespace ACCTS {

void row_inst::prints()
{
	if( !debug_table )
	{
		/*
			a row shouldnt have a weak uplink to its table
			but whilst debugging, it should!
		*/
		e_print("# DBG # row_inst debug_table is NULL\n");
		return;
	}
	int ncol = debug_table -> col_specs.N();
	for( int i=0; i< ncol; i++ )
	{
		col_spec * spec = debug_table -> col_specs[i];
		if( ! spec )
		{
			e_print("# DBG # ERR # table spec[] is NULL\n");
			return;
		}
		str2 * s2;
		str0 s0;
		obj_ref * t = spec->get_obj( this );
		if( t )
		{
			s2 = spec->get_str2_for_display( this );
			if( s2 ) s0 = s2->str;
		}
		else
			s0 = "NULL";
		if(1)
		e_print( "# DBG # Table.key: %s.%-10s = '%s'\n",
			(STR0) debug_table->name->str,
			(STR0) spec->name->str,
			(STR0) s0
		);
	}
	e_print( "\n" );
}

/*!
	the key of this row

	This is unofficial, its not a part of the basic structure,
	but it is nice to be able to print the identifying name
	of the row.

	This is a grungy knowledge that the first field is
	the key, and that its a str2.

*/
str0 row_inst::debug_row_name()
{
	if( !debug_table )
	{
		/*
			a row shouldnt have a weak uplink to its table
			but whilst debugging, it should!
		*/
		e_print("# DBG # debug_row_name: row_inst debug_table is NULL\n");
		prints();
		return "ANON";
	}
	col_spec * spec = debug_table -> key_spec;
	if(!spec) {
		e_print("# DBG # debug_row_name: debug_table.key_spec is NULL\n");
		prints();
		return "ANON";
	}
	// CAN THIS HAPPEN ??
	obj_ref * t = spec->get_obj( this );
	if( !t ) {
		e_print("# DBG # debug_row_name: get_obj returned NULL\n");
		prints();
		return "ANON";
	}
	str2 * s2 = spec->get_str2_for_display( this );
	if(!s2) {
		e_print("# DBG # debug_row_name: get_str2 returned NULL\n");
		prints();
		return "ANON";
	}
	return s2->str;
}




/*!
	This should be in tbl_row_inst.cxx

	Each cell is allowed to be shared, even across tables.
	Typicall its a str2* or ...
*/
row_inst * row_inst:: row_clone()
{
	row_inst * r2 = new row_inst( debug_table, row_sort );
	int nc = cells.N();
	for(int i=0; i< nc; i++ )
	{
		r2->cells.append( cells[i] );
	}
	return r2;
}
}; // NAMESPACE
