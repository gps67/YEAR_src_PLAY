#include "TOPAPP_Task_Window_Minor.h"
using namespace APPS;

#include "str2.h"
#include "w_dialog_msg.h"
#include "w_file_sel.h"
#include "w_vbox.h"
// #include "w_menu_bar.h"
#include "w_menu1.h"
#include "dgb_fail.h"

/*!
	filesel has picked a filename to open
*/
void TOPAPP_Task_Window_Minor::C_file_sel_open( GtkWidget * abtn, TOPAPP_Task_Window_Minor * app )
{
	abtn = NULL;
	str0 filename = app->filesel->get_filename(); 
	app->filesel->call_destroy();
	app->filesel = NULL;
	app->file_open( filename );
}

/*!
	filesel has picked a filename to save
*/
void TOPAPP_Task_Window_Minor::C_file_sel_save( GtkWidget * abtn, TOPAPP_Task_Window_Minor * app )
{
	e_print("file_sel_save\n");
	abtn = NULL;
	// lurk filesel cannot be NULL
	str0 filename = app->filesel->get_filename(); 
	bool compression = app->filesel->get_compression_option();
	// app->filesel->call_destroy();
	app->filesel = NULL;
	app->file_save( filename, compression );
}

/*!
	open filesel to pick a filename to open
*/
void TOPAPP_Task_Window_Minor::C_menu_file_open( GtkWidget * abtn, TOPAPP_Task_Window_Minor * app )
{
	// allow caller to pass a fake NULL abtn
	int LURK_TITLE_for_filesel;
	const char * title = "Open File";
	app->filesel = new w_file_sel( app->main_window, title, app->filename );
	app->filesel->connect_ok(
		(SignalFunc) C_file_sel_open,
		(obj_ref *) app
	);
}

/*!
	tell app to save its primary file
*/
void TOPAPP_Task_Window_Minor::C_menu_file_save( GtkWidget * abtn, TOPAPP_Task_Window_Minor * app )
{
	// gdb_invoke(1);
	/*
		BUGFIX: the obj_hold<str2> didnt cleanly cast into str with *
		(str0 random = * app->filename )
	*/
#warning "str2 * filename2 = app->filename; returns NULL"

	// THIS ALSO FAILED // str2 * filename2 = app->filename;
	str2 * filename2 = (str2*)app->filename.get_ptr();

/*
	there is a *recent* str0 s = (str0) s2;
	but that might need something to get a (str2*) from an obj_hold<str2>
*/
	if(!filename2)
		C_menu_file_save_as( abtn, app );
	else {
		app->file_save( * filename2, app->compress );
	}
}

/*!
	open filesel to pick a filename
*/
void TOPAPP_Task_Window_Minor::C_menu_file_save_as( GtkWidget * abtn, TOPAPP_Task_Window_Minor * app )
{
//	// allow caller to pass a fake NULL abtn
	const char * title = "Save As  CSV";
	app->filesel = new w_file_sel( app->main_window, title, app->filename );
	app->filesel->connect_ok(
		(SignalFunc) C_file_sel_save,
		(obj_ref *) app
	);
	int LURK_compression_option;
	app->filesel->add_compression_option();
}

/*!
*/
void TOPAPP_Task_Window_Minor::C_menu_help_about( GtkWidget * abtn, TOPAPP_Task_Window_Minor * app )
{
	app->help_about();
	if( app->main_window ) app->main_window -> save_png( "shot", "This the screen-dumps comment" );
}

/*!
*/
void TOPAPP_Task_Window_Minor::C_enable_gdb_break_point( GtkWidget * abtn, TOPAPP_Task_Window_Minor * app )
{
	gdb_off = FALSE;
}

/*!
*/
void TOPAPP_Task_Window_Minor::C_gdb_invoke( GtkWidget * abtn, TOPAPP_Task_Window_Minor * app )
{
	gdb_invoke( true );
}

/*!
*/
// called by temp button not File/Exit
void TOPAPP_Task_Window_Minor::C_gtk_main_quit( GtkWidget * abtn, TOPAPP_Task_Window_Minor * app )
{
	// if(!abtn) return;
	gtk_main_quit();
}

/*!
*/
void TOPAPP_Task_Window_Minor::C_e_print_show( GtkWidget * abtn, TOPAPP_Task_Window_Minor * app )
{
	// if(!abtn) return;
	e_print_show();
}

/*!
*/
void TOPAPP_Task_Window_Minor::C_e_print_onoff_toggle( GtkWidget * abtn, TOPAPP_Task_Window_Minor * app )
{
	// if(!abtn) return;
	e_print_switch_gui_onoff_toggle();
}

/*!
*/
void TOPAPP_Task_Window_Minor::C_btn_noop( GtkWidget * btn, obj_ref * self )
{
}

///////////////////////////////////////////////////////////////////////

// VIRTUAL
TOPAPP_Task_Window_Minor::~TOPAPP_Task_Window_Minor()
{
}

TOPAPP_Task_Window_Minor::TOPAPP_Task_Window_Minor(
	TOPAPP_Holder * _m_a_h,
	const char * w_title,
	int w_width,
	int w_height,
	const char * _filename,
	bool _compress
)
: TOPAPP_Task_gtk( _m_a_h )                                
{
/*
	static TOPAPP_Holder * FIRST_m_a_h = NULL; // not obj_ref though
	if(!FIRST_m_a_h) 
		FIRST_m_a_h = _m_a_h;
	if(!_m_a_h) 
		_m_a_h = FIRST_m_a_h;
*/
	if(!w_title) 
		w_title = "TOPAPP_Task_Window_Mxxxx-ANON!";
	debug_set_obj_name( w_title );
	debug_track_pointer();

#warning WANT to move this to a parameter, or an invisible button
// this is the difference between classes

	has_btn_hide = true;
	has_btn_hide = false;
	has_btn_quit = false;
	// all false now ... // e_print adds back

	main_window = new w_win_top(this);
  if( w_width && w_height )
	main_window->set_usize( w_width, w_height );
 	main_window->debug_set_obj_name(w_title);
	main_window->set_title( w_title );
// OK 	main_window->set_bg("purple");
	// main_window->connect_quit();
	main_window->connect_event_destroyed();

	compress = _compress;

	main_area = new w_vbox();
	menu_bar = new w_menu_bar(); 
	app_area = new w_vbox();
	status_bar = new w_hbox();

	main_area->debug_set_obj_name("WIN.main_area");
	menu_bar->debug_set_obj_name("WIN.menu_bar");
	app_area->debug_set_obj_name("WIN.app_area");
	status_bar->debug_set_obj_name("WIN.status_bar");

	filesel = NULL; // default anyway
	if( _filename )
	{
		file_open( _filename );
		filename = new str2( _filename);
	} else
		filename = new str2( "" ); // else save_as filesel throws NULL
}


// VIRTUAL
bool TOPAPP_Task_Window_Minor::file_open( str0 filename )
{
	return false;
}
// VIRTUAL
bool TOPAPP_Task_Window_Minor::file_save( str0 filename, bool compression )
{
	return false;
}

// VIRTUAL
void TOPAPP_Task_Window_Minor::help_about()
{
	w_dialog::msg_failed(
		main_window,
		"Free Software",
		"\n"
		"lib_apps component\n"
		"\n"
		"This is Free Software,\n"
		"Licensed under the LGPL, with some GPL extras \n"
		"http://www.information-cascade.co.uk\n"
		"http://freshmeat.net\n"
		"http://www.gnu.org\n"
	);
}

// VIRTUAL
bool TOPAPP_Task_Window_Minor::fill_menu_bar()
{
	// this default is used by the gtk_text_view function

	w_menu * m_file = menu_bar->mk_menu( "File" );
//	w_menu * m_edit = menu_bar->mk_menu( "Edit" );
	w_menu * m_devel = NULL;
//	bool with_devel = false;
	bool with_devel = true;
	if(with_devel) m_devel = menu_bar->mk_menu( "Devel" );
	w_menu * m_help = menu_bar->mk_menu_right( "Help" );

// gdb_invoke(true);
	STEP("the default menu items including Devel2");

	m_file->mk_item(
		// "Open ... TOPAPP_Task_Window_Minor::fill_menu_bar",
		"Open ...",
		// "Open ..." __FUNCTION__ " in " __FILE__,
		(SignalFunc) C_menu_file_open,
		(obj_ref *) this
	);

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

	if(with_devel) {
	m_devel->mk_item(
		(char *) "e_print log",
		(SignalFunc) C_e_print_show,
		(obj_ref *) this
	);

	m_devel->mk_item(
		(char *) "e_print on off",
		(SignalFunc) C_e_print_onoff_toggle,
		(obj_ref *) this
	);

	m_devel->mk_item(
		(char *) "Enable gdb_break_point",
		(SignalFunc) C_enable_gdb_break_point,
		(obj_ref *) this
	);

	m_devel->mk_item(
		(char *) "gdb_invoke()",
		(SignalFunc) C_gdb_invoke,
		(obj_ref *) this
	);
	}

//	obj_hold<w_menu_item> mi_copy; mi_copy = m_edit->mk_item( "copy" );
//	obj_hold<w_menu_item> mi_copy; mi_paste = m_edit->mk_item( "Paste" );

	return true;
}

void TOPAPP_Task_Window_Minor::use_standard_layout()
{
	/*
	 CTOR has set these to be non NULL
		main_window = w_win_top
		main_area = w_win_top
		menu_bar = w_menu_bar
		app_area = w_vbox();
	// status bar
	*/

	// the window was dangerously empty / NULL
	// but we need the full VTABLE for this to make sense
	// maybe

	/*
	main_window
		which is a w_win_top
		which is a w_window with added app*
		which is a w_container_1
		which is a w_container_
		which is a w_widget0
		it has ONE child - so 'pack' should be pack_add_solo(child)
		It holds main_area

	main_area
		which is a w_vbox (a vertical stack of varying size layers)
		specifically:

			menu_bar
			hide | quit | absent
			app_area
			status_line
	*/
	main_window->pack_solo( main_area ); // single thing

#if 0 // 2.18 plus // never used here - attempt to fix windowless cent2
	gtk_widget_set_has_window( app_area -> w, true );
#endif
 	main_area->set_bg("purple"); // vbox is not holding it properly!
 	app_area->set_bg("purple");
	gtk_container_set_border_width (GTK_CONTAINER (app_area->w), 4);

	// actually the order was trumped - putting menu bar first - OK
	// ALL must have a menu_bar!
	main_area->pack_asis( menu_bar );

	if( has_btn_hide && has_btn_quit ) { has_btn_hide = false; }
	if( has_btn_hide ) add_hide_button();
	if( has_btn_quit ) add_quit_button();

	// this is the MAIN app area where you can pack child things
	main_area->pack_big( app_area ); // another vbox - its what is expected

	add_status_line();


	// NOW fill up the menu items
	// BUT VTBL ??
	fill_menu_bar();	// PROBLEMS WITH VTABLE FROM BASECLAS CTOR
}


void C_hide( GtkWidget * abtn, TOPAPP_Task_Window_Minor * app )
{
	app->main_window->hide();
	// gtk_widget_hide( app->main_window->w );
}

/*
	ASIDE: w_btn_base( args ) different order from mk_item( args )
*/
void TOPAPP_Task_Window_Minor::hide()
{
	if( main_window ) main_window->hide();
}

void TOPAPP_Task_Window_Minor::add_hide_button()
{
        obj_hold<w_btn_base> btn_hide; btn_hide = new w_btn_base(
		(char *)"Hide",
		(obj_ref *) this,
		(SignalFunc) C_hide
	);
	btn_hide->set_align_mid();
	main_area->pack_asis( btn_hide );
	e_print("add_hide_button()\n");
}

/*!
	(MAYBE) The window should close, not the app exit!
	This should call a virtual function that releases
	the pointer to the window (or call destroyed?)
*/
void TOPAPP_Task_Window_Minor::add_quit_button()
{
// not/adding the QUIT button is surely the callers choice
// cutting it here is a bit naughty
// #ifdef QUITBTN
        obj_hold<w_btn_base> btn_exit; btn_exit = new w_btn_base(
		(char *)"Exit",
		NULL,
		(SignalFunc) C_gtk_main_quit
	);
	btn_exit->set_align_mid();
	if(0) {
		main_area->pack_asis( btn_exit );
	} else {
		main_area->pack_end( btn_exit );
	}
// #endif

}

void TOPAPP_Task_Window_Minor::add_status_line()
{
// #warning status line not implemented
}


