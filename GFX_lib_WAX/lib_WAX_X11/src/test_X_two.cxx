
#include "dgb.h"

// #include <stdlib.h>
// #include <string.h>

#include "X_STUBS.h"
#include "A_point_plus.h"
#include "X_Window.h"
#include "X_Draw.h"
#include "X_Display.h"

// #include "WAX_argv_decoder.h" // they are all very similar
#include "test_X_two.h"
#include "XFT.h"


	void X_test_two::
	event_expose( A_Rectangle & xywh )
	{
		INFO_report(xywh);

		// xywh1 is this window on parent // copy it 
		A_Rectangle xywh2 = xywh1;
		xywh2.reduce4( 50 );
		// a point inside, top left ish
		A_Point xy3( xywh2.x+10, xywh2.y+20 );
		const char * str = "abc\ndef";
		str = "STR0 _t String into draw_green.XDrawString";
		draw_green.XDrawRectangle( xywh1 );
		draw_green.XDrawRectangle( xywh2 );
//		draw_green.XDrawString( xy3, str );
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

		/*
			this shows a stupidity with X11 XDrawRectangle
			the number of pixels drawn is width+1 height+1
			-1	unisgned means 64K wide
			0	*
			1	**
			2	*_*
			3	*__*
		*/

		int x= 31;

#if 0
		// these are 4 wierd tiny squares

		A_Rectangle xywh7( x+=35, 30, 0, 0 );
		draw_green.XDrawRectangle( xywh7 );

		A_Rectangle xywh5( x+=35, 30, 1, 1 );
		draw_green.XDrawRectangle( xywh5 );

		A_Rectangle xywh4( x+=35, 30, 2, 2 );
		draw_green.XDrawRectangle( xywh4 );

		A_Rectangle xywh6( x+=35, 30, 3, 3 );
		draw_green.XDrawRectangle( xywh6 );
#endif

		MY_XFT mine( *this ); // X_Window

		mine.test_redraw();

		Xft_Pen & xft_pen = mine.pen;
		xft_pen.show_XGlyphInfo( xft_pen.pen_extents );
		INFO("that is pen_extents");

		A_Rectangle xywh8(
		//	xft_pen.pen_extents.x,
		//	xft_pen.pen_extents.y,
#if 0
			xft_pen.pen_extents.xOff,
			xft_pen.pen_extents.yOff,
#endif
		// request was 100,100 // is baseline left
		// extents (x,y) is that baseline from top left
		// extents (w,h) is bounding box (from 1
			mine.test_x -
			xft_pen.pen_extents.x,
			mine.test_y -
			xft_pen.pen_extents.y,
			xft_pen.pen_extents.width,
			xft_pen.pen_extents.height
		);
		// 35, 30, 3, 3 );
		draw_green.XDrawRectangle( xywh8 );

		A_Rectangle xywh9(
		//	xft_pen.pen_extents.x,
		//	xft_pen.pen_extents.y,
#if 0
			xft_pen.pen_extents.xOff,
			xft_pen.pen_extents.yOff,
#endif
		// request was 100,100 // is baseline left
		// extents (x,y) is that baseline from top left
		// extents (w,h) is bounding box (from 1
		// (xOff, yOff) is advance from baseline to baseline
			mine.test_x -
	0, //		xft_pen.pen_extents.x,
			mine.test_y -
	0, //		xft_pen.pen_extents.y,
			xft_pen.pen_extents.xOff,
			xft_pen.pen_extents.yOff
		);
		// 35, 30, 3, 3 );
	//	draw_green.XDrawRectangle( xywh9 );
		draw_green.XDrawLine(
			mine.test_x,
			mine.test_y,
			mine.test_x+ xft_pen.pen_extents.xOff,
			mine.test_y+ xft_pen.pen_extents.yOff
		);

	}



