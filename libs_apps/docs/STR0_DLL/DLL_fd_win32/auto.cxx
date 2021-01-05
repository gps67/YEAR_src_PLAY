#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <windows.h>
#include "win_main.h"
#include "dgb.h"
#include "WIN32_stuff.h"

#define NOISE if(1)

//
// Your prefix dir here
//
#define APPNAME "appname"

/*

	STR0_win32_DLL {

	auto.exe uses (and is returning) code from STR0_win32_DLL

	things like: get current working dir + drive
	things like: knowing where app puts the DLLs
	things like: setting PATH for session
	things like: console for session
	things like: un-dis-attaching-console
	things like: parsing_ARGV_from_command_line
	things like: win32_CreateProcess
	things like: win32_HANDLE
	things like: win32_EVENT
	}



*/

char get_drive_letter()
{
	char e_drive_letter = 'A';
	char BUFF[ 4096 ];
	int BUFFLEN = sizeof( BUFF );
	int len = GetCurrentDirectory( BUFFLEN, BUFF );
	BUFF[len] = 0;
	if( BUFF[1] == ':' ) {
		e_drive_letter = BUFF[0];
NOISE		printf( "# OK # GetLogicalDirectory = %s\n", BUFF );
	}
	else {
		printf( "# FAIL # PAILED TO GET A DRIVE LETTER\n");
		printf( "# FAIL # GetLogicalDirectory = %s\n", BUFF );
	}
	return e_drive_letter;
}

int prefix_PATH( char e_drive_letter )
{
	char PATH2[ 4096 ]; // LURK also env var space problems
	sprintf( PATH2, "PATH=%c:\\%s\\gtk\\bin;%c:\\%s\\bin;%s",
		e_drive_letter,
		APPNAME,
		e_drive_letter,
		APPNAME,
		getenv( "PATH" )
	);
	if( putenv( PATH2 ) ) {
NOISE		printf( "# FAIL # calling putenv( PATH2 )\r\n#\r\n#\t%s\r\n#\r\n", PATH2 );
		return 0;
	} else {
NOISE		printf( "calling putenv( PATH2 )\r\n#\r\n#\t%s\r\n#\r\n", PATH2 );
	}
	return 1;
}

/*!
	execvp_con(
		"app_viewer_3",
		argv/* -parameters-file '%s' */,
		console_suppress = true
		console_extra = false
	) is almost exactly like execvp(progname,argv))
	except maybe argv_0 constraint
	except added parameters and code
	except dont ask too much from exec
	except dont ask win32 to fake fork without allowing threads
	and the usual exex'd issues, using win32_cred to carry pipes
	its not completely secure, but its convenient to use a standard viewer
	on unix (and inetd) in_out_err without maybe tty
	on win32 use CreateProcess

	needs refactor to use STR0_DLL_win32 functions
*/
bool execvp_con(
	const char * argv_0,
	char ** argv,
	bool console_suppress,
	bool console_extra		// create a console ? HOW ?
)
{
	int len = 0;
	int i = 0;

	if( argv_0 ) {
		if( argv_0 != argv[0] ) {
			FAIL("argv_0 is only there for UNIX compat reasons" );
			INFO("proceeding anyway");
		}
	}
	while( argv[i] ) {
		len += strlen( argv[i] ) + 1;
		/*
			if quotes needed - add that here


			this will fail if they were needed
			as they ae not added
			nor even detected as needed

			you cannot have a space in the program\ files! 
		*/
		len += 2; // allow for quotes
		i++;
	}
	len += 1; // NULL TERM

	TRACE_1( "buf[ len=%d ]", len );

	char buf[ len + 1 ];
	char * P = buf;
	char * P2 = buf+len;
	char sep = 0;
	i = 0;
	while( argv[i] ) {
		if( sep ) *P++ = sep;
		sep = ' ';
		int avail = P2 - P;
		if( avail <= 0 ) {
			FAIL( "avail <=0 ## CODING ERROR");
			exit(23); // internal quoting error
		}
		int used = strlen( argv[i] );
		memmove( P, argv[i], used );
		TRACE_1( "buf[] += argv[i] = '%s'", argv[i] );
		P += used;
		i++;
	}
	*P = 0; // terminating NUL

	TRACE_1( "buf[] = '%s'", buf );

	WIN32_CreateProcess_base proc;
	proc.lpCommandLine = buf;

//	proc.set_no_console_vpn();
	if( console_suppress ) proc.set_CREATE_NO_WINDOW();
//	if( console_suppress ) proc.set_HIDE(); // lost gtk toplevel
	if( console_extra ) proc.set_CREATE_NEW_CONSOLE();
	// there is also a speed issue - openvpn must open early
	// there is also ...

	if( !proc.call_CreateProcess() ) return false;
	TRACE("Waiting");
	proc.wait_forever_for_child_exit();

	return true;
}


extern "C"
int no_win_main( int argc, char ** argv )
{
	int i = 0;
	int j = 0;

	bool console_extra = false;
	bool console_suppress = false;

	INFO("startup");
// PATH
	char e_drive_letter = get_drive_letter();
	prefix_PATH(  e_drive_letter );

// other ENV
	chdir( ".." );		// for when run from appname/bin
	chdir( "appname" );	// for when run from /

// copy argv through - NB _DOWN_ is required - cannot insert new args here

	i++; // skip "auto.exe"

// initial -parameters for auto (not for subsequent exe)
	for( ; i< argc; i++ ) {
		if( argv[i][0] != '-') {
			break; 	// it is the new exe name
		}
		if( 0==strcmp(argv[i], "-console_suppress")) {
			console_suppress = true;
			console_extra = false;
			printf( "argv[-]='%s' # auto will not suppress a console\n", argv[i] );
			continue;
		}
		if( 0==strcmp(argv[i], "-console_extra")) {
			console_extra = true;
			console_suppress = false;
			printf( "argv[-]='%s' # auto will not suppress a console\n", argv[i] );
			continue;
		}
		if( 0==strcmp(argv[i], "-log")) {
			// other option to auto
			const char * filename = argv[i];
			if( (!filename ) || ( filename[0]=='-') ) {
				printf( "-log filename# missing filename \n" );
				exit(22);
			}
			i++;
			continue;
		}
		if( 0==strcmp(argv[i], "-gdb")) {
			// special case for me
			// force copy options until "--"
			argv[j++] = "c:/msys/1.0/dgb/bin/insight.exe";
			// argv[j++] = "--args";
			while(1) {
				i++;
				if( !argv[i] ) break;
				if( 0 == strcmp(argv[i], "--" )) break;
				argv[j++]=argv[i];
			}
			i--;
			continue;
		}
		if( argv[i][0] == '-') {
			printf( "option not recognised: '%s'\n", argv[i] );
			exit(22);
		}
		break; // all options parsed
	}

	// copy down exe name and all its args
	for( ; i< argc; i++ ) {
		printf( "argv[%d]='%s' \n", i, argv[i] );
		argv[j] = argv[i];
		j++; // i++ is in loop
	}
	argv[j] = NULL;

	printf( "# (last one should be NULL) \n" );

	if( !argv[0] ) {
		argv[0] = "vnc_connect_F12" ;
		argv[1] = NULL;
		printf( "Changing argv[] to default %s\r\n", argv[0] );	
	}

NOISE	printf( "calling %s\r\n", argv[0] );
	execvp_con( argv[0], argv, console_suppress, console_extra );
	// its not exec - it always returns
	// here when -wait isnt provided
     	// printf( "exec returned - maybe file %s not found \r\n", argv[0] );
	TRACE("Process/WAIT returned");

//	sleep(5);
	exit(0);
}
