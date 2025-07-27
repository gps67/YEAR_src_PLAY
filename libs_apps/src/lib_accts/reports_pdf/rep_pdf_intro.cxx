
// report1.cxx does this - _pdf doesnt (except here - lots of bitty things)
#include "rpt_help_tbl.h"

// get some common code (TODO)
#include "rep_pdf_0.h"

#include "rep_pdf_Reports.h"

#define as_landscape true

using namespace ACCTS; // defining its contents

/*!
	set is_hidden for each named column spec of the VatSum table

	I dont have ANY euro-vat transactions, so this simplifies the table
	which enables it to be printed portrait, not landscape
*/
void rep_pdf_0::show_EUROVAT( bool show ) // too wide for summary
{
	table * tbl = dset->get_table( STR0_VatSum );
	if(!tbl) return;
	col_spec * spec;
	const char * field_names[] = {
		"pre-vat_EURO_BUY-INP",
		"amnt-vat_EURO_BUY-INP",
		"pre-vat_EURO_SELL-OUT",
		"amnt-vat_EURO_SELL-OUT"
	};
	int n = 4;
	for( int i=0; i<n; i++ )
	{
		spec = tbl->get_col_spec( field_names[i] );
		if( !spec )
		{
			e_print("MISSING EUROVAT field name %s\n", field_names[i] );
			continue;
		}
		spec->is_hidden = !show;
	}
}

/*!
	print  intro section:
		dset.INFO.title		Information Cascade 2002-2003
		dset.INFO.year_start	2002-02-01
		dset.ACCT[*] {.key .amnt } start balences
		dset.VAT[*] { .rate .desc }
*/
bool rep_pdf_Cash_Cat::intro()
{
	pdf_pen_text * pen;
	// pdf_pen_draw * draw;

	buffer2 buff;
	// flt80 x;
	// flt80 w;

// INFO is now a macro in dgb.h

	help_INFO info( dset );

	pen = pen_titles;
	flt80 h = pen->get_line_height_v();
	// y_pos += h*2;

	str0 title = "Default Title: Some ACCTS";
	if(info.fetch_row( STR0_title ))
	{
		title = str0( info._value );
	}

	str0 year_start_str = "1999-12-31"; // looks suspect ?
	if(info.fetch_row( STR0_year_start ))
	{
		year_start_str = info._value;
	}
	date_glib year_start;
	year_start.set_year_mm_dd( year_start_str );

#if 1
	str0 next_year_start_str = "1999-12-31"; // looks suspect ?
	if(info.fetch_row( STR0_next_year_start ))
	{
		next_year_start_str = info._value;
	}
	date_glib next_year_start;
	next_year_start.set_year_mm_dd( next_year_start_str );
#else
	date_glib next_year_start;
	int year, mm, dd;
	year_start.get_year_mm_dd( year, mm, dd );
	next_year_start.set_year_mm_dd( year+1, mm, dd );
#endif

	date_glib year_end;
	year_end = next_year_start;
	year_end.add_days( -1 );

	y_pos += h;
	pen->xy_mid( 0.5, y_pos, title );
	y_pos += h;
	y_pos += h;

	buff.clear();
	buff.print("Year Start: %s", year_start.tmp_year_mm_dd() );
	pen->xy_mid( 0.5, y_pos, buff );
	y_pos += h;

	buff.clear();
	buff.print("Year End: %s", year_end.tmp_year_mm_dd() );
	pen->xy_mid( 0.5, y_pos, buff );
	y_pos += h;

	draw_table( "VAT", "key", "", false );
	y_pos += h;

 if(1)
 {
	// flat (because its not picking up that 'key's table has group)
	// but 'key' field is not centralised
	// thats auto-logic for you , manual cobol programs are manually edited
	draw_table( "ACCT", "key", "amnt_closing", false );
 } else {
	draw_table(
		"ACCT",		// table name
		// "key",		// key name
		// "dotdot",		// key name
		// NULL,			// key name
		"group",			// key name
		"amnt_closing",	// summing total
		// true,			// landscape
		as_landscape,
		// false,	// portrait,
		true,	// full_key_path
		-1,	// interior_depth - zero means infinite
		// true puts the GREEN node at the top
		// true shows totals - without
		true, // false, // true,	// interleave
		// false, // true,	// interleave
		true	// show leaves
			// generated table "SUM_cat"
	);
 }

	/*
		show the 4 quarters VAT SUMMARY
		hide the EURO columns here, but leave them un-hidden
	*/
	show_EUROVAT( false );
	draw_table(
		STR0_VatSum,		// display table "VatSum" (prepared)
		STR0_date_tax,		// split/regroup by col "date_tax"
		STR0_turnover,		// TOTAL = sum of col "turnover"
		as_landscape,	// 
		false,	// full_key_path
		1,	// interior_depth - top plus single
		true,	// interleave
		false	// show leaves
	);
	show_EUROVAT( true );

	/*
		show the Categorised SUMMARY
	*/
	draw_table(
		STR0_Categorised,
		STR0_cat,
		STR0_turnover,		//	total per node
		// true,			// landscape
		// false,	// portrait,
		as_landscape,	// 
		true,	// full_key_path
		0,	// interior_depth - single
		false,	// true,	// interleave
		false,	// show leaves
		STR0_SUM_cat		// generated table "SUM_cat"
	);

	end_page();
	return true;
}
