#include "buffer1.h" // print is here
#include <stdarg.h>
#include <stdio.h> // vsnprintf
#include "e_print.h"

/*!
	fprintf style functions, no overruns
	glibc >= 2.1, C99 standard
*/
bool buffer1::print( const char * fmt, ... )
{
	va_list args;
	va_start( args, fmt );
	bool ok = vprint( false, fmt, args );
	va_end( args );
	return ok;
}

/*!
	This is a bit confused
*/
bool buffer1::print_into_utf8( const char * fmt, ... )
{
	va_list args;
	va_start( args, fmt );
	bool ok = vprint( true, fmt, args );
	va_end( args );
	return ok;
}

/*!
*/
bool buffer1::putc_into_utf8_slower( int ch )
{
	if(ch<=0)
	{
		if(!ch) return putc_NUL_into_utf8();
		if( ch >= -128 )
			return putc_into_utf8_slower( ch & 0xFF );
		{
			e_print("ERROR putc_into_utf8_slower(%d)\n", ch );
			return false;
		}
		
	} else if( ch < 0x80 )
	{
		return put_byte( ch );
	} else if( ch < 0x800 ) // 110. .... + 10.. .... = 11 bits
	{
		if(0) put_byte('U');
		uchar c2 = 0x80 + (ch & 0x3F); ch = ch >> 6;
		uchar c1 = 0xC0 + (ch);
		return put_2_bytes( c1, c2 );
	} else if( ch < 0x10000 ) // 1110 .... + 10.. .... + 10.. ....
	{
		if(0) put_byte('U');
		uchar c3 = 0x80 + (ch & 0x3F); ch = ch >> 6;
		uchar c2 = 0x80 + (ch & 0x3F); ch = ch >> 6;
		uchar c1 = 0xE0 + (ch);
		return put_3_bytes( c1, c2, c3 );
	} else if( ch < 0x200000 )
	{
		if(0) put_byte('U');
		if(!get_space(4)) return false;
		uchar c4 = 0x80 + (ch & 0x3F); ch = ch >> 6;
		uchar c3 = 0x80 + (ch & 0x3F); ch = ch >> 6;
		uchar c2 = 0x80 + (ch & 0x3F); ch = ch >> 6;
		uchar c1 = 0xF0 + (ch);
		put_byte( c4 );
		put_byte( c3 );
		put_byte( c2 );
		put_byte( c1 );
		return true;
	} else if( ch < 0x4000000 )
	{
		if(0) put_byte('U');
		if(!get_space(5)) return false;
		uchar c5 = 0x80 + (ch & 0x3F); ch = ch >> 6;
		uchar c4 = 0x80 + (ch & 0x3F); ch = ch >> 6;
		uchar c3 = 0x80 + (ch & 0x3F); ch = ch >> 6;
		uchar c2 = 0x80 + (ch & 0x3F); ch = ch >> 6;
		uchar c1 = 0xF8 + (ch);
		put_byte( c5 );
		put_byte( c4 );
		put_byte( c3 );
		put_byte( c2 );
		put_byte( c1 );
		return true;
	} else
	{
		if(0) put_byte('U');
		if(!get_space(6)) return false;
		uchar c6 = 0x80 + (ch & 0x3F); ch = ch >> 6;
		uchar c5 = 0x80 + (ch & 0x3F); ch = ch >> 6;
		uchar c4 = 0x80 + (ch & 0x3F); ch = ch >> 6;
		uchar c3 = 0x80 + (ch & 0x3F); ch = ch >> 6;
		uchar c2 = 0x80 + (ch & 0x3F); ch = ch >> 6;
		uchar c1 = 0xFC + (ch);
		put_byte( c6 );
		put_byte( c5 );
		put_byte( c4 );
		put_byte( c3 );
		put_byte( c2 );
		put_byte( c1 );
		return true;
	}
}

/*!
*/
bool	buffer1::put_nn_bytes_8859_to_utf8( unsigned n, const uchar * _bytes )
{
	uchar * bytes  = (uchar *) _bytes;
	if(!get_space( n*2 )) return false; // vague guess
	for( unsigned int i=0; i<n; i++ )
	{
		uchar c = bytes[i] ;
		if(!putc_into_utf8( c )) return false;
	}
	return true;
}

/*!
*/
bool buffer1::vprint( bool conv_8859_to_utf8, const char * fmt, va_list args )
{
	if( conv_8859_to_utf8 )
		return vprint_8859_to_utf8( fmt, args );
	else
		return vprint( fmt, args );
}

/*!
*/
bool buffer1::vprint_8859_to_utf8( const char * fmt, va_list args )
{
	static buffer1 converter;
	if(!converter.vprint( fmt, args )) return false;
	if( !put_nn_bytes_8859_to_utf8( converter.nbytes_used, converter.buff ))
	{
		converter.clear();
		return false;
	}
	converter.clear();
	return true;
}

/*!
*/
bool buffer1::vprint( const char * fmt, va_list args )
{
	get_space(1);
	int maxlen = space_avail();
	if(!maxlen) {
		// report
		// vsnprintf with maxlen==0 simply returns 0
		return false;
	}
/*
man sprintf:
  Return value
       These  functions  return  the number of characters printed
       (not including the trailing `\0' used  to  end  output  to
       strings).   snprintf  and vsnprintf do not write more than
       size bytes (including the trailing '\0'), and return -1 if
       the  output  was truncated due to this limit.  (Thus until
       glibc 2.0.6. Since glibc 2.1 these  functions  follow  the
       C99  standard and return the number of characters (exclud­
       ing the trailing '\0') which would have  been  written  to
       the final string if enough space had been available.)
*/

	int t = vsnprintf( (char *)gap_addr(), maxlen, fmt, args );
	if( t < 0 ) 
	{
		// handle old truncating version
		for( int i = 1; i < 20; i++ )
		{
			get_space( i * 1024 );
			maxlen = space_avail();
			t = vsnprintf( (char *)gap_addr(), maxlen, fmt, args );
			if( t > 0 ) break;
		}
		if( t < 0 )
		{
			// report it
			return false;
		}
	}
	if( t >= maxlen )
	{
		if(! get_space( t+10 ) ) return false;
		maxlen = space_avail();
		t = vsnprintf( (char *)gap_addr(), maxlen, fmt, args );
		if( t > maxlen ) {
			fprintf(stderr, "vsnprintf() logic error\n");
			return false;
		}
	}
	nbytes_used += t;
	return true;
}



