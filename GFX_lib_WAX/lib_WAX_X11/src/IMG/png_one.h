#ifndef png_one_H
#define png_one_H

#include "u32_RGBA.h"
#include <png.h>

extern "C" {
	// neither of these work - cant see why
	// struct png_image;
	// typedef struct png_image png_image;
}


namespace WAX {

 /*!
 	a well defined frame array [h][w] of u32_pixel // buffer //

	libpng knows how to load / save a file in this format

	we can also access it and understand it as a simple array
 */
 struct png_one {

 	png_image image;	// header info // lib_png simple interface
        png_bytep buffer;	// pixel data // see get_EA(x,y)

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

	int get_width() { return image.width; }
	int get_height() { return image.height; }

	// see calc_bytes_per
	int bytes_per_row;
	int bytes_per_pixel;
	bool calc_bytes_per();

	u32_RGBA_t * get_EA_row( int y ) const {
		u64 offs = y * bytes_per_row;
		u32_RGBA_t * rgba = (u32_RGBA_t *) (buffer + offs);
		// pointer to row // array of pixels
		// u32_RBGA * row_pixel = get_EA_row( y );
		// row_pixel[ x ] = pixel_value;
		// or other preferred array syntax
		return rgba;
	}

	u32_RGBA_t * get_EA( int x, int y ) const {
		u64 offs = y * bytes_per_row  +  x * bytes_per_pixel;
		u32_RGBA_t * rgba = (u32_RGBA_t *) (buffer + offs);
		// a pointer to 1 pixel is
		// a pointer to N pixels
		return rgba;
	}

	bool draw_fill_xywh( int x, int y, int w, int h, u32_RGBA_t rgba );
	bool draw_nasty_blob(); // test

	// SHARED buffer // X11 pixmap // ?
	// SHARED buffer // lib pixman pixmap // ?
	// SHARED buffer // UDEF FT2 pixmap // ?
	// second code for u8 grey // u4_in_u8 // u4_u4 // u1x8 //
 };

};

using namespace WAX; // TEMP

#endif
