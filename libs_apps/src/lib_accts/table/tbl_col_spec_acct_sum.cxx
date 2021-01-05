#include "tbl_col_spec_plus.h"
#include "tbl_col_spec_acct_sum.h"
#include "tbl_cell_base.h"
#include "dgb_fail.h"

#include "enum_flags.h"

namespace ACCTS {

/*
	col_spec_acct_sum::
*/

/*!
	constructor
*/
col_spec_acct_sum::col_spec_acct_sum
(
	str2 * _name,			//!< name of this col
	amnt_t _init_bal,		//!< initial balance
	row_inst * _acct		//!< retain link to account
)
: col_spec_amnt( _name )
{
	// set_align_right();
	init_bal = _init_bal;
	curr_bal = _init_bal;
	acct = _acct;
	show_zero_as_empty = true;
	show_null_as_error = false;	// as was
	/*
		show_null_as_error = true;	// debug first row N2
	I THINK:

		the account columns are added as they are encountered
		when all other rows in that table have a NULL cell value
		The first value gets the correct init_bal, so there is no
		residue, but the above rows dont

	*/
}

/*!
	this is rubbish - add_amnt( row, amnt ); is_used();
*/
void col_spec_acct_sum::recalc( row_inst * row )
{
	// return_VOID_dgb_fail("UNUSED reacalc");
	cell_amnt * cell = (cell_amnt *) get_obj( row );
	if(cell)
	{
		cell->cache_clear();
	}
	else
	{
		cell = new cell_amnt( (STR0)NULL );
		set_obj( row, cell );
	}
	/*
		str2_display_cached has not been recalculated,
		but it will be when required by the base cell class
	*/
}

/*!
	I think this is unused
*/
obj_ref *
col_spec_acct_sum::
new_cell_from_str( str0 s )
{
	// return (obj_ref *) NULL_dgb_fail("UNUSED new_cell_from_str");
	amnt_t v1( s );
	return new_cell_from_amnt( v1 );
}

/*!
*/
obj_ref *
col_spec_acct_sum::
new_cell_from_amnt( amnt_t _amnt )
{
		curr_bal.add( _amnt );
		return new cell_amnt( _amnt );
}

/*!
*/
str2 * col_spec_acct_sum::get_str2_BASE( row_inst * row )
{
	cell_amnt * cell = (cell_amnt *) get_obj( row );
	if(!cell)
		return new str2("0");
	return cell->get_str2();
}

/*!
*/
w_widget0 * col_spec_acct_sum::mk_edit(  row_inst * row )
{
	return NULL;
}

/*!
*/
col_spec * col_spec_acct_sum::col_spec_clone( table * t2 )
{
	return NULL;
}


}; // NAMESPACE

