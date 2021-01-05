// #include <sys/types.h>
// #include <sys/stat.h>
// #include <fcntl.h>

// #include <stdlib.h>
#include <stdio.h>
// #include <time.h>
// #include <errno.h>

// #include "misc.h"
//#include "array_base_1248.h"

// #include "vm18int.h"
#include "tm_parts.h"

#include "line1.h"

time_t line1::calc_t0()
{
	tm_parts tm;
	// struct tm tm;
	int t;
	int d;
	time_t t1;
	int MM;
	char * fmt;
	char str_time[40];
	char * TZ_env;

	/* reverse order */
	tm.year( year );
	tm.month( jan ); // LURK no error detected
	tm.dd(	 DD );
	tm.hour( hh );
	tm.min(	 mm );
	tm.sec(	 ss );
	/* empties */
	// tm.tm_wday = -1;
	// tm.tm_yday = -1;
	// tm.tm_isdst = -1;
	tm.sub_zone( tz );

/*
	Strangely enough this works! But tm_parts is on QUIETLY a tm !
	t1 = time = mktime( &tm );	
*/
	t1 = tm.time_from_local();

	if( -1 == t1 ) {
		fprintf(stderr,"ERROR time convertion failed, errno = %d\n", errno );
		if( errno ) perror ("ERROR time convertion failed\n" );
		// tm_print( &tm );
		return -1;
	}

	/* ctime adds \n but doesnt show GMT/BST */
	fprintf(stderr,"Convertion(%d) done with TZ(%s) SUB(%s) %s\n", t1, getenv("TZ"),
		tz,
		tm.str_fmt1() );
		// ctime( &t1 ) );

	return t1;
}

int line1::parse_line1( const char * str )
{
	char * ptn;
	int t;
	time_t t1;
	struct tm tm;
	struct tm *tm2;

/* this is not thread safe - global var TZ etc */
/*
# medprod # 0731 # vmstat 60 1440 # Sat Jul 31 00:00:01 BST 1999
*/
// DEb_BUG 0728 is a bad octal number

	ptn = "# %10s # %s # vmstat %i %i # %3s %3s %i %i:%i:%i %4s %i";
	t = sscanf( str, ptn, 
		&hostname,
		&MMDD,
		&dt,
		&nn,
		&tue,
		&jan,
		&DD,
		&hh,
		&mm,
		&ss,
		&tz,	/* this is the problem - mktime uses tzname */
		&year
	);
	if (12 != t ) {
		fprintf(stderr,"ERROR line didnt match (%d of 12): '%s'\n", t, str );
		return 0;
	}

	t1 = calc_t0();

	if( -1 == t1 ) {
		return 0;
	}

	/* store the result */
	t0 = t1;

	return 1;
}
/*
	sed 5q vmstat_medprod_0731
# medprod # 0731 # vmstat 60 1440 # Sat Jul 31 00:00:01 BST 1999
*/
