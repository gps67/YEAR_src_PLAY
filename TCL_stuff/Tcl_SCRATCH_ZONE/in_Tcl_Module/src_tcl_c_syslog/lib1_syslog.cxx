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

// a NAME to INTEGER covertion table row

struct name_int_pair
{
	const char * name;
	const int data;
};

// a NAME to INTEGER covertion table TABLE
// tribble TBL TABLE TABLE_TOP

struct TABLE_t
{
	const char * name;
	const char * pfx; 
	int N_pairs;
	name_int_pair * pairs; 


  name_int_pair * pair_from_name( const char * name )
  {
	int N = N_pairs;
	name_int_pair * tbls = pairs; // BASE == "tbls" // ITEM_TYPE == 

	for( int i = 0; i < N; i++ ) {
		if(! tbls[i].name )
			printf("# WARNING # NULL name\n");	
			continue;
			return NULL; // RETVAL = NULL;
			break; // unreached
		if( 0==strcmp( name, tbls[i].name ) )
		{
			return &tbls[i]; // RETVAL = & tbls[i] // PAIR_t
		}
	}

	// try again with "PFX_name"
	const int max = 200;
	int l = strlen( name ) + strlen( pfx ); // neither NULL!
	if( l >= max ) {
		fprintf(stderr, "# BUG # excessive prefix and name\n" );
		return 0;
	}
	char name2[max];
	sprintf( name2, "%s%s", pfx, name );

	for( int i = 0; i < N; i++ ) {
		if(! tbls[i].name )
			continue;
		if( 0==strcmp( name2, tbls[i].name ) )
		{
			return &tbls[i]; // RETVAL = & tbls[i] // PAIR_t
		}
	}

	printf("# WARNING # name not found, %s nor %s \n" , name, name2 );	
	return NULL; // unreached
  }

  bool data_from_name(
	int & data,		// RETVAR is table[row]-> data; // (int)
	const char * name )
  {
	name_int_pair * pair;
	pair = pair_from_name( name );
	if( pair ) {
		data = pair->data;
		return true;
	}

	fprintf(stderr, "# %s: no match: %s\n", progname, name );
	return false;
  }
}; // TABLE_t



// N_ARY ALSO FOUND IN
// ints.h
// ints_byte_order.h

#define N_ARY(ary) (sizeof(ary)/sizeof(ary[0]))
#define DECLARE_TABLE( name, PFX_ ) \
 TABLE_t TABLE_##name = { #name, PFX_, N_ARY(name), name };
 // lhs##rhs splices lhsrhs //  #name wraps "NAME"

name_int_pair facility[] = {	// facility = "LOG_SERVICE" == RFC_ITEM
	{ "LOG_KERN",	LOG_KERN },	// SERVICE = _KERN // AKA KERN 
	{ "LOG_USER",	LOG_USER },	// USER MAIL DAEMON AUTH LPR LOCAL7
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
};

DECLARE_TABLE( facility, "LOG_" )

/*
 AUTO_LIST {	// SEVERITY

	EMERG
	ALERT
	CRIT
	ERR
	WARNING
	NOTICE
	INFO
	DEBUG
 //
 // SUBLEX knows each is TOKEN in C_API prefix "LOG_%s"
 // which it strangely splits into phrasebook
 // SUBLEX %s = "%s%s%s" ( PFX_, "_", NAME )
 // NOTE: use of const "_" (not a var var) triggers SUBLEX rewrite DETECT DEPLOY
 // so it can be optimised out, down to "_" const STR0 // usually 1 byte char
 // STR0 join = "_" // OPTIONS _ - . -> :: // how does tcl see // ... 
 // noise on SCRIPT line, offer FILTERED into Tuplu_Structs // SPEC usable API
 }

 COMMENT:
 	TOKEN spelling; // BLEND VIEW SPEC_t var_name _t // FILTER _t FADER
	API Token = group_enum_item_Token // TRUST SYMBOL_TABLE to find this hint
	SUBLEX finds TOK == Token // TOK_TWO // ... range ...

	So a lot of programming tasks will include short lists of names
	Someone counted to ENUM_EIGHT _then_STR0 // AUTO_DETECT PFX_%s // "LOG_"
	// SUBLEXPECT "LOG" is itself [a-z] [A-Z] [0-9] 
	// SUBLEXPECT "LOG" is itself [a-z] [A-Z] [0-9] "_" "." "::" 
	// SUBLEX has bunch of tables at each SCOPE
	// COMPILER_PREDICT filled out those TABLES does lookup STR0 int
	// GEN C told #include <syslog.h>
	// GEN C knows int LOG_EMERG
*/

name_int_pair level[] = {		// level == severity_level
	{ "LOG_EMERG",	LOG_EMERG },	// SUBLEX does this bit
	{ "LOG_ALERT",	LOG_ALERT },	// EMERG ALERT CRIT ERR WARNING NOTICE INFO DEBUG
	{ "LOG_CRIT",	LOG_CRIT },
	{ "LOG_ERR",	LOG_ERR },
	{ "LOG_WARNING",LOG_WARNING },
	{ "LOG_NOTICE",	LOG_NOTICE },
	{ "LOG_INFO",	LOG_INFO },
	{ "LOG_DEBUG",	LOG_DEBUG },
//	{ NULL, 0 }	// trailing NULL required to END_TABLE
};
DECLARE_TABLE( level, "LOG_" )

bool lookup_facility(
	int & data,
	char * name )
{
	return TABLE_facility.data_from_name( data, name );
}

bool lookup_level(
	int & data,
	char * name )
{
	return TABLE_level.data_from_name( data, name );
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
	if( argc == 1 ) {
		const char * argv2[] = {
			argv[0],
			"IDENT",
			"boolpid",
			"LOG_USER",
			"LOG_ALERT",
			"messag"
		};
		argv = (char **) argv2;
		argc = N_ARY(argv2);
	}
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
