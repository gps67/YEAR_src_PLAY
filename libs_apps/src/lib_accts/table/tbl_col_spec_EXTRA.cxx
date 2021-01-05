#include "tbl_col_spec_plus.h"
#include "tbl_col_spec_date_rel.h"
#include "dgb_fail.h"

namespace ACCTS {

/*
	col_spec_date_rel::
*/

/*!
	constructor
*/
col_spec_date_rel::col_spec_date_rel
(
	str2 * name,			//<! name of this col
	col_spec_date * _spec_rel,	//<! the cell that gives the origin
	col_spec_date * _spec_val	//<! the cell that gives the value
)
: col_spec( name )
{
	if( !_spec_val ) return_VOID_dgb_fail( "NULL spec_val");
	if( !_spec_rel ) return_VOID_dgb_fail( "NULL spec_rel");
	spec_val = _spec_val;
	spec_rel = _spec_rel;
	set_align_right();
}

/*!
	recalculate the relative days (( but cell hold old ))
*/
void col_spec_date_rel::recalc( row_inst * row )
{
	cell_date_rel * cell = (cell_date_rel *) get_obj( row );
	if(cell)
	{
		// cell->str2_display_cached = NULL;
		cell->cache_clear();
	}
	else
	{
		if(!spec_val) return_VOID_dgb_fail( "NULL spec_val" );
		if(!spec_rel) return_VOID_dgb_fail( "NULL spec_rel" );

		cell_date * cell_d0 = (cell_date *) spec_rel->get_obj( row );
		cell_date * cell_d1 = (cell_date *) spec_val->get_obj( row );

		if(!cell_d0) return_VOID_dgb_fail( "NULL cell_d0" );
		if(!cell_d1) return_VOID_dgb_fail( "NULL cell_d1" );

		cell = new cell_date_rel( cell_d0->date, cell_d1->date);
		if(!cell) return_VOID_dgb_fail( "cell is NULL");
		set_obj( row, cell );
	}
	/*
		str2_display_cached has not been recalculated,
		but it will be when required by the base cell class
	*/
}

/*!
*/
obj_ref * col_spec_date_rel::new_cell_from_str( str0 s )
{
	/*
		ignore incoming string ? do calc

		This might upset any caller that EXPECTS a non-null val
	*/
	return NULL;
}

/*!
*/
str2 * col_spec_date_rel::get_str2_BASE( row_inst * row )
{
	cell_date_rel * cell = (cell_date_rel *) get_obj( row );
	if(!cell)
		return new str2("NULL-NULL");
	return cell->get_str2();
}

/*!
*/
table * col_spec_date_rel::get_linked_table()
{
	return NULL;
}

/*!
*/
row_inst * col_spec_date_rel::get_linked_row( row_inst * r )
{
	return NULL;
}

/*!
*/
w_widget0 * col_spec_date_rel::mk_edit( row_inst * r )
{
	return NULL;
}

/*!
	what are we comparing the date or the difference ?

	Originally I wanted the date, but its a good check
	to search for items which are "well out of date"
*/
IS_DIFF_t col_spec_date_rel::cmp_rr( row_inst * r1, row_inst * r2 )
{
	int days_diff1 = get_days_diff( r1 );
	int days_diff2 = get_days_diff( r2 );

	if( days_diff1 < days_diff2 ) return IS_LESS;
	if( days_diff1 > days_diff2 ) return IS_MORE;
	return IS_SAME;
}

/*!
	retuirn the nukber of days delay (or advannce payment) within row
*/
int col_spec_date_rel::get_days_diff( row_inst *r )
{
	//! should not happen?
	if(!r) return 0;

	cell_date_rel * cell = (cell_date_rel *) get_obj( r );
	if(!cell) {
		recalc( r );
		cell = (cell_date_rel *) get_obj( r );
		if(!cell) return IS_SAME;
	}

	int days_diff = cell->date_val - cell->date_rel;
	return days_diff;
}


/*!
*/
col_spec * col_spec_date_rel::col_spec_clone( table * t2 )
{
	return NULL;
}


}; // NAMESPACE

