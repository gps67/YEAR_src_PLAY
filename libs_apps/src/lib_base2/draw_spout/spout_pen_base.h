#ifndef spout_pen_base_H
#define spout_pen_base_H

#include "spout_obj_ref.h"
#include "spout_STUBS.h"

#include "spout_styles.h"
#include "axes_calc.h"

#include "spout_PAGE_base.h" // using spout -> *

namespace SPOUT_GEN {


/*!
	A pen is a single fixed style and its own axes.
	It provides drawing routines.

	The base has a single pen as its current style,
	any use of the pen installs THIS style and
	flushes the styles actually used (flushing any old).
*/
class spout_pen_base : public Style_Tray
{
 public:

	Axes_Calc &		axes;

	~spout_pen_base()
	{
	}

	spout_pen_base(
		spout_PAGE_base * _spout
	);

	spout_pen_base(
		spout_pen_base * _base
	);

	void unset_current()
	{
		spout->unset_current_pen( this );
	}

	void set_mode_text()
	{
		set_current();
		goto_mode( spout_PAGE_base::mode_text_fill );
	}

	void set_current()
	{
		spout->set_current_pen( this );
	}

	void goto_mode( spout_PAGE_base::Mode m )
	{
		set_current();
		spout -> goto_mode( m );
		set_current();
	}

};

class spout_pen_text : public spout_pen_base
{
 public:
	spout_pen_text(
		spout_PAGE_base * _base
	)
	: spout_pen_base( _base )
	{
	}


	float get_text_x() ;
	float get_text_y() ;

	float get_descender() ;
	float get_ascender()  ;
	float get_descender_v() ; // _vector is from baseline +/-
	float get_ascender_v()  ; // _vector is from baseline +/-
	float get_line_height_v();
	float get_capheight() ;
	float calc_string_width( str0 s) ;

	void set_text_pos( float x, float y );
	void show( str0 str_8859 );
	void continue_text( str0 str_8859 );
	void xy_left( float x, float y, str0 str_8859 );
	void xy_mid( float x, float y, str0 str_8859 );
	void xy_right( float x, float y, str0 str_8859 );

// DEL THIS SOON
	Style_Font_Face_Size * curr_font()
	{
		return spout->curr_font();
	}


};

class spout_pen_draw : public spout_pen_base
{
 public:
	spout_pen_draw(
		spout_PAGE_base * _base
	)
	: spout_pen_base( _base )
	{
	}

	void moveto( float x, float y );
	void lineto( float x, float y );
	void line( float x1, float y1, float x2, float y2 );
	void fill( float x1, float y1, float x2, float y2 );

	void x_line( float x0, float x1, float y0 )
	{
		line( x0, y0, x1, y0 );
	}

	void y_line( float x0, float y0, float y1 )
	{
		line( x0, y0, x0, y1 );
	}

	void y_tick( float x0, float y0, float ly, float ry )
	{
		y_line( x0, y0-ly, y0+ry );
	}

	void x_tick( float x0, float y0, float lx, float rx )
	{
		x_line( x0-lx, x0+rx, y0 );
	}

}; 

}; // NAMESPACE
#endif
