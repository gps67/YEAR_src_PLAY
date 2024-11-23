
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



#include "ft2.h"


using namespace WAX;

// FT2 adds to WAX so that pixmaps and stuff comes from somewhere
// the WAX code is in it's original dir, symb-linked in
// all the .o files are created here and there, 
// but thats not in git, just in the build

int main_loop_once( X_Display & disp, XEvent & report )
{
	disp.process_event( report );
	return 0;
}

int main_two( argv_decoder & ARGS ) {


	// open up a display connection
	X_Display disp( NULL );
// AUTO ?
// X_Window::register_root( disp, "R-O-O-T" );

	// recently added feature

	FT2::ft2 ft;
	ft.test1();

	// pick a rectangle
	A_Rectangle xywh1( 0, 0, 500, 500 );
	A_Rectangle xywh2( 100, 10, 750, 750 );
	A_Rectangle xywh3( 30, 250, 150, 150 );

	// create a window on the display // 
	const static int border_0 = 0;
	X_test_two win1( "topwin", & disp, xywh1, border_0 );

	// TODO near here


	X_FontStr font_strs;
	font_strs.set_helvetica();
	font_strs.set_courier();
	font_strs.set_point(72); // this is working

	font_strs.print_fields();
	win1.draw_green.XSetFont( (STR0) font_strs.join() );

//	// create a window within the window

	win1.map();
	win1.XSelectInput( ExposureMask | KeyPressMask   | ButtonPressMask |ResizeRequest );
	
	INFO("CALLS disp.test1()");
	// KEY
// FIX this used to do something - but that has moved
// FIX commented out to compile 2023-12
//	disp.test1();

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
	if(1)  dgb_fork_stderr_to_tcl_text(); // only when gdb is in use ?
	WAX:: argv_decoder ARGS ( argc, argv, envp );
	return main_two( ARGS );
}

