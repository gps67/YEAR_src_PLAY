#include "str1.h"
#include "my_hash.h"

// #include "dbg.h"


//! clone a string
void str1::set( const str0 & s ) {
	if( str ) free( (void *) str );
	str = NULL;
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

//! delete a string
void str1::set_NULL()
{
	if( str ) set( str0() );
}
