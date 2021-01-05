#include "dgb.h"
#include "run_report.h" // own decl ?
#include "str0.h"
#include "dset.h"
#include "rpt_help_tbl.h"
#include "buffer3.h"
#include "CH1_Names.h"
#include "CH1_Dset.h"
#include "rep_pdf_CH.h"
#include "rep_pdf_RUN_C.h"
#include "rpt_tbl_iter.h"
#include "fork_progs.h"
#include "word_splitter.h"

// #include "dgb_fail.h"

// #include "misc.h"
// #include "amnt_t.h"
// #include "date_glib.h"


/*
	using hardcoded values

	just edit this file amd call with different years
*/


using namespace PDF_GEN;
// using namespace SPOUT_GEN;
using namespace ACCTS;

// -----------------------------------------------------------------

class CH1_Machine // plain base
{
 public:
	int year2;
	CH1_Dset CH1_dset;

	obj_hold<data_set> dset_CURR;
	obj_hold<table> tbl_SUM_cat;
	obj_hold<col_spec_amnt> spec_SUM_turnover;
	obj_hold<col_spec_amnt> spec_SUM_pre_vat;

	// obtain pointers for reuse
	bool with_dset_CURR();
	bool with_dset_CURR_SUM_cat();
	bool with_spec_SUM_turnover();
	bool with_spec_SUM_pre_vat();

	bool run_CH1_YEAR2b( int year2 );
	bool init_from_year2( int year2 );
	bool change_CAT_D3_to_PL2_f1(); // before summing reports
	bool run_reports_prep_and_pdf();
	bool copy_CURR_CAT_sum_turnover_to_CH1();
	bool apply_sum( str0 pfx, str0 lhs, str0 rhs );
	bool apply_calc( str0 lhs, str0 rhs );
	bool apply_carry_forward( str0 key_new, str0 key_old );
	bool apply_copy_from( str0 lhs, str0 rhs );
	bool apply_copy_neg( str0 lhs, str0 rhs );
	bool apply_val_from_acct( str0 PFX_c, str0 acct );
	bool apply_sum_of_accts( str0 PFX_c, str0 acct_list_str );
	bool get_val_from_acct( str0 acct, amnt_t & amnt );
	bool check_neg_cat_pre_vat( str0 key_new, str0 key_old );
	bool apply_year_PL2_import() ;
	bool check_same_no_fix( str0 lhs, str0 rhs );
	bool get_CURR_CAT_sum_turnover( str0 key_name, amnt_t & amnt );
	bool get_CURR_CAT_sum_pre_vat( str0 key_name, amnt_t & amnt );
	bool get_NFS_TAX_d_from_T();
	bool apply_year_TFA_depreciation();

	STR0 P_name_amnt_cmnt( buffer1 & b1, STR0 name, amnt_t & amnt, STR0 cmnt = NULL );

	// cant use year2 because its Y2011_12a
	bool compare_two_CH1_columns( str0 col1, str0 col2 );
};

bool run_CH1_YEAR2( int year2 ) {
	CH1_Machine machine;
	machine.init_from_year2( year2 );
	return  machine.run_CH1_YEAR2b( year2 );
}

bool run_CH1_YEAR2_ZERO( int year2 ) {
	// zero out the year (add col)
	// you will have to move over manually
	CH1_Machine machine;
	machine.init_from_year2( year2 );
	machine.CH1_dset.save_play();
	return  true; // machine.run_CH1_YEAR2b( year2 );
}

bool run_CH1_check1(int year2)
{
	CH1_Machine machine;
	machine.init_from_year2( year2 );
	if(!machine.run_CH1_YEAR2b( year2 )) return FAIL_FAILED();

	return machine.compare_two_CH1_columns( "Y2010_11", "Y2010_11a" );
}

// -----------------------------------------------------------------

#define INFO_name_amnt_cmnt( b1, name, amnt, cmnt ) \
	 INFO("%s", P_name_amnt_cmnt( b1, name, amnt, cmnt ))

#define PASS_name_amnt_cmnt( b1, name, amnt, cmnt ) \
	 PASS("%s", P_name_amnt_cmnt( b1, name, amnt, cmnt ))

STR0 CH1_Machine:: P_name_amnt_cmnt( buffer1 & b1, STR0 name, amnt_t & amnt, STR0 cmnt )
{
	buffer1 b2;
	if(!cmnt) cmnt = "";
	b1.clear();
	b1.print( "%-9s %10s %s",
		name,
		(STR0) amnt.get_nocurr_str( b2 ),
		cmnt
	);
	return b1; 
}

bool CH1_Machine:: init_from_year2( int _year2 ) {

	year2 = _year2;

	if(!CH1_dset.init_from_year2( year2 ))
		return FAIL_FAILED();

	CH1_dset.zero_column();
	CH1_dset.save_play(); // see if OK, if new col added
	return true;
}

bool CH1_Machine:: change_CAT_D3_to_PL2_f1()
{
	// this.get_dset_curr().CAT.D3.key = "PL2_f1" // wide LEAF key
	//
	// dset_curr
	// dset_prev
	// dset_CH1
	//
	obj_hold<table> curr_CAT = dset_CURR->get_table( "CAT" );
	if(! curr_CAT ) return FAIL("CAT");
	//
	obj_hold<row_inst> curr_CAT_D3 = curr_CAT -> find_row( "D3" );
	if(! curr_CAT_D3 ) return FAIL("curr_CAT_D3");
	//
	obj_hold<col_spec> spec_curr_key = curr_CAT->must_get_col_spec("key");
	if(!spec_curr_key) return FAIL_FAILED();
	//
	spec_curr_key-> set_from_edit( curr_CAT_D3, "PL2_f1" );

	return true;
}

bool CH1_Machine:: run_reports_prep_and_pdf()
{
        // dset_curr.run_reports();                // prep tables
        // this.add_depreciation_TRANS();          // BUT DONT ADD TWICE !!!
        // dset_curr.rep_pdf( "Cash_Cat", f3 );    // main report
        // dset_curr.rep_pdf( "VatTrack", f4 );    // euro vat report

	// run_reports
	bool ok = true;
	ok = ok && run_report_1( dset_CURR );
	ok = ok && run_report_2( dset_CURR );
	ok = ok && run_report_3( dset_CURR );
	ok = ok && run_report_4( dset_CURR );
	if(!ok) return FAIL_FAILED();

/*
        // this.add_depreciation_TRANS();          // BUT DONT ADD TWICE !!!
 {
        // this is now done manually - with an exception if not!
        print("# ++++ # add_depreciation_TRANS()");
        this.CH1.need_dset_CH1()
        var deprecr = this.CH1.get_curr( "NFS_TFA_i" );
        print("# ---- # add_depreciation_TRANS()" + deprecr);
  }
*/

        // dset_curr.rep_pdf( "Cash_Cat", f3 );    // main report
        // dset_curr.rep_pdf( "VatTrack", f4 );    // euro vat report
	// errm do the above do anything of worth ? sideeffects?
	// so doing anyway
	//

	buffer1 b1;

	str0 s_PL2_CashCat_pdf;
	s_PL2_CashCat_pdf = str_CH_name( b1, year2, e_PL2_CashCat_pdf );
	if(! run_pdf_Cash_Cat( dset_CURR, s_PL2_CashCat_pdf )) return FAIL_FAILED();

	str0 s_PL2_VatTrack_pdf;
	s_PL2_VatTrack_pdf = str_CH_name( b1, year2, e_PL2_VatTrack_pdf );
	if(! run_pdf_VatTrack( dset_CURR, s_PL2_VatTrack_pdf )) return FAIL_FAILED();

	// dset_curr.hide_table( "SUM_cat", false );       // this is the MAIN outp
	obj_hold<str2> name2 = new str2( "SUM_cat" );
	dset_CURR->hide_table( name2, false );


 if(1)
	if(!dset_CURR->file_save(  str_CH_name( b1, year2, e_csv_CURR_PL2 )))
		return FAIL_FAILED();

	return true;
}

bool CH1_Machine:: with_dset_CURR()
{
	if(dset_CURR) return true; // inline check, the rest if _fn

	buffer1 b_csv_CURR;
	const char * s_csv_CURR = str_CH_name( b_csv_CURR, year2, e_csv_CURR );
	dset_CURR = new data_set();
	if(!dset_CURR) return FAIL_FAILED();
	if(!dset_CURR->file_load( s_csv_CURR )) return FAIL_FAILED();
	return PASS("%s", s_csv_CURR );
}

bool CH1_Machine:: with_dset_CURR_SUM_cat()
{
	// fast exit - already done
	if(tbl_SUM_cat) return true; // inline check, the rest if _fn

	// uses dset
	if(!with_dset_CURR()) return FAIL_FAILED();

	// obtain table
	tbl_SUM_cat = dset_CURR -> get_table_or_FAIL("SUM_cat");
	if(!tbl_SUM_cat) return FAIL_FAILED();

	return true;
}

bool CH1_Machine:: with_spec_SUM_turnover()
{
	// fast exit - already done
	if( spec_SUM_turnover ) return true; // inline - all done

	// uses table
	if(! with_dset_CURR_SUM_cat() ) return FAIL_FAILED();

	// obtain col_spec
	spec_SUM_turnover
	= tbl_SUM_cat -> must_get_col_spec_amnt("turnover");
	if(! spec_SUM_turnover ) return FAIL_FAILED();
	return true;
}

bool CH1_Machine:: with_spec_SUM_pre_vat()
{
	// fast exit - already done
	if( spec_SUM_pre_vat ) return true; // inline - all done

	// uses table
	if(! with_dset_CURR_SUM_cat() ) return FAIL_FAILED();

	// obtain col_spec
	spec_SUM_pre_vat
	= tbl_SUM_cat -> must_get_col_spec_amnt("amnt_pre_vat");
	if(! spec_SUM_pre_vat ) return FAIL_FAILED();
	return true;
}

bool CH1_Machine:: get_CURR_CAT_sum_pre_vat( str0 key_name, amnt_t & amnt ) 
{
	// uses col_spec
	if(! with_spec_SUM_pre_vat() ) return FAIL_FAILED();

	// obtain row
	row_inst * SUM_cat_row = tbl_SUM_cat -> find_row_or_FAIL( key_name );
	if( !SUM_cat_row ) return FAIL_FAILED();

	// get amnt from row
	spec_SUM_pre_vat -> get_amnt( SUM_cat_row, amnt );

	return true;
}

// within CH1 table
bool CH1_Machine:: apply_copy_from( str0 lhs, str0 rhs )
{
	amnt_t amnt;
	if(!CH1_dset.get_CURR( rhs, amnt ) ) return FAIL_FAILED();
	if(!CH1_dset.set_CURR( lhs, amnt ) ) return FAIL_FAILED();
	buffer1 b1;
	return PASS_name_amnt_cmnt( b1, lhs, amnt, rhs );
	return true;
}

// within CH1 table
bool CH1_Machine:: apply_copy_neg( str0 lhs, str0 rhs )
{
	amnt_t amnt;
	if(!CH1_dset.get_CURR( rhs, amnt ) ) return FAIL_FAILED();
	amnt.negate();
	if(!CH1_dset.set_CURR( lhs, amnt ) ) return FAIL_FAILED();
	buffer1 b1;
	return PASS_name_amnt_cmnt( b1, lhs, amnt, rhs );
	return true;
}

bool CH1_Machine:: get_val_from_acct( str0 acct, amnt_t & amnt )
{
// from ACCT of mm3_CURR
// into CH1 table
//	return this.CH1.dset_curr.ACCT[ acct ].amnt_closing

	// could move to static // uses tbl_ACCT
	obj_hold<table> tbl_ACCT;
	tbl_ACCT = dset_CURR -> get_table_or_FAIL( "ACCT" );
	if(!tbl_ACCT) return FAIL_FAILED();

	// could move to static // uses col_spec_ACCT_closing
	// nb acct[n] has no row in table!
	obj_hold<col_spec_amnt> spec_acct_closing;
	spec_acct_closing = tbl_ACCT->must_get_col_spec_amnt( "amnt_closing" );
	if(!spec_acct_closing) return FAIL_FAILED();

	// obtain row for account
	row_inst * acct_row = tbl_ACCT -> find_row( acct );
	if(!acct_row) {
		amnt.set_zero(); // overwrite as damage limitation
		if( acct == "n" ) {
			WARN("OK using ZERO for 'n' accont" );
			return true;
		}
		// generate standard error message
		acct_row = tbl_ACCT -> find_row_or_FAIL( acct );
	}
	if(!acct_row) return FAIL_FAILED();

	// get closing bal
	if(!spec_acct_closing -> get_amnt( acct_row, amnt )) {
		return FAIL_FAILED();
	}

	buffer1 b1;
	return PASS_name_amnt_cmnt( b1, acct, amnt, "account" );
}

// from mm3.ACCT[a].closing
// into CH1.CH1[PFX_c].CURR
bool CH1_Machine:: apply_val_from_acct( str0 PFX_c, str0 acct )
{
	amnt_t amnt;
	if(!get_val_from_acct( acct, amnt )) return FAIL_FAILED();
	if(!CH1_dset.set_CURR( PFX_c, amnt )) return FAIL_FAILED();
	buffer1 b1;
	return PASS_name_amnt_cmnt( b1, (STR0) PFX_c, amnt, (STR0) acct );
	return true;
}

bool CH1_Machine:: apply_sum_of_accts( str0 PFX_c, str0 acct_list_str )
{
// from mm3
// into CH1
// several accounts can have one account meaning
	buffer1 b1;
	word_splitter spl;
	spl.split_by_spaces( acct_list_str );
	amnt_t amnt_sum;
	int n = spl.N();
	for(int i = 0 ; i < n; i++ ) {
		STR0 acct_name = spl[i];
		amnt_t amnt_val;
		if(!get_val_from_acct( acct_name, amnt_val ))
			return FAIL_FAILED();
		if(!amnt_sum.add( amnt_val )) return FAIL_FAILED();
		INFO_name_amnt_cmnt( b1, "account", amnt_val, (STR0) acct_name );
	}
	if(!CH1_dset.set_CURR( PFX_c, amnt_sum )) return FAIL_FAILED();
	return PASS_name_amnt_cmnt( b1, (STR0) PFX_c, amnt_sum, (STR0) acct_list_str );
	return true;
}

/*
	from CURR years csv get the SUM_cat[ key_name ]
	get the turnover part. not vat or other
	used to get the 'T' item which is renamed as NFS2_TAX_d or something
	but not renamed in PL2 because D-T-O-X-I-N are not renamed (?)
*/
bool CH1_Machine:: get_CURR_CAT_sum_turnover( str0 key_name, amnt_t & amnt ) 
{
	if(! with_spec_SUM_turnover() ) return FAIL_FAILED();

	// lookup same key in CURR
	row_inst * SUM_cat_row = tbl_SUM_cat -> find_row( key_name );
	if( !SUM_cat_row ) {
		INFO("dset_CURR debug name = %s", dset_CURR -> debug_get_obj_name() );
		INFO("tbl_SUM_cat debug name = %s", tbl_SUM_cat -> debug_get_obj_name() );
		return FAIL("missing key '%s'", (STR0) key_name );
	}

	// by now SPEC is guaranteed (not fed NULLS)
	spec_SUM_turnover -> get_amnt( SUM_cat_row, amnt );

	return true;
}

/*!
	this TEST corrupts col_spec_CURR and _PREV as _CURR and CURR_a
*/
bool CH1_Machine:: compare_two_CH1_columns( str0 col_CURR, str0 col_PREV )
{
	bool ok_ish = true;
	CH1_dset.iter1_use_CH1(); // init

        // need to add a new column to the LEFT of previous year
        // OK if it already exists ?
        CH1_dset.col_spec_CURR = (col_spec_amnt*) CH1_dset.tbl_CH1->must_get_col_spec( col_CURR );
        CH1_dset.col_spec_PREV = (col_spec_amnt*) CH1_dset.tbl_CH1->must_get_col_spec( col_PREV );
        if(!CH1_dset.col_spec_CURR) {
                return FAIL("previous years data MUST exist (%s)",
                        (STR0) col_CURR );
        }
        if(!CH1_dset.col_spec_PREV) {
                return FAIL("previous years data MUST exist (%s)",
                        (STR0) col_PREV );
        }

// gdb_invoke(true);

	if(! CH1_dset.iter1_check_ok() ) return FAIL_FAILED();
	while( CH1_dset.iter1_get_row() ) {
		str2 * key_name = CH1_dset.iter1_get_CH1_row_key();
		amnt_t val_curr;
		amnt_t val_diff;
		amnt_t val_prev_p;
		obj_hold<str2> desc2;
		if(!CH1_dset.get_CURR_row_val( key_name,  val_curr )) return FAIL_FAILED();
		if(!CH1_dset.get_PREV_row_val( key_name,  val_prev_p )) return FAIL_FAILED();
		if(!CH1_dset.get_DESC( key_name, desc2 )) return FAIL_FAILED();
		val_diff = val_curr;
		val_diff.set( val_curr );
		val_diff.sub( val_prev_p );
		buffer1 line;
		buffer1 b1;
		buffer1 b2;
		buffer1 b3;
		line.print( "%-10s", (STR0) key_name->str ); // OLD MADNESS need for ->str ???
		line.print( " %10s", (STR0) val_curr.get_nocurr_str(b1) );
// 		if(val_diff.is_negative()) line.print("(NEG)"); else line.print(".....");
		line.print( " %10s", (STR0) val_diff.get_nocurr_str(b3) );
// 		if(val_prev_p.is_negative()) line.print("(NEG)"); else line.print(".....");
		line.print( " %10s", (STR0) val_prev_p.get_nocurr_str(b2) );
		line.print( " %-10s", (STR0) desc2->str );
		INFO("%s", (STR0) line );
	}
	return ok_ish;
}

bool CH1_Machine:: copy_CURR_CAT_sum_turnover_to_CH1() 
{
        
//	// -------------------------------------------------------------
	// START
	// this.adj_PL2_copy_over_attrs( this.get_dset_curr() )
	//
	// foreach attribute in CH1 copy SUM_cat to CH1
	// if missing - may be an added later attr
	//
//	// --------


	if(! with_spec_SUM_turnover() ) return FAIL_FAILED();

	const char * found;

	// foreach row of CH1
	CH1_dset.iter1_use_CH1(); // init
	if(! CH1_dset.iter1_check_ok() ) return FAIL_FAILED();
	while( CH1_dset.iter1_get_row() ) {
		str2 * key_name = CH1_dset.iter1_get_CH1_row_key();

		// lookup same key in CURR
		row_inst * SUM_cat_row = tbl_SUM_cat -> find_row( key_name );
		if( !SUM_cat_row ) {
			found = "----- ABSENT";
			INFO("key = %s %s", found, (STR0) key_name->str );
		//	INFO("key = %s", (STR0) key_name );
		//	why didnt auto cast work str2* => STR0 ??
			continue;
		}
		found = "FOUND ------";

		// when found copy attribute from SUM_cat to CH1
		// when not found skip and loop around
		if( !SUM_cat_row ) {
			INFO("key = %s %s NOT IN SUM_cat", found, (STR0) key_name->str );
			continue;
		}

		// get SUM_cat[ key ].turnover
		// this is when the ROUNDING takes place
		// which may be a problem for last year
		// after this, rounded sums add upp rounded
		// except of course % depreciation etc

		amnt_t amnt;
		spec_SUM_turnover -> get_amnt( SUM_cat_row, amnt );
		amnt.round_for_CH1();

		buffer1 b2;
		if(1) {
			INFO("key = %s %s",
			 found,
			 P_name_amnt_cmnt( b2, (STR0) key_name->str, amnt )
			);
		}

		// set CH1[ row_with_key ].Y2011_12 = amnt
	// Y2010_11
		CH1_dset.iter1_set_current_row_CURR_col_to_rounded_amnt( amnt );
	}
//	// --------
	// END
	// this.adj_PL2_copy_over_attrs( this.get_dset_curr() )
//	// -------------------------------------------------------------
	return true;
}

bool CH1_Machine:: apply_year_PL2_import() 
{
//	.check_year_PL2_import start
	apply_sum( "PL2", "c", "a b" );
	apply_sum( "PL2", "o", "d e f g h i j k l m n" );
	apply_sum( "PL2", "p", "c o" );
	apply_sum( "PL2", "t", "q r s" );
	apply_sum( "PL2", "u", "p t" );
	apply_sum( "PL2", "w", "u v" );
//	.check_year_PL2_import end
	return("DONE");
}

bool CH1_Machine:: get_NFS_TAX_d_from_T()
{

/*
	This is a tad daft, 'T' is not found
	and looking, it isnt there
	
	MORE-OVER it is the UK Corporate tax figure - which need computation

	OVER-MORE it is later copied into PL1_j
*/
// gdb_invoke(true);
//	check_cat_amnt_not_turnover( "NFS_TAX_d", "T" )    // tax from MM3
	amnt_t amnt_T;
 //	get_cat_amnt_not_turnover( "T", amnt_T );    // tax from MM3
 //	get_CURR_CAT_sum_turnover( "T", amnt_T );    // tax from MM3
	if(! get_CURR_CAT_sum_turnover( "T",  amnt_T )) {
	 if(0) // should be 1
		return FAIL_FAILED();
		WARN("Ignoring missing T");
		amnt_T.set_zero();
#warning "TAX COMPUTATION NFS_TAX_d"
	}
	amnt_T.round_for_CH1();
	if(! CH1_dset.set_CURR( "NFS_TAX_d",  amnt_T ))
		return FAIL_FAILED();
	return PASS("DONE");
}

bool CH1_Machine:: apply_year_TFA_depreciation()
{
	// Tangible Fixed Assets - calcs
	// NB THIS IS NOT THE WDV - Written Down Value !!!!
	// thats what you get from 25-hour/3-day week workers

	// carry forward previous years, staggered end/start
	apply_carry_forward( "NFS_TFA_f", "NFS_TFA_g" );
	apply_carry_forward( "NFS_TFA_h", "NFS_TFA_j" );
	apply_carry_forward( "NFS_TFA_l", "NFS_TFA_k" );

	// get additions from ... PL2/_/H1
	// H1 is special ;-)
	// It has zero turnover
	// must use "pre_vat" field (not turnover)
	check_neg_cat_pre_vat( "NFS_TFA_F", "H1" );
	// H2 is depreciation ONLY - which we are checking !

	// total ever purchased (daft way of calculation same)
	apply_calc( "NFS_TFA_g", "NFS_TFA_f NFS_TFA_F" );

	// Charge for year = depreciation
	// start + additions * 25% (-ve)
	// 0606
	// rate = CH1.get_rate_depreciation();
	// NOTE: must be careful with the - that gets prefixed to it, and use in str
	STR0 rate_str = "0.20"; //  rate = CH1.get_rate_depreciation();
	rate_str = "0.25"; // CH is not the same as IR, but I am merging
	rate_str = "0.18"; // 2011-12 and then on ?
	// it also varies between CH and IR
	switch( year2 ) {
	 case 2011:
		rate_str = "0.2"; // why did I use this daft value
		break;
	 case 2012:
		rate_str = "0.25"; // why did I use this daft value
		break;
	 case 2013:
		rate_str = "0.18"; // why did I use this daft value
		break;
	 case 2014:
		rate_str = "0.18"; // why did I use this daft value
		break;
	 default:
		rate_str = "0.2"; // why did I use this daft value
	}
#warning depreciation rate shoud vary with year and CH vs IR
	INFO( "USING depreciation RATE: %s", rate_str );
	buffer1 b_calc;
	b_calc.print("NFS_TFA_l + NFS_TFA_F * -%s", rate_str ); // * -0.25
	INFO("apply_calc( 'NFS_TFA_i', '%s'", (STR0) b_calc );
	apply_calc( "NFS_TFA_i", (STR0) b_calc );
	// apply_calc( "NFS_TFA_i", "NFS_TFA_l + NFS_TFA_F * -0.25" ) // CHARGE is -ve

	// total ever depreciated
	apply_calc( "NFS_TFA_j", "NFS_TFA_h + NFS_TFA_i" );

	// end = start + addition + charge # charge for year is already negative
	apply_calc( "NFS_TFA_k", "NFS_TFA_l + NFS_TFA_F + NFS_TFA_i" );     //
	// EXPORT BACKWARDS BY COMPLAINING
	// AS-IF COPY RESULT TO INPUT - BUT NOT
	// THIS THROWS AN EXCEPTION - NOT JUST A MESSAGE, IT STOPS
	check_same_no_fix( "PL2_v", "NFS_TFA_i" );

	return PASS("DONE");
}

bool CH1_Machine:: apply_sum( str0 pfx, str0 lhs, str0 rhs )
{
	buffer1 b1;
	buffer1 b_PL2_c;
	buffer1 b_PL2_x;
	b_PL2_c.print( "%s_%s", (STR0) pfx, (STR0) lhs );
	INFO("%s =", (STR0) b_PL2_c );

	word_splitter spl;
	spl.split_by_spaces( rhs );
	amnt_t amnt_sum;
	amnt_t amnt_val;
	int n = spl.N();
	for( int i =0; i<n; i++ ) {
		b_PL2_x.clear();
		b_PL2_x.print("%s_%s", (STR0) pfx, (STR0) spl[i] );
		if(! CH1_dset.get_CURR( b_PL2_x, amnt_val )) {
			INFO("PL2_c = %s", (STR0) b_PL2_c );
			return FAIL_FAILED();
		}
		if(! amnt_sum.add( amnt_val ) ) {
			INFO("PL2_c = %s", (STR0) b_PL2_c );
			INFO("PL2_x = %s", (STR0) b_PL2_x );
			return FAIL_FAILED();
		}
		b1.clear();
		INFO_name_amnt_cmnt( b1, (STR0) b_PL2_x, amnt_val, "val" );
	}
	if(! CH1_dset.set_CURR( b_PL2_c, amnt_sum )) {
		b1.clear();
		INFO_name_amnt_cmnt( b1, (STR0) b_PL2_x, amnt_sum, "amnt_sum" );
		return FAIL_FAILED();
	}
		b1.clear();
	INFO_name_amnt_cmnt( b1, (STR0) b_PL2_c, amnt_sum, "===" );
	
	return true;
	return FAIL("TODO");
}

bool CH1_Machine:: apply_calc( str0 lhs, str0 rhs )
{
	buffer1 b1;
	INFO("%s = %s", (STR0) lhs, (STR0) rhs );

	enum en_OP {
		en_OP_plus,
		en_OP_minus,
		en_OP_times,
		en_OP_divide
	};

	en_OP OP = en_OP_plus;

	word_splitter spl;
	spl.split_by_spaces( rhs );
	double amnt_dbl;
	amnt_t amnt_sum;
	amnt_t amnt_val;
	int n = spl.N();
	for( int i =0; i<n; i++ ) {
		str0 word = (STR0) spl[i];
		if(word == "+") {
			OP = en_OP_plus;
			continue;
		} else if(word=="-") {
			OP = en_OP_minus;
			continue;
		} else if(word=="*") {
			OP = en_OP_times;
			continue;
		} else if(word=="/") {
			OP = en_OP_divide;
			continue;
//		} else if(word=="PUSH") {
//		} else if(word=="POP") {
		} else if(word.is_double(amnt_dbl)) {
			amnt_val.set_pounds( amnt_dbl );
			
			b1.clear();
			INFO_name_amnt_cmnt( b1, "AMNT", amnt_val, "amnt_val" );
			// stay to add or mult it to amnt_sum
		} else {
			// will stay 
			// convert PL2_c to value
			str0 b_PL2_x = (STR0) word;
			if(! CH1_dset.get_CURR( b_PL2_x, amnt_val )) {
				return FAIL("word = %s", (STR0) word );
			}
			b1.clear();
			INFO_name_amnt_cmnt( b1, (STR0) b_PL2_x, amnt_val, "amnt_val" );
		}
		bool ok = true;
		switch( OP ) {
		       case en_OP_plus: ok = amnt_sum.add( amnt_val );
		break; case en_OP_minus: ok = amnt_sum.sub( amnt_val );
		break; case en_OP_times: ok = amnt_sum.mult( amnt_val );
		break; case en_OP_divide: ok = amnt_sum.divide( amnt_val );
		break; default:
			ok = FAIL("bad code - no such en_OP opcode");
		}
		if(! ok ) {
			// not sure how that fails - out of range ?
			return FAIL_FAILED();
		}
	}
	if(! CH1_dset.set_CURR( lhs, amnt_sum )) {
		b1.clear();
		INFO("%s = %10s",
		 (STR0) lhs,
		 (STR0) amnt_sum.get_nocurr_str(b1)
		);
		return FAIL_FAILED();
	}
	b1.clear();
	INFO_name_amnt_cmnt( b1, (STR0) lhs, amnt_sum, "===" );
	return true;
}

bool CH1_Machine:: apply_carry_forward( str0 key_new, str0 key_old )
{
	amnt_t amnt;
	if(!CH1_dset.get_PREV( key_old, amnt ) ) return FAIL_FAILED();
	if(!CH1_dset.set_CURR( key_new, amnt ) ) return FAIL_FAILED();

	buffer1 b1;
	return PASS_name_amnt_cmnt( b1, key_new, amnt, key_new );
	return PASS("%s = %s %10s",
		(STR0) key_new,
		(STR0) key_old,
		(STR0) amnt.get_nocurr_str(b1)
	);
	return true;
}

bool CH1_Machine:: check_neg_cat_pre_vat( str0 key_new, str0 key_old )
{
	// check_neg_cat_pre_vat( "NFS_TFA_F", "H1" );
	amnt_t amnt;
	if(!get_CURR_CAT_sum_pre_vat( key_old, amnt ))
		return FAIL_FAILED();
	amnt.negate();
	if(!CH1_dset.set_CURR( key_new, amnt ))
		return FAIL_FAILED();
	buffer1 b1;
	return PASS_name_amnt_cmnt( b1, key_old, amnt, key_new );
	return PASS("%s = %s %10s",
		(STR0) key_new,
		(STR0) key_old,
		(STR0) amnt.get_nocurr_str(b1)
	);
}

bool CH1_Machine:: check_same_no_fix( str0 lhs, str0 rhs )
{
	// DO NOT SET (or fix), only compare

	amnt_t val_lhs;
	amnt_t val_rhs;

	if(!CH1_dset.get_CURR( lhs, val_lhs )) return FAIL_FAILED();
	if(!CH1_dset.get_CURR( rhs, val_rhs )) return FAIL_FAILED();
	
	buffer1 b1;
	buffer1 b2;
	if( val_lhs.pence != val_rhs.pence )
	{
		INFO_name_amnt_cmnt( b1, (STR0) lhs, val_lhs, (STR0) "<>" );
		INFO_name_amnt_cmnt( b1, (STR0) rhs, val_rhs, (STR0) "<>" );
		return FAIL("%s != %s %10s != %10s",
			(STR0) lhs,
			(STR0) rhs,
			(STR0) val_lhs.get_nocurr_str(b1),
			(STR0) val_rhs.get_nocurr_str(b2)
		);
	}
	
	
	return PASS("%s = %s %10s",
		(STR0) lhs,
		(STR0) rhs,
		(STR0) val_lhs.get_nocurr_str(b1)
	);
}

bool CH1_Machine:: run_CH1_YEAR2b( int year2 ) {

	INFO("year2 = %d", year2 );
#if 0
	CH1_Names CH1_names;
	CH1_names.init_from_year2( year2 );
#endif
	if(!CH_Name_Tag_show_test(year2)) return FAIL_FAILED();

	if(!with_dset_CURR()) return FAIL_FAILED();
	/*
		dset_CURR = new data_set();
		if(!dset_CURR->file_load( s_csv_CURR )) return FAIL_FAILED();
	*/

/*
  this.adj_cat_grp =
  function adj_cat_grp( )
  {
        this.adj_PL2_base( )
        this.adj_PL2_wide_key_leaf( )
        this.adj_PL2_trackers( ) // runs reports that create SUM_cat etc
        this.adj_PL2_copy_over_attrs( this.get_dset_curr() )
  }
*/


        // this.adj_PL2_base( )
	// load CH1_adj_PL2.csv
	//
	// REUSES // buffer b1;
	buffer1 b_adj;
	str0 s_adj = str_CH_name( b_adj, year2, e_csv_CAT_adj );
	INFO("adjusting CAT with %s", (STR0) s_adj );
	INFO("this changes the debug object name but should work");
	if(!dset_CURR->file_load( s_adj )) return FAIL_FAILED();

	// this.get_dset_curr().CAT.D3.key = "PL2_f1" // wide LEAF key
	if(! CH1_Machine:: change_CAT_D3_to_PL2_f1() ) return FAIL_FAILED();


        // dset_curr.run_reports();                // prep tables
        // this.add_depreciation_TRANS();          // BUT DONT ADD TWICE !!!
        // dset_curr.rep_pdf( "Cash_Cat", f3 );    // main report
        // dset_curr.rep_pdf( "VatTrack", f4 );    // euro vat report
	if(! run_reports_prep_and_pdf() ) return FAIL_FAILED();

	// ??? ???
	// end js fn this.adj_PL2_trackers()

	// where CH1.CH1 has key, copy CURR.CAT_sum[ key ].turnover
	if(! copy_CURR_CAT_sum_turnover_to_CH1() ) return FAIL_FAILED();
	// IS this.adj_PL2_copy_over_attrs( this.get_dset_curr() )

//	// END // function adj_cat_grp()

/*
	main_adj_Y1Y1
		CH1.adjust_cat_grp()
		CH1.check_year_correct()
		CH1.rep_pdf_CH( true, 6 )


	SO NOW:
		.../PRIV_CH_CSV-2013-04-11/cls_CH1_checker.js
		CH1.check_year_correct()
		   .check_year_base()
			.check_year_PL2_import
			...
*/


	FAIL("errno has not been cleared = %d", (int)errno);
#warning FAIL("errno has not been cleared");
errno_zero();

	if(! apply_year_PL2_import() ) return FAIL_FAILED();

	if(! get_NFS_TAX_d_from_T() ) return FAIL_FAILED();
	if(! apply_year_TFA_depreciation() ) return FAIL_FAILED();

// --------------------
// CURRENT EDIT
        // propagate NFS_TFA_i and others to other forms
        apply_copy_from( "BAL_a", "NFS_TFA_k" ); // nett book value
        apply_copy_from( "NFS_OP_a", "NFS_TFA_i" ); // depreciation 
        apply_copy_from( "NFS_OP_b", "PL2_f" ); // pension
        apply_copy_from( "NFS_OP_c", "PL2_d" ); // emoluments

        /*
                need to split the 't' account into 
        */
        // Debtors
        apply_val_from_acct( "NFS_DBT_M", "i" );; // trade debtors
        // Creditors
        apply_val_from_acct( "NFS_CRD_m", "g" );    // directors ACCTS
        apply_val_from_acct( "NFS_CRD_n", "n" );    // t_NI calc
        apply_val_from_acct( "NFS_CRD_o", "v" );    // t_VAT calc acct
        apply_val_from_acct( "NFS_CRD_p", "t" );    // CORP TAX
        apply_val_from_acct( "NFS_CRD_q", "o" );    // arbitrary ACCRUED expenses
        apply_sum( "NFS_CRD", "r", "m n o p q" );

        // there is also the VAT account which is ZERO provided accurate!
        // LOST + FOUND papers change that!

        apply_copy_from( "BAL_a", "NFS_TFA_k" ); // Tan Fix Ass
        apply_copy_from( "BAL_b", "NFS_DBT_M" ); // DEBTORS
        apply_sum_of_accts( "BAL_c", "a s" ); // Cash in Bank
        apply_sum( "BAL", "d", "b c" );
        apply_copy_from( "BAL_e", "NFS_CRD_r" ); // Creditors
        apply_sum( "BAL", "f", "d e" );
        apply_sum( "BAL", "g", "a f" );
        apply_copy_neg( "BAL_h", "NFS_SH_t" ); // 100 called up cap shares
        apply_sum( "BAL", "i", "g h" );
        apply_copy_from( "BAL_j", "BAL_g" ); // Shareholders funds

// -----------------------

//	apply_year_base_PL1(); - START

	        // Sales (not Interest);
        apply_copy_from( "PL1_a", "PL2_a" );
        // PL1_b - calc below BACKWARDS ****
        // PL1_c - calc below forwards

        // Interest receivable
        apply_copy_from( "PL1_d", "PL2_b" ); //
        // PL1_e - calc below forwards

        // Interest payable
        apply_calc( "PL1_f", "PL2_q + PL2_r" );
                // this is a STUPID: (it makes PL2_s a chameleon)
                // PL2_t = _q + _r + _s // which makes sense
                // PL1_f = _q + _r // NOT _s // different sense
        // PL1_g - calc below forwards

        // TAX
        apply_copy_from( "PL1_h", "NFS_TAX_d" ); // should be from D ACCTS
        // PL1_i - calc below forwards

        apply_copy_from( "PL1_j", "NFS_DIV_e" ); // should be from D ACCTS
        // PL1_k - calc below forwards

        // last years carried forward
        apply_carry_forward( "PL1_l", "PL1_m" );


        // PL1_b - ADMINISTRATIVE EXPENSES - major category BUT
        // This is stupid tricky thing
        // calculating from PL2_w upwards to get PL1_b
        // PL2_w == NET LOSS/ PROFIT - but slightly different on PL2_s !!
        apply_calc( "PL1_b", "PL2_w - PL1_f - PL1_d - PL1_a" );

        apply_sum( "PL1", "c", "a b" );
        apply_sum( "PL1", "e", "c d" );
        apply_sum( "PL1", "g", "e f" );
        apply_sum( "PL1", "i", "g h" );
        apply_sum( "PL1", "k", "i j" );
        apply_sum( "PL1", "m", "k l" );

//	apply_year_base_PL1(); - END





/*
	DONE:
		CH1.check_year_correct()
*/

	if(1) {
		WARN("saving incomplete file - mid way");
		CH1_dset.save_play();
	}

/*
	SO NOW:
		CH1.rep_pdf_CH( true, 6 )

 func rep_pdf_CH::
	this.dset_CH1.rep_pdf( 'CH', pdf, this.dset_curr, this.dset_prev )
	bool run_pdf_CH( --as-below-- )

	dset is CH1_dset with table "CH1"

	the other two dsets are a bit overkill
	should have already been put into CH1, but not so.
	dset_CURR
	so obtain the full .csv for y1y2.csv and y0y1.csv
	dset_curr provides .table("INFO").rowkey("year_start").valuestr
	dset_curr provides .table("INFO").rowkey("next_year_start").valuestr
	dset_prev provides .table("INFO").rowkey("year_start").valuestr
*/


	// csv_PREV provides year some strings
	buffer1 b_PREV;
	const char * s_prev = str_CH_name( b_PREV, year2, e_csv_PREV );
	obj_hold<data_set> dset_PREV = new data_set();
	if(!dset_PREV) return FAIL_FAILED();
	if(!dset_PREV->file_load( s_prev )) return FAIL_FAILED();

// gdb_invoke(true);

	buffer1 b_CH1_pdf;
	const char * s_CH1_pdf = str_CH_name( b_CH1_pdf, year2, e_CH1_pdf_play );

	// generate the Y1Y2_CH1_draft.pdf from loaded tmp_archive_CH1.csv
	if(!run_pdf_CH(
		CH1_dset.dset,
		s_CH1_pdf,
		dset_CURR,
		dset_PREV
	)) return FAIL_FAILED();

	if(1) {
		WARN("saving incomplete file - mid way");
		CH1_dset.save_play();
	}

/*
	now display pdf files

	NB the CashCat has changed name,
	and is now the PL2 version
	is this wrong?
*/

	if(1) {
	int page = 20; // varies with user sessions - make a static config

	 if(0) {
		buffer1 b1;
		str0 s_PL2_CashCat_pdf;
		s_PL2_CashCat_pdf = str_CH_name( b1, year2, e_PL2_CashCat_pdf );
		page = 1; // varies with user sessions - make a static config
		fork_xpdf_page( s_PL2_CashCat_pdf, page ); 
	 }

	 if(1) {
		page = 7; // varies with user sessions - make a static config
		fork_xpdf_page( s_CH1_pdf, page ); 
		// added bonus - comes up in landscape mode
	 }
	}

	









	

	/*

		1011_CH1_draft.pdf <= from archive ?

	str1 filename_html;
	str1 filename_pdf;

 // return 0;

	dir_name_ext file_named_X( filename_csv );
	file_named_X.ext = "html";
	file_named_X.mk_full_path_name();
	filename_html.set( file_named_X.full_path_name );

	file_named_X.ext = "pdf";
	file_named_X.mk_full_path_name();
	filename_pdf = file_named_X.full_path_name;

	// i THINK its ok to mix HTML and SPOUT -- clue ?
	if(!run_pdf_Cash_Cat( dset1, filename_pdf )) return false;

	int page = 20; // varies with user sessions - make a static config
	page = 1; // varies with user sessions - make a static config
	fork_xpdf_page( filename_pdf, page ); 
	// added bonus - comes up in landscape mode

	if(!run_html_Cash_Cat( dset1, filename_html )) return false;
//	fork_netscape( filename_html );
	*/

	return true;
	/*
	*/
}




/*
	CH1 is a different thing altogether?

	It can take the existing CAT report,
	which is already CH1 sensitive.
	Apply a few category renames, "PL2" sort of thing
	then produce the PL2.csv output report for later

	unfortunately ANY subtree of the CAT view
	must be rounded to nearest pound BEFORE computations (sum of branches)
*/
bool run_report_5_CHaaa( str0 filename_csv, str0 filename_CH1_archive_csv )
{

	INFO("file %s", (STR0) filename_csv );
	/*
		FIRST load the dset and run the basic reports ??
		(why were these not saved?)
		(or maybe run the CAT rename earlier?)
	*/
	obj_hold<data_set> dset1 = new data_set();	
	if( !dset1->file_load( filename_csv ) ) return false;

	// io_csv_accts io1( dset1 );
	// if(!io1.load_file( filename_csv ))
	// 	return false;

if(0) {
	if(!run_report_1( dset1 )) return false;
	if(!run_report_2( dset1 )) return false;
	if(!run_report_3( dset1 )) return false;
	if(!run_report_4( dset1 )) return false;
}

	/*
		extract date from dset
	*/
	help_INFO info( dset1 );
	date_glib year_start;
	if(!info.must_fetch_date_year_start( year_start )) {
		return FAIL_FAILED();
	}

	PASS("year_start %s", (STR0) year_start.tmp_year_mm_dd());
	return FAIL("truncated");
}
