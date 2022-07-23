#include "png_one.h"
#include "dgb.h"

#include "X_STUBS.h"
#include "A_point_plus.h"
#include "X_Window.h"
#include "X_Pixmap.h"
#include "X_Draw.h"
#include "X_Display.h"


#include "fmt_system.h"
#include "buffer2.h"
#include <stdlib.h>
using namespace WAX;


class X_test_png : public X_Window {
 public:
//	A_Rectangle xywh1;      // the inner frame // eg this is border
	png_one & png;
	GC gc;

	XImage * ximage;
	X_Pixmap pixmap;

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
		call_create_image_from_png( png );
	}

	bool list_depths()
	{
		return disp->test_list_depths();
	}

	bool call_create_image_from_png( png_one & png )
	{
		// copy png to ximage //
		// png must be the one suitable for window
		// ie it is already png2

		/*
			png buffer uses u32_RGBA_t lohi
WAX::u32_RGBA_t::test_byte_order() # RGBA A=0xFF 0xFF332211 AAGGBBRR

			ximage SAYS it uses u32_ARGB_
			ximage->  red_mask FF0000
			ximage->green_mask   FF00
			ximage-> blue_mask     FF

			ie 0xAARRGGBB // maybe without AA

			The problem is that ximage and png share buffer
			maybe there are two invertions on the way to screen

		*/

		// png buffer
		// ximage 

		int pad =8; // bits per pixel
		int depth = 32; depth = 24; // of screen
		list_depths(); // 24 1 4 8 15 16 32 //
		depth = disp->Default_Depth();
		INFO("default depth of screen 0 = %d\n", depth );

		int bytes_per_line = 0;
		bytes_per_line = 4 * png.image.width;
		bytes_per_line = 0; // maybe it figures that out

		INFO("png  W %d H %d", png.image.width, png.image.height );
		ximage = ximage = XCreateImage(
			disp->display,
			disp->get_Default_Visual(), // vanilla plus
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

		A_WH WH( png.image.width, png.image.height );
		if(!pixmap.create( *this, WH ))
			return FAIL_FAILED();
		call_put_image_to_pixmap();

		return true;
	}

	void event_expose( A_Rectangle & xywh ){

		X_Draw draw_green(*this);
		const char * colour_spec_green = "#00FF00";
		XColor green_col = disp->cmap.Parse_Alloc( colour_spec_green );
		draw_green.set_fg( green_col );

		A_Rectangle xywh1 = xywh;
		A_Rectangle xywh2 = xywh;
		A_Rectangle xywh_m1 = xywh;
		xywh_m1.reduce2(1); // reduce 2 sides

		A_Point xy3( xywh2.x+10, xywh2.y+10 );
                const char * str = "abc\ndef";
                draw_green.XDrawRectangle( xywh_m1 ); // -1 of WH is X11 off?
                draw_green.XDrawString( xy3, str );
		// width of line is >1 pixel ?
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

		// call_put_image_to_window();
		if(!call_put_pixmap_to_window())
			FAIL_FAILED();// as part of expose redraw
		return;
	}

	bool show_image_data( XImage * ximage )
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
		const char * s = "UNKNOWN";
		switch(ximage->byte_order) {
		case LSBFirst: s = "LSBFirst"; break;
		case MSBFirst: s = "MSBFirst"; break;
		default: ;
		}
		INFO( "ximage->byte_order %d == %s", ximage->byte_order, s );
		return true;
	}

	bool call_put_pixmap_to_window() // as part of expose redraw
	{
		INFO("XFlush - pre");
		disp->XFlush();

		Drawable src = pixmap.pixmap;
		Drawable dst = drawable; // this-> drawable window
		int src_x = 0;
		int src_y = 0;
		int width = pixmap.WH.w; 
		int height = pixmap.WH.h;
		int dst_x = 0;
		int dst_y = 0;

	//	width *= 0.9; // TEST // should reveal half a green X on bg
		width *= 0.5; // TEST // should reveal half a green X on bg

		// clip
		// src_XY
		// src_WH - ie limit of src
		// dst_XY
		// dst_WH - ie limit of dst
		// cpy_WH - sub area of request relative to req_XY
		//
		// this gives the algebra of WH1 ^ WH2 intersection overlap etc
		// a few well known verbs
		// eg ENTIRE AREA (probably same WH)
		// eg SPECIFIC expose AREA (of same entire) (of panel)
		// eg PANEL area // LWW clip zone
		// add in other clip zone (but that also added by OS)

		XCopyArea(
			display,
			src,
			dst,
			gc,
			src_x,
			src_y,
			width,
			height,
			dst_x,
			dst_y
		);

		INFO("sent");
		return true;
	}

	bool call_put_image_to_pixmap() // to pixmap // to drawable might be Pixmap
	{
		INFO("XFlush - pre");
		disp->XFlush();
		int Y_pos = 0;
		int H16 = 16;
		int H_remain = ximage->height;
		while( H_remain > 0 ) {
			int H_band = H_remain;
		if(0)	if( H_band > H16 ) H_band = H16; // disable chunking
			INFO("sending %d rows of %d", H_band, H_remain );
			XPutImage(
				disp->display,
				pixmap.pixmap, // window,
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
			INFO("XFlush");
			disp->XFlush();
			// break;
		}
		INFO("sent");
		return true;
	}

	bool call_put_image_to_window() // to window // to drawable might be Pixmap
	{
			INFO("XFlush - pre");
			disp->XFlush();
		int Y_pos = 0;
		int H16 = 16;
		H16 = (1024 * 32) / ximage->bytes_per_line;
		H16--;
		int H_remain = ximage->height;
		while( H_remain > 0 ) {
			int H_band = H_remain;
		if(0)	if( H_band > H16 ) H_band = H16;
			INFO("sending %d rows of %d", H_band, H_remain );
			XPutImage(
				disp->display,
				drawable,	// dest
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
			INFO("XFlush");
			disp->XFlush();
			// break;
		}
		INFO("sent");
		return true;
	}

};

// global to outlive main() ??
png_one png1;

bool bool_main( int argc, char ** argv ) {

	X_Display disp( NULL );
	X_Window::register_root( disp, "R-O-O-T" );


	const char * filename_1 = "/tmp/filename_1.png";
	const char * filename_2 = "/tmp/filename_2.png";
	const char * filename_3 = "/tmp/filename_3.png";

	filename_1 = "/home/gps/Downloads/sea_ice_vol_zoomed_out.png";
	filename_1 = "/home/gps/Pictures/wordle/wordle_4/Screenshot from 2022-07-06 04-42-53.png";
	filename_1 = "/home/gps/Downloads/meme/JWST/STScI-01G7DDBW5NNXTJV8PGHB0465QP.png";




 if(1) {
	if(!png1.read_from_file( filename_1 )) return FAIL_FAILED();
	// png1.draw_nasty_blob(); // test RGBA 
  if(0) {
	if(!png1.write_to_file( filename_2 )) return FAIL_FAILED();
	buffer2 cmd;
	cmd.print( "m %s", filename_2 );
	system( (STR0) cmd );
  }
 if(0)
  	fmt_system("m '%s'", filename_1 );
 }

 if(0) {
	png_one png3;
	if(!png3.create( 300,700 ) ) return FAIL_FAILED();
	png3.draw_nasty_blob(); // test RGBA 
	if(!png3.write_to_file( filename_3 )) return FAIL_FAILED();
	buffer2 cmd;
	cmd.print( "m %s", filename_3 );
	system( (STR0) cmd );
 }


	// copy reduce png1 to png2

	// TODO // resize WIN where png is displayed //
	// that instead of display_WH/3
	// get initial WH from display_WH/3
	// think about zoom > 1.0 
	// float zoom = 1.0 / ratio;
	// dst = src * zoom

 	A_WH display_WH;
	if(!disp.guess_screen_size(display_WH)) return FAIL_FAILED();

	float ratio = (1.0 * png1.image.width) / (0.3 * display_WH.w);
	INFO(" ratio %5f = png1.image.width %d / (0.3 * display_WH.w %d ) ",
		ratio, png1.image.width, display_WH.w
	);
	// ratio is 2.5/1 for big image
	// triple scale down to fit in 1/3 of screen // 8/1 ish
	// src / dst = 7.0 // src = 7.0 * dst
	// src = dst * ratio
	// dst = src / ratio
	// 576 = 4537 / 7.876


	A_WH dst_WH;
	dst_WH.w = png1.image.width / ratio;
	dst_WH.h = png1.image.height / ratio;

	INFO("src_w %d dst_w %d ratio %5f", png1.image.width, dst_WH.w, ratio );
	INFO("src_h %d dst_h %d ratio %5f", png1.image.height, dst_WH.h, ratio );

	png_one png2;
	png2.create( dst_WH.w, dst_WH.h );


	for( int dst_y = 0; dst_y < dst_WH.h; dst_y++ ) {
	  int src_y = dst_y * ratio;
 //	  INFO("src_y %d dst_y %d ROW ratio %5f", src_y, dst_y, ratio );

	  u32_RGBA_t * src_row = png1.get_EA_row( src_y );
	  u32_RGBA_t * dst_row = png2.get_EA_row( dst_y );

	  for( int dst_x = 0; dst_x < dst_WH.w; dst_x++ ) {
	    int src_x = dst_x * ratio;
	 //   INFO("src_x %d dst_x %d", src_x, dst_x );
	    dst_row[ dst_x ] = src_row[ src_x ]; // v simple resample
	  }
	}

	

	// INFO("PNG  W %d H %d ..", png1.image.width, png1.image.height );
	// 1200 800 // 
	A_Rectangle xywh1( 0, 0, png2.image.width, png2.image.height );
	X_test_png win1( "png1", disp, xywh1, 0, png2 );
	win1.set_title( filename_1 );

	win1.map();

	        long mask = 0;
        mask |= ExposureMask ;
        mask |= KeyPressMask ;
        mask |= ButtonPressMask ;
        mask |= ButtonReleaseMask; // Pointer button up
//      mask |= ResizeRequest ; // NOT SURE WHERE THIS CAME FROM
        mask |= ResizeRedirectMask; // Redirect resize of this window

	win1.XSelectInput( mask );


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
