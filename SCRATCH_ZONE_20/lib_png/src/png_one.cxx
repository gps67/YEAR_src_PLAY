#include "png_one.h"
#include "dgb.h"
#include <memory.h> // memset

WAX::png_one:: png_one()
{
	buffer = NULL; // 
	image_memset_zero(); // and then set version no
}

WAX::png_one:: ~png_one()
{
	clear();
}

void WAX::png_one:: clear()
{
	buffer_free(); // 
	image_free();;
	image_memset_zero(); // and then set version no
}

void WAX::png_one:: image_memset_zero()
{
	memset(&image, 0, sizeof image);
	image.version = PNG_IMAGE_VERSION;
	image.format = PNG_FORMAT_RGBA; // because I forgot, you might too
}

void WAX::png_one:: image_free()
{
	png_image_free(&image); // the opaque contents not the struct itself
}

bool WAX::png_one:: buffer_alloc() // for image header
{
	buffer_free();
	u64 nbytes_1 = image.width * image.height * 4;
	u64 nbytes = PNG_IMAGE_SIZE(image);
	INFO("nbytes %lld nbytes_1 %lld diff %lld", nbytes, nbytes_1, nbytes-nbytes_1 );
	// same
	buffer = (png_bytep) malloc(nbytes);
	if (!buffer) {
		return FAIL("malloc: out of memory: %lu bytes\n",
		       (unsigned long)PNG_IMAGE_SIZE(image));
	}
	// load from file needs no memset, create() likes a default
//	memset(buffer, 0, nbytes);	// black and transparent
	memset(buffer, 0xFF, nbytes);	// white and opaque
	return true;
}

void WAX::png_one:: buffer_free()
{
	if(buffer)
            free(buffer);
	buffer = NULL;
}

bool WAX::png_one:: create( int w, int h )
{
	clear();
	image.width = w;
	image.height = h;
	image.format = PNG_FORMAT_RGBA; // we only do u32_RGBA_t atm
	if(!buffer_alloc()) return FAIL_FAILED();
	if(!calc_bytes_per()) 
		return FAIL_FAILED();
	return true;
}

bool WAX::png_one:: read_from_file( const char * filename )
{
	u32_RGBA_t::test_byte_order();

	clear(); // of image // pre-done

	if(!png_image_begin_read_from_file(&image, filename )) {
           return FAIL( "%s: %s", filename, image.message);
	}

	// we only do u32_RGBA_t atm
	// the libpng loader converts to this format
	image.format = PNG_FORMAT_RGBA;

	if(!buffer_alloc()) return FAIL_FAILED();

	if(!png_image_finish_read(
		&image,
		NULL	/*background*/,
		buffer,
		0	/*row_stride*/,
		NULL	/*colormap for PNG_FORMAT_FLAG_COLORMAP */
	)) {
		  fprintf(stderr, "read %s: %s\n", filename,
		      image.message);
	}

	if(!calc_bytes_per()) 
		return FAIL_FAILED();

	return PASS("%s",filename);
}

bool WAX::png_one:: write_to_file( const char * filename )
{
	INFO("image.flags = 0x%2X", image.flags );
	if(!png_image_write_to_file(
		&image,
		filename,
		0/*convert_to_8bit*/,
		buffer,
		0/*row_stride*/,
		NULL/*colormap*/
	)) {
		return FAIL("write %s: %s", filename, image.message );
	}
	return PASS("%s",filename);
}

bool WAX::png_one:: calc_bytes_per() // and bytes_per_pixel
{
	u32_RGBA_t rgba; 
	bytes_per_pixel = rgba.bytes_per_pixel();
	bytes_per_row = PNG_IMAGE_ROW_STRIDE( image );

	// general expectation is non zero
	if( image.width < 1 ) {
		return FAIL("image width %d", image.width );
	}
	if( image.height < 1 ) {
		return FAIL("image height %d", image.height );
	}
	return true;
}

/*!
	solid fill XYWH with rgba pixel value
*/
bool WAX::png_one:: draw_fill_xywh( int x1, int y1, int w, int h, u32_RGBA_t rgba )
{
	u32_RGBA_t::test_byte_order();
	int x2_clip = image.width; // 1 outside
	int y2_clip = image.height; // 1 outside

	int x2 = x1 + w;
	int y2 = y1 + h;

	if( x1 < 0 ) x1 = 0;
	if( y1 < 0 ) y1 = 0;

	if( x2 > x2_clip ) x2 = x2_clip;
	if( y2 > y2_clip ) y2 = y2_clip;

	// draw upto not over X2 nor Y2
	for( int y = y1; y < y2; y++ ) {
	  u32_RGBA_t * pixel_row = get_EA_row( y ); // or *P++ = rgba; //
	  for( int x = x1; x < x2; x++ ) {
	  	pixel_row[ x ] = rgba;
	//	u32_RGBA_t * pixel = get_EA( x, y );
	//	* pixel = rgba;
	  }
	}
	return true;
}

bool WAX::png_one:: draw_nasty_blob() // test
{
	int x1 = image.width / 4;
	int y1 = image.height / 4;
	int w = x1 * 2;
	int h = y1 * 2;


	u32_RGBA_t rgba;
	rgba.red(); // opaque

	if(0) {
	rgba.R = 0xFF;
	rgba.G = 0x00; // leaves purple not white
	rgba.G = 0xFF; // white
	rgba.B = 0xFF;
	rgba.B = 0x00; // yellow
	rgba.A = 0x00; // transparent
	rgba.A = 0xFF; // opaque
	}

	return draw_fill_xywh( x1, y1, w, h, rgba );

	return true;
}
