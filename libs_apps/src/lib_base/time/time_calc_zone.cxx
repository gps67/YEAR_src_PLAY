
#include <stdio.h>

#include "time_calc_zone.h"

// this completely ignores leap seconds

#define SEC_DAY  (24*60*60)
#define SEC_WEEK (7*SEC_DAY)

//! constructor - t1 = rounded; t1 + era_width == t2,
time_calc_zone::time_calc_zone( time_t t1, int era_width ) {

	/*
		When era is not a week, the result is still aligned
		to a weeks boundry!! This can be confusing

		There is no such thing as year 0
		but numerically it started on a thursday
		and multiples of 7 days needs an initial adjustment

		If 0 has started on a Sun, all numbers would have been
		4 days more (or 3 days less)

		This works on hours, days, weeks, etc,
		You could have multiples of 1000 seconds, 
		but that would be relative to a week-aligned zero!
	*/

	// adjust to week alignment
	int d1 = +4 * SEC_DAY; // Thursday from Sun
	time_t t = t1 + d1;

	// round down to nearest multiple of era_width
	t = t / era_width;
	t = t * era_width;

	// unadjust for week alignment
	t = t - d1;

	// save era start and next era start
	T0 = t;
	T2 = t + era_width;

#ifdef EXTRA_STUFF_99
	// debugging print
 if(0) {
	tm_parts tm;
	tm.local_from_time( t1 );
	fprintf(stderr,"calc_zone: %s\n", tm.str_fmt1() );
	tm.local_from_time( t );
	fprintf(stderr,"calc_zone: %s\n", tm.str_fmt1() );
 }
#endif
}
