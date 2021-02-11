#ifndef PERR_H
#define PERR_H /* print errno alng with a message */
#include "dgb_fluff_v9.h"
#include "dgb_print_v9.h"

#include <stdlib.h>
#include <string.h>
#include <errno.h>
#pragma interface

struct perr : public dgb_print
{
	char buff[200];	/* allocated on stack */

//	void	dgb_sprintf( char * fmt, ... )
	perr( char * fmt, ... )
	{
		va_list ap;
		va_start(ap, fmt);
		(void) vsprintf( buff, fmt, ap);
		va_end(ap);
		perror( buff );
	}

	perr( char * str )
	{
		perror( str );
	}
};
#endif
