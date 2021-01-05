#ifndef int_bitmap_H
#define int_bitmap_H

	struct int_bitmap
	{
		int bitmap;

		int_bitmap() { bitmap = 0; }
		void clear_all() { bitmap = 0; }

		int bits_test(int mask) { return bitmap & mask; }
		bool bits_set(int mask) { bitmap |= mask; return true; }
		bool bits_clr(int mask) { bitmap &= ~mask; return false; }

		bool test( int b ) { return bits_test( 1<<b ); }
		bool set( int b )  { return bits_set( 1<<b ); }
		bool clear( int b )  { return bits_clr( 1<<b ); }
		bool reset( int b )  { return bits_clr( 1<<b ); }

		bool bit_test( int b ) { return bits_test( 1<<b ); }
		bool bit_set( int b )  { return bits_set( 1<<b ); }
		bool bit_clr( int b )  { return bits_clr( 1<<b ); }

		// now add range of bits ....
	};

#endif
