#include "dgb.h"

// YOU CAN USE THIS WITHOUT THE REST OF libs_apps
// YOU MAY STATICALLY LINK IT (.o) WITH YOUR APPLICATION
// AS LONG AS YOU RESPECT THE REST OF THE LGPL

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#ifdef WIN32
#include "MY_WIN32.h"
#else
// #include <perror.h>
#endif
#include <errno.h>
#include <string.h>
// #include "gdb_invoke.h" // a conflicting progname system // LURK TODO
#include "dgb.h" // a conflicting progname system // LURK TODO

int complain_depth = 4;

static const char * prog_alias = 0;

// extern
void set_prog_alias( const char * s )
{
	if(!s) {
		s = get_prog_alias(); // from dgb.cxx
	}
	if( prog_alias == s ) return;
	if( prog_alias ) free( (void*) prog_alias );
	char * buf = (char*) malloc( strlen(s) + 1 ); // strdup()
	strcpy( buf, s );
	prog_alias = buf;
}

// extern
const char * get_prog_alias()
{
	if(! prog_alias ) set_prog_alias(0);
	return prog_alias;
}


bool ends_with_lf( const char * s )
{
	int l = strlen( s );
	if( !l ) return false;
	return s[l-1] == '\n';
}


void do_ERRNO()
{
#ifdef WIN32
	DWORD WIN32_err =  GetLastError();
	if(WIN32_err) {

		static const unsigned int sz = 400;
		char * buf[ sz ];

		DWORD dw = GetLastError(); 

		DWORD dwFlags = 0
//		|	FORMAT_MESSAGE_ALLOCATE_BUFFER
		|	FORMAT_MESSAGE_FROM_SYSTEM
		|	FORMAT_MESSAGE_IGNORE_INSERTS
		;
		LPCVOID lpSource = NULL;
		DWORD dwMessageId = dw;
		DWORD dwLanguageId = MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT);
		LPTSTR lpBuffer = (LPTSTR) &buf;
		DWORD nSize = sz;
		va_list *Arguments = NULL;


		FormatMessage(
			dwFlags,
			lpSource,
			dwMessageId,
			dwLanguageId,
			lpBuffer,
			nSize,
			Arguments
		);

		fprintf( stderr, "# WIN32 ERRNO # %d # %s", (int) dw, (const char *) buf );
		// NEWLINE ...
		// FORMAT_MESSAGE_ALLOCATE_BUFFER // LocalFree(lpDisplayBuf);
		SetLastError( 0 );
		if(!errno) {
			fflush(stderr);
			return;
		}
	}
#endif
	if(errno) {
		fprintf( stderr, "# UNIX/LIBC ERRNO # %d # %s\n", errno, strerror(errno) );
		errno=0;
		fflush(stderr);
	}
}

bool fn_WARN(
	const char * NOTE,
	int depth,
	const char * file,
	const char * func,
	const char * fmt,
	...
)
{
	if(!fmt) fmt="(null)";
	bool do_err = false;
	if( (NOTE[0] == 'P') && (0==strcmp(NOTE, "PERROR")) ) {
		// unix_errno = errno ...
		do_err = true;
	}
	if( depth > complain_depth ) return false; // always return false
	fprintf( stderr, "# %s # %s # %s(): ", get_prog_alias(), NOTE, func );
	fflush(0);
	va_list ap;
	va_start(ap, fmt);
	vfprintf( stderr, fmt, ap );
	va_end(ap);
	if( !ends_with_lf( fmt ) ) fprintf( stderr, "\n" );
	if( do_err ) do_ERRNO();
	fflush(stderr);
	return false;
}

// ... rest unused ...

bool fn_COMPLAIN1( const char * fmt, ... )
{
	fprintf( stderr, "# COMPLAIN-1 # " );
	fflush(0);
	va_list ap;
	va_start(ap, fmt);
	vfprintf( stderr, fmt, ap );
	va_end(ap);
	if( !ends_with_lf( fmt ) ) fprintf( stderr, "\n" );
	do_ERRNO();
	fflush(stderr);
	fflush(0); // ALL
	return false;
}

bool fn_COMPLAIN2( const char * fmt, ... )
{
	fprintf( stderr, "# 2 # " );
	fflush(0);
	va_list ap;
	va_start(ap, fmt);
	vfprintf( stderr, fmt, ap );
	va_end(ap);
	fflush(stderr);
	fflush(0); // ALL
	return false;
}

bool fn_COMPLAIN3( const char * fmt, ... )
{
	fprintf( stderr, "# 3 # " );
	fflush(0);
	va_list ap;
	va_start(ap, fmt);
	vfprintf( stderr, fmt, ap );
	va_end(ap);
	fflush(stderr);
	fflush(0); // ALL
	return false;
}

