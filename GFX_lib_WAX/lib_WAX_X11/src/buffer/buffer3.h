#ifndef buffer3_H
#define buffer3_H

#include "buffer2.h"
#include "str1.h"

#include <stdargs.h>

/*!
*/
struct buffer3 : public buffer2
{
/*!
*/
	buffer3( int N ) : buffer1( N ) { /* ctor */ }
/*!
*/
	~buffer3() { /* dtor */ }

/*!
*/
	bool print( const char * fmt, ... )
	{
	}

};

#undef Put
#undef Get_space

#endif

