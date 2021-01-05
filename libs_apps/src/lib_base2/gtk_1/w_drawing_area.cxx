#include "w_drawing_area.h"

/*!
	destructor - delete pixmap
*/
 w_drawing_area::
~w_drawing_area()
{
	if (pixmap)
	{
		gdk_pixmap_unref(pixmap);
		pixmap = NULL;
	}
}

/*!
	constructor - allocate w, connect events
*/
w_drawing_area::
w_drawing_area()
{
	w_set( gtk_drawing_area_new());
	pixmap = NULL;

	connect_event_configure();	// resize action
	connect_event_expose();		// redraw

	// connect_event_size_request();	// resize negociate
	// connect_event_mouse_moved();		// cross-hairs
}

/*!
	VIRTUAL - "configure" means the size has been decided (changed)
*/
void
w_drawing_area::
event_configure( GdkEventConfigure *event )
{
	reallocate_pixmap();
}

/*!
	re-allocate a pixmap the same size as the screen window
*/
void
w_drawing_area::
reallocate_pixmap()
{
	if (pixmap) {
		gdk_pixmap_unref(pixmap);
		pixmap = NULL;
	}

	pixmap = gdk_pixmap_new(
		w->window,
		w->allocation.width,
		w->allocation.height,
		-1
	);
	blank_it();
	pixmap_reallocated();
}

/*!
	VIRTUAL

	The application gets told when a new pixmap has been allocated
	(and probably configure). eg set axes, and first draw 
*/
void
w_drawing_area::
pixmap_reallocated()
{
	e_print("w_drawing_area::pixmap_reallocated() -client should blank_it\n" );
}

/*!
	Redraw a part of the screen by copying from buffer

	hide and show the XOR'd annotations before and after.
	(NB maybe optimise multiple zones by not flushing SHOW
	until idle)

	Few apps will override this
*/
void
w_drawing_area::
event_expose( GdkEventExpose *event )
{
	expose_event_hide_annotations( TRUE );
	gdk_draw_pixmap(
		w->window,
		w->style->fg_gc[GTK_WIDGET_STATE (w)],
		pixmap,
		event->area.x, event->area.y,
		event->area.x, event->area.y,
		event->area.width, event->area.height
	);
	expose_event_hide_annotations( FALSE );
}

/*!
	VIRTUAL - HIDE/SHOW annotations

	App can draw a few annotations to the screen, which are not
	on the pixmap (eg crosshairs)

	The w_graph uses XOR which requires exact counts
*/
void w_drawing_area::expose_event_hide_annotations( bool hide )
{
	/*
		eg w_graph2 draws the cross-hairs to the SCREEN
		and not to the PIXMAP. These need to be removed
		whilst an expose copies pixmap to screen, then
		re-added.

		This is the same idea as hide_pointer()
	*/
}

/*!
	blank the pixmap to  w->style->bg_gc[GTK_WIDGET_STATE (w)]
*/
void
w_drawing_area::
blank_it()
{
	gdk_draw_rectangle(
		pixmap,
		w->style->bg_gc[GTK_WIDGET_STATE (w)],
		TRUE,
		0, 0,
		w->allocation.width,
		w->allocation.height
	);
	/*
		debug
	*/
	if(0) {
		int w4 = width();
		int w1 = w4/4;
		int w3 = w4 - w1;
		int w2 = w3 - w1;

		int h4 = height();
		int h1 = h4/4;
		int h3 = h4 - h1;
		int h2 = h3 - h1;

		gdk_draw_rectangle(
			pixmap,
		get_gc_black(), 
			TRUE,
			w1, h1,
			w2, h2
		);
	}
}

/*!
	VIRTUAL - pixmap or screen
*/
GdkDrawable * w_drawing_area::get_drawable()
{
	if( !w ) return NULL;
	if( pixmap ) return pixmap;
	return w->window; 
}

/*!
	track mouse - eg as cross-hairs
*/
void w_drawing_area::event_mouse_moved( int x, int y )
{
	e_print("w_drawing_area::event_mouse_moved(%d,%d)\n", x, y );
}

extern void gdb_break_point();

/*!
	DELETE THIS
*/
void w_drawing_area:: draw_lines_black( GdkPoint * points, int n )
{
	if(!w) {
		e_print("w_drawing_area::NULL widget w\n");
		return;
	}
	draw_lines_gc( get_gc_black(), points, n );
}

/*!
	DELETE THIS
*/
void w_drawing_area:: draw_lines_gc( GdkGC * gc, GdkPoint * points, int n )
{
	if(!pixmap) {
		e_print("w_drawing_area::NULL pixmap\n");
		return;
	}
	// gdb_break_point();
	if(0) e_print("w_drawing_area::%d points\n", n);
	if(0) for( int i=0; i<n; i++ )
	{
		if( i>=n ) break;
		e_print("Point[%d](%d,%d)\n", i, points[i].x, points[i].y );
	}

	gdk_draw_lines(
		pixmap,
		gc,
		points,
		n
	);
	if(0) {
		e_print("W_H(%d,%d)\n", width(), height() );
		gdk_draw_line(
			pixmap,
			get_gc_black(),
			0, 0,
			width(), height()
		);
	}
	// remember to call redraw to copy the pixmap
}

/*!
	DELETE THIS
*/
void w_drawing_area::draw_line( GdkGC * gc, int x1, int y1, int x2, int y2 )
{
	gdk_draw_line( pixmap, gc, x1, y1, x2, y2 );
}


