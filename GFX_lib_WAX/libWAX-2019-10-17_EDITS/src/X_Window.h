#ifndef X_Window_H
#define X_Window_H

#include "X_STUBS.h"
// free // #include <stdlib.h>
// strdup // #include <string.h>
#include <X11/Xlib.h>

namespace WAX {


/*!
	child class must provide behavior functions
*/
struct X_Window
{
	X_Window * parent;
	Display * display;
	Window window;
	X_Display * disp;
	const char * name;


	/*!
		child classes use this to create the object
	*/
	X_Window(
		X_Window * _parent,	// provides display ;-)
		Window _window,		// possibly 0 or XXXX
		const char * _name	// usually set
	);

	/*!
		NULL_parent
	*/
	X_Window(
		X_Window * _parent,	// possibly NULL
		X_Display * _disp,	// must have display
		Window _window,		// possibly 0 or XXXX
		const char * _name	// usually set
	);


	/*!
		call this once
	*/
	static X_Window * register_root(
		X_Display & disp_,
		const char * _name
	);

	/*!
		every widget has a (copied string) name for debugging / other
	*/
	void set_name( const char * _name );

	/*!
		create a simple window, NB-TODO some unsigned work 
	*/
	X_Window(
		const char * _name,
		X_Display & disp_,
		A_Rectangle xywh,
		int borderwidth
	);

	/*!
		create a simple window on the parent
	
		really - should create the local X_Window object
		without a display's window.
		Then do the tree-layout,
		then INSTANCIATE the window
	*/
	X_Window(
		const char * _name,
		X_Window * parent_,
		A_Rectangle xywh,
		int borderwidth
	);


/*
		create a child-of-this sub-window
	X_Window * mk_child( const char * _name, A_Rectangle xywh, int borderwidth )
	{
		X_Window * child = new X_Window( name, this, xywh, borderwidth );
		return child;
	}
*/

	/*!
	*/
	void map()
	{
		::XMapWindow( display, window );
	}

	/*!
		create a GC for this window
	*/
	GC CreateGC()
	{
		unsigned long valuemask = 0;
		XGCValues * values = NULL;
		return ::XCreateGC( display, window, valuemask, values );
	}

	/*!
		set all (of mask) events to cause a callback

		ExposureMask - so redraw
		KeyPressMask - so act
		KeyReleaseMask - act after thought
		ButtonPressMask - mouse

	NO:

		ResizeRequest - Conly called when window is resized by another
	*/
	void XSelectInput( long event_mask )
	{
		::XSelectInput( display, window, event_mask );
	}

	virtual void event_expose( A_Rectangle & xywh ) = 0;

};

}; // NAMESPACE
#endif
