#include "date_glib.h"
#include <stdio.h>

//! some test function
void date_glib::test()
{
	i32 d_y2k = 0;
	date_glib d1;
	d1.set_year_mm_dd( 2000, 01, 01 ); // not octal
	d_y2k = d1.julian();
	fprintf( stderr, "d_y2k = %d\n", d_y2k );

	str0 dates[] = {
		"2000-01-01",
		"1999-12-31",
		"1999-01-01",
		"2001-01-01",
		"2000-06-01",	// summer time
		"1999-06-01",	// summer time -ve
		"fudgecake",
		"2000-13-01",	// glibc seems forgiving about simple math - glib presumes precision
		"2000-01-32",
	};

	int N = (sizeof(dates) / sizeof( char *) );

	for( int i = 0; i < N; i++ ) {
		str0 s = dates[i];
		date_glib d;
		if( !d.set_year_mm_dd( s ) ) {
			fprintf( stderr, "CONV ERROR on '%s'\n", (STR0) s );
			if(0) continue;
		}
// OK //	fprintf( stderr, "date: '%s' -> %4d days\n", (STR0) s, d.julian()-d_y2k );
		fprintf( stderr, "date: '%s' -> %4d days\n", (STR0) s, d-d1 );
		fprintf( stderr, "date: '%s' -> %4d days\n", (STR0) s, d.y2k_days() );

		int year,mm,dd;
		d.get_year_mm_dd( year, mm, dd );
		fprintf( stderr, "---->  %4.4d-%2.2d-%2.2d\n", year,mm,dd);

		d.add_days(1);
		str1 s1;
		d.get_year_mm_dd( s1 );
		fprintf( stderr, "-+1->  %s\n", (STR0) s1 );
	}
}
