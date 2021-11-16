#include "buffer2.h"

void buffer2::hex_dump_into_8859( void * buf, int len )
{
	uchar * p = (uchar *)buf;
	while( len > 0 )
	{
		int n = 8;
		if( n > len ) n = len;
		for( int i=0; i<n; i++ )
		{
			uchar c1 = p[i];
			print("%2.2X ", c1 );
		}
		for( int i=n; i<8; i++ )
		{
			print( "__ " );
		}
		print("  " );
		for( int i=0; i<n; i++ )
		{
			uchar c = p[i];
			if( c<32 ) c = '.';
			print("%c", c );
		}
		print("\n" );
		p += n;
		len -= n;
	}
	trailing_nul();
}


