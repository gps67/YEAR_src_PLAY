#ifndef X_Draw_H
#define X_Draw_H

// #include "X_STUBS.h"
#include "X_ret_err.h"
#include "X_Drawable_Surface.h"
#include "X_Pixmap.h"

namespace WAX {


/*!
	X_Draw is the basic drawing context (with GC)

	I'm not happy with this layout, as it creates a GC,
	maybe it should hold a GC created elsewhere.

		GC contains clipping ? - not sure, it can do, does it?
		GC can be shared within display - yes - bit clipping!
		GC fragments and combinations
	
	An X_Draw is kind of an X_Drawable += GC += Display

		see X_Drawable_Surface already += Display
*/
struct X_Draw
{
	Display * display;	// TODO X_Display
	Drawable drawable;
	GC gc;			// TODO flag = I_own_this_GC
//	A_WH WH;		// duplicate ??

	/*!
		copy the X_Draw
	*/
	X_Draw( X_Draw & D )
	: display( D.display )
	, drawable( D.drawable )
	, gc( D.gc )		// share ownership of GC
	{
	}

	/*!
		allocate a new GC for the window
	
		TODO choose syntax field = value or field(value) PICK VIEW
	*/
	X_Draw( X_Drawable_Surface & W )
	{
		display = W.display;
		drawable = W.drawable;
		gc = W.CreateGC();
	}

	/*!
		A Pixmap is like a Window
	*/
	X_Draw( X_Drawable_Surface & P, GC _gc )
	{
		display =  P.display; // P.display
		drawable = P.drawable;
		gc = _gc; // gc = W.CreateGC();
	}

	/*!
		A Drawable
	X_Draw( Display * _display, Drawable _drawable, GC _gc )
	{
		display = _display;	 // P.display
		drawable = _drawable;
		gc = _gc;		 // gc = W.CreateGC(); or not
	}
	*/

	/*!
	*/
	void set_fg( XColor colour )
	{
		XSetForeground( colour );
	}

	/*!
	*/
	void XSetForeground( XColor colour )
	{
		::XSetForeground( display, gc, colour.pixel );
	}

	/*!
	*/
	bool XSetFont( Font font )
	{
		int ret = ::XSetFont( display, gc, font );
		if(ret) return FAIL("ret == %d %s (as str)", ret, name_of_X_err(ret) );
		PASS("ret == %d %s (as str)", ret, name_of_X_err(ret) );
		return true;
	}

	/*!
	*/
	bool XSetFont( const char * str )
	{
		INFO("str %s", str );
		XFontStruct * 
		font = ::XLoadQueryFont( display, str );
		if(!font) {
			return FAIL("::XLoadQueryFont( display, '%s'", str );
		}

		int ret = ::XSetFont( display, gc, font->fid );
		if(ret) return FAIL("ret == %d %s (as str)", ret, name_of_X_err(ret) );
		return true;
	}

	/*!
	*/
	void XDrawLine( int x1, int y1, int x2, int y2 )
	{
		::XDrawLine( display, drawable, gc, x1, y1, x2, y2 );
	}

	/*!
	*/
	void XDrawRectangle( int x, int y, int w, int h )
	{
		::XDrawRectangle( display, drawable, gc, x, y, w, h );
	}

	/*!
	*/
	void XDrawRectangle( A_Rectangle xywh )
	{
		::XDrawRectangle( display, drawable, gc, xywh.x, xywh.y, xywh.width, xywh.height );
	}

	/*!
	*/
	void XDrawString( A_Point xy, const char * str )
	{
		::XDrawString( display, drawable, gc, xy.x, xy.y, str, strlen(str) );
	}

};

}; // NAMESPACE
#endif
