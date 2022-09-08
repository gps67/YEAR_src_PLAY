#ifndef X_Drawable_Surface_H
#define X_Drawable_Surface_H

#include "X_STUBS.h"
#include "A_point_plus.h"

#include <X11/Xlib.h>

#include "XFT.h"
#include "X_Display.h"

namespace WAX {

struct X_Drawable_Surface { // base of X_Window X_Pixmap // own spin

	Display * display; // refactor as X_Display
	Drawable drawable; // caller creates drawable then this holds it
	A_WH WH;

	#if 1 // permit simple prototype sketch design
	X_Drawable_Surface(): display(NULL), drawable(0) {}
	#endif
	X_Drawable_Surface( int temp_skim ) {}

	X_Drawable_Surface(
		X_Display * _disp,	// move to keeping shared PTR 
		Drawable _drawable,
		A_WH _WH
	)
	:	display( _disp->display )
	,	drawable( _drawable )
	,	WH( _WH )
	{
	}

	X_Drawable_Surface(
		X_Display * _disp,	// move to keeping shared PTR 
		Drawable _drawable
	)
	:	display( _disp->display )
	,	drawable( _drawable )
	,	WH( 0,0 )
	{
	}

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

	void get_WH( A_WH & _WH ) { _WH = WH; }


};

}; // namespace
#endif
