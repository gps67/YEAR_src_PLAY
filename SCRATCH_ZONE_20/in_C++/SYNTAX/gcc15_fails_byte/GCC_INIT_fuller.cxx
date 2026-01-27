
// atoi
#include <stdlib.h>
#include <stdio.h>

typedef unsigned char u8;

u8 table_flip_4[16] = {
#if 0
        0x00,   // 0 0000 -> 0000 = 0
        0x08,   // 1 0001 -> 1000 = 8
        0x04,   // 2 0010 -> 0100 = 4
        0x0C,   // 3 0011 -> 1100 = C
        0x02,   // 4 0100 -> 0010 = 2
        0x0A,   // 5 0101 -> 1010 = A
        0x06,   // 6 0110 -> 0110 = 6
        0x0E,   // 7 0111 -> 1110 = E
        0x01,   // 8 1000 -> 0001 = 1
        0x09,   // 9 1001 -> 1001 = 9
        0x05,   // A 1010 -> 0101 = 5
        0x0D,   // B 1011 -> 1101 = D
        0x03,   // C 1100 -> 0011 = 3
        0x0B,   // D 1101 -> 1011 = B
        0x07,   // E 1110 -> 0111 = 7
        0x0F    // F 1111 -> 1111 = F
#endif
};

u8 reverse_bits_in_byte( u8 byte_in ){
#if 0

        u8 lo_in =  byte_in & 0x0F;
        u8 hi_in = (byte_in >> 4) & 0x0F;
        u8 hi_out = table_flip_4[ lo_in ] << 4;
        u8 lo_out = table_flip_4[ hi_in ];
        u8 byte_out = hi_out + lo_out;
        return byte_out;
#else
	return 0x12;
}
#endif

void test1( u8 val ) {
	u8 rev = reverse_bits_in_byte( val );
	printf("%02X <-%02X\n", rev, val ); 
}


int main( int argc, char ** argv ) {
	u8 val = 0xA5;
	if( argc > 1 ) {
		val = atoi( argv[1] );
		test1( val );
	}
	test1( 0xF0 );
	test1( 0xA5 );
	test1( 0x0F );

	return 99;
}

