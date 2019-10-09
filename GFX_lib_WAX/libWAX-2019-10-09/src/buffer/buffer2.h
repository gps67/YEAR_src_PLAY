#ifndef buffer2_H
#define buffer2_H

#include "buffer1.h"
#include "str1.h"
#include <stdio.h>

#ifdef WITH_MYSQL
#include <mysql.h> // escape string !!
#endif

/*
	blk1 intentionally avoided "put()" so that derived can use it
*/

// these macros are undefined at the end of this file
#define Put( X ) if( !put(X) ) return FALSE
#define Get_space( X ) if( !get_space(X) ) return FALSE

/*!
*/
struct buffer2 : public buffer1
{
/*!
	constructor - with an initial buffer of at least N bytes
*/
	buffer2( int N=0 ) : buffer1( N ) { /* ctor */ }
	~buffer2() { /* dtor */ }

/* --
	bool put_nul()
	{
		// added to buffer, unlike trailing_nul which pokes it past the end
		return put_byte( 0 );
	}
*/

/*!
	Put plain char as single byte
*/
	bool put( uchar c1 ) {
		return put_byte( c1 );
	}

/*!
	SQL presumes 8859 ?
*/
	
	bool put( const str0 & str ) {
		return put_plain( str );
	}

/* --
	bool put( const uchar * str ) {
		int len = strlen( (char *) str );
		return put_n_bytes( len, str );
	}
*/

/* --
	bool put( const char * str ) {
		int len = strlen( str );
		return put_n_bytes( len, (uchar *) str );
	}
*/

/* --
	bool put( const str1 & str ) {
		return put( (uchar *) str.str );
	}
*/

	bool put( const blk1 & b ) {
		return blk1::put( b );
	}

/*!
	put a decimal string
*/
	bool put_d( int d ) {
		char str[17];
		sprintf( str, "%d", d );
		return put_plain( str );
	}

//	bool print( const char * fmt, ... );

#ifdef WITH_MYSQL
/*!
*/
	bool put_q2_blob_mysql( blk1 * b ) {
		int n2 = b->nbytes_used * 2 + 10;
		// int l = 0;
		Get_space( n2 );
		Put( '"' );
		/*
			nb manual suggests mysql_real_escape_string()
			but this is to quote BLOBS into SQL (NUL ' \n ...)
		*/
		nbytes_used += mysql_escape_string(
		(char*) gap_addr(),	// TO
		(char*)	b->buff,	// FROM
			b->nbytes_used
		);
		Put( '"' );
		return TRUE;
	}
#endif

	// simple single quotes 'q1' or double "q2" 

/*!
	single quotes with unmodified str inside
*/
	bool put_q1( const str0 str ) {
		Put( '\'' );
		Put( str );
		Put( '\'' );
		return TRUE;
	}
/* --
	bool put_q1( const str1 & str ) {
		Put( '\'' );
		Put( str );
		Put( '\'' );
		return TRUE;
	}
*/

/*!
	double quotes with unmodified str inside
*/
	bool put_q2( const str0 str ) {
		Put( '"' );
		Put( str );
		Put( '"' );
		return TRUE;
	}

/* --
	bool put_q2( const str1 & str ) {
		Put( '"' );
		Put( str );
		Put( '"' );
		return TRUE;
	}
*/


	buffer2 & operator = ( str0 s )
	{
		set(s);
		return *this;
	}

	void hex_dump_into_8859( void * buf, int len );

};

#undef Put
#undef Get_space

#endif

