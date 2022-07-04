#ifndef tm_parts_H
#define tm_parts_H

// #include <sys/types.h>
// #include <sys/stat.h>
// #include <fcntl.h>

// #include <stdlib.h>
// #include <stdio.h>
#include <time.h>
// #include <errno.h>
// #include <string.h>

#include "dbg.h"
#include "tz.h"
#include "misc.h"	// bool


/*!
	tm_parts is a necessary wrapper on UNIX time converter.

	tm is the libc struct from <time.h>

	tz from lib_base/time/tz.h is "UTC" or "GMT" or "GB" ...
	tz TZ; // a global variable, from getenv("TZ")
	calling tz:save(); any number of times will save the first

	calling tz::set_TZ("UST") sets a TEMPORARY value
	LURK - current code suggests an infinite loop, TZ-NULL

	so to compute times for different places, call set_TZ
	to get back to TZ_SAVED ... read these (TODO FIX)

	tval (TODO rename) is the time_t, but it is only set ... _now() ...
	TODO CTOR to 1999-12-31 23:59 UST , or 1970, or xxxx


	tm_fields --> int
	_mktime_from_ converts from broken down tm fields to tval

	tm_fields <-- int
	local_from_time( time_t t ) converts INTO broken down fields tm

	str_pdf_now() // obtaines tval
	str_http() // uses previous setting

	for consistency, a bunch of functions format dates as expected by proto
	they presume you have ALREADY set the time
	TODO move to use buffer2 
	TODO add ..._now() which obtains time() breaks it down
	const char * str_http(); // 'Mon, 16 Sep 2002 16:37:14 GMT' // NOW


	There is a big UNIX/libc deficiency with time convertions,
	(1) there is no mktime without TZ, to get UST overwrite TZ!
	(2) TZ is GLOBAL, so to convert data from (far away) overwrite TZ
	(3) is_dst MUST USUALLY be set to -1, but it matters for that 1 hour

	All in all a lot of confusion, and I think this sorts it out,
	but ALWAYS GATHER YOUR DATA in 'UST' (date -u on hpux).

	NB I count months from 1, Jan is 1.

	((I always count arrays from zero, but when a value has an external
	value, the internal value should not be different ))

	Internationalisation is a bit missing.

	TODO: maybe this should retain its 'LOCAL' TZ setting.

*/
class tm_parts : public tm {
 #if 0
 MAN mktime says:
            struct tm {
               int tm_sec;    /* Seconds (0-60) */
               int tm_min;    /* Minutes (0-59) */
               int tm_hour;   /* Hours (0-23) */
               int tm_mday;   /* Day of the month (1-31) */
               int tm_mon;    /* Month (0-11) */
               int tm_year;   /* Year - 1900 */
               int tm_wday;   /* Day of the week (0-6, Sunday = 0) */
               int tm_yday;   /* Day in the year (0-365, 1 Jan = 0) */
               int tm_isdst;  /* Daylight saving time */
           };
 #endif
public:
	time_t tval;	// mktime(TZ) decodes tval into tm_
	// PARSER STRAY // int MM_from_Jan( char * jan );
	tm_parts() ;	// constuctor
	void init0() ;	// default all zeros (-1 for mm)
	void init1() ;	// init 0101
	void print() ;	// debugging printer

	// returns the time_t from the string parts, considered local time
	// returns -1 on fail
	bool call_mktime();// SETS TZ !!! ?? wrap in test/set ??
	bool mktime_from_UST();	// SETS TZ !!! ?? wrap in test/set ??
	bool mktime_from_TZ() { return mktime_from_local(); }
	bool mktime_from_local();
	bool mktime_from_local_with_isdst( int isdst = -1 );

	// DEPRECATE THESE - use if(mktime_from_ditto()) t=tval; else FAIL
	time_t time_from_UST();	// SETS TZ !!! ?? wrap in test/set ??
	time_t time_from_TZ() { return time_from_local(); }
	time_t time_from_local();
	time_t time_from_local_with_isdst( int isdst = -1 );

	// 
  static 
	time_t OS_time_now() { return time(NULL); } //

	bool UST_from_time( time_t t );
	bool TZ_from_time( time_t t ) { return local_from_time( t ); }
	bool local_from_time( time_t t );

	// set all the tm_parts // you probably want this // or for old time_t
	bool UST_from_time_now() { return UST_from_time( time(NULL)); }
	bool local_from_time_now() { return local_from_time( time(NULL) ); }

	int Jan_to_MM( char * jan ) { return MM_from_Jan( jan ) ; }
	int MM_from_Jan( char * jan );
	const char * Jan_from_MM( int mm );
	const char * January_from_MM( int mm );
	const char * Sun_from_int( int dow );
	const char * Sunday_from_int( int dow );

	const char * str_Sun() { return Sun_from_int( tm_wday ); }
	const char * str_Sunday() { return Sunday_from_int( tm_wday ); }
	const char * str_Jan() { return Jan_from_MM( mm() ); }
	const char * str_January() { return January_from_MM( mm() ); }

	int days_after_Sunday() const { return tm_wday ; }
	int days_after_Saturday() const { return (tm_wday+1)%7 ; }
	int days_after_Monday() const { return (tm_wday+6)%7 ; }
	int is_weekend() const { return (tm_wday==0) || (tm_wday==6) ; }

	bool check_tm_year(); // WARN when 1900 offset is seen

	void year( int year ) {
		tm_year = year - 1900;
	}
	int year() {
		return tm_year + 1900;
	}
	void month( char * s ) {
		mm( Jan_to_MM( s ));
	}
	void mm(int m) {
		tm_mon = m - 1;
	}
	int mm() {
		return tm_mon + 1;
	}
	void dd(int d) {
		tm_mday = d;
	}
	int dd() {
		return tm_mday;
	}

	void hour(int h) {
		tm_hour = h;
	}
	int hour() {
		return tm_hour;
	}
#ifdef min
#undef min
#endif
	void min(int m) {
		tm_min = m;
	}
	int min() {
		return tm_min;
	}
	void sec(int s) {
		tm_sec = s;
	}
	int sec() {
		return tm_sec;
	}

	// set sub_zone // TZ must already be set!
	int sub_zone( char * s );

	// generate strings (temp statics)
	const char * str_year_mm_dd();
	const char * str_hh_mm_ss();
	const char * str_hh_mm();
	const char * str_fmt1();
	const char * str_http(); // 'Mon, 16 Sep 2002 16:37:14 GMT' // NOW
	const char * str_syslog(); // 'Sep 16 16:37:14' // NOW
	const char * str_pdf(); // 'Sun Nov  1 20:43:07 2020' // NOW
	const char * str_pdf_now(); // first get time()

static
	const char * str_http( time_t t  );
	const char * str_pdf( time_t t  );

 static
	const char * time_fmt( time_t t );

	bool http_parse( const char * str );
	bool syslog_parse( const char ** str );
	bool parse_Jan( const char ** str, int & _mm );
	bool parse_int( const char ** str, int & _mm );

	// years have different numbers of seconds !!
	// this is flawed when used with PIOMASS // anything else too //
	void set_float_year( double year ); // eg 1979.5 is midsummer
	void get_float_year( double & year ); // get_as_ double

	time_t step_to_first_of_next_month_local();
	time_t step_to_first_of_next_month_UST();
	time_t step_to_00_of_next_day_local();
	time_t step_to_00_of_next_day_UST();
	void slide_past_hour_23();

 static
	bool is_leap( int yyyy );
 static
	int seconds_in_year( int year );
 static
 	int seconds_in_day() { return 24*60*60; }
 static
 	int minutes_in_day() { return 24*60; } // 1440
};
#endif
