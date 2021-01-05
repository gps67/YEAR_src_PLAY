#ifndef axes_calc_h
#define axes_calc_h

#include <stdio.h>

typedef long double flt80;
// typedef float flt80;

#include <string.h>
#include <stdlib.h>
#include "str1.h"
#include "buffer2.h"

/*!
	Axis_Calc is mostly used to invert pdf's origin placement
	but it is generalised.

	The origin is on the dev, and the factor stretches or inverts,
	after the origin has been shifted,
	so be careful with the sequence, and plus/minus. It might
	go unnoticed as X/-1 == X*-1 and X+0 == X-0

	Subsequent axes are layered on top, but map to the lowest

	Factor may not be zero
*/
class Axis_Calc : public GRP_lib_base2_pdf
{
	Axis_Calc()
	{
	}
 public:
	flt80 origin;		//<! position of surface zero on dev
	flt80 factor;		//<! scale dev units per surface unit
	str1 name;

	void set_name( str0 prefix, str0 suffix )
	{
		if(!prefix) prefix = "";
		if(!suffix) suffix = "";
		buffer2 buff;
		buff.print( "%s_%s", (STR0) prefix, (STR0) suffix );
		name.set( str0( buff ));
	}

	Axis_Calc( const Axis_Calc & axis2 )
	: name( axis2.name )
	{
		origin = axis2.origin;
		factor = axis2.factor;
	}

	Axis_Calc & operator=( const Axis_Calc & axis2 )
	{
		origin = axis2.origin;
		factor = axis2.factor;
		name.set( axis2.name );
		return *this;
	}

	~Axis_Calc()
	{
	}

	Axis_Calc( str0 _name, str0 XY, float _origin, float _factor, const Axis_Calc & page )
	{
		set_name( _name, XY );
		origin = page.dev_from_surface( _origin );
		factor = page.factor * _factor;
	}

	Axis_Calc( str0 _name, str0 XY, float _origin = 0.0, float _factor = 1.0 )
	{
		set_name( _name, XY );
		origin = _origin;
		factor = _factor;
	}

	flt80 dev_from_surface( flt80 pos ) const
	{
#if 0
		float t1 = pos;
		float t2 = ((pos * factor + origin)/72.0);
		printf("%s %5.2f dev_from_surface( %5.2f )\n",
			(STR0) name, 
			t2, t1
		);
#endif
		return pos * factor + origin;
	}

	flt80 surface_from_dev( flt80 pos ) const
	{
		return (pos - origin) / factor;
	}

	flt80 dev_from_surface_step( flt80 step ) const
	{
		return step * factor;
	}

	flt80 surface_from_dev_step( flt80 step ) const
	{
		return step / factor;
	}
};

class Axes_Calc : public GRP_lib_base2_pdf
{
 public:
	Axis_Calc X;
	Axis_Calc Y;

	Axes_Calc(
		str0 name,
		float _Xorigin,
		float _Yorigin,
		float _Xfactor,
		float _Yfactor
	)
	: X( name, "X", _Xorigin, _Xfactor )
	, Y( name, "Y", _Yorigin, _Yfactor )
	{
	}

	Axes_Calc(
		str0 name,
		float _Xorigin,
		float _Yorigin,
		float _Xfactor,
		float _Yfactor,
		const Axes_Calc & page
	)
	: X( name, "X", _Xorigin, _Xfactor, page.X )
	, Y( name, "Y", _Yorigin, _Yfactor, page.Y )
	{
	}

	void set_name( str0 base )
	{
		X.set_name( base, "X" );
		Y.set_name( base, "Y" );
	}

};

#endif
