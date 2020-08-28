#ifndef SPOUT_xy_H
#define SPOUT_xy_H

namespace SPOUT {

struct XY_t
{
	float x;
	float y;

	XY_t &  set_zero()
	{
		x = 0.0;
		y = 0.0;
		return * this;
	}

	XY_t()
	{
		set_zero();
	}

	XY_t( float _x, float _y )
	: x(_x)
	, y(_y)
	{
	}

	XY_t & set( float _x, float _y )
	{
		x = _x;
		y = _y;
		return *this;
	}

	bool xy_print_3d( buffer2 & out )
	{
		// NB leading space
		// NB trailing space
		return out.print( " %3d %3d ", (int) x, (int) y );
	}

}; // class
}; // namespace

#endif
