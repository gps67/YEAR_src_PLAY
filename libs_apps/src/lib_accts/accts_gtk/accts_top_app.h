#ifndef ACCTS_TOP_APP_h
#define ACCTS_TOP_APP_h

// rename this file top_app_ACCTS

//#include "win_top.h"
class w_win_top;
#include "obj_hold.h"
#include "w_btn_base.h"
#include "w_vbox.h"
//#include "clist.h"
#include "ints.h"
#include "dset.h"
#include "str_list.h"
#include "w_note_book_one.h"
#include "w_file_sel.h"

#include "w_graph1.h"

#include "TOPAPP_Task_Window_Major.h"
using namespace APPS;


class w_menu_bar;

namespace ACCTS {

	class c_tree_tbl_two;
	class accts_top_app;;

//! accts_top_app is the main class top-window
/*!
	A TopLevel window, with a menu and a notebook.
	For various resons, some signals from the pages
	get routed through the top. Specifically a
	note_book_one is generic, not app-aware.
*/
class accts_top_app : public TOPAPP_Task_Window_Major
{
 public:
	//! the tables
	data_set		dset;

	//! the main notebook widget
	obj_hold<w_note_book_one> book;

	//! THE TEST GRAPH
	obj_hold<w_graph1>	graph1;

	//! Destructor
	virtual ~accts_top_app();

	//! open a window on file
	accts_top_app( APPS::TOPAPP_Holder_gtk * holder, str0 first_file );

	//! the book widget needs to be rebuilt
	void rebuild_book();

	//! open a new file (drop old)
	bool file_open( str0 filename );

	//! save the data
	bool file_save( str0 filename, bool compressed );

	//! rebuild a page (TABLE)
	void replace_page(
		int page_number,
		table * tbl,
		int sort_col,
		row_inst * sel_row
	);

	//! select a different row
	void tree_select_row( c_tree_tbl_two * tree, row_inst * row );

// VIRTUAL
	void help_about();
};

}; // NAMESPACE
#endif 
