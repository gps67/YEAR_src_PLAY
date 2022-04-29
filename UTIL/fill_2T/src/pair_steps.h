#ifndef pair_steps_H
#define pair_steps_H

// monotonic increasing //
// past threshold

#include "ints.h"
#include "str1.h"
#include "fd_hold.h"

struct pair_steps_t {
	
	// X0 Y0 are 

	i64 X1;
	i64 Y1;

	bool started;	// the first pair_data starts (X1,Y1)

	i64 X2;		// crossing this threshold emits a point, recomputes
	i64 Y2;		// crossing this threshold emits a point, recomputes

	i64 Y100;	// 100% 

	i64 DX_UNIT; // 1 sec = 1000 * 1000 X ticks
	i64 DY_UNIT; // 1 MB = 1024 * 1024 Y ticks

	str1 DX_UNIT_name; // sec
	str1 DY_UNIT_name; // MB

	i64 DX; // DX_units // 1.2 seconds as DX ticks
	i64 DY; // DY_units // 64K // 64/1024 MB as DY ticks // second unit M

	fd_hold_1 fd_data_log;

	// UNIT tick = 1 byte 
	// UNIT byte = 1
	// UNIT KB = 1024 byte
	// UNIT MB = 1024 * 1024 byte
	// STEP DY = 64K   // but report as MB/sec

	pair_steps_t();
	pair_steps_t( i64 _DX, i64 _DY, i64 _Y100 );
	bool late_init( i64 _DX, i64 _DY, i64 _Y100 );

	bool open_data_log( const char * filename );

	bool pair_data( i64 X, i64 Y );
	bool pair_step( i64 X, i64 Y );

};

#endif

