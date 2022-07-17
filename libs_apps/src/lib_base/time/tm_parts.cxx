// #include <sys/types.h>
// #include <sys/stat.h>
// #include <fcntl.h>

#include <stdlib.h> // getenv
#include <stdio.h>
// #include <time.h>
// #include <errno.h>
// #include <string.h>

#include "dbg.h"
#include "tz.h"
#include "tm_parts.h"
#include "e_print.h"

#include "dgb.h" // FAIL // rework to use return bool and time is tval

/*DELETE THESE */
time_t tm_parts::time_from_local()
{
	WARN("DEPRECATED use bool mktime_from_local");
	mktime_from_local();
	return tval;
}


tm_parts::tm_parts() {
	init0();
}

void tm_parts::init0() {
	tm_sec = 0;
	tm_min = 0;
	tm_hour = 0;
	tm_mday = 0;
	tm_mon = -1;
	tm_year = 0;

	tm_wday = -1; // output
	tm_yday = -1; // output
	tm_isdst = -1; // output only ??

	tval = 0;
}

void tm_parts::init1() {
	init0();
	tm_mday = 1;
	tm_mon =  0;
	// if you now set year, you get 1st Jan (but midnight exact)
}

void tm_parts::print( ) {
/*
	// Order - Declaration
	printf("tm_sec = %2.2d\n",	tm_sec );
	printf("tm_min = %2.2d\n",	tm_min );
	printf("tm_hour = %2.2d\n",	tm_hour );
	printf("tm_mday = %d\n",	tm_mday );
	printf("tm_mon+= %2.2d\n",	tm_mon+1 );
	printf("tm_year+= %4.4d\n",	tm_year+1900 );
	printf("tm_wday = %d\n",	tm_wday );
	printf("tm_yday = %d\n",	tm_yday );
	printf("tm_isdst = %d\n",	tm_isdst );

//	// Order - Reading
	printf("tm_year+= %4.4d\n",	tm_year+1900 );
	printf("tm_mon+= %2.2d\n",	tm_mon+1 );
	printf("tm_mday = %d\n",	tm_mday );
	printf("tm_hour = %2.2d\n",	tm_hour );
	printf("tm_min = %2.2d\n",	tm_min );
	printf("tm_sec = %2.2d\n",	tm_sec );
	printf("tm_isdst = %d\n",	tm_isdst );
	printf("tm_wday = %d\n",	tm_wday );
	printf("tm_yday = %d\n",	tm_yday );
*/
	// one liner
	e_print( "tm.print( %4.4d-%2.2d-%2.2d %2.2d:%2.2d:%2.2d",
		tm_year+1900,
		tm_mon+1,
		tm_mday,
		tm_hour,
		tm_min,
		tm_sec
	);

	e_print( " isdst=%d",	tm_isdst );
	e_print( " wday=%d",	tm_wday );
	e_print( " yday=%d )\n",	tm_yday );
}

/*!
	Join the fields of tm_parts into a time_t, or fail

	this.tval = mktime((tm*)this); || bool failed // tval == -1

	called by:
	mktime_from_local()	// TZ == TZ_SAVED
	mktime_from_local_with_isdst( int isdst ) // TZ == TZ_SAVED
	mktime_from_UST() 	// TZ == "UST"
*/
bool tm_parts::call_mktime()
{
/*	
	// CALLER has parsed a DATE_in_the_filename
	// CALLER places tm_
	// CALLER SETS TZ with
	// CALLER TZ.set_TZ_SAVED();
	// CALLER SETS tm_isdst
	The  mktime()  function converts a broken-down time struc-
	ture, expressed as local time, to calendar time  represen-
	NOTE "expressed as LocalTime"

	Since mktime is the ONLY str_to_int converter, you have no choice.
	You must call tzset(), or write your own functions. Shambles.

	tz::set_TZ(str) calls tzset() which uses putenv("TZ","UST")
	or else it uses /etc/localtime (see man tzset)

	
	mktime mostly appears to ignore tm_isdat, but its how it distinguishes
	between those two cases where an hour happens twice (locatime->UTC).

	AARGH! 'appears' that YOU have to know tm_isdst when you call mktime
	That actually makes sense for Autumn, when there are TWO lots of 1.30
*/
	tval = mktime( (struct tm * )this );
	if(tval == -1 ) return FAIL("mktime() failed");
	return true;
}

/*
	convert tm_ parts to time_t using "UST"
	this.tval; || FAIL

	The physical clock on all UNIX boxes worldwide shoule be UST,
	then getenv(TZ) converts that to local wall clock time
	"cron" often uses UST, scripts often set it.
	If you do, you can always add "local" back later

	TZ.set_TZ("UST"); saves the original $TZ (first time only)
	so _local still works afterwards.

	You may need to restore $TZ before fork+exec or calling libr code,
	as this code leaves it where it was last left.

*/
bool tm_parts::mktime_from_UST()
{
/*	RETURNS: -1 on error else unix time in seconds
	The  mktime()  function converts a broken-down time struc-
	ture, expressed as local time, to calendar time  represen-
	NOTE "expressed as LocalTime"
*/
	TZ.set_TZ("UST");
	tm_isdst = -1; // UST has no dst
	return call_mktime();
}

/*
	convert tm_ parts to time_t using processes local TZ
	this.tval; || FAIL
*/
bool tm_parts::mktime_from_local()
{
	/*
		TZ_saved is the processes local getenv("TZ")
	*/
	TZ.set_TZ_saved();
	tm_isdst = -1;
	return call_mktime();
}

/*
	convert parts to time, without default(-1) tm_isdst

	This is only needed to resolve that hour in UK autumn,
	when the clocks go backwards, and there are two interpretations
	of the same time-string (GMT BST selects is_dst).

	So for 99% usefulness, isdst=-1
*/
bool tm_parts::mktime_from_local_with_isdst( int isdst )
{
	TZ.set_TZ_saved();
	tm_isdst = isdst;
	return call_mktime();
}


// _pdf


bool tm_parts::local_from_time( time_t t ) {
	tval = t;
	struct tm * tp; /* non-thread-safe global user of localtime()'s hidden*/
	// thread_safe_needs_lock_start
	tp = localtime( &t );
	if(!tp) return FAIL("localtime(&t)");;
	// dest = this, src = hidden_returned_by_localtime()
	memcpy( (struct tm *) this, tp, sizeof( struct tm ) );
	// if( daylight ) tm_isdst = 1; else tm_isdst = 0;
	// thread_safe_needs_lock_end
	return TRUE;
}

const char * tm_parts::str_hh_mm_ss( ) {
	static char s[10];
	sprintf(s,"%2.2d:%2.2d:%2.2d", hour(), min(), sec() );
	return s;
}

const char * tm_parts::str_hh_mm( ) {
	static char s[10];
	sprintf(s,"%2.2d:%2.2d", hour(), min() );
	return s;
}

const char * tm_parts::str_year_mm_dd( ) {
	// after local_from_time()
	static char s[30];
	sprintf(s, "%d-%2.2d-%2.2d", year(), mm(), dd() );
	return s;
}
const char * tm_parts::str_fmt1( ) {
	char * dst = tzname[0];
	if( tm_isdst )
		dst = tzname[1];
	static char s[180];
	const char * sun = Sun_from_int( tm_wday );
	const char * jan = Jan_from_MM( mm() );
	sprintf(s,"%s %s %s %s !%s (%s) [%s/%s] %ld",
		str_year_mm_dd(),
		sun, jan,		// placing here is sortable
		str_hh_mm_ss(),
		dst,
		getenv("TZ")
		,tzname[0], tzname[1], timezone
	);
	return s;
}

// static
const char * tm_parts::str_http( time_t t  )
{
	// static so needs a tm_parts object
	tm_parts tm;
	tm.UST_from_time(t);
 return
	tm.str_http();
}

// static
const char * tm_parts::str_pdf( time_t t  )
{
	// static so needs a tm_parts object
	tm_parts tm;
	tm.UST_from_time(t);
 return
	tm.str_pdf();
}

const char * tm_parts::str_http()
{
	// 'Mon, 16 Sep 2002 16:37:14 GMT'
	/*
	// ignore dst - HTTP only uses GMT
	char * dst = tzname[0];
	if( tm_isdst )
		dst = tzname[1];
	*/
	// not very reentrant, but single threaded 
	// static returned string holder
	// (vs direct to buffer)
	static char s[180];
	const char * sun = Sun_from_int( tm_wday );
	const char * jan = Jan_from_MM( mm() );
	sprintf(s,"%s, %d %s %d %s %s",
		sun,
		dd(),
		jan,
		year(),
		str_hh_mm_ss(),
		"GMT"
	);
	return s;
}

const char * tm_parts::str_pdf() // the otherwise set time
{
	// 'Sun Nov  1 20:43:07 2020'

	static char s[180];
	const char * sun = Sun_from_int( tm_wday );
	const char * jan = Jan_from_MM( mm() );
	sprintf(s,"%s %s %d %s %d",
		sun,
		jan,
		dd(),
		str_hh_mm_ss(),
		year()
	);
	return s;
}

const char * tm_parts::str_pdf_now()
{
	return str_pdf(time(NULL)); // the time is now
}

const char * tm_parts::str_syslog()
{
	// 'Sep 16 16:37:14'
	// 'Mon, 16 Sep 2002 16:37:14 GMT'
	static char s[180];
	const char * jan = Jan_from_MM( mm() );
	sprintf(s,"%s %d %2.2d:%2.2d:%2.2d",
		jan,
		dd(),
		hour(), min(), sec()
	);
	return s;
}

bool tm_parts::UST_from_time( time_t t ) {
	tval = t;
	struct tm * tp; /* non-thread-safe global */
	tp = gmtime( &t );
	if(!tp) return FALSE;
	memcpy( (struct tm *) this, tp, sizeof( struct tm ) );
	return TRUE;
}

const char * tm_parts::January_from_MM( int mm ) {
	const char * mnames[] = {
		0,
		"January", "February", "March",
		"April", "May", "June",
		"July", "August", "September",
		"October", "November", "December",
		0
	};
	return mnames[mm];
};

const char * tm_parts::Sun_from_int( int dow ) {
	const char * dnames[] = {
		"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
	};
	return dnames[ dow ];
}

const char * tm_parts::Sunday_from_int( int dow ) {
	const char * dnames[] = {
		"Sunday",
		"Monday",
		"Tuesday",
		"Wednesday",
		"Thursday",
		"Friday",
		"Saturday"
	};
	return dnames[ dow ];
}

const char * tm_parts::Jan_from_MM( int mm ) {
	const char * mnames[] = {
		0,
		"Jan", "Feb", "Mar",
		"Apr", "May", "Jun",
		"Jul", "Aug", "Sep",
		"Oct", "Nov", "Dec",
		0
	};
	return mnames[mm];
};
int tm_parts::MM_from_Jan( char * jan )
{
	int i;
	const char * mnames[] = {
		"Jan", "Feb", "Mar",
		"Apr", "May", "Jun",
		"Jul", "Aug", "Sep",
		"Oct", "Nov", "Dec",
		0
	};
	for( i=0; i<12; i++ ) {
		if( 0 == strcmp( jan, mnames[i] ) ) {
			i += 1;
			// printf("Jan_to_MM(%s) --> %d\n", jan, i );
			return i;
		}
	}
	return 0;
}

int tm_parts::sub_zone( char * s ) {

	// this is rubbish but you need something like it when ..
	// autumn clocks fall back, requires 'BST' or 'GMT' from string
	// requires is_dst to be set ?

        if( 0==strcmp( s, "GMT" ) ) {
                tm_isdst = 0;
        } else if( 0==strcmp( s, "UTC" ) ) {
                tm_isdst = 0;
        } else if( 0==strcmp( s, tzname[0] ) ) {
                tm_isdst = 0;
        } else if( 0==strcmp( s, tzname[1] ) ) {
		// e_print(  "sub_zone %s matches tzname[1]\n", s );
                tm_isdst = 1;
        } else if( 0==strcmp( s, "BST" ) ) {
                tm_isdst = 1;
        } else {
                tm_isdst = -1;
        }
	return tm_isdst;
}

// STATIC
const char * tm_parts::time_fmt( time_t t )
{
	tm_parts tm;
	
	// tm.UST_from_time( t );
	tm.local_from_time( t );
	return tm.str_fmt1();

}

bool tm_parts::http_parse( const char * str )
{
	// 'Mon, 16 Sep 2002 16:37:14 GMT'
	char sun[5+10];
	int _dd;
	char jan[5+10];
	int _year;
	int _h;
	int _m;
	int _s;
	char gmt[5+10];
	int itemcount = sscanf( "%4s, %d %4s %d %d:%d:%d %4s", str,
		& sun, 
		& _dd,
		& jan,
		& _year,
		& _h,
		& _m,
		& _s,
		& gmt
	);
	if( itemcount != 8 )
	{
		e_print( "tm_parts::http_parse(%s) failed\n", str );
		return false;
	}
#warning "http_parse -- put the result somewhere?"
	return true;
}

bool tm_parts::parse_int( const char ** str_var, int & _val )
{
	// Q+D // replace with SCAN
	const char * P1 = * str_var;
	int val = 0;
	bool found = false;
	while( *P1 == ' ' ) P1++; 
	// ignore any -
	while( ('0' <= *P1 ) && ( *P1 <= '9' ) )
	{
		found = true;
		val *= 10;
		val += *P1 - '0';
		P1++; 
	}
	while( *P1 == ' ' ) P1++; 
	if(!found) return false;
	// advance CSR str_var
	*str_var = P1;
	_val = val;
	return true;
}

bool tm_parts::parse_Jan( const char ** str_var, int & _mm )
{
	const char * P1 = * str_var;

	// skip leading blanks
	while( *P1 == ' ' ) P1++; 
	char name[4];
	name[0] = P1[0];
	name[1] = P1[1];
	name[2] = P1[2];
	name[3] = 0;
	int t = MM_from_Jan( (char *) name );
	if( !t ) return false;
	P1 += 3;
	while( *P1 == ' ' ) P1++; 
	// advance CSR str_var
	* str_var = P1;
	_mm = t;
	return true;
}

bool tm_parts::syslog_parse( const char ** str_var )
{
	// 'Feb 23 17:18:10 '
//	char jan[5+10];
	int _month;
	int _dd;
	int _h;
	int _m;
	int _s;
	// year must be taken from RECENT (setup with care)
//	int _year;
//	char sun[5+10];
//	char gmt[5+10];

	const char * str = *str_var;
	if( !parse_Jan( &str, _month ) )
	{
		e_print("syslog_parse(%s) failed to find Jan\n", str );
		return false;
	}

	if( !parse_int( &str, _dd ) )
	{
		e_print("syslog_parse(%s) failed to find DAY\n", str );
		return false;
	}

	if( !parse_int( &str, _h ) )
	{
		e_print("syslog_parse(%s) failed to find HOUR\n", str );
		return false;
	}
	if( *str == ':' ) str++; else {
		e_print("syslog_parse(%s) failed to find ':'\n", str );
		return false;
	}
	if( !parse_int( &str, _m ) )
	{
		e_print("syslog_parse(%s) failed to find MIN\n", str );
		return false;
	}
	if( *str == ':' ) str++; else {
		e_print("syslog_parse(%s) failed to find ':'\n", str );
		return false;
	}
	if( !parse_int( &str, _s ) )
	{
		e_print("syslog_parse(%s) failed to find SEC\n", str );
		return false;
	}

	mm( _month );
	dd( _dd );
	hour( _h );
	min( _m );
	sec( _s );
	// advance CSR str_var
	*str_var = str;
	return mktime_from_local();
}
	

time_t tm_parts::step_to_first_of_next_month_local()
{
	int m = mm();
	dd( 1 );
	hour( 0 );
	min( 0 );
	sec( 0 );
	if( m < 12 )
		mm( m+1 );
	else
	{
		mm( 1 );
		year( year() + 1 );
	}
	mktime_from_local();
	local_from_time( tval );
//	slide_past_hour_23();
	return tval;
}


time_t tm_parts::step_to_first_of_next_month_UST()
{
	int m = mm();
	dd( 1 );
	hour( 0 );
	min( 0 );
	sec( 0 );
	if( m < 12 )
		mm( m+1 );
	else
	{
		mm( 1 );
		year( year() + 1 );
	}
	mktime_from_UST();
//	slide_past_hour_23();
	return tval;
}

time_t tm_parts::step_to_00_of_next_day_local()
{
	dd( dd() + 1 );
	hour( 0 );
	min( 0 );
	sec( 0 );
	mktime_from_local();
	local_from_time( tval );
//	slide_past_hour_23();

	// tval is auto set
	return tval;
}

time_t tm_parts::step_to_00_of_next_day_UST()
{
	dd( dd() + 1 );
	hour( 0 );
	min( 0 );
	sec( 0 );
	time_t t = mktime_from_UST();
	UST_from_time( t );
//	slide_past_hour_23();
	// tval is auto set
	return tval;
}


void tm_parts::slide_past_hour_23()
{
	while(1)
	{
		local_from_time( tval );
		if( hour() < 4 )
			break;
		e_print("step_to_00_of_next_day_local() slides by an hour from %d\n", hour());
		tval += 60 * 60;
	}
}

// BUG // TODO // FIX // timezone should probably be GMT0

/*!
	For systems that use a fractional year as an exact time
*/
void tm_parts::set_float_year( double year_time ) // eg 1979.5 is midsummer
{
	// year_time is year
	// the result will be inaccurate because input used 2 decimal places
	// leap is also smeared over entire year
	int int_year = (int) year_time;
	double year_frac = year_time - int_year;
	init1(); //
	year( int_year ); // must use getter setter because yyyy - 1900 == yy

	if(!check_tm_year()) { /* OK 1900 message is enough*/ }

	if(!mktime_from_local()) { FAIL_FAILED(); return; };

	int secs_in_year = seconds_in_year( int_year );
	// if a data set has 365 values for a 366 day year, 1 will be missing
	// look around YEAR-12-30 for the missing day // certain input criteria

	int second_of_year = secs_in_year * year_frac;
	tval += second_of_year;

	local_from_time( tval ); // local or zulu

	// that leaves all the fields ready to print
}

void tm_parts::get_float_year( double & year_time ) // get_as_ double
{
	// tval must be correct // tm_year too
	int int_year = year();
	tm_parts tm2;
	tm2.set_float_year( (double) int_year ); // ONE SIDED LOOP //
	double second_of_year = tval - tm2.tval;
	double year_frac = second_of_year / seconds_in_year(int_year);

	year_time = int_year + year_frac;
}

bool tm_parts::check_tm_year() // WARN when 1900 offset is seen
{
	bool is_ok = true;
	if( tm_year == 0 ) {
		INFO("# OK # tm_year == %d", tm_year );
		return true;
	}
	int yyyy = year();
	if( yyyy == 0 ) {
		INFO("# OK # yyyy == %d", yyyy );
		return true;
	}
	if( yyyy < 120 ) { // most likely error
		return FAIL("yyyy == %d # 1999 not 99 (5d)", yyyy, yyyy+1900 );
	}
	if( yyyy < 1582 ) { // when gregorian came into being
		return FAIL("yyyy == %d # 1900 off (%d)", yyyy, yyyy+1900 );
	}
	if( yyyy > 3700 ) { // 1900 + 1900 == 3800
		return FAIL("yyyy == %d # 1900 too many (%d)", yyyy, yyyy - 1900 );
	}
	if( yyyy > 2200 ) { // 100 years from now
		return WARN("yyyy == %d # (%d)", yyyy, yyyy - 1900 );
	}
	return is_ok;
}


bool tm_parts::is_leap( int yyyy )
{
	bool is_leap = false;
	if( yyyy %   4 == 0 ) is_leap = true;
	if( yyyy % 100 == 0 ) is_leap = false;
	if( yyyy % 400 == 0 ) is_leap = true;
	return is_leap;
}

int tm_parts::seconds_in_year( int year )
{
	int days = 365;
	if( is_leap( year ) ) days ++;
	return days*seconds_in_day();
}
