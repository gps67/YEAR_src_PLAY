#include "pair_steps.h"
#include "dgb.h"
// #include <sys/types.h>
// #include <sys/stat.h>
// #include <fcntl.h>


pair_steps_t::
pair_steps_t()
{
	fd_data_log.ref_static();
	// *** stack smashing detected ***: terminated

	late_init( 1, 1, 1024 );
}

pair_steps_t::
pair_steps_t( i64 _DX, i64 _DY, i64 _Y100  )
{
	fd_data_log.ref_static();
	// *** stack smashing detected ***: terminated

	late_init( _DX, _DY, _Y100 );
}

bool
pair_steps_t::
late_init( i64 _DX, i64 _DY, i64 _Y100 )
{
	DX = _DX;
	DY = _DY;
	Y100 = _Y100;

	X1 = Y1 = 0;
	X2 = Y2 = 0;

	started = false;
	return true;
}


bool
pair_steps_t::
open_data_log( const char * filename )
{
	
	bool is_async = false; // is NOT async
	int flags_plus = O_APPEND;
	if(! fd_data_log.open_RW_can_CREAT( filename, is_async, flags_plus ) ) {
	
		return FAIL_FAILED();
	}
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
		Y2 = Y1 + DY;
		return true;
	}

	if(( X > X2 ) || ( Y > Y2 )) {

		// after several smmall steps, the total big step that counts
		int step_X = X - X1;
		int step_Y = Y - Y1;
		if( !step_X ) { step_X = 1; }
		float dy_dx = ((float) step_Y ) / ((float) step_X );

		dy_dx *= 1000 * 1000; // 1 microsecond to 1 sec // BPS
	//	dy_dx /= 1024 ; // 1 byte to 1 KB // kps
		dy_dx /= 1024 * 1024; // 1 byte to 1 MB // MB/sec

		// USB 1.1    12 Mbps
		// USB 2.0   480 Mbps
		// USB 3.0  5000 MPS
		// USB 3.1 10000 MPS

		// USB SD WR   6 Mbps
		// USB SD RD  10 Mbps

		// this class need access to full capacity to calc percent
		float percent = Y;
		percent /= Y100;
		percent *= 100.0;
		INFO( "X %08llX Y %08llX dy_dx %7.3f %% %5.2f", X, Y, dy_dx, percent );

		// convert dy_dx into K_p_S // factor to_K to_S

		X1 = X;
		Y1 = Y;

		X2 = X1 + DX;
		Y2 = Y1 + DY;

		return pair_step( X, Y );
	}

	return true;
}

bool
pair_steps_t::
pair_step( i64 X, i64 Y )
{
	
	if(fd_data_log.is_open()) {
		FILE * F = fd_data_log.file(); // does fdopen
		if(F) fprintf( F,"0x%08llX 0x%08llX \n", X, Y );
	}

	return true;
}
