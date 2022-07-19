#ifndef X_Display_H
#define X_Display_H

#include "X_STUBS.h"

#include "A_Map_W.h"
#include "X_Colours.h"

namespace WAX {

/*!
	X_Display_One is the basic connection to the X server.
	There is no default constructor, to prevent accidental creates,
	but NULL uses the default display_name.

	Copy constructor is C++ default
*/
struct X_Display_One
{
	Display * display;
	X_Colours cmap;
	A_Map_W map_W;	// find X_Window from { Window win } // LEAK

	/*!
		create a connection
	*/
	X_Display_One( const char * display_name )
	: cmap( NULL ) // what is the correct C++ of doing this ?
	{
		display = ::XOpenDisplay( display_name );
		cmap.late_init( display );
	}

	/*!
		send all drawing requests
	*/
	void XFlush()
	{
		::XFlush( display );
	}

	/*!
		wait for an event

		X11 can open several connections,
		need to get a non-blocking XNextEvent
		and to feed X11 using (end of chapter2)
			XAddConnctionWatch()
			conn_watch_proc()
			CProcessInternalConnection( display, fd )
	*/
	void XNextEvent( XEvent & event )
	{
		::XNextEvent( display, & event );
	}

	void add( X_Window * W )
	{
		map_W.add( W );
	}

	X_Window * find_Window( Window w )
	{
		return map_W.find( w );
	}

	static const int SCREEN_0 = 0;

	// vanilla plus // screen 1 fails on my 2 monitor laptop
	// and SCREEN_0 is opening on the second monitor, same as xterm mon
	int get_screen_0() const { return SCREEN_0; }

	Visual * get_Default_Visual() {
		return DefaultVisual( display, SCREEN_0 );
	}

	bool test_list_depths(); //
// https://tronche.com/gui/x/xlib/display/display-macros.html
// Other depths may also be supported on this screen (see .PN XMatchVisualInfo ).

	int Default_Depth() { return DefaultDepth( display, SCREEN_0 ); }

	GC Default_GC() { return DefaultGC( display, SCREEN_0 ); }

	// DefaultRootWindow
	// DefaultScreenOfDisplay
	// ScreensOfDisplay
	// DefaultScreen 	// TRY maybe using ssh second screen ? xvnc ?
	// DefaultColormap
	// DisplayCells // of cmap
	// DisplayPlanes // depth ??
	// DisplayString // kept opening string
	// int n = ScreenCount( display ) // 1
	// ServerVendor
	// VendorRelease
	//XExtendedMaxRequestSize
	// XMaxRequestSize
	// WidthOfScreen( Screen * screen )
	// HeightOfScreen( Screen * screen )
	// WidthMMOfScreen
	// HeightMMOfScreen
	// Screen *DefaultScreenOfDisplay(Display *display);

	Screen *Default_Screen_Of_Display(Display *display) {
		return  DefaultScreenOfDisplay(display);
	}

	bool guess_screen_size( A_WH & WH );




};

struct X_Display : public X_Display_One
{
	// should throw if the display is not openable
	X_Display( const char * display_name )
	: X_Display_One( display_name )
	{
	}

	/*!
		the main event handler - calls the subwindows code

		the event has been received over (transport)
	*/
	void process_event( XEvent & report );

	void test1();

	void process_events_forever();

};


}; // NAMESPACE
#endif
