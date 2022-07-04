#ifndef BUFFER1_H
#define BUFFER1_H
#include "blk1.h"
#include <stdarg.h>
#include "str0.h"

/*
	OOPS -its a tad confused
	I can b1.print(...)
	I can b2.put("-")
	I cannot b1.put("c");

	I think it might be put_ascii() -vs- charset
	its a problem with amnt_t which uses buffer2 
	but that turns out to be put(" ") -vs- put(' ');
	when str is not clearly cset but byte MIGHT be
*/


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

	/*!
		constructor
	*/
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
		inline putc from 8859 to utf8
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

	/*!
		inline putc from 8859 to utf8
	*/
	bool	put_CRLF()
	{
		bool ok = true;
		ok &= put_byte('\r');
		ok &= put_byte('\n');
		return ok;
	}

	/*!
		inline putc from 8859 to utf8
	*/
	bool	put_LF()
	{
		bool ok = true;
		ok &= put_byte('\n');
		return ok;
	}


	/*!
	*/
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

		8859 -> 8859 = direct 
		UTF8 -> UTF8 = direct

		8859 -> UTF8 = available_if_you_select_function_name
		UTF8 -> 8859 = not here

		Could ADD flag to blk1 ? (prefer not to)
		Could ADD flag to buffer1 ? (good idea - use how?
		Could also stretch printf to use avail nbytes  (?DONE?)
	*/
// GCC Function-Attributes to check for fmt
// GXX increases parameter numbers by 1 == this
// #define FMT0 __attribute__((__format__ (printf, 1, 2)))  // C_FMT1
#define FMT1 __attribute__((__format__ (printf, 2, 3)))  
// #define FMT2 __attribute__((__format__ (printf, 3, 4))) 

FMT1	bool print( const char * fmt, ... );
FMT1	bool printf( const char * fmt, ... ); // it has fmt so printf
FMT1	bool print_into_utf8( const char * fmt, ... );
#undef FMT1
	bool vprint( const char * fmt, va_list args );
	bool vprint( bool into_utf8, const char * fmt, va_list args );
	bool vprint_8859_to_utf8( const char * fmt, va_list args );

	/*!
		Put NUL terminated string s without any convertion
	*/
	bool	put_plain( const str0 s )
	{
		return put_nn_bytes( s.str_len(), s );
	}

	/*!
		Put p0p2 string s without any convertion
	*/
	bool	put_plain( const p0p2 & zone )
	{
		return put_nn_bytes( zone.str_len(), zone.p0 );
	}

	bool trim_trailing_eoln();
	bool trim_trailing_gaps();
	bool trim_trailing_blanks();
	bool trim_leading_blanks();
	bool trim_leading_and_trailing_blanks();

	/*
		must manually re-inherit this !!
		a buffer is false if it is empty

		apps might also interpret STR0 NULL or "" as empty as false
		apps might also interpret "-" as a placeholder for NULL
		funcs might use STR0 arg = NULL as DEFAULT arg="set_by_func"
		funcs might use STR0 arg = "" as DEFAULT arg="set_by_func"
		funcs might use STR0 arg = "-" as DEFAULT arg="set_by_func"
	*/
	operator bool() {
		return blk1::operator bool();
	}

	// Q // boot retval is specific or generic
	bool operator += ( const str0 s ) { return put_plain( s ); }

	// REPEAT for s one of { str0 STR1 buffer1 blk1 P0P2 CSR utf8 }

};
#endif
