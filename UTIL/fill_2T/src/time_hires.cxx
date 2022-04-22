#include "time_hires.h"
#include "dgb.h"
#include <stdio.h>
#include "tm_parts.h"

bool
time_hires_t::
call_get_time()
{
	if( gettimeofday( &tv, &tz ) ) { // 0 means OK
		FAIL("gettimeofday");
	}
	time64 = tv.tv_sec;
	time64 *= (1000*1000);
	time64 += tv.tv_usec;
	return true;
}

bool
time_hires_t::
set_time64(i64 _time64)
{
	if( gettimeofday( &tv, &tz ) ) { // 0 means OK
		FAIL("gettimeofday");
	}
	time64 = _time64;
	tv.tv_sec  = time64 / (1000*1000);
	tv.tv_usec = time64 % (1000*1000);
	return true;
}

bool
time_hires_t::
show_info()
{
	float frac = tv.tv_usec;
	frac /= (1000*1000);
	printf("tv.tv_sec  0x%08lX\n", tv.tv_sec );
	printf("tv.tv_usec 0x%08lX %4.2fs \n", tv.tv_usec, frac );
	printf("time64     0x%08llX\n", time64 );

	tm_parts parts;
	if(!parts.local_from_time( tv.tv_sec ))
		return FAIL_FAILED();
	parts.print();

	return true;
}

bool test_time()
{
	printf("call_get_time\n" );
	time_hires_t time_hires;
	time_hires.call_get_time();
	time_hires.show_info();

	printf("set_time64\n" );

	time_hires_t time_hires_2;
	time_hires_2.set_time64(time_hires.time64);
	time_hires_2.show_info();

	return true;
}
