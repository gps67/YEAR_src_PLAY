
#include "str_builder.h"
#include "string.h"

//! destructor - default
str_builder::~str_builder()
{
}

//! constructor - with initial space for sz bytes
str_builder::str_builder( int sz )
: buff( sz )
{
}

//! append string to builder buffer
void str_builder::append( str2 * s )
{
	if(!s)
		append( " /*STR2-NULL*/ " );
	else
		append( (str0) *s );
}

//! append string to builder buffer
void str_builder::append( str0 s )
{
	if(!s) s = (str0) " /*STR-NULL*/ ";
	int len = s.str_len();
	buff.put_nn_bytes( len, s );
}

//! append char to builder buffer
void str_builder::append( char c )
{
	buff.put_byte( (uchar) c );
}

//! append NUL char to builder buffer
void str_builder::add_nul()
{
	buff.trailing_nul();
}

//! extract a new str2 from the buffer
str2 * str_builder::get_str2()
{
	return new str2( sget() );
}

//! extract access to the existing buffer, with a trailing NUL
uchar * str_builder::get()
{
	buff.trailing_nul();
	return buff.buff;
}
void str_builder::clear()
{
	buff.clear();
}
