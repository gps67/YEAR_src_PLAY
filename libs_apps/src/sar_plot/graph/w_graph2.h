#ifndef w_graph2_H
#define w_graph2_H

#include "w_graph1.h"
#include "obj_hold.h"

class dset_api;
class mem_line_spec;

#include "flt80_2048_pair.h"

#include "sar_top_app_signals.h"
#include "axis_scale.h"

#include "axis_draw.h"

class axis_draw_xtime;

/*!
	w_graph2 does most of the work that sar_plot requires,
	but you must define a sub-class (w_graph3) which:

		knows what to do with the updated cursor position
		fetches the data

	TODO: move code from graph2 to graph3, and reuse graph2 for
	other things. line_plot() ADD virtual cross_hair_now_at()

	TODO: rework gc's and drawings (eg for pdf)
*/
class w_graph2 : public w_graph1
{
 public:
//	axis_draw_xtime * draw_xaxis;
//	axis_draw_y	draw_yaxis;

	axis_scale_linear xmap;
	axis_scale_linear ymap;

	virtual void pixmap_reallocated();
	virtual void event_mouse_moved(int x, int y) =0;
	virtual void event_size_request( GtkRequisition *req );

	obj_hold<d_gc> gc_hair;
	obj_hold<d_gc> gc_green;
	obj_hold<d_gc> gc_black;
	obj_hold<d_gc> gc_axis_black;

	//! data_store [2048] independent of screen width
	// AS LONG AS SCREEN WIDTH < 2048!
	flt80_2048_pair data1;

	int x_hair;
	int y_hair;
	void xor_grid_hairs();
	void xor_grid_hairs( int x, int y );
	virtual void expose_event_hide_annotations( bool hide );
//	virtual void cross_hairs_at( time_t tval, flt80  yval );

	int h_axis; // height of x axis (beyond any overlap)
	int w_axis; // width  of y axis
	int h_gap; // gap at top
	int w_gap; // gap at right
/*
	enum t_dow_fmt
	{
		dow_off,
		dow_ticked,
		dow_w,
		dow_weds,
		dow_wednesday
	};
	t_dow_fmt dow_fmt;
*/

	void layout_x_axis();

	bool data1_plot_black();
	bool data1_plot_green();

	~w_graph2();
	w_graph2();

	void set_invisible_cursor();
	void set_diamond_cursor();

	// MAIN call to plot a line (the only line?)
	void plot_line( dset_api * dset, mem_line_spec * spec,  time_t t0, time_t t2 );

	//! condense data1 to data2
	void data2_from_data1( flt80_2048_pair & data2, int w2 );
	void data3_from_data2( flt80_2048_pair & data3, int w2, flt80_2048_pair & data2 );

	//! calculate y_lo y_hi and y_factor
	void setup_yscale( flt80_2048_pair & data2 );

	void draw_axes(); // AFTER y_factor set // w_graph2_axes.cxx
};


#endif

