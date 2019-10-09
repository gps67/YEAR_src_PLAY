#ifndef str1_H
#define str1_H

#include <stdlib.h> // malloc
#include "str0.h"

// str0.h // typedef unsigned char uchar;

/*!
	str1 holds a malloc'd pointer to a plain nul terminated C string

	copies of str1 malloc new copies, but str0 accesses within it

	LURK: str0 is very open, str1 is very closed. Adjacent.
	IE: be careful to not allow (default) operations to set
	the base class, because it must be malloc'd via set(str)
*/
class str1 : public str0
{
 public:
	// all assignment MUST go through this
	void set( const str0 & s );
	void set_NULL();
 
	//! constructor - sets itself to NULL
	str1() : str0()
	{
	}

	//! destructor - delete held string
	~str1()
	{
		if( str ) set_NULL();
	}

	//! constructor -
	str1( const str0 & s ) : str0()
	{
		if( s ) set( s );
	}

	//! constructor -
	str1( const str1 & s2 ) : str0()
	{
		if( s2 ) set( s2 );
	}

	/*
		Not sure about this, I want the caller to get
		easy readonly access to this, but not allow
		them to modify the string held.

		The str0 is already const uchar *, and
		its perfectly valid to use it in a S++ as long
		as this value remains unmodified

		GCC SAYS warning:
		conversion to a base class will never use a type conversion operator

		str0 is tuned to upgrade SIGNED char,
		when we already have UNSIGNED, we still need to avoid
		overloading. 

		ANYWAY: gcc says no operator baseclass required

	operator str0() { return (str0) (const char *)str; }
	*/

	//! assign new value
	void set( const str1 & s2 )
	{
		set( (str0) s2 );
	}

	//! assign new value
	void set( const str0 & s0, int len );

	//! assign new value
	str1 & operator=( const str0 & s )
	{
		set( s );
		return *this;
	}

	//! assign new value
	str1 & operator=( const str1 & s ) // I dont think this is necessary
	{
		set( s );
		return *this;
	}

};
#endif
