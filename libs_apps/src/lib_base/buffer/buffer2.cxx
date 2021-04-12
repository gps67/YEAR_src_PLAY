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
		INFO("LF found");
		nbytes_used --;
		if( buff[nbytes_used-1] == '\r' ) {
			INFO("CR found");
			nbytes_used --;
		}
		buff[nbytes_used] == 0x00;
		return true;
	}

	INFO("not found");
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
