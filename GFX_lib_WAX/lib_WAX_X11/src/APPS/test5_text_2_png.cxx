// test2
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
#include "X_Display_UDEF.h"
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

int main_loop_once( X_Display * disp, XEvent & report )
{
	disp->process_event( report );
	return 0;
}

// there is a main_one
int main_two( argv_decoder & ARGS ) {
	// ARGV_DECODER // _t // ARGS_t // TEMPLATE DOT // "ALIAS" // EXPR
	// TODO(); // CMNT // ARGV_PARSER // _t // TYPES += NEW_TYPE("ARGS") // ARGV
	// KNOW // DECL // %s_t $0 // THIS_COPDE_POINT_as_EA_EXPR
	// PARSE // ARGV_PARSER // EA_NAME_by_EXPR_ARGV // ARGS // STR0 NAME //
	// EXPR // OBJ.FIELD _T = "SESS.main_two.CODE_POINT("STR0")
	// ALIAS STR0 "TYPE_CAST_STR0
	// ARGS.on_VARS _fully_loaded

	// open up a display connection
	X_Display_UDEF disp_( NULL );
	X_Display_UDEF * disp = & disp_;
	if(!disp->open_display()) {
		FAIL_FAILED();
		return 1;
	}
//	X_Window::register_root( disp, "R-O-O-T" );

	// pick a rectangle
	A_Rectangle xywh1( 0, 0, 500, 500 );
	A_Rectangle xywh2( 100, 10, 750, 750 );
	A_Rectangle xywh3( 30, 250, 150, 150 );

	// create a window on the display
	X_test_two win1( "topwin", disp, xywh1, 0 );

	// TODO near here


	// PICK FONT_one
	// plain X11 font strings (with FT point size)
	// TODO FT2 on client side not server side
	X_FontStr font_strs;
	font_strs.set_courier(); // this works
	font_strs.set_helvetica(); // this fails
	font_strs.set_point(72);
	font_strs.set_point(18);
	font_strs.set_point(8);
	font_strs.set_point(12);
	font_strs.set_point(36);

	font_strs.print_fields();
	win1.draw_green.XSetFont( (STR0) font_strs.join() );
	// no check result ?

//	// create a window within the window

	win1.map();
	win1.XSelectInput // () 
	  ( ExposureMask
	  | KeyPressMask
	  | ButtonPressMask
	  | ResizeRequest
	);
	

	// KEY
if(1)	disp->test1(); // what does this do ???

	// has to be in the redraw
	INFO("win1 USED");

/*
	when win3 is child of win1
	KeyPress is received
*/

	XEvent report;
	while (1)  {
		disp->XFlush();
		disp->XNextEvent( report );
		int t =  main_loop_once( disp, report );
	}

	return 0;
}

////////////////////////////////////////////

/*
	This is the basis of a C++ wrapper for lib W11 and libX11
*/
int main( int argc, char ** argv, char ** envp ) {
        gdb_sigaction( argv[0] ); // sets progname_argv0
 if(1)  dgb_fork_stderr_to_tcl_text(); // only when gdb is in use ?
	WAX:: argv_decoder ARGS ( argc, argv, envp );
	return main_two( ARGS );
}

