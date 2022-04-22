#ifndef pair_steps_H
#define pair_steps_H

// monotonic increasing //
// past threshold

#include "ints.h"

struct pair_steps_t {
	
	// X0 Y0 are 

	i64 X1;
	i64 Y1;

	bool started;

	i64 X2;
	i64 Y2;

	i64 DX;
	i64 DY;

	pair_steps_t();
	bool late_init( i64 _DX, i64 _DY );
	pair_steps_t( i64 _DX, i64 _DY );

	bool pair_data( i64 X, i64 Y );
	bool pair_step( i64 X, i64 Y );


};

#endif

