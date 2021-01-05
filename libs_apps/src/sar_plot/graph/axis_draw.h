#ifndef axis_draw_H
#define axis_draw_H

#include "obj_hold.h"
#include "w_widget0.h"
#include "axis_scale.h"
#include "d_text.h"
#include "d_gc.h"

#include "w_graph2.h" // get pixmap


/*!
	Base class for drawing axes, with ticks and labels

	TODO: refactor with abstract (x11) drawing utils classes 

	TODO: non-linear scale
*/
class axis_draw_base : public GRP_sar_plot
{
 public:

	//! to draw text
	d_text text;
	//! to be refactored out
	d_gc   gc;
	//! the scale maps VAL->POS and ticks
	axis_scale_linear scale;

	/*!
		constructor - prep to draw on widgets drawable
	*/
	axis_draw_base( w_widget0 * wid )
	: text( wid )
	, gc( wid )
	, scale()
	{
		// NOT an obj_ref // text.ref_fixed();
		gc.ref_fixed();
	}

	//! switch drawable -- ?
 	void set_drawable( GdkDrawable * drawable )
	{
		text.set_drawable( drawable );
	}

	//! assign scale
	void set_scale( axis_scale_linear & _scale )
	{
		scale = _scale;
	}

	//! draw a line - LURK mixed metaphor of gc
	void draw_line( d_gc & gc1, int x1, int y1, int x2, int y2 )
	{
		gdk_draw_line( text.drawable, gc1.gc, x1, y1, x2, y2 );
	}
};

/*!
	Linear Y axis, with labels and ticks
*/
class axis_draw_y : public axis_draw_base
{
 public:
	//! The colour / dottedness of dotted lines (refactor?)
	d_gc line_dotted;
	//! 123.45 has 3 digits left
	int digits_left;
	//! 123.45 has 2 digits right
	int digits_right;
	//! cached step value for tick level Z, as VAL not pos
	flt80 val_step1;
	//! pixel width of major ticks
	int tick1_width;
	//! pixel width of minor ticks
	int tick2_width;
	//! number of tick2 intervals 's per tick1, 5 is normal (4 drawn)
	int tick2_per_tick1;

	axis_draw_y( w_widget0 * graph );
	void recalc_layout();
	int layout_width();
	char * str_val( flt80 val );
	void draw_all( int x1, int x2 );
};

#endif
