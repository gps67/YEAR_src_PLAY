// test3
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

int main_loop_once( X_Display & disp, XEvent & event )
{
	disp.process_event( event );
	return 0;
}

// there is a main_two

int main_two( argv_decoder & ARGS ) {

	// open up a display connection
	X_Display disp( NULL );

	// X_Window * X_Window_ROOT =
	X_Window::register_root( disp, "R-O-O-T" );

	// pick a rectangle, for initial size request
	A_Rectangle xywh1( 0, 0, 500, 500 );
	A_Rectangle xywh2( 100, 10, 750, 750 );
	A_Rectangle xywh3( 30, 250, 150, 150 );

	// create a window on the display
	// xywh is the paper not the window
	// xywh3 show border not matching contenets
	buffer1 buf;
	X_test_two win1( ARGS.get_prog_name(buf), disp, xywh1, 0 );

	// TODO near here


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

	// decide to show the window
	win1.map();

	// pick what events we will get
	long mask = 0;
	mask |= ExposureMask ;
	mask |= KeyPressMask ;
	mask |= ButtonPressMask ;
	mask |= ButtonReleaseMask; // Pointer button up 
//	mask |= ResizeRequest ; // NOT SURE WHERE THIS CAME FROM
	mask |= ResizeRedirectMask; // Redirect resize of this window

/*

	mask = NoEventMask; // No events wanted

	mask |= KeyPressMask; // Keyboard down 
	mask |= KeyReleaseMask; // Keyboard up 
	mask |= ButtonPressMask; // Pointer button down 
	mask |= ButtonReleaseMask; // Pointer button up 
	mask |= EnterWindowMask; // Pointer window entry 
	mask |= LeaveWindowMask; // Pointer window leave 

	mask |= PointerMotionMask; // Pointer motion 
	mask |= PointerMotionHintMask; // Pointer motion hints 
	mask |= Button1MotionMask; // Pointer motion while button 1 down
	mask |= Button2MotionMask; // Pointer motion while button 2 down
	mask |= Button3MotionMask; // Pointer motion while button 3 down
	mask |= Button4MotionMask; // Pointer motion while button 4 down
	mask |= Button5MotionMask; // Pointer motion while button 5 down
	mask |= ButtonMotionMask; // Pointer motion while any button down

	mask |= KeymapStateMask; // Keyboard state at window entry and focus in
	mask |= ExposureMask; // Any exposure 
	mask |= VisibilityChangeMask; // Any change in visibility 
	mask |= StructureNotifyMask; // Any change in window structure 
	mask |= ResizeRedirectMask; // Redirect resize of this window
	mask |= SubstructureNotifyMask; // Substructure notification 
	mask |= SubstructureRedirectMask; // Redirect structure requests on children
	mask |= FocusChangeMask; // Any change in input focus 
	mask |= PropertyChangeMask; // Any change in property 
	mask |= ColormapChangeMask; // Any change in colormap 
	mask |= OwnerGrabButtonMask; // Automatic grabs should activate with owner_events set to True
*/

	win1.XSelectInput( mask );
	win1.X_Raise_Window();
	

	// KEY grab - eg NP_Add could become a TSR hotkey
if(1)	disp.test1(); // what does this do ??? // global grab of + key
	// INFO says name of window is "R-O-O-T" or "test3.elf"

	/*
		X11 event loop
		XFlush sends all queued messages
		XNextEvent gets 1 event
		main_loop_once calls disp.process_event( event );

		nb disp handles the event
		it finds the relevent X_Window
		which is a field of the event
		and looks it up in HT( int -> OBJ* )

		APP event loop would also select on sockets
		maybe even select on X11 sockets
	*/
	XEvent event;
	while (1)  {
		disp.XFlush();
		disp.XNextEvent( event );
		disp.process_event( event );
	//	int t =  main_loop_once( disp, event );
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

