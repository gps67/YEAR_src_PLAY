#ifndef X_Window_H
#define X_Window_H

#include "X_STUBS.h"
#include "A_point_plus.h"

// free // #include <stdlib.h>
// strdup // #include <string.h>
#include <X11/Xlib.h>

#include "XFT.h"
#include "X_Display.h"

namespace WAX {

struct X_Drawable_Surface { // base of X_Window X_Pixmap // own spin

	Display * display;
	Drawable drawable;
	A_WH WH;

	X_Drawable_Surface(
		Display * _display,
		Drawable _drawable,
		A_WH _WH
	)
	:	display( _display )
	,	drawable( _drawable )
	,	WH( _WH )
	{
	}

	X_Drawable_Surface(
		Display * _display,
		Drawable _drawable
	)
	:	display( _display )
	,	drawable( _drawable )
	,	WH( 0,0 )
	{
	}

	/*!
		create a GC for this drawable
	*/
	GC CreateGC()
	{
		unsigned long valuemask = 0;
		XGCValues * values = NULL;
		return ::XCreateGC( display, drawable, valuemask, values );
	}

};


/*!
	child class must provide behavior functions
*/
struct X_Window : public X_Drawable_Surface
{
	X_Window * parent;
//	Window window; // base class .drawable
	X_Display * disp;
	const char * name;

	Window get_window() { return drawable; }

//	Xft_Draw xft_draw;

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
		::XMapWindow( display, get_window() );
	}

	/*!
		set all (of mask) events to cause a callback

		ExposureMask - so redraw
		KeyPressMask - so act
		KeyReleaseMask - act after thought
		ButtonPressMask - mouse

	NO:

		ResizeRequest - only called when window is resized by another

	https://tronche.com/gui/x/xlib/event-handling/XSelectInput.html
	https://tronche.com/gui/x/xlib/events/mask.html

	*/
	void XSelectInput( long event_mask )
	{
		::XSelectInput( display, get_window(), event_mask );
	}

	void set_title( const char * name );

// virtuals

	virtual void event_expose( A_Rectangle & xywh ) = 0;

};

}; // NAMESPACE
#endif
