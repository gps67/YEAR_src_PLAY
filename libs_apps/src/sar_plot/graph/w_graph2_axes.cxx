#include "w_graph2.h"
#include "dset_api.h"
#include "mem_line_spec.h"
#include "dset_api_segs_map.h"

#include "flt80_2048_pair.h"
#include "tm_parts.h"
#include "d_text.h"
#include "axis_draw_xtime.h"

/*!
	Draw the X and Y axes, ticks and numbers

	REFACTOR: draw lines using abstract or template

	INPUTS:
		xmap - the known x axis range
		ymap - 

	TODO:
		The x_axis layout should be prepared and held in vars
		plot_line() calls  xmap.set_val_range( t0, t2 );
		which should call layout_x_axis() which effects y areas
		h_axis h_gap dow_fmt ... would be retained
*/
void w_graph2::draw_axes()
{

/////////////////////////////////////////////////////////////////////////
	// X axis
/////////////////////////////////////////////////////////////////////////

	axis_draw_xtime xaxis( this );
	xaxis.set_scale( xmap );
	xaxis.recalc_layout();
	int y_axis_origin = ymap.scn_lo;
	int y_top = ymap.scn_hi;
	xaxis.draw_all( y_axis_origin, y_top );

/////////////////////////////////////////////////////////////////////////
	// Y axis
/////////////////////////////////////////////////////////////////////////

	axis_draw_y yaxis( this );
	yaxis.set_scale( ymap );
	yaxis.recalc_layout();
	int x_axis_origin = xmap.scn_lo;
	int x_right = xmap.scn_hi;
	yaxis.draw_all( x_axis_origin, x_right );

	e_print("draw_axes() - DONE\n" );
	return;
}


///////////////////////////////////////////// graph2

/*!
	and y axis too
*/
void w_graph2::layout_x_axis()
{
	// set screen area
	// to be upgraded ... measure font sizes and amount of labelling
	/*
		DAFT - but not
		xmap is set after its used
		The available screen area is reduced by the axis
		which MIGHT change the layout used, hence the area used!
		However thats life, and only slightly wrong
	*/
	axis_draw_xtime draw_xaxis( this );
	draw_xaxis.set_scale( xmap );
	draw_xaxis.recalc_layout();
	h_axis = draw_xaxis.layout_height(); // extra height of x axis
	
	w_axis = 50; // width  of y axis
	h_gap =  20; // gap at top
	w_gap =  20; // gap at right
	
	xmap.set_scn_range( w_axis, width() - w_gap );
	ymap.set_scn_range( height() - h_axis, h_gap ); // NEGATIVE LURK
	
	// setup_yscale - MUST BE REFACTORED, eg draw_axes needs it
	setup_yscale( data1 );
}
