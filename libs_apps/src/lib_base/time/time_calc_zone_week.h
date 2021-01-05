#ifndef time_calc_zone_week_H
#define time_calc_zone_week_H

#include "time_calc_zone.h"

//! previously I wanted eras of a week, now I#m not so sure
struct time_calc_zone_week : public time_calc_zone
{
	time_calc_zone_week( time_t t );
};

struct time_calc_zone_week4 : public time_calc_zone
{
	time_calc_zone_week4( time_t t );
};

#endif
