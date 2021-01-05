#ifndef sar_top_app_H
#define sar_top_app_H

#include "sar_top_app_signals.h"
#include "w_segs_map.h"
// class w_segs_map; // 3 fns used - all should be linked (?vtl?)

class c_tree_tbl_two;
class sar_top_app;;

//#include "w_win_top.h"
class w_win_top;
#include "obj_hold.h"
#include "w_btn_base.h"
#include "w_label.h"
#include "w_vbox.h"
//#include "w_clist.h"
#include "ints.h"
#include "dset.h"
#include "str_list.h"
#include "sar_top_layout.h"
#include "w_file_sel.h"

#include "w_graph3.h"

#include "dset_api.h"
#include "tm_parts.h"

class w_menu_bar;
#include "TOPAPP_Holder.h"

#define W_LABELS w_label
// #define W_LABELS w_btn_base

#include "TOPAPP_Task_Window_Major.h"
using namespace APPS;

/*!
	sar_top_app is the main class top-window
*/
class sar_top_app : public TOPAPP_Task_Window_Major
{
 public:

	//! dset_api_pdb - loaded by file_open
	obj_hold<dset_api> dset;

	obj_hold<sar_top_layout> layout;

	// these need to be vars to be individually settable
	obj_hold<W_LABELS> curr_date;
	obj_hold<W_LABELS> curr_date2;
	obj_hold<W_LABELS> curr_date3;
	obj_hold<W_LABELS> curr_hhmm;
	obj_hold<W_LABELS> curr_yval;

	// set span to 21 days ... buttons held by vbox
	obj_hold<w_vbox> span_vals; // must keep

	obj_hold<w_segs_map> years_map;

	//! layout tells THIS what to plot
	void layout_selected_leaf( mem_line_spec * spec );
	void layout_selected_time( time_t t0 );
	void layout_selected_time_range( time_t t0, time_t t2 );
	void layout_selected_time_span_days( int ndays );
	void layout_selected_time_span_secs( int secs );
	void layout_selected_time_adj( int secs );

	// retain selected 
	mem_line_spec * selected_spec;
	time_t t_lo;
	time_t t_hi;

	//! THE GRAPH
	obj_hold<w_graph3>	graph_2;

	//! open a window on file
	sar_top_app( TOPAPP_Holder_gtk * holder, const char * first_file );
	~sar_top_app();
	//! the book widget needs to be rebuilt
//	void rebuild_book();
	//! open a new file (drop old)
	void file_open( str2 * filename );

	//! save the data
//	void file_save( str2 * filename, bool compressed );
	//! rebuild a page (TABLE)
//	void replace_page(
//		int page_number,
//		table * tbl,
//		int sort_col,
//		row_inst * sel_row
//	);
	//! select a different row
//	void tree_select_row( c_tree_tbl_two * tree, row_inst * row );

	bool fill_menu_bar(); // used in ctr part II

	//! k
	void cross_hairs_at( time_t x_time, flt80 y_val );

        void pre_fast_exit();

};

#endif
