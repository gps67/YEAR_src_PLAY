#include "png_one.h"
#include "dgb.h"

#include "A_point_plus.h"
#include "X_Window.h"
#include "X_Pixmap.h"
#include "X_Draw.h"
#include "X_Display.h"

#include "TJ_blk.h"
#include "TJ_FB_image.h"
#include "TJ_loader.h"
#include "IMG_loader.h"

#include "fmt_system.h"
#include "buffer2.h"
#include <stdlib.h>
using namespace TJ;
using namespace WAX;

#include "IMG_loader.h"

	bool WAX:: show_image_data( XImage * ximage )
	{
// 	INFO("default depth = %d\n", DefaultDepth(disp->display, 0));
		INFO( "ximage->width %d", ximage->width );
		INFO( "ximage->height %d", ximage->height );
		INFO( "ximage->format %d", ximage->format );
		INFO( "ximage->data %p", ximage->data );
		INFO( "ximage->byte_order %d LSBFirst/MSBFirst %d/%d", ximage->byte_order, LSBFirst, MSBFirst );
		INFO( "ximage->bitmap_unit %d", ximage->bitmap_unit );
		INFO( "ximage->bitmap_bit_order %d", ximage->bitmap_bit_order );
		INFO( "ximage->bitmap_pad %d", ximage->bitmap_pad );
		INFO( "ximage->depth %d", ximage->depth );
		INFO( "ximage->bytes_per_line %d", ximage->bytes_per_line );
		INFO( "ximage->bits_per_pixel %d", ximage->bits_per_pixel );
		INFO( "ximage->  red_mask %6lX", ximage->red_mask );
		INFO( "ximage->green_mask %6lX", ximage->green_mask );
		INFO( "ximage-> blue_mask %6lX", ximage->blue_mask );
		INFO( "ximage->obdata %p", ximage->obdata );
		const char * s = "UNKNOWN";
		switch(ximage->byte_order) {
		case LSBFirst: s = "LSBFirst"; break;
		case MSBFirst: s = "MSBFirst"; break;
		default: ;
		}
		INFO( "ximage->byte_order %d == %s", ximage->byte_order, s );
		return true;
	}


	bool WAX:: call_create_ximage_from_png( X_Display & disp, XImage * & ximage, png_one & png )
	{
		WARN("TODO test pixelformat");
	
		int pad =8; // bits per pixel
		int depth = 32; depth = 24; // of screen
		disp.test_list_depths(); // 24 1 4 8 15 16 32 //
		depth = disp.Default_Depth();
		INFO("default depth of screen 0 = %d\n", depth );
	
		int bytes_per_line = 0;
		bytes_per_line = 4 * png.image.width;
		bytes_per_line = 0; // maybe it figures that out
	
		INFO("png  W %d H %d", png.image.width, png.image.height );
		ximage = ximage = XCreateImage(
			disp.display,
			disp.get_Default_Visual(), // vanilla plus
			depth,          // 24 not 32
			ZPixmap,        // RGB triplets not plane of R plane of G plane of B
			0,              // ignore left pixels on scanline
			(char *)png.buffer,  // xdata = malloc( 4 * W * H )
			png.image.width,
			png.image.height,
			pad,            // 32 // 8 // bits // bitmap_pad //
			bytes_per_line // bytes_per_line //
		);
//		ximage->byte_order = LSBFirst; // 0 // x86 default after NUL
//		ximage->byte_order = MSBFirst; // 1
		INFO( "ximage->data %p - SHARED", ximage->data );
		INFO( "png.buffer   %p - SHARED", png.buffer );
	
		show_image_data( ximage );
		return true;
	}

	bool WAX:: call_create_ximage_from_TJ_IMG( X_Display & disp, XImage * & ximage, TJ_FB_image_t & img )
	{
		WARN("TODO test pixelformat");
	
		int pad =8; // bits per pixel
		int depth = 32; depth = 24; // of screen
		disp.test_list_depths(); // 24 1 4 8 15 16 32 //
		depth = disp.Default_Depth();
		INFO("default depth of screen 0 = %d\n", depth );
	
		int bytes_per_line = 0;
		bytes_per_line = 4 * img.width;
		bytes_per_line = 0; // maybe it figures that out
	
		INFO("png  W %d H %d", img.width, img.height );
		ximage = ximage = XCreateImage(
			disp.display,
			disp.get_Default_Visual(), // vanilla plus
			depth,          // 24 not 32
			ZPixmap,        // RGB triplets not plane of R plane of G plane of B
			0,              // ignore left pixels on scanline
			(char *)img.img_buf.buff,  // xdata = malloc( 4 * W * H )
			img.width,
			img.height,
			pad,            // 32 // 8 // bits // bitmap_pad //
			bytes_per_line // bytes_per_line //
		);
//		ximage->byte_order = LSBFirst; // 0 // x86 default after NUL
//		ximage->byte_order = MSBFirst; // 1
		INFO( "ximage->data %p - SHARED", ximage->data );
		INFO( "png.buffer   %p - SHARED", img.img_buf.buff );
	
		show_image_data( ximage );
		return true;
	}

	bool WAX:: call_put_image_to_pixmap( X_Display & disp, GC gc, XImage & ximage, X_Pixmap & pixmap ) // to pixmap // to drawable might be Pixmap
	{
		INFO("XFlush - pre");
		disp.XFlush();
		int Y_pos = 0;
		int H16 = 16;
		int H_remain = ximage.height;
		while( H_remain > 0 ) {
			int H_band = H_remain;
		if(0)	if( H_band > H16 ) H_band = H16; // disable chunking
			INFO("sending %d rows of %d", H_band, H_remain );
			XPutImage(
				disp.display,
				pixmap.pixmap, // window,
				gc,
				& ximage,
				0,		// src_x
				Y_pos,		// src_y
				0,		// dst_x
				Y_pos,		// dst_y
				ximage.width,    // copied image size
				H_band		// 16 rows per update
			);
			Y_pos += H_band;
			H_remain -= H_band;
			INFO("XFlush");
			disp.XFlush();
			// break;
		}
		INFO("sent");
		return true;
	}

	bool WAX:: create_pixmap_from_ximage( X_Display & disp, GC gc, Drawable drawable, X_Pixmap & pixmap, XImage & ximage )
	{
		A_WH WH( ximage.width, ximage.height );
		if(! pixmap.create( disp.display, drawable, WH ))
			return FAIL_FAILED();
		if(! call_put_image_to_pixmap( disp, gc, ximage, pixmap ))
			return FAIL_FAILED();
		return true;
	}

	bool WAX:: create_pixmap_from_png( X_Display & disp, GC gc, Drawable drawable, X_Pixmap & pixmap, png_one & png )
	{
		XImage * ximage = NULL;
		if(! call_create_ximage_from_png( disp, ximage, png ))
			return FAIL_FAILED();
		if(! ximage ) return FAIL("ximage NULL but didnt fail");
		if(! create_pixmap_from_ximage( disp, gc, drawable, pixmap, * ximage ))
			return FAIL_FAILED();
		// tested not null above
		WARN("XDestroyImage() does it delete borrowed memory?");
		ximage->data = NULL;
		     XDestroyImage(ximage);
	//	if(! XDestroyImage(ximage) )	// not a bool retval
	//		return FAIL_FAILED();
		return true;
	}

	bool WAX:: create_pixmap_from_TJ_IMG( X_Display & disp, GC gc, Drawable drawable, X_Pixmap & pixmap, TJ_FB_image_t & img )
	{
		XImage * ximage = NULL;
		if(! call_create_ximage_from_TJ_IMG( disp, ximage, img ))
			return FAIL_FAILED();
		if(! ximage ) return FAIL("ximage NULL but didnt fail");
		if(! create_pixmap_from_ximage( disp, gc, drawable, pixmap, * ximage ))
			return FAIL_FAILED();
		// tested not null above
		WARN("XDestroyImage() does it delete borrowed memory?");
		ximage->data = NULL;
		     XDestroyImage(ximage);
	//	if(! XDestroyImage(ximage) )	// not a bool retval
	//		return FAIL_FAILED();
		return true;
	}
