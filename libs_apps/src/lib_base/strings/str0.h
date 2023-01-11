#ifndef str0_H
#define str0_H


#define WITH_STR2 

#include <string.h>
#include <stdlib.h> // malloc

#include "misc.h"
#include "ints.h"
#include "is_diff.h"

typedef unsigned char uchar;
typedef const char * STR0;
typedef const uchar * uSTR0; // OK but cant cast from STR0 !!

inline // else multiple definition
uSTR0 uSTR0_from_STR0( STR0 s ) {
//	uSTR0 str = static_cast<uSTR0>(s);
	uSTR0 str = reinterpret_cast<uSTR0>(s);
	return str;
}

#ifndef e_print_H
#include "e_print.h"
#endif

	// invoke C++ cast for fprintf
//#define STR0 (const char *)
#define str0_const /* NOT const */


class str2;

//////////////////////////////////////////
struct  blk1;
struct  buffer1;
//////////////////////////////////////////


/*!
	str0 is the same as a plain old-fashioned pointer to
	a NUL terminated string.

	There is no consideration of 'malloc', thats provided by the
	str1 derived class, and the most common use of the string
	is as a constant, passed (ready made) between functions.

	There are wrappers on the strcmp() style functions,
	that might make str0 re-factorable to other classes,
	but probably not std::string.

	Its a variable, its a register copy, not a full variable
	It is SUPPOSED to be a single pointer value, no extras.

	The chars in it are DEFINITELY unsigned, so that should
	prevent any problems with 8-bit chars being negative.

	The chars in the string are not CURRENTLY const,
	but this is not definite. Changing 'str0_const' 
	(and recompile everything) should switch that.
	An interesting problem with C++ is casting things
	in and out of 'const' without having to specify the
	item-type. I wish there was an (unconst) cast.


	To get a (const char *) from an str0, use the (STR0) macro.
	Thats a bit of a hassle, maybe I need to read up on
	how to pass 'things' to printf, without this message:

warning: cannot pass objects of non-POD type `class str0' through `...';
call will abort at runtime



*/
class str0 : public GRP_lib_base
{

 public:

	/*!
		This entire class is (supposed to be) a single pointer
		that fits into a register.
		There must not be any other data fields, or
		compiler added cookies.

		The chars are definitely unsigned, and as 'const' as
		the 'str0_const' macro.
	*/
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
		//
		// error invalid static_cast
		// str = static_cast<str0_const uchar *>(s);
		//
		// !! error reinterpret_cast casts away constness !!
		// METHINKS thats the point of the cast ...
		// str = reinterpret_cast<str0_const uchar *>(s);
		//
		// warning - use of old-style cast
		// str = (str0_const uchar *)(s);
		//
#if 0
		// str = static_cast<str0_const uchar *>(const_cast<char *>(s));
		char * s_2 = const_cast<char *>(s);
		// static_ fails, but reinterpret_ works
		// uchar * s_3 = static_cast<uchar *>(s_2);
		uchar * s_3 = reinterpret_cast<uchar *>(s_2);
		str = s_3;
#else
		str = reinterpret_cast<uchar *>(const_cast<char *>(s));
#endif
	}

	void clear() {
		str = 0;
	}

	//! constructor - assign from plain C string
	str0( const uchar * s )
	{
		// NB loses const
		// str = (str0_const uchar *)s;
		str = const_cast<uchar *>(s);
	}

 	//! constructor - extract (uncopied) plain C string from string
	str0( const str0 & s2 )
	{
		str = s2.str;
	}

//////////////////////////////////////////
	str0( blk1 & b2 );
//	str0( buffer1 & b2 );

//////////////////////////////////////////

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
		return reinterpret_cast<str0_const char *>(str);
	}

	//! str0 can be used where you use char *
	operator str0_const uchar *() const
	{
		// return (str0_const uchar *)str;
		return reinterpret_cast<str0_const uchar *>(str);
	}

	//! str0 can be used where you use char *
	operator const char *() const
	{
		// return (const char *)str;
		return reinterpret_cast<str0_const char *>(str);
	}

	//! str0 can be used where you use char *
	operator const void *() const
	{
		// return (const void *)str;
		return reinterpret_cast<const void *>(str);
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
		// WARN str = (str0_const uchar *) s2;
		str = reinterpret_cast<str0_const uchar *>( const_cast<char*>(s2) );
		return *this;
	}

	//! Nice Macro - calls strcmp - this does get called
	bool operator==( const char * s ) const
	{
		return operator==( str0( s ) );
	}

	//! ADDED THIS 2023 - had hoped for cast to unconst
	bool operator==( char * s ) const
	{
		return operator==( str0( s ) );
	}

	//! Nice Macro - calls strcmp
	bool operator==( const uchar * s ) const
	{
		return operator==( str0( s ));
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
		// return (str0_const char *) str ;
		return reinterpret_cast<str0_const char *>( str ) ;
	}

	str0_const char * get_str() const { return sget(); }

	//! Nice Macro - calls strcmp
	int str_cmp( const str0 & s2 ) const
	{
		if(!str) return -1;
		if(!s2.str) return +1;
		// return strcmp( (STR0) str, (STR0) s2.str );
		return strcmp( STR0( str ), STR0( s2.str ) );
	}

	//! Nice Macro - calls strcmp
	int str_cmp_n( const str0 & s2, int n ) const
	{
		if(!str) return -1;
		if(!s2.str) return +1;
		// return strncmp( (STR0) str, (STR0) s2.str, n );
		return strncmp( STR0( str ), STR0( s2.str ), n );
	}

	//! Nice Macro - calls strcmp
	bool has_prefix( const str0 & pfx ) const
	{
		return( 0==str_cmp_n( pfx, pfx.str_len() ) );
	}

	//! Nice Macro - calls strcmp
	bool has_suffix( const str0 & sfx ) const
	{
		int l1 =     str_len();
		int l2 = sfx.str_len();
		if( l1 == 0 ) return true;
		int skip = l1 - l2;
		if( skip < 0 ) return false;
		if( skip > l1 ) return false;
		str0 s0 = STR0( str ) + skip;
		return (0==strncmp( STR0(s0), sfx, l2 ));
	}

	bool ends_with( const str0 & s ) const {
		return has_suffix( s );
	}

	bool starts_with( const str0 & s ) const {
		return has_prefix( s );
	}

	//! NULL or str-with-tail of substr 
	str0 pos_substr( const str0 & sub )
	{
		const char * s2 = strstr( STR0(str), STR0(sub) );
		return s2;
	}

	//! THIS has sub-str sub 
	bool has_substr( const str0 & sub )
	{
		return NULL != strstr( STR0( str ), STR0( sub ) );
	}

	bool has_lex_substr(
		const str0 & entire,
		const str0 & left,
		const str0 & mid,
		const str0 & right
	);

	//! Nice Macro - calls strlen
	int str_len() const
	{
		if(!str) return 0;
		// return strlen( (char *)str );
		return strlen( STR0(str) );
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
		return strchr( STR0(str), ch );
	}

	//! Nice Macro - calls strrchr
	str0 str_rchr( char ch ) const
	{
		if(!str) return str0();
		return strrchr( STR0(str), ch );
	}

	//! Nice Macro - calls strchr
	str0 str_chr( uchar ch ) const
	{
		if(!str) return str0();
		return strchr( STR0(str), char(ch) );
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
		return 0 == strcasecmp( STR0(str), STR0(s2) );
	}

	//! Nice Macro - calls strcmp
	bool is_same_as( const str0 & s2 ) const
	{
		return (0==strcmp( STR0(str), STR0(s2.str) ));
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
	// when used within lexer, allow nonstarter
	bool	is_double( double & f ) const;
	bool	is_float( float & f ) const;

	bool as_int( int & i ) const; // decimal only
	bool as_int_from_anybase( int & i ) const;
	bool as_int_from_base( int & i, int base ) const;

	uint get_hash_key();

#ifdef WITH_STR2
	str0( const str2 * s2 );
#endif
};

/*
	Not in a class
*/

	const char * str_true_false( bool b );
	const char * str_true_false_YN( bool b );
	const char * str_true_false( bool b, const char * YES, const char* NO );

#endif
