#ifdef WIN32 
/*
// win_main.c
// THIS SNIPPET (C) GPS - freely re-usable
//
// To run without a console, under mingw32, as a WIN32 sort of program
// you use this instead of main()
// it calls no_win_main( argc, argv )
//
// TODO: keep the argv[0] somewhere (if you dont do that with main()))
// TODO: keep the E: drive letter 
// TODO: some first steps, cross_platform, approach
// TODO: code against MAX argc, in a hostile environment, with utf8, etc
//
*/

// to use ATTACH_PARENT_PROCESS, ... or later version ...
#define _WIN32_WINNT 0x0501

#include <windows.h>
#include "win_main.h"
#include "dgb.h"
// #include "word_splitter.h"
// extern int main( int, char ** );
// extern "C" int no_win_main( int, char ** );

// these are quite wrong ...
#define MAXPATHLEN 300
#define ARGC_MAX 33
#define NUL 0 // char byte

int WINAPI
WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nShowCmd
)
{
	int argc = 0;
	int argc_limit = ARGC_MAX;
	char * argv[ argc_limit + 1 + 2 ]; // trailing NULL plus XS of 2

	// argv[0] = F:\somewhere\bin\progname.exe
        TCHAR program[MAXPATHLEN];
        GetModuleFileName( hInstance, program, sizeof(program) );
	argv[argc++] = program;

	TCHAR * last_dot = strrchr( program, '.' );
	TCHAR * last_slash = strrchr( program, '\\' ); // backslash
	if( last_dot ) { * last_dot = 0; }
	if( last_slash ) {
		set_prog_alias( last_slash+1 );
	} else
		set_prog_alias( program );
	if( last_dot ) { * last_dot = '.'; }

	// if we dont have a process
	// and we have not yet found a reason not to
	// attach to our parents
	//
	// maybe check to see if we have one first

       AttachConsole( ATTACH_PARENT_PROCESS );

# if 1

	//
	// start to parse the command line, using unsigned char casting
	//
	TRACE_2("# ( %s %s ) \r\n", program, lpCmdLine );

	// P points to argv[i]byte[+]
	// P parses over argv received as text line
	// plain assignment - should not raise warning
	// forced cast for unsigned reasons, doesnt hide a bad cast
	//
	char * sP = lpCmdLine; 
	unsigned char * P = (unsigned char *) sP;
	//
	// WIN32 has already stripped out leading spaces
	// anything else results in an empty first parameter
	// after that there is not 'quote' interpretation
	// not like g_lib which needs backslashes surrounded in quotes
	//
//	// no need
//	while( (*P) && (*P <= ' ')) P++;

	//
	// loop over each word found (upto NUL)
	//
	while(*P) {
		// simple code - limited argv
		if( argc == argc_limit ) {
			INFO("# FAIL # argc limit exceeded\r\n");
			// exit // user is malicious ?
			break;
		}

		// record start of word in P1
		unsigned char * P1 = P;

		// step over plain letters of word (uchar)
		while( *P > ' ') P++;

		// record end of word = start of spaces [P0[P2
		// skip over any spaces before overwriting the NUL byte
		// terminate the in-situ word
		// seems ineviatable benefit that lpCmdLine is writeable copy
		unsigned char * P2 = P;
		// skip over trailing spaces
		while( (*P) && (*P <= ' ')) P++;
		// leave P pointing to first letter of next argv or NUL
		// P1+n=P2 truncate_at_NUL to reuse ARGV memory
		*P2 = NUL;

		// place the in-situ-copy in argv[i]
		argv[ argc ++ ] = (char *) P1;

		// argc-1, argv[argc-1] );
		INFO_2("argv[%d] <== '%s'\r\n", argc-1, P1 );
	}

#else
	// just put your own parameters in here
	// maybe copy program through
	argv[argc++] = "FAKE";
#endif
	// set_argv_trailing_NULL // safety pos at end of list
	argv[argc] = NULL;

	// ACTUALLY ITS MAIN THAT CALLS WINMAIN
	// SO CALLING IT BACK JUST MAKES A LOOP ;-)
	// maybe that one is linked in from a library
	// and I cant see how to stop mingw doing its own things
        // DONT // return( main( argc, argv ) );
	//
        return( no_win_main( argc, argv ) );
}
# else // NOT WIN32

// UNIX 
#include "dgb.h"
#include "win_main.h"
#include <string.h>
	// nothing - call no_win_main() as if it had been main()

	extern "C" int main( int argc, char ** argv );

	int main( int argc, char ** argv )
	{
		char * program = argv[0];
		char * last_dot = strrchr( program, '.' );
		char * last_slash = strrchr( program, '/' ); // unixslash
		if( last_dot ) { * last_dot = 0; }
		if( last_slash ) {
			set_prog_alias( last_slash+1 );
		} else
			set_prog_alias( program );
		if( last_dot ) { * last_dot = '.'; }

		return no_win_main( argc, argv );
	}

#endif /* WIN32 */
