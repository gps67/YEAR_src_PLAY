#ifndef gtk_text_top_app_H
#define gtk_text_top_app_H

#include "g_text_buffer.h"

class c_tree_tbl_two;
class gtk_text_top_app;;

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

class w_menu_bar;

// #include "w_win_top.h"
/*
	This message comes from having stub {{class w_win_top;}},
	which is not enough to initialise {{ obj_hold<w_win_top> main_window; }}
	in the constructor.


../src/lib_base/obj/obj_hold.h: In member function `void 
   obj_hold<BASE>::set(BASE*) [with BASE = w_win_top]':
../src/lib_base/obj/obj_hold.h:78:   instantiated from `obj_hold<BASE>::~obj_hold() [with BASE = w_win_top]'
../src/gtk_text/gtk_text/gtk_text_top_app.h:44:   instantiated from here
../src/lib_base/obj/obj_hold.h:30: error: `ref_incr' undeclared (first use this 
   function)
../src/lib_base/obj/obj_hold.h:30: error: (Each undeclared identifier is 
   reported only once for each function it appears in.)
../src/lib_base/obj/obj_hold.h:40: error: `ref_decr' undeclared (first use this 
   function)
*/
// #include "w_dialog_msg.h"



class gtk_text_top_app : public TOPAPP_Task_Window_Major
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
	gtk_text_top_app( TOPAPP_Holder_gtk * holder, const char * first_file );

	~gtk_text_top_app();
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
#endif
