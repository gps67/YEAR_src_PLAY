#include "tbl_col_spec.h"
#include "dset.h"
#include "dyn_array.h"

#include "io_csv_accts.h"
#include "io_csv_out.h"

namespace ACCTS {


/*!
	Create a new data_set - a list of tables
*/
data_set::data_set()
: tbl_list( NULL )
{
	tbl_list = new table_list( this );
}

/*!
	Clear all tables for a new data file.
*/
void data_set::reset()
{
	tbl_list = NULL; // ref counted delete
	/* ref_counting should delete any data
	if( tbl_list )
	{
		delete tbl_list;
	}
	*/
	tbl_list  = new table_list( this );
}

/*!
	load a file through an io_csv_accts helper
*/
bool data_set::file_load( str0 filename ) 
{
	e_print("data_set::file_load('%s');\n", (STR0) filename);
	if(!filename) return FAIL("NULL filename");
	if(!filename) gdb_invoke(1);
	debug_set_obj_name( (STR0) filename );
	/*
	*/
	int n = tbl_list->tbl_list.N();
	for(int i=0; i<n; i++ )
	{
		tbl_list->tbl_list[i]->csv_line1_seen = false;
	}
	/*
	*/
	bool ok = FALSE;
	io_csv_accts io(this);
	ok = io.load_file( filename );
	return ok;
}

/*!
	save a file using an io_csv_out helper.

	Note that the output columns are text aligned,
	which requires this function to recalculate the widths,
	and makes this a long but simple function.
	This presumes monospaced 8859.
*/
bool data_set::file_save( str0 filename, bool compressed ) // AKA save_file
{
	INFO("filename '%s'", (STR0) filename );
	if( ! filename ) return FALSE;

	if( compressed ) WARN("compressed");

	io_csv_out io;
	if( !io.open( filename, compressed ) ) return FALSE;

	// column widths
	dyn_array<int>	widths;
	dyn_array<align_val>	just_align;

	// for each table in tbl_list ...
	int n_tbl = tbl_list->tbl_list.N();
	for( int t=0; t<n_tbl; t++ )
	{
		table * tbl = tbl_list->tbl_list[t];
		if( tbl->is_hidden ) continue;
		int nc = tbl->col_specs.N();
		widths.extend_to( nc );
		just_align.extend_to( nc );

		// start with the widths of the headers
		// and cache the just_fication
		for( int c=0; c<nc; c++ )
		{
			just_align[c] = tbl->col_specs[c]->get_align();
			widths[c] = strlen( tbl->col_specs[c]->get_name()->sget() );
		}

		// measure the col-widths of all data row
		// for each row in tbl ... for each col in row
		int nr = tbl->rows_list.N();
		for( int r=0; r<nr; r++ )
		{
			row_inst * row = tbl->rows_list[r];
			for( int c=0; c<nc; c++ )
			{
				str2 * s2 =
					tbl
					->col_specs[c]
					->get_str2_for_csv( row );
				int w2 = 0;
				str0 s = "";
				if( s2 ) s = s2->sget();
				if( s ) w2 = s.str_len();
				if( widths[c] < w2 ) widths[c] = w2;
			}
		}

		// common first column
		str0 tbl_name = tbl->get_name()->sget();
		INFO("tbl '%s'", (STR0) tbl_name );
		if(! (STR0) tbl_name ) {
			WARN("NULL TABLE NAME");
			continue;
		}
		int tbl_name_w = strlen( tbl_name );

		// OUTPUT column headers
		io.put_field( tbl_name, FALSE, tbl_name_w );
		for( int c=0; c<nc; c++ )
		{
if( tbl->col_specs[c]->is_hidden ) continue;
			io.put_field(
				tbl->col_specs[c]->get_name()->sget(),
				just_align[c],
				widths[c]
			);
		}
		io.eoln();

		// OUTPUT rows
		// int nr = tbl->rows_list.N();
		for( int r=0; r<nr; r++ )
		{
			row_inst * row = tbl->rows_list[r];
			io.put_field( tbl_name, FALSE, tbl_name_w );
			for( int c=0; c<nc; c++ )
			{
if( tbl->col_specs[c]->is_hidden ) continue;
				col_spec * spec = tbl->col_specs[c];
				str2 * s2 = spec->get_str2_for_csv( row );
				str0 s = "";
				if( s2 ) s = s2->sget();
				io.put_field( s, just_align[c], widths[c] );
			}
			io.eoln(); 
		}

		// blank line after each table
		io.eoln(); 
	}
	if( !io.close() ) return FALSE;;
	INFO("HERE");
	INFO("filename '%s'", (STR0) filename );
	return PASS("filename '%s'", (STR0) filename );
	return TRUE;
}

/*!
	find the table called name
*/
table * data_set::get_table( str2 * name )
{
	return tbl_list->get( name );
}

/*!
	find the table called name
*/
table * data_set::get_table(  str0 name )
{
	return tbl_list->get( name );
}

table * data_set:: get_table_or_FAIL(  str0 name )
{
	table * t = get_table( name );
	if(!t) {
		FAIL("absent table %s", (STR0) name );
	}
	return t;
}

/*!
	ensure that the table called name exists, return it
*/
table * data_set::find_add( str2 * name )
{
	return tbl_list->find_add( name );
}

/*!
	ensure that the table called name exists, return it
*/

table * data_set::find_add(  str0 name )
{
	return tbl_list->find_add( name );
}

/*!
	ensure that the table called name exists, return it
*/
table * data_set::find_add_table( str2 * name ) 
{
	return find_add( name );
}


/*!
	a fake table is a hidden one, to hold tables which are not saved
	but are visible within the running program.
*/
table * data_set::find_add_fake_table( str2 * name )
{
	return tbl_list->find_add_hidden( name );
}

/*!
	ensure that the UNSAVED table called name exists, return it
*/
table * data_set::find_add_fake_table(  str0 name )
{
	obj_hold<str2> name2; name2 = new str2( name );
	return find_add_fake_table( name2 );
}

/*!
	hide a table, so that it wont be saved
*/
void data_set::hide_table( str2 * name )
{
	// add is odd, but maybe
	tbl_list->find_add_hidden( name );
/*
	table * tbl = tbl_list->get_table( name );
	if( !tbl ) return;
	tbl->is_hidden = false;
	tbl_list->tbl_map.remove( name );
*/
}

void data_set::hide_table( str2 * name, bool hide )
{
	table * tbl = get_table( name );
	if( !tbl ) return;
	tbl->is_hidden = hide;
	if( hide )
		tbl_list->tbl_map.remove( name );
	else
		tbl_list->tbl_map.add( name, tbl );
}
str2 * data_set::get_table_row_col(
	str0 name_table,
	str0 name_row,
	str0 name_col
)
{
	// GCC version change - jump over initialisation - but should be OK
	table * _table = NULL;
	col_spec * _spec = NULL;
	row_inst * row = NULL;
	str2 * val =  NULL;

	_table = get_table( name_table );
	if(! _table ) goto fail;

	_spec = _table -> get_col_spec( name_col );
	if( !_spec ) goto fail;

	row = _table -> find_row( name_row );
	if( !row ) goto fail;

	val =  _spec -> get_str2_for_display( row );
	if( !val ) goto fail;
	return val;


fail:
	e_print( "## MISSING ## %s.%s.%s\n", (STR0) name_table, (STR0) name_row, (STR0) name_col );
	return NULL;

}


}; // NAMESPACE
