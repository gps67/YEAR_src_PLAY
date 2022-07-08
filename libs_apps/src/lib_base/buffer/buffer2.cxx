#include "buffer2.h"
#include "dgb.h"

void buffer2::hex_dump_into_8859( void * buf, int len )
{
//	const int n8 = 8;
	const int n16 = 16;
	uchar * p = (uchar *)buf;
	while( len > 0 )
	{
		int n = n16;
		if( n > len ) n = len;
		print("#  ");
		for( int i=0; i<n16; i++ )
		{
			if((i%4) == 0 && i ) {
				print(" ");
				if((i%8) == 0 ) print(" ");
			}
			if( i < n ) {
				uchar c1 = p[i];
				print("%2.2X ", c1 );
			} else {
				print( "__ " );
			}
		}
		print("  " );
		for( int i=0; i<n16; i++ )
		{
			if(  (i%4) == 0 ) {
			//	print(" ");
				if((i%8) == 0 ) print(" ");
			}
			uchar c = '_';
			if( i < n ) c = p[i];
			if( c<32 ) c = '.';
			if( c>126 ) c = '.';
			print("%c", c );
		}
		print("\n" );
		p += n;
		len -= n;
	}
	trailing_nul();
}


bool buffer2:: drop_trailing_eoln()
{
	if(!nbytes_used) return false; // not found in an empty string
	if( buff[nbytes_used-1] == '\n' ) {
	//	INFO("LF found");
		nbytes_used --;
		if( buff[nbytes_used-1] == '\r' ) {
	//		INFO("CR found");
			nbytes_used --;
		}
		buff[nbytes_used] = 0x00;
		return true;
	}

	INFO("EOLN not found");
	return false;
}

bool buffer2:: print_n_digits_binary(
	int ndigits,
	int value,
	u8 C0,
	u8 C1
)
{
	if(!C0) C0 = '0';
	if(!C1) C1 = '1';

	if( ndigits < 1 ) ndigits = 1;
	if( ndigits > 64 ) ndigits = 64; // now you are being silly

	if(!get_space( ndigits + 1 )) return FAIL_FAILED();
	u8 * P = gap_addr();
	nbytes_used += ndigits;

	while( ndigits > 0 ) {
		ndigits --;
		u8 C = C0;
		if( value & 1 ) C = C1;
		value >>= 1;
		P[ ndigits ] = C;
	}
	trailing_nul();
	return true;
}

// LOCAL

bool parse_0x( const char *& S ) {
	if(S[0] == '0' && S[1] == 'x') {
		S += 2;
		return true;
	}
	return false;
}

bool parse_skip_gap( const char *& S ) { // incl EOLN
	bool found = false;
	while( *S > 0 && *S <= ' ') {
		S++; // skip blanks // incl EOLN
		found = true;
	}
	return found;
}

bool peek_hex_digit( int & d , const char *& S ) {
	bool found_digit = false;
	if( '0' <= *S && *S <= '9' ) {
		d = *S - '0';
		found_digit = true;
	} else
	if( 'A' <= *S && *S <= 'F' ) {
		d = *S - 'A' + 10;
		found_digit = true;
	} else
	if( 'a' <= *S && *S <= 'f' ) {
		d = *S - 'a' + 10;
		found_digit = true;
	} else {
		// NOT a HEX digit // maybe complain
	}
	return found_digit;
}

bool parse_hex_digit( int & d , const char *& S ) {
	if( peek_hex_digit( d, S )) {
		S++;
		return true;
	}
	return false;
}

bool buffer2:: parse_hex( const char * s )
{
	if(!s) return FAIL("NULL");
	bool found_hex = false; // nothing found
	const char * S0 = 0;
	bool multi_run = false;
	while(*s) {
		if( S0 == s ) {
			return FAIL("str ptr s has not moved '%s'", S0);
		} else {
			S0 = s;
		}
		int val = 0;
		parse_skip_gap( s );
		parse_0x( s );
		int d = 0;
		if( parse_hex_digit( d, s ) ) {
			val = d;
			if( parse_hex_digit( d, s ) ) {
				val *= 16;
				val += d;
		#if 0
				// option to just stay
				// and long HEX will sort themselves as HI LO
				// 0xAABBCCDD
				// or use this code to complain on 3'd digit
				if( parse_hex_digit( d, s ) ) {
					return FAIL("only 2 digit hex allowed");
				}
		#else
				multi_run = peek_hex_digit( d, s );
				// maybe it WAS but now it isnt // or is
				// this is to detect odd hex digits 0xABC
		#endif
			} else {
				if( multi_run ) {
					// if above 0xABBCC is enabled AB BC C
					// all 4 bits shifted off
					return FAIL("odd number of HEX digits");
				}
				// option to accept single digit HEX eg 0
				WARN("single digit HEX");
			}
			put_byte( val );
			found_hex = true;
		} else {
			break;
		}
	}
	return found_hex;
}

bool buffer2:: parse_hex_expect( const char * str )
{
	// append do not clear
	if( parse_hex( str ) )
		return true;
	else
		return FAIL("EXPECTED HEX got '%s'", str );
}

bool buffer2:: set_parse_hex( const char * str )
{
	clear();
	return parse_hex( str );
}

bool buffer2:: set_parse_hex_expect( const char * str )
{
	clear();
	return parse_hex_expect( str );

}

