// #include "tbl_col_spec_plus.h"
// #include "dgb_fail.h"
// #include "tbl_col_spec_date_rel.h"

#include "rpt_help_RPT_base.h"
#include "reporter_base.h"

using namespace ACCTS;

help_RPT_base::
help_RPT_base(
	data_set * dset,
	str0 tablename
)
: help_TX_base()
, row_show_sums( false )
{
	created_table_base = false;
	late_init( dset, tablename );
	if( already_existed )
		e_print("WARNING TABLE %s ALREADY EXISTS\n", (STR0) tablename );
}

/*!
	shared component
*/
bool help_RPT_base::create_table_and_specs_BASE( data_set * dset, reporter_base * R )
{
	if( created_table_base ) return true;
	created_table_base = true;

	if(!tbl) return false;
	if(!tbl->key_spec)
	{
		e_print( "WARN: %s: !tbl->key_spec\n", __PRETTY_FUNCTION__ );
		// this needs to be set for the sort by 
	}


	/*
		get some dest tables (weak way)
	*/
	table * tbl_ACCT = R->ACCT.tbl;
	table * tbl_CAT  = R->CAT.tbl;
	table * tbl_VAT  = R->VAT.tbl;
	table * tbl_REF  = R->REF.tbl;
	table * tbl_ERA  = R->ERA.tbl;

	/*
		copy the TRANS table to the RPT table by cloning it

		NOTICE: new col_spec means that we MUST use obj_hold
		(Other tables borrow an unrecorded reference) This manifests itself
		AFTER something has added and removed a ref from a spec
	*/
	spec_key  = new col_spec_str2_key( new str2("key" ));
	spec_date_tax = new col_spec_date( new str2( "date_tax" ), tbl_ERA );
	spec_date_pay = new col_spec_date( new str2( "date_pay" ), tbl_ERA );
	spec_date_tax_rel = new col_spec_date_rel(
		new str2("date_tax_rel"),
		spec_date_pay,
		spec_date_tax
	);
	spec_date_pay_rel = new col_spec_date_rel(
		new str2("date_pay_rel"),
		spec_date_tax,
		spec_date_pay
	);


	spec_cat       = new col_spec_row_link_key( new str2("cat"), tbl_CAT );
	spec_acct1     = new col_spec_row_link_key( new str2("acct1"), tbl_ACCT );
	spec_acct2     = new col_spec_row_link_key( new str2("acct2"), tbl_ACCT );        
	spec_amnt      = new col_spec_amnt( new str2("amnt") );
	spec_amnt_xfer = new col_spec_amnt( new str2("amnt_xfer") );        
	spec_vat       = new col_spec_row_link_key( new str2("vat"), tbl_VAT );
//	spec_vat_rate;
	spec_amnt_vat  = new col_spec_amnt( new str2("amnt_vat") );
	spec_amnt_pre  = new col_spec_amnt( new str2("amnt_pre_vat") );
	spec_ref       = new col_spec_row_link_key( new str2("ref" ), tbl_REF );
	spec_img       = new col_spec_str2( new str2("img" ));;
	spec_desc      = new col_spec_str2( new str2("desc" ));;

	spec_acct     = new col_spec_row_link_key( new str2("acct"), tbl_ACCT );
	spec_date     = new col_spec_date( new str2( "date" ), tbl_ERA );

	spec_date_pay_rel->set_align_center();
	spec_date_tax_rel->set_align_center();
	spec_acct1       ->set_align_center();
	spec_acct2       ->set_align_center();

	spec_acct -> is_hidden = true;
	spec_date -> is_hidden = true;

	if(! create_specs_vats( dset, R ) ) return false;;


	/*!
		check the above again for NULL
	*/
	if(! check_specs_all() ) return false;

	/*
		tell report-pdf to generate column totals
	*/
	spec_amnt    ->is_summable = true;
	spec_amnt_vat->is_summable = true;
	spec_amnt_pre->is_summable = true;

	/*
		cleaner reports
	*/
	spec_amnt      ->show_zero_as_empty = true;
	spec_amnt_vat  ->show_zero_as_empty = true;
	spec_amnt_pre  ->show_zero_as_empty = true;
	spec_amnt_xfer ->show_zero_as_empty = true;

	// call must now do the second half of this function
	// so that these columns belong to a table !

	return true;
}

/*!
	the specs which are added changes the table produced,
	and the sequence of the fields.

	In future it may be necessary to add all cols, but set is_hidden
*/
bool help_RPT_base::add_specs_to_table_1() // Categorised 
{
	/*
		both spec_date_pay and _tax must be present otherwise
		the rel calculation fails, so how can that col be dropped?
		(hidden)

		If you dont add_col() the col_pos comes out as (1??!)
		and it conflicts with the real one
	*/
	tbl->add_col( spec_key );
	tbl->add_col( spec_cat );
	tbl->add_col( spec_date_tax_rel );
	tbl->add_col( spec_date_pay );
	tbl->add_col( spec_date_tax );
	tbl->add_col( spec_date_pay_rel );

  	tbl->add_col( spec_turn_non_vat ); // AMNT - VAT
	tbl->add_col( spec_vat );
	tbl->add_col( spec_amnt );
  	tbl->add_col( spec_amnt_pre_excl ); // which is excluded from vat report
  	tbl->add_col( spec_amnt_pre );
  	tbl->add_col( spec_amnt_vat );
	tbl->add_col( spec_acct1 );
	tbl->add_col( spec_amnt_xfer );
	tbl->add_col( spec_acct2 );
	tbl->add_col( spec_ref );
	tbl->add_col( spec_img );
	tbl->add_col( spec_desc );
	return true;
}

/*!
*/
bool help_RPT_base::create_table_and_specs_Categorised( data_set * dset, reporter_base * R )
{
	if( already_existed ) return true;

	// common top half
	if(! create_table_and_specs_BASE( dset, R )) return false; 

	// the order of the fields in the table
	if(! add_specs_to_table_1() ) return false;

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

	tbl->col_spec_tree = spec_cat; // CATEGORISED VIEW
	tbl->col_spec_sort_1 = spec_date_tax; // TAX oriented dates
	tbl->col_spec_sort_2 = spec_cat; // 
	tbl->desc_spec = spec_desc;

	/*
		ALL columns must be added to tbl,
		but you can hide ones you dont want visible.
	*/
	 if(1)
	{
		// TAX date oriented view
		spec_date_tax_rel->is_hidden = true;
		spec_date_pay->is_hidden = true;
	} else {
		spec_date_pay_rel->is_hidden = true;
		spec_date_tax->is_hidden = true;
	}
	spec_ref->is_hidden = true;
	spec_key->is_hidden = true;

	return true;
}

void help_RPT_base::clear()
{
	clear_TX();
	clear_vat();

	row_show_sums = false;

	// sums ae NOT cleared - should hav ealrady done step()
	// dyn_array<amnt_sum_step> _sums;
	// acct_map endures
	// ht2<str2,col_spec_acct_sum>	acct_map;
}

void help_RPT_base::clear_vat()
{
	_amnt_vat		.set_zero();
	_amnt_pre		.set_zero();
	_turn_non_vat		.set_zero();
	_amnt_vat_uk_i		.set_zero();
	_amnt_pre_uk_i		.set_zero();
	_amnt_vat_uk_o		.set_zero();
	_amnt_pre_uk_o		.set_zero();
	_amnt_vat_euro_i	.set_zero();
	_amnt_pre_euro_i	.set_zero();
	_amnt_vat_euro_o	.set_zero();
	_amnt_pre_euro_o	.set_zero();
	
//	_amnt_vat_excl		.set_zero();
	_amnt_pre_excl		.set_zero();
	
	// spec_turn_non_vat //  _turn_non_vat
	// spec_amnt_pre_uk_i // _amnt_pre_uk_i
	
	_turn_non_vat		.set_zero();
}

