
#include "pdf_base.h"
#include "pdf_styles.h"
#include "pdf_pen_base.h"

#include <stdio.h>
#include <stdlib.h>

/*
	avoiding the runtime dynamic load - no point, still have DLL linkage
*/

#include "pdf_styles.h"

namespace PDF_GEN {


/*!
	destructor
*/
pdf_base::~pdf_base()
{
}

/*!
	constructor
*/
pdf_base::pdf_base( pdf_ctxt & ctxt )
: pdf_base0( ctxt.pdf )
, axes( "default_init", 0, 0, 1, 1 )
, style_settings( new Style_Settings(this) )
{
	mode = mode_none;
	landscape = false;
	page_open = false;
}

/*!
	open file and set title/author/...
*/
bool pdf_base::open_file(
	str0 filename,
	str0 title,
	str0 author,
	str0 creator
)
{
	if( -1== PDF_open_file( pdf, (STR0) filename )) {
		FAIL("PDF_open_file(pdf,'%s'", (STR0) filename );
		return false;
		throw "open_error";
		// lands well when throw called within gdb
	}
	PDF_set_info( pdf, "Creator", (STR0) creator );
	PDF_set_info( pdf, "Author", (STR0) author );
	PDF_set_info( pdf, "Title", (STR0) title );
	return true;
}

/*!
*/
void pdf_base::axes_set()
{
	/*
	*/
	Axes_Calc axes2(
		"pdf_base",
		0,
		page_height,
		 1.0 * page_width,
		-1.0 * page_height
	);
	axes = axes2;
#if 0
	float t1 = axes.X.dev_from_surface( 0.0 );
	float t2 = axes.Y.dev_from_surface( 0.0 );
	float t3 = axes.X.dev_from_surface( 1.0 );
	float t4 = axes.Y.dev_from_surface( 1.0 );
	// printf( " 0( %5.2f %5.2f ) 1( %5.2f %5.2f ) \n", t1, t2, t3, t4 );
	t1 = axes.X.origin;
	t2 = axes.Y.origin;
	t3 = axes.X.factor;
	t4 = axes.Y.factor;
	// printf( " +( %5.2f %5.2f ) *( %5.2f %5.2f ) \n", t1, t2, t3, t4 );
#endif
}


/*!
	The caller needs lanscape (portrait if false) NOW.
	If already in that mode, not even a blank line.
*/
void pdf_base::now_need_landscape( bool _landscape )
{
	if( landscape == _landscape )
	{
		now_need_page();
		return;
	}

	if(page_open) end_page();

	if( _landscape )
		begin_page_a3_landscape();	// go big
	//	begin_page_a4_landscape();
	//	begin_page_a4(); // default portrait 
	//	OPTION A4_A4 LHS_RHS // apply rule to A4_PAGE_L _L _R ALIAS
	//	USES a4_% _height _width 
// ./src/lib_base2/draw_spout/spout_PAGE_base.cxx:void spout_PAGE_base::begin_page_a4_landscape()
	else
		begin_page_a4();		// go home

	// figure out print a3 on a4 printer // with borders and gaps
	// eg split a3_Landscape to a4_Portrait_LEFT_side _RIGHT_
	// eg zoom down a3_Landscape to a4_landscape_at_ZOOM_HALF
	//
}

bool pdf_base::set_WH_A4( pdf_WH_t & WH_A4 ) {
	WH_A4.H = a4_height;
	WH_A4.W = a4_width;
	return true;
}

bool pdf_base::set_WH_A3( pdf_WH_t & WH_A3 ) {
	WH_A3.H = a3_height;
	WH_A3.W = a3_width;
	WH_A3.H = a2_height;
	WH_A3.W = a2_width;
	WH_A3.FLIP_WH();
	return true;
}

/*!
	get a4 sizes and call PDF begin page
*/
bool pdf_base::begin_page_a3_landscape()
{
	pdf_WH_t WH_A3; 
	set_WH_A3( WH_A3 ); 
	landscape = true;
	begin_page_WH(WH_A3);
	return true;
}

/*!
	get a4 sizes and call PDF begin page
*/
bool pdf_base::begin_page_a4_landscape()
{
	pdf_WH_t WH_A4; 
	set_WH_A4( WH_A4 ); 
	landscape = true;
	begin_page_WH(WH_A4);
	return true;
}

/*!
	get a4 sizes and call PDF begin page
*/
bool pdf_base::begin_page_a4()
{
	landscape = false;
	pdf_WH_t WH_A4; 
	set_WH_A4( WH_A4 ); 
	begin_page_WH(WH_A4);
	return true; // OK
}

/*!
	get a4 sizes and call PDF begin page
*/
bool pdf_base::begin_page()
{
#ifdef _SHOW_OLD_CODE
	if( landscape )
	{
		WH.H = a4_height;
		WH.W = a4_width;
	} else {
		WH.W = a4_width;
		WH.H = a4_height;
	}
#endif _SHOW_OLD_CODE

	// copy value of WH from current page SESS // complete BATCH STEP //
	return begin_page_WH( page_WH );
}

/*!
	get a4 sizes and call PDF begin page
*/
bool pdf_base::begin_page_WH( pdf_WH_t WH )
{
	// OPTION NULL_WH means reuse CURR value //
	// OPTION CHECK TOKENISED SPELLING // CHOICES and AVAIL_DATA OPTS
	// on_DETECT "{ LHS = RHS }" called when MATCH_SAME_SELF
	// MATCH MULTI SAME // we are all SELF THIS THAT HERE LOCN CSR IDX
	// MATCH MULTI USAGE of "IDX" incl as "idx" incl as "i8_idx"
	// SUB_LEX_PLAIN "i8 _ idx -- because of C "{ i8 BYTE }" // PICK i8
	// PICK i8 i16 i24 i32 i48 i64 // ALIAS i9 += i32 // ENTIRE BASE WORD

	page_WH = WH;
	if( landscape )
	{
		page_WH.FLIP_WH();
		// page_width = a4_height;
		// page_height = a4_width;
	} else {
		// PORTRAIT is the DEFAULT home printer
		// page_width = a4_width;
		// page_height = a4_height;
	}

	INFO("W %f H %f", page_WH.W, page_WH.H );
	if( page_open ) return true; // possible set dims just before next page
	PDF_begin_page( pdf, page_WH.W, page_WH.H );
	page_open = true;
	axes_set();
	on_page_top();
	return true; //	void looks clean code - but lacks detailed error mgmt
}

/*!
	flush current drawing command, clear styles and end page
*/
void pdf_base::end_page()
{
	goto_mode( mode_none );
	style_settings->page_break_reset();
	if(!page_open) return;
	PDF_end_page( pdf );
	page_open = false;
}

/*!
	end_page() begin_page (same a4 dimensions)
*/
void pdf_base::next_page()
{
	end_page();
	begin_page();
}

/*!
	PDF_close()
*/
void pdf_base::close_file()
{
	end_page(); // iff open
	PDF_close( pdf );
}

bool pdf_base::goto_mode_none()
{
	Mode old_mode = mode; mode = mode_none;
	// go from mode X to mode none
	switch( old_mode ) {
	 case mode_none:
	 break;
	 case mode_fill:
		PDF_fill( pdf );
	 break;
	 case mode_line:
		PDF_stroke( pdf );
	 break;
	 case mode_both:
		PDF_fill_stroke( pdf );
	 break;
	 case mode_text_fill:
		// show does its own stuff ?
		// PDF_fill( pdf );
	 break;
	 case mode_text_fill_line:
		// show does its own stuff ?
		// PDF_fill_stroke( pdf );
	 break;
	 case mode_text_line:
		// show does its own stuff ?
		// PDF_fill( pdf );
	 break;
	}
	return true;
}


/*!
	each drawing call, goes to its required mode,
	which might be a continuation of the previous mode,
	or it might end that mode, causing a draw request
	for all the previous motions/actions.

	Notice that there is no caller to return FAIL info to
	The present caller is a new task flushing the old task BATCH SWEEP
	But no SWEEP just goto_mode( mode_none )
	Mostly unconnected events, when run do nothing, and are done
*/
bool pdf_base::goto_mode( Mode m )
{
	// If PDF is already in that mode return immediately.
	if( m == mode ) return true;

	// go from mode X to mode none
	goto_mode_none();
	// if(!goto_mode_none()) return FAIL_FAILED ;

	// go from mode none to mode m - setup new/current styles
	mode = m;
	switch( mode ) {
	 case mode_none:
	 break;
	 case mode_line:
		style_settings->flush_line();	// APPLY CACHE QUEUED STYLE
	 break;
	 case mode_fill:
		style_settings->flush_fill();	// APPLY CACHE QUEUED STYLE
	 break;
	 case mode_both:
		style_settings->flush_line();
		style_settings->flush_fill();
	 break;
	 case mode_text_fill:
		style_settings->flush_text();	// APPLY QUEUED STYLE LIST
		style_settings->flush_line();
		style_settings->flush_fill();
	 break;
	 case mode_text_line:
	 case mode_text_fill_line:
		style_settings->flush_text();
		style_settings->flush_line();
		style_settings->flush_fill();
	 break;
	}
	return true;
}

/*!
*/
void pdf_base::set_current_pen_fn( pdf_pen_base * pen )
{
	if( pen_cache == pen ) return;
	pen_cache = pen;
	// void style_use( int s );
	goto_mode( mode_none );
 // if( pen )
	style_settings->requesting_style( pen ); // pen is a Style_Tray
}

/*!
*/
Style_Font_Face_Size * pdf_base::curr_font()
{
	return style_settings->curr_font();
}

}; // NAMESPACE
