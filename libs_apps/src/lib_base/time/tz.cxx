// #include <sys/types.h>
// #include <sys/stat.h>
// #include <fcntl.h>

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
// #include <errno.h>
// #include <string.h>

#include "dbg.h"
#include "tz.h"

#include "e_print.h"

/*
	man tzset says getenv("TZ") is EMPTY or "UST" or "..."
	else:
	/etc/localtime -> /usr/share/zoneinfo/Europe/London
*/

tz::tz() {
	save();		// this gets the process TZ

	char * TZ_env;
	TZ_env = getenv("TZ");
	if( !TZ_env )
		TZ_env = (char *)"";
	if( 0==strcmp( "", TZ_env  ) ) {
		DBG1( print_tz_vars(); )
		// set_TZ( "GMT" );
		set_TZ( TZ_LOCAL_LURK );
	} else if( 0==strcmp( "", tzname[0] ) ) {
		DBG2( fprintf(stderr,"PROG calling tzset()'\n"); )
		tzset();
		DBG1( print_tz_vars(); )
	}
}

const char * tz::TZ_SAVED = 0;

void tz::save()
{
	if( TZ_SAVED ) return;

	TZ_SAVED = getenv( "TZ" ); // COPY MEM ??
	if( TZ_SAVED ) {
		set_TZ( TZ_SAVED );
		return;
	}
	// do what for null TZ ??
	if(0)
		TZ_SAVED = "UST";
	else
		TZ_SAVED = TZ_LOCAL_LURK; //  "UK"
	set_TZ( TZ_SAVED );
}

bool tz::set_TZ( const char * str )
{
	save(); // if not already saved
//	if( !str ) return FALSE;

	// this static speeds up repeat setting of same value
	static const char * TZ_PREV = 0;
	if( TZ_PREV && str ) {
		if( TZ_PREV == str ) return TRUE;
		if( 0==strcmp( TZ_PREV, str )) return TRUE;
	}
	if(0) e_print("#  OK  # tz::set_TZ(%s)\n", str );
	TZ_PREV = str;	// LURK REFERENCE RETAINED

	// the other static holds the malloc variable
	static char * TZ_var = 0;
	char * TZ_old = TZ_var;

	int l = strlen( str ) + 4;
	TZ_var = (char *)malloc( l );
	if(!TZ_var) {
		fprintf(stderr,"malloc fail inside set_TZ\n");
		return FALSE;
	}
	sprintf( TZ_var, "TZ=%s", str );
	if( putenv( TZ_var ) ) {
		fprintf(stderr,"putenv fail inside set_TZ\n");
		return FALSE;
		// SLIGHT leak from TZ_old
	}
	if( TZ_old ) free( TZ_old );	

	tzset(); // no return value, use UTC if TZ not liked!
	DBG2( fprintf(stderr,"PROG setting TZ to '%s'\n", str); )
	DBG2( print_tz_vars(); )
	return TRUE;
}

void tz::print_tz_vars() {

	fprintf (stderr,"TZ: %s", getenv("TZ") );
	fprintf (stderr," tzname[0,1]: %s", tzname[0]);
	fprintf (stderr," / %s", tzname[1]);
	fprintf (stderr," daylight: %d", daylight);
	fprintf (stderr," timezone: %ld\n", timezone);
	// $TZ == "Europe/London" or "GB" or UK ??
	// tzname[0] = GMT = name used when not DST
	// tzname[1] = BST = name used when  is DST
	// daylight = 1 if DST when tzset ran (else 0)
	// timezone  = -3600 (relative to localtime - opposite of here)
	// GMT_real = GMT(localstr)+timezone
	// localstr = as_string( TIME - timezone ) (labelled DST)
}


// a single global instance
tz TZ;

