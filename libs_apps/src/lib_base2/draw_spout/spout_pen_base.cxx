#include "spout_PAGE_base.h"
#include "spout_pen_base.h"

// WRITE THESE
#define	SPOUT_set_text_pos(A,B,C)
#define	SPOUT_show(spout,str_8859)
#define	SPOUT_continue_text(spout,str_8859)
#define	SPOUT_moveto(spout,X,Y)
#define	SPOUT_lineto(spout,X,Y)
#warning WRITE THESE FUNCTIONS

using namespace SPOUT_GEN;

// spout_pen_base::


spout_pen_base::spout_pen_base(
	spout_PAGE_base * _spout
)
: Style_Tray( _spout )
, axes( spout->axes )
{
}

/*!
	creating one pen from another pen, is usually followed
	by setting some different style or axes.

	It is allowed to change things during the initialisation
	phase, but after that ... at least call unset_current()
*/
spout_pen_base::spout_pen_base(
	spout_pen_base * _base
)
: Style_Tray( _base->spout )
, axes( _base->axes )
{
	// axes = _base->axes;
	axes.set_name("PEN");
}



///////////////////////////////////

#if 0
// I think that PDF had its own way of doing things
//

float spout_pen_text::get_text_x()
{
	return axes.X.surface_from_dev( SPOUT_get_value( spout, "textx", 0 ));
}

float spout_pen_text::get_text_y()
{
	return axes.Y.surface_from_dev( SPOUT_get_value( spout->spout, "texty", 0 ));
}
#endif

/*!
*/
float spout_pen_text::get_descender()
{
	set_mode_text();
	return axes.Y.surface_from_dev_step(
		curr_font()->get_descender()
	);
}

/*!
*/
float spout_pen_text::get_ascender()
{
	set_mode_text();
	return axes.Y.surface_from_dev_step(
		curr_font()->get_ascender()
	);
}

// _vector is from baseline +/-
/*!
*/
float spout_pen_text::get_descender_v()
{
	if( 0.0 > get_descender() )
	{
		e_print("get_descender_v() not posative! %6.5f\n", (float) get_descender() );
	}
	return  get_descender(); // already negative
}

/*!
*/
float spout_pen_text::get_ascender_v()
{
	return get_ascender();
}

/*!
*/
float spout_pen_text::get_line_height_v()
{
	return get_descender_v() - get_ascender_v();
	//return get_ascender_v() - get_descender_v();
}


/*!
*/
float spout_pen_text::get_capheight()
{
	set_mode_text();
	return axes.Y.surface_from_dev_step(
		curr_font()->get_capheight()
	);
}

/*!
*/
float spout_pen_text::calc_string_width( str0 s)
{
	set_mode_text();
	return axes.X.surface_from_dev_step(
		curr_font()->calc_string_width(s)
	);
}

/*!
*/
void spout_pen_text::set_text_pos( float x, float y )
{
	/*
		must not be in 'path' mode, so clear any previous mode
	*/
	goto_mode( spout_PAGE_base::mode_none );
	SPOUT_set_text_pos(
		spout,
		axes.X.dev_from_surface(x),
		axes.Y.dev_from_surface(y)
	);
}

/*!
*/
void spout_pen_text::xy_left( float x, float y, str0 str_8859 )
{
	set_text_pos( x, y );
	show( str_8859 );
}

/*!
*/
void spout_pen_text::xy_right( float x, float y, str0 str_8859 )
{
	float w = calc_string_width( str_8859 );
	set_text_pos( x-w, y );
	show( str_8859 );
}

/*!
*/
void spout_pen_text::xy_mid( float x, float y, str0 str_8859 )
{
	float w = calc_string_width( str_8859 );
	set_text_pos( x-(w/2.0), y );
	show( str_8859 );
}

/*!
*/
void spout_pen_text::show( str0 str_8859 )
{
	set_mode_text();
	// goto_mode( spout_PAGE_base::mode_text_fill );
	SPOUT_show( spout, (STR0) str_8859 );
}

/*!
*/
void spout_pen_text::continue_text( str0 str_8859 )
{
	set_mode_text();
	goto_mode( spout_PAGE_base::mode_text_fill );
	SPOUT_continue_text( spout, (STR0) str_8859 );
}


///////////////////////////////////


/*!
*/
void spout_pen_draw::moveto( float x, float y )
{
	// could be line or fill mode ...
	SPOUT_moveto(
		spout,
		axes.X.dev_from_surface(x),
		axes.Y.dev_from_surface(y)
	);
}

/*!
*/
void spout_pen_draw::lineto( float x, float y )
{
	// could be line or fill mode ...
	SPOUT_lineto(
		spout,
		axes.X.dev_from_surface(x),
		axes.Y.dev_from_surface(y)
	);
}

/*!
*/
void spout_pen_draw::line( float x1, float y1, float x2, float y2 )
{
	goto_mode( spout_PAGE_base::mode_line );
	moveto( x1, y1 );
	lineto( x2, y2 );
}

/*!
*/
void spout_pen_draw::fill( float x1, float y1, float x2, float y2 )
{
	goto_mode( spout_PAGE_base::mode_fill );
	moveto( x1, y1 );
	lineto( x2, y1 );
	lineto( x2, y2 );
	lineto( x1, y2 );
	lineto( x1, y1 );
}


