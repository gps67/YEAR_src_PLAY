#ifndef u32_RGBA_H
#define u32_RGBA_H

#include "ints.h"
#include <byteswap.h>


namespace WAX {

 struct u32_RGBA_t {
 	// a 32 bit pixel value - used by libpng - used by ximage
	// mupdf uses RGBA // additive more light
	// mupdf uses CMYK // subtractive less light more ink
 	// byte order in C { R G B A } works OK with libpng
	// magically byte packed - struct is a single word LOHI
 	u8 R;
 	u8 G;
 	u8 B;
 	u8 A;

	int bytes_per_pixel() { return 4; }

	u32 get_RGBA_lohi() {
		return *(u32*)this; // intel lohi specific
	}
	u32 get_RGBA_hilo() {
		return bswap_32( get_RGBA_lohi() );
	}

	void set( u8 r, u8 g, u8 b ) { R=r; G=g; B=b; A=0xFF; }
	void black() { set( 0,0,0 ); }
	void white() { set( 0xFF,0xFF,0xFF ); }
	void red() { set( 0xFF,0,0 ); }
	static bool test_byte_order();
 };

}; // namespace WAX
#endif

