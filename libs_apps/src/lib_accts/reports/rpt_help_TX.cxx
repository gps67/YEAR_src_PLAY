// #include "report2.h"
// #include "tbl_col_spec_plus.h"
// #include "dgb_fail.h"
// #include "tbl_col_spec_date_rel.h"

#include "rpt_help_TX.h"
#include "rpt_help_RPT_base.h"

using namespace ACCTS;

#define IFT( spec ) if( spec && spec->tbl ) spec // -> set_thing( row, value )


help_TX_base::help_TX_base()
: help_tbl()
{
	// spec fields are auto cleared

	clear_TX();
}


void help_TX_base::clear_TX()
{
	_key	= NULL;
	_date.clear();
	_date_tax.clear();
	_date_pay.clear();
	_cat	= NULL;
	_acct = NULL;
	_acct1 = NULL;
	_acct2 = NULL;
	_ref	= NULL;
	_img	= NULL;
	_desc	= NULL;
	_amnt.set_zero();
	_vat = NULL;

	// PLUS:	date_tax_rel
	// PLUS:	date_pay_rel

	_amnt_xfer.set_zero();
	_amnt_vat.set_zero();
	_amnt_pre.set_zero();
}

bool help_TX_base::write_data_both( row_inst * row )
{
//	spec_key         ->set_from_edit( row, *_key ); // EXCLUDED
 IFT(	spec_cat        ) ->set_obj( row, _cat ); // linked
 IFT(	spec_amnt       ) ->set_amnt( row, _amnt );
 IFT(	spec_ref        ) ->set_obj( row, _ref );
 IFT(	spec_img        ) ->set_from_edit( row, * _img );
 IFT(	spec_desc       ) ->set_from_edit( row, * _desc );
	return true;
}

bool help_TX_base::write_data_trans( row_inst * row )
{
	if( !spec_date_tax ) return false;
	if( !spec_date_tax->tbl ) return false;

 IFT(	spec_date_tax   ) ->set_date( row, _date_tax );
 IFT(	spec_date_pay   ) ->set_date( row, _date_pay );
 IFT(	spec_vat        ) ->set_obj( row, _vat ); // linked
if(0)	spec_acct	 ->set_obj( row, _acct ); // EXCLUDED
	return true;
}

bool help_TX_base::write_data_xfer( row_inst * row )
{
 IFT(	spec_acct1      ) ->set_obj( row, _acct1 ); // linked
 IFT(	spec_acct2      ) ->set_obj( row, _acct2 ); // linked
if(0)	spec_date	 ->set_date( row, _date ); // EXCLUDED
	return true;
}

bool help_TX_base::write_data_plus( row_inst * row )
{
 IFT(	spec_amnt_xfer  ) ->set_amnt( row, _amnt_xfer );
 IFT(	spec_amnt_vat   ) ->set_amnt( row, _amnt_vat );
 IFT(	spec_amnt_pre   ) ->set_amnt( row, _amnt_pre );

 IFT(	spec_date_tax_rel)->recalc( row );
 IFT(	spec_date_pay_rel)->recalc( row );
	return true;
}


/////////// MOVE THIS BACK HERE ???????????????
/////////// bool help_RPT_base::write_row_back_123( row_inst * row )

////////////////////


bool help_TX_base::get_specs_both()
{
	spec_key      =		  tbl ->must_get_col_spec( "key" );
	spec_cat      =		  tbl ->must_get_col_spec( "cat" );
	spec_ref      =		  tbl ->must_get_col_spec( "ref" );
	spec_img      =		  tbl ->must_get_col_spec( "img" );
	spec_desc     =		  tbl ->must_get_col_spec( "desc" );
// 	spec_amnt     = (col_spec_amnt*) tbl ->must_get_col_spec( "amnt" );
	spec_amnt     = dynamic_cast<col_spec_amnt*>( tbl ->must_get_col_spec( "amnt" ));
	// e_print( " spec_amnt -> type_name == '%s'\n", (STR0) spec_amnt -> type_name() );

	if(! check_specs_both() ) return false;
	return true; // optimistic
}

bool help_TX_base::get_specs_trans()
{
	spec_date_tax = (col_spec_date*) tbl ->must_get_col_spec( "date_tax" );
	spec_date_pay = (col_spec_date*) tbl ->must_get_col_spec( "date_pay" );
	spec_acct     =		  tbl ->must_get_col_spec( "acct" );
	spec_vat      =		  tbl ->must_get_col_spec( "vat" );
	if(! check_specs_trans() ) return false;
	return true;
}

bool help_TX_base::get_specs_xfer()
{
	spec_date  =(col_spec_date*) tbl->must_get_col_spec( "date" );
	spec_acct1 =                 tbl->must_get_col_spec( "acct1" );
	spec_acct2 =                 tbl->must_get_col_spec( "acct2" );
	if(! check_specs_xfer() ) return false;
	return true;
}

bool help_TX_base:: get_specs_all()
{
	return FALSE_dgb_fail("STUPID CODE");
}


bool help_TX_base::fetch_data_both( row_inst * row )
{
	_key	  = spec_key      -> get_str2_for_edit( row );
	_cat      = spec_cat      -> get_linked_row( row );
	_ref	  = spec_ref      -> get_linked_row( row );
	            spec_amnt     -> get_amnt( row, _amnt );
	_img      = spec_img      -> get_str2_for_edit( row );
	_desc	  = spec_desc     -> get_str2_for_edit( row );
	return true;
}

bool help_TX_base::fetch_data_trans( row_inst * row )
{
	            spec_date_tax -> get_date( row, _date_tax );
	            spec_date_pay -> get_date( row, _date_pay );
	_acct     = spec_acct     -> get_linked_row( row );
	_vat      = spec_vat      -> get_linked_row( row );
	return true;
}

bool help_TX_base::fetch_data_xfer( row_inst * row )
{
	         spec_date  -> get_date( row, _date );
	_acct1 = spec_acct1 -> get_linked_row( row );
	_acct2 = spec_acct2 -> get_linked_row( row );
	return true;
}



bool help_TX_base::check_specs_both()
{
	if(! spec_key )		 return FALSE_dgb_fail("NULL SPEC spec_key");
	if(! spec_cat )		 return FALSE_dgb_fail("NULL SPEC spec_cat");
	if(! spec_amnt )	 return FALSE_dgb_fail("NULL SPEC spec_amnt");
	if(! spec_ref )		 return FALSE_dgb_fail("NULL SPEC spec_ref");
	if(! spec_img )	 	 return FALSE_dgb_fail("NULL SPEC spec_img");
	if(! spec_desc )	 return FALSE_dgb_fail("NULL SPEC spec_desc");
	return true;

}

bool help_TX_base::check_specs_trans()
{
	if(! spec_date_tax )	 return FALSE_dgb_fail("NULL SPEC spec_date_tax");
	if(! spec_date_pay )	 return FALSE_dgb_fail("NULL SPEC spec_date_pay");
	if(! spec_vat )		 return FALSE_dgb_fail("NULL SPEC spec_vat");
	if(! spec_acct )	 return FALSE_dgb_fail("NULL SPEC spec_acct");
	return true;
}

bool help_TX_base::check_specs_xfer()
{
	if(! spec_date )	 return FALSE_dgb_fail("NULL SPEC spec_date");
	if(! spec_acct1 )	 return FALSE_dgb_fail("NULL SPEC spec_acct1");
	if(! spec_acct2 )	 return FALSE_dgb_fail("NULL SPEC spec_acct2");
	return true;
}

bool help_TX_base::check_specs_plus()
{
	if(! spec_date_tax_rel ) return FALSE_dgb_fail("NULL SPEC spec_date_tax_rel");
	if(! spec_date_pay_rel ) return FALSE_dgb_fail("NULL SPEC spec_date_pay_rel");
	if(! spec_amnt_xfer )	 return FALSE_dgb_fail("NULL SPEC spec_amnt_xfer");
	if(! spec_amnt_vat )	 return FALSE_dgb_fail("NULL SPEC spec_amnt_vat");
	if(! spec_amnt_pre )	 return FALSE_dgb_fail("NULL SPEC spec_amnt_pre");
	return true;
}

bool help_TX_base::check_specs_all()
{
	if(! check_specs_both() ) return false;
	if(! check_specs_trans() ) return false;
	if(! check_specs_xfer() ) return false;
	if(! check_specs_plus() ) return false;
	return true;
}




//////////////////////////


/*!
*/
help_TRANS::help_TRANS( data_set * dset )
: help_TX_base()
{
	late_init( dset, "TRANS" );
}

bool help_TRANS::fetch_data( row_inst * row )
{
	fetch_data_both( row );
	fetch_data_trans( row );
	return true; // optimistic
}

/*!
*/
bool help_TRANS::get_specs()
{
	if(! get_specs_both()	) return false;
	if(! get_specs_trans()	) return false;
	return true;
}


////////////////////

help_XFER::help_XFER( data_set * dset )
: help_TX_base()
{
	late_init( dset, "XFER" );;
}

/*!
*/
bool help_XFER::get_specs()
{
	if(! get_specs_both() ) return false;
	if(! get_specs_xfer() ) return false;

	return true;
}

/*!
*/
bool help_XFER::fetch_data( row_inst * row )
{

	fetch_data_both( row );
	fetch_data_xfer( row );
	return true;
}
