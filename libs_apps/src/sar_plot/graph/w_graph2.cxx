#include "w_graph2.h"
#include "dset_api.h"
#include "mem_line_spec.h"
#include "dset_api_segs_map.h"

#include "flt80_2048_pair.h"
#include "tm_parts.h"

// #include "draw_axis_xtime.h"

#define MAXINT (unsigned(-1) >>1)


/*!
	Desstructor - plain
*/
w_graph2::~w_graph2()
{
}

/*!
	Constructor -
*/
w_graph2::w_graph2()
: data1()
{
	// initially there is no pixmap
	// shoud maybe catch expose/realloc/...
	// W doesnt exist yet ?? // set_invisible_cursor();

	connect_event_mouse_moved();
}

/*!
	fetch the data from dset_src and plot them (axes?)

	TODO: move this to graph3
*/
void w_graph2::plot_line(
	dset_api * dset_src,
	mem_line_spec * spec,
	time_t t0,
	time_t t2
)
{
	xmap.set_val_range( t0, t2 );

	time_t t0_xs = t0 - 20 * spec->dt;
	time_t t2_xs = t2 + 20 * spec->dt;

	e_print("w_graph2::plot_line() ");
	spec->print1(stderr);

	int nmax = ( t2_xs - t0_xs ) / spec->dt;
	if( nmax > 2048 ) nmax = 2048;
	data1.init_1( t0_xs, t2_xs, nmax );

	// get the data into data1, segment by segment
	dset_src->select_segs_to_dset(
		& data1,
		spec,
		t0_xs, 		// (time_t)xmap.val_lo,
		t2_xs		// (time_t)xmap.val_hi
	);

	if(1)
	{
		if(0) data1.smudge3();
	} else {
		flt80 xs = 1.0;
		data1.count.each_add( xs );
		xs *= data1.smudge( 1 );
		data1.count.each_sub( xs );
		data1.plain_avg();
	}

	if((!pixmap )||(!w) )
	{
		e_print("There is no pixmap/w\n");
		return;
	}

	// clear + draw all
	pixmap_reallocated();
}

/*!
	Gtk has (re-) allocated the pixmap, or a complete redraw is needed,
	(ie after some datachange). EG: window resized, or initially opened.

	This doesnt fetch the data, thats already loaded  by plot_line()
*/
void w_graph2::pixmap_reallocated()
{
	gc_hair = NULL;
	blank_it();	// done by base class with bg white
	set_invisible_cursor();

	layout_x_axis(); // should be called by plot line, and also resize

	data1_plot_green();
	draw_axes();
	data1_plot_black();

	request_redraw(); // copy bitmap to display
	event_mouse_moved( x_hair, y_hair );
}


/*!
	data1 is already from the database, data2 is copied from data1.
*/
void w_graph2::data2_from_data1( flt80_2048_pair & data2, int w2 )
{
	data3_from_data2( data2, w2, data1 );
}

/*!
	copy/project reducing the number of slots
*/
void w_graph2::data3_from_data2( flt80_2048_pair & data3, int w2, flt80_2048_pair & data2 )
{
	// cannot expand but can compress. Want: n2 <= n1
	int n1 = data2.N();
	int n2 = w2;
	if( n1 < n2 ) n2 = n1;

	data3.init_1( data2.t_lo(), data2.t_hi(), n2 );
	data3.calc1 = data2.calc1;

	for( int i=0; i<n1; i++ )
	{
if(0)		if( data2.count[i] < 0.75 ) continue;

		flt80 xval = data2.xval_of_idx( i );
		flt80 yval = data2.val[ i ];
		flt80 cnt  = data2.count[ i ];

		data3.merge_point( xval, yval, cnt );
	}
	// data3.first_avg();
}

/*!
	recalculate the y axis scale from the data
*/
void w_graph2::setup_yscale( flt80_2048_pair & data2 )
{
/*
	? use spec to lookup preset scale
*/
/*
	changing y_scale comes from avg functions changing ??
*/
	// start with the full range
	flt80 y_lo;
	flt80 y_hi;
	data2.find_hi_lo(  y_hi, y_lo );
	e_print("y_lo=%6.3f y_hi=%6.3f (MAX)\n", (float)y_lo, (float)y_hi);

	flt80 y;
	flt80 y_avg, y_vary; // member: y_lo, y_hi, 
	data2.calc_vary( y_avg, y_vary );

	y_hi = y_avg *2.0;
	y_lo = y_avg /2.0;
	y_lo = 0.0;

	// set apparent varience to at least 10%
	if(0) {
		y = y_avg / 10;
		if( y_vary < y ) y_vary = y;
	}

	// plot -3 .. +5 variance, but no maore than MAX
	y = y_avg - 3 * y_vary;
	y = y_avg / 2.0;
	if( y > y_lo) y_lo = y;

	y = y_avg + 3 * y_vary;
	y = y_avg * 2.0;
	if( y < y_hi) y_hi = y;
	y_hi = y;

	if(0) data2.val.find_hi_lo(  y_hi, y_lo );

	y_lo = 0;
	y_hi *= 2;

	ymap.set_val_range( y_lo, y_hi );
if(0)	ymap.set_val_range( 0, 100 );
}

/*!
	do a minimum of averaging and plot the data as lines
*/
bool w_graph2::data1_plot_black()
{
	if(!data1.N()) return FALSE; // happens when no data from DB ?

	// reduce 2048 to 715 (screen width)
	// LURK - what if 40-->715 ? (p[i].x != i )
	flt80_2048_pair data2;
	int  w1 = xmap.scn_hi - xmap.scn_lo;
	data2_from_data1( data2, w1 );
	w1 = data2.N();
	
 if(0)
 {
	flt80 xs = 1.0;
	data2.count.each_add( xs );
	xs *= data2.smudge(1);
	data2.count.each_sub( xs );
 }
	// data2.smudge(1);

	/*
		plain_avg MUST be called, as data2_from_data1()
		can add multiple items to 1 slot
	*/
	data2.plain_avg();

	if(0) setup_yscale( data2 ); // CALLED AT WRONG TIME

	GdkPoint p[ w1 ];
	int n = data2.N();
	int i2 = 0;	// allow skip of early points
	for( int i =0; i<n; i++ )
	{
		if( data2.count[i] < 0.75 ) continue;
		p[i2].x = (int) xmap.scn_of_val( data2.xval_of_idx(i) );
		p[i2].y = (int) ymap.scn_of_val( data2.val[i] );
		if( p[i2].x < xmap.scn_lo ) continue;
		if( p[i2].x > xmap.scn_hi ) break;
		i2++;
	}
	n = i2;
	if(1) e_print("merge_floats plotting %d points\n", n );
	if(1) draw_lines_black( p, n);

	return TRUE;
}

/*!
	do a lot of averaging and plot the solid area under the lines
*/
bool w_graph2::data1_plot_green()
{
	if(!pixmap) return FALSE;

	if(!data1.N()) return FALSE; // happens when no data from DB ?

	if( ! gc_green )
	{
		if(! pixmap ) {
			return FALSE;
		}
		gc_green = new d_gc( pixmap );
		gc_green->set_fg( "green" );
		/*
			pixmap might change but the screen they
			came from should be the same, so this gc is OK
		*/
	}

	flt80_2048_pair data2;

	// pick a resolution to reduce to
	// reduce from 2048

	// w1 = screen width which is 350 or 715 or
	int  w1 = xmap.scn_hi - xmap.scn_lo;
	// w4 = count for grain of 4 pixels - reduces 715 to 178
	int w4 = w1 / 4;
	// int w2 = w1 / 2;

	// sometimes - at hi res view there are not 2048 data, but 130

	if( w1 > data1.N() ) w1 = data1.N();
	if( w4 > w1 ) w4 = w1;
	if( w4 > (data1.N() / 2)) w4 = data1.N();

 if(0)
 {
	// reduce the data down to w1 points (NB exceeds x axis!)
	data2_from_data1( data2, w1 );
	w1 = data2.N();
 } else {
	// get a full size copy (720)
	flt80_2048_pair data2a;

	data3_from_data2( data2a, data1.N(), data1 );

	// damp it down - but this reduces the real data average
	// real data will hopefully be several in count
	// partially it makes absent data have a zero value
	if(0) data2a.count.each_add(1.1);

	// smudge it early
	if(1) data2a.smudge(1);
	if(0) data2a.smudge(8);
	// data2a.plain_avg();

	// reduce to 70 points
	// reduce the data down to w4 points (NB exceeds x axis!)
	e_print("reducing from %d to %d points", data1.N(), w4 );
	data3_from_data2( data2, w4, data2a );

	// damp it down when count per item is highest
	if(0) data2.count.each_add(1.1); // looks OK but .77 .98 @ 3 hrs
	if(1) data2.count.each_add(0.2);

	// smudge it late
	if(1) data2.smudge(3);
 }

 if(1)
 {
	// data2.count_add_1(); - desperate - but no effect
	// data2.smudge(3);
 }else if(1)
 {
	data2.smudge3();

 } else ;

	/*
		This must be called last to reduce count to 1 (or absent)

		In theory, it only needs to be called at the end, as
		upto then, val/count is always valid, and flt80 is
		wider than arith overflow
	*/
	data2.plain_avg();

	/*
		This has to be called elsewhere - before ?
	*/
	if(0) setup_yscale( data2 ); // CALLED AT WRONG TIME

	// int x1 = xmap.scn_lo;
	GdkPoint p1[ w4 + 2 ];
	GdkPoint * p2 = & p1[1];
	int n = data2.N();
	int j = 0;
	// bool skipped = false;
	for( int i =0; i<n; i++ )
	{
#if 0
		if( data2.count[i] < 0.15 )
		{
			if( skipped ) continue;
			skipped = true;
			continue;
		}
		if( skipped )
		{
			p2[j].y = p2[ j-1 ].y;
			p2[j].x = (int) xmap.scn_of_val( data2.xval_of_idx(i-1) );
			if( p2[j].x < xmap.scn_lo ) continue;
			if( p2[j].x > xmap.scn_hi ) break;
			j++;
			skipped = false;
		}
#endif
		p2[j].y = (int) ymap.scn_of_val( data2.val[i] );
	//	p2[j].x = x1 + i;
		p2[j].x = (int) xmap.scn_of_val( data2.xval_of_idx(i) );
		if( p2[j].x < xmap.scn_lo ) continue;
		if( p2[j].x > xmap.scn_hi ) break;
		j++;
	}
	if(1) e_print("merge_floats plotting %d points\n", n );
	if(0) draw_lines_black( p2, n);


	// draw_lines_gc( gc_green->gc, p2, w4);
	gint filled = true;
	p1[0].x = xmap.scn_lo;
	p1[0].y = ymap.scn_lo;
	p2[j].x = xmap.scn_hi;
	p2[j].y = ymap.scn_lo;
	gdk_draw_polygon(
		pixmap,
		 gc_green->gc,
		filled,
		p1,
		j + 2
	);
	if(1) draw_lines_black( p2, j);

	return TRUE;
}

/*!
	Draw or undraw the cross-hairs using XOR, at the RECORDED x_hair.

	To avoid redrawing the entire pixmap, the lines are XOR'd onto
	the screen and not the pixmap. This relies on window clipping.

*/
void w_graph2::xor_grid_hairs()
{
	// first run initialises gc_hair
	if(!gc_hair) {
		if(!pixmap) return;
		gc_hair = new d_gc( pixmap );
		gc_hair->set_fg( "green" ); // white XOR green is purple
		gc_hair->set_xor();
		return; // first is OFF second is ON
	}

	if(0) e_print("XOR %d %d\n", x_hair, y_hair );
	// NOTE - draw to window but not to pixmap!
	// a lot faster than draw();

	// The bounds of the lines
	int x_lo = x_hair - 30;
	int x_hi = x_hair + 30;
	int y_lo = y_hair - 30;
	int y_hi = y_hair + 30;

	// Not 30 pixels, but the screen-area extents
	int x1 = xmap.scn_lo;
	int w1 = xmap.scn_hi - xmap.scn_lo;
	int x2 = x1 + w1 - 1;
	int y1 = ymap.scn_lo;
	int y2 = ymap.scn_hi;

	if(1) {
		x_lo = x1;
		y_lo = y1;
	}

	if(1) {
		x_hi = x2;
		y_hi = y2;
	}

	// extend left/below axis to read agains ticks
	if(1) {
		x_lo -= 5;
		y_lo += 5;
	}

	gdk_draw_line(
		w->window,	// SCREEN not pixmap
		gc_hair->gc,
		x_lo, y_hair,
		x_hi, y_hair
	);

	gdk_draw_line(
		w->window,	// SCREEN not pixmap
		gc_hair->gc,
		x_hair, y_lo,
		x_hair, y_hi
	);
}

/*!
	Move the cross-hairs using two XOR's
*/
void w_graph2::xor_grid_hairs(int x, int y)
{
	xor_grid_hairs();
	x_hair = x;
	y_hair = y;
	xor_grid_hairs();
}

/*!
	Hook that is called at the right times
*/
void w_graph2::expose_event_hide_annotations( bool hide )
{
	xor_grid_hairs();
}

/*
		derived class should do this
		Note how (x,y) becomes (time,yval)

	void w_graph2::event_mouse_moved(int x, int y)
	{
		xor_grid_hairs( x, y );
		//
		time_t t_hair = xmap.time_val_of_scn( x );
		flt80   yval  = ymap.val_of_scn( y );
	//	cross_hairs_at( t_hair, yval );
	}

	// VIRTUAL
	void w_graph2::cross_hairs_at( time_t tval, flt80  yval )
	{
		e_print("BASE w_graph2::cross_hairs_at( %d, %f ) -- ", tval, yval );
		debug_print_ln();
	}
*/

/*!
	When the mouse is over the graph, use gridhairs, and a hidden cursor.

	This function creates a cursor bitmap (with a blank design),
	and sets it.

	The code is very comparable with an example, and retained with
	non-invisible functionality for future use.
*/
#define cursor0_width 16
#define cursor0_height 16
#define N32 (cursor0_width*2)
void w_graph2::set_invisible_cursor()
{
	static gchar cursor0_bits[N32] = { 0 };
	static gchar cursor0mask_bits[N32] = { 0 };

	GdkCursor *cursor;
	GdkPixmap *source, *mask;
	GdkColor fg = { 0, 65535, 0, 0 }; /* Red. */
	GdkColor bg = { 0, 0, 0, 65535 }; /* Blue. */

	source = gdk_bitmap_create_from_data(
		NULL,
		cursor0_bits,
		cursor0_width,
		cursor0_height
	);
	mask = gdk_bitmap_create_from_data(
		NULL,
		cursor0mask_bits,
		cursor0_width,
		cursor0_height
	);
	cursor = gdk_cursor_new_from_pixmap (source, mask, &fg, &bg, 8, 8);
	gdk_pixmap_unref (source);
	gdk_pixmap_unref (mask);

	gdk_window_set_cursor (w->window, cursor);
}

// error: narrowing conversion of ‘128’ from ‘int’ to ‘gchar {aka char}
// #define xx80 (gchar) 0x80

/*!
*/
#define cursor1_width 16
#define cursor1_height 16
void w_graph2::set_diamond_cursor()
{
	// Pasted from Gtk tutorial/sample code
	/* This data is in X bitmap format, and can be created with the 'bitmap'
	utility. */
	static const unsigned char cursor1_bits[] = {
	0x80, 0x01, 0x40, 0x02, 0x20, 0x04, 0x10, 0x08, 0x08, 0x10, 0x04, 0x20,
	0x82, 0x41, 0x41, 0x82, 0x41, 0x82, 0x82, 0x41, 0x04, 0x20, 0x08, 0x10,
	0x10, 0x08, 0x20, 0x04, 0x40, 0x02, 0x80, 0x01};

	static const unsigned char cursor1mask_bits[] = {
	0x80, 0x01, 0xc0, 0x03, 0x60, 0x06, 0x30, 0x0c, 0x18, 0x18, 0x8c, 0x31,
	0xc6, 0x63, 0x63, 0xc6, 0x63, 0xc6, 0xc6, 0x63, 0x8c, 0x31, 0x18, 0x18,
	0x30, 0x0c, 0x60, 0x06, 0xc0, 0x03, 0x80, 0x01};

	GdkCursor *cursor;
	GdkPixmap *source, *mask;
	GdkColor fg = { 0, 65535, 0, 0 }; /* Red. */
	GdkColor bg = { 0, 0, 0, 65535 }; /* Blue. */

	source = gdk_bitmap_create_from_data(
		NULL,
	 (const gchar *)
		cursor1_bits,
		cursor1_width,
		cursor1_height
	);
	mask = gdk_bitmap_create_from_data(
		NULL,
	 (const gchar *)
		cursor1mask_bits,
		cursor1_width,
		cursor1_height
	);
	cursor = gdk_cursor_new_from_pixmap (source, mask, &fg, &bg, 8, 8);
	gdk_pixmap_unref (source);
	gdk_pixmap_unref (mask);

	gdk_window_set_cursor (w->window, cursor);
}

#define SET_MIN( VAR, VAL ) if( VAR < VAL ) VAR = VAL

/*!
	ensure that the graph height is a minimum
*/
void w_graph2::event_size_request( GtkRequisition *req )
{
	// calc_layout
	SET_MIN( req->height, 200 );
}
