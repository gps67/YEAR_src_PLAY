#include "pdf_pen_base.h"
#include "pdf_base.h"

using namespace PDF_GEN;

// pdf_pen_base::


pdf_pen_base::pdf_pen_base(
	pdf_base * _pdf
)
: Style_Tray( _pdf )
, axes( pdf->axes )
{
}

/*!
	creating one pen from another pen, is usually followed
	by setting some different style or axes.

	It is allowed to change things during the initialisation
	phase, but after that ... at least call unset_current()
*/
pdf_pen_base::pdf_pen_base(
	pdf_pen_base * _base
)
: Style_Tray( _base->pdf )
, axes( _base->axes )
{
	// axes = _base->axes;
	axes.set_name("PEN");
}



///////////////////////////////////

float pdf_pen_text::get_text_x()
{
	return axes.X.surface_from_dev( PDF_get_value( pdf->pdf, "textx", 0 ));
}

float pdf_pen_text::get_text_y()
{
	return axes.Y.surface_from_dev( PDF_get_value( pdf->pdf, "texty", 0 ));
}

/*!
*/
float pdf_pen_text::get_descender()
{
	set_mode_text();
	return axes.Y.surface_from_dev_step(
		curr_font()->get_descender()
	);
}

/*!
*/
float pdf_pen_text::get_ascender()
{
	set_mode_text();
	return axes.Y.surface_from_dev_step(
		curr_font()->get_ascender()
	);
}

// _vector is from baseline +/-
/*!
*/
float pdf_pen_text::get_descender_v()
{
	if( 0.0 > get_descender() )
	{
		e_print("get_descender_v() not posative! %6.5f\n", (float) get_descender() );
	}
	return  get_descender(); // already negative
}

/*!
*/
float pdf_pen_text::get_ascender_v()
{
	return get_ascender();
}

/*!
*/
float pdf_pen_text::get_line_height_v()
{
	return get_descender_v() - get_ascender_v();
	//return get_ascender_v() - get_descender_v();
}


/*!
*/
float pdf_pen_text::get_capheight()
{
	set_mode_text();
	return axes.Y.surface_from_dev_step(
		curr_font()->get_capheight()
	);
}

/*!
*/
float pdf_pen_text::calc_string_width( str0 s)
{
	set_mode_text();
	return axes.X.surface_from_dev_step(
		curr_font()->calc_string_width(s)
	);
}

/*!
*/
void pdf_pen_text::set_text_pos( float x, float y )
{
	/*
		must not be in 'path' mode, so clear any previous mode
	*/
	goto_mode( pdf_base::mode_none );
	PDF_set_text_pos(
		pdf->pdf,
		axes.X.dev_from_surface(x),
		axes.Y.dev_from_surface(y)
	);
}

/*!
*/
void pdf_pen_text::xy_left( float x, float y, str0 str_8859 )
{
	set_text_pos( x, y );
	show( str_8859 );
}

/*!
*/
void pdf_pen_text::xy_right( float x, float y, str0 str_8859 )
{
	float w = calc_string_width( str_8859 );
	set_text_pos( x-w, y );
	show( str_8859 );
}

/*!
*/
void pdf_pen_text::xy_mid( float x, float y, str0 str_8859 )
{
	float w = calc_string_width( str_8859 );
	set_text_pos( x-(w/2.0), y );
	show( str_8859 );
}

/*!
*/
void pdf_pen_text::show( str0 str_8859 )
{
	set_mode_text();
	// goto_mode( pdf_base::mode_text_fill );
	PDF_show( pdf->pdf, (STR0) str_8859 );
}

/*!
*/
void pdf_pen_text::continue_text( str0 str_8859 )
{
	set_mode_text();
	goto_mode( pdf_base::mode_text_fill );
	PDF_continue_text( pdf->pdf, (STR0) str_8859 );
}


///////////////////////////////////


/*!
*/
void pdf_pen_draw::moveto( float x, float y )
{
	// could be line or fill mode ...
	PDF_moveto(
		pdf->pdf,
		axes.X.dev_from_surface(x),
		axes.Y.dev_from_surface(y)
	);
}

/*!
*/
void pdf_pen_draw::lineto( float x, float y )
{
	// could be line or fill mode ...
	PDF_lineto(
		pdf->pdf,
		axes.X.dev_from_surface(x),
		axes.Y.dev_from_surface(y)
	);
}

/*!
*/
void pdf_pen_draw::line( float x1, float y1, float x2, float y2 )
{
	goto_mode( pdf_base::mode_line );
	moveto( x1, y1 );
	lineto( x2, y2 );
}

/*!
*/
void pdf_pen_draw::fill( float x1, float y1, float x2, float y2 )
{
	goto_mode( pdf_base::mode_fill );
	moveto( x1, y1 );
	lineto( x2, y1 );
	lineto( x2, y2 );
	lineto( x1, y2 );
	lineto( x1, y1 );
}


