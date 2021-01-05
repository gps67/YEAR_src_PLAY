#include "accts_top_app.h"
// #include "top.h"
#include "w_menu1.h"
#include "c_tree_tbl_two.h"
#include "w_vpaned.h"
#include "w_table.h"
#include "w_entry.h"
#include "nb_page_tbl.h"
// #include "note_book.h"
#include "tbl_col_spec.h"
#include "w_dialog_msg.h"

#include "run_report.h"

namespace ACCTS {

void accts_top_app::help_about()
{
	w_dialog::msg_failed(
		main_window,
		"Free Software",
		"\n"
		"accts_gtk\n"
		"\n"
		"This is Free Software,\n"
		"Licensed under the LGPL\n"
		"http://www.information-cascade.co.uk\n"
		"http://freshmeat.net\n"
		"http://www.gnu.org\n"
	);
}

void C_menu_report1( GtkWidget * abtn, accts_top_app * app )
{
	run_report_1( & app->dset );
}

accts_top_app::~accts_top_app() {}

bool accts_top_app::file_save( str0 _filename, bool compressed )
{
	/*
		_filename is this->filename.ptr->str

		so overwriting it, deletes old str, THEN USE THAT!!! 
	*/
	obj_hold<str2> loopy = filename;
	filename = new str2( _filename );
	compress = compressed;

	if(dset.file_save( _filename, compressed )) // try again ?
	{
		// file_open( filename ); // test the file!!
	}
	else
		C_menu_file_open( NULL, this );;
	return true;
}

bool accts_top_app::file_open( str0 first_file )
{
///?///
	if(!filename) main_window->show();

	compress = TRUE; // LURK - want to save as found

	// filename = new str2( first_file );

	book->remove_all_pages();
	dset.reset();

	if(dset.load_file( first_file )) // try again ?
	{
		if(0) {
			str0 filename2 = "Grid_TempOut.gz";
			file_save( filename2, TRUE );
		}
		filename = new str2( first_file  );
		rebuild_book();
	} else
		C_menu_file_open( NULL, this );
	return true;
}

void accts_top_app::tree_select_row( c_tree_tbl_two * tree, row_inst * row )
{
	int LURK_this_function_is_unused;
	e_print("-------UNUSED----------TOP_APP:: page->rebuild_table( row );\n");
}

void accts_top_app::replace_page(
	int page_number,
	table * tbl,
	int sort_col,
	row_inst * sel_row
)
{
	bool must_set_tree_col = false;
	if(!tbl) return;
	if( sort_col == -1 )
	{
		sort_col = 0;
		must_set_tree_col = true;
	}
	nb_page_tbl * page = new nb_page_tbl(
		this,
		tbl,
		sort_col,
		sel_row
	);
	if(!page) return_VOID_dgb_fail("NULL page");

	if( page_number < 0 ) {
		book->page_new( tbl->get_name(), page );                                                                      
	} else {
		book->page_replace( page_number, page );
	}
	// must be added before this works
	if( must_set_tree_col )
	{
		if( tbl->col_spec_tree )
		{
			page->change_col_sort( * tbl->col_spec_tree->name );
		}
	}
	// leaves last notebook page selected - OK
	// very scattered, bitty design - not so OK

	/*
		This is called in the page = constructor(),
		but it doesnt work there - because its not packed?
		Calling it here doesnt work properly either, as the
		selected line is at the top, but it is visible.
		Maybe even now the panel height is 0, and 0.4 of the way
		down is centered, with the mid point of the line at
		the 0.4 point of the panel.
	*/
	if( sel_row) page->row_selected( sel_row );
}

void fixup_some( data_set * dset )
{
	static obj_hold<str2> s_CAT  = new_str2_static("CAT");
	static obj_hold<str2> s_path = new_str2_static("path");

	fprintf(stderr,"fixup_some - START\n");

	/*
		ensure that each table has at least one row

		This could be moved to the page so that delete
		doesnt leave no way of adding
	*/
	int n = dset->tbl_list->tbl_list.N();
	for( int i=0; i<n; i++ )
	{
		table * tbl = dset->tbl_list->tbl_list[i];
		if(!tbl->rows_list.N())
			tbl->find_row_add_fake("PLACE-HOLDER");
	}

//	return;

	/*
		This is a wierd and naughty place to put some test calls
	*/

	run_report_1( dset );
	run_report_2( dset );

	return;

	table * tbl_cat = dset->get_table( s_CAT );
	if(!tbl_cat) return;
	fprintf(stderr,"fixup_some - tbl_cat\n");
	col_spec * path_spec = tbl_cat->get_col_spec( s_path );
	if(!path_spec) return;
	tbl_cat->group_spec = path_spec;

	fprintf(stderr,"fixup_some - OK\n");
}

void accts_top_app::rebuild_book()
{
/*
 * 	First a hook for some other schema cleanup - after file load
*/
	fixup_some( & dset );
	// book-> clear
	int nt = dset.tbl_list->tbl_list.N();
	for( int i=0; i<nt; i++ )
	{
		table * tbl = dset.tbl_list->tbl_list[i]; 
		// dont set the initially tree-indexed column
		replace_page( -1, tbl, -1, NULL );
	}
}

accts_top_app::accts_top_app( TOPAPP_Holder_gtk * holder, str0 first_file )
: TOPAPP_Task_Window_Major( holder, "Accts", 700, 550, first_file, false )
{

	// main_window->connect_event_destroyed();

/*
	obj_hold<w_menu> m_file = new w_menu( "File" );
	menubar->append( m_file );
	// similar but condensed
	obj_hold<w_menu> m_edit    = menubar->mk_menu( "Edit" );
	obj_hold<w_menu> m_reports = menubar->mk_menu( "Reports" );
	obj_hold<w_menu> m_help    = menubar->mk_menu_right( "Help" );

	obj_hold<w_menu_item> item = new w_menu_item(
		"Open ._.",
		(SignalFunc) C_menu_file_open,
		(obj_ref *) this
	);
	m_file->append( item );

	m_file->mk_item(
		"Save",
		(SignalFunc) C_menu_file_save,
		(obj_ref *) this
	);

	m_file->mk_item(
		"Save As ...",
		(SignalFunc) C_menu_file_save_as,
		(obj_ref *) this
	);

	m_file->mk_item(
		"Exit",
		(SignalFunc) gtk_main_quit,
		(obj_ref *) NULL
	);

	m_reports->mk_item(
		"Report1",
		(SignalFunc) C_menu_report1,
		(obj_ref *) this
	);

	m_help->mk_item(
		"About",
		(SignalFunc) C_menu_help_about,
		(obj_ref *) this
	);

	obj_hold<w_menu_item> gdb_item; gdb_item = new w_menu_item(
		(char *) "Enable gdb_break_point",
		(SignalFunc) C_enable_gdb_break_point,
		(obj_ref *) this
	);
	m_file->append( gdb_item );

//	obj_hold<menu_item> mi_copy = m_edit->mk_item( "copy" );
//	obj_hold<menu_item> mi_paste = m_edit->mk_item( "Paste" );
*/

if(0)
{
	graph1 = new w_graph1();
	app_area->pack_asis( graph1 );
}

	book = new w_note_book_one(); // needs a clear()
	app_area->pack_big( book );

// GPS	// 2011-10- // I forgot about this ... its needed in all main().cxx
//	// use_standard_layout() packs the items into the main window
//	// it must be called, to allow different versions, style_B
	use_standard_layout();
	main_window->show();	// main_window has no pack to call show()

	if( first_file )
	{
		filename = new str2( first_file );
		main_window->show();	// main_window has no pack to call show()
		file_open( first_file );
// main_loop not yet started! // gtk_main_quit();
	} else {
		filename = new str2( "" );
		main_window->show();	// main_window has no pack to call show()
		C_menu_file_open( NULL, this );
		// C_menu_file_save_as( NULL, this ); // this KILLS files!
	}

	if(0)  C_menu_help_about( NULL, this );

	// return(0);
}

}; // NAMESPACE
