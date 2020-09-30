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
#include "X_FontStr.h"
#include "WAX_argv_decoder.h" // they are all very similar

#include "dgb.h"
//
#include "test_X_one.h"
#include "test_X_two.h"

// tutorials, docs, links
// https://www.x.org/releases/current/doc/libX11/libX11/libX11.html
//	https://tronche.com/gui/x/xlib/graphics/font-metrics/
//
// X Get Image
// X Put Image
// set_pixel( x, y, VAL )
// set_row_of_pixels( maybe )
// want a local it to take value from FT_bitmap or grey16 or RGBA ARGB [u8]
// that gets FreeType to X11's GL or my GL, it might have RGBA definitions
// shader script BLIT_via_FILTER letter_to_word multi_part_word words block
// shader script BORDERS TABS ANIMS LAYOUT ladders headers panels
// tree of data matches tree of panels (line breaks, other new info)
// SET item == type_set_this_item // notice how "this item" evaporated in ROM
// layout += GAP + WORD + GAP + ...
// layout GAP . asked_for_padding_by_text // (x, y) in EM_units _factors



using namespace WAX;

/*
class X_test_box : public X_Window
*/

int main_loop_once( X_Display & disp, XEvent & report )
{
	disp.process_event( report );
	return 0;
}

// there is a main_two

int main_one() {
	// open up a display connection
	X_Display disp( NULL );
	X_Window::register_root( disp, "R-O-O-T" );

	// pick a rectangle
	A_Rectangle xywh1( 0, 0, 500, 500 );
//	A_Rectangle xywh2( 100, 10, 150, 150 );
	A_Rectangle xywh3( 30, 250, 150, 150 );

	// create a window on the display
	X_test_box win1( "win1", disp, xywh1, 0 );

	// TODO near here
	const char * font_str = "6x10" ;
	font_str = "8x16" ;
// NO	font_str = "16x32" ;
	font_str = "-bitstream-bitstream charter-*-r-*-*-*-*-*-*-p-*-*-*";
	// font_str = "12x24"; // does not start with -


	font_str = "-bitstream-bitstream charter-*-r-*-*-*-*-*-*-p-*-*-*";
	font_str = "-adobe-courier-*-*-*-*-*-*-*-*-*-*-*-*";
	font_str = "-adobe-courier-medium-r-normal--18-180-75-75-m-120-iso8859-1";
	font_str = "-sony-fixed-medium-r-normal--24-170-100-100-c-120-iso8859-1";
	font_str = "-adobe-helvetica-medium-r-normal--0-0-0-0-p-0-iso8859-1";
	X_FontStr font_strs;
	if(font_strs.parse( font_str )) {
		PASS("parsed font into %s",
			font_strs.join() );
		font_strs.print_fields();
	} else {
		FAIL_FAILED();
	}
	win1.draw_green.XSetFont( (STR0) font_strs.join() );

//	// create a window within the window
// 	X_test_box win2( "win2", & win1, xywh2, 0 );
///	X_test_box win3( "win3", & win1, xywh3, 0 );

//	// create a window within the window
	X_test_box win3( "win3", & win1, xywh3, 0 );

	win1.map();
//	win2.map();
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


// there is a main_one
int main_two( argv_decoder & ARGS ) {
	// open up a display connection
	X_Display disp( NULL );
	X_Window::register_root( disp, "R-O-O-T" );

	// pick a rectangle
	A_Rectangle xywh1( 0, 0, 500, 500 );
	A_Rectangle xywh2( 100, 10, 750, 750 );
	A_Rectangle xywh3( 30, 250, 150, 150 );

	// create a window on the display
	X_test_two win1( "topwin", disp, xywh1, 0 );

	// TODO near here


	X_FontStr font_strs;
	font_strs.set_courier();
	font_strs.set_helvetica();
	font_strs.set_point(72);
	font_strs.set_point(18);
	font_strs.set_point(36);

	font_strs.print_fields();
	win1.draw_green.XSetFont( (STR0) font_strs.join() );

//	// create a window within the window

	win1.map();
	win1.XSelectInput( ExposureMask | KeyPressMask   | ButtonPressMask |ResizeRequest );
	
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

////////////////////////////////////////////

/*
	This is the basis of a C++ wrapper for lib W11 and libX11
*/
int main( int argc, char ** argv, char ** envp ) {
	WAX:: argv_decoder ARGS ( argc, argv, envp );
	return main_two( ARGS );
}

