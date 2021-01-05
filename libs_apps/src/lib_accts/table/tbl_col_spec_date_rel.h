#ifndef table_col_spec_date_rel_H
#define table_col_spec_date_rel_H

#include "tbl_col_spec_plus.h"
#include "tbl_col_spec_date_rel.h"

namespace ACCTS {


/*!
	A column that is like a date but prints as +5 relative to another col

	Both values are taken from other cells - so this isnt really editable?
	mk_edit returns [edit the other cell] ??

	I think this is ill conceived - maybe it should take VALS not EXPRS
	eg: mk_edit() ?? get_store_val ?? setup_cell ??

	This also exposes - do what when cell is NULL? autofixup?
*/
class col_spec_date_rel : public col_spec
{
 public:
	col_spec_date *spec_val; //!< the spec of the value of this cell
	col_spec_date *spec_rel; //!< the date that this is relatice to

	col_spec_date_rel( str2 * name, col_spec_date *_spec_rel, col_spec_date * _spec_val );
 // virtual
 	virtual col_spec * col_spec_clone( table * tb );
	virtual T_field get_field_type() { return T_date_rel; }

 public:
	virtual obj_ref * new_cell_from_str( str0 s );
	str2 * get_str2_BASE( row_inst * row );
	table * get_linked_table();
	row_inst * get_linked_row( row_inst * r );
	w_widget0 * mk_edit( row_inst * r );
	virtual IS_DIFF_t cmp_rr( row_inst * r1, row_inst * r2 );

	//! extra function for date_rel, used to sort by payment gap
	int get_days_diff( row_inst *r );

	void recalc( row_inst * r );
};
}; // NAMESPACE

#endif



