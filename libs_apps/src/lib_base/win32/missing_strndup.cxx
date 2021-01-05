#include <string.h>

#include "missing_strndup.h"
#include "dgb.h"
#include <stdlib.h> // malloc

#ifdef WIN32
// always - test it anyways
#endif

char * missing_strndup(const char *s, size_t n)
{
	size_t len = strlen(s);
	if( n < len ) len = n;
	char * s2 = (char *) malloc( len+1 );
	if(!s2) {
		FAIL("malloc");
		return NULL;
	}
	memcpy( s2, s, len );
	s2[len] = 0;
	return s2;
}

