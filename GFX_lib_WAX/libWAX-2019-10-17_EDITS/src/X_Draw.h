#ifndef X_Draw_H
#define X_Draw_H

// #include "X_STUBS.h"
#include "X_ret_err.h"

namespace WAX {


/*!
	X_Draw is the basic drawing context (with GC)

	I'm not happy with this layout, as it creates a GC,
	maybe it should hold a GC created elsewhere.

		GC contains clipping ? - not sure, it can do, does it?
		GC can be shared within display - yes - bit clipping!
		GC fragments and combinations
*/
struct X_Draw
{
	Display * display;
	Window window;
	GC gc;

	/*!
		copy the X_Draw
	*/
	X_Draw( X_Draw & D )
	: display( D.display )
	, window( D.window )
	, gc( D.gc )
	{
	}

	/*!
		allocate a new GC for the window

		TODO choose syntax field = value or field(value) PICK VIEW
	*/
	X_Draw( X_Window & W )
	{
		display = W.display;
		window = W.window;
		gc = W.CreateGC();
	}

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
		::XDrawLine( display, window, gc, x1, y1, x2, y2 );
	}

	/*!
	*/
	void XDrawRectangle( int x, int y, int w, int h )
	{
		::XDrawRectangle( display, window, gc, x, y, w, h );
	}

	/*!
	*/
	void XDrawRectangle( A_Rectangle xywh )
	{
		::XDrawRectangle( display, window, gc, xywh.x, xywh.y, xywh.width, xywh.height );
	}

	/*!
	*/
	void XDrawString( A_Point xy, const char * str )
	{
		::XDrawString( display, window, gc, xy.x, xy.y, str, strlen(str) );
	}

};

}; // NAMESPACE
#endif
