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

	/*!
		put a block of (ASCII) without any convertion
	*/
	bool put( const blk1 & b ) {
		return blk1::put_blk( b );
	}

	/*!
		put a decimal string
	*/
	bool put_d( int d ) {
		char str[17];
		sprintf( str, "%d", d );
		return put_plain( (str0) str );
	}

#ifdef WITH_MYSQL
	/*!
		put blob b as "quoted \xxx ASCII" for mysql command
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

	bool print_n_digits_binary( int ndigits, int value, u8 C0 = 0, u8 C1 = 0 );


	/*!
		set the initial value to null terminated s
	*/
	buffer2 & operator = ( str0 s )
	{
		set(s);
		return *this;
	}


	bool drop_trailing_eoln();

	/*!
		debugging thing, to dump another object into this buffer
	*/
	void hex_dump_into_8859( void * buf, int len );

	/*!
		debugging thing, to dump another object into this buffer
	*/
	void hex_dump_into_8859( const blk1 & b ) {
		hex_dump_into_8859( b.buff, b.nbytes_used );
	}

	/*!
		debugging thing, also useful to set binary blobs AUTH keys

		parse hex bytes upto eoln or # comment or error

		must be bytes not pair quad or eight

		return FALSE if nothing found or error
	*/
	bool parse_hex( const char * str );

	/*!
		hex must be there, append to buffer
	*/
	bool parse_hex_expect( const char * str );

	/*!
		hex optional, clear buffer before start
	*/
	bool set_parse_hex( const char * str );

	/*!
		hex must be there, clear buffer before start
	*/
	bool set_parse_hex_expect( const char * str );


	/*
		must manually re-inherit this !!
	*/
	operator bool() {
		return buffer1::operator bool();
	}

};

#undef Put
#undef Get_space

#endif

