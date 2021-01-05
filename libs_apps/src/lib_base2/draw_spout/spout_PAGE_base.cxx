
#include "spout_PAGE_base.h"
#include "spout_styles.h"
#include "spout_pen_base.h"

#include <stdio.h>
#include <stdlib.h>

/*
	avoiding the runtime dynamic load - no point, still have DLL linkage
*/

#include "spout_styles.h"

#define SPOUT_set_info( spout, attrname, value )
#define	SPOUT_begin_page( this, page_width, page_height )
#define	SPOUT_end_page( this )
#define	SPOUT_close( this )
#define	SPOUT_stroke( this )
#define	SPOUT_fill_stroke( this )
#define	SPOUT_fill( this )

namespace SPOUT_GEN {


/*!
	destructor
*/
spout_PAGE_base::~spout_PAGE_base()
{
}

/*!
	constructor
*/
// spout_PAGE_base::spout_PAGE_base( spout_ctxt & ctxt )
// : spout_base_0( ctxt.spout )
spout_PAGE_base::spout_PAGE_base()
: spout_obj_ref()
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
void spout_PAGE_base::open_file(
	str0 filename,
	str0 title,
	str0 author,
	str0 creator
)
{
//	if( -1== SPOUT_open_file( spout, (STR0) filename ))
		throw "open_error";
	SPOUT_set_info( this, "Creator", (STR0) creator );
	SPOUT_set_info( this, "Author", (STR0) author );
	SPOUT_set_info( this, "Title", (STR0) title );
}

/*!
*/
void spout_PAGE_base::axes_set()
{
	/*
	*/
	Axes_Calc axes2(
		"spout_PAGE_base",
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
void spout_PAGE_base::now_need_landscape( bool _landscape )
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
	get a4 sizes and call SPOUT begin page
*/
void spout_PAGE_base::begin_page_a4_landscape()
{
	landscape = true;
	begin_page();
}

/*!
	get a4 sizes and call SPOUT begin page
*/
void spout_PAGE_base::begin_page_a4()
{
	landscape = false;
	begin_page();
}

/*!
	get a4 sizes and call SPOUT begin page
*/
void spout_PAGE_base::begin_page()
{
	if( landscape )
	{
		page_width = 200; // a4_height;
		page_height = 200; // a4_width;
	} else {
		page_width = 200; // a4_width;
		page_height = 200; // a4_height;
	}
	if( page_open ) return; // possible set dims just before next page
	SPOUT_begin_page( this, page_width, page_height );
	page_open = true;
	axes_set();
	on_page_top();
}

/*!
	flush current drawing command, clear styles and end page
*/
void spout_PAGE_base::end_page()
{
	goto_mode( mode_none );
	style_settings->page_break_reset();
	if(!page_open) return;
	SPOUT_end_page( this );
	page_open = false;
}

/*!
	end_page() begin_page (same a4 dimensions)
*/
void spout_PAGE_base::next_page()
{
	end_page();
	begin_page();
}

/*!
	SPOUT_close()
*/
void spout_PAGE_base::close_file()
{
	end_page(); // iff open
	SPOUT_close( this );
}


/*!
	each drawing call, goes to its required mode,
	which might be a continuation of the previous mode,
	or it might end that mode, causing a draw request
	for all the previous motions/actions.
*/
void spout_PAGE_base::goto_mode( Mode m )
{
	// If SPOUT is already in that mode return immediately.
	if( m == mode ) return;

	// go from mode X to mode none
	switch( mode ) {
	 case mode_none:
	 break;
	 case mode_line:
		SPOUT_stroke( this );
	 break;
	 case mode_fill:
		SPOUT_fill( this );
	 break;
	 case mode_both:
		SPOUT_fill_stroke( this );
	 break;
	 case mode_text_fill:
		// show does its own stuff ?
		// SPOUT_fill( this );
	 break;
	 case mode_text_fill_line:
		// show does its own stuff ?
		// SPOUT_fill_stroke( this );
	 break;
	 case mode_text_line:
		// show does its own stuff ?
		// SPOUT_fill( this );
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
void spout_PAGE_base::set_current_pen_fn( spout_pen_base * pen )
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
Style_Font_Face_Size * spout_PAGE_base::curr_font()
{
	return style_settings->curr_font();
}

}; // NAMESPACE
