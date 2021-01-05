#ifndef date_glib_H
#define date_glib_H

/*
	glib provides a neat 64-bit date holder, with some fields pre converted
	There is no APPARENT cache required, and this is a wrapper around it

	Invalid dates should never happen, but some value is used
	Caller should check bool return value

	some refactoring needed to remove glib enumerates for Jan, Tue

	some design to have a STORAGE date-int and an expanded one

	some adjustments to realign with time functions

	ENGLISH locale (:-I)
*/

#include "str1.h"
#include "ints.h"
#include <glib.h>
#include "buffer1.h"

/*!
	date_glib counts days (not seconds), and converts "1999-12-31"
*/
struct date_glib : public GRP_lib_base
{
	GDate date;
 public:
	// init to sane but null value
	date_glib();

	void clear() {
		g_date_clear( &date, 1 );
	}

// SET

	/*!
		constructor( 1999, 12, 31 );
	*/
	date_glib( int year, int mm, int dd )
	{
		g_date_clear( &date, 1 );
		set_year_mm_dd( year, mm, dd );
	}

	/*!
		constructor( "1999-12-31" );
	*/
	date_glib( const str0 s )
	{
		g_date_clear( &date, 1 );
		set_year_mm_dd( s );
	}

	/*!
		copy constructor
	*/
	date_glib( date_glib & rhs )
	{
		g_date_clear( &date, 1 );
		*this = rhs;
	}

	bool set_year_mm_dd( int year, int mm, int dd );
	bool set_year_mm_dd( const str0 s );
	bool set_dd_Jan_yy( const str0 s ); // English Jan,Feb,.. "31 Dec 99"
	void set_julian( i32 J);

// SUB-TYPES - ENUMERATES and PRIVATES

	// convert/extract the fields

	static bool parse_Jan( const str0 s, int & _mm );
	static str0 Tue_from_GDateWeekday( GDateWeekday dow );

	/*!
		PRIVATE - NOT THREAD SAFE
	*/
	const char * tmp_year_mm_dd();

// GET

	void get_year_mm_dd( int & year, int & mm, int & dd );
	void get_year_mm_dd( str1 & s );
	void get_year_mm( str1 & s );
	void get_dd( str1 & s );
	i32 julian();
	int get_day_of_week();
	str0 get_Tue();
	str0 get_Jan();
	int get_mm(); // Jan is 1
	bool is_weekend();

	/*!
		as an alternative to 1970, why not 2000-01-01
	*/
	i32 y2k_days() {
		return julian() - 730120; // hardcoded julian glib // LURK
	}

	/*!
		The difference between two date. (updates internal cache so not const)
	*/
	i32 operator-( date_glib & d2 ) {
		return julian() - d2.julian(); // julian loses const
	}

// CHANGERS

	/*!
	*/
	void add_days( i32 d ) {
		g_date_add_days( &date, d );
	}

	/*!
		standard assignment (not const because rhs might change)
	*/
	date_glib & operator=( date_glib & rhs )
	{
		set_julian( rhs.julian() );
		return *this;
	}

	/*!
		raise the date to be at_least rhs_bottom
	*/
	void at_least( date_glib & rhs )
	{
		int j1 = julian();
		int j2 = rhs.julian();
		if( j1 < j2 ) set_julian( j2 );
	}

	/*!
		lower the date to be at_most rhs_top
	*/
	void at_most( date_glib & rhs )
	{
		int j1 = julian();
		int j2 = rhs.julian();
		if( j1 > j2 ) set_julian( j2 );
	}

	int days_after_sunday();

	void slide_back_GDateWeekday( GDateWeekday dow );
	void slide_back_Mon() { slide_back_GDateWeekday( G_DATE_MONDAY ); }
	void slide_back_Tue() { slide_back_GDateWeekday( G_DATE_TUESDAY ); }
	void slide_back_Wed() { slide_back_GDateWeekday( G_DATE_WEDNESDAY ); }
	void slide_back_The() { slide_back_GDateWeekday( G_DATE_THURSDAY ); }
	void slide_back_Fri() { slide_back_GDateWeekday( G_DATE_FRIDAY ); }
	void slide_back_Sat() { slide_back_GDateWeekday( G_DATE_SATURDAY ); }
	void slide_back_Sun() { slide_back_GDateWeekday( G_DATE_SUNDAY ); }

// COMPARISONS

	IS_DIFF_t cmp( date_glib & date2 );

	/*!
	*/
	bool is_before( date_glib & date2 )
	{
		return IS_LESS == cmp( date2 );
	}

	/*!
	*/
	bool is_after( date_glib & date2 )
	{
		return IS_MORE == cmp( date2 );
	}

	/*!
	*/
	bool operator==( date_glib & date2 )
	{
		return IS_SAME == cmp( date2 );
	}

	/*!
	*/
	bool operator!=( date_glib & date2 )
	{
		return IS_SAME != cmp( date2 );
	}

	/*!
	*/
	bool operator>=( date_glib & date2 )
	{
		return IS_LESS != cmp( date2 );
	}

	/*!
	*/
	bool operator<=( date_glib & date2 )
	{
		return IS_MORE != cmp( date2 );
	}

	/*!
	*/
	bool operator<( date_glib & date2 )
	{
		return IS_LESS == cmp( date2 );
	}

	/*!
	*/
	bool operator>( date_glib & date2 )
	{
		return IS_MORE == cmp( date2 );
	}

// MISC

 static
	void test();

};
#endif
