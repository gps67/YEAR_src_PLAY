#ifndef help_RPT_base_H
#define help_RPT_base_H

// #include "obj_hold.h"
// #include "dset.h"
// #include "tbl_col_spec_plus.h"
// #include "dgb_fail.h"
// #include "tbl_col_spec_date_rel.h"

// #include "rpt_help_tbl.h"
// // #include "reporter_base.h"
// #include "rpt_tag.h"

#include "rpt_help_TX.h"
#include "tbl_col_spec_acct_sum.h"
#include "amnt_sum_step.h"

#include "rpt_names.h"

namespace ACCTS {
	class reporter_base;

/*!
	a help_RPT is a CREATED report table (not an input pre-existing)

	Its constructor is slightly different as it has a reporter_base * available
	which provides access to any other other (standard) tables
*/
class help_RPT_base : public help_TX_base
{
 public:

	/*
		during the base initialisation, this gets called.
		Thats good for plain tables, but not for this ?
	*/
	bool get_specs()
	{
		return true;
	}

	/*
		you COULD use a RPT as a subsequent INPUT table,
		but I dont, and flag any such use
	*/
	bool fetch_data( row_inst * row )
	{
		return FAIL("BASE_CLASS_is RPT not original data");
		e_print("help_RPT_base::fetch_data()\n");
		return FALSE;
	}

	bool create_table_and_specs_Categorised( data_set * dset, reporter_base * rpt1 );
	bool create_table_and_specs_CashBook( data_set * dset, reporter_base * R );
	bool create_table_and_specs_VatSum( data_set * dset, reporter_base * R );

	help_RPT_base(
		data_set * dset,
		str0 tablename
	);

	bool created_table_base;
	bool create_table_and_specs_BASE( data_set * dset, reporter_base * R );

 // VAT TRACK report

	obj_hold<col_spec_amnt> spec_turn_non_vat; // amnt - vat

	obj_hold<col_spec_amnt> spec_amnt_pre_uk_i;
	obj_hold<col_spec_amnt> spec_amnt_vat_uk_i;

	obj_hold<col_spec_amnt> spec_amnt_pre_uk_o;
	obj_hold<col_spec_amnt> spec_amnt_vat_uk_o;

	obj_hold<col_spec_amnt> spec_amnt_pre_euro_i;
	obj_hold<col_spec_amnt> spec_amnt_vat_euro_i;
	obj_hold<col_spec_amnt> spec_amnt_pre_euro_o;
	obj_hold<col_spec_amnt> spec_amnt_vat_euro_o;

	obj_hold<col_spec_amnt> spec_amnt_pre_excl;
///	obj_hold<col_spec_amnt> spec_amnt_vat_excl;

	amnt_t       		    _turn_non_vat;
	amnt_t       		    _amnt_pre_uk_i;
	amnt_t       		    _amnt_vat_uk_i;
	amnt_t      	 	    _amnt_pre_uk_o;
	amnt_t		       	    _amnt_vat_uk_o;
	amnt_t		       	    _amnt_pre_euro_i;
	amnt_t     	  	    _amnt_vat_euro_i;
	amnt_t      	 	    _amnt_pre_euro_o;
	amnt_t     	  	    _amnt_vat_euro_o;
	amnt_t      	 	    _amnt_pre_excl;
//	amnt_t       		    _amnt_vat_excl;

	/*
		as each row arrives, (report2) acct_amnt( acct, amnt, '+' )
		adds the amount into correct column. spec_sums, _sums
		xfer calls + and -
		acct_map is the map of the spec_sums
	*/
	obj_list<col_spec_acct_sum> spec_sums;
	dyn_array<amnt_sum_step> _sums;
	ht2<str2,col_spec_acct_sum>	acct_map;

	/*
		show sums in each account column, (this is an N2 row)

		this needs to be reset between rows, - slightly scattered
		this uses spec_sums _sums ...
	*/
	bool row_show_sums; // this row only

	bool check_specs_vats();
	bool create_specs_vats( data_set * dset, reporter_base * R );
	bool write_back_vats( row_inst * row );

	bool add_specs_to_table_1();

	bool create_new_row_123();
	bool write_row_back_123( row_inst * row );
	bool write_row_back_sums( row_inst * row );

	void clear();
	void clear_vat();
};

}; // NAMESPACE
#endif
