
// rep_pdf_CH.cxx // Companies House

// A BIT LIKE: rep_pdf_Reports.cxx

/*
	http://www.frc.org.uk/asb/press/pub0785.html
	http://www.frc.org.uk/asb/technical/frsse.cfm  # free
*/


// #include "rep_pdf_Reports.h"
#include "rep_pdf_CH.h"
#include "dgb_fail.h"
// DONE // #include "date_glib.h"

using namespace PDF_GEN;
using namespace ACCTS;

/*!
	factory to create a report
	without caller knowing the TYPE rep_pdf_CH

	In fact rep_pdf_CH.h could be absorbed into this file ;-)
*/
rep_pdf_0 *
new_rep_pdf_CH(
	pdf_ctxt & _pdf,
	data_set * _dset,
	str0 filenamepdf,
	data_set * _dset_curr,
	data_set * _dset_prev
)
{
	return new rep_pdf_CH(
		_pdf, _dset, filenamepdf, _dset_curr, _dset_prev );
}

rep_pdf_CH::
~rep_pdf_CH()
{
}

rep_pdf_CH::
rep_pdf_CH(
	pdf_ctxt & _pdf,
	data_set * _dset,
	str0 filenamepdf,
	data_set * _dset_curr,
	data_set * _dset_prev
)
: rep_pdf_0(
	_pdf,
	_dset,
	filenamepdf
)
{
	dset_curr = _dset_curr ;
	dset_prev = _dset_prev ;
	/*
		openup calls create_pens (in subclass) and init_here
	*/
}

str2 * rep_pdf_CH::INFO_attr( str0 attrname )
{
	static obj_hold<str2> Empty = new str2( "_" );
	row_inst * row = INFO_tbl -> find_row( attrname );
	if(!row) {
		e_print( "INFO_attr( %s ) -- no such row\n", (STR0) attrname );
		return Empty;
	}
	str2 * val = INFO_val_spec->get_str2_for_display( row );
	if(!val) {
		e_print( "INFO_attr( %s ) -- NULL VALUE!\n", (STR0) attrname );
		return Empty;
	}
	return val;
}

bool rep_pdf_CH::init()
{
	/*
		init 0 - NULL or not random (font not yet available)
	*/
	h_normal = 0;
	h_bold   = 0;
	h_titles = 0;
	h = h_normal;
	pen = NULL;

	/*
		to obtain attributes from main dset (CH1_INFO)
	*/
	INFO_tbl = dset -> get_table( "INFO" );
	if(!INFO_tbl) return FALSE_dgb_fail( "NULL INFO_tbl");

	INFO_val_spec = INFO_tbl -> get_col_spec( "val" );
	if(!INFO_val_spec) return FALSE_dgb_fail( "NULL INFO_val_spec");

	/*
		get some values from the OTHER dsets (0304.csv)

		curr	year_tag	
		prev	-ditto-
		curr	year_start --> (curr)_year_end
		prev	-ditto-
	*/

	str2 * curr_info_year_tag = dset_curr -> get_table_row_col(
		"INFO", "year_tag", "value" );
	if(! curr_info_year_tag ) return FALSE_dgb_fail("NULL curr_info_year_tag");

	str2 * prev_info_year_tag = dset_prev -> get_table_row_col(
		"INFO", "year_tag", "value" );
	if(! prev_info_year_tag ) return FALSE_dgb_fail("NULL prev_info_year_tag");

	str2 * curr_info_year_start = dset_curr -> get_table_row_col(
		"INFO", "year_start", "value" );
	if(! curr_info_year_start ) return FALSE_dgb_fail("NULL curr_info_year_start");

	str2 * curr_info_next_year_start = dset_curr -> get_table_row_col(
		"INFO", "next_year_start", "value" );
	if(! curr_info_next_year_start ) return FALSE_dgb_fail("NULL curr_info_next_year_start");

	date_glib dat;
	int year, mm, dd;

	/*
		LURK - a year is not a year if its the first year or 
		when the A.R.D. is changed, so the calling data
		should specify the year end to the report, !

		methinks it changed later with the data fields added to X
		eg CSVY1Y2.INFO.next_year_start.value "YEAR-MM-DD"
	*/

	dat.set_year_mm_dd( * curr_info_next_year_start );
	dat.add_days( -1 );
        curr_year_end.set( dat.tmp_year_mm_dd() );

	// should be given ARD, not provide it, but set it here 
	// used to change report for different years
	ARD = dat;

	// 31 Jan 2001
	buff.clear();
	dat.get_year_mm_dd( year, mm, dd );
	buff.print( "%d %s %d", dd, (STR0) dat.get_Jan(), year ); 
        curr_year_end_long.set( buff );

	str2 * prev_info_year_start = dset_prev -> get_table_row_col(
		"INFO", "year_start", "value" );
	if(! prev_info_year_start ) return FALSE_dgb_fail("NULL prev_info_year_start");

/*
	dat.set_year_mm_dd( * prev_info_year_start );
	dat.get_year_mm_dd( year, mm, dd );
	dat.set_year_mm_dd( year+1, mm, dd );
	dat.add_days( -1 );
        prev_year_end.set( dat.tmp_year_mm_dd() );
*/

	dat.set_year_mm_dd( * curr_info_year_start );
	dat.add_days( -1 );
        prev_year_end.set( dat.tmp_year_mm_dd() );

	/*
		used often
	*/
	company_name.set( INFO_attr( "company_name" ));
	if( company_name ) {
		PASS( "company_name %s", (STR0) company_name );
	}
	if(! company_name ) {
		FAIL( "company_name NULL" );
		gdb_invoke(true);
	}

	/*
		CH1 contains most of the numeric data
	*/
	CH1_tbl = dset -> get_table( "CH1" );
	if(!CH1_tbl) return FALSE_dgb_fail( "NULL CH1_tbl");

	// the 0304.csv INFO table gave us "Y2003_04", its a column in CH1
//	curr_spec = (col_spec_amnt *) CH1_tbl->get_col_spec( curr_info_year_tag );
//	prev_spec = (col_spec_amnt *) CH1_tbl->get_col_spec( prev_info_year_tag );
	/*
		dynamic_cast needs RTTI and returns NULL if bad cast
	*/
	curr_spec = dynamic_cast<col_spec_amnt *>(
		CH1_tbl->get_col_spec( curr_info_year_tag )
	);
	prev_spec = dynamic_cast<col_spec_amnt *>(
		CH1_tbl->get_col_spec( prev_info_year_tag )
	);	
	desc_spec =                   CH1_tbl->get_col_spec( "desc" );
	io_spec   =                   CH1_tbl->get_col_spec( "IO" );
	lr_spec   =                   CH1_tbl->get_col_spec( "LR" );
	if(!curr_spec) return FALSE_dgb_fail( "NULL curr_spec" );
	if(!prev_spec) return FALSE_dgb_fail( "NULL prev_spec" );
	if(!desc_spec) return FALSE_dgb_fail( "NULL desc_spec" );
	if(!io_spec) return FALSE_dgb_fail( "NULL io_spec" );
	if(!lr_spec) return FALSE_dgb_fail( "NULL lr_spec" );

	/*
		x layout
	*/
//	x1 = 0.05;
//	x2 = 0.60;
//	x3 = 0.65;
//	x4 = 0.70;
//	x5 = 0.75;
//	x6 = 0.80;
//	x7 = 0.85;
//	x8 = 0.90;
//	x9 = 0.95;
	x1 = 0.55;

	/*
		TAB_ruler

		[  [  [  [  [
	LPH;	---Page Header
		[  [  [  [  [
	L0	------Outdented Title UNUSED
		[  [  [  [  [
	L1	---------Title and Text			
		[  [  [  [  [
	L2	------------Indented Text
		[  [  [  [  [
	L3	---------------Sub Indented Text
		[  [  [  [  [

	*/

#define PAGE_LEFT_X 0.05	// left minimum border - used in header
#define MAIN_LEFT_X 0.15	// left NORMAL border (when plenty of space)
#define PAGE_RIGHT_X 0.95	// right maximum border - used in header

// contents page
#define CON_C1_X 0.25		// balanced at quarters
#define CON_C2_X 0.75

// Company Information
#define COMP_INFO_C1_X 0.25
#define COMP_INFO_C2_X 0.55

// Directors Report
#define DIR_C1_X MAIN_LEFT_X // indented
#define DIR_C2_X 0.75
#define DIR_C3_X 0.85

#define PL1_LEFT_X  MAIN_LEFT_X
#define PL1_NOTES_X 0.60
#define PL1_CURR_X  0.75
#define PL1_PREV_X  0.85

// Balance is wider
#define BAL_LEFT_X   0.10
#define BAL_NOTES_X  0.56
#define BAL_CURR_L_X 0.65
#define BAL_CURR_R_X 0.75
#define BAL_PREV_L_X 0.85
#define BAL_PREV_R_X 0.95

#define NFS_LEFT_X PL1_LEFT_X
#define NFS_CURR_X PL1_CURR_X
#define NFS_PREV_X PL1_PREV_X

#define PL2_LEFT_X   BAL_LEFT_X
#define PL2_CURR_L_X BAL_CURR_L_X
#define PL2_CURR_R_X BAL_CURR_R_X
#define PL2_PREV_L_X BAL_PREV_L_X
#define PL2_PREV_R_X BAL_PREV_R_X

	/*
		layout adjustments
NULL pen_normal
	*/
	w_par = pen_normal->calc_string_width( ")" );

	/*
		initial TAB rile
	*/
	x1 = MAIN_LEFT_X;

	return true;
}

bool rep_pdf_CH::run_wrapped_report()
{
	/*
		openup needs pdftitle - which is company name !!!
		init() needs open_up() to create pens
		company_name is set by init()
	*/

	// gdb_invoke(1);
	// bool landscape = true;
	bool portrait = false;
	buff.clear();
#if 0
	buff.print( "Companies House Report - %s - upto - %s",
		(STR0) company_name,
		(STR0) curr_year_end
	);
#else
	buff.print( "Companies House Report" );
#endif
	str0 pdftitle = (str0) buff;
	// done by openup // create_pens();
	open_up( filenamepdf, pdftitle, portrait );
	if(!init())
		THROW_dgb_fail("rep_pdf_CH::init() failed");

	pen_bold = new pdf_pen_text( this );
	pen_bold ->init_font( "Times-Bold", 12 );
	h_normal = pen_normal -> get_line_height_v() * 1.2;
	h_bold   = pen_bold   -> get_line_height_v() * 1.2;
	h_titles = pen_titles -> get_line_height_v() * 1.2;
	h = h_normal;
	pen = pen_normal;

if(1)
{
	title_page();
	contents_page();
	company_information_page();
	report_of_the_director_page();
	profit_and_loss_page();
}

	balance_sheet_page();
	notes_to_the_fs_pages();
	report_of_the_accountants_page();
	profit_and_loss_account_page();

	end_page();
	close_file();	// close PDF document
	return true;
}

void rep_pdf_CH::set_pen_normal()
{
	pen = pen_normal;
	h = h_normal;
}

void rep_pdf_CH::set_pen_titles()
{
	pen = pen_titles;
	h = h_titles;
}

void rep_pdf_CH::set_pen_bold()
{
	pen = pen_bold;
	h = h_bold;
}

void rep_pdf_CH::y_down_line()
{
	y_pos += h;
}

void rep_pdf_CH::y_down_lines( int lines )
{
	y_pos += h * lines;
}

void rep_pdf_CH::page_header( str0 page_name )
{
	now_need_page();
	flt80 t = y_pos;
	y_pos = 0;
	y_down_line(2);

	set_pen_normal();
	pen->xy_right( PAGE_RIGHT_X, y_pos, (STR0) company_name );
	y_down_line();

	buffer2 buf; // buff might be passed as a parameter to this function!
	buf.clear();
	buf.print( "For the year ended %s", (STR0) curr_year_end_long );
	pen->xy_right( PAGE_RIGHT_X, y_pos, (str0) buf );
	y_down_line();
	flt80 t2 = y_pos;

	pen->xy_right( PAGE_RIGHT_X, y_pos, (STR0) page_name );
	y_down_lines(2);

	set_pen_titles();
	y_down_line();
	pen->xy_left( PAGE_LEFT_X, t2, page_name );

	set_pen_normal();
	y_pos = t;
}

void rep_pdf_CH::page_footer( str0 notice, str0 page_number )
{
	y_pos = 0.95;
	set_pen_normal();
	pen->xy_mid( 0.5, y_pos, notice );
	y_down_line();
	pen->xy_mid( 0.5, y_pos, page_number );
	end_page();
}

bool rep_pdf_CH::title_page()
{
	buff.clear();
	buff.print("Comp: %s", (STR0) * INFO_attr("registered_number") );
	page_header( buff );
	set_pen_normal();
	y_down_lines(9);

	set_pen_titles();
	y_down_line();
	pen_titles->xy_mid( 0.5, y_pos, (STR0) company_name );
	y_down_line(2);

	set_pen_normal();
	pen->xy_mid( 0.5, y_pos, "Report of the Director" );
	y_down_line();
	pen->xy_mid( 0.5, y_pos, "and" );
	y_down_line();
	pen->xy_mid( 0.5, y_pos, "Financial Statements" );
	y_down_line();
	pen->xy_mid( 0.5, y_pos, "for the Year that ended" );
	y_down_line(4);

	set_pen_titles();
	buff.clear();
	buff.print( "%s", (STR0) curr_year_end );
	pen->xy_mid( 0.5, y_pos, buff );
	y_down_line();
	set_pen_normal();
	y_down_line();

	// end_page();
//	page_footer( "", "" );
	return true;
}

bool rep_pdf_CH::contents_page()
{
	flt80 xa = CON_C1_X;
	flt80 xb = CON_C2_X;

//	page_header( "Contents" );
//	y_down_lines(4);

	set_pen_normal();
	y_down_lines(6);

	y_down_lines(4);
	y_down_lines(6);
	pen_titles->xy_left( xa, y_pos, "Table Of Contents" );
	pen_titles->xy_mid( xb, y_pos, "Page" );
	y_down_lines(3);

	pen->xy_left( xa, y_pos, "Company Information" );
	pen->xy_left( xb, y_pos, "1" );
	y_down_lines(2);

	pen->xy_left( xa, y_pos, "Report of the Director" );
	pen->xy_left( xb, y_pos, "2" );
	y_down_lines(2);

	pen->xy_left( xa, y_pos, "Profit and Loss Account" );
	pen->xy_left( xb, y_pos, "3" );
	y_down_lines(2);

	pen->xy_left( xa, y_pos, "Balance Sheet" );
	pen->xy_left( xb, y_pos, "4" );
	y_down_lines(2);

	pen->xy_left( xa, y_pos, "Notes to the Financial Statements" );
	pen->xy_left( xb, y_pos, "5" );
	y_down_lines(2);

	pen->xy_left( xa, y_pos, "Report of the Accountants" );
	pen->xy_left( xb, y_pos, "7" );
	y_down_lines(2);

	pen->xy_left( xa, y_pos, "Profit and Loss Account" );
	pen->xy_left( xb, y_pos, "8" );
	y_down_lines(2);

	page_footer( "", "" );

	// end_page();
	return true;
}

bool rep_pdf_CH::company_information_page()
{
	page_header( "Company Information" );

	flt80 xa = COMP_INFO_C1_X;
	flt80 xb = COMP_INFO_C2_X;

	set_pen_normal();
	y_down_lines(5);

	pen_titles->xy_left( xa, y_pos, "Director" );
	pen_normal->xy_left( xb, y_pos, INFO_attr( "director") );
	y_down_lines(3);

	pen_titles->xy_left( xa, y_pos, "Secretary" );
	pen_normal->xy_left( xb, y_pos, INFO_attr( "secretary") );
	y_down_lines(3);

	pen_titles->xy_left( xa, y_pos, "Registered Office" );
	pen_normal->xy_left( xb, y_pos, INFO_attr( "reg_off_addr_1") );
	y_down_line();
	pen_normal->xy_left( xb, y_pos, INFO_attr( "reg_off_addr_2") );
	y_down_line();
	pen_normal->xy_left( xb, y_pos, INFO_attr( "reg_off_addr_3") );
	y_down_line();
	pen_normal->xy_left( xb, y_pos, INFO_attr( "reg_off_addr_4") );
	y_down_line();
	pen_normal->xy_left( xb, y_pos, INFO_attr( "reg_off_addr_5") );
	y_down_lines(3);

	pen_titles->xy_left( xa, y_pos, "Registered number" );
	pen_normal->xy_left( xb, y_pos, INFO_attr( "registered_number") );
	y_down_line();
	pen_normal->xy_left( xb, y_pos, "(England and Wales)" );
	y_down_lines(3);

	pen_titles->xy_left( xa, y_pos, "Accountants" );
	pen_normal->xy_left( xb, y_pos, INFO_attr( "acct_addr_1") );
	y_down_line();
	pen_normal->xy_left( xb, y_pos, INFO_attr( "acct_addr_2") );
	y_down_line();
	pen_normal->xy_left( xb, y_pos, INFO_attr( "acct_addr_3") );
	y_down_line();
	pen_normal->xy_left( xb, y_pos, INFO_attr( "acct_addr_4") );
	y_down_line();
	pen_normal->xy_left( xb, y_pos, INFO_attr( "acct_addr_5") );
	y_down_lines(3);

	page_footer( "", "Page 1" );

	return true;
}



bool rep_pdf_CH::report_of_the_director_page()
{
	flt80 xa = DIR_C1_X; // indented
	flt80 xb = DIR_C2_X;
	flt80 xc = DIR_C3_X;
	/*
		very specific 
		maybe I _SHOULD_ have gone for the
		template.rtf with sed script written by prog
	*/
	page_header( "Report of the Director" );
	set_pen_normal();

	y_down_lines(3);

	buff.clear();
	buff.print( "%s %s",
		"The director presents his report with the financial statements for the year ended",
		(STR0) curr_year_end_long
	);
	pen->xy_left( xa, y_pos, (str0) buff );
	y_down_line(4);

	set_pen_bold();
	pen->xy_left( xa, y_pos, "PRINCIPAL ACTIVITY" );
	y_down_line(2);

	set_pen_normal();
	pen->xy_left( xa, y_pos, "The principal activity of the company in the year under review was that of computer consultancy." );
	y_down_line(4);

	set_pen_bold();
	pen->xy_left( xa, y_pos, "DIRECTOR" );
	y_down_line(2);

	set_pen_normal();
	buff.clear();
	buff.print( "%s %s",
		(STR0) *INFO_attr("director"),
		"was the sole director during the year under review."
	);
	pen->xy_left( xa, y_pos, (str0) buff );
	y_down_line();
	pen->xy_left( xa, y_pos, "His beneficial interest in the issued cshare capital of the company was as follows:" );
	y_down_line();
	y_down_line();

	set_pen_bold();
	pen->xy_mid( xb, y_pos, curr_year_end );
	pen->xy_mid( xc, y_pos, prev_year_end );
	y_down_line(2);

	set_pen_normal();
	pen->xy_left( xa, y_pos, "Ordinary \2431 shares" );
// £1
	pen->xy_mid( xb, y_pos, "100" );
	pen->xy_mid( xc, y_pos, "100" );
	y_down_line(4);

	pen->xy_left( xa, y_pos, "This report has been prepared in accordance with the special provisions" );
	y_down_line();

	pen->xy_left( xa, y_pos, "of Part VII of the Companies Act 1985 relating to small companies." );
	y_down_line(4);

	set_pen_bold();
	pen->xy_left( xa, y_pos, "ON BEHALF OF THE BOARD:" );
	y_down_line(5);

	set_pen_normal();
	pen->xy_left( xa, y_pos, "....................................." );
	y_down_line();

	buff.clear();
	buff.print( "%s - DIRECTOR", (STR0) * INFO_attr("director") );
	pen->xy_left( xa, y_pos, (str0) buff );
	y_down_line();
	y_down_line();

	set_pen_normal();
	pen->xy_left( xa, y_pos, "Dated:" );
	y_down_line();

	page_footer( "", "Page 2" );

	return true;
}

/*
	round the amount to the nearest pound
	print with rounded parentheses if negative
	and with comma thousands

	round up - or round down ??

	My reports round to NEAREST not downwards (amnt_t.h)
	which covers small arith errors, and suits me best,
	but if you want rounding DOWN, you should change it.
	Maybe reintroduce a 2p margin of error ?
	see amnt.get_nocurr_nosign_nopence_str( buff )

	round_down - I think .. at least towards zero
	still no good, as the running totals need to be correct
	so it is the INPUTs that need to be rounded down
	at least when they are used in the report, they get rounded

	YES this causes a drift away from reality (book != bank)
	SO - look for inputs and something ?
*/
void rep_pdf_CH::print_amnt_1( flt80 x_right, amnt_t &amnt, int overlines, int underlines )
{
	buff.clear();
	// amnt.round_down(); // else nearest
	amnt.round_down(); // 2013 - its down towards zero, or further ?
	amnt.get_nocurr_nosign_nopence_str( buff );

	// flt80 w_par = pen->calc_string_width( ")" );
	flt80 w1 = pen->calc_string_width( buff );
	flt80 xr = x_right - w_par;
	if( amnt.is_negative() )
	{
		flt80 xl = xr - w1;
		pen->xy_right( xl, y_pos, "(" ); 
		pen->xy_right( xr, y_pos, buff ); 
		pen->xy_left( xr, y_pos, ")" ); 
	} else {
		pen->xy_right( xr, y_pos, buff ); 
	}

	flt80 w = pen_normal ->calc_string_width( "99,999" );
	flt80 y1 = y_pos + (h * -1.2 );
	flt80 y2 = y_pos + (h * -1.0 );
	flt80 y3 = y_pos + (h *  0.3 );
	flt80 y4 = y_pos + (h *  0.5 );
	flt80 xb = x_right - w_par;
	flt80 xa = xb - w;
	// flt80 XM = (xa + xb) / 2;
	if(  overlines > 1 ) line_borders -> line( xa, y1, xb, y1 );
	if(  overlines > 0 ) line_borders -> line( xa, y2, xb, y2 );
	if( underlines > 0 ) line_borders -> line( xa, y3, xb, y3 );
	if( underlines > 1 ) line_borders -> line( xa, y4, xb, y4 );
	
}

void rep_pdf_CH::y_space_for_overlines( int overlines )
{
	/*
		the typesetting approach SHOULD be:

			start_line
				change font
				change super/subscript
				use overlines
				use underlines
			endline (flush buffered multiplart line)
	*/
	switch( overlines ) {
	 case 0:
	 break;
	 case 1:
		y_pos += h * 0.3;
	 break;
	 case 2:
		y_pos += h * 0.5;
	 break;
	 default:
		THROW_dgb_fail("case overlines");
	}
}

void rep_pdf_CH::y_space_for_underlines( int underlines )
{
	switch( underlines ) {
	 case 0:
	 break;
	 case 1:
		y_pos += h * 0.3;
	 break;
	 case 2:
		y_pos += h * 0.5;
	 break;
	 default:
		THROW_dgb_fail("case underlines");
	}
}


bool rep_pdf_CH::get_row( str0 key )
{
	if(1) { // -- SHOW MARGIN NOTES -- key MARKS SCRATCHES LBL
		// GLOBALLY EFFECTIVE
		// put margin notes on each value printed
		// works for all the wrong reasons
		// but looks good
		// except for A4 clipping on printed paper
		// and scratchy effect
		// margin notes - or debug feature ?
		// YES THAT IS HERE!
		pen_titles_small->xy_left( 0 , y_pos, key );
	}

	curr_amnt.set_zero();
	prev_amnt.set_zero();
	desc = NULL;

	row_inst * row = CH1_tbl -> find_row( key );
	if(!row) return FALSE_dgb_fail( key );

	desc = desc_spec -> get_str2_for_display( row );
	io_is_normal = true;
	lr_is_l = true;

	str2 * s2;
	s2  = io_spec -> get_str2_for_display( row );
	if( * s2 == "O" ) io_is_normal = false;
	else if( *s2 != "I" ) return FALSE_dgb_fail( "IO should be I or O " );
 
	s2  = lr_spec -> get_str2_for_display( row );
	if( * s2 == "R" ) lr_is_l = false;
	else if( *s2 != "L" ) return FALSE_dgb_fail( "LR should be L or R " );
 
	if(!desc) return FALSE_dgb_fail("NULL desc");
	if(!curr_spec -> get_amnt( row, curr_amnt ))return FALSE_dgb_fail("curr_get");
	if(!prev_spec -> get_amnt( row, prev_amnt ))return FALSE_dgb_fail("prev_get");

	if(*desc == "tot_anon") {
		static obj_hold<str2> tot_anon_str = new str2(
			// "=S=U=B==T=O=T=A=L="
			"="
		);
		desc = tot_anon_str;
	}

	show_curr_amnt = curr_amnt;
	show_prev_amnt = prev_amnt;
	if( ! io_is_normal ) {
		show_curr_amnt.negate();
		show_prev_amnt.negate();
	}

	return true;
}

void rep_pdf_CH::profit_and_loss_hdr()
{
	set_pen_bold();
	// set_pen_normal();

	pen->xy_mid( PL1_NOTES_X, y_pos, "Notes" );
	pen->xy_right( PL1_CURR_X, y_pos, curr_year_end );
	pen->xy_right( PL1_PREV_X, y_pos, prev_year_end );
	y_down_line();
	pen->xy_right( PL1_CURR_X, y_pos, STR_POUND "      " );
	pen->xy_right( PL1_PREV_X, y_pos, STR_POUND "      " );
	y_down_line();
	y_down_line();
	// y_down_line();
}

bool rep_pdf_CH::profit_and_loss_row(
	bool bold,
	str0 key,
	str0 notes,
	int overlines,
	int underlines
)
{
	y_space_for_overlines( overlines );

	if(!get_row( key )) return false;

	if(bold) {
		set_pen_bold();
	}
	else
		set_pen_normal();

	pen->xy_left( PL1_LEFT_X, y_pos, *desc );
	set_pen_normal();
	pen->xy_left( PL1_NOTES_X, y_pos, notes );
	print_amnt_1( PL1_CURR_X, show_curr_amnt, overlines, underlines );
	print_amnt_1( PL1_PREV_X, show_prev_amnt, overlines, underlines );
	y_space_for_underlines( underlines );
	y_down_line();
	return false;
}

bool rep_pdf_CH::profit_and_loss_page()
{
	page_header( "Profit and Loss Account" );
	profit_and_loss_hdr();

	// bool bold = true;
	bool norm = false;
	profit_and_loss_row( norm, "PL1_a", " ", 0, 0 ); 
	profit_and_loss_row( norm, "PL1_b", " ", 0, 0 ); 
	profit_and_loss_row( norm, "PL1_c", "2", 1, 0 ); 
	y_down_line();
	profit_and_loss_row( norm, "PL1_d", " ", 0, 0 ); 
	profit_and_loss_row( norm, "PL1_e", " ", 1, 0 ); 
	y_down_line();
	profit_and_loss_row( norm, "PL1_f", " ", 0, 0 ); 
	profit_and_loss_row( norm, "PL1_g", " ", 1, 0 ); 
	y_down_line();
	profit_and_loss_row( norm, "PL1_h", "3", 0, 0 ); 
	profit_and_loss_row( norm, "PL1_i", " ", 1, 0 ); 
	y_down_line();
	profit_and_loss_row( norm, "PL1_j", "4", 0, 0 ); 
	profit_and_loss_row( norm, "PL1_k", " ", 1, 0 ); 
	y_down_line();
	profit_and_loss_row( norm, "PL1_l", " ", 0, 0 ); 
	profit_and_loss_row( norm, "PL1_m", " ", 1, 2 ); 
	// profit_and_loss_hline();
	page_footer( "The notes form part of the financial statements", "Page 3" );
	return true;
}

void rep_pdf_CH::balance_sheet_hdr()
{
	set_pen_bold();
	// flt80 w = pen_normal->calc_string_width( STR_POUND ",456)" );
	flt80 w = pen_normal->calc_string_width( ")" );
	pen->xy_right( BAL_CURR_R_X, y_pos, curr_year_end );
	pen->xy_right( BAL_PREV_R_X, y_pos, prev_year_end );
	y_down_line();
	pen->xy_right( BAL_NOTES_X, y_pos, "Notes" );
	pen->xy_right( BAL_CURR_L_X - w, y_pos, STR_POUND "___" );
	pen->xy_right( BAL_CURR_R_X - w, y_pos, STR_POUND "___" );
	pen->xy_right( BAL_PREV_L_X - w, y_pos, STR_POUND "___" );
	pen->xy_right( BAL_PREV_R_X - w, y_pos, STR_POUND "___" );
	y_down_line();
	// y_down_line();
	// flt80 xa = BAL_CURR_L_X - w;
	// flt80 xb = BAL_CURR_R_X;
	// flt80 xc = BAL_PREV_L_X - w;
	// flt80 xd = BAL_PREV_R_X;

}

bool rep_pdf_CH::balance_sheet_row(
	bool bold,
	str0 key,
	str0 notes,
	bool left,
	int overlines,
	int underlines
)
{
	if(bold) y_down_line(); // before key printed
	y_space_for_overlines( overlines );
	if(!get_row( key )) return false;

	if(bold) {	// SHOULD NOT BE A TITLE SUBSTITUTE - daft sjd
		set_pen_bold();
	}
	else
		set_pen_normal();

	flt80 BAL_CURR_X = BAL_CURR_R_X;
	flt80 BAL_PREV_X = BAL_PREV_R_X;
	if(left) {
		BAL_CURR_X = BAL_CURR_L_X;
		BAL_PREV_X = BAL_PREV_L_X;
	}

	pen->xy_left( BAL_LEFT_X, y_pos, *desc );
	set_pen_normal();
	pen->xy_right( BAL_NOTES_X, y_pos, notes );
	print_amnt_1( BAL_CURR_X, show_curr_amnt, overlines, underlines );
	print_amnt_1( BAL_PREV_X, show_prev_amnt, overlines, underlines );
	y_down_line();
	y_space_for_underlines( underlines );
	return true;
}

/*
	whatever pen is, use it for a single line (or last word)
*/
void rep_pdf_CH::x1_line( str0 text )
{
	pen->xy_left( x1, y_pos, text );
	y_down_line();
}

void rep_pdf_CH::x1_line_bold( str0 text )
{
	// pen_titles_small->xy_left( 0, y_pos, "--b2" );
	set_pen_bold();
	x1_line( text );
	set_pen_normal();
}

void rep_pdf_CH::x1_subtitle( str0 title )
{
	blank_line();
	x1_line_bold( title );
}

void rep_pdf_CH::tab_ruler (
	flt80 _L1,
	flt80 _L2,
	flt80 _L3
)
{
// TAB
	x1 = _L1;
	// x2 = _L2;
	// x3 = _L3;
}

void rep_pdf_CH::L0_title( str0 title )
{
	// pen_titles_small->xy_left( 0, y_pos, "L0" );
	flt80 x_old = x1;
	x1 = PAGE_LEFT_X;
	set_pen_titles();
	blank_line();
	x1_line( title );
	x1 = x_old;
	set_pen_normal();
	// blank_line();
}

bool rep_pdf_CH::balance_sheet_page()
{
	flt80 x1_old = x1;
	x1 = BAL_LEFT_X;
	page_header( "Balance Sheet" );
	balance_sheet_hdr();
	x1_subtitle( "FIXED ASSETS:");
	balance_sheet_row( false, "BAL_a", "5", false, 0, 0 );
	x1_subtitle( "CURRENT ASSETS:");
	balance_sheet_row( false, "BAL_b", " ", true,  0, 0 );
	balance_sheet_row( false, "BAL_c", " ", true,  0, 0 );
	balance_sheet_row( false, "BAL_d", " ", true,  1, 0 );
	x1_subtitle( "CREDITORS:");
	balance_sheet_row( false, "BAL_e", "6", true,  0, 0 );
	balance_sheet_row( true,  "BAL_f", " ", true,  1, 0 );
	balance_sheet_row( true,  "BAL_g", " ", false, 1, 2 );
	x1_subtitle( "CAPITAL AND RESERVES:");
	balance_sheet_row( false, "BAL_h", "8", false, 0, 0 );
	balance_sheet_row( false, "BAL_i", " ", false, 0, 0 );
	balance_sheet_row( true,  "BAL_j", " ", false, 1, 2 );

/*
	I hate the UK,
	I have had enough
	this is enough to return the accounts
	because I was still on 1995/2002

http://www.companieshouse.gov.uk/companiesAct/implementations/apr2008AccStatement.shtml

*/

	set_pen_normal();
	y_down_line();
	x1_line( "These accounts have been delivered in accordance with the provisions" );
	x1_line( "applicable to companies subject to the small companies regime" );
	x1_line( "" );
	buffer2 buf;
	buf.print( "For the year ending %s the company was entitled to exemption from audit", (STR0) curr_year_end_long );
	x1_line( (STR0) buf );
	x1_line( "under section 477 of the Companies Act 2006" );
	y_down_line();
	x1_line( "The members have not required the company to obtain an audit" );
	x1_line( "in accordance with section 476 of the Companies Act 2006" );
	y_down_line();
	x1_line( "The directors acknowledge their responsibility for" );
	x1_line( "complying with the requirements of the Act with respect to accounting records and for the preparation of accounts" );
	y_down_line();

	set_pen_bold();
	pen->xy_left( BAL_LEFT_X, y_pos, "ON BEHALF OF THE BOARD:" );
	y_down_line(5);

	set_pen_normal();
	pen->xy_left( BAL_LEFT_X, y_pos, "....................................." );
	y_down_line();

	buff.clear();
	buff.print( "%s - DIRECTOR", (STR0) * INFO_attr("director") );
	pen->xy_left( BAL_LEFT_X, y_pos, (str0) buff );
	y_down_line();
	y_down_line();

	set_pen_normal();
	pen->xy_left( BAL_LEFT_X, y_pos, "Dated:" );
	y_down_line();

	page_footer( "The notes form part of the financial statements", "Page 4" );
	x1 = x1_old;;
	
	return true;
}

void rep_pdf_CH::NFS_hdr( str0 title )
{
	set_pen_bold();
	// flt80 w = pen_normal->calc_string_width( STR_POUND ",456)" );
	flt80 w = pen_normal->calc_string_width( ")" );
	pen->xy_left( NFS_LEFT_X, y_pos, title );
	pen->xy_right( NFS_CURR_X, y_pos, curr_year_end );
	pen->xy_right( NFS_PREV_X, y_pos, prev_year_end );
	y_down_line();
	pen->xy_right( NFS_CURR_X - w, y_pos, STR_POUND "___" );
	pen->xy_right( NFS_CURR_X - w, y_pos, STR_POUND "___" );
	pen->xy_right( NFS_PREV_X - w, y_pos, STR_POUND "___" );
	pen->xy_right( NFS_PREV_X - w, y_pos, STR_POUND "___" );
	y_down_line();
	// y_down_line();
}

bool rep_pdf_CH::NFS_row(
	str0 key,
	int overlines,
	int underlines
)
{
	y_space_for_overlines( overlines );
	if(!get_row( key )) return false;

	set_pen_normal();

	pen->xy_left( NFS_LEFT_X, y_pos, *desc );
	set_pen_normal();
	print_amnt_1( NFS_CURR_X, show_curr_amnt, overlines, underlines );
	print_amnt_1( NFS_PREV_X, show_prev_amnt, overlines, underlines );
	y_down_line();
	y_space_for_underlines( underlines );
	return true;
}

bool rep_pdf_CH::notes_to_the_fs_pages()
{
	page_header( "Notes to the Financial Statements" );

	L0_title( "1. Acounting Policies" );

	x1_subtitle( "Accounting Convention");
	x1_line("The financial statements have been prepared under the historical cost convention" );
	x1_line("and in accordance with the FRSSE (eff June 2002)");

	x1_subtitle( "Turnover");
	x1_line("Turnover represents net invoiced sales of services, excluding VAT" );

	x1_subtitle( "Tangible Fixed Assets");
	x1_line("Depreciation is provided at the following annual rates in order to write off each asset over its estimated useful life");
// LURK 20% is fixed string - but is computed elsewhere
// should be a setting in CH1, rate
// cls_CH1_checker.js:     rate = this.CH1.get_rate_depreciation()
//
	x1_line("Plant and machinery etc  --  20% on reducing balance");

	x1_subtitle( "Deferred Tax");
	x1_line("Deferred Tax is recognised in respect of all timing differences that have originated but not yet been reversed");

	x1_subtitle( "Pensions");
	x1_line("The company operates a defined contribution Pension scheme (EPP).");
	x1_line("Contributions were significantly reduced Dec 2006.");
	x1_line("Contributions are charged in the profit and loss account");

	L0_title( "2. Operating (Loss) Profit" );
	NFS_hdr("This is stated after charging:");
	NFS_row( "NFS_OP_a", 0, 0 );
	NFS_row( "NFS_OP_b", 0, 0 );
	NFS_row( "NFS_OP_c", 0, 0 );

	L0_title( "3. Taxation" );
//	x1_line("Here Current tax is the same as UK corporation tax");
//	x1_line("Charged at 20%");
	NFS_hdr( "Current tax:  Corp. Tax" );
	NFS_row( "NFS_TAX_d", 0, 0 );

	L0_title( "4. Dividends" );
	NFS_hdr("");
	NFS_row( "NFS_DIV_e", 0, 0 );
// 	NFS_row( "T3", 0, 0 );	// if not zero

// actually I think that F1 fines should go here
// as it is post taxation payment
//
// not T3 is being treated as a +ve tax charge

	page_footer( "", "Page 5" );

	page_header( "Notes ..." );

	/*
		the sign of assets is a bit tricky
		HISTORY + DEPRECIATION = CURRENT

		as asset is good for the company is +ve
		the cost of buying an asset is -ve
		within year transactions: -ve cost/+ve asset = zero
		at end year depreciation = -25%


		ERRM - is that reduced my micro-managing Brown to 20% - when ?

	*/
	L0_title( "5. Tangible fixed assets" );
	NFS_hdr( "Plant and Machinery etc");
	x1_subtitle( "ASSET ACCUMULATED HISTORY COST:" );
	NFS_row( "NFS_TFA_f", 0, 0 );
	NFS_row( "NFS_TFA_F", 0, 0 );
	NFS_row( "NFS_TFA_g", 0, 0 );
	x1_subtitle( "DEPRECIATION:" );
	NFS_row( "NFS_TFA_h", 0, 0 );
	NFS_row( "NFS_TFA_i", 0, 0 );
	NFS_row( "NFS_TFA_j", 0, 0 );
	x1_subtitle( "NET BOOK VALUE:" );
	NFS_row( "NFS_TFA_k", 0, 0 );
	NFS_row( "NFS_TFA_l", 0, 0 );

	L0_title( "6. Debtors:" );
	NFS_hdr( "amounts falling due within one year" );
	NFS_row( "NFS_DBT_M", 0, 0 );

	L0_title( "7. Creditors:" );
	NFS_hdr( "amounts falling due within one year of");
	NFS_row( "NFS_CRD_m", 0, 0 );
	NFS_row( "NFS_CRD_n", 0, 0 );
	NFS_row( "NFS_CRD_o", 0, 0 );
	NFS_row( "NFS_CRD_p", 0, 0 );
	NFS_row( "NFS_CRD_q", 0, 0 );
	NFS_row( "NFS_CRD_r", 1, 2 );

	L0_title( "8. Called UP Share Capital:" );
	NFS_hdr( "Authorised number of ordinary " STR_POUND "1(nominal)");
	NFS_row( "NFS_SH_s", 0, 0 );
	y_down_lines(1);
	NFS_hdr( "Allotted, issues and fully paid. ordinary " STR_POUND "1(nominal)");
	NFS_row( "NFS_SH_t", 0, 0 );

	page_footer( "", "Page 6" );
	return true;
}

struct year_finder
{
	bool is_XXXX_2003;
	bool is_2003_2004;
	bool is_2004_2005;
	bool is_2005_2006;
	bool is_2006_2006;	// the 2 month year
	bool is_done_gone;	// oriinals have been send this is not the orig
	bool is_2006_2007;
	bool is_2007_2008;

	bool is_2008_2009;
	bool is_2009_2010;
	bool is_2010_2011;
	bool is_2011_2012;
	bool is_2012_2013;
	bool is_2013_stop;

	bool is_2008_XXXX;

	year_finder( date_glib & ard )
	{
		init_0();
		pick_year(ard);
	}

	void init_0()
	{
		is_XXXX_2003 = false; // SDJ accountant, or earlier
		is_2003_2004 = false;
		is_2004_2005 = false;
		is_2005_2006 = false;
		is_2006_2006 = false;
		is_done_gone = false; // original is POSTED so mark any rerun
		is_2006_2007 = false;
		is_2007_2008 = false; // edit this name to is_2008_PLUS
		is_2008_2009 = false;
		is_2009_2010 = false;
		is_2010_2011 = false;
		is_2011_2012 = false;
		is_2012_2013 = false;
		is_2013_stop = false;

		is_2008_XXXX = false; // edit this name to is_2008_PLUS
	}

	void pick_year( date_glib & ard )
	{
	 //	i32 ard_julian = ard.julian(); // unused
		int ard_year = 0;
		int ard_mm = 0;
		int ard_dd = 0;
		ard.get_year_mm_dd( ard_year, ard_mm, ard_dd );                  
		if( ard_year < 2004 ) {
			is_XXXX_2003 = true; // old accountant
			return;
		} 

		if( ard_year < 2011 ) { // edit each year, after post sent
			is_done_gone = true; // posted
		}

		if( ard_year > 2011 ) { // edit each year, after post sent
			is_2008_XXXX = true; // error - edit this cript
		}
		switch( ard_year ) {
		 case 2004: is_2003_2004 = true; return;
		 case 2005: is_2004_2005 = true; return;
		 case 2006: 
			if( ard_mm == 1 )
			    is_2005_2006 = true;
			else
			    is_2006_2006 = true;
			return;
		 case 2007: 
			is_2006_2007 = true;
			return;
		 case 2008: 
			is_2007_2008 = true;
			return;
		 case 2009: 
			is_2008_2009 = true;
			return;
		 case 2010: 
			is_2009_2010 = true;
			return;
		 case 2011: 
			is_2010_2011 = true;
			return;
		 case 2012: 
			is_2011_2012 = true;
			return;
		 case 2013: 
			is_2012_2013 = true;
			return;
		 case 2014: 
			is_2013_stop = true;
			return;
		 default  : is_2008_XXXX = true; return;
		}
	}
};

bool rep_pdf_CH::report_of_the_accountants_page()
{
	year_finder YY ( ARD );

	page_header( "Report of the Accountants" );

	if( YY.is_XXXX_2003 ) {
		x1_line("THESE PAGES ARE NOT THE ORIGINALS FROM THE ACCOUNTANT" );
		x1_line("THESE ARE REPORTS SIMILAR TO BUT DIFFERENT FROM THEM" );
		blank_line();
		x1_line("Prior to 2003-02-01, the previous accountants were SJD Accountancy (HP23 5AP)" );
		x1_line("They retain and support previous years accounting records." );
		blank_line();
	} else {

		x1_line("These accounts have been prepared by Information Cascade Ltd" );
		blank_line();
	}

#if 0
	x1_line("Prior to 2003-02-01, the previous accountants were SJD Accountancy (HP23 5AP)" );
	x1_line("They retain and support previous years accounting records." );
	blank_line();
#endif

	if( YY.is_done_gone ) {
		x1_line("THESE PAGES ARE NOT THE ORIGINALS" );
		x1_line("Those have been saved, printed and posted.");
		x1_line("look in the /SENT/ subdirectory" );
		x1_line("This report is a re-run, possibly with different results.");
		blank_line();
	}

#if 0
	if( YY.is_2003_2004 || YY.is_2004_2005 ) {
	x1_line("Corporation Tax is pending recalculation, and will be shown in the following year:" );
	blank_line();
	}

	if( YY.is_2005_2006 ) {
		x1_line("Corporation Tax included from previous years is: XXX" );
	}
#endif

	if( YY.is_2006_2006 ) {
		x1_line("2006-02-01 .. 2006-03-31 is a two month year" );
		x1_line("2006-03-31 is the new A-R-D, aligned close to the UK IR tax year" );
		x1_line("This simplifies PAYE splits and CT splits (over year parts)" );
		x1_line("particularly where the adjacent year (no-longer) overlaps its next neighbour" );
		blank_line();
		x1_line("Asset write-down uses approx 4% (instead of 25%)" );
		x1_line("59 days = Feb + Mar = 28 + 31" );
		x1_line("Linear arithmetic says 4.04 % = 59 / 365 * 25 %" );
		x1_line("Compound arithmetic says 4.54 % (0.95456 = exp(59/365*log_nat(.75)))" );
		x1_line("Copy+Paste on UNIX:  echo '100*(1-e(59/365*l((100-25)/100)))' | bc -l");
	}

	if( YY.is_2006_2007 ) {
		x1_line("2006-02-01 .. 2006-03-31 was a two month year" );
		x1_line("Chosen to re-align the A-R-D close to the UK IR tax year" );
		x1_line("A 2 month year was chosen over a 14 month year, for comparisons" );
		x1_line("So compare this years figures with the year before last" );
		blank_line();
		x1_line("ILEA tax refund of 2005-10-04 is shown as if taking effect 2006-04-06 this year" );
		x1_line("Cheque cleared 2007-05-22 This is a minor error correction." );
		blank_line();
		x1_line("The Pension scheme still exists, but contributions were (temp) suspended December." );
		blank_line();
		x1_line("This page generated by: " __FILE__ );
		blank_line();
	}

	if( YY.is_2007_2008 ) {
	//	x1_line("Using an asset depreciation of 20%" ); // see page ...
		x1_line("" );
		x1_line("The Pension scheme still exists, but contributions are (still) suspended." );
		x1_line("" );
		x1_line("" );
		x1_line("To modify this page, edit the source" );
		x1_line( __FILE__ );
		blank_line();
	}

	if( YY.is_2008_2009 ) {
	//	x1_line("Using an asset depreciation of 20%" ); // see page ...
		x1_line("" );
		x1_line("The Pension scheme still exists, but contributions are (still) suspended." );
		x1_line("" );
		x1_line("The HMRC corp tax figures are yet to be confirmed." );
		x1_line("" );
		x1_line("To modify this page, edit the source" );
		x1_line( __FILE__ );
		blank_line();
	}

// 0809 has been sent, 2010, a bit late $375 F1 2010-05-17

	if( YY.is_2009_2010 ) {
	//	x1_line("Using an asset depreciation of 20%" ); // see page ...
		x1_line("" );
		x1_line("The Pension scheme still exists, but contributions are (still) suspended." );
		x1_line("" );
		x1_line("There is £56.05 CORP TAX from 2008-2009" );
		x1_line("which is carried over into here" );
		x1_line("" );
		x1_line("There is a late filing penalty from end 2008-2009" );
		x1_line("which is carried over into here" );
		x1_line("" );
		x1_line("There is a double late filing penalty from end 2009-2010" );
		x1_line("which is carried over out from here" );
		x1_line("" );
		x1_line("" );
		x1_line("To modify this page, edit the source" );
		x1_line( __FILE__ );
		blank_line();
	}

	if(
	 ( YY.is_2010_2011 )
	 ) {
	//	x1_line("Using an asset depreciation of 20%" ); // see page ...
		x1_line("" );
		x1_line("The Pension scheme still exists, but contributions are (still) suspended." );
		x1_line("" );
		x1_line("There is a double late filing penalty from end 2009-2010" );
		x1_line("which is carried over into 2010_2011" );
		x1_line("" );
		x1_line("To modify this page, edit the source" );
		x1_line( __FILE__ );
		x1_line("" );
		x1_line("UPDATE:" );
		x1_line("THIS MAY DIFFER FROM THE SUBMITTED RETURN, by a few pounds only" );
		x1_line("That was prepared to the penny, added, then rounded," );
		x1_line("This is prepared to the pound then added" );
		x1_line("" );
		x1_line("The sum of approximations, is not the sum of exact rounded" );
		x1_line("Several part-pounds may be added then rounded, or rounded first." );
		x1_line("" );
		x1_line("I need to regenerate this, to dovetail the PREVIOUS years numbers into next years report" );
		x1_line("And the HMRC adobe javascript forms are pedantic, about adding up post rounded inputs" );
		x1_line("IE The figures have been adjusted to fit into the form" );
		x1_line("" );
		blank_line();
	}

	if(
	 ( YY.is_2011_2012 )
	 ) {
	//	x1_line("Using an asset depreciation of 20%" ); // see page ...
		x1_line("" );
		x1_line("The Pension scheme still exists, but contributions are (still) suspended." );
		x1_line("" );
		x1_line("" );
		x1_line("To modify this page, edit the source" );
		x1_line( __FILE__ );
		x1_line("" );
		x1_line("Last years numbers have been recomputed, and may differ by a few pounds" );
		x1_line("These numbers are rounded THEN added, which adds together the rounding-error" );
		x1_line("Last years were computed to the penny, then rounded to be reported." );
		x1_line("" );
		x1_line("Company will be shutting down. 2013" );
		blank_line();
	}

	if(
	 ( YY.is_2012_2013 )
	 ) {
	//	x1_line("Using an asset depreciation of 20%" ); // see page ...
		x1_line("" );
		x1_line("The Pension scheme still exists, but contributions are (still) suspended." );
		x1_line("XXX EDIT XXX" );
		x1_line("" );
		x1_line("To modify this page, edit the source" );
		x1_line( __FILE__ );
		x1_line("" );
		x1_line("Company will be shutting down. 2013" );
		blank_line();
	}

	if(
	 ( YY.is_2013_stop )
	 ) {
	//	x1_line("Using an asset depreciation of 20%" ); // see page ...
		x1_line("" );
		x1_line("The Pension scheme still exists, but contributions are (still) suspended." );
		x1_line("XXX EDIT XXX" );
		x1_line("" );
		x1_line("To modify this page, edit the source" );
		x1_line( __FILE__ );
		x1_line("" );
		x1_line("Company shutting down. this is partial year, and possibly a draft estimate for HMRC" );
		blank_line();
	}

	if( YY.is_2008_XXXX ) {
		x1_line("THESE PAGES ARE IN NEED OF BEING UPDATED" );
		x1_line("edit rep_pdf_CH.cxx ");
		x1_line( __FILE__ );
		blank_line();
	}

	page_footer( "This page does NOT form part of the financial statements", "Page 7" );
	return true;
}

void rep_pdf_CH::PL2_hdr( str0 title )
{
	set_pen_bold();
	flt80 w = pen_normal->calc_string_width( STR_POUND ",456)" );
	flt80 wb = pen_normal->calc_string_width( ")" );
	pen->xy_left(  PL2_LEFT_X,   y_pos, "Item" );
	pen->xy_right( PL2_CURR_R_X, y_pos, curr_year_end );
	pen->xy_right( PL2_PREV_R_X, y_pos, prev_year_end );
	y_down_line();
 if(0) {
	pen->xy_left( PL2_CURR_L_X - w, y_pos, STR_POUND "___" );
	pen->xy_left( PL2_CURR_R_X - w, y_pos, STR_POUND "___" );
	pen->xy_left( PL2_PREV_L_X - w, y_pos, STR_POUND "___" );
	pen->xy_left( PL2_PREV_R_X - w, y_pos, STR_POUND "___" );
 } else {
	pen->xy_right( PL2_CURR_L_X - wb, y_pos, STR_POUND "___" );
	pen->xy_right( PL2_CURR_R_X - wb, y_pos, STR_POUND "___" );
	pen->xy_right( PL2_PREV_L_X - wb, y_pos, STR_POUND "___" );
	pen->xy_right( PL2_PREV_R_X - wb, y_pos, STR_POUND "___" );
 }
	y_down_line();
	// y_down_line();
}

bool rep_pdf_CH::PL2_row(
	str0 key,
	bool left,
	int overlines,
	int underlines
)
{
	y_space_for_overlines( overlines );
	if(!get_row( key )) return false;

	set_pen_normal();

	flt80 PL2_CURR_X = PL2_CURR_R_X;
	flt80 PL2_PREV_X = PL2_PREV_R_X;
	if(left) {
		PL2_CURR_X = PL2_CURR_L_X;
		PL2_PREV_X = PL2_PREV_L_X;
	}

	pen->xy_left( PL2_LEFT_X, y_pos, *desc );
	set_pen_normal();
	print_amnt_1( PL2_CURR_X, show_curr_amnt, overlines, underlines );
	print_amnt_1( PL2_PREV_X, show_prev_amnt, overlines, underlines );
	y_down_line();
	y_space_for_underlines( underlines );
	return true;
}

bool rep_pdf_CH::profit_and_loss_account_page()
{
	flt80 x1_old = x1;
	x1 = PL2_LEFT_X;
	page_header( "Profit and Loss Account" );
	PL2_hdr("ITEM");
	x1_subtitle("Income");
	PL2_row( "PL2_a", false, 0, 0 );
	PL2_row( "PL2_b", false, 0, 0 );
	PL2_row( "PL2_c", false, 1, 0 );
	x1_subtitle("Expendature");
	PL2_row( "PL2_d", true,  0, 0 );
	PL2_row( "PL2_e", true,  0, 0 );
	PL2_row( "PL2_f", true,  0, 0 );
	PL2_row( "PL2_g", true,  0, 0 );
	PL2_row( "PL2_h", true,  0, 0 );
	PL2_row( "PL2_i", true,  0, 0 );
	PL2_row( "PL2_j", true,  0, 0 );
	PL2_row( "PL2_k", true,  0, 0 );
	PL2_row( "PL2_l", true,  0, 0 );
	PL2_row( "PL2_m", true,  0, 0 );
	PL2_row( "PL2_n", true,  0, 0 );
	PL2_row( "PL2_o", true,  1, 0 );
	PL2_row( "PL2_p", false, 1, 0 );
	x1_subtitle("Finance Costs");
	PL2_row( "PL2_q", true,  0, 0 );
	PL2_row( "PL2_r", true,  0, 0 );
	PL2_row( "PL2_s", true,  0, 0 );
	PL2_row( "PL2_t", true,  1, 0 );
	PL2_row( "PL2_u", false, 1, 0 );
	x1_subtitle("Depreciation");
	PL2_row( "PL2_v", false, 0, 0 );
	x1_subtitle("NETT");
	PL2_row( "PL2_w", false, 1, 2 );

	page_footer( "This page does NOT form part of the financial statements", "Page 8" );
	x1 = x1_old;
	return true;
}

