#ifndef w_drawing_area_H
#define w_drawing_area_H

#include "w_widget0.h"
#include "d_gc.h"

/*!
	A canvas with a backup pixmap.

	Also good for derived classes.
*/
class w_drawing_area : public w_widget0
{
 public:
	// already more than plain drawing area

	//! The double-buffering pixmap
	GdkPixmap *pixmap;

//	GtkDrawingArea * DrawingArea() { return (GtkDrawingArea *) w; }

	//! get the underlying GTK widget
	GtkDrawingArea * DRAWING_AREA() { return (GtkDrawingArea *) w; }

	~w_drawing_area();

	w_drawing_area();

	/*!
		set the gtk size
	*/
	void set_size( int w, int h )
	{
		gtk_drawing_area_size( DRAWING_AREA(), w, h);
	}

  virtual
	void event_configure( GdkEventConfigure *event );

	//! allocate or reallocate a pixmap for current size
  virtual
	void reallocate_pixmap();

	//! clear pixmap to GTK background current state
	void blank_it();

	//! tell app that the pixmap has beenresized, reallocated
  virtual
	void pixmap_reallocated();

	//! expose = redraw = always from pixmap
  virtual
	void event_expose( GdkEventExpose *event);

	//! allow graph2 to draw annotations to screen as well as pixmap
  virtual 
	void expose_event_hide_annotations( bool hide );

  virtual
	void event_mouse_moved( int x, int y );

  virtual
	GdkDrawable * get_drawable();

	void draw_lines_black( GdkPoint * points, int n );

	void draw_lines_gc( GdkGC * gc, GdkPoint * points, int n );

	/*!
		DELETE THIS temp code
	*/
	GdkGC * get_gc_black()
	{
		return w->style->black_gc;
	}

	void draw_line( GdkGC * gc, int x1, int y1, int x2, int y2 );

};

#endif
