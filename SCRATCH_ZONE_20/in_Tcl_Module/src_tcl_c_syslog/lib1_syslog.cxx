/*
	to_syslog.c
	report an error via the syslog mechanism
	man syslogd
	man syslog
*/

#include <syslog.h>
#include <unistd.h> /* getopt */
#include <stdio.h> /* fprintf */
#include <stdlib.h> /* atoi */
#include <string.h> /* atoi */

static const char * progname = "PROGNAME";

// a NAME to INTEGER covertion table

typedef struct s_name_int_item
{
	const char * name;
	const int data;
} name_int_item;

name_int_item * ptr_from_name( name_int_item *tbl, const char * name )
{
	int pos = -1;
	while(1) {
		pos++;
		if(! tbl[pos].name )
			break;
		if( 0==strcmp( name, tbl[pos].name ) )
		{
			return &tbl[pos];
		}
	}
	return NULL;
}

int /* bool */ data_from_tbl_pfx_name(
	int & data,
	name_int_item *tbl,
	const char * pfx,
	const char * name )
{
	name_int_item * pair;
	pair = ptr_from_name( tbl, name );
	if( pair ) {
		data = pair->data;
		return 1;
	}
	const int max = 200;
	int l = strlen( name ) + strlen( pfx ); // neither NULL!
	if( l >= max ) {
		fprintf(stderr, "# BUG # excessive prefix and name\n" );
		return 0;
	}
	char name2[max];
	sprintf( name2, "%s%s", pfx, name );
	pair = ptr_from_name( tbl, name2 );
	if( pair ) {
		data = pair->data;
		return 1;
	}
	fprintf(stderr, "# %s: no match: %s\n", progname, name );
	return 0;
}


name_int_item tbl_facility[] = {
	{ "LOG_KERN",	LOG_KERN },
	{ "LOG_USER",	LOG_USER },
	{ "LOG_MAIL",	LOG_MAIL },
	{ "LOG_DAEMON",	LOG_DAEMON },
	{ "LOG_AUTH",	LOG_AUTH },
	{ "LOG_LPR",	LOG_LPR },
	{ "LOG_LOCAL0",	LOG_LOCAL0 },
	{ "LOG_LOCAL1",	LOG_LOCAL1 },
	{ "LOG_LOCAL2",	LOG_LOCAL2 },
	{ "LOG_LOCAL4",	LOG_LOCAL3 },
	{ "LOG_LOCAL4",	LOG_LOCAL4 },
	{ "LOG_LOCAL5",	LOG_LOCAL5 },
	{ "LOG_LOCAL6",	LOG_LOCAL6 },
	{ "LOG_LOCAL7",	LOG_LOCAL7 },
	{ NULL, 0 }
};


name_int_item tbl_level[] = {
	{ "LOG_EMERG",	LOG_EMERG },
	{ "LOG_ALERT",	LOG_ALERT },
	{ "LOG_CRIT",	LOG_CRIT },
	{ "LOG_ERR",	LOG_ERR },
	{ "LOG_WARNING",LOG_WARNING },
	{ "LOG_NOTICE",	LOG_NOTICE },
	{ "LOG_INFO",	LOG_INFO },
	{ "LOG_DEBUG",	LOG_DEBUG },
	{ NULL, 0 }
};

int /* bool */ lookup_facility(
	int & data,
	char * name )
{
	return data_from_tbl_pfx_name( data, tbl_facility, "LOG_", name );
}

int /* bool */ lookup_level(
	int & data,
	char * name )
{
	return data_from_tbl_pfx_name( data, tbl_level, "LOG_", name );
}

int bad_usage()
{
	fprintf(stderr, "usage: %s IDENT boolpid LOG_USER LOG_ALERT message\n", progname );
	fflush(NULL);
	return 22;
}

extern "C"
int syslog_main(int argc, char ** argv)
{
	progname = argv[0];
	if( argc !=6 ) {
		return bad_usage();
	}
	int option = 0;
	int facility = LOG_USER;
	int level = LOG_ALERT;
	char * ident = argv[1];
	int usepid = atoi( argv[2] );
	if( usepid ) option |= LOG_PID;
	if(!lookup_facility( facility, argv[3] )) return bad_usage();
	if(!lookup_level( level, argv[4] )) return bad_usage();
	openlog( "naff", option, facility );
	openlog( ident, option, facility );
	syslog( level, "%s", argv[5] );
	return 0;
}
