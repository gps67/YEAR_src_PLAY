#ifndef tty_scroll_H
#define tty_scroll_H

namespace TTY_CURSES {

 class tty_scroll {

 	int dpi_x256;
	float dpi_float;

	bool set_dpi_float( float dpi ) {
	 dpi_float = dpi;
	 dpi_x256 = int (dpi * 256); // any float_to_int rounding is OK
	 return true;
	}

	XYWH xywh_view_on_scroll_surface;
	XYWH xywh; //_view_on_scroll_surface;
	XYWH xywh; //_view_on_screen_surface;

	XY view_on_scroll; //
	XY view_on_frame; // its window
	XY frame_on_screen; // parent_screen

	NUM y_screen_from_scroll( NUM y ) { return y - xywh.Y; }
	NUM x_screen_from_scroll( NUM x ) { return x - xywh.X; }

	NUM y_frame_from_scroll( NUM y ) { return y - xywh.Y; }
	NUM x_frame_from_scroll( NUM x ) { return x - xywh.X; }

	NUM y_scroll_from_screen( NUM y ) { return y + xywh.Y; }
	NUM x_scroll_from_screen( NUM x ) { return x + xywh.X; }

	NUM y_frame_from_screen( NUM y ) { return y + xywh.Y; }
	NUM x_frame_from_screen( NUM x ) { return x + xywh.X; }

	NUM y_screen_from_frame( NUM y ) { return y + xywh.Y; }
	NUM x_screen_from_frame( NUM x ) { return x + xywh.X; }

	NUM y_scroll_from_frame( NUM y ) { return y + xywh.Y; }
	NUM x_scroll_from_frame( NUM x ) { return x + xywh.X; }

	VOID * Tree; // = NULL

	void goto_XY( const XY & xy ) {
	 
	}
 };

}; // namespace
#endif
