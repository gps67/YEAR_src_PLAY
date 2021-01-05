#ifndef reporter_plus_H
#define reporter_plus_H

#include "reporter_base.h"
#include "rpt_help_RPT_plus.h"
// #include "rpt_tag.h"

namespace ACCTS {

/*!
	reporter_base has all the tables, _plus adds logic 
 */
class reporter_plus : public reporter_base
{
 public:
	help_RPT_plus RPT;

	row_inst * N2;
	row_inst * VAT_ACCT;
	row_inst * HW_ACCT;

	bool skip_zero_amnt;
	bool show_N2_sums;

	reporter_plus( data_set * _dset, str0 tablename );

	bool copy_XFER_to_RPT();
	bool copy_TRANS_to_RPT();

	bool CAT_fetch_TRANS_cat(); // and has a key->str
	bool TRANS_is_tax_layer();
	bool TRANS_CAT_is_Income();
///	bool TRANS_VAT_is_excluded();
	bool update_amnt_closing();

	col_spec_acct_sum * find_spec_to_sum_acct( row_inst * acct );
	col_spec_acct_sum * find_spec_to_sum_acct_OK_NULL( row_inst * acct );
	bool acct_amnt( row_inst * acct, amnt_t amnt, char op );

	void do_vat_calc();

	~reporter_plus();

	bool run_merge();
	bool run_pre_merge_sweep();

	bool sort_date_pay;	// else sort date_tax
	IS_DIFF_t cmp_TRANS_XFER();	// by date then key sort pos
	bool show_acct_cols;		// exist as well as show ?
};

}; // NAMESPACE

#endif
