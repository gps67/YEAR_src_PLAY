#ifndef test_X_two
#define test_X_two

// #include <stdlib.h>
// #include <string.h>

// #include "X_STUBS.h"
// #include "A_point_plus.h"
// #include "X_Window_Top_Level.h"
// #include "X_Draw.h"
// #include "X_Display.h"
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
class X_test_two : public X_Window_Top_Level
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
	: X_Window_Top_Level( _name, disp_, xywh, border )
	, draw_green( *this )
	, xywh1( xywh )
	{
		xywh1.reduce2(1);
		XColor green_col = disp->cmap.Parse_Alloc( colour_spec_yellow );
		draw_green.set_fg( green_col );
		set_title( _name ); // second name
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

	void event_expose( A_Rectangle & xywh );
};

#endif
