#include "png_one.h"
#include "dgb.h"

#include "X_STUBS.h"
#include "A_point_plus.h"
#include "X_Window.h"
#include "X_Draw.h"
#include "X_Display.h"


#include "buffer2.h"
#include <stdlib.h>
using namespace WAX;


class X_test_png : public X_Window {
 public:
//	A_Rectangle xywh1;      // the inner frame // eg this is border
	png_one & png;
	GC gc;

	XImage * ximage;

 	X_test_png(
		const char * _name,
		X_Display & disp_,
		A_Rectangle xywh,
		int border,
		png_one & png_
	)
	: X_Window( _name, disp_, xywh, border )
//	, xywh1( xywh )
	, png( png_)
	{
		gc = CreateGC();
		call_create_image();
	}

	bool call_create_image()
	{
		int pad =8; // bits per pixel
		int depth = 32; depth = 24; // of screen
		int bytes_per_line = 0;
		bytes_per_line = 4 * png.image.width;
		bytes_per_line = 0; // maybe it figures that out

		INFO("png  W %d H %d", png.image.width, png.image.height );
		ximage = ximage = XCreateImage(
			disp->display,
			disp->get_Default_Visual(), // vanilla plus
			depth,          // 32
			ZPixmap,        // RGB triplets not plane of R plane of G plane of B
			0,              // ignore left pixels on scanline
			(char *)png.buffer,  // xdata = malloc( 4 * W * H )
			png.image.width,
			png.image.height,
			pad,            // 32 // 8 // bits // bitmap_pad //
			bytes_per_line // bytes_per_line //
		);
		ximage->byte_order = LSBFirst; // 0
		ximage->byte_order = MSBFirst; // 1 // x86 default after NUL

		show_image( ximage );
		return true;
	}

	void event_expose( A_Rectangle & xywh ){

		X_Draw draw_green(*this);
		const char * colour_spec_green = "#00FF00";
		XColor green_col = disp->cmap.Parse_Alloc( colour_spec_green );
		draw_green.set_fg( green_col );

		A_Rectangle xywh1 = xywh;
		A_Rectangle xywh2 = xywh;

		A_Point xy3( xywh2.x+10, xywh2.y+10 );
                const char * str = "abc\ndef";
                draw_green.XDrawRectangle( xywh1 );
                draw_green.XDrawString( xy3, str );
                draw_green.XDrawLine(
                        xywh1.x,
                        xywh1.y,
                        xywh1.x_last(),
                        xywh1.y_last()
                );
                draw_green.XDrawLine(
                        xywh1.x_last(),
                        xywh1.y,
                        xywh1.x,
                        xywh1.y_last()
                );

		call_put_image();
		return;
	}

	bool show_image( XImage * ximage )
	{
	INFO("default depth = %d\n", DefaultDepth(disp->display, 0));

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
		return true;
	}

	bool call_put_image() // to window // to drawable might be Pixmap
	{
			INFO("XFlush - pre");
			disp->XFlush();
		int Y_pos = 0;
		int H16 = 16;
		H16 = (1024 * 32) / ximage->bytes_per_line;
		H16--;
		int H_remain = png.image.height;
		while( H_remain > 0 ) {
			int H_band = H_remain;
		if(0)	if( H_band > H16 ) H_band = H16;
			INFO("sending %d rows of %d", H_band, H_remain );
			XPutImage(
				disp->display,
				window,
				gc,
				ximage,
				0,		// src_x
				Y_pos,		// src_y
				0,		// dst_x
				Y_pos,		// dst_y
				png.image.width,    // copied image size
				H_band		// 16 rows per update
			);
			Y_pos += H_band;
			H_remain -= H_band;
			INFO("XFlush");
			disp->XFlush();
			// break;
		}
		INFO("sent");
		return true;
	}

};

png_one png;

bool bool_main( int argc, char ** argv ) {

	X_Display disp( NULL );
	X_Window::register_root( disp, "R-O-O-T" );


	const char * filename_1 = "/tmp/filename_1.png";
	const char * filename_2 = "/tmp/filename_2.png";
	const char * filename_3 = "/tmp/filename_3.png";

	filename_1 = "/home/gps/Downloads/sea_ice_vol_zoomed_out.png";


 if(1) {
	if(!png.read_from_file( filename_1 )) return FAIL_FAILED();
	png.draw_nasty_blob(); // test RGBA 
  if(0) {
	if(!png.write_to_file( filename_2 )) return FAIL_FAILED();
	buffer2 cmd;
	cmd.print( "m %s", filename_2 );
	system( (STR0) cmd );
  }
 }

 if(0) {
	png_one png;
	if(!png.create( 300,700 ) ) return FAIL_FAILED();
	png.draw_nasty_blob(); // test RGBA 
	if(!png.write_to_file( filename_3 )) return FAIL_FAILED();
	buffer2 cmd;
	cmd.print( "m %s", filename_3 );
	system( (STR0) cmd );
 }

	// INFO("PNG  W %d H %d ..", png.image.width, png.image.height );
	// 1200 800 // 
	A_Rectangle xywh1( 0, 0, png.image.width, png.image.height );
	X_test_png win1( "png1", disp, xywh1, 0, png );
	win1.set_title( filename_1 );

	win1.map();

	win1.XSelectInput(
	 ExposureMask | KeyPressMask   | ButtonPressMask |ResizeRequest
	 );


	// the main loop uses a virtual on the relevent X_Window // TODO
	disp.process_events_forever();

	return true;
}

int main( int argc, char ** argv )
{
	if( bool_main( argc, argv )) {
		return 0;
	} else {
		if(errno) return errno;
		return 1;
	}
}
