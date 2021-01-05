
#include "date_glib.h"
#include "e_print.h"

#include <stdio.h>

/*!
	constructor - default glib clear
*/
date_glib::date_glib() {
	g_date_clear( &date, 1 );
}

/*!
	set year-mm-dd from three ints, return bool OK

	WARNING - invalid dates revert to 2000-01-01, or other
*/
bool date_glib::set_year_mm_dd( int _year, int _mm, int _dd )
{
	GDateYear year = (GDateYear) _year; // no change ;-)
	GDateMonth mm = (GDateMonth) _mm;
	GDateDay dd = (GDateDay)_dd;

	if( ! g_date_valid_dmy( dd, mm, year ) ) {
		e_print( "Invalid date %d-%2.2d-%2.2d\n", year,mm,dd );         
		// try fixing dd
		dd = 01;
		if( ! g_date_valid_dmy( dd, mm, year ) )
			// try fixing mm
			mm = (GDateMonth) 1;
		if( ! g_date_valid_dmy( dd, mm, year ) )
			// try fixing year
			year = 2000;
		g_date_set_dmy( &date, dd, mm, year );
		return FALSE;
	}
	g_date_set_dmy( &date, dd, mm, year );
	return TRUE;
}

/*!
	set year-mm-dd from a string - return bool ok
*/
bool date_glib::set_year_mm_dd( const str0 s )
{
	int year, mm, dd;
	if( 3 != sscanf( s, "%4d-%2d-%2d", &year, &mm, &dd ) ) {
		fprintf( stderr, "Expected YEAR-MM-DD got '%s'\n", (STR0) s );
		set_year_mm_dd( 1999, 12, 31 );
		return FALSE;
	}
	return set_year_mm_dd( year, mm, dd );
}

/*!
	2-Dec-02 is brain-damaged, but people do insist on it.

	NB Try to avoid 09 causing an octal overload 

	NB English Jan,Feb,..

	NB 1950 - 2049 # same as yahoo ...

	HINT: 2002-12-02 is ascii sortable, fixed width and THE WAY TO DO IT
*/
bool date_glib::set_dd_Jan_yy( const str0 s )
{
	const char * fmt = "%d-%3c-%d";
	int _dd = 0;
	int _mm = 0;
	int _yy = 0;
	char Jan[4];
	int e1;
	if( 3!= (e1=sscanf( s, fmt, &_dd, &Jan, &_yy ) ))
	{
		e_print("Expected %s got %s (e1=%d)\n", fmt, (STR0) s, e1 );
		return false;
	}
	Jan[3] = 0;

	if( !parse_Jan( Jan, _mm ) )
	{
		e_print("Expected %s got %s (e1=%d)\n", fmt, (STR0)s,  e1 );
		return false;
	}
	int _year = 1;
	#warning "Y2K date wraps at 2050"
	if( _yy < 50 )
		_year = 2000 + _yy;
	else
		_year = 1900 + _yy;

	
	return set_year_mm_dd( _year, _mm, _dd );
}

/*!
	Convert "Jan" to 1 

	NB English only
*/
bool date_glib::parse_Jan( const str0 s, int & _mm )
{
	_mm = 0;
	/**/ if(s == "Jan") _mm=1; 
	else if(s == "Feb") _mm=2; 
	else if(s == "Mar") _mm=3; 
	else if(s == "Apr") _mm=4; 
	else if(s == "May") _mm=5; 
	else if(s == "Jun") _mm=6; 
	else if(s == "Jul") _mm=7; 
	else if(s == "Aug") _mm=8; 
	else if(s == "Sep") _mm=9; 
	else if(s == "Oct") _mm=10; 
	else if(s == "Nov") _mm=11; 
	else if(s == "Dec") _mm=12; 
	return _mm;
}

/*!
	return number of days since (julian scale)
*/
i32 date_glib::julian()
{
	return g_date_julian( &date );
}

/*!
	return number of days since (julian scale)
*/
void date_glib::set_julian( i32 J)
{
	return g_date_set_julian( &date, J );
}

/*!
	GET year mm dd as 3 int vars
*/
void date_glib::get_year_mm_dd( int & year, int & mm, int & dd )
{
	year = g_date_year( & date );
	mm   = g_date_month( & date );
	dd   = g_date_day( & date );
}

/*!
	get "year-mm-dd" as a static temp string
*/
const char * date_glib::tmp_year_mm_dd()
{
	int year, mm, dd;
	get_year_mm_dd( year, mm, dd );
	static char s1[10+1+10];
	sprintf( s1, "%d-%2.2d-%2.2d", year, mm, dd );
	return s1;
}

/*!
	get "year-mm-dd" as a string into s2
*/
void date_glib::get_year_mm_dd( str1 & s2 )
{
	s2.set( tmp_year_mm_dd() );
}

/*!
	get "year-mm" as a string var
*/
void date_glib::get_year_mm( str1 & s2 )
{
	int year, mm, dd;
	get_year_mm_dd( year, mm, dd );
	char s1[10+1+10];
	// sprintf( s1, "%d/%2.2d/%2.2d", year, mm, dd ); // show tracer
	sprintf( s1, "%d-%2.2d", year, mm );
	s2.set( (str0) s1 );
}

/*!
	get "dd" as a 2 digit string (might look like octal)
*/
void date_glib::get_dd( str1 & s2 )
{
	int year, mm, dd;
	get_year_mm_dd( year, mm, dd );
	char s1[10+1+10];
	// sprintf( s1, "%d/%2.2d/%2.2d", year, mm, dd ); // show tracer
	sprintf( s1, "%2.2d", dd );
	s2.set( (str0) s1 );
}

/*!
	compare two dates ; IS_{ LESS SAME MORE }
*/
IS_DIFF_t date_glib::cmp( date_glib & date2 )
{
#if 0
	// useful in gdb to see which date is being compared
	// expands julian to year-mm-dd in the fields
	// not sure where the data wasnt copied, though ?
	g_date_day( & date );
	g_date_day( & date2.date );
#endif
	i32 t = julian() - date2.julian();
	if( t <  0 ) return IS_LESS;
	if( t == 0 ) return IS_SAME;
	return IS_MORE;
}

/*!
	get Mon, Tue, ... as an integer -- glib GDateWeekday as int
*/
int date_glib::get_day_of_week()
{
	GDateWeekday g_dow = g_date_get_weekday( & date );
	return (int) g_dow;
}

/*!
	get "Mon" "Tue" as ENGLISH 3 letter string
*/
str0 date_glib::get_Tue()
{
	int dow = get_day_of_week();
	return Tue_from_GDateWeekday( (GDateWeekday) dow );
}

/*!
	get mm month number where Jan == 1
*/
int date_glib::get_mm()
{
	GDateMonth m1 = G_DATE_JANUARY;
	GDateMonth mm = g_date_get_month( & date ); 
	return (int)mm - (int)m1 + 1;
}

/*!
	get "Jan" "Feb" as ENGLISH 3 letter string

	This should be moved to an ENUM where the class-init
	declares each into the enum group
*/
str0 date_glib::get_Jan()
{
	GDateMonth mm = g_date_get_month( & date ); 
	switch( mm ) {
	 case G_DATE_JANUARY:	return "Jan";
	 case G_DATE_FEBRUARY:	return "Feb";
	 case G_DATE_MARCH:	return "Mar";
	 case G_DATE_APRIL:	return "Apr";
	 case G_DATE_MAY:	return "May";
	 case G_DATE_JUNE:	return "Jun";
	 case G_DATE_JULY:	return "Jul";
	 case G_DATE_AUGUST:	return "Aug";
	 case G_DATE_SEPTEMBER:	return "Sep";
	 case G_DATE_OCTOBER:	return "Oct";
	 case G_DATE_NOVEMBER:	return "Nov";
	 case G_DATE_DECEMBER:	return "Dec";

	case G_DATE_BAD_MONTH:;
	}
	return "-X-";
}


/*!
	convert int to string (ENGLISH etc)
*/
// static
str0 date_glib::Tue_from_GDateWeekday( GDateWeekday dow )
{
	switch( dow ) {
	case G_DATE_MONDAY:	return "Mon";
	case G_DATE_TUESDAY:	return "Tue";
	case G_DATE_WEDNESDAY:	return "Wed";
	case G_DATE_THURSDAY:	return "Thu";
	case G_DATE_FRIDAY:	return "Fri";
	case G_DATE_SATURDAY:	return "Sat";
	case G_DATE_SUNDAY:	return "Sun";

	case G_DATE_BAD_WEEKDAY: ;
	}
	return "-X-";
}

/*!
	move to the start of the week (possibly today)
*/
void date_glib::slide_back_GDateWeekday( GDateWeekday dow )
{
	GDateWeekday g_dow = g_date_get_weekday( & date );
	if( g_dow == dow ) return;
	int ndays = (int) g_dow - (int) dow;
	if( ndays < 0 ) ndays += 7;
	add_days( -ndays );
}


/*!
	BOOL: is Sat or Sun (or not)
*/
bool date_glib::is_weekend()
{
	switch(  g_date_get_weekday( & date ) )
	{
	 case G_DATE_SATURDAY:
	 case G_DATE_SUNDAY:
		return true;
	 default:
		return false;
	}
}

int date_glib::days_after_sunday()
{
	switch(  g_date_get_weekday( & date ) )
	{
	 case G_DATE_SUNDAY:	return 0;
	 case G_DATE_MONDAY:	return 1;
	 case G_DATE_TUESDAY:	return 2;
	 case G_DATE_WEDNESDAY:	return 3;
	 case G_DATE_THURSDAY:	return 4;
	 case G_DATE_FRIDAY:	return 5;
	 case G_DATE_SATURDAY:	return 6;
	 default:
		return 0;
	}
}
