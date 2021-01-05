#include "str2.h"
#include "tbl_row_inst.h"
#include "tbl_cell_base.h"
#include "tbl_col_spec.h"
#include "tbl_col_spec_plus.h"
#include "table.h"

#include "w_entry.h"
#include "w_calendar.h"

#include "w_combo.h"
#include "w_combo_plus.h"
#include "str_list.h"

#include <gtk/gtk.h> // only for GTK_JUSTIFY_XXX enum type

#include "buffer1.h"

#include "w_btn_plus.h"
// #include "w_img_id.h"

namespace ACCTS {

extern void mk_w_img_id_panel1( ::ACCTS::col_spec_img_id * _spec, ::ACCTS::row_inst * _row );


// prevent - keep it private
/*!
*/
col_spec::col_spec( const col_spec * spec2 ) {
	fprintf(stderr,"col_spec(col_spec) COPY INIT \n");
}

/*
	col_spec is virtual base class - not instantiateable

	col_spec * col_spec::col_spec_clone( table * t2 )
	{
		col_spec * spec2 = new col_spec( name );
		spec2->tbl = t2;
		spec2->align_just = align_just;
		return spec2;
	}
*/

void check_tbl_null( col_spec * spec, row_inst * row )
{
	if( spec && row && spec->tbl ) return;

	e_print("# WARN # check_tbl_null()" );

	if( !spec ) {
		e_print(" # WARN # NULL col_spec" );
		if( row ) row->prints();
		return;
	}

	if( !row ) {
		e_print(" # NULL row" );
		spec -> prints();
		return;
	}
		
	if( !spec->tbl ) {
		e_print(" # NULL spec->tbl" );
	}
	e_print("# \n" );
	if( spec ) spec->prints();
	if( row ) row->prints();
	gdb_break_point();
}

/*!
*/
obj_ref * col_spec::get_obj( row_inst * row )
{
	// fprintf( stderr,"col_pos == %d\n", col_pos );
	if( !tbl ) check_tbl_null( this, row );
	return row->cells[ col_pos ];
}

/*!
*/
void col_spec::set_obj( row_inst * row, obj_ref * cell )
{
	if( !tbl ) check_tbl_null( this, row );
	// fprintf( stderr,"col_pos == %d\n", col_pos );
	row->cells[ col_pos ] = cell;
}

/*!
*/
align_val col_spec::get_align()
{
	return align_just; // wrapped enum
}

/*!
*/
void col_spec::bind_as_tbl_col( table * t, int col )
{
	tbl = t;
	col_pos = col;
/*
	fprintf(stderr,"bind %s[%d] - %s\n",
		table->get_name()->sget(),
		col,
		get_name->sget()
	);
*/
}

/*!
*/
str2 * col_spec::get_name()
{
	return name;
}

/*!
*/
void col_spec::set_name_hdr( str2 * hdr )
{
	name_hdr = hdr;
}

/*!
*/
str2 * col_spec::get_name_hdr()
{
	if(name_hdr)
		return name_hdr;
	buffer1 buff;
	int l = name->str_len();
	for(int i=0; i<l; i++ )
	{
		uchar c = name->str[i];
		if( c == '_' )
			buff.putc_into_utf8( '\n' );
		else
			buff.putc_into_utf8( c );
	}
	return new str2( (blk1) buff );
}

/*!
*/
str0 col_spec::get_name_str()
{
	return name->sget();;
}

/*!
*/
// VIRTUAL 
obj_ref * col_spec::new_cell_from_str( str0 s )
{
	// TODO OTHERS
	fprintf(stderr,"BAD_COL_SPEC::new_cell_from_str\n" );
	return new str2( s );
}

/*!
*/
// VIRTUAL 
obj_ref * col_spec::new_cell_from_NULL()
{
	// TODO OTHERS
	str0 tblname = "NULLTBL";
	if(tbl) tblname = * tbl->name;
	fprintf(stderr,"LURK::new_cell_from_NULL(%s.%s)\n",
	(STR0)	tblname,
	(STR0) *name
	);
	// return new str2( "" );
	return NULL;
}

/*!
*/
void col_spec::init0()
{
	name = NULL;
	tbl = NULL;
	set_align_left();
	col_pos = 1;
	display_width_expanded = 0;
	display_width = 0;
	is_hidden = false;
	is_summable = false; // by request
	is_blankable = false;
}

/*!
*/
col_spec::col_spec( str2 * name_ )
{
	init0();
	name = name_;
}

/*!
*/
col_spec::col_spec( str0 name_ )
{
	init0();
	name = new str2( name_ );
}

/*!
*/
void col_spec::set_align_left() {
	align_just.set_align_left();
}

/*!
*/
void col_spec::set_align_right() {
	align_just.set_align_right();
}

/*!
*/
void col_spec::set_align_center() {
	align_just.set_align_center();
}

// widget0 * mk_edit_combo_keys_for_table( table * tbl2 )

/*!
*/
w_widget0 * col_spec_link::mk_edit( row_inst * row )
{
	return mk_edit_combo_link( row );
}

/*!
*/
w_widget0 * col_spec::mk_edit_combo_link( row_inst * row )
{
	/*
		This exposes a weakness in combo: KEY - Description

		Later: try combo_item = any thing with a string
	*/
	table * tbl2 = get_linked_table();
	int n = tbl2->rows_list.N();
	w_combo_plus * combo = new w_combo_plus();
	for( int i=0; i<n; i++)
	{
		row_inst * row2 = tbl2->rows_list[i];
		str2 * key = tbl2->get_key_of_row( row2 );
		str2 * desc = tbl2->get_desc_of_row( row2 );
		str2 * pair = tbl2->get_key_desc_of_row( row2 );
		combo -> prep_row( key, desc, pair );
	}
	combo->prep_done();
	combo->exact_only();

	// LURK - presume link is dest row
	row_inst * row2 = (row_inst *) get_obj( row );
	// LURK - presume key cell is str2
	if( row2 ) {
		// str2 * s2 = row2->key;
		str2 * s2 = tbl2->get_key_of_row( row2 );
		combo->set_value( s2 );
	}
	
	// set the initial value ??
	return combo;
}

/*
w_widget0 * col_spec::mk_edit_combo_link( row_inst * row )
{
	table * tbl2 = get_linked_table();
	int n = tbl2->rows_list.N();
	str_list s_list( 0 );	// if init(n) --> lots of NULL values
	for( int i=0; i<n; i++)
	{
		row_inst * row = tbl2->rows_list[i];
		// str2 * s = row->key;
		str2 * s = tbl->get_key_of_row( row );
		// e_print("KEY %s\n", s->sget() );
		s_list.append( s );
	}
	w_combo * combo = new w_combo();
	combo->set_strings( & s_list );
	combo->exact_only();
	// LURK - presume link is dest row
	row_inst * row2 = (row_inst *) get_obj( row );
	// LURK - presume key cell is str2
	if( row2 ) {
		// str2 * s2 = row2->key;
		str2 * s2 = tbl2->get_key_of_row( row2 );
		combo->set_value( s2 );
	}
	
	// set the initial value ??
	return combo;
}
*/


/*!
*/
// virtual
w_widget0 * col_spec::mk_edit( row_inst * row )
{
/*
	table * tbl2 = get_linked_table();
	if( tbl2 )
	// and set current value
		return mk_edit_combo_keys_for_table( tbl2 ); 
*/
	w_entry * edit = new w_entry();
	obj_hold<str2> s2;
	if( row ) {
		s2 = get_str2_for_edit( row );
	} else {
		static obj_hold<str2> s2_null  = new_str2_static( "EDIT" );
		s2 = s2_null;
	}
	edit -> set_text( s2 );
	return (w_widget0 *) edit;
}

// -- base --

/*!
*/
table * col_spec::get_linked_table()
{
	return (table *) NULL; // column has no grouping (eg amnt)
}
/*!
*/
row_inst * col_spec::get_linked_row( row_inst * row )
{
	return (row_inst *) NULL; // column has no grouping (eg amnt)
}


// -- amnt --

/*!
*/
col_spec_amnt::col_spec_amnt( str0 name )
: col_spec( name )
{
	set_align_right();
	show_null_as_error = false;
	show_zero_as_empty = false; // NULL is drawn as empty, so why not zero
}

/*!
*/
col_spec_amnt::col_spec_amnt( str2 * name )
: col_spec( name )
{
	set_align_right();
}

/*!
*/
obj_ref * col_spec_amnt::new_cell_from_str( str0 s ) {
	return new cell_amnt( s );
}

/*!
*/
col_spec * col_spec_amnt::col_spec_clone( table * t2 )
{
	col_spec_amnt * spec2 = new col_spec_amnt( name );
	spec2->tbl = t2;
	spec2->align_just = align_just;
	return spec2;
}

// -- percent --

/*!
*/
col_spec_percent::col_spec_percent( str2 * name )
: col_spec( name )
{
	set_align_right();
}

/*!
*/
obj_ref * col_spec_percent::new_cell_from_str( str0 s ) {
	return new cell_percent( s );
}

/*!
*/
col_spec * col_spec_percent::col_spec_clone( table * t2 )
{
	col_spec_percent * spec2 = new col_spec_percent( name );
	spec2->tbl = t2;
	spec2->align_just = align_just;
	return spec2;
}

// -- date --

/*!
*/
col_spec_date::col_spec_date( str2 * name, table * _era_tbl )
: col_spec( name )
{
	era_tbl = _era_tbl;
}
/*!
*/
obj_ref * col_spec_date::new_cell_from_str( str0 s ) {
	cell_date * d1 = new cell_date( s );
	return d1;
}

/*!
*/
table * col_spec_date::get_linked_table()
{
	return era_tbl; // column is grouped in same table
}
/*!
*/
row_inst * col_spec_date::get_linked_row( row_inst * row )
{
	if(!era_tbl) return NULL;
	cell_date * cell = (cell_date *) get_obj( row );
	if(!cell)
	{
		e_print("ERROR cell SHOULD NOT BE NULL in %s\n", __PRETTY_FUNCTION__ );
		row->prints();
	}
	obj_hold<str2> year_mm; year_mm = cell->get_YEAR_MM();
	row_inst * era_row = era_tbl->find_row_add_fake( year_mm );
	return era_row;
}

/*!
*/
w_widget0 * col_spec_date::mk_edit( row_inst * row )
{
	cell_date * cell = (cell_date *) get_obj( row );
	w_calendar * cal = new w_calendar();
	cal->set_date( cell );
	return cal;
}

/*!
*/
col_spec * col_spec_date::col_spec_clone( table * t2 )
{
	col_spec_date * spec2 = new col_spec_date( name, era_tbl );
	spec2->tbl = t2;
	spec2->align_just = align_just;
	return spec2;
}

// -- str2 --

/*!
*/
col_spec_str2::col_spec_str2( str2 * name )
: col_spec( name )
{
}
/*!
*/
obj_ref * col_spec_str2::new_cell_from_str( str0 s ) {
	return new str2( s );
}

/*!
*/
col_spec * col_spec_str2::col_spec_clone( table * t2 )
{
	col_spec_str2 * spec2 = new col_spec_str2( name );
	spec2->tbl = t2;
	spec2->align_just = align_just;
	return spec2;
}

// -- str2_key --

/*!
*/
col_spec_str2_key::col_spec_str2_key( str2 * name )
: col_spec( name )
{
	align_just = align_center;
}

/*!
	The stored cell value is managed by the col_spec.
*/
obj_ref * col_spec_str2_key::new_cell_from_str( str0 s )
{
        char snum[10]; // build auto key here
        /*
                This is where '+' can be used as a key value
                which is replaced with a unique id.
                On the way out that is replaced with '+'.
                HOPEFULLY there is no mix with plain key strings
        */
        if(( ! s )
        || ( 0==strcmp(s,""))
        || ( 0==strcmp(s,"+"))
        || ( 0==strcmp(s,"-"))
        )
        {
                static int uniqueid = 0;
                sprintf(snum,"#-%4.4d", uniqueid++ * 10 );
                s = &snum[0];
        }

	return new str2( s );
}

/*!
*/
table * col_spec_str2_key::get_linked_table()
{
	return tbl; // column is grouped in same table
}
/*!
*/
row_inst * col_spec_str2_key::get_linked_row( row_inst * row )
{
	// other fields link via their col_specs
	// The key field uses the tables grouping
	return tbl->get_local_group_of_row( row );
	// str2 * key1 = (str2 *) get_obj( row );
	// row_inst * row2 = tbl->find_row( key1 );
	// return row2;
}

/*!
*/
col_spec * col_spec_str2_key::col_spec_clone( table * t2 )
{
	col_spec_str2_key * spec2 = new col_spec_str2_key( name );
	spec2->tbl = t2;
	spec2->align_just = align_just;
	return spec2;
}

/*
	col_spec_link_dot_dot	is just like ...
	col_spec_tbl_row_link_key	except its within the SAME table
					so see its docs and set tbl2==tbl1
*/

// -- link --

/*!
*/
row_inst * col_spec_link::get_linked_row( row_inst * row )
{
	return (row_inst *) get_obj( row );
}

/*!
*/
col_spec_link::col_spec_link(
	str2 * name
)
: col_spec( name )
{
	set_align_center();	// looks better for MOST, really a padding issue
}

/*!
*/
col_spec * col_spec_link::col_spec_clone( table * t2 )
{
	col_spec_link * spec2 = new col_spec_link( name );
	spec2->tbl = t2;
	spec2->align_just = align_just;
	return spec2;
}

// -- link_dot_dot -- IN SAME TBL (only on keyfield)

/*!
*/
col_spec_link_dot_dot::col_spec_link_dot_dot(
	str2 * name
)
: col_spec_link( name )
{
}

/*!
*/
col_spec * col_spec_link_dot_dot::col_spec_clone( table * t2 )
{
	col_spec_link_dot_dot * spec2 = new col_spec_link_dot_dot( name );
	spec2->tbl = t2;
	spec2->align_just = align_just;
	return spec2;
}

// -- row_link_key --

/*!
*/
col_spec_row_link_key::col_spec_row_link_key(
	str2 * name,
	table * _tbl2
)
: col_spec_link( name )
{
	tbl2 = _tbl2;
}

/*!
*/
row_inst * col_spec_row_link_key::get_linked_row( row_inst * row )
{
/*
	The cell has ALREADY been replace with a row_inst
	so simply fetch it with get_obj (col_pos from row)
*/
	obj_ref * ptr = get_obj( row ); 
	return (row_inst *) ptr;
}



/*!
*/
col_spec * col_spec_row_link_key::col_spec_clone( table * t2 )
{
	col_spec_row_link_key * spec2 = new col_spec_row_link_key( name, tbl2 );
	spec2->tbl = t2;
	spec2->align_just = align_just;
	return spec2;
}

/*!
*/
void col_spec::prints()
{
	const char * tbl_name = "(NULL)";
	if( tbl ) tbl_name = (STR0) tbl->name->str;

	buffer1 tn;
	e_print( "# DBG # table %s pos %d '%s' TYPE %s \n",
		tbl_name,
		col_pos,
		(STR0) name->str,
		type_name(tn)
	);
}

// -- img_id --


/*!
*/
col_spec_img_id::col_spec_img_id( str2 * name )
: col_spec_str2( name )
{
}


/*!
	wrapper for btn
*/
class spec_img_edit_btn : public w_btn_plus
{
 public:
	obj_hold<col_spec_img_id>  spec;
	obj_hold<row_inst>  row;
	spec_img_edit_btn(
		col_spec_img_id * _spec,
		row_inst * _row,
		str2 * name
	)
	// : w_btn_base( (str0) name->str )
	: w_btn_plus( "SCAN IMAGE" )
	, spec( _spec )
	, row( _row )
	{
	}

	virtual void event_clicked()
	{	
		mk_w_img_id_panel1( spec, row ); // app hold own ref
	}

	// VIRTUAL FOR mk_edit
        str0 get_edited_value_as_cstr()
	{
		static obj_hold<str2> holdmem;
		holdmem = spec->get_str2_for_edit( row );
		return str0(holdmem->str);
	}

};

/*!
	The img edit is a button that brings up a new window that scans

	This shows an interesting design weakness, as this needs some
	way to write the changed resukt back itself (other edit fields
	simply wait for the caller to call get_str_from_edit when the
	Replace button is pressed).

	The fudged compromise is to write the result back (spec+row),
	but not tell the ctree to re-display.

	Realistically, the only route is to get spec->tbl, and then
	ask a global lookup to inform all clients that tbl.x[row]
	has changed.
*/
// virtual
w_widget0 * col_spec_img_id::mk_edit( row_inst * row )
{
	obj_hold<str2> s2;
	if( row ) {
		s2 = get_str2_for_edit( row );
	} else {
		// never happens ?
		static obj_hold<str2> s2_null  = new_str2_static( "EDIT" );
		s2 = s2_null;
	}
	// e_print("img_id:mk_edit called\n");
	return new spec_img_edit_btn( this, row, s2 );
}
}; // NAMESPACE
