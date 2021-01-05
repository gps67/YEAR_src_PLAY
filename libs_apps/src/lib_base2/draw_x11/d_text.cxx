#include "d_text.h"
#include <string.h>
#include "d_gc.h"

/*
	over		outside of text, outside of gap
	ascent		outside of text, inside gap
	tick
	crossout
	baseline	text sits just over the baseline - usually the ORIGIN
	descent		outside of text, inside of gap
	beneath		outside of text, outside of gap
	step_height	between lines 

	If you draw a line along the baseline, the text sits on top of it.

	If you draw a line along y_descent, the text sits above it.

	If you draw a line along y_ascent, it sits on top of the text.

	y_over and y_beneath (if drawn as lines) are outside of the gap,
	and inside the adjacent object. The first and last dots within the
	area are 1 pixel away.


	I think that my interpretation of gtk's interpretation of layout
	is slightly off. There should be a FONT ascent and a CHAR ascent,
	maybe for this font they are the same.

	I am subtracting 1 from the ascent (to get the above) so that the
	gap between lines is zero. That makes centralisation better,
	(and allows smallest buttons), but doesnt give a recommended
	gap_size between lines (other than the 1 subtracted).

	The formulae for tick and crossout are wrong, but OK for now.
	Crossout sits just below the horizontal part of 'e'.  Tick on.

TODO

	more work required for multi-font in 1 line
	more work required for multi-line - varying font
	... similar set of layout ints for LINE ALIGNMENT LAYOUT

ALSO

	gap_above gap_below - do they overlap - maybe they should,
	merged by align.
*/

// d_text::d_text( w_widget0 * wid, y_alignment _y_align )
/*!
	constructor - take the widgets default font, drawable and gc_black
*/
d_text::d_text( w_widget0 * wid )
{
	/*
		initialise with default drawable, gc, font and gaps
	*/
	font = NULL;
	gap_above = 0;
	gap_below = 0;
	gc = wid->w->style->black_gc;
#ifdef WITH_GTK2
	set_font( gtk_style_get_font( wid->w->style ) );
#else
	set_font( wid->w->style->font );
#endif
	/*
		caller provides virtual function get_drawable();
	*/
	set_drawable( wid->get_drawable() ); 
}

/*!
	constructor - setting more parameters than usual
*/
d_text::d_text( GdkDrawable * _drawable, GdkFont * _font, GdkGC * _gc )
{
	font = NULL;
	gap_above = 0;
	gap_below = 0;
	set_gc( _gc );
	set_font( _font );
	set_drawable( _drawable );
}

/*!
	constructor - but requires subsequent settings - AVOID
*/
d_text::d_text()
{
	font = NULL;
	gc = NULL;
	drawable = NULL;
}

/*!
	constructor - copy to be the same (LURK ref_counts)
*/
d_text::d_text( d_text & src )
{
	*this = src;
}

/*!
	HMMM ...
*/
void d_text::set_gap_above( int gap ) // spoils any previous goto_y
{
	if( gap > 0 )
		gap_above = gap;
}

/*!
	HMMM ...
*/
void d_text::set_gap_below( int gap )
{
	if( gap > 0 )
		gap_below = gap;
}

/*!
	Position so that _y is above the top of the font
*/
void d_text::goto_y_over( int _y )
{
	goto_y_baseline( _y - get_BL_dy_over() );
}

/*!
	Position so that _y is the top of the font
*/
void d_text::goto_y_top( int _y )
{
	goto_y_baseline( _y - get_BL_dy_top() );
}

/*!
	Position so that _y is the suitable for y-axis ticks
*/
void d_text::goto_y_tick( int _y )
{
	goto_y_baseline( _y - get_BL_dy_tick() );
}

/*!
	Position so that _y is the bottom of the font
*/
void d_text::goto_y_baseline( int _y )
{
	y_baseline = _y;
}

///////////////////

/*
*/

int d_text::get_BL_dy_over()
{
	return 0 - font_ascent - gap_above ;
}

int d_text::get_BL_dy_top()
{
	return 0 - font_ascent - gap_above + 1 ;
}

int d_text::get_BL_dy_ascent()
{
	return 0 - font_ascent;
}

int d_text::get_BL_dy_tick()
{
	return 0 - font_ascent/2 ;
}

int d_text::get_BL_dy_crossout()
{
	return 0 - ( 2 * font_ascent / 5 );
}

int d_text::get_BL_dy_baseline()
{
	return 0;
}

int d_text::get_BL_dy_descent()
{
	return 0 + font_descent ;
}

int d_text::get_BL_dy_beneath()
{
 if( FALSE && (gap_above <= gap_below ))
	return 0 + font_descent + gap_below - 1 ; // DISCOUNT BASELINE
 else
	return 0 + font_descent + gap_below ;
}

///////////////////


/*!
	return step_height
*/
int d_text::get_step_height()
{
	return get_BL_dy_beneath() - get_BL_dy_top();
}

/*!
	return y-pos of ...
*/
int d_text::get_y_over()
{
	return y_baseline + get_BL_dy_over();
}

/*!
	return y-pos of ...
*/
int d_text::get_y_top()
{
	return y_baseline + get_BL_dy_top();
}
/*!
	return y-pos of ...
*/
int d_text::get_y_ascent()
{
	return y_baseline + get_BL_dy_ascent();
}
/*!
	return y-pos of ...
*/
int d_text::get_y_tick()
{
	return y_baseline + get_BL_dy_tick();
}
/*!
	return y-pos of ...
*/
int d_text::get_y_crossout()
{
	return y_baseline + get_BL_dy_crossout();
}
/*!
	return y-pos of ...
*/
int d_text::get_y_baseline()
{
	return y_baseline + 0 ;
}
/*!
	return y-pos of ...
*/
int d_text::get_y_descent()
{
	return y_baseline + get_BL_dy_descent();
}
/*!
	return y-pos of ...
*/
int d_text::get_y_beneath()
{
	return y_baseline + get_BL_dy_beneath();
}

////////////////////////

/*!
	measure layout of str
*/
void d_text::string_extents( const char * str )
{
        gdk_string_extents(
                font,
                str,
                & text_lbearing,
                & text_rbearing,
                & text_width,
                & text_ascent,
                & text_descent
        );
}

/*!
	switch to a new font
*/
void d_text::set_font( GdkFont * _font )
{
	if( !font ) {
		font_ascent = 0;
		font_descent = 0;
	}
	// int old_ascent = font_ascent;
	// int old_descent = font_descent;
	font = _font;
	if(!font) return;

	font_ascent = font->ascent -1; // ascent usually includes a gap
	font_descent = font->descent; // descent does NOT include the baseline
	width1 = char_width( '0' );

	font_line_pre_gap = font_descent; // descender makes caps look clear
	font_line_height = font_ascent + font_descent;
	font_padded_single_line_height = font_line_pre_gap + font_line_height;

	if(0) e_print("font ascent.descent %d/%d %s\n",
		font_ascent,
		font_descent,
		"--" ); // 	gdk_font_full_name_get(font));
}

/*!
	draw str starting at x
*/
void d_text::draw_x_str( int x, const char * str )
{
	int str_len = strlen(str);
	gdk_draw_text( drawable, font, gc, x, y_baseline, str, str_len );
}

/*!
	draw str centered on x
*/
void d_text::draw_xmid_str( int x, const char * str )
{
	int xleft = x - string_width( str ) / 2;
	int str_len = strlen(str);
	gdk_draw_text( drawable, font, gc, xleft, y_baseline, str, str_len );
}

/*!
	draw str right aligned on x
*/
void d_text::draw_xright_str( int x, const char * str )
{
	int xleft = x - string_width( str );
	int str_len = strlen(str);
	gdk_draw_text( drawable, font, gc, xleft, y_baseline, str, str_len );
}

/*!
	sneaky side route to gc of text, drawable, same axis
*/
void d_text::line( int x1, int y1, int x2, int y2 )
{
	gdk_draw_line( drawable, gc, x1, y1, x2, y2 );
}

#define H_LINE( y, str ) \
	if(ANN&1) \
		text2.set_gc( gc1.gc ); \
	else \
		text2.set_gc( gc2.gc ); \
	text2.line( x1, y, x3[ANN], y ); \
	text2.line( x3[ANN], y, x4[ANN], y4[ANN] ); \
	text2.goto_y_tick( y4[ANN] ); \
	text2.draw_x_str( x4[ANN], str ); \
	ANN++; \
 //

void d_text::test1_markup( int x1, int rx, int ry )
{
	int x2 = x1 + rx;
	int y2 = y_baseline + ry;
	d_text text2 ( *this );
	d_gc gc1( drawable );
	d_gc gc2( drawable );
 	gc1.set_fg( "blue" );
	gc2.set_fg( "green" );
	const static int N_ANN=20;
	// x3 is the corner or the line
	int x3[N_ANN];
	// x4,y4 is the line end at the attotation text
	int x4[N_ANN];
	int y4[N_ANN];
	// for 20 try//  y4[i] = y2 - 50 + 12 *     i;
	// for 7 try//
	for(int i=0; i<N_ANN; i++ )
	{
		x4[i] = x2 + 50 + 10 * (10-i);
		x3[i] = (x4[i] + x2) /2;
		x3[i] = x2 + 60;
		y4[i] = y2 - 30 + 12 *     i;
	}
	int ANN = 0;

	H_LINE( get_y_over(),		"get_y_over()" );
	H_LINE( get_y_top(),		"get_y_top()" );
	H_LINE( get_y_ascent(),		"get_y_ascent()" );
	H_LINE( get_y_tick(),		"get_y_tick()" );
	H_LINE( get_y_crossout(),	"get_y_crossout()" );
	H_LINE( get_y_baseline(),	"get_y_baseline()" );
	H_LINE( get_y_descent(),	"get_y_descent()" );
	H_LINE( get_y_beneath(),	"get_y_beneath()" );
}

/*!
	unused testing calls
*/
void d_text::test1()
{
	// lines start at x1
	int x1 = 40; 
	// text start x2
	int x2 = x1 + 25;
	// text top at y1
	int y2 = 110;

	set_gap_above( 0 );
	set_gap_below( 0 );

	d_gc gc0( drawable );

	GdkGC * gc_old = gc;
	gc = gc0.gc;

	goto_y_top( y2 );
	draw_x_str( x2, "y| |First" );

	goto_y_top( get_y_beneath() );
	test1_markup( x1, 80, -30 );
	draw_x_str( x2, "y|| Second" );

	goto_y_top( get_y_beneath() );
	test1_markup( x1, 40, +80 );
	draw_x_str( x2, "y| |Third" );

	// redraw with lines behind
	goto_y_top( y2 );
	draw_x_str( x2, "y| |First" );

	gc = gc_old;
	// gc1.set_fg( "green" );

}




