#include "reporter_plus.h"

using namespace ACCTS;

/*!
	CTOR: _base fetches all tables from dset, _plus adds "N2" and a few others

	prefetch N2, VAT_ACCT, HW_ACCT
*/
reporter_plus::
reporter_plus(
	data_set * _dset,
	str0 tablename
)
: reporter_base( _dset )
, RPT( _dset, this, tablename )
, skip_zero_amnt( false )	// skip
, show_N2_sums( false )		// cat == N2 causes a row of subtotals
, sort_date_pay( false )	// ie sort date_tax
, show_acct_cols( false )	// cashbook shows accounts, categorised doesnt
{

	/*
		every N2 causes a subtotals green-line for all accounts
	*/
	N2 = CAT.tbl->find_row( STR0_CAT_N2 );

	/*
		this had the awful result of an initial non-zero sum
		OK if account already existed
	*/
	VAT_ACCT = ACCT.tbl->find_row( STR0_VAT_ACCT );
	if(! VAT_ACCT ) {
		VAT_ACCT = ACCT.tbl->find_row_add_fake( STR0_VAT_ACCT );
		ACCT.tbl -> set_desc_of_row( VAT_ACCT, STR0_VAT_ACCT_desc );
	}

	HW_ACCT = ACCT.tbl->find_row( STR0_HW_ACCT );
	if(! HW_ACCT ) {
		HW_ACCT = ACCT.tbl->find_row_add_fake( STR0_HW_ACCT );
		ACCT.tbl -> set_desc_of_row( HW_ACCT, STR0_HW_ACCT_desc );
	}
}

/*!
	vanilla
*/
reporter_plus::~reporter_plus()
{
}

/*!
	move CAT to the right row - from TRANS not XFER?
	// for TRANS_CAT_is_Income()
	check that the key is a valid str
	caller shoudl do that too, but maybe not
*/
bool reporter_plus::CAT_fetch_TRANS_cat()
{
	if(! CAT.fetch_data( TRANS._cat )) return false;
	if(! CAT._key) return false;
	if(! CAT._key->str) return false;
	return true;
}

/*!
	maybe this should be a field set by mm3_scan/import

	maybe other T1T2T3 classes
 */
bool reporter_plus::TRANS_is_tax_layer()
{
	if(! CAT_fetch_TRANS_cat()) return false; // and has a key->str
	if(! CAT._key) return false;
	if(! CAT._key->str) return false;
	uchar c1 = CAT._key->str[0];
	switch( c1 ) {
	 case 'T': return true;
	 case 'I': return true;
	 default:  return false;
	}
	/*
		the above logic is a bit of a fluke for my site only
		there should be some bit set in the CAT transaction .csv
	*/
}

// move this to help_CAT
bool reporter_plus::TRANS_CAT_is_Income()
{
	if(! CAT_fetch_TRANS_cat()) return false; // and has a key->str
	if(! CAT._key) return false;
	if(! CAT._key->str) return false;
	uchar c1 = CAT._key->str[0];
	switch( c1 ) {
	 case 'I': return true;
	 case 'i': return true;
	 default:  return false;
	}
	/*
		the above logic is a bit of a fluke for my site only
		there should be some bit set in the CAT transaction .csv
	*/
}

/*!
	transaction vat calculation

	fetch the VAT rate
	calculate base + vat == total
	enter the VAT amnount into the VAT account
	enter the PRE base amount into the turnover account
	enter all the amounts into the I/O UK/EURO (turn) columns

	NB STRANGE MATH ACTION
*/      
void reporter_plus::do_vat_calc()
{

	RPT.clear_vat();

	RPT._amnt_pre_excl	.set_zero();
	RPT._amnt_pre		.set_zero();
	RPT._amnt_vat		.set_zero();

	uchar k = '0';		// vat rate key

	if( RPT._vat )
	{
		VAT.fetch_data( RPT._vat );
		k = VAT._key->str[0];
	}
	else
	{
		VAT._rate.set(0);
	}

	// gdb_invoke( true );

	/*
		although I have EURO fields, I currently dont use them
		the VAT calc is based on these common fields
		then split into UK _i or _o
	*/
	// A = b + c
	// 117 = 100 + 17
	VAT._rate.calc_abc_from_A(
		RPT._amnt,
		RPT._amnt_pre,
		RPT._amnt_vat
	);

	// amnt - vat = pre tax turnover (incl exc but not xfers!)
 	RPT. _turn_non_vat .set_zero();
 	RPT. _turn_non_vat .add( RPT._amnt );
 	RPT. _turn_non_vat .sub( RPT._amnt_vat );

	switch( k )
	{
	 case 'x':
			// VAT._rate should have been zero // CHECK IT?
			RPT._amnt_vat.set( 0 ); 
			RPT._amnt_pre.set( 0 ); // ???
			RPT._amnt_pre_excl.set( RPT._amnt );
	 break;
	 case 'e':
		if( TRANS_CAT_is_Income() )
		{
			RPT._amnt_pre_euro_o	.set( RPT._amnt_pre );
			RPT._amnt_vat_euro_o	.set( RPT._amnt_vat );
		} else {
			RPT._amnt_pre_euro_i	.set( RPT._amnt_pre );
			RPT._amnt_vat_euro_i	.set( RPT._amnt_vat );
		}
	 break;
	 default:
		if( TRANS_CAT_is_Income() )
		{
			RPT._amnt_pre_uk_o	.set( RPT._amnt_pre );
			RPT._amnt_vat_uk_o	.set( RPT._amnt_vat );
		} else {
			RPT._amnt_pre_uk_i 	.set( RPT._amnt_pre );
			RPT._amnt_vat_uk_i	.set( RPT._amnt_vat );
		}
	}

	/*
		add the amnt_vat into the VAT_ACCT journal

		note that a negative value becomes positive - to be reclaimed
	*/
 	acct_amnt( VAT_ACCT, RPT._amnt_vat, '-' );


	/*
		H HARDWARE ASSET
		appears line a TRANS - vat processed
		appears like a XFER - zero value

		H1 - ASSET TRANSFER
		H2 - asset depreciation ?

		I still dont like this, its almost a third category

		NB STRANGE MATH ACTION

		Maybe it should bring a new column (instead of the h account)
		or maybe the 'h' account should be summable (is that possible?)
	*/
	// if( CAT._key->str[0] == 'H' ) { }
	// if( (* CAT._key == "H1" ) || (* CAT._key == "H1" )) {
	if( (* CAT._key == "H1" ) ) {
		acct_amnt( HW_ACCT, RPT._amnt_pre, '-' );
		RPT. _turn_non_vat .set_zero();
	}
	/*
		TAX also has PRE_VAT turnover

		NB STRANGE MATH ACTION
	 */
	if( CAT._key->str[0] == 'T' ) { 
	// if( (* CAT._key == "H1" ) ) {
		// ADD TAX TO SOME REPORT ?? //
		// acct_amnt( HW_ACCT, RPT._amnt_pre, '-' );
		RPT. _turn_non_vat .set_zero();
	}

}

/*!
*/
col_spec_acct_sum * reporter_plus::find_spec_to_sum_acct_OK_NULL( row_inst * acct )
{
	if( !acct ) return NULL;
	return find_spec_to_sum_acct( acct );
}


/*!
	return RPT.acct_map[ acct.name ] ; or create new table column 

	Two arrays maintained in parallel:
		spec_sums
		_sums
		acct_map is the map of the spec_sums

	ACCT row must already exist!

	This has to be report not ROW because ACCT is needed (once) for initbal
*/
col_spec_acct_sum * reporter_plus::find_spec_to_sum_acct( row_inst * acct )
{
	if(!acct) return (col_spec_acct_sum*) NULL_dgb_fail("find_spec_to_sum_acct-NULL");

	// get acct.name
	ACCT.fetch_data( acct );
	str2 * acct_name = ACCT._key;

	// get RPT.acct_map[ acct.name ] 
	col_spec_acct_sum * sum_spec = RPT.acct_map.lookup( acct_name );
	if( !sum_spec )
	{
		// create new col_spec_acct_map

		amnt_t init_bal;
		init_bal = ACCT._initbal;

		sum_spec = new col_spec_acct_sum( acct_name, init_bal, acct );
		if(!sum_spec) return NULL;

		RPT.acct_map.add( acct_name, sum_spec );

// #warning "acct name used as reporter_plus.column_name - but it might clash"
		RPT.tbl->add_col( sum_spec );
		RPT.spec_sums.append( sum_spec );
		RPT._sums.extend_by_fill( 1, init_bal );
	}
	return sum_spec;
}

/*!
	+ add = subtract amnt to acct's running total RPT._sums[idx]

	uses spec_sums, changes _sums
*/
bool reporter_plus::acct_amnt( row_inst * acct, amnt_t amnt, char op )
{
	if(!acct) return true; // done as much as asked
	if( !amnt.pence ) return true; // no change

	col_spec_acct_sum * sum_spec = find_spec_to_sum_acct( acct );
	if(!sum_spec) return false;

	// find sub-col-pos of sum_spec in list spec_sums

	int n = RPT._sums.N();
	for( int i=0; i<n; i++ )
	{
		if( RPT.spec_sums[i] == sum_spec )
		{
			switch( op )
			{
			 case '+': RPT._sums[i].add( amnt ); break;
			 case '-': RPT._sums[i].sub( amnt ); break;
			 default:
				return FALSE_dgb_fail("acct_amnt - bad op");
			}
			return true;
		}
	}
	return FALSE_dgb_fail("acct_amnt - acct not found");
}

/*!
	at the end of the cashbook year
	update the amnt_closing values
	in memory at least

	Modified to copy all untouched accounts
	which dont appear in spec_sums[]
	BUT this gets called several times,
	whilst default must only be done once
*/
bool reporter_plus::update_amnt_closing()
{
	// ACCT.copy_all_balances();

	int n = RPT._sums.N();
	for( int i=0; i<n; i++ )
	{
		row_inst * acct = RPT.spec_sums[i]->acct;
		amnt_t & amnt = RPT._sums[i].sum;
		ACCT.fetch_data( acct );
		ACCT.write_back_end_bal( acct, amnt );
	}
	return true;
}

/*!
	compare by date_pay or date_tax, then key_id (input sequence)
*/
IS_DIFF_t
reporter_plus::
cmp_TRANS_XFER()
{
	IS_DIFF_t cmp1;
	if( sort_date_pay )
		cmp1 = TRANS._date_pay.cmp( XFER._date );
	else
		cmp1 = TRANS._date_tax.cmp( XFER._date );

	return cmp1;

	// this will always weight one way (trans then XFER) for generated keys

	if( cmp1 == IS_SAME ) {
		if( * TRANS._key && * XFER._key )
			cmp1 = TRANS._key->cmp( * XFER._key );
	}
	return cmp1;
}

/*!
	sweep through all transactions,
	looking for account names
	so that initial N2 isnt blank!

	this allows UNUSED accounts to not appear!
*/
bool reporter_plus::run_pre_merge_sweep()
{

	TRANS.iterator_restart();
	while( TRANS.iterator_get_row() )
	{
		find_spec_to_sum_acct_OK_NULL( TRANS._acct );
	}

	XFER.iterator_restart();
	while( XFER.iterator_get_row() )
	{
		find_spec_to_sum_acct_OK_NULL( XFER._acct1 );
		find_spec_to_sum_acct_OK_NULL( XFER._acct2 );
	}

	return true;
}

/*!
	run the basic report and leave a table for the default display

	merge TRANS and XFER 
	add lots of cols

	At end of year, the IS_MORE makes XFERS happen before TRANS
	so that the VAT on the 31'st Jan is before the N2 which is last.

	need to re-arrange col sequence ?


*/
bool reporter_plus::run_merge()
{
	if( RPT.already_existed ) return true;
	e_print("reporter_plus::run_merge()\n");

	if( sort_date_pay )
	{
		TRANS.tbl->sort_by_col_names(
			(str0) "date_pay"
		);
	} else {
		TRANS.tbl->sort_by_col_names(
			(str0) "date_tax"
		);
	}

	XFER.tbl->sort_by_col_names(
		(str0) "date"
	);

	// XFER before TRANS so that N2 gets end of day total
	bool do_trans_first_default = false;

	// maybe cmp_TRANS_XFER uses #key seq - when known 1,2

	TRANS.iterator_restart();
	TRANS.iterator_get_row(); // need date to work in - preloaded
	XFER.iterator_restart();
	XFER.iterator_get_row(); // need date to work in - preloaded
	while(1)
	{
		IS_DIFF_t cmp1 = cmp_TRANS_XFER();

		bool do_trans_first = do_trans_first_default;

		switch( cmp1 ) {
		 case IS_SAME:	// TRANS = XFER
			// retain default
			// do_trans_first = do_trans_first_default;
			break;
		 case IS_MORE:  // TRANS > XFER
			do_trans_first = false; // must do lower first (increasing)
			break;
		 case IS_LESS:  // TRANS < XFER
			do_trans_first = true; // must do lower first
			break;
		}

		if( do_trans_first )
		{
			copy_TRANS_to_RPT();
			if(! TRANS.iterator_get_row() ) break;
		}
		else
		{
			copy_XFER_to_RPT();
			if(! XFER.iterator_get_row() ) break;
		}
		// neither of these can happen ?
		if( TRANS.iterator_finished() ) break;
		if(  XFER.iterator_finished() ) break;
	}
	while( ! XFER.iterator_finished() )
	{
		copy_XFER_to_RPT();
		if(! XFER.iterator_get_row() ) break;
	}
	while( ! TRANS.iterator_finished() )
	{
		copy_TRANS_to_RPT();
		if(! TRANS.iterator_get_row() ) break;
	}

	// NOW only done by CachBook -
	// - seems more consistent
	// update_amnt_closing();
	return true; // optimistic
}


/*!
*/
bool reporter_plus::copy_TRANS_to_RPT()
{
	RPT.clear();

	RPT._key	  = TRANS._key;
	RPT._date_tax	  = TRANS._date_tax;
	RPT._date_pay	  = TRANS._date_pay;
//	RPT._date_pay_rel = TRANS._date_pay_rel;
//	RPT._date_tax_rel = TRANS._date_tax_rel;
	RPT._cat	  = TRANS._cat;
	RPT._acct1	  = TRANS._acct;
	RPT._acct2	  = NULL;
	RPT._amnt_xfer	  .set(0);
	RPT._amnt	  = TRANS._amnt;
	RPT._vat	  = TRANS._vat;
//	RPT._vat_rate	  = 
	RPT._ref	  = TRANS._ref;
	RPT._img	  = TRANS._img;
	RPT._desc	  = TRANS._desc;

	if(! CAT_fetch_TRANS_cat()) /* OK ? */;
	if( show_N2_sums )
		if( TRANS._cat == N2 )
			RPT.row_show_sums = true;
	if( show_acct_cols ) {
		acct_amnt( RPT._acct1, RPT._amnt, '+' );
	}
	if( skip_zero_amnt )
		if( RPT._amnt.pence == 0 ) return true;

	do_vat_calc();
	RPT.create_new_row_123();
	return true; // optimistic
}

/*!
*/
bool reporter_plus::copy_XFER_to_RPT()
{
	RPT.clear();
	RPT._key	  = XFER._key;
	RPT._date_tax	  = XFER._date;
//	RPT._date_pay_rel = XFER._date_pay_rel;
//	RPT._date_tax_rel = XFER._date_tax_rel;
	RPT._date_pay	  = XFER._date;
	RPT._cat	  = XFER._cat;
	RPT._acct1	  = XFER._acct1;
	RPT._acct2	  = XFER._acct2;
	RPT._amnt_xfer	  = XFER._amnt;
	RPT._amnt	  .set(0);
	RPT._vat	  = NULL;
	RPT._amnt_vat	  .set(0);
	RPT._amnt_pre	  .set(0);
	RPT._ref	  = XFER._ref;
	RPT._img	  = XFER._img;
	RPT._desc	  = XFER._desc;

	RPT._amnt_pre_excl	  .set_zero();

	if( show_acct_cols ) {
		acct_amnt( RPT._acct1, RPT._amnt_xfer, '-' );
		acct_amnt( RPT._acct2, RPT._amnt_xfer, '+' );
	}	

	do_vat_calc();
	RPT.create_new_row_123();

	return true; // optimistic
}



