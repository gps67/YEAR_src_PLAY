
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
		str = "STR0 _t String1a";
		draw_green.XDrawRectangle( xywh1 );
		draw_green.XDrawRectangle( xywh2 );
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

		A_Rectangle xywh7( x+=5, 30, 0, 0 );
		draw_green.XDrawRectangle( xywh7 );

		A_Rectangle xywh5( x+=5, 30, 1, 1 );
		draw_green.XDrawRectangle( xywh5 );

		A_Rectangle xywh4( x+=5, 30, 2, 2 );
		draw_green.XDrawRectangle( xywh4 );

		A_Rectangle xywh6( x+=5, 30, 3, 3 );
		draw_green.XDrawRectangle( xywh6 );
	}



