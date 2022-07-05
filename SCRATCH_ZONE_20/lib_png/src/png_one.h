#ifndef png_one_H
#define png_one_H

#include "ints.h"
#include <byteswap.h>
#include <png.h>

extern "C" {
	// neither of these work - cant see why
	// struct png_image;
	// typedef struct png_image png_image;
}


namespace GFX {

 struct u32_RGBA {
 	// byte order in C { R G B A } works OK with libpng
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
 };

 struct png_one {

 	png_image image;
        png_bytep buffer;

	png_one();
	~png_one();
	void clear(); // as in delete not clear

	void image_memset_zero();
	void image_free();
	bool buffer_alloc(); // for image header
	void buffer_free();

	bool create( int w, int h );
	bool read_from_file( const char * filename );
	bool write_to_file( const char * filename );

	int bytes_per_row;
	int bytes_per_pixel;
	bool calc_bytes_per();

	u32_RGBA * get_EA( int x, int y ) const {
		u64 offs = y * bytes_per_row  +  x * bytes_per_pixel;
		u32_RGBA * rgba = (u32_RGBA *) (buffer + offs);
		return rgba;
	}

	bool draw_xywh( int x, int y, int w, int h, u32_RGBA rgba );
	bool draw_nasty_blob(); // test

	// SHARED buffer // X11 pixmap // ?
	// SHARED buffer // lib pixman pixmap // ?
	// SHARED buffer // UDEF FT2 pixmap // ?
	// second code for u8 grey // u4_in_u8 // u4_u4 // u1x8 //
 };

};

using namespace GFX; // TEMP

#endif
