#include "reporter_plus.h"

/*
	INTERNAL reports that gereate tables

	see ../pdf for external reports (also numbered but not the same )
*/

using namespace ACCTS;

/*!
	initial fixup,
	might be post load_dset
	but specific to all (calling) reports

*/
bool run_report_0( data_set * _dset)
{
	static bool done = false;
#warning "LURK - one shot run on single file - next file will fail"
	// The DSET has been loaded from csv (common case)
	if( done ) return true;
	done = true;
	// copy ACCT.initbal to ACCT.end_bal (for each row, within mem dset)
	help_ACCT ACCT( _dset );
	ACCT.copy_all_balances();

	return true;
}

/*!
	the "Categorised" report is done by class

	rename as run_merge_iterator_TRANS_XFER()
	rename as run_iterator_merge_TRANS_XFER()
	rename as run_merge_TRANS_XFER()
	calling copy_TRANS_to_RPT() // or _XFER_
	which copies each field over
	and processes the amnts  and sets the row colours and vat
	and saves RPT helper row to the dset // create_new_row_123()
*/ 
bool run_report_1( data_set * _dset)
{
	run_report_0( _dset );
	obj_hold<reporter_plus> rpt = new 
		reporter_plus( _dset, "Categorised" );
	rpt->RPT.create_table_and_specs_Categorised( _dset, rpt );
	rpt->skip_zero_amnt = true;
	rpt->run_merge();
	return true; // optimistic
}

/*!
	the "CashBook" report produces the CashBook table
*/
bool run_report_2( data_set * _dset)
{
	run_report_0( _dset );
	obj_hold<reporter_plus> rpt = new 
		reporter_plus( _dset, "CashBook" );
	// this is done ONCE and this report is the most sensible
	// or move to just after load ....
	rpt->RPT.create_table_and_specs_CashBook( _dset, rpt );
	rpt->show_N2_sums = true;
	rpt->sort_date_pay = true;
	rpt->show_acct_cols = true;
	rpt->run_pre_merge_sweep();
	rpt->run_merge();
	// this make MOST sense with CahBook
	rpt->update_amnt_closing();

	return true; // optimistic
}


/*!
	the VatSum report is like the CashBook table
	but it has rows omitted when cat 'x'
	eg PAYE itemsa are not on the VAT report
*/
bool run_report_3( data_set * _dset)
{
	// gdb_invoke(true);
	run_report_0( _dset );
	obj_hold<reporter_plus> rpt = new 
		reporter_plus( _dset, "VatSum" );
	rpt->RPT.create_table_and_specs_VatSum( _dset, rpt );
	rpt->skip_zero_amnt = true; // CashBood shows Notes, Vat doesnt
	rpt->run_merge();
	return true; // optimistic
}


/*!
	BLANK ...
*/
bool run_report_4( ::ACCTS::data_set * _dset)
{
	run_report_0( _dset );
	// ... //
	return true;
}
