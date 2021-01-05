
#include "time_calc_zone_week.h"

#define SEC_DAY  (24*60*60)
#define SEC_WEEK (7*SEC_DAY)

//! previously I wanted eras of a week, now I'm not so sure

/*!
	constructor - set this era to the week surrounding t1
*/
time_calc_zone_week::time_calc_zone_week( time_t t1 )
: time_calc_zone( t1, SEC_WEEK )
{
}

/*!
	constructor - set this era to the 4-week surrounding t1
*/
time_calc_zone_week4::time_calc_zone_week4( time_t t1 )
: time_calc_zone( t1, SEC_WEEK*4 )
{
}
