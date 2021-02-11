#ifndef OWN_STR_H
#define OWN_STR_H /* the holder of a malloc'ed string0 */

/*
	this is a simple string holder 
	no fancy concat functions
	or even str__len() (other than strlen)
*/

#include <string.h>
#include <malloc.h>
#include "p0p2.h"
#include "is_diff.h"

struct own_str
{
	char * str;

	own_str( void )
	{
		str = NULL;
	}

	void clear( void )
	{
		if(str) free( str );
		str = NULL;
	}
	own_str & operator=( const char * s1 )
	{
		clear();
		if(!s1) { return *this; } // clear did work
		int len = strlen( s1 );
		str = (char *)malloc( len+1 );
		if(!str) return *this ;
		memcpy( str, s1, len ); 
		str[len] = 0;
		return *this;
	}
	own_str & operator=( own_str & s1 )
	{
		this->operator=( s1.str );
		return *this;
	}
	own_str & operator=( p0p2 & p )
	{
		clear();
		str = p.str_cpy();
		return *this;
	}
	bool operator==( own_str & s2 )
	{
		return(0==strcmp(str, s2.str));
	}
	own_str( const char * s1 )
	{
		str = NULL;
		this->operator=( s1 );
	}

	own_str( p0p2 & p )
	{
		str = p.str_cpy();
	}

	void no_comma()
	{
		int len = str_len();
		char * p = str;
		char * p2 = p + len;
		while( p < p2 )
		{
			if( *p == ',' ) *p = ';';
			p++;
		}
	}

	bool is_negative( own_str & s2 )
	{
		double d1 = atof( str );
		double d2 = atof( s2.str );
		double adj = d1 + d2;
		if( adj >  0.001 ) return false;
		if( adj < -0.001 ) return false;
		return true;
	}

	own_str( own_str & s1 )
	{
		str = NULL;
		this->operator=( s1 );
	}

  int	str_len( void )
  	{
  		if(!str) return 0;
  		return strlen( str );
  	}
	
};

#endif
