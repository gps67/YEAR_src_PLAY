// #include <stdio.h>
// #include <X11/Xlib.h>
// #include <X11/Xutil.h>
// #include <X11/Xos.h>
// #include <X11/Xatom.h>
// #include <X11/keysym.h>
// 
// #include <string.h>
// 
// #include "X_Colours.h"
// #include "A_Map_W.h"

#include <stdlib.h>
#include <string.h>

#include "X_STUBS.h"
#include "A_point_plus.h"
#include "X_Window.h"
#include "X_Draw.h"
#include "X_Display.h"

#include "dgb.h"

using namespace WAX;

extern void e_print( const char * fmt, ... );


const char * colour_spec_green = "#00FF00";
const char * colour_spec_blue  = "#0000FF";

/*!
	an X_test_box is a sub-widget

	Its parent can be a DISPLAY or a WIDGET

	It has a drawing GC

*/
class X_test_box : public X_Window
{
 public:
	X_Draw draw_green;

	A_Rectangle xywh1;	// the inner frame // eg this is border

	/*!
		constructor onto a DISPLAY

	  X_test_box(
		const char * _name,
		X_Display & disp_,	X_DISPLAY can have VTBL X_Panels
		A_Rectangle xywh,	ZERO_is_DISPLAY_SCREEN_TOP_LEFT_ZERO
		int border
	  )

	*/
	X_test_box( const char * _name, X_Display & disp_, A_Rectangle xywh, int border )
	: X_Window( _name, disp_, xywh, border )
	, draw_green( *this )
	, xywh1( xywh )
	{
		xywh1.reduce2(1);
		XColor green_col = disp->cmap.Parse_Alloc( colour_spec_green );
		draw_green.set_fg( green_col );
	}

	/*!
		constructor inside a FRAME

	  X_test_box(
	   const char * _name,
	     X_Window * parent, // Frame is a VTBL X_WINDOW _FRAME
	   A_Rectangle  xywh,	// xywy ZERO is X_WINDOW_BASE_ZERO_plus_OFFS
	            int border  // x11 gives us this useful border // nobacking
	  )

	   // border += nobacking_redraw_on_demand pixel_box_border
	   // SUBLEX gets excited at the line above,
	   // SUBLEX attempt to place Oxford Commas in Lists within identifiers
	   // SUBLEX attempt_to_place_Oxford_Commas_in_Lists_within_identifiers
	   // SUBLEX detects PSG sequence, using JOIN was "_"
	   // REGEN SUBLEX can plant machine settings in comments, in segment
	   // REGEN SUBLEX can pick up machine settings from comments, in segment
	   // SUBLEX SAYS //

		   _nobacking_
		   _redraw_on_demand_
		   _pixel_box_border_

	   // REGEN SUBLEX can write out machine settings as DIAG_Parameters
	   // DIAG_PARAMETER = CODE_n_DATA = classNname * VAR_name = ACCESS()
	   // REGEN SUBLEX can ANNOTATE up machine settings from comments
	   // REGEN SUBLEX can ANNOTATE up machine settings from comments
	  )
	*/
	X_test_box( const char * _name, X_Window * parent, A_Rectangle xywh, int border )
	: X_Window( _name, parent, xywh, border )
	, draw_green( *this )
	, xywh1( 0,0, xywh.width, xywh.height )
	{
		xywh1.reduce2(1);
		XColor blue_col = disp->cmap.Parse_Alloc( colour_spec_blue );
		draw_green.set_fg( blue_col );
	}

/*
 CODE REQUIRED

 	XYWH has (some sort of) ZERO
	ZERO has
		a SURFACE, a HOTSPOT(token) on surface for ZERO
		a DPI, alternatively a MATRIX_Transformer // rotates extras
		a expr decoder = CTXT_SCRIPT_expr_decoder
		a TOKEN and its collection of idx_exp and item_expr
		ZERO means ZERO
	
	This is a major handrail, STRONG TYPE checking

		CT fast_track pre_built derived_by_SCRIPT

			yes it is a security nightmare
			SCRIPT generating C code 
			 via FILTER = USER_DATA_ACCESS_permit_SCRIPTS

			So start off with the minimum calls to unwritten code
			There will still be idx = lookup( TOKEN ) # expanded
			There will still be u32_hilo idx = u32_TOKEN; # compiled
			Reintroduce as // u32_hilo idx = lookup_token( u32_TOKEN) ; # compiled
			// const within PREBUILT
	
	--COMMENTS-- also discussing TOKENISATION of SCRIPTS
	--COMMENTS-- also discussing SAFE RUN COMPILED ASM


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

		A_Rectangle xywh2 = xywh1;
		xywh2.reduce4( 50 );
		// a point inside, top left ish
		A_Point xy3( xywh2.x+10, xywh2.y+10 );
		const char * str = "abc\ndef";
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

int main_loop_once( X_Display & disp, XEvent & report )
{
	disp.process_event( report );
}

/*
	This is the basis of a C++ wrapper for lib W11 and libX11
*/

int main() {
	// open up a display connection
	X_Display disp( NULL );
	X_Window::register_root( disp, "R-O-O-T" );

	// pick a rectangle
	A_Rectangle xywh1( 0, 0, 500, 500 );
//	A_Rectangle xywh2( 100, 10, 150, 150 );
	A_Rectangle xywh3( 30, 250, 150, 150 );

	// create a window on the display
	X_test_box win1( "win1", disp, xywh1, 0 );

//	// create a window within the window
// 	X_test_box win2( "win2", & win1, xywh2, 0 );
///	X_test_box win3( "win3", & win1, xywh3, 0 );

//	// create a window within the window
	X_test_box win3( "win3", & win1, xywh3, 0 );

	win1.map();
	win1.XSelectInput( ExposureMask | KeyPressMask   | ButtonPressMask |ResizeRequest );
//	win2.XSelectInput( ExposureMask | KeyPressMask   | ButtonPressMask |ResizeRequest );
	win3.XSelectInput( ExposureMask | KeyReleaseMask | ButtonPressMask |ResizeRequest );
	// win1.draw();
	
	// KEY
	disp.test1();

/*
	when win3 is child of win1
	KeyPress is received
*/

	XEvent report;
	while (1)  {
		disp.XFlush();
		disp.XNextEvent( report );
		int t =  main_loop_once( disp, report );
	}

	return 0;
}

