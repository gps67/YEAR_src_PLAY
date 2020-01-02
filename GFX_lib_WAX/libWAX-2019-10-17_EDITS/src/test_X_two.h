#ifndef test_X_two
#define test_X_two

// #include <stdlib.h>
// #include <string.h>

// #include "X_STUBS.h"
// #include "A_point_plus.h"
// #include "X_Window.h"
// #include "X_Draw.h"
// #include "X_Display.h"
// #include "X_FontStr.h"
// #include "WAX_argv_decoder.h" // they are all very similar

// #include "dgb.h"

using namespace WAX;

// extern void e_print( const char * fmt, ... );


static const char * colour_spec_yellow = "#FFFF00";
static const char * colour_spec_purple = "#FF00FF";


/*!
	an X_test_two is a sub-widget

	Its parent can be a DISPLAY or a WIDGET

	It has a drawing GC

*/
class X_test_two : public X_Window
{

// static const char * colour_spec_green = "#00FF00";
// static const char * colour_spec_blue  = "#0000FF";

 public:
	X_Draw draw_green; // draw_green.fg = green // yellow

	A_Rectangle xywh1;	// the inner frame // eg when this is border

	/*!
		constructor onto a DISPLAY

	  X_test_two(
		const char * _name,
		X_Display & disp_,	X_DISPLAY can have VTBL X_Panels
		A_Rectangle xywh,	ZERO_is_DISPLAY_SCREEN_TOP_LEFT_ZERO
		int border
	  )

	*/
	X_test_two(
	 const char * _name,
	 X_Display & disp_,
	 A_Rectangle xywh,
	 int border
	)
	: X_Window( _name, disp_, xywh, border )
	, draw_green( *this )
	, xywh1( xywh )
	{
		xywh1.reduce2(1);
		XColor green_col = disp->cmap.Parse_Alloc( colour_spec_yellow );
		draw_green.set_fg( green_col );
		set_title( _name ); // second name
	}

	/*!
	*/
	X_test_two(
	 const char * _name,	// _dgb_ AND X11 find window by name
	 X_Window * parent,	// child of window has no title
	 A_Rectangle xywh,
	 int border
	 )
	: X_Window( _name, parent, xywh, border )
	, draw_green( *this )
	, xywh1( 0,0, xywh.width, xywh.height )
	{
		xywh1.reduce2(1);
		XColor blue_col = disp->cmap.Parse_Alloc( colour_spec_purple );
		draw_green.set_fg( blue_col );
	}

/*
*/

	void INFO_report( A_Rectangle & xywh ) {
		e_print("'%s' (%d,%d)+(%d,%d) EA_this = %p\n",
		 name,
		 xywh.x,
		 xywh.y,
		 xywh.width,
		 xywh.height,
		 this
		);
		// e_print( -ditto- );
		// a second rectangle inside the first
	}

	void event_expose( A_Rectangle & xywh )
	{
		INFO_report(xywh);

		// xywh1 is this window on parent // copy it 
		A_Rectangle xywh2 = xywh1;
		xywh2.reduce4( 50 );
		// a point inside, top left ish
		A_Point xy3( xywh2.x+10, xywh2.y+20 );
		const char * str = "abc\ndef";
		str = "String1";
		draw_green.XDrawRectangle( xywh1 );
		draw_green.XDrawRectangle( xywh2 );
		draw_green.XDrawString( xy3, str );
		draw_green.XDrawLine(
			xywh1.x,
			xywh1.y,
			xywh1.x_last(),
			xywh1.y_last()
		);
		draw_green.XDrawLine(
			xywh1.x_last(),
			xywh1.y,
			xywh1.x,
			xywh1.y_last()
		);

		/*
			this shows a stupidity with X11 XDrawRectangle
			the number of pixels drawn is width+1 height+1
			-1	unisgned means 64K wide
			0	*
			1	**
			2	*_*
			3	*__*
		*/

		int x= 31;

		A_Rectangle xywh7( x+=5, 30, 0, 0 );
		draw_green.XDrawRectangle( xywh7 );

		A_Rectangle xywh5( x+=5, 30, 1, 1 );
		draw_green.XDrawRectangle( xywh5 );

		A_Rectangle xywh4( x+=5, 30, 2, 2 );
		draw_green.XDrawRectangle( xywh4 );

		A_Rectangle xywh6( x+=5, 30, 3, 3 );
		draw_green.XDrawRectangle( xywh6 );
	}
};

#endif
