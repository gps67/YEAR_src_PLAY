#include "bench_top_app.h"

// #include "top.h"
#include "w_menu1.h"
#include "c_tree_tbl_two.h"
#include "w_vpaned.h"
#include "w_table.h"
#include "w_entry.h"
// #include "nb_page_tbl.h"
// #include "note_book.h"
// #include "tbl_col_spec.h"
#include "w_dialog_msg.h"
#include "w_file_sel.h"
#include "g_text_buffer.h"

using namespace BENCH;
// using namespace APPS;


bench_top_app::~bench_top_app()
{
	e_print("~bench_top_app(); // DTOR(app_top) //\n");
	// e_print_set( NULL );
	// actually _above_ might be _top_ // so relay bounce thru
	//
}

// void bench_top_app::file_save( str2 * _filename, bool compressed )
// {
//	filename = _filename;
//	compress = compressed;
//
//	if(dset.file_save( filename, compressed )) // try again ?
//	{
//		// file_open( filename ); // test the file!!
//	}
//	else
//		C_menu_file_open( NULL, this );;
// }

bool bench_top_app::file_open( str2 * first_file )
{
//	compress = TRUE; // LURK - want to save as found
	filename = first_file;
	obj_hold<str2> filename1; filename1 = first_file;
//
	test_edit1 -> clear();
	test_edit1 -> append_file( filename );
	return true;
}

w_menu_bar * bench_top_app::xx_make_menu_bar()
{
	w_menu_bar * menubar = new w_menu_bar(); // CALLER MUST hold or LEAK

	obj_hold<w_menu> m_file; m_file = new w_menu( "File" );
	menubar->append( m_file );
	// similar but condensed
	obj_hold<w_menu> m_edit; m_edit = menubar->mk_menu( "Edit" );
	obj_hold<w_menu> m_help; m_help = menubar->mk_menu_right( "Help" );

	obj_hold<w_menu_item> item; item = new w_menu_item(
		"Open ...",
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

//	obj_hold<w_menu_item> mi_copy = m_edit->mk_item( "copy" );
//	obj_hold<w_menu_item> mi_paste = m_edit->mk_item( "Paste" );

	return menubar;
}


bench_top_app::bench_top_app(
	TOPAPP_Holder_gtk * holder,
	const char * first_file
)
: TOPAPP_Task_Window_Major( // BASE_CLASS CTOR // MAJOR_window // one per APP
   holder,			// hold holds this app, api 2 way link
   "BENCH_was_GTK_TEXT",	// window title text 
   450, 450,			// W x H // WMAN places // REPEAT RETAINS
   first_file,			// window is of MIME file // or not
   false			// compress // relic //
 )
{                                  
//	compress = TRUE;

/*
	There is a lot of work to do, eg W_x_H XY WH XYWH
	profile or cache memory or visual recall 
	this is probably called from a file browser
	so is likely to be a Module or APPLET or LIBR for MIME type
	SESS for guest in SAFE mode

	EG every PANEL is a DOCUMENT MIME FILE DISPLAY or WRAP
	EG .txt .md .pdf .png .sh .ext
	EG DESKTOP is more then a PANEL EG VFS_BROWSER with several CSR VIEW
	EG BENCH is multiple panels VARS DIARY_LOGS multi-task-each-replay
	EG REPLAY EDIT_LIST attach AVR to GUI_SAID_LAYOUT_TEMPLATE_SESS_STO
	EG SPEC is CXX or LIBR_CLASS_FUNC_ext_C // ext is ARGV_DECODED _C
	EG Cant use VTBL and choose not to use STDLIB 
	EG need to to build without glib // ESP32 // hash HT now usable
*/

 // QUIT button
#ifdef QUITBTN
	has_btn_quit = true;
#endif

	// CTOR_part_II();
	use_standard_layout();

#if 0
////////////////////

 // VBOX = main_window contents
	obj_hold<w_vbox> vbox; vbox = w_vbox::new_vbox();
	main_window->pack( vbox );

 // MENUS
	obj_hold<w_menu_bar> menubar; menubar = make_menu_bar(); // LURK hold or LEAK
	vbox->pack_asis( menubar ); // could have a var pack_asis_here

 // BUTTONS
//	btns = new gtk_text_top_layout();
//	vbox->pack( btns );
////////////////////
#endif

 // GRAPH
	graph1 = new w_graph1();
	app_area->pack_asis( graph1 );

 // EDIT

	test_edit1 = new g_text_buffer();
 if(0) // shows that you can call this multimple times
	app_area->pack_big( test_edit1->mk_view_scrolled() );
	app_area->pack_big( test_edit1->mk_view_scrolled() );

	/*
		initialise the error reporting window
	*/
	// e_print_set( test_edit1 );

  // OPEN-FILE
	if( first_file )
	{
		filename = new str2( first_file );
		file_open( filename );
		main_window->show();	// main_window has no pack to call show()
	} else {
		filename = new str2( "" );
		main_window->show();	// main_window has no pack to call show
 // easy	// helpful open_file for blank init empty // annoying auto
 if(0)		C_menu_file_open( NULL, this );
		// C_menu_file_save_as( NULL, this ); // this KILLS files!
	}

 // Start program with the About BOX
	if(0)  help_about();
}
