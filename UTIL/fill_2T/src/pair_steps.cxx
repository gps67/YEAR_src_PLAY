#include "pair_steps.h"

pair_steps_t::
pair_steps_t()
{
	late_init( 0, 0 );
}

pair_steps_t::
pair_steps_t( i64 _DX, i64 _DY )
{
	late_init( _DX, _DY );
}

bool
pair_steps_t::
late_init( i64 _DX, i64 _DY )
{
	DX = _DX;
	DY = _DY;

	X1 = Y1 = 0;
	X2 = Y2 = 0;

	started = false;
	return true;
}

bool
pair_steps_t::
pair_data( i64 X, i64 Y )
{
	if( !started ) {
		started = true;

		X1 = X;
		Y1 = Y;

		X2 = X1 + DX;
		Y2 = X1 + DY;
		return true;
	}

	if(( X > X2 ) || ( Y > Y2 )) {

		X1 = X;
		Y1 = Y;

		X2 = X1 + DX;
		Y2 = X1 + DY;

		return pair_step( X, Y );
	}

	return true;
}

bool
pair_steps_t::
pair_step( i64 X, i64 Y )
{
	return true;
}
