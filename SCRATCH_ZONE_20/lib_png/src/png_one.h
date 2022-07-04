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

 struct RGBA {
 	u8 R;
 	u8 G;
 	u8 B;
 	u8 A;

	u32 get_RGBA_lohi() {
		return *(u32*)this; // intel lohi specific
	}
	u32 get_RGBA_hilo() {
		return bswap_32( get_RGBA_lohi() );
	}
 };

 struct png_one {

 	png_image image;
        png_bytep buffer;

	png_one();
	~png_one();
	void clear();

	void image_memset_zero();
	void image_free();
	void buffer_free();

	bool read_from_file( const char * filename );
	bool write_to_file( const char * filename );

	bool draw_nasty_blob(); // test
 };

};

using namespace GFX; // TEMP

#endif
