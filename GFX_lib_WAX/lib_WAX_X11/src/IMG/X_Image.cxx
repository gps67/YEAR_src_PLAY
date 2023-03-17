#include "X_Image.h"
#include "X_Pixmap.h"
#include "X_Display.h"
#include "X11/Xutil.h"
using namespace WAX;

/*
	X_Image holds a frame buffer on the client side

	you can send it to the X11 server, fetch it from
	you can draw it on a Pixmap, or on a window
	you can access the individual Pixels RGBA[y][x]

	you can send it to JPEG or PNG saver ... or ... loader

	there is an issue with who does malloc/free
	to save one copy, the lender must keep whilst borrowed

	there is an unexplored world of RGBA -vs- GREY_BYTE
	currently everybody must agree on u32_RGBA

	X_Pixmap now holds it's own X_Display (or at least Display)
	That is an optional extra, coming from X_Drawable_Surface
	(which is not an X approach)
	so we _COULD_ remove all other X_Display * disp parameter routes

*/


X_Image:: X_Image() {
	ximage = NULL;
}

X_Image:: ~X_Image() {
	INFO("sizeof unsigned long %ld", sizeof(unsigned long) );

	del_ximage();
	/*
		the image buffer memory was loaned to us
		it is held by png_one or tj_loader

		The safer alternative, is to malloc and memcopy
		to a block that we hold
	*/
}

bool X_Image:: expect_NULL_ximage() {
	if(!ximage) return true;
	show_image_data("was supposed to be NULL");
	del_ximage();
	return FAIL("expected NULL ximage");
}

bool X_Image:: expect_PTR_ximage() {
	if(ximage) return true;
	return FAIL("expected ximage got NULL");
}

bool X_Image:: get_WH( A_WH & _WH ) {
	if(ximage) {
		_WH.w = ximage->width;
		_WH.h = ximage->height;
		return true;
	} else {
		WARN("NULL ximage");
		_WH.w = 0;
		_WH.h = 0;
		return false;
	}
	return true;
}

bool X_Image:: put_pixel( int x, int y, u32 pixel ) {
	XPutPixel( ximage, x, y, pixel );
	return true;
}

u32 X_Image:: get_pixel( int x, int y ) {
	// XGetPixel returns u64
	// dovetail this to RGBA_t
	// FUTURE u16[4] as RGBA_u16_each_t & retval or get_pixel_64
	return (u32) XGetPixel( ximage, x, y );
}

bool X_Image:: show_image_data( const char * msg ) {
	if(!expect_PTR_ximage()) return FAIL_FAILED();
	//
	INFO( "ximage: %s", msg );
	INFO( "ximage->width %d",            ximage->width );
	INFO( "ximage->height %d",           ximage->height );
	INFO( "ximage->format %d",           ximage->format );
	INFO( "ximage->data %p",             ximage->data );
	INFO( "ximage->byte_order %d LSBFirst/MSBFirst %d/%d",
					     ximage->byte_order,
					     LSBFirst, MSBFirst );
	INFO( "ximage->bitmap_unit %d",      ximage->bitmap_unit );
	INFO( "ximage->bitmap_bit_order %d", ximage->bitmap_bit_order );
	INFO( "ximage->bitmap_pad %d",       ximage->bitmap_pad );
	INFO( "ximage->depth %d",            ximage->depth );
	INFO( "ximage->bytes_per_line %d",   ximage->bytes_per_line );
	INFO( "ximage->bits_per_pixel %d",   ximage->bits_per_pixel );
	// these masks look wrong // fortunately unused ?? //
	INFO( "ximage->  red_mask %6lX",     ximage->red_mask );
	INFO( "ximage->green_mask %6lX",     ximage->green_mask );
	INFO( "ximage-> blue_mask %6lX",     ximage->blue_mask );
	INFO( "ximage->obdata %p",           ximage->obdata );
	const char * s = "UNKNOWN";
	switch(ximage->byte_order) {
	case LSBFirst: s = "LSBFirst"; break;
	case MSBFirst: s = "MSBFirst"; break;
	default: ;
	}
	INFO( "ximage->byte_order %d == %s", ximage->byte_order, s );
	return true;
// 	INFO("default depth = %d\n", DefaultDepth(disp->display, 0));
}

void X_Image:: del_ximage()
{
	if( ximage ) {
		// DROP LEAK memory
		ximage->data = NULL; // X11 never releases anyway
		XDestroyImage(ximage);
		ximage = NULL;
	}
}

bool X_Image:: create_image_from_RGBA_frame_buffer(
	X_Display * disp,
	char * image_buffer, // caller arranges to keep it alloc'd
	A_WH _WH
) {
	expect_NULL_ximage();

	// we asked for these from png and we guess what it provided
	int pad = 8; // bit padding per line // 32 would make sense too
	int depth = 32; depth = 24; // of screen
	// disp->test_list_depths(); // 24 1 4 8 15 16 32 //
	depth = disp->Default_Depth();
	INFO("default depth of screen 0 = %d\n", depth ); // 32

	int bytes_per_line = 0; // zero means auto calc
	bytes_per_line = 4 * _WH.w; // round up to pad 
	bytes_per_line = 0; // maybe it figures that out

	INFO("png  W %d H %d", _WH.w, _WH.h );
	ximage = ximage = XCreateImage(
		disp->display,
		disp->get_Default_Visual(), // vanilla plus
		depth,          // 24 not 32
		ZPixmap,        // RGB triplets not plane of R plane of G plane of B
		0,              // ignore left pixels on scanline
		image_buffer,  // xdata = malloc( 4 * W * H )
		_WH.w,
		_WH.h,
		pad,            // 32 // 8 // bits // bitmap_pad //
		bytes_per_line // bytes_per_line //
	);

	if(! ximage ) return FAIL("ximage NULL but didnt fail");
	show_image_data("created from PNG");
	return true;
}

/*
	X_Image knows a fraction about it's siblings image formats

	someone has to bridge the close gap

	This is all specific to u32 RGBA and near zero gaps per line
*/

bool X_Image:: call_create_ximage_from_png( X_Display * disp, png_one * png ) 
{
	expect_NULL_ximage();
	WARN("TODO test pixelformat");

	if( png->image.format == PNG_FORMAT_RGBA ) { 
	} else {
		return FAIL("expected png->image.format == PNG_FORMAT_RGBA got ...");
		// mayhem follows
	}
	char * image_buffer = (char *)png->buffer;
	A_WH _WH( png->image.width, png->image.height );

	if(! create_image_from_RGBA_frame_buffer( disp, image_buffer, _WH ))
		return FAIL_FAILED();

//		ximage->byte_order = LSBFirst; // 0 // x86 default after NUL
//		ximage->byte_order = MSBFirst; // 1

	INFO( "ximage->data %p - SHARED", ximage->data );
	INFO( "png->buffer   %p - SHARED", png->buffer );

	return true;
}

bool X_Image:: call_create_ximage_from_TJ_IMG( X_Display * disp, TJ:: TJ_FB_image_t * img ) {
	expect_NULL_ximage();
	WARN("TODO get pixelformat from Display and from TJ image");
	/*
	*/
	char * image_buffer = (char *) img->img_buf.buff;
	A_WH _WH( img->width, img->height );

	if(! create_image_from_RGBA_frame_buffer( disp, image_buffer, _WH ))
		return FAIL_FAILED();

	INFO( "ximage->data %p - SHARED", ximage->data );
	INFO( "png->buffer   %p - SHARED", img->img_buf.buff );

	show_image_data("created from TJ_IMG");
	return true;
}

bool X_Image:: call_put_image_to_pixmap( X_Display * disp, GC gc, X_Pixmap * pixmap ) {
	if(!expect_PTR_ximage()) return FAIL_FAILED();
	disp->XFlush();
	int Y_pos = 0;
	int H16 = 16;
	int H_remain = ximage->height;
	while( H_remain > 0 ) {
		int H_band = H_remain;
	if(0)	if( H_band > H16 ) H_band = H16; // disable chunking
		INFO("sending %d rows of %d", H_band, H_remain );
		XPutImage(
			disp->display,	// can move to X_Pixmap.X_Display // ??
			pixmap->drawable, // window,
			gc,
			ximage,
			0,		// src_x
			Y_pos,		// src_y
			0,		// dst_x
			Y_pos,		// dst_y
			ximage->width,    // copied image size
			H_band		// 16 rows per update
		);
		Y_pos += H_band;
		H_remain -= H_band;
		// INFO("XFlush");
		disp->XFlush();
		// break;
	}
	// INFO("sent");
	return true;
}

bool X_Image:: get_ximage_from_pixmap( X_Display * disp, X_Drawable_Surface * pixmap ) {
	del_ximage();
	unsigned long plane_mask = AllPlanes;
	int format = ZPixmap;
	ximage = XGetImage(
		disp->display,
		pixmap->drawable, // drawable
		0,0, // x, y,
		pixmap->WH.w, // width,
		pixmap->WH.h, // height,
		plane_mask,
		format
	);
	show_image_data("created from Pixmap");
	return ximage; // not NULL
}

bool X_Image:: put_ximage_into_TJ_IMG( TJ:: TJ_FB_image_t * img_tj ) {
	img_tj->pixel_format_tj = TJPF_BGRX; // HOW ??
	img_tj->width = ximage->width;
	img_tj->height = ximage->height;
	img_tj->img_buf.buff = (unsigned char *) ximage->data;
	img_tj->img_buf.size = -1; // unknown
	img_tj->img_buf.borrowed = true;		// it is not tj to release
	// caller must continue to loan out ximage data
	return true;
}


// ---------------------------

bool X_Image:: create_pixmap_from_ximage( X_Display * disp, GC gc, Drawable drawable, X_Pixmap * pixmap ) {
	if(!expect_PTR_ximage()) return FAIL_FAILED();
	A_WH WH;
	get_WH(WH); // ( ximage->width, ximage->height );
	if(! pixmap->create( drawable, WH ))
		return FAIL_FAILED();
	if(! call_put_image_to_pixmap( disp, gc, pixmap ))
		return FAIL_FAILED();
	return true;
}

bool X_Image:: create_pixmap_from_png( X_Display * disp, GC gc, Drawable drawable, X_Pixmap * pixmap, png_one * png ) {
	if(! call_create_ximage_from_png( disp, png ))
		return FAIL_FAILED();
	if(! create_pixmap_from_ximage( disp, gc, drawable, pixmap ))
		return FAIL_FAILED();
	return true;
}

bool X_Image:: create_pixmap_from_TJ_IMG( X_Display * disp, GC gc, Drawable drawable, X_Pixmap * pixmap, TJ:: TJ_FB_image_t * img ) {
	if(! call_create_ximage_from_TJ_IMG( disp, img ))
		return FAIL_FAILED();
	if(! create_pixmap_from_ximage( disp, gc, drawable, pixmap ))
		return FAIL_FAILED();
	return true;
}

#if 0
bool X_Image:: create_pixmap_from_file( X_Display * disp, GC gc, Drawable drawable, X_Pixmap * pixmap, const char * filename ) {
	return FAIL("TODO");
}
#endif


