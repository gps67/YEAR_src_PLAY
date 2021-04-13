#include "bit_flip.h"

#include "ints.h"
#include "dgb.h"
#include "buffer2.h"

u8 table_flip_4[16] = {
	0x00,	// 0 0000 -> 0000 = 0
	0x08,	// 1 0001 -> 1000 = 8
	0x04,	// 2 0010 -> 0100 = 4
	0x0C,	// 3 0011 -> 1100 = C
	0x02,	// 4 0100 -> 0010 = 2
	0x0A,	// 5 0101 -> 1010 = A
	0x06,	// 6 0110 -> 0110 = 6
	0x0E,	// 7 0111 -> 1110 = E
	0x01,	// 8 1000 -> 0001 = 1
	0x09,	// 9 1001 -> 1001 = 9
	0x05,	// A 1010 -> 0101 = 5
	0x0D,	// B 1011 -> 1101 = D
	0x03,	// C 1100 -> 0011 = 3
	0x0B,	// D 1101 -> 1011 = B
	0x07,	// E 1110 -> 0111 = 7
	0x0F	// F 1111 -> 1111 = F
};

u8 reverse_bits_in_byte( u8 byte_in ){

	u8 lo_in =  byte_in & 0x0F;
	u8 hi_in = (byte_in >> 4) & 0x0F;
	u8 hi_out = table_flip_4[ lo_in ] << 4;
	u8 lo_out = table_flip_4[ hi_in ];
	u8 byte_out = hi_out + lo_out;
	return byte_out;
}

#if 0
bool print_4_binary( int b )
{
	char buff[5];
	for( int i = 0; i<3; i++ ) {
		char c = '0';
		if(b & 1 ) c = '1';
		buff[i] = c;
	}
	buff[4] = 0;
	return false;
}
#endif

bool TEST_reverse_bits_in_byte()
{
	buffer2 msg;
	for( u8 i = 0 ; i < 16; i++ ) {
		msg.print( "%X = ", i );
		msg.print_n_digits_binary( 4, i );
		u8 r = reverse_bits_in_byte( i );
		msg.print( " ==> " );
		msg.print_n_digits_binary( 4, r );
		msg.print( " = %X", r );

		u8 r2 = reverse_bits_in_byte( r );
		if( r2 != i ) return FAIL("r2 != i");
		if( r2 == r ) switch( r ) {
		// should be 4 symmetrical cases
		 case 0:	// 0000
		 case 6:	// 0110
		 case 9:	// 1001
		 case 0xF:	// 1111
			break;
		 default:
			return FAIL("r2 == r");
		}

		msg.print(" OK");

		INFO("%s",(STR0) msg );
		msg.clear();
	}
	return true;
};

