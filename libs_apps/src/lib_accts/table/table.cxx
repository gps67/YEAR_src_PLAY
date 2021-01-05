#include "table.h"
#include <stdio.h>
#include "dset.h"
#include "tbl_col_spec_plus.h"
#include "io_csv_accts.h"
#include "str_builder.h"
#include "buffer2.h"
// DONE // #include "dgb.h"

namespace ACCTS {

/*!
	destroying a table is automatic, the fields
	are obj_hold and delete themselves.
*/
table::~table()
{
	if(0) fprintf(stderr,"DEL table %s\n", (STR0) name->sget());
	// delete rows_map -->?
}

/*!
	Tables are created by table_list, which registers this new object
	into the dset context. Theres not much local initialisation,
	and most fields are automatic.
*/
table::table( str2 * _name, data_set * _dset )
{
	dset = _dset;
	name = _name;
	csv_line1_seen = FALSE;	// one shot load 
	row_sort_counter = 0;
	key_spec = NULL;
	is_hidden = false;
}

void table::add_col( str0 name )
{
	if( name.is_empty() ) {
		e_print("# BAD # add_col('') - null name, maybe trailing comma\n");
		name = "MISSINGNAME";
	}
	obj_hold<str2> colname; colname = new str2( name );
	col_spec * spec;
	// LURK - key field has to be suitable
	spec = dset->guess_col_spec( this, colname );
	add_col( spec );
	// guess_col_spec
}

col_spec * table::mk_col_spec( str0 type, str2 * name )
{
	/*
		TODO: move this to col_spec_base static function ...
		see amnt/T_field.h
		see tbl_col_spec_bases.h
		see __enum_ as pointers to named integers as well as direct code

		the idea would be to define the acceptable list of names
		that can create T_fields, and then the list of acceptable
		T_fields which make sense with the information available.

		Then add some aliases or things that are specific to the
		table. eg "key" becomes "T_str_key".

		Then i18n

		However, that then leaves the unfactored need to attach
		the col_spec to the table at time of creation, ie to
		know its table, name and (PARTIALLY) pos or offset
	*/
	col_spec * spec = NULL;
	if( type == "key" )
		spec = new col_spec_str2_key( name );
	else if( type == "date" )
	{
		obj_hold<str2> name2 = new str2( type );
		spec = dset->guess_col_spec( this, name2 );
		spec->name = name;
	}
	else if( type == "amount" )
		spec = new col_spec_amnt( name );
	else if( type == "percent" )
		spec = new col_spec_percent( name );
	else if( type == "string" )
		spec = new col_spec_str2( name );
	else if( type == "dotdot" )
		spec = new col_spec_link_dot_dot( name );
	else if( type == "img_id" )
		spec = new col_spec_img_id( name );
	else
		spec = dset->guess_col_spec( this, name );
	return spec;
}

col_spec * table::add_type_col( str0 type, str0 name0 )
{
	/*
		type is "amount" or "date" or .... see mk_col_spec

		you call this function, not mk_col_spec,
		it doesnt register the spec properly,
		nor create the data in the table (so other cols are shifted)

		or call the _before varients ...
	*/
	obj_hold<str2> name = new str2( name0 );
	col_spec * spec = NULL;
	// accts_js uses this, maybe others could too!
	if(( spec = get_col_spec( name ) )) {
		e_print("# WARN # add_type_col('%s','%s') - already exists\n",
			(STR0) type, (STR0) name0 );
		return spec;
	}
	spec = mk_col_spec( type, name );

	add_col( spec );
	return spec;
}

/*!
	add a rightmost column (spec) and fill table with blank data
*/
void table::add_col( col_spec * spec )
{
	if(!spec) {
		return_VOID_dgb_fail( "add_col( NULL )" );
	}
	int pos = col_specs.N();

	col_specs.append( spec );
	spec->bind_as_tbl_col( this, pos );
	col_map.add( spec->name, spec ); 

	// append a column of data
	int n = rows_list.N();
	for( int i = 0; i < n; i++ )
	{
		rows_list[i]->cells.append( NULL );
	}

	// allow first to be key WHATEVER TYPE ?? LURK
	// maybe do this as some sort of fixup later?
	// particularly when a field named "key" is GUESSED to be key
	if(!key_spec)
		key_spec = spec;
}

col_spec * table::add_type_col_before( str0 type, str0 name0, str0 before )
{
	/*
		_before is rarer than _after
	*/
	e_print( "# INFO # table::add_type_col_before( %s, %s, %s )\n",
		(STR0) type,
		(STR0) name0,
		(STR0) before
	);

	// normalise name
	obj_hold<str2> name = new str2( name0 );

	// check that name does not already exist 
	col_spec * spec = NULL;
	if(( spec = get_col_spec( name ) )) {
		e_print("# WARN # add_type_col('%s','%s') - already exists\n",
			(STR0) type,
			(STR0) name0
		);
		return spec;
	}

	// figure where to put the column
	int pos = 0;
	col_spec * spec_right = get_col_spec( before ); 
	if( !spec_right ) {
		e_print( "# FAIL # table::add_type_col_before( %s, %s, /* ===> */ %s ) cant find '%s'\n",
			(STR0) type,
			(STR0) name0,
			(STR0) before,
			(STR0) before
		);
		spec_right = key_spec;
	}
	if( spec_right ) {
		pos = spec_right -> col_pos - 1;
	}

	// create a blank spec into this table
	spec = mk_col_spec( type, name );
	// spec->bind_as_tbl_col( this, pos ); // later

	// add the spec to the list and the data to each rows data
	ins_col( pos, spec );
	return spec;
}

col_spec * table::add_type_col_after( str0 type, str0 name0, str0 after )
{
	col_spec * spec = NULL;
	e_print( "# INFO # table::add_type_col_after( %s, %s, %s )\n",
		(STR0) type,
		(STR0) name0,
		(STR0) after
	);
	// usually name doesnt exist, so create str2 and keep its hash
	obj_hold<str2> name = new str2( name0 );

	// check that name does not already exist 
	// maybe accts_js uses this, maybe others could too!
	if(( spec = get_col_spec( name ) )) {
		e_print("# WARN # add_type_col('%s','%s') - already exists\n",
			(STR0) type, (STR0) name0 );
		return spec;
	}

	// figure where to put the column
	int pos = 0;
	col_spec * spec_left = get_col_spec( after );
	if( !spec_left ) {
		e_print( "# FAIL # table::add_type_col_after( %s, %s, /* ===> */ %s ) cant find '%s'\n",
			(STR0) type,
			(STR0) name0,
			(STR0) after,
			(STR0) after
		);
		spec_left = key_spec;
	}
	if( spec_left ) {
		pos = spec_left -> col_pos + 1;
	}

	// create a blank spec into this table
	spec = mk_col_spec( type, name );
	// spec->bind_as_tbl_col( this, pos ); // later

	// add the spec to the list and the data to each rows data
	ins_col( pos, spec );
	return spec;
}

void table::ins_col( int pos, col_spec * spec )
{
	int N_col = col_specs.N();
	if( pos < 0 ) {
		// maybe use -2 as append?
		pos = 0;
	}
	if( pos > N_col ) {
		pos = N_col; 
	}

	for( int p = pos; p < N_col; p++ )
	{
		// rebind each col_spec to its new position in the list
		col_specs[p]->col_pos ++;
	}

	spec->bind_as_tbl_col( this, pos ); // sets tbl and col_pos!

	// insert to list
	col_specs.ins( pos, spec );
	col_map.add( spec->name, spec ); // NAME => object

	int n = rows_list.N();
	for( int i = 0; i < n; i++ )
	{
		rows_list[i]->cells.ins( pos, NULL );
	}

	// allow first to be key WHATEVER TYPE ?? LURK ++ split lurk
	if(!key_spec)
		key_spec = spec;
}

/*!
	Used when loading data, to check the FIRST line of names

	Currently the CSV load creates the fields from the file
	where it gets exactly the same argv, and checks them again.

	It could be changed to be more liberal about the order of the
	fields, or to force the datafile to have preset fields,
	maybe adding others if missing.

		each table would have a 'current load order'
		a partial list of specs 
		set by the first line 
		then subsequent data lines would use that not col_specs[]
		and other fields would remain NULL, or something

	The argv fields belong to the CSV caller.
*/
bool table::check_field_names( int argc, char ** argv )
{
	// bool ok = TRUE;

	if( argc != col_specs.N() )
	{
		// const char * tablename = (STR0) name->str;
		const char * tablename = (STR0) * name;
		FAIL("wrong number of fields in table %s expected %d got %d\n",
			tablename,
			col_specs.N(),
			argc
		);
		for( int i = 0; i<argc; i++ )
		{
			FAIL("GOT: '%s'\n", (STR0) argv[i] );
		}
		return FALSE;
	}
	for( int i = 0; i<argc; i++ )
	{
		if( !col_specs[i]->get_name()->is_same_as( argv[i] ))
		{
			return FAIL("expected '%s' got '%s'\n",
				(STR0) col_specs[i]->get_name()->sget(),
				(STR0) argv[i]
			);
		}
	}
	if(0) {
		buffer1 msg;
		for( int i = 0; i<argc; i++ )
		{
			msg.print(" %s", (STR0) argv[i] );
		}
		PASS( "%s", (STR0) msg );
	}

	/*
		can group_spec be NULL ??
	if(!group_spec) 
		group_spec = col_specs[0];
	*/
	if(!key_spec)
	{
		fprintf(stderr, "ALERT: check_field_names() key was NULL" );
		key_spec = col_specs[0];
	}
	return TRUE;
}

/*!
	If the row called 'key' is not found return NULL
*/
row_inst * table::find_row( str0 key ) 
{
	return rows_map.lookup( key );
}


// convert to virtual ... maybe
bool table:: complain_row_not_found( str0 key )
{
	STR0 dset_name = "DSET";
	if(!dset) {
		FAIL("DSET is NULL");
		// never happens, that was a crashing clue
	} else {
		dset_name = dset -> debug_get_obj_name();
	}
	const char * tablename = (STR0) * name;
	return FAIL("dset %s table %s row %s",
		dset_name,
		tablename,
		(STR0) key
	);
	return false;
}

/*!
	If the row called 'key' is not found return NULL
*/
row_inst * table::find_row_or_FAIL( str0 key ) 
{
	row_inst * r = rows_map.lookup( key );
	if(!r) {
		complain_row_not_found( key );
			
	}
	return r;
}

/*!
	If the row called 'key' is not found return NULL
*/
row_inst * table::find_row( str2 * key ) 
{
	return rows_map.lookup( key );
}

/*!
	find a row, creating a blank one if new.
	Used to create targets of linked fields.

	Fake rows have default values (except the key),
	but if the input file subsequently finds that row,
	the proper values will overwrite the placebo ones.

	Loading proper rows from CSV also calls this function,
	which is mildly in-efficient, but shares the same code.
*/

row_inst * table::find_row_add_fake( str2 * key ) 
{
	return find_row_add_fake( key->sget() ) ;
}

row_inst * table::find_row_add_fake( str0 key ) 
{
	row_inst * r = find_row( key );
	if(r) return r;
	if(0) e_print("find_row_add_fake(%s[%s])\n",
		(STR0) *name,
		(STR0) key );

	// Number all rows by their input suquence, could sort by it ...
	// TODO - write a col_spec for the counter field in each row
	row_sort_counter += 5;

	// Create a new row skeleton (no fields yet)
	r = new row_inst(this, row_sort_counter);

	/*
		Add the correct number of NULL cells,
		but make sure that the key field is correct.

		This PRESUMES that each col is real.

		It would be easier to extend_to( nc )
		then overwrite key_pos
	*/
	int n = col_specs.N();
	for(int i = 0; i < n; i++ )
	{
		obj_ref * cell = NULL;
		col_spec * spec = col_specs[i];
		if( spec == key_spec ) {
			cell = spec->new_cell_from_str( key );
		} else {
			cell = NULL;
		}
		r->cells.append( cell );
	}

	// r->xx_set_key( key );
	add_row( r );
	return r;
}

/*!
	csv has read in a data row to be added to the table.

	The strings belong to the caller (temps in reusable buffers).
*/
bool table::csv_line_in( io_csv_accts * csv ) // strings belong to caller
{
	bool ok = TRUE;
	int argc = csv->nfield;
	char ** argv = &csv->fields[0]; 

	if( argc < 2 )
	{
		fprintf(stderr, "FAIL: csv_line_in - needs at least 2 cols\n" );
		return FALSE;
	}

	if( ! name->is_same_as( argv[0] ))
	{
		ok = FALSE;
		fprintf(stderr, "FAIL: tablename %s does match '%s' \n",
			(STR0) *name,
			(STR0) argv[0]
		);
	}
	argc--;
	argv++;

	int ncol = col_specs.N();

	/*
		TODO: allow MERGE load_file where number of fields differ,
		or even the order of the fields is different. (That requires
		more code, list of pos of field).
	*/

	if( ncol == 0 )
	{
		if(0) fprintf(stderr, "PASS: guessing schema for new table %s\n",
			(STR0) *name );
		// this is line1 of a generic table with no col_specs YET
		for( int i = 0; i<argc; i++ )
		{
#warning "trailing comma on all lines means a column with null name"
			add_col( argv[i] );

		}
		ncol = col_specs.N();
	}

	if( argc != ncol )
	{
		ok = FALSE;
		const char * tablename = (STR0) *name;
		fprintf(stderr, "# FAIL: csv_line_in() wrong number of data fields in %s expected %d got %d\n",
			tablename, ncol, argc );
		for( int i = 0; i<argc; i++ )
		{
			const char * s = "-XS-";
			if( i < ncol ) s = col_specs[i]->get_name()->sget();
			fprintf(stderr, "# FAIL: got fielddata[%d] %20s: '%s'\n",
				i, s,  (STR0) argv[i] );
		}
	}	// continue to process first line ?

	if( !csv_line1_seen )
	{
		csv_line1_seen = TRUE;	// one shot load 
		if(!check_field_names( argc, argv ) )
		{
			fprintf(stderr, "FAIL: csv_line_in\n" );
			return FALSE;
		}
		return ok;
	}

	if(!ok) return ok;

	// assemble a row, referring to col_specs

	row_sort_counter += 10;
	row_inst * r = NULL;


	int key_pos = 0;
	key_pos = key_spec->col_pos;
	r = find_row_add_fake( argv[key_pos] );
	if(!r)
	{
		e_print("csv_line_in() new row failed\n");
		return FALSE;
	}
	// DONE // r->key = key;
	// DONE // add_row( r );

	for(int i = 0; i < argc; i++ )
	{
		col_spec * spec = col_specs[i];
		spec->set_from_csv( r, argv[i] );
	}


	return ok;
}

/*!
	Add a prepared row to the rows_list and rows_map hash table.

	This is only called from one place, so it should be moved there
*/
bool table::add_row( row_inst * r )
{
	// r->prints();

	rows_list.append(r);

	str2 * k = get_key_of_row( r );
	rows_map.add( k, r );

	return TRUE;
}

/*!
	debugging utility
*/
void table::prints()
{
	e_print("# DBG # TABLE: %s ", (STR0) *name);
	fprintf(stderr,"# OK # table::prints() --KEYS-->");
	int N = rows_list.N();
	for(int i = 0; i<N; i++ )
	{
		rows_list[i]->prints();
		e_print(", ");
	}
	fprintf(stderr,"<--KEYS--\n");
}

/*!
	get a col_spec by field-name
*/
col_spec * table::get_col_spec( str0 key )
{
	if(!key) return NULL;
	return col_map.lookup( key ); // NULL
}

/*!
	get a col_spec by field-name
*/
col_spec * table::get_col_spec( str2 * key )
{
	if(!key) return NULL;
	return get_col_spec( *key);
}

col_spec * table::must_get_col_spec( str0 key )
{
	col_spec * resp = get_col_spec( key );
	if( resp ) return resp;
	return (col_spec*) NULL_dgb_fail( key );
}

col_spec_amnt * table::must_get_col_spec_amnt( str0 key )
{
	col_spec * resp = get_col_spec( key );
	if( !resp ) {
		const char * tablename = (STR0) *name;
		FAIL( "table %s col %s - no such spec", tablename, (STR0) key );
		return (col_spec_amnt*) NULL_dgb_fail( key );
	}

	col_spec_amnt * spec = (col_spec_amnt *) resp;
	if( !spec ) {
		// errm actually I dont think the cast fails here
		// but TODO
		const char * tablename = (STR0) *name;
		INFO( "table %s col %s - no such spec", tablename, (STR0) key );
		FAIL("col_spec  isnt amnt - cast failed");
		return (col_spec_amnt*) NULL_dgb_fail( key );
	}

	return spec;
}

/*!
	From a row, get its key field.
	Thats the 'normalised' shared field,
	and not necessarily the first field.
*/
str2 * table::get_key_of_row( row_inst * row )
{
	if(!key_spec) {
		static obj_hold<str2> no_key_str = new_str2_static( "KEY-NO-SPEC" );
		return no_key_str;
	}
	// LURK ASSERT field type is str2
	return (str2 *) key_spec->get_obj( row );
}

/*!
	Most tables have a description field (see guess...) --> str2

	SHould it return NULL ?

	Linked fields have ro
*/
str2 * table::get_desc_of_row( row_inst * row )
{
	// see also tbl_col_spec_get.cxx col_spec_link::get_linked_desc_for_display

	static obj_hold<str2> nullstr1 = new_str2_static( "TABLE-WITHOUT-DESC-COL" );
	if(! desc_spec )
	{
		e_print("TABLE NAME %s ROW- \n", (STR0) name->str );
	}

	if(! desc_spec ) return nullstr1;
	return desc_spec->get_str2_for_display( row );
}

/*!
*/
bool table::set_desc_of_row( row_inst * row, str0 s )
{
	// see also tbl_col_spec_get.cxx col_spec_link::get_linked_desc_for_display

	if(! desc_spec )
	{
		e_print("# %s TABLE NAME %s ROW- \n",
			__PRETTY_FUNCTION__,
			(STR0) name->str
		);
		return false;
	}
	return desc_spec->set_from_edit( row, s );
}

/*!
	This is how the "key - desc" strings are combined

	Maybe it shouldnt be HERE, but it has to be somewhere
*/
str2 * table::get_key_desc_of_row( row_inst * row )
{
	if( !row->cache_key_desc )
	{
		str2 * k = get_key_of_row( row );
		str2 * d = get_desc_of_row( row );
		static str_builder buff;
		buff.clear();
		buff.append( k );
		buff.append( " - " );
		buff.append( d );
		row->cache_key_desc = buff.get_str2();
	}
	return row->cache_key_desc;
}

/*!
	Get 'dotdot' or NULL if absent or in OTHER table

	If the row doesnt have a main-key parent within the same table,
	return NULL, also top level nodes return NULL.

	return group_spec->get_group_row( row );

	group_spec indicates the field that names the parent.
	((which might even be substituted with a row-link)).
	That col_spec provides get_group_row(r1) which returns r2.

*/
row_inst * table::get_local_group_of_row( row_inst * row )
{
	if( !row ) return NULL;
	if( !group_spec ) return NULL;

	return group_spec->get_linked_row( row );
}

/*!
	debugging thing
*/
void print_rows_list( obj_list<row_inst> & rows_list )
{
	int n = rows_list.N();
	for( int i = 0; i<n; i++ )
	{
		row_inst * r = rows_list[i];
		e_print("rows_list[%2d] ref#%d key=%s\n",
			i,
			r->get_ref_count(),
		(STR0) 	r->debug_row_name()
		);
	}
}

/*!
	Delete the row from the table. It isnt easy to figure
	if the row is referred to from another table, or even the same.
	The code is incomplete, but you can always null out the fields,
	save and reload, and then delete unused rows? Or what?
*/
void table::delete_row( row_inst * row )
{
	if( !row ) return;
	if( group_spec )
	{
		e_print("cannot delete row that is a link target !!!\n");
		return;
	}
#if 0
	e_print("# ---l # table::delete_row( %s )\n", (STR0) row->debug_row_name() );
#endif
	if(! rows_list.del_val( row ) )
	{
		e_print("# fail # table::delete_row( %s )\n", (STR0) row->debug_row_name() );
	};
	rows_map.remove( get_key_of_row( row ) );
	if(0) 	e_print("rows ref#%d key=%s\n",
			row->get_ref_count(),
		(STR0) 	row->debug_row_name()
		);
}

/*!
	Delete the row from the table by name
*/
void table::del_row( str0 row_key )
{
	row_inst * row = find_row( row_key );
	if( !row ) {
		e_print("# WARN # del_row('%s') no such row\n", (STR0) row_key );
		return;
	}
	delete_row( row );
}

/*!
	Delete the row from the table by name
*/
void table::del_row( str2 * row_key )
{
	if( row_key ) del_row( row_key->str);
}


table * table::new_clone( str2 * name2 )
{
	return new_clone( *name2 );
}

/*!
	REPORT USES: Create a new table, as the basis of a report or calc area.
*/
table * table::new_clone( str0 name2 )
{
	e_print("UNUSED? table::new_clone()");

	table * t2 = dset->find_add( name2 );
	int n = col_specs.N();
	for( int i=0; i<n; i++ )
	{
		col_spec * spec1 = col_specs[i];
		col_spec * spec2 = spec1->col_spec_clone( t2 );
		if( spec1 == key_spec ) t2->key_spec = spec2;
		if( spec1 == group_spec ) t2->group_spec = spec2;
		t2->add_col( spec2 );
	}
	int nr = rows_list.N();
	for( int i = 0; i< nr; i++ )
	{
		row_inst * r1 = rows_list[i];
		row_inst * r2 = r1->row_clone(); // LURK COPY WANTED
		t2->add_row( r2 );
	}
	return t2;
}

/*!
	upto 4 names, or NULL
*/
void table::sort_by_col_names(
	str0 name0,
	str0 name1,
	str0 name2,
	str0 name3
)
{
	str0 names[4] = {
		name0,
		name1,
		name2,
		name3
	};
	col_spec * specs[4] = { NULL };
	int j = 0;
	for( int i = 0; i<4; i++ )
	{
		if(!names[i]) break;
		obj_hold<str2> s2; s2 = new str2( names[i] );
		col_spec * spec = get_col_spec( s2 );
		if(spec)
			specs[j++] = spec;
	}
	if( j )
		sort_by_col_spec(
			specs[0],
			specs[1],
			specs[2],
			specs[3]
		);
}
}; // NAMESPACE
