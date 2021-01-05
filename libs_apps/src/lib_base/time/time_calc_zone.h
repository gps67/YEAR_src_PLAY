#ifndef time_calc_zone_H
#define time_calc_zone_H

#include <time.h>
#include "ints.h"

//! calculate the bound of an era (eg a week)
struct time_calc_zone : public GRP_lib_base
{
	//! the start of the week
	time_t	T0;
	//! the end of the week = start of next week
	time_t	T2;

	time_calc_zone( time_t t1, int era_width );
};

#endif
