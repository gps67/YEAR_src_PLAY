#include "str1.h"
#include "p0p2.h"
#include "my_hash.h"
#include "buffer1.h"

// #include "dbg.h"

#define REF_IS_NULL(R) (((long) & R) == 0L )

//! clone a string
void str1::set( const str0 & s ) {
	if( str ) free( (void *) str );
	str = NULL;
	if( REF_IS_NULL( s )) {
		FAIL("NULL str0");
		return;
	}
	if( !s.str ) return;

	int l = strlen( (STR0) s.str );                                 
	str = (uchar *) malloc( l + 1 );                                        
	strcpy( (char *)str, (STR0) s.str );                             
}

//! clone a string
void str1::set( const str0 & s, int l ) {
	if( str ) free( (void *) str );
	str = NULL;
	if( !s.str ) return;

	// int l = strlen( (STR0) s.str );                                 
	str = (uchar *) malloc( l + 1 );                                        
	strncpy( (char *)str, (STR0) s.str, l );                             
	str[l] = 0;
}

void str1::set( const p0p2 & zone ) {
	set( zone.p0, zone.str_len() );
}

//! delete a string
void str1::set_NULL()
{
	if( str ) set( str0() );
}

/*!
	copy bytes without any cset convertion 
*/
void str1::set(blk1 & buf )
{
	set( buf.buff, buf.nbytes_used );

}

void str1::zap()  // erase password or other
{
	u8 * P = (u8*) str;
	if(!P) return;
	while(*P) {
		*P++ = 0;
	}
	set_NULL();
}

bool str1::print( const char * fmt, ... )
{
	buffer1 buf;
	va_list args;
	va_start( args, fmt );
	buf.vprint_8859_to_utf8( fmt, args );
	va_end( args );
	// now copy the buf to own malloc
	set( buf );
	// set has no bool to return (currently)
	return true;
}
