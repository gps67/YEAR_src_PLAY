#ifndef A_point_plus_H
#define A_point_plus_H
#include <X11/Xlib.h>

namespace WAX {

/*
	There are so many variations on point, rectangle, etc
	
	WAX follows X11, mostly
*/

/*!
	A_Point is an alias for XPoint (x,y) (i16,i16)
*/
struct A_Point : public XPoint
{
	/*!
	*/
	A_Point( short _x, short _y )
	{
		x = _x;
		y = _y;
	}
};

/*!
	A_XY is an alias for A_Point and XPoint (i16,i16)
*/
struct A_XY : public A_Point
{
	/*!
	*/
	A_XY( short _x, short _y )
	: A_Point( _x, _y )
	{
	}

	/*!
	*/
	A_XY( A_Point xy )
	: A_Point( xy )
	{
	}
};

/*!
	A_WH (w,h) holds width and height, cannot be negative

	nb A_WH uses i32 when X11 uses u16 sometimes signed
	so should check excess_of_u15 any_signed excess_of_

	WANT A LANG where I can edit the header, setting 

		int & h = y; // CT macro renamer
		int & w = x; // or actually set x = x_offs =  w_offs = w
	
	CHOICE here to change UNITS (eg i16 to i32)

		maybe not here, but just seen,
		OFFS = i32_value_held_in_ROM
		OFFS = i32_value_held_in_STO
		OFFS = i32_value_held_in_FRAME
		OFFS = i32_value_held_SOMEWHERE
		OFFS = u16_from_SOMEWHERE // segment is not from middle
		OFFS = i16_from_SOMEWHERE
		BASE = ZERO + OFFS


*/
struct A_WH
{
	int w;
	int h;

	/*!
	*/
	A_WH( int _w, int _h )
	{
		w = _w;
		h = _h;
		// enforce_unsigned();
	}

	/*!
	*/
	void enforce_unsigned()
	{
		if( w < 0 ) w = 0;
		if( h < 0 ) h = 0;
	}

	/*!
	*/
	void reduce( int _w, int _h )
	{
		w -= _w;
		h -= _h;
		enforce_unsigned();
	}

	/*!
	*/
	void reduce( int _w )
	{
		reduce( _w, _w );
	}

	/*!
	*/
	void reduce( A_WH wh )
	{
		reduce( wh.w, wh.h );
	}
};

/*!
	A_Rectangle (x,y,width,height) is an alias for XRectangle

	XRectangle uses i16 and u16 so beware!
*/
struct A_Rectangle : XRectangle
{
	/*!
	*/
	A_Rectangle( short _x, short _y, unsigned short _w, unsigned short _h )
	{
		x = _x;
		y = _y;
		width = _w;
		height = _h;
	}

	/*!
	*/
	A_Rectangle( A_XY xy, A_WH wh )
	{
		x = xy.x;	// i32 to i16
		y = xy.y;	// i32 to i16
		width = wh.w;
		height = wh.h;
	}
	
	/*!
		reduce the right / bottom sides by adj
	*/
	void reduce2( short adj )
	{
	 /*
		width -= adj ;
		height -= adj ;

		except that width and height are UNSIGNED
		so beware of subtractions
	 */
		if( width > adj )
			width -= adj ;
		else
			width = 0;

		if( height > adj )
			height -= adj ;
		else
			height = 0;
	}
	
	/*!
		reduce all 4 sides by adj each
	*/
	void reduce4( short adj )
	{
		x += adj;
		y += adj;
		reduce2( 2 * adj );
	}

	/*!
		the right most XPOS - inside not out
	*/
	int right()
	{
		return x + width -1;
	}

	/*!
		the bottom YPOS - inside not out
	*/
	int bottom()
	{
		return y + height -1;
	}

};

};
#endif

