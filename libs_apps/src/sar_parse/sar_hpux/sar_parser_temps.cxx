#include "sar_parser_temps.h"

#include <stdlib.h>
#include <string.h>

/*
	systems often skip a second, this realigns

	When dt is known, each time MUST be a multiple of it.
	If your program was started manually (not by cron on the hour),
	the times will be shifted to the nearest multiple.

	ERA calculations make it more complicated, dt must be a divisor/factor
	of a week, and the era calculation is shifted by -3 days, so dt=7 is
	probably not valid (or call align_time( era1.T0, spec.dt ))

	If you get it wrong, you feed unaligned data to merge_floats
	which fails to merge segments that dont align (alternatively
	re-calculate n-th slot of each sample, overriding t0).
*/
void sar_parser_temps::align_time( time_t & _t )
{
	int grain = 10;
	grain = 5;	// allow 15 second
	if( dt_now > 0 ) grain = dt_now;
	time_t t = _t;
	t += (grain / 2);
	t /= grain;
	t *= grain;
	_t = t;
}

void sar_parser_temps::set_time_now()
{
	// LURK - run the parser with the same TZ as the collector
	// and check for this program manually setting TZ

	// convert the parsed date into time_t
	time_conv.init1();
	time_conv.year( year );
	time_conv.mm( mm );
	time_conv.dd( dd );

	time_conv.hour( hour );
	time_conv.min( min );
	time_conv.sec( sec );

//	time_now = time_conv.time_from_UST();
	time_now = time_conv.time_from_local();

	align_time( time_now );
	time_start = time_now;

	/*
		time_now MUST align to dt, which is not yet known
		tbl->set_time0( time_now );
	*/

	// tell tbl_collect
	// where is hostname - it also has to be set at approx here
	tbl->set_time0( time_now );
}

void sar_parser_temps::adj_time_now()
{
	// LURK - OK - dt must be less then 12 hours
	// LURK - cannot run over two midnight boundries (or 47 HRS?)
	// else add 1 to day, and HOPE 31 day wrap doesnt matter
	// Because SAR doesnt emit year-mm-dd on day change
	time_t t_old = time_now;
	bool wrapped = FALSE;
	// hour+1 is for DST shift 
	if( hour+1 < time_conv.hour() ) {
		wrapped = TRUE;
	}
	time_conv.hour( hour );
	time_conv.min( min );
	time_conv.sec( sec );
	if( wrapped ) {
		// update year-mm-dd tomorrow using tm, get-add-set
		time_now = time_conv.time_from_local();
		time_now += 60*60*24;
		time_conv.local_from_time( time_now );
	} else
		time_now = time_conv.time_from_local();

	align_time( time_now );

	int dt = time_now - t_old;

	/*
		dt_now has never been set, this is the first nonheader stanza

		Apart from this, this function does nothing, except check time.
		If you changed the system clock - it will blowup!

		dt_now must be a divisor of a week (eg 7 seconds will crash)
		and align requires it to be a multimple of 10.
	*/
	if( dt_now < 1 ) {
		// first setting
		const static int SECS_WEEK = 60 * 60 * 24 * 7;
		if( SECS_WEEK % dt ) {
			e_print("# FAIL # dt must be a factor of 1 week, it is %d\n", dt );
			// allowed = 1 2 3 4 5 6 7 8 9 10 12 (not 11 13 ... )
			// allowed 10 20 30 40 50 60
		}
		dt_now = dt;
		align_time( t_old );
		align_time( time_now );
		
		// are these different ?
		// tbl->set_time0( t_old );
		tbl->set_dt_in_all_cols( dt_now );
	}

	if( dt == dt_now ) {
		// as it should be // return
	} else
	if( dt == dt_now + 60*60 ) {
		fprintf( stderr, "-time- DST starting - should have been caught by TZ\n" );
		// all of 6 months data is WRONG!
		// least damage done by fudging midnight to 1 am
		time_start += 60*60;
		// dt -= 60*60;
		tbl->set_time0( time_start );
	} else
	if( dt == dt_now - 60*60 ) {
		fprintf( stderr, "-time- DST ending - should have been caught by TZ\n" );
		// all of 6 months data was WRONG!
		// least damage done by fudging midnight to 1 am
		time_start -= 60*60;
		// dt += 60*60;
		tbl->set_time0( time_start );
	}

	if( dt != dt_now ) {
		static const bool z= false;
		if(z) fprintf( stderr, "dt/dt_now = %d/%d   %s (t_old)\n",
			dt,
			dt_now,
			tm_parts::time_fmt( t_old )
		);
		if(1) fprintf( stderr, "dt/dt_now = %d/%d   %s (time_now)\n",
			dt,
			dt_now,
			tm_parts::time_fmt( time_now )
		);
		if(z) fprintf( stderr, "dt/dt_now = %d/%d   %s (-1 hour)\n",
			dt,
			dt_now,
			tm_parts::time_fmt( time_now -60*60 )
		);
		if(z) fprintf( stderr, "dt/dt_now = %d/%d   %s (+1 hour)\n",
			dt,
			dt_now,
			tm_parts::time_fmt( time_now +60*60 )
		);
		if(z) fprintf( stderr, "dt/dt_now = %d/%d   %s (-1 day)\n",
			dt,
			dt_now,
			tm_parts::time_fmt( time_now -60*60*24 )
		);
		if(z) fprintf( stderr, "dt/dt_now = %d/%d   %s (+1 day)\n",
			dt,
			dt_now,
			tm_parts::time_fmt( time_now +60*60*24 )
		);
		// dt_now = dt;
	}
}
