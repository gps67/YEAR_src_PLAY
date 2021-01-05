
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
void pdf_base::open_file(
	str0 filename,
	str0 title,
	str0 author,
	str0 creator
)
{
	if( -1== PDF_open_file( pdf, (STR0) filename ))
		throw "open_error";
	PDF_set_info( pdf, "Creator", (STR0) creator );
	PDF_set_info( pdf, "Author", (STR0) author );
	PDF_set_info( pdf, "Title", (STR0) title );
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
		begin_page_a4_landscape();
	else
		begin_page_a4();
}

/*!
	get a4 sizes and call PDF begin page
*/
void pdf_base::begin_page_a4_landscape()
{
	landscape = true;
	begin_page();
}

/*!
	get a4 sizes and call PDF begin page
*/
void pdf_base::begin_page_a4()
{
	landscape = false;
	begin_page();
}

/*!
	get a4 sizes and call PDF begin page
*/
void pdf_base::begin_page()
{
	if( landscape )
	{
		page_width = a4_height;
		page_height = a4_width;
	} else {
		page_width = a4_width;
		page_height = a4_height;
	}
	if( page_open ) return; // possible set dims just before next page
	PDF_begin_page( pdf, page_width, page_height );
	page_open = true;
	axes_set();
	on_page_top();
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


/*!
	each drawing call, goes to its required mode,
	which might be a continuation of the previous mode,
	or it might end that mode, causing a draw request
	for all the previous motions/actions.
*/
void pdf_base::goto_mode( Mode m )
{
	// If PDF is already in that mode return immediately.
	if( m == mode ) return;

	// go from mode X to mode none
	switch( mode ) {
	 case mode_none:
	 break;
	 case mode_line:
		PDF_stroke( pdf );
	 break;
	 case mode_fill:
		PDF_fill( pdf );
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

	// go from mode none to mode m - setup new/current styles
	mode = m;
	switch( mode ) {
	 case mode_none:
	 break;
	 case mode_line:
		style_settings->flush_line();
	 break;
	 case mode_fill:
		style_settings->flush_fill();
	 break;
	 case mode_both:
		style_settings->flush_line();
		style_settings->flush_fill();
	 break;
	 case mode_text_fill:
		style_settings->flush_text();
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
