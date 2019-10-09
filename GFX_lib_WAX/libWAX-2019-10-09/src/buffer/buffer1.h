#ifndef BUFFER1_H
#define BUFFER1_H
#include "blk1.h"
#include <stdarg.h>
#include "str0.h"

/*!
	buffer1 is a blk1 used as a byte buffer,
	buffer2 is the text buffer,
	it keeps blk1's namespace clean of names
	that other classes might like (eg print())

	Every buffer type object should know what type of data
	it holds, and every contributor should know what type
	it is adding (UTF8 or 8859 or ...).

	Base classes might provide functions which name both types,
	so that derived classes (which ARE a specific type, or have
	an internal flag) can inline over the base functions,
	but only naming one is a mistake. Guess what - I only name one!

	That means YOU have to know the destination encoding, and
	call the correct functions. Fortunately vprint(fmt,args)
	can be used for UTF8-to-UTF8 as well as 8859-to-8859.

	putc( int unicode ) knows the origin type, but the destination
	encoding must be specified, hence putc_into_8859(int ch)
	and putc_into_utf8( int ch ). Both are efficient, but
	vprint_8859_to_utf8 has to vprint_8859_to_8859() into
	a temp buffer, then convert the result, which is because
	there is no libc vprint convertor.

	Very unfortunately there is no non-NUL encoding of NUL,
	which is a shame, since UTF8 _COULD_ accept it, and where
	adopted, it would allow nul-terminated strings to contain
	NUL-byte values (encoded into UTF8+). I do provide such
	a function, but GLib rejects its use, so its a private
	feature. If you dont use it, its not a problem.

*/
struct buffer1 : public blk1 {

	// IE //	unsigned nbytes_alloc;
	// IE //	unsigned nbytes_used;
	// IE //	uchar * buff;

	/**/	buffer1( int N = 0 )
	: blk1( N )
	{
	}

	/**/	~buffer1()
	{
	}

	bool	put_nn_bytes_8859_to_utf8( unsigned n, const uchar * bytes );

/*!
	This is illegal, but I wish it worked, and might use it privately.
*/
	bool	putc_NUL_into_utf8()
	{
		uchar c1 = 0xC0;
		uchar c2 = 0x80;
		return put_2_bytes( c1, c2 );
	}

/*!
*/
	bool	putc_into_utf8( unsigned ch )
	{
		if(!ch)
			return putc_NUL_into_utf8();
		else if (ch <128 )
			return put_byte( ch );
		else
			return putc_into_utf8_slower( ch );
	}

	buffer1 & operator = ( str0 s )
	{
		set(s);
		return *this;
	}

	/* this should be non-inline */
	// http://www.cl.cam.ac.uk/~mgk25/unicode.html
	bool	putc_into_utf8_slower( int ch );

	/*
		absorbing buffer1 text-stream functions into base blk1
	*/
	bool print( const char * fmt, ... );
	bool print_into_utf8( const char * fmt, ... );
	bool vprint( const char * fmt, va_list args );
	bool vprint( bool into_utf8, const char * fmt, va_list args );
	bool vprint_8859_to_utf8( const char * fmt, va_list args );

/*!
*/
	bool	put_plain( const str0 s )
	{
		return put_nn_bytes( s.str_len(), s );
	}

};
#endif
