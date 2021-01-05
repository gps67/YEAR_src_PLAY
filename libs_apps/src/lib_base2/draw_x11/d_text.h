#ifndef d_text_H
#define d_text_H

#include "obj_hold.h"
#include "w_widget0.h"


/*!
	d_text is a text drawing GC and surface.

	Y increases downwards (as in pixmap, opposite of PostScript)
	however caller can align within text, eg goto_y_tick(ypos)

	It takes the default font from its widget
*/
class d_text : public GRP_lib_base2_x11
{
 public:
	// maybe this should name the points

	GdkDrawable * drawable;
	GdkFont * font;
	GdkGC * gc;

	int gap_above;
	int gap_below;
	void set_gap_above( int gap );
	void set_gap_below( int gap );

        int text_lbearing;
        int text_rbearing;
        int text_width;
        int width1;	// width of '0' with trailing gap
        int text_ascent;
        int text_descent;
        int font_ascent;
        int font_descent;

	int font_line_pre_gap; // descender makes caps look clear
	int font_line_height;
	int font_padded_single_line_height;

	d_text( w_widget0 * wid );
	d_text( GdkDrawable * _drawable, GdkFont * _font, GdkGC * _gc );
	d_text();
	d_text( d_text & src );

	void string_extents( const char * str );
	//! The pixel width of the string
	int string_width( const char * str )
	{
		return gdk_string_width( font, str ) - 1; // no following text ?
	}
	//! The pixel width of the char
	int char_width( char c )
	{
		return gdk_char_measure( font, c );
	}
	//! select a new gc (colour etc)
	void set_gc( GdkGC * _gc )
	{
		gc = _gc;
	}
	void set_font( GdkFont * font );
	//! select a new drawable
	void set_drawable( GdkDrawable * _drawable )
	{
		drawable = _drawable;
	}
	int y_baseline;

	/*
		having placed text (set baseline) get positions
	*/

	int get_y_over();
	int get_y_top();
	int get_y_ascent();
	int get_y_tick();
	int get_y_crossout();
	int get_y_baseline();
	int get_y_descent();
	int get_y_beneath();

	int get_step_height();

	/*
		get positions relative to baseline
	*/

	int get_BL_dy_over();
	int get_BL_dy_top();
	int get_BL_dy_ascent();
	int get_BL_dy_tick();
	int get_BL_dy_crossout();
	int get_BL_dy_baseline();
	int get_BL_dy_descent(); //  { return 0; }
	int get_BL_dy_beneath();

	void goto_y_over( int _y ); 
	void goto_y_top( int _y ); 
	void goto_y_tick( int _y ); 
	void goto_y_baseline( int _y ); 

	void draw_x_str( int _x, const char * str );
	void draw_xmid_str( int _x, const char * str );
	void draw_xright_str( int _x, const char * str );

	void line( int x1, int y1, int x2, int y2 );
	void test1();
	void test1_markup( int x1, int rx, int ry );
};

#endif
