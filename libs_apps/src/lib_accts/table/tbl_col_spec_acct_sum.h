#ifndef table_col_spec_acct_sum_H
#define table_col_spec_acct_sum_H

#include "tbl_col_spec_plus.h"

typedef unsigned int u32;

namespace ACCTS {

/*!
	col_spec_amnt_sum is an amnt column, but there is an initial value
	and the step values come from  calls, not from an auto formula
*/
class col_spec_acct_sum : public col_spec_amnt
{
 public:
	amnt_t	init_bal;
	amnt_t	curr_bal;
	obj_hold<row_inst>  acct;

	col_spec_acct_sum( str2 * name, amnt_t _init_bal, row_inst * _acct );
 	virtual col_spec * col_spec_clone( table * tb );

 public:
	virtual obj_ref * new_cell_from_str( str0 s );
	virtual obj_ref * new_cell_from_amnt( amnt_t amnt );
	str2 * get_str2_BASE( row_inst * row );
	w_widget0 * mk_edit( row_inst * r );

	void recalc( row_inst * r );
};
}; // NAMESPACE

#endif



