#ifndef X_Image_H
#define X_Image_H

#include "X_STUBS.h"
// free // #include <stdlib.h>
// strdup // #include <string.h>
#include <X11/Xlib.h>
#include "A_point_plus.h"
#include "png_one.h"
#include "TJ_loader.h"

namespace WAX {

struct X_Image
{
	XImage * ximage;

	// char * buffer_to_free_via_png;	// png_bytep buffer;
	// char * buffer_to_free_via_TJ;	// TJ_blk
	// char * buffer_to_free_via_libc;	// blk1

	X_Image();
	~X_Image();

	void del_ximage();
	bool expect_NULL_ximage(); // return OK
	bool expect_PTR_ximage(); // return OK

	bool get_WH( A_WH & _WH );

	bool put_pixel( int x, int y, u32 pixel );
	u32 get_pixel( int x, int y );
//
// sub panel of window, who allocs mem ?
// XImage *XSubImage(ximage, x, y, subimage_width, subimage_height)
//
// screen dump
// pixmap to local ximage // draw one get other
// XImage *XGetImage(display, d, x, y, width, height, plane_mask, format)
//


	bool show_image_data( const char * msg );
	/*
		frame buffer must have pad32, no gap bytes_per_line, RGBA
	*/
	bool create_image_from_RGBA_frame_buffer(
		X_Display & disp,
		char * image_buffer, // caller arranges to keep it alloc'd
		A_WH _WH
	);
	bool call_create_ximage_from_png( X_Display & disp, png_one & png );
	bool call_create_ximage_from_TJ_IMG( X_Display & disp, TJ:: TJ_FB_image_t & img );
	bool get_ximage_from_pixmap( X_Display & disp, X_Drawable_Surface & pixmap );
	bool put_ximage_into_TJ_IMG( TJ:: TJ_FB_image_t & img );

	// move to X_Drawable_Surface // ??

	bool call_put_image_to_pixmap( X_Display & disp, GC gc, X_Pixmap & pixmap ); // to pixmap // to drawable might be Pixmap
	bool create_pixmap_from_ximage( X_Display & disp, GC gc, Drawable drawable, X_Pixmap & pixmap );

	// double step through X_Image
	// actually move AWAY so that ximage is a temp local

	bool create_pixmap_from_png( X_Display & disp, GC gc, Drawable drawable, X_Pixmap & pixmap, png_one & png );

	bool create_pixmap_from_TJ_IMG( X_Display & disp, GC gc, Drawable drawable, X_Pixmap & pixmap, TJ:: TJ_FB_image_t & img );

//	bool create_pixmap_from_file( X_Display & disp, GC gc, Drawable drawable, X_Pixmap & pixmap, const char * filename );

 }; // X_image
}; // WAX

#endif
