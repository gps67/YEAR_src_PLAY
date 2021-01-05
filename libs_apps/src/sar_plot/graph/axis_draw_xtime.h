#ifndef axis_draw_xtime_H
#define axis_draw_xtime_H

#include "axis_draw.h"

typedef enum Fmt_dow
{
	fmt_dow_none,
	fmt_dow_ticks,
	fmt_dow_W,
	fmt_dow_Wed,
	fmt_dow_Wednesday
};

typedef enum Fmt_mon
{
	fmt_mon_none,
	fmt_mon_ticks,
	fmt_mon_J,
	fmt_mon_Jan,
	fmt_mon_January
};

/*!
	Linear X axis, with labels and ticks

	This time x-axis is multi-line with time of day, day of week, month, ...

	Caller must set_scale( xmap )
*/
class axis_draw_xtime : public axis_draw_base
{
 public:

	flt80 calc_pix_per_second();
	flt80 calc_pix_per_hour();
	flt80 calc_pix_per_day();
	flt80 calc_pix_per_month();

	int y_1;
	int y_2;

	//  y_top;	is zero - relative to Y0
	int y_pos;	// current pos rel to Y0 used within layout / redraw
	int y_btm;

	//! The colour / dottedness of dotted lines (refactor?)
	d_gc line_dotted;

	// tod_ time of day
	int tod_tick1_dt;
	int tod_tick1_height;
	int tod_tick2_dt;
	int tod_tick2_height;
	int tod_tick3_dt;
	int tod_tick3_height;
	int tod_tick4_dt;
	int tod_tick4_height;

	Fmt_dow fmt_dow; //  = fmt_dow_none;

	//  y_dow_top;
	int y_dow_baseline;
	int y_dow_btm;

	Fmt_mon fmt_mon; //  = fmt_mon_none;

//	//! pixel height of major ticks
//	int tick1_height;
//	//! pixel height of minor ticks
//	int tick2_height;
//	//! number of tick2 intervals 's per tick1, 5 is normal (4 drawn)
//	int tick2_per_tick1;

	axis_draw_xtime( w_widget0 * graph );

	void recalc_layout();
	void recalc_layout_tod();
	void recalc_layout_dow();
	void recalc_layout_mon();

//	int layout_width();
	//! additional space required
	flt80 layout_height();

//	char * str_val( flt80 val );
	void draw_all( int y0, int y2 );
	void draw_tod_line();
	void draw_dow_line();
	void draw_mon_line();
	void draw_horiz_line( int y );
	void tick_each( flt80 y_lo, flt80 y_hi, flt80 tod_tick1_dt );

	// str WILL get overwritten / truncated
	void mid_l1( int x1, int x2, char * str );
	void mid_l2( int x1, int x2, char * str );
	void mid_l_base( int x1, int x2, char * str, int & x_A, int & x_B );

};

#endif

