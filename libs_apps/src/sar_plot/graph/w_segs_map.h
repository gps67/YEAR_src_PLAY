#ifndef w_segs_map_H
#define w_segs_map_H

#include "obj_hold.h"
#include "w_widget0.h"

#include "dset_api_segs_map.h"
#include "w_drawing_area.h"

#define GDB_FIXED_STATIC_CONST

class sar_top_app;

/*!
	segs_map is a gui-view of 4 years,
	to show where segs of data is available,
	and where the current cursor position.

	When you click on it, the main graphs shows that day/week
*/
class w_segs_map : public w_drawing_area
{
 public:
	sar_top_app * top;

	// build a list of t0_t2, to find highest t2
	obj_hold<dset_api_segs_map> segs_map;

	GdkGC * gc_font1;
	GdkGC * gc_lines;
	GdkGC * gc_fill_absent;
	GdkGC * gc_fill_present;
	GdkGC * gc_highlight;

	int h_map; // guessed or calculated height

	// The number of year rows
#ifdef GDB_FIXED_STATIC_CONST
	// gdb gui blows up when it sees <optimised out>
	const static int N4 = 7;	// number of rows
#	define w_segs_map_N4 N4
#else
#define w_segs_map_N4 7
	// COMPILER INTERNAL ERROR // static int N4;	// number of rows
#endif

	void init_years_now();
	int year0;
	time_t year_start[w_segs_map_N4+1];
	int find_row_for_time( time_t t1 );
	flt80 xfraction1( int row, time_t t1 );
	int xpos_for_row_time( int row, time_t t1 );

	time_t find_time_for_xy( int x, int y );

	void calc_layout_1(); // when font is available
	int h_line;
	int h_gap_above;
	int h_gap_below;
	int h_baseline;
	int h_row;

	int w_labels;
	int w_line;
	int w_gap;

	void calc_layout_2(); // when width() is available
	int w_segs;

virtual // widget0
	void event_size_request( GtkRequisition *requisition );

	w_segs_map( sar_top_app * _top );
	void get_map( dset_api * dset_src, mem_line_spec * spec );

	time_t highlight_t0;
	time_t highlight_t2;
	void highlight_set(time_t t0, time_t t2 );
	void highlight_draw();
	void highlight_1(time_t t0, time_t t2 );

	virtual void pixmap_reallocated();
	GdkGC * get_gc_white();
	GdkGC * get_gc_green();
	GdkGC * get_gc_red();
	void gc_init();
	GdkFont * get_font1();
	void paint();
	void paint_frame();
	void paint_segments();

};

#endif
