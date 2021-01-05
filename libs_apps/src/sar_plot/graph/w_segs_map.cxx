#include "w_segs_map.h"

#include "obj_hold.h"
#include "w_widget0.h"
#include "d_colour.h"

#include "dset_api_segs_map.h"
#include "tm_parts.h"
#include <time.h>
#include <math.h>

#include "sar_top_app.h"

#ifdef GDB_FIXED_STATIC_CONST
	// const static int N4 = 5;	// number of rows
#else
#define N4 w_segs_map_N4
#endif

/*!
*/
bool C_button_release1(
	GtkWidget *widget,
	GdkEventButton * event,
	w_segs_map * self
	// w_widget0 * self
) {
	time_t t1 = self->find_time_for_xy( (int)event->x,(int) event->y );
	if(1) e_print( "C_button_release1 %d\n", (int) t1 );
	if( t1 < 100 ) return TRUE;
	self->top->layout_selected_time( t1 );
	return TRUE;
}

/*!
*/
void w_segs_map::event_size_request( GtkRequisition *req )
{
	// ONLY called before first mapping - not on resize after!
	gc_init();
	calc_layout_1(); // when font is available
	if(0) e_print("w_segs_map::size_request( h = %d )\n", req->height );
	// force height, any width
	req->height = h_map ;
}

/*!
*/
w_segs_map::w_segs_map( sar_top_app * _top )
: w_drawing_area()
{
	top = _top;
	segs_map = new dset_api_segs_map();
	gc_font1 = NULL;
	gc_lines = NULL;
	gc_fill_absent = NULL;
	gc_fill_present = NULL;
        connect_event_size_request();                                                 
	w_segs = 2; // width() not yet known
	if(0) calc_layout_1(); // when font is available

	init_years_now();
	// this is crude - click interpretation is NAFF
	connect(
		// "button-release-event",
		"button-press-event",
		(SignalFunc) C_button_release1,
		this
	);
	gtk_widget_add_events ( w, GDK_BUTTON_PRESS_MASK );
}

/*!
*/
void w_segs_map::calc_layout_1()
{
	h_line = 1;
	h_gap_above = 0;
	h_gap_below = 0;

	w_line = 1;
	w_gap = 0;

	if(1) {
		h_gap_above++;	// there is an APPARENT gap below = descender
		w_gap++;	// so cheap and it looks good
	}
	if(0) {
		h_gap_above++;	// extra padding to look good
		h_gap_below++;	// lift 'Aug' g above next line!
	}

	/*
		font IS available (else sigsegv)
		calc layout to figure the height min
	*/

	int lbearing;
	int rbearing;
	int width4;
	int h_ascent;
	int h_descent;
	gdk_string_extents(
		get_font1(),
		"1999",
		&lbearing,
		&rbearing,
		&width4,
		&h_ascent,
		&h_descent
	);
	width4--; // tight left
	// NEVER // h_ascent--; // tight top
	/*
		SEEMS MAYBE MY MATH IS ALL WRONG!!
		EG MAYBE THE BASELINE IS +1
		BUT WHERE IS THE SECOND +1 COMING FROM !!!


		THIS IS BASED ON THE BOUNDING BOX
		THAT DOESNR ACTUALLY OVERLAP THE DRAWN TEXT
		THATS DIFFERENT FROM VISUALLY CENTERED THOUGH
	*/
if(1)	h_descent ++; // THIS IS WRONG
if(1)	h_descent ++; // THIS IS WRONG
	// note that numbers go ONTO the baseline
	if(1) e_print("lbearing/rbearing = %d/%d\n", lbearing, rbearing );

	// h_baseline is the position of the baseline relative to the TLC
	h_baseline = h_line + h_gap_above + h_ascent;
	// h_baseline is the position of the nextline relative to the TLC
	h_row      = h_line + h_gap_above + h_ascent + h_descent + h_gap_below; 
	h_map = h_row * N4 + h_line;

	// unlike rows, the w_segs includes both end lines
	// and the lines overlap - remove that!
	w_labels = w_line + w_gap + width4 + w_gap ; // + w_line;
}

/*!
*/
void w_segs_map::calc_layout_2()
{
	int w_of_something = 1; // width() is 1 beyong right!
	w_segs = width() - w_labels - w_of_something;
}

/*!
*/
void w_segs_map::get_map( dset_api * dset_src, mem_line_spec * spec )
{
	time_t t_lo = 0;
	time_t t_hi = unsigned(-1)>>1;
	segs_map->clear();
	dset_src->list_segs_to_dset( segs_map, spec, t_lo, t_hi );
	segs_map->merge_all();
	for( int i=0; i<N4+1; i++ )
	{
		segs_map->split( year_start[i] );
	}

	pixmap_reallocated();
}

/*!
*/
void w_segs_map::pixmap_reallocated()
{
	if( !pixmap ) return;
	gc_init();
	calc_layout_2();
	paint();
}

/*!
*/
GdkGC * w_segs_map::get_gc_white()
{
	return w->style->white_gc;
}

/*!
*/
GdkGC * w_segs_map::get_gc_red()
{
	static GdkGC * gc_green = NULL;
	if( gc_green ) return gc_green;
	if (! pixmap ) return get_gc_black();

	gc_green = gdk_gc_new( pixmap );
	/*
		pixmap might change but the screen they
		came from should be the same, so this gc is OK
	*/
	d_colour * colour = Colour_Map->create_new_colour( "red" );
	gdk_gc_set_foreground( gc_green, & colour->colour );
	return gc_green;
}

/*!
*/
GdkGC * w_segs_map::get_gc_green()
{
	static GdkGC * gc_green = NULL;
	if( gc_green ) return gc_green;
	if (! pixmap ) return get_gc_black();

	gc_green = gdk_gc_new( pixmap );
	/*
		pixmap might change but the screen they
		came from should be the same, so this gc is OK
	*/
	d_colour * colour = Colour_Map->create_new_colour( "green" );
	gdk_gc_set_foreground( gc_green, & colour->colour );
	return gc_green;
}

/*!
*/
void w_segs_map::gc_init()
{
	/*
		initialise the fonts and colours
		NB: need a font to calculate the height, 
		which happens during init - before pixmap available ?
	*/
	if( gc_font1 ) return; // already done
	if( !pixmap ) return;
//	PangoFontDescription* font_desc = pango_font_description_new();
	gc_font1 = get_gc_black();
	gc_lines = get_gc_black();
	gc_fill_absent = get_gc_green();
	gc_fill_present = get_gc_black();
	gc_highlight = get_gc_red();
//	if(!font_desc) return;
	if(0) calc_layout_1(); // when font is available // caller does
}

/*!
*/
GdkFont * w_segs_map::get_font1()
{
#ifdef WITH_GTK2
	return gtk_style_get_font( w->style );
#else
	return w->style->font;
#endif
}

/*!
*/
int w_segs_map::find_row_for_time( time_t t1 )
{
	for( int i = 0; i<N4; i++ )
	{
		if(( year_start[i] <= t1 ) && ( t1 < year_start[i+1] ))
			return i;
	}
	return -1;
}

/*!
*/
flt80 w_segs_map::xfraction1( int row, time_t t1 )
{
	// int row = find_row_for_time( t1 );
	if( row < 0 ) return 0;
	int time_of_year = t1 - year_start[ row ];
	int length_of_year = year_start[ row+1 ] - year_start[ row ];
	flt80 fraction = time_of_year;
	fraction /= length_of_year;
	return fraction;
}

/*!
*/
int w_segs_map::xpos_for_row_time( int row, time_t t1 )
{
	int xb = w_labels;

	flt80 fraction = xfraction1( row, t1 );
	fraction *= w_segs;
	fraction += xb;
	return (int)rint(fraction);
}

/*!
*/
void w_segs_map::init_years_now()
{
	tm_parts tm;
	tm.local_from_time( time(NULL) );
	year0 = tm.year() - N4 + 1;
	tm.init1();
	for( int i=0; i<N4+1; i++ )
	{
		tm.year( year0 + i );
		if(!tm.mktime_from_local()) FAIL_FAILED();
		year_start[i] = tm.tval;
	}
	highlight_set( year_start[1], year_start[1] + 7*24*60*60 );
}

/*!
*/
void w_segs_map::paint_frame()
{
	// locals
	tm_parts tm;
	int x = 0;
	int y = 0;

	int xa = 0;		// left visible
	int xb = xa + w_labels; // right visible
	int xc = width() - 1;

	int ya = 0;		// top
	// int yc = h_line + h_row * N4;

	// horizontal lines
	for( int i=0; i<N4+1; i++ )
	{
		y = h_row * i ;
		draw_line( gc_lines, xa, y, xc, y  );
	}

	// vertical lines // y still set
	int yc = y;
	draw_line( gc_lines, xa, ya, xa, yc );
	draw_line( gc_lines, xb, ya, xb, yc );
	draw_line( gc_lines, xc, ya, xc, yc );

	// x of months
	/*
		It looks ugly to adjust for the leap year,
		which re-aligns at dec31 anyway (fixed width proportion)
		So use the first years xpos
	*/
	// tm_parts tm;
	tm.init1();
	int x_m[ 14 ];
	tm.year( year0 );
	x_m [ 0 ] = w_labels;
	for( int m = 1; m <= 13; m++ )
	{
		tm.mm( m );
		if(!tm.mktime_from_local()) FAIL_FAILED();
		time_t t1 = tm.tval;
		x = xpos_for_row_time( 0, t1 );
		x_m[ m ]  = x;

		// vertical line
		draw_line( gc_lines, x, ya, x, yc );
	}

	// year numbers
	tm.init1();
	char buff[8];
	for(int i = 0; i<N4; i++ )
	{
		y = h_row * i + h_baseline;
		sprintf( buff, "%d", year0+i );
		gdk_draw_text( pixmap,
			get_font1(),
			gc_font1,
			xa+w_line+w_gap,
			y,
			buff,
			strlen(buff)
		);
	}
	y = 0;


	// i8n will need alternative to 3 letter width
	int lbearing;
	int rbearing;
	int width3;
	int h_ascent;
	int h_descent;
	gdk_string_extents(
		get_font1(),
		"Jan",
		&lbearing,
		&rbearing,
		&width3,
		&h_ascent,
		&h_descent
	);
	width3--; // tight left
	int w_Jan = width3;
	int dx_t = (x_m[2] - x_m[1] - w_Jan) / 2;
	if( dx_t < 0 ) dx_t = 0;
	e_print( "# CENTER %4d %4d %4d %4d\n", 
		x_m[2], x_m[1], w_Jan, dx_t );
	// month m is from 1 // gdb_invoke(1);

	for( int i=0; i<N4; i++ )
	{
		int y0 = h_row * i;
		int y_t = y0 + h_baseline;
		int y2 = y0 + h_row ;

		for( int m = 1; m <= 12; m++ )
		{
		/*
			int x1 = x_m[ m ];
			int x3 = x_m[ m+1 ];
			int x2_lo = x1 + w_line + w_gap;
			int x2 = (x1 + x3) / 2 - w_Jan;
			if( x2 < x2_lo ) x2 = x2_lo;
			if(0) draw_line( gc_lines, x1, y0, x1, y2  );
		 */

			int x_t = x_m[ m ] + w_line + dx_t;
			const char * Jan = tm.Jan_from_MM( m );

		/*
			// reduce month to single letter
			char buff[2] = { '*', 0 };
			buff[0] = *Jan;
			Jan = buff;
		 */
			gdk_draw_text( pixmap,
				get_font1(),
				gc_lines,
				x_t,
				y_t,
				Jan,
				strlen(Jan)
			);
		}
	}
}

/*!
*/
void w_segs_map::paint_segments()
{
	int n = segs_map->N();
	for( int i = 0; i<n; i++ )
	{
		time_t t0 = segs_map->t0_t2[i].t0;
		time_t t2 = segs_map->t0_t2[i].t2;

		int row = find_row_for_time( t0 );
		if( row < 0 ) continue;

		int x_green  = xpos_for_row_time( row, t0 );
		int x2_green = xpos_for_row_time( row, t2 );

		int y_green  = (  row   * h_row) + h_line;
		int y2_green = ((row+1) * h_row) - h_line;

		int w_green = x2_green - x_green;
		int h_green = y2_green - y_green;

		// show each segment as a green rectangle
		gdk_draw_rectangle(
			pixmap,
			get_gc_green(), // gc_fill_present,
			1, // filled
			x_green,
			y_green,
			w_green,
			h_green
		);
		// surround green with a border (else ugly)
		gdk_draw_rectangle(
			pixmap,
			get_gc_black(),
			0, // filled
			x_green,
			y_green,
			w_green,
			h_green
		);
		// diagonal line for debug
		if(0) draw_line(
			gc_fill_present,
			x_green, y_green,
			x2_green, y2_green
		);
	}
}

/*!
*/
void w_segs_map::paint()
{
	if(!pixmap) return;
	blank_it();
	paint_frame();
	paint_segments();
	highlight_draw();
	request_redraw(); // BIT-BLT pixmap_to_screen
}

/*!
*/
void w_segs_map::highlight_set(time_t t0, time_t t2 )
{
	highlight_t0 = t0;
	highlight_t2 = t2;
	paint();
}

/*!
*/
void w_segs_map::highlight_draw()
{
	time_t t0 = highlight_t0;
	time_t t2 = highlight_t2;
	for(int i=0; i<N4; i++ )
	{
		time_t ylo = year_start[i];
		time_t yhi = year_start[i+1];
		if( yhi <= t0 ) continue; // year ends before seg starts
		if( t2 <= ylo ) break; // seg ends before year starts
		if( t0 < ylo ) t0 = ylo; // should never happen
		if( t2 > yhi ) {
			highlight_1( t0, yhi );
			t0 = yhi;
			continue; // along next years row
		}
		highlight_1( t0, t2 );
	}
}
/*!
*/
void w_segs_map::highlight_1(time_t t0, time_t t2 )
{
	if( t0 >= t2 ) return;
	int row = find_row_for_time( t0 );
	if( row < 0 ) return;
	int x1 = xpos_for_row_time( row, t0 );
	int x2 = xpos_for_row_time( row, t2 );
	int y1 =    row  * h_row ;
	int y2 = (1+row) * h_row + h_line;

	int w1 = x2 - x1 - 1;
	int h1 = y2 - y1 - 1;

	gdk_draw_rectangle(
		pixmap,
		get_gc_black(),
		0, // filled
		x1,
		y1,
		w1,
		h1
	);

	// surround by 1 pixel
	x1--;
	y1--;
	x2++;
	y2++;

	w1 = x2 - x1 - 1;
	h1 = y2 - y1 - 1;

	if(0) e_print("x,y,w,h = %d,%d,%d,%d\n", x1, y2, w1, h1 );

	gdk_draw_rectangle(
		pixmap,
		gc_highlight,
		0, // filled
		x1,
		y1,
		w1,
		h1
	);

}

/*!
*/
time_t w_segs_map::find_time_for_xy( int x, int y )
{
	flt80 x1;
	x1 = x - w_labels;
	if( x1 < 0 ) return 0;
	int row = y / h_row;
	x1 /= w_segs;
	x1 *= (year_start[row+1] - year_start[row] );
	x1 += year_start[row];
	return (int)rint( x1 );
}

