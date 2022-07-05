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
	A_Map_W map_W;

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

	// vanilla plus
	int get_screen_0() { return 0; }

	Visual * get_Default_Visual() {
		return DefaultVisual( display, get_screen_0() );
	}


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
