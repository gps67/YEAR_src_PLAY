// #include "tbl_col_spec_plus.h"
// #include "dgb_fail.h"
// #include "tbl_col_spec_date_rel.h"

#include "rpt_help_RPT_base.h"
#include "reporter_plus.h"
// #include "enum_flags.h" // ROW_show_sums

using namespace ACCTS;

/*
	CashBook-2 view
*/



/*!
	CashBook
*/
bool help_RPT_base::create_table_and_specs_CashBook( data_set * dset, reporter_base * R )
{
	if( already_existed ) return true;

	// common top half
	if(! create_table_and_specs_BASE( dset, R )) return false; 

	/*
		This defines the order of the columns in the table

		both spec_date_pay and _tax must be present otherwise
		the rel calculation fails, so how can that col be dropped?

		If you dont add_col() the col_pos comes out as (1??!)
		and it conflicts with the real one
	*/
  	spec_amnt_pre_excl	-> is_hidden = true;
  	spec_amnt_pre 		-> is_hidden = true;
  	spec_turn_non_vat	-> is_hidden = true;

	tbl->add_col( spec_key );
	tbl->add_col( spec_date_tax_rel );
	tbl->add_col( spec_date_pay );		// CashBook
	tbl->add_col( spec_date_tax );
	tbl->add_col( spec_date_pay_rel );
	tbl->add_col( spec_cat );
	tbl->add_col( spec_amnt );

	tbl->add_col( spec_acct1 );
	tbl->add_col( spec_amnt_xfer );
	tbl->add_col( spec_acct2 );
	tbl->add_col( spec_vat );
  	tbl->add_col( spec_amnt_vat );

  	tbl->add_col( spec_amnt_pre_excl ); // which is excluded from vat report
  	tbl->add_col( spec_turn_non_vat );
  	tbl->add_col( spec_amnt_pre );

	tbl->add_col( spec_ref );
	tbl->add_col( spec_img );
	tbl->add_col( spec_desc );
	// putting on RHS

	/*
		RPT.tbl->key_spec -- must be set
	*/

	if(!tbl->key_spec)
	{
		// this needs to be set for the sort by 
		tbl->key_spec = spec_key;
		// also key_desc
		/*
			when the table is sorted by category (or other)
			the date_tax (or pay) is not used! only the
			key_spec, however those values tend to be
			in ascending order.

			The XFER table tends to be loaded AFTER the TRANS
			so all its values ae higher, when grouped by acct
			(or other) that puts transfers last

			This fixes itself for newly added rows when the
			data is saved and reloaded, as all the key values
			get saved as '-' (but the save order is by key?)
			so maybe save needs to sort.
		*/
	}

	tbl->col_spec_tree = spec_date_pay; // CashBook VIEW
	tbl->col_spec_sort_1 = spec_date_pay; // PAY oriented dates
	tbl->col_spec_sort_2 = spec_cat; // 
	tbl->desc_spec = spec_desc;

	/*
		ALL columns must be added to tbl,
		but you can hide ones you dont want visible.
	*/
	if(0)
	{
		spec_date_tax_rel->is_hidden = true;
		spec_date_pay->is_hidden = true;
	} else {
		// PAY oriented dates
		spec_date_pay_rel->is_hidden = true;
		spec_date_tax->is_hidden = true;
	}
	spec_ref->is_hidden = true;
	spec_key->is_hidden = true;

	return true; // optimistic
}

