#include "png_one.h"
#include "dgb.h"
#include <memory.h> // memset

GFX::png_one:: png_one()
{
	image_memset_zero(); // and then set version no
	buffer = NULL;
}

GFX::png_one:: ~png_one()
{
	clear();
}

void GFX::png_one:: clear()
{
	buffer_free(); // 
	image_free();
}

void GFX::png_one:: buffer_free()
{
	if(buffer)
            free(buffer);
	buffer = NULL;
}

void GFX::png_one:: image_memset_zero()
{
      memset(&image, 0, sizeof image);
      image.version = PNG_IMAGE_VERSION;
}

void GFX::png_one:: image_free()
{
	png_image_free(&image); // the opaque contents not the struct itself
}

bool GFX::png_one:: read_from_file( const char * filename )
{
      // clear(); // of image

	if(!png_image_begin_read_from_file(&image, filename )) {
           return FAIL( "%s: %s", filename, image.message);
	}

	// we only do RGBA atm
	image.format = PNG_FORMAT_RGBA;

	buffer_free();
	buffer = (png_bytep) malloc(PNG_IMAGE_SIZE(image));
	if (!buffer) {
		return FAIL("malloc: out of memory: %lu bytes\n",
		       (unsigned long)PNG_IMAGE_SIZE(image));
	}

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

	return PASS("%s",filename);
}

bool GFX::png_one:: write_to_file( const char * filename )
{

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

bool GFX::png_one:: draw_nasty_blob() // test
{
	if( image.width < 10 ) {
		return FAIL("image width %d", image.width );
	}
	return true;
}
