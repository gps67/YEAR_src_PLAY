#ifndef TOPAPP_Task_Window_Minor_H
#define TOPAPP_Task_Window_Minor_H

#include "TOPAPP_Holder_gtk.h"
#include "TOPAPP_Task_gtk.h"
class w_menu_bar;

class w_file_sel;
class w_vbox;
class w_hbox;
class str2;
// struct GtkWidget;
#include "w_widget0.h"

#include "APPS_stubs.h"
namespace APPS {
/*!
	This Window Minor is a Task with a window, eg a Document

	This Window has a common-style layout (menu bar, status line, ... )

	This Task can be held by a Task holder that doesnt know about windows
	(just as well becase this is not a window, it holds one).

	this->main_window is the actual TOP_LEVEL Window
	this->lots_of_other_stuff

	This Window/Task can be in a SESSION-STATE,
	and all the GUI-BTN's call back to this to do their job

	It retains a single file-selector, holds the menubar, etc.
	It provides virtual functions for the app to call,
	and it defines those fiddly C_ callbacks into virtuals
	EG C_menu_file_open() (because this Tasks Window has a Menu)

	Minor - TOPLEVEL, closing this probably doesnt close the app

	Major - TOPLEVEL, closing this MAY exit the app

	TODO: move this to Task_Window_Layout (_base of _Minor and _Major)
	TODO: other layouts are possible

	REMEMBER: this is a Task, (sess) more than a window which it holds

	The TOPAPP_Task_List_Holder has responsibility to know that
	we are not opening the same document twice, or displaying
	e_print_text twice (which is a nice way of keeping it alive
	but hidden sometimes)

*/
class TOPAPP_Task_Window_Minor : public TOPAPP_Task_gtk
{
 public:
	virtual ~TOPAPP_Task_Window_Minor();
	TOPAPP_Task_Window_Minor(
		TOPAPP_Holder * _m_a_h,
		const char * w_title,
		int w_width,
		int w_height,
		const char * _filename = NULL,
		bool _compress = false
	);

	//! the main window
	obj_hold<w_win_top>	main_window;

	//! the main_area of the main window // ir app_area ?
	obj_hold<w_vbox>	main_area;

	//! the empty or full menu bar
	obj_hold<w_menu_bar>	menu_bar;

	//! part of the main window
	obj_hold<w_vbox>	app_area;

	//! h_box for now
	obj_hold<w_hbox>	status_bar;

	//! retain the file selector dialog // or make this LIBR_global only 1
	obj_hold<w_file_sel>	filesel;

	//! the selected filename
	obj_hold<str2>		filename;
	//! is the file to be compressed (no detect on load)
	bool			compress;

	bool has_btn_quit;
	bool has_btn_hide;

	/*
		C functions for the menu callbacks, with cast pointers
	*/
static
void C_file_sel_open( GtkWidget * abtn, TOPAPP_Task_Window_Minor * app );
static
void C_file_sel_save( GtkWidget * abtn, TOPAPP_Task_Window_Minor * app );
static
void C_menu_file_save_as( GtkWidget * abtn, TOPAPP_Task_Window_Minor * app );
static
void C_menu_file_open( GtkWidget * abtn, TOPAPP_Task_Window_Minor * app );
static
void C_menu_file_save( GtkWidget * abtn, TOPAPP_Task_Window_Minor * app );
static
void C_menu_help_about( GtkWidget * abtn, TOPAPP_Task_Window_Minor * app );
static
void C_enable_gdb_break_point( GtkWidget * abtn, TOPAPP_Task_Window_Minor * app );
static
void C_gdb_invoke( GtkWidget * abtn, TOPAPP_Task_Window_Minor * app );
static
void C_gtk_main_quit( GtkWidget * abtn, TOPAPP_Task_Window_Minor * app );
static
void C_e_print_show( GtkWidget * btn, TOPAPP_Task_Window_Minor * self );
static
void C_e_print_onoff_toggle( GtkWidget * btn, TOPAPP_Task_Window_Minor * self );
static
void C_btn_noop( GtkWidget * btn, obj_ref * self );

// XX	void use_standard_layout_one( bool hide_not_quit );
// you cannot call virtuals from the  CTOR
// you can, but not then override them, OK
// so move 'use_standard_layout - to OUTSIDE of CTOR!

	virtual void use_standard_layout();
	virtual void add_hide_button();
	virtual void add_quit_button();
	virtual bool fill_menu_bar();
	virtual void add_status_line(); 
	

	virtual bool file_open( str0 filename );
	virtual bool file_save( str0 filename, bool compression );
	virtual void help_about();

	void hide();
};
}; // NS


#endif
