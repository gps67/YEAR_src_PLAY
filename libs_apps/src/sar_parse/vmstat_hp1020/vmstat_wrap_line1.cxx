#include <stdio.h>
#include "tm_parts.h"

#include "vmstat_wrap_line1.h"

/*!
	calculate T0 the start time of the command from the parsed fields

	NB This ignores the timezone, and uses local.
	If you are writing new data gatherers, use UST and presume UST
*/
time_t vmstat_wrap_line1::calc_t0()
{
	tm_parts tm;
	// struct tm tm;
	time_t t1;

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

/*!
	Parse the str for the expected fields, and put into lots of fields.
	Call calc_t0 and put into t0

# host57 # 0731 # vmstat 60 1440 # Sat Jul 31 00:00:01 BST 1999
*/
bool vmstat_wrap_line1::parse_line1( const char * str )
{
	const char * ptn;
	int t;
	time_t t1;

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
		&tz,	/* this is a problem - mktime uses tzname */
		&year
	);
	if (12 != t ) {
		fprintf(stderr,"ERROR line didnt match (%d of 12): '%s'\n", t, str );
		return false;
	}

	t1 = calc_t0();

	if( -1 == t1 ) {
		return false;
	}

	/* store the result */
	t0 = t1;

	return true;
}
