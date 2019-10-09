#ifndef str0_H
#define str0_H

#include <string.h>
#include <stdlib.h> // malloc

#include "misc.h"
#include "is_diff.h"

typedef unsigned char uchar;
typedef const char * STR0;

#ifndef e_print_H
#include "e_print.h"
#endif

	// invoke C++ cast for fprintf
//#define STR0 (const char *)

/*!
	str0 is a plain C-string value, without any malloc

	Its a variable, its a register copy, not a full variable
	It is SUPPOSED to be a single pointer value, no extras.

	It opens the door to strcmp() using locale info

	It nails down problems with (char *) (uchar*),
	mostly wrapping strchr() strcmp() etc

	Its suposed to be almost like:  typedef const uchar * str0_;
	but it doesnt quite substitute, mostly because of TRAD-C's
	definition of (char) when we want (uchar), cros-product-multiplied
	by (const char) or variable, and the need to
	put in too many casts (or get ambiguous resolutions).
	Intensive functions may require lots of pre-written
	expressions, like: str0 p2 = p1.after_non_spaces();

	It has extra features such as if(!str.as_double(dbl)) ...
*/
/*
	you have the choice of (uchar * str) or (const uchar * str )
*/
#define str0_const /* NOT const */


class str2;

class str0
{

 public:

	str0_const uchar * str;

	/*!
		str0 is a value not a variable
	*/
	str0()
	{
		str = NULL;
	}

	//! destructor - no delete, just drop register
	~str0()
	{
		// temp without delete
	}

	//! constructor - assign from plain C string
	str0( const char * s )
	{
		// NB loses const
		str = (str0_const uchar *)s;
	}

	//! constructor - assign from plain C string
	str0( const uchar * s )
	{
		// NB loses const
		str = (str0_const uchar *)s;
	}

 	//! constructor - extract (uncopied) plain C string from string
	str0( const str0 & s2 )
	{
		str = s2.str;
	}

	// this is suppposed to be inline - but ...
	//! is string NULL or empty
#if 0
	operator bool() const;
#else
	operator bool() const
	{
	//	if(!( str && *str )) e_print("str0 IS false\n"); else e_print("+");
		return str && *str;
	}
#endif

	bool is_empty() const
	{
		return !(str && *str);
	}

	bool isnt_empty() const
	{
		return (str && *str);
	}

	// gchar is char

	//! str0 can be used where you use char *
	operator str0_const char *() const
	{
		return (str0_const char *)str;
	}

	//! str0 can be used where you use char *
	operator str0_const uchar *() const
	{
		return (str0_const uchar *)str;
	}

	//! str0 can be used where you use char *
	operator const void *() const
	{
		return (const void *)str;
	}

	//! get first char
	uchar operator*() const
	{
		return *str;
	}

#if 0
	str0 operator ++ (int x)
	{
		str0 s = *this; str++; return s;
	}

	str0 operator -- (int x)
	{
		str0 s = *this; str--; return s;
	}

	int operator - ( str0 p2 )
	{
		return str - p2.str;
	}
#endif

/*
	This is ambiguous, is it the pointer or the value ?
	bool operator < ( str0 p2 ) { return str < p2.str; }
	bool operator > ( str0 p2 ) { return str > p2.str; }
	bool operator >= ( str0 p2 ) { return str >= p2.str; }
	bool operator <= ( str0 p2 ) { return str <= p2.str; }
*/

	//! str0 has no copied data, just a pointer
	str0 & operator=( const str0 & s2 )
	{
		str = s2.str;
		return *this;
	}

	//! str0 has no copied data, just a pointer
	str0 & operator=( const char * s2 )
	{
		// NB lose const
		str = (str0_const uchar *) s2;
		return *this;
	}

	//! Nice Macro - calls strcmp - this does get called
	bool operator==( const char * s ) const
	{
		return operator==( (str0) s );
	}

	//! Nice Macro - calls strcmp
	bool operator==( const uchar * s ) const
	{
		return operator==( (str0) s );
	}

	bool operator==( const str0 & s ) const;
#if 0
	//! Nice Macro - calls strcmp
	bool operator==( const str0 & s ) const
	{
		if( s.str == str ) return true;
		if( !str || ! s.str) return false;
		return (0==strcmp( (STR0) str, (STR0) s.str ));
	}
#endif

/*
	These reduce the number of (ambiguous) casts
*/
	//! Nice Macro - calls strcmp
	bool operator!=( const char * s ) const
	{
		return !operator==( s );
	}

	//! Nice Macro - calls strcmp
	bool operator!=( const uchar * s ) const
	{
		return !operator==( s );
	}

	//! Nice Macro - calls strcmp
	bool operator!=( const str0 & s ) const
	{
		return !operator==( s );
	}

	//! get char
	str0_const uchar & operator[]( int pos )
	{
		return str[pos];
	}

	str0_const uchar * uget() const
	{
		return  str ;
	}

	str0_const char * sget() const
	{
		return (str0_const char *) str ;
	}

	//! Nice Macro - calls strcmp
	int str_cmp( const str0 & s2 ) const
	{
		if(!str) return -1;
		if(!s2.str) return +1;
		return strcmp( (STR0) str, (STR0) s2.str );
	}

	//! Nice Macro - calls strcmp
	int str_cmp_n( const str0 & s2, int n ) const
	{
		if(!str) return -1;
		if(!s2.str) return +1;
		return strncmp( (STR0) str, (STR0) s2.str, n );
	}

	//! Nice Macro - calls strcmp
	bool has_prefix( const str0 & pfx ) const
	{
		return( 0==str_cmp_n( pfx, pfx.str_len() ) );
	}

	//! Nice Macro - calls strlen
	int str_len() const
	{
		if(!str) return 0;
		return strlen( (char *)str );
	}

	//! point to the NULL at the end
	str0 str_end() const
	{
		if(!str) return str0();
		return str_len() + str;
	}

	//! point to the char before NULL at the end
	str0 str_last() const
	{
		if(!str) return str0();
		int l = str_len();
		if(!l) return str;
		return str_len() + str - 1;
	}

	//! Nice Macro - calls strchr
	str0 str_chr( char ch ) const
	{
		if(!str) return str0();
		return strchr( (char *)str, ch );
	}

	//! Nice Macro - calls strrchr
	str0 str_rchr( char ch ) const
	{
		if(!str) return str0();
		return strrchr( (char *)str, ch );
	}

	//! Nice Macro - calls strchr
	str0 str_chr( uchar ch ) const
	{
		if(!str) return str0();
		return strchr( (char *)str, (char) ch );
	}

	//! compare - returning standardised LESS/SAME/MORE
	IS_DIFF_t cmp ( const str0 & s2 ) const
	{
		if( str == s2.str ) return IS_SAME; // even if both NULL
		if( !str ) return IS_LESS;
		if( !s2.str ) return IS_MORE;

		int t = str_cmp( s2 );
		if( t < 0 ) return IS_LESS;
		if( t > 0 ) return IS_MORE;
		return IS_SAME;
	}

/*
	//! compare - returning standardised LESS/SAME/MORE
	IS_DIFF_t cmp ( const str0 * s2 ) const
	{
		if(!s2) return IS_MORE;
		return cmp( *s2 );
	}
*/

	//! Nice Macro - calls strcasecmp - ignores case
	bool IS_same_AS( const str0 & s2 )
	{
		return 0 == strcasecmp( (STR0) str, (STR0) s2 );
	}

	//! Nice Macro - calls strcmp
	bool is_same_as( const str0 & s2 ) const
	{
		return (0==strcmp( (STR0) str, (STR0) s2.str ));
	}

/*
	//! Nice Macro - calls strcmp
	bool is_same_as( const char * s2 ) const
	{
		// NULL ??
		return (0==strcmp( (STR0) str, (STR0) s2 ));
	}

	//! Nice Macro - calls strcmp
	bool is_same_as( const uchar * s2 ) const
	{
		// NULL ??
		return (0==strcmp( (STR0) str, (STR0) s2 ));
	}
*/

	bool	as_double( double & f ) const;
	bool	as_float( float & f ) const;

	bool as_int( int & i ) const; // decimal only
	bool as_int_from_anybase( int & i ) const;
	bool as_int_from_base( int & i, int base ) const;

	int get_hash_key();

#ifdef WITH_STR2
	str0( const str2 * s2 );
#endif
};
#endif
