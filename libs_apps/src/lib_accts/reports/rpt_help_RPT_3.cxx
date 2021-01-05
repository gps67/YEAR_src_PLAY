
#include "rpt_help_RPT_base.h"
#include "enum_flags.h"

using namespace ACCTS;

/*!
*/
bool help_RPT_base::create_specs_vats( data_set * dset, reporter_base * R )
{
 spec_turn_non_vat   = new col_spec_amnt( new str2( STR0_turnover ) );
 spec_amnt_pre_uk_i  = new col_spec_amnt( new str2("pre-vat_UK_BUY-INP") );
 spec_amnt_vat_uk_i  = new col_spec_amnt( new str2("amnt-vat_UK_BUY-INP") );
 spec_amnt_pre_uk_o  = new col_spec_amnt( new str2("pre-vat_UK_SELL-OUT") );
 spec_amnt_vat_uk_o  = new col_spec_amnt( new str2("amnt-vat_UK_SELL-OUT") );
 spec_amnt_pre_euro_i  = new col_spec_amnt( new str2("pre-vat_EURO_BUY-INP") );
 spec_amnt_vat_euro_i  = new col_spec_amnt( new str2("amnt-vat_EURO_BUY-INP") );
 spec_amnt_pre_euro_o  = new col_spec_amnt( new str2("pre-vat_EURO_SELL-OUT") );
 spec_amnt_vat_euro_o  = new col_spec_amnt( new str2("amnt-vat_EURO_SELL-OUT") );
 spec_amnt_pre_excl  = new col_spec_amnt( new str2("excluded_vat_report") );
 // spec_amnt_vat_excl  = new col_spec_amnt( new str2("amnt-vat_excl") );

	/*
		this tells the report generator to add subtots per group
		and to blank out zero value fields
		(occasionally thats confusing, but clearer when noticed)
	*/
	spec_amnt            ->is_summable = true;
	spec_amnt            ->show_zero_as_empty = true;

	spec_turn_non_vat    ->is_summable = true;
	spec_amnt_pre_uk_i   ->is_summable = true;
	spec_amnt_vat_uk_i   ->is_summable = true;
	spec_amnt_pre_uk_o   ->is_summable = true;
	spec_amnt_vat_uk_o   ->is_summable = true;
	spec_amnt_pre_euro_i ->is_summable = true;
	spec_amnt_vat_euro_i ->is_summable = true;
	spec_amnt_pre_euro_o ->is_summable = true;
	spec_amnt_vat_euro_o ->is_summable = true;
	spec_amnt_pre_excl   ->is_summable = true;
///	spec_amnt_vat_excl   ->is_summable = true;
	/*
		cleaner reports
	*/
	spec_turn_non_vat    ->show_zero_as_empty = true;
	spec_amnt_pre_uk_i   ->show_zero_as_empty = true;
	spec_amnt_vat_uk_i   ->show_zero_as_empty = true;
	spec_amnt_pre_uk_o   ->show_zero_as_empty = true;
	spec_amnt_vat_uk_o   ->show_zero_as_empty = true;
	spec_amnt_pre_euro_i ->show_zero_as_empty = true;
	spec_amnt_vat_euro_i ->show_zero_as_empty = true;
	spec_amnt_pre_euro_o ->show_zero_as_empty = true;
	spec_amnt_vat_euro_o ->show_zero_as_empty = true;
	spec_amnt_pre_excl   ->show_zero_as_empty = true;
///	spec_amnt_vat_excl   ->show_zero_as_empty = true;

	if(! check_specs_vats() ) return false;

	return true;
}

/*!
*/
bool help_RPT_base::check_specs_vats()
{
	if(! spec_turn_non_vat )	 return FALSE_dgb_fail("NULL SPEC");
	if(! spec_amnt_pre_uk_i )	 return FALSE_dgb_fail("NULL SPEC");
	if(! spec_amnt_vat_uk_i )	 return FALSE_dgb_fail("NULL SPEC");
	if(! spec_amnt_pre_uk_o )	 return FALSE_dgb_fail("NULL SPEC");
	if(! spec_amnt_vat_uk_o )	 return FALSE_dgb_fail("NULL SPEC");
	if(! spec_amnt_pre_euro_i )	 return FALSE_dgb_fail("NULL SPEC");
	if(! spec_amnt_vat_euro_i )	 return FALSE_dgb_fail("NULL SPEC");
	if(! spec_amnt_pre_euro_o )	 return FALSE_dgb_fail("NULL SPEC");
	if(! spec_amnt_vat_euro_o )	 return FALSE_dgb_fail("NULL SPEC");
	if(! spec_amnt_pre_excl )	 return FALSE_dgb_fail("NULL SPEC");
///	if(! spec_amnt_vat_excl )	 return FALSE_dgb_fail("NULL SPEC");
	return true;
}

bool help_RPT_base::write_back_vats( row_inst * row )
{
	if( spec_amnt_pre_excl &&
	    spec_amnt_pre_excl -> tbl )
	    spec_amnt_pre_excl    ->set_amnt( row, _amnt_pre_excl );

	if( spec_turn_non_vat &&
	    spec_turn_non_vat -> tbl )
	    spec_turn_non_vat    ->set_amnt( row, _turn_non_vat );

	if( !spec_amnt_pre_uk_i ) return true;
	if( !spec_amnt_pre_uk_i->tbl ) return true;

	spec_amnt_pre_uk_i    ->set_amnt( row, _amnt_pre_uk_i );
	spec_amnt_vat_uk_i    ->set_amnt( row, _amnt_vat_uk_i );
	spec_amnt_pre_uk_o    ->set_amnt( row, _amnt_pre_uk_o );
	spec_amnt_vat_uk_o    ->set_amnt( row, _amnt_vat_uk_o );
	spec_amnt_pre_euro_i  ->set_amnt( row, _amnt_pre_euro_i );
	spec_amnt_vat_euro_i  ->set_amnt( row, _amnt_vat_euro_i );
	spec_amnt_pre_euro_o  ->set_amnt( row, _amnt_pre_euro_o );
	spec_amnt_vat_euro_o  ->set_amnt( row, _amnt_vat_euro_o );
	spec_amnt_pre_excl    ->set_amnt( row, _amnt_pre_excl );
///	spec_amnt_vat_excl    ->set_amnt( row, _amnt_vat_excl );
	return true;
}


/*!
*/
bool help_RPT_base::create_table_and_specs_VatSum( data_set * dset,  reporter_base * R )
{
	create_table_and_specs_BASE( dset, R );

	/*
		THIS IS WRONG

		The table already exists, but these col_specs dont

		The QUICK alternative is to not call this twice
	*/
	if( already_existed ) return true;

	/*
		get some dest tables (weak way)
	*/
//	table * tbl_ACCT = dset->get_table("ACCT");
//	table * tbl_CAT  = dset->get_table("CAT");
//	table * tbl_VAT  = dset->get_table("VAT");
//	table * tbl_REF  = dset->get_table("REF");
//	table * tbl_ERA  = dset->get_table("ERA");

	spec_amnt    -> is_summable = true;
	spec_amnt    -> show_zero_as_empty = true;

	if(! create_specs_vats( dset, R ) ) return false;;


	if(! spec_key )		 return FALSE_dgb_fail("NULL SPEC");
	if(! spec_date_tax )	 return FALSE_dgb_fail("NULL SPEC");
///	if(! spec_date_era )	 return FALSE_dgb_fail("NULL SPEC");
	if(! spec_cat )		 return FALSE_dgb_fail("NULL SPEC");
	if(! spec_acct1 )	 return FALSE_dgb_fail("NULL SPEC");
	if(! spec_amnt )	 return FALSE_dgb_fail("NULL SPEC");
	if(! spec_vat )		 return FALSE_dgb_fail("NULL SPEC");
//	if(! spec_vat_rate )	 return FALSE_dgb_fail("NULL SPEC");

	if(! check_specs_vats() ) return false;

	if(! spec_ref )		 return FALSE_dgb_fail("NULL SPEC");
	if(! spec_desc )	 return FALSE_dgb_fail("NULL SPEC");
	if(! spec_img )	 return FALSE_dgb_fail("NULL SPEC");

	/*
		This defines the order of the columns in the table

		both spec_date_pay and _tax must be present otherwise
		the rel calculation fails, so how can that col be dropped?

		If you dont add_col() the col_pos comes out as (1??!)
		and it conflicts with the real one

		BUG 2005-04-18 - subtotals not matching display (VatSum pdf_doc)
		show_hidden_cols not used in base-class
	*/

	tbl->add_col( spec_key );
	tbl->add_col( spec_date_tax );
///	tbl->add_col( spec_date_era );
	tbl->add_col( spec_cat );
	tbl->add_col( spec_img );
	tbl->add_col( spec_desc );

	tbl->add_col( spec_ref );
	tbl->add_col( spec_acct1 );
	tbl->add_col( spec_amnt );
  	tbl->add_col( spec_turn_non_vat );	// VatSum (pre all vat)
	tbl->add_col( spec_vat );		// rate letter
  	tbl->add_col( spec_amnt_pre_excl ); // ie exclude from vat report
  //	tbl->add_col( spec_vat_rate );

  	tbl->add_col( spec_amnt_pre_uk_i );
  	tbl->add_col( spec_amnt_vat_uk_i );
  	tbl->add_col( spec_amnt_pre_uk_o );
  	tbl->add_col( spec_amnt_vat_uk_o );
  	tbl->add_col( spec_amnt_pre_euro_i );
  	tbl->add_col( spec_amnt_vat_euro_i );
  	tbl->add_col( spec_amnt_pre_euro_o );
  	tbl->add_col( spec_amnt_vat_euro_o );

	// putting on RHS

	/*
		RPT.tbl->key_spec -- must be set
	*/
	if(!tbl->key_spec)
	{
		// this needs to be set for the sort by 
///		tbl->key_spec = spec_date_era;
		tbl->key_spec = spec_key;
		// also key_desc
	}

	tbl->col_spec_tree = spec_date_tax; // tree by TAX date
	tbl->col_spec_sort_1 = spec_date_tax; // PAY oriented dates
	tbl->col_spec_sort_2 = spec_cat; // 
	tbl->desc_spec = spec_desc;

	/*
		ALL columns must be added to tbl,
		but you can hide ones you dont want visible.
	*/
///	spec_date_era->is_hidden = true; //
	spec_ref->is_hidden = true;
	spec_key->is_hidden = true;
	spec_amnt->is_hidden = true;
	spec_acct1->is_hidden = true;


	return true; // optimistic
}

////////////////////

