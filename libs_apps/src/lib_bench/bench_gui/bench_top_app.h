#ifndef bench_top_app_H
#define bench_top_app_H

#include "g_text_buffer.h"

class c_tree_tbl_two;

//#include "win_top.h"
class w_win_top;
#include "obj_ref.h"
#include "obj_hold.h"
#include "w_btn_base.h"
#include "w_vbox.h"
//#include "clist.h"
#include "ints.h"
#include "dset.h"
#include "str_list.h"
//#include "note_book_one.h"
#include "w_file_sel.h"

#include "w_graph1.h"
//#include "test_edit.h"


#include "TOPAPP_Task_Window_Major.h"
using namespace APPS;
// using namespace BENCH; // instead CLASS_NAME  bench _top_app // JOIN //
class TOPAPP_Holder;
class w_menu_bar;

 namespace BENCH {
 };

class bench_top_app : public TOPAPP_Task_Window_Major
{
 public:

 /*
	THE DATA OBJECT
 */
	obj_hold<g_text_buffer> test_edit1; // not the view
	// the view is packed but not held in a var
	//! the tables
//	data_set		dset;

	//! the row of columns of buttons (its an hbox)
	//! THE TEST GRAPH
	obj_hold<w_graph1>	graph1;

	//! open a window on file
	bench_top_app( TOPAPP_Holder_gtk * holder, const char * first_file );

	~bench_top_app();
	//! the book widget needs to be rebuilt

//	void rebuild_book();
	//! open a new file (drop old)
	bool file_open( str2 * filename );
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

	w_menu_bar * xx_make_menu_bar(); // used in ctr
};
// }; // NS
#endif
