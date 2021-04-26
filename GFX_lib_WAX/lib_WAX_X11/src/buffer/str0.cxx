
#include "str0.h"
#include "e_print.h"
#include "my_hash.h"
#include <stdlib.h> // strtod
#include <stdio.h> // perror

// #include "dbg.h"

/*!
	iterate over string generating its hash key
*/
int str0::get_hash_key() {
	my_hash hash;
	if(!str) return hash.hash;
	const uchar * s = str;
	while( uchar c = *s++ ) { // unsigned is significant
		hash.incl( c );
	}
	hash.incl( 0 ); // and the null ?
	return hash.hash;
}

/*!
	convert string to a float (or return false)
*/
bool str0::as_float( float & f ) const
{
	double d;
	bool ok = as_double( d );
	if(!ok) return false;
	f = d;
	return true;
}

/*!
	convert string to a double (or return false)
*/
bool str0::as_double( double & f ) const
{
	if( ! str ) return false;
	const char * s1 = (const char *) str;
	char * s2 = (char *) s1;

	double f2 = strtod( s1, &s2 );

	if( s1 == s2 ) 
	{
		perror(s1);
		e_print("(%s)as_double() - %m\n", s1 );
		return false;
	}
	/*
		overflow is unlikely
	*/
	f = f2;
	return true;
}

/*!
	convert string to an int, NOT OCTAL or leading zeros 08 is OK
*/
bool str0::as_int( int & i ) const // decimal only
{
	return as_int_from_base( i, 10 );
}

/*!
	convert string to an int, OCTAL from leading zeros: 017 is 15
*/
bool str0::as_int_from_anybase( int & i ) const // users wont want octal?
{
	return as_int_from_base( i, 0 );
}

/*!
	convert string to an int, base 0 means auto detect
*/
bool str0::as_int_from_base( int & i, int base ) const
{
	const char * s1 = (const char *) str;
	      char * s2 = (char *) s1;
	long int ret = strtol( s1, &s2, base );
	if( s1 == s2 ) 
	{
		perror(s1);
		e_print("(%s)as_int_from_base(%d) - %m\n", s1, base );
		return false;
	}
	if( !base ) // auto detect
		if( *s2 == '8' || *s2 == '9' ) // leading zero looked like octal
		{
			ret = strtol( s1, &s2, 10 );
		}
	i = ret; // int is not long, even if usually 32 bit ;-)
	return true;
}

////////// un certain

#ifdef WITH_STR2
#include "str2.h"

str0::str0( const str2 * s2 )
{
	if( s2 )
		str = s2->str;
	else
		str = (str0_const uchar *) "";
		// str = NULL;
}
#endif


#if 0
//! is string NULL or empty
str0::operator bool() const
{
	/*
		somehow this is never called, probably prototype form
		recompile means it gets called - by str1 ??
	*/
//	if(!( str && *str )) e_print("str0 IS false\n"); else e_print("+");
	return str && *str;
}
#endif

bool str0::operator==( const str0 & s ) const
{
	if( s.str == str ) return true;
	if( !str || ! s.str) return false;
	return (0==strcmp( (STR0) str, (STR0) s.str ));
}

