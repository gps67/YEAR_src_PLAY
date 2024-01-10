#include "buffer1.h" // print is here
#include <stdarg.h>
#include <stdio.h> // vsnprintf
#include "e_print.h"
#include "CR_LF_NUL.h"

/*!
	maybe move to obj.printf( fmt, ... )

	fprintf style functions, no overruns
	glibc >= 2.1, C99 standard
*/
bool buffer1::printf( const char * fmt, ... )
{
	va_list args;
	va_start( args, fmt );
	// false = conv_8859_to_utf8 // upgrade somehow
	bool ok = vprint( false, fmt, args );
	va_end( args );
	return ok;
}

/*!
	maybe move to obj.printf( fmt, ... )

	fprintf style functions, no overruns
	glibc >= 2.1, C99 standard
*/
bool buffer1::print_ln( const char * fmt, ... )
{
	va_list args;
	va_start( args, fmt );
	// false = conv_8859_to_utf8 // upgrade somehow
	bool ok = vprint( false, fmt, args );
	va_end( args );
	print_EOLN();
	return ok;
}

bool buffer1::print_ln()
{
	return put_EOLN();
}

bool buffer1::print_EOLN()
{
	return put_EOLN();
}

/*!
	maybe move to obj.printf( fmt, ... )

	fprintf style functions, no overruns
	glibc >= 2.1, C99 standard
*/
bool buffer1::print( const char * fmt, ... )
{
	va_list args;
	va_start( args, fmt );
	// false = conv_8859_to_utf8 // upgrade somehow
	bool ok = vprint( false, fmt, args );
	va_end( args );
	return ok;
}

/*!
	This is a bit confused
	IF the input is already UTF8 this will double encode
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
	This is the 8-BIT transport format (not UNICODE from 8859)
	It needs conv ch_8859 to ch_uni
	It needs conv ch_byte to ch_unicode // 8859-II // iso646 // etc
*/
bool buffer1::putc_into_utf8_slower( int ch )
{
	// SIMPLE LIFE //
	// RUN FILTER HERE
	// ch = UNICODE_from_8859_byte

	// BUG // this function demands parent sign extends ch to -ve
	// the signed parameter does that
	if(ch<=0)
	{
		if(!ch) return putc_NUL_into_utf8();
		// somehow unicode got sign extended
		if( ch >= -128 ) {
			WARN("i8 0x%02X got into i32", ch );
			return putc_into_utf8_slower( ch & 0xFF );
		}
		
		FAIL("ERROR ch is -ve %d", ch );
		return false;
	}
	if( ch < 0x80 )
	{
		// ASCII (not iso646) presumed
		return put_byte( ch );
	}
	if( ch < 0x100 ) // within the BYTE range
	{
		// ch = UNICODE_from_8859_high
		// no detect of double encode

	}
	if( ch < 0x800 ) // 110. .... + 10.. .... = 11 bits
	{
		if(0) put_byte('U');
		// 0x3F is 6 bit mask 0011 1111
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
	This is used whenever you want to write 8859 to an gtk text buffer.
	Maybe need to encourage people with
		put_nn_bytes_ASCII_to_utf8
		put_nn_bytes_ASCII_to_8859
		put_nn_bytes_8859_to_utf8
		put_nn_bytes_8859_to_8859
		put_nn_bytes_UTF8_to_utf8
		put_nn_bytes_UTF8_to_8859 // _with_XML_sdata_char_names
	This can be optimised a bit more, each putc is calling get_space(1)
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
	true - convertion is required
	false - its already utf8
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
	// static // -vs- thread safe
	buffer1 converter;
	if(!converter.vprint( fmt, args )) return false;
	if( !put_nn_bytes_8859_to_utf8( converter.nbytes_used, converter.buff ))
	{
		// converter.clear();
		return FAIL_FAILED();
	}
	// converter.clear();
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

/*
	NEW BUG: args is usually processed ONCE (and incremented)
	We need to use it twice - for the buffer full repeat,
	then again for the print to screen and stdout.
	We could print it ONCE to screen and stdout
	(but only if both are same utf8) via a buffer,
	and that would be better. (TODO - repeat print from buffer)

	hence
		 va_copy( args2, args );
		 ...
		 va_end( args2 );
	
*/

	va_list args2;
	va_copy( args2, args );
	int t = vsnprintf( (char *)gap_addr(), maxlen, fmt, args2 );
	va_end( args2 );
	if( t < 0 ) 
	{
		// handle old truncating version
		for( int i = 1; i < 20; i++ )
		{
			get_space( i * 1024 );
			maxlen = space_avail();
			va_copy( args2, args );
			t = vsnprintf( (char *)gap_addr(), maxlen, fmt, args2 );
			va_end( args2 );
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
// gdb_invoke(true);
		if(! get_space( t+10 ) ) return false;
		maxlen = space_avail();
		va_copy( args2, args );
		t = vsnprintf( (char *)gap_addr(), maxlen, fmt, args2 );
		va_end( args2 );
		if( t > maxlen ) {
			fprintf(stderr, "vsnprintf() logic error\n");
			return false;
		}
	}
	nbytes_used += t;
	return true;
}



bool buffer1:: trim_leading_blanks()
{
//	gdb_invoke();
	if(!nbytes_used) return false; // not found
	p0p2 zone( buff, nbytes_used );
	zone.trim_leading_blanks();
	if(zone.p0 == buff) return false; // not found
	del_copy_down( zone.p0 );
	return true; // found
}

bool buffer1::trim_trailing_eoln()
{
//	INFO("CALLED nbytes_used == %d", nbytes_used);
	bool found_gap = false;
	bool looping = true;
	int found_count = 0;
	while( nbytes_used && looping ) {
		u8 * P = buff + nbytes_used - 1;
		switch( *P ) {
		 case CR:
		 case LF:
//		 	INFO("found CHR(%2X)",*P);
			if( found_gap ) {
				if( *P == LF ) {
					WARN("trim multiple LF");
				}
			}
			*P = NUL;
			nbytes_used --; // will overwrite BYTE
			found_gap = true;
			looping = true;
		 break;
		 default:
		 	looping = false;
//		 	INFO("Found CHR(%2X)",*P);
		}
	}
	trailing_nul();
	return found_gap;
}

bool buffer1::trim_trailing_blanks()
{
	bool found_gap = false;
	bool looping = true;
	while( nbytes_used && looping ) {
		u8 * P = buff + nbytes_used - 1;
		switch( *P ) {
		 case SP:
		 case TAB:
		 case CR:
		 case LF:
			*P = NUL;
			nbytes_used --; // will overwrite BYTE
			found_gap = true;
			looping = true;
		 break;
		 default:
		 	looping = false;
		}
	}
	trailing_nul();
	return found_gap;
}

bool buffer1:: trim_leading_and_trailing_blanks()
{
	bool found = false;
	if(trim_trailing_blanks()) found = true;
	if(trim_leading_blanks()) found = true;
	return found;
}

bool buffer1::trim_trailing_gaps()
{
	return trim_trailing_blanks();
}

