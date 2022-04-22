#ifndef time_hires_H
#define time_hires_H

/*
	We are Linux (not even UNIX), no port yet to WIN

	on WIN try GetSystemTimeAsFileTime()
	to get 100 ns resolition (0.1ms)
	relative to Jan 1 1600
	as a 64 bit number

  FileTimeToSystemTime ->

  record SYSTEMTIME {
   wYear: Word;
   wMonth: Word;
   wDayOfWeek: Word;
   wDay: Word;
   wHour: Word;
   wMinute: Word;
   wSecond: Word;
   wMilliseconds: Word;

   StackOverflow suggests:

FILETIME t;
GetSystemTimeAsFileTime(&t); // unusable as is

ULARGE_INTEGER i;
i.LowPart = t.dwLowDateTime;
i.HighPart = t.dwHighDateTime;

int64_t ticks_since_1601 = i.QuadPart; // now usable
int64_t us_since_1601   = (i.QuadPart * 1e-1);
int64_t ms_since_1601   = (i.QuadPart * 1e-4);
int64_t sec_since_1601  = (i.QuadPart * 1e-7);

// unix epoch
int64_t unix_us  = (i.QuadPart * 1e-1) - 11644473600LL * 1000000;
int64_t unix_ms  = (i.QuadPart * 1e-4) - 11644473600LL * 1000;
double  unix_sec = (i.QuadPart * 1e-7) - 11644473600LL;

// i.QuadPart is # of 100ns ticks since 1601-01-01T00:00:00Z
// difference to Unix Epoch is 11644473600 seconds (attention to units!)

}

*/



/*

man gettimeofday

struct timeval {
	time_t      tv_sec;     // seconds 
	suseconds_t tv_usec;    // microseconds 
};

	micro second = 1/1,000,000 (2^20)

	64 bit arith, precalc
		(1024*1024) << 32 /(1000*1000) // >> 32

		52 bits / 19.5 bits -> 32 bits of precision (1.01)

		if you have 64 (active) bits of microseconds
		you may need to rethink the precision -vs- headroom

struct timezone {
	int tz_minuteswest;     // minutes west of Greenwich 
	int tz_dsttime;         // type of DST correction 
};

	tv or tz may be NULL

int gettimeofday(struct timeval *tv, struct timezone *tz);

*/


#include <sys/time.h>
#include "ints.h"

struct time_hires_t { // possibly time_hires_assist

	timeval tv;
	timezone tz;
	i64 time64;

	bool call_get_time(); // syscall gettimeofday
	bool set_time64(i64 _time64);
	bool show_info();

	static const
	i64 us_from_s( float s ) { return (i64) (1000.0 * 1000.0 * s); }
};


// int gettimeofday(struct timeval *tv, struct timezone *tz);

extern 
bool test_time();




#endif
