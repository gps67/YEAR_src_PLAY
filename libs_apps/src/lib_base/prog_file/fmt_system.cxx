
#include <stdarg.h>
#include <stdlib.h> // system
#include "fmt_system.h"
#include "buffer1.h"

// __attribute__((__format__ (printf, 1, 2))) // fn_WARN(a,b,c,d,fmt,..
bool fmt_system( const char * fmt, ... ) {
	va_list args;
	va_start(args, fmt);
	buffer1 cmd;
	int t = cmd.vprint( fmt, args );
	va_end(args);

	t = system( (STR0) cmd );
	if( t==-1 ) return FAIL("CMD %s", (STR0) cmd );
	if( t==0 ) return PASS( "OK %d = system( %s )", t, (STR0) cmd ); // log cmd
	if( t==0 ) return true;
	return FAIL("CMD %s RETVAL %d", (STR0) cmd, t );
}


