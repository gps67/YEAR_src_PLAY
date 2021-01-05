#include "CH1_Names.h"
#include "dgb.h"
#include "tm_parts.h"
#include "buffer2.h"


using namespace ACCTS;


// local static
bool print_Y1Y2( buffer1 & buf, int y1, int y2 )
{
	return buf.print( "%2.2d%2.2d", y1, y2 );
}

// local static
bool print_YEAR_YY( buffer1 & buf, int year, int yy )
{
	return buf.print( "%d_%2.2d", year, yy );
}

const char * ACCTS:: str_CH_name( buffer1 & buf, int year2, CH_Name_Tag tag )
{
	if(!mk_CH_name( buf, year2, tag ))
		FAIL_FAILED();
	return (STR0) buf;
	
}

bool ACCTS:: mk_CH_name( buffer1 & buf, int year2, CH_Name_Tag tag )
{
	buf.clear();
	buf.print("_del_tmp_error");

	if(year2 < 1900 ) return FAIL("year is %d", year2 );
//	int year2 = _year2;
	int year1 = year2 - 1;
	int yearP = year1 - 1; // year0 is rename yearP Previous
	int yP = yearP % 100;
	int y1 = year1 % 100;
	int y2 = year2 % 100;

	const char * PRIV_sample2 = "../PRIV_sample2";

	str1 YPY1; YPY1.print("%2.2d%2.2d", yP, y1 );
	str1 Y1Y2; Y1Y2.print("%2.2d%2.2d", y1, y2 );

	const char * d1 = "../PRIV_samples/"; // trailing /
	const char * d2 = "../PRIV_samples2/"; // trailing /
	const char * d3 = d1; // when it is correct
	d3 = d2; // whilst testing - no live data

	time_t t_now;
	time( & t_now );
	tm_parts tm_now;
	tm_now.local_from_time( t_now );
	const char * datestr = tm_now.str_year_mm_dd(); 

	const char * dir_dot = ".";
	const char * dir_d1 = d1;
	const char * dir_d2 = d2;
	const char * dir_none = "";
	const char * ext_none = "";
	const char * ext_colname = ext_none;
	const char * ext_csv = ".csv";
	const char * ext_pdf = ".pdf";
	const char * ext_spout = ".ps";
	const char * ext_mm3 = ".mm3";

	const char * dir = ".";
	const char * ext = ext_none;


	buffer1 name;

	switch( tag ) {

	 break; case e_CH1_col_PREV:	// column Y2010_11 PREV
		dir = dir_none;
		name.print( "Y" );
		print_YEAR_YY( name, yearP, y1 );
		ext = ext_colname;
	 break; case e_CH1_col_CURR:	// column Y2011_12 CURR
		dir = dir_none;
		name.print( "Y" );
		print_YEAR_YY( name, year1, y2 );
		ext = ext_colname;

	 break; case e_CH1_archive_csv:	//  tmp_CH1_archive_draft.csv
		dir = dir_d1;
		name.print( "tmp_CH1_archive_draft" );
		ext = ext_csv;
	 break; case e_CH1_archive_csv_play:	//  tmp_CH1_archive_play.csv
		dir = dir_d2;
		name.print( "tmp_CH1_archive_%s_play", datestr );
		ext = ext_csv;

	 break; case e_CH1_pdf:	// 1112_CH1_drft.pdf
		dir = dir_d2;
		print_Y1Y2( name, y1, y2 );
		name.print( "_CH1_draft" );
		ext = ext_pdf;
	 break; case e_CH1_pdf_play:	// 1112_CH1_YEAR_MM_DD_play.pdf
		dir = dir_d2;
		print_Y1Y2( name, y1, y2 );
		name.print( "_CH1_draft" );
		name.print( "_%s_play", datestr );
		ext = ext_pdf;

	 break; case e_csv_CURR_PL2:	// 1112_PL2.csv
		dir = dir_d2;
		print_Y1Y2( name, y1, y2 );
		name.print( "_PL2" );
		ext = ext_csv;
	 break; case e_mm3_PREV:	// 1011.mm3
		dir = dir_d1;
		print_Y1Y2( name, yP, y1 );
		ext = ext_mm3;
	 break; case e_mm3_CURR:	// 1112.mm3
		dir = dir_d1;
		print_Y1Y2( name, y1, y2 );
		ext = ext_mm3;
	 break; case e_csv_PREV:	// 1011.csv
		dir = dir_d1;
		print_Y1Y2( name, yP, y1 );
		ext = ext_csv;
	 break; case e_csv_CURR:	// 1112.csv
		dir = dir_d1;
		print_Y1Y2( name, y1, y2 );
		ext = ext_csv;

	 break; case e_PL2_CashCat_pdf: // 1112_PL2_CashCat.csv
		dir = dir_d2;
		print_Y1Y2( name, y1, y2 );
		name.print("_PL2_CashCat");
		ext = ext_pdf;
	 break; case e_PL2_VatTrack_pdf:	// 1112_PL2_VatTrack.csv
		dir = dir_d2;
		print_Y1Y2( name, y1, y2 );
		name.print("_PL2_VatTrack");
		ext = ext_pdf;

	break; case e_csv_CAT_adj:	// PRIV_samples/CH_adj_pl2.csv
		dir = dir_d1;
		name.print("CH_adj_pl2" );
		ext = ext_csv;

	 break; default:
		FAIL( "tag not found" );

	};

	// omit malloc check - unlikely when its my own strings
	buf.clear();
	buf.print( dir );
	buf.print( name );
	buf.print( ext );

	return PASS("year2 %d tag %d name %s", year2, (int) tag, (STR0) buf );
  }


bool ACCTS:: CH_Name_Tag_show_test( int year2 )
{
	CH_Name_Tag tag;
	tag = e_zero;
	buffer1 buf;
	while(true) {
		tag = (CH_Name_Tag) ((int)tag+1 ); // succ / incr
		if( tag == e_last ) break; // rename zero rename last // stop
		if(!mk_CH_name( buf, year2, tag ))
			return FAIL_FAILED();
	// INFO("year2 = %d tag = %d name = %s", year2, (int) tag, (STR0) buf );

	}
	return PASS("DONE");
}
	

