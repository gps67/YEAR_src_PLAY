
#include "tbl_col_spec_plus.h"

namespace ACCTS {

///////////////////

str2 * str_NULL_ROW()
{
	static obj_hold<str2> nullstr = new_str2_static("");
//	static obj_hold<str2> nullstr = new_str2_static("NULL-ROW");
	return nullstr;
}

str2 * str_empty()
{
	static obj_hold<str2> nullstr = new_str2_static("");
	return nullstr;
}

str2 * str_dash()
{
	static obj_hold<str2> nullstr = new_str2_static("-");
	return nullstr;
}

//////////////////

// the key func that each class must provide

str2 * col_spec::get_str2_BASE( row_inst * row )
{
	static obj_hold<str2> naff_static = new_str2_static("BASE CLASS::get_str2_BASE");
	naff_static->ref_fixed();
	return naff_static;
}

// most types go through here

str2 * col_spec::get_str2_for_display( row_inst * row )
{
	str2 * s2 = get_str2_BASE( row );
	if(!s2) s2 = new str2( "" );
	return s2;
}

str2 * col_spec::get_str2_for_edit( row_inst * row )
{
	str2 * s2 = get_str2_BASE( row );
	if(!s2) s2 = new str2( "" );
	return s2;
}

str2 * col_spec::get_str2_for_csv( row_inst * row )
{
	str2 * s2 = get_str2_BASE( row );
	if(!s2) s2 = new str2( "" );
	return s2;
}


// amnt

str2 * col_spec_amnt::get_str2_BASE( row_inst * row )
{
	if(!row) return str_empty();
	if(!row) return str_NULL_ROW();

	obj_ref * ptr = get_obj( row );
	cell_amnt * amnt = (cell_amnt *) ptr; // TESTIT
	if(!amnt) return str_dash();
	return amnt->get_str2();
}


str2 * col_spec_percent::get_str2_BASE( row_inst * row )
{
	if(!row) return str_NULL_ROW();

	obj_ref * ptr = get_obj( row );
	cell_percent * percent = (cell_percent *) ptr; // TESTIT
	if(!percent) {
		// static obj_hold<str2> nullpercent = new_str2_static("NULL-RATE");
		static obj_hold<str2> nullpercent = new_str2_static("0");
		return nullpercent;
	}
	// assert( type ptr is str2
	return percent->get_str2();
}

str2 * col_spec_date::get_str2_BASE( row_inst * row )
{
	obj_ref * ptr = get_obj( row );
	cell_date * date = (cell_date *) ptr; // TESTIT
	if(!date) return NULL;
	// assert( type ptr is str2
	return date->get_str2();
}

str2 * col_spec_str2::get_str2_BASE( row_inst * row )
{
	obj_ref * ptr = get_obj( row );
	// assert( type ptr is str2
	return (str2*)ptr;
}

str2 * col_spec_str2_key::get_str2_BASE( row_inst * row )
{
	obj_ref * ptr = get_obj( row );
	// assert( type ptr is str2
	return (str2*)ptr;
}

str2 * col_spec_str2_key::get_str2_for_csv( row_inst * row )
{
	obj_ref * ptr = get_obj( row );
	if(!ptr) INFO("null from get(obj)row)");
	if(!ptr) return str_empty();
	if(!ptr) return str_NULL_ROW();
	// assert( type ptr is str2
	str2 * s2 = (str2*)ptr;
	if( s2->has_prefix( (str0) "#-" ) )
	{
		s2 = str_dash();
	}
	return s2;
}

str2 * col_spec_link::get_str2_BASE( row_inst * row )
{
/*
	get_obj returns the row that this cell points to
	so ask tbl2 to resolve that row to its primary key
	which is where we got the row_value from
*/
	// get the row_inst (which is held as the cells data)
	obj_ref * ptr = get_obj( row );
	// assert( type ptr is row
	row_inst * r = (row_inst *) ptr;
	if(!r) return str_dash();

	// r is a row in tbl2, get its keyfields display string
	// presuming its key is a suitable str2_for_display (LURK)
	table * tbl2 = get_linked_table();
	return tbl2->get_key_of_row( r );
}

/*!
#include <typeinfo>
str2 * col_spec::get_linked_desc_for_display( row_inst * row )
{
	return new str2(  typeid(*this).name() );

	return name;
	static obj_hold<str2> nullstr = new_str2_static( "((NOT-LINKED))" );
	return nullstr;
}

str2 * col_spec::get_key_desc_for_display( row_inst * row )
{
	obj_hold<str2> desc; desc = get_linked_desc_for_display( row );

	int l = strlen( desc->sget() );
	if( !l ) return get_str2_for_display( row );

	obj_hold<str2> key; key = get_str2_for_display( row );
	l += strlen( key->sget() );
	l += 20;

	char buff[ l ] ; // gcc specific to avoid malloc free
	sprintf( buff, "%s - %s", key->sget(), desc->sget() );
	return new str2( (char *) buff );
}
*/


/*!
	Only linked columns can do this, non-linked will
	return a nasty message. It uses virtual functions
	that col_spec_link provides
*/
str2 * col_spec::get_linked_key_desc( row_inst * r )
{
	static obj_hold<str2> nullstr = new str2("NULL_LINKED");
	static obj_hold<str2> nullstr2 = new str2("NULL_TABLE");

	table * linked_table = get_linked_table();
	if(! linked_table ) return nullstr2;

	row_inst * r2 = get_linked_row( r );
	if(! r2 ) return nullstr;

	return linked_table->get_key_desc_of_row( r2 );
}

/*!
	Almost all rows have a desc, and tbl wont be NULL here
*/
str2 * col_spec::get_key_desc( row_inst * r )
{
	return tbl->get_key_desc_of_row( r );
}

str2 * col_spec_link::get_linked_desc_for_display( row_inst * row )
{
 /*
	// see also table.cxx  table::get_desc_of_row
	static str2 * nullstr = new_str2_static( "((NOT-LINKED_NULL_TBL))" );
	static str2 * nullstr2 = new_str2_static( "((NOT-LINKED_NULL_DESC))" );
	table * tbl = get_group_table();
	if(! tbl ) return nullstr;

	col_spec * desc_spec = tbl -> desc_spec;
	if(! desc_spec ) return nullstr2;
	return desc_spec->get_str2_for_display( row );
 */
	// see also table.cxx  table::get_desc_of_row
	static obj_hold<str2> nullstr = new_str2_static( "((NOT-LINKED_NULL_TBL))" );
	static obj_hold<str2> nullstr2 = new_str2_static( "((NOT-LINKED_NULL_ROW))" );
	table * linked_tbl = get_linked_table();
	if(! linked_tbl ) return nullstr;

	row_inst * linked_row = get_linked_row(row);
	if( ! linked_row ) return nullstr2;

	return linked_tbl->get_desc_of_row( linked_row );
}

//////////// these specs have direct access to values ///////////


bool col_spec_amnt::get_amnt( row_inst * row, amnt_t & amnt )
{
	obj_ref * ptr = get_obj( row );
	cell_amnt * p_amnt = (cell_amnt *) ptr;
	if(!p_amnt)
	{
		 return false;
	}
	amnt = p_amnt->amnt;
	return true;
}

bool col_spec_amnt::set_amnt( row_inst * row, amnt_t amnt )
{
	obj_ref * ptr = get_obj( row );
	if(!ptr) 
	{
		ptr = new_cell_from_str( "0" );
		if(!ptr) return FALSE_dgb_fail("set_amnt");
		set_obj( row, ptr );
	}
	cell_amnt * p_amnt = (cell_amnt *) ptr; // TESTIT
	p_amnt->set_amnt( amnt );
	// DIDNT CLEAR CACHE // p_amnt->amnt = amnt;
	return true;
}

str2 * col_spec_amnt::get_str2_for_display( row_inst * row )
{
	static obj_hold<str2> empty_cell = new str2("");
	static obj_hold<str2> error_cell = new str2("!ERROR");
	amnt_t amnt;
	if( !get_amnt( row, amnt ) )
	{
		if( show_null_as_error )
		{
			return error_cell;
		}
		return empty_cell;
	}

	if( show_zero_as_empty )
	{
		if( amnt.pence == 0 ) return empty_cell;
	}
	return new str2( amnt.get_nocurr_str() ); // with comma without PND
}



bool col_spec_percent::get_percent( row_inst * row, per_cent_t & amnt )
{
	obj_ref * ptr = get_obj( row );
	if(!ptr) {
		amnt.set( 0.0 );
		return false;
	}
	cell_percent * p_amnt = (cell_percent *) ptr; // TESTIT
	amnt = p_amnt->percent;
	return true;
}
bool col_spec_percent::set_percent( row_inst * row, per_cent_t amnt )
{
	obj_ref * ptr = get_obj( row );
	if(!ptr) 
	{
		ptr = new_cell_from_str( "0.0" );
		if(!ptr) return FALSE_dgb_fail("set_percent");
		set_obj( row, ptr );
	}
	cell_percent * p_amnt = (cell_percent *) ptr; // TESTIT
	// p_amnt->cache_clear();
	p_amnt->set_percent( amnt ) ;
	return true;
}



bool col_spec_date::get_date( row_inst * row, date_glib & amnt )
{
	obj_ref * ptr = get_obj( row );
	if(!ptr) 
	{
		// amnt = ?
		return false;
	}
	cell_date * p_amnt = (cell_date *) ptr; // TESTIT
	amnt = p_amnt->date;
	return true;
}

bool col_spec_date::set_date( row_inst * row, date_glib amnt )
{
	obj_ref * ptr = get_obj( row );
	if(!ptr) 
	{
		ptr = new_cell_from_str( "1999-01-01" );
		if(!ptr) return FALSE_dgb_fail("set_date");
		set_obj( row, ptr );
	}
	cell_date * p_amnt = (cell_date *) ptr; // TESTIT
	p_amnt->set_date( amnt );
	return true;
}

}; // NAMESPACE
