
#include "rep_pdf_Reports.h"

using namespace PDF_GEN;
using namespace ACCTS;

// virtual
bool rep_pdf_Cash_Cat::run_wrapped_report()
{
	str0 title = "Multi-Part document";
	bool landscape = true;
	bool portrait = false;
	open_up( filenamepdf, title, portrait );
	// mydoc->create_pens();

	// title page
	intro();

	if(0) draw_table(
		STR0_VatRep,
		STR0_date_tax,
		"",
		portrait,
		false,	// full_key_path
		-1,	// interior_depth
		true,	// interleave
		true	// show leaves
	);
	end_page(); // Categorised intro can overflow

// summary
	draw_table(
		STR0_CashBook,
		STR0_date_pay,
		STR0_turnover,		//	 "amnt",
		 landscape,
		false,	// full_key_path
		4,	//	2,	// interior_depth // ERA/YEAR/QN/MONTH
		false,	//	true,	// interleave
		false	//	false	// show leaves
	);
	end_page(); // looks better

	draw_table(
		STR0_CashBook,
		STR0_date_pay,
		STR0_turnover,		//	 "amnt",
		 landscape,
		false,	// full_key_path
		-1,	// interior_depth
		false,	// interleave
		true	// show leaves
	);

	end_page(); // Categorised header was orphaned
	draw_table(
		STR0_Categorised,
		STR0_cat,
		STR0_turnover,		//	"amnt_pre_vat",
		landscape, // portrait,
		true,	// full_key_path
		1000,	// interior_depth
		true,	// interleave
		true	// show leaves
		// DONE // "SUM_" STR0_cat		// generated table
	);

	end_page(); // SUM_cat on own page

	draw_table(
		STR0_SUM_cat,		// generated table
		"key",
		"",		//	"amnt_pre_vat",
		landscape, // portrait,
		true,	// full_key_path
		1000,	// interior_depth
		true,	// interleave
		true,	// show leaves
		(STR0) NULL
	);

	close_file();	// close PDF document
	return true;
}

// virtual
bool rep_pdf_VatTrack::run_wrapped_report()
{
	str0 title = "Vat Track";
	bool landscape = true;
//	bool portrait = false; // not landscape;
	open_up( filenamepdf, title,  landscape );
	// mydoc->create_pens();

	// title page
	// intro(); // no intro for Vat Track except ...


	pdf_pen_text * pen = pen_titles;
	// flt80 h = 
		pen->get_line_height_v();
	mid_title_gap(3);
	mid_title( "Vat Tracking Report" );
	mid_title( "(some items may have moved date)" );
	mid_title_gap(1);
	mid_title( "NB: H1 hardware/assets have zero turnover value" );
	mid_title( "but should have correct pre/vat values" );

	// dset.VatSum.img.is_hidden = true
	table * tbl_VatSum = dset->get_table( STR0_VatSum );
	if(!tbl_VatSum ) { return FALSE_dgb_fail("NULL TABLE"); }
	col_spec * spec_img = tbl_VatSum->get_col_spec( STR0_img );
	// if( ! spec_img ) { return FALSE_dgb_fail("NULL SPEC_img"); }
	if( spec_img ) spec_img -> is_hidden = true;

	draw_table(
		STR0_VatSum,		// table "VatSum"
		STR0_date_tax,		// group/split by col "date_tax"
		STR0_turnover,		// bottom line col "turnover"
		// THIS ADDED THE REMAINS OF THE OTHERS!! // "",
		landscape,
		false,	// full_key_path
		-1,	// interior_depth
		true,	// interleave
		true	// show leaves
	);
	// end_page(); // Categorised intro can overflow

	close_file();	// close PDF document
	return true;
}


// virtual
bool rep_pdf_table::run_wrapped_report()
{
	// bool use_gui = true;
	// use_gui = false;
	// gdb_invoke( use_gui );

	open_up( filenamepdf, title->str,  landscape );

	str0 treecolname; // NULL
	treecolname = "key";
	if( keyfield )
		treecolname = keyfield->str;

	str0 totcol = "";
	if( totcol_name ) totcol = * totcol_name;

	draw_table(
		tablename->str,		// lookup of table in dset and title
		treecolname,
		totcol,
		landscape,		// false
		full_key_path, 		// false
		interior_depth,		// -1
		true,	// interleave		// true
		true	// show leaves		// true
	);

	// end_page(); // Categorised intro can overflow
	close_file();	// close PDF document
	return true;
}


rep_pdf_table::rep_pdf_table(
	pdf_ctxt & _pdf,
	data_set * _dset,
	table * _tbl,
	str0 _filenamepdf
)
: rep_pdf_0(
	_pdf,
	_dset,
	_filenamepdf
)
, tablename()
, title()
, keyfield()
, full_key_path( false )
, interior_depth( -1 )
{
	// used for debug - so
	landscape = true;
	tablename = _tbl->name;
	title = _tbl->name;

if(1)
	if( * _tbl->name == "WEEK" ) {
		int nc = _tbl->col_specs.N();
		// actually, i think is_summable is already auto set
		for( int i=0; i<nc; i++ )
		{
			col_spec * spec = _tbl->col_specs[i];
			switch( spec->get_field_type()) {
			 case T_amnt:
				spec->is_summable = true;
			 break;
			 default:
				;
			}
		}
		col_spec * spec = _tbl->get_col_spec("wk_gross");
		if( spec ) totcol_name = spec->name;
	}
	/*
		the default totcol is the sum of the visible sums
		which is often duplication (eg tot,vat,pre, = 2x )
		so look for a plausible thing to report
		(Then maybe opt for first/last amnt col?)

		The name is converted to a spec (or NULL) then to a name.
		That is stored and later used in the call to draw_table
		(which can also be used for scripts to provide the name)
		which converts the name to a SPEC. Obvious really.
	*/
	const char * names[] = {
		"turnover",	// current favourite
		"amnt",
		"amount",
		"amnt_closing",	// this isnt used, but is fail-safe added
		"gross",
		"pre_tax",
		"wk_gross",
	};
	int n_names = sizeof( names ) / sizeof(names[0]);
	for( int i=0; i<n_names; i++ )
	{
		if( totcol_name ) break;
		col_spec * spec = _tbl->get_col_spec( names[i] );
		if( spec ) totcol_name = spec->name;

	}
}
