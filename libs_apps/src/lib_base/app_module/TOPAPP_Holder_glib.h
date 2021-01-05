#ifndef TOPAPP_Holder_glib_H
#define TOPAPP_Holder_glib_H

class w_win_top;

#include "obj_list.h"
// #include "w_widget0.h" // ie gtk
#include <glib.h> // ie gtk
#include "APPS_stubs.h" // to check this isnt different

#include "TOPAPP_Holder.h" // _Holder
#include "TOPAPP_Task_glib.h" // _Holder

// #define TOPAPP_Task char

namespace APPS {

//! a semi-static object, probably created in main, to to hold all top wins
/*!
	The TOPAPP_Holder has a single main app object, and
	a list of windows. When each window is closed, call window_detach().
	When the last window is closed, call_gtk_main_quit() is called,
	which sets m_a_h to NULL, and main_app to NULL,
	 which SHOULD refcount them away.
	(LURK - FUTURE DELAYS IN CLEARING NEED TO BE FLUSHED).
	Then gtk_main_quit() gets called.

	There might also be a fast_exit route that avoids C++ atexit.

	This sounds more confusing than it is. Currently it seems to
	be coming together as all the main() functions are being
	merged into one, and a few extra issues being thrown into the mix.
	IE GETOPT SETSIG PLC-coprocs may require a bit more nudging.

	TODO: non GUI App with adjusted gtk_main, then threads ...
*/
class TOPAPP_Holder_glib  : public TOPAPP_Holder
{ public:

	//! track this list of windows
	obj_list<w_win_top> win_list;

	//! track this list of ... sessions ... connections ... things
	obj_list<obj_ref0> OBJ_list;

	//! track this list of ... extra things that noone else holds
	obj_list<obj_ref0> OBJ_keep_list;

	//! only call quit once!
	bool gtk_closed;

	//! this is/not a non gui app, so use g_main_loop instead
	bool g_not_gtk;

	//! non gui main loop has some stuff
	GMainLoop *  loop;

	TOPAPP_Holder_glib( int * p_argc, char *** p_argv, bool is_non_gui = false );
	~TOPAPP_Holder_glib();
	void app_register( TOPAPP_Task * app );
	void app_destroyed();
	// void call_gtk_main_quit();
// VIRTUAL

	virtual bool call_gtk_or_glib_init(int * p_argc, char *** p_argv); // or your main loop owner ...
	 
 //	virtual bool call_gtk_init(int * p_argc, char *** p_argv); // or your main loop owner ...
	virtual bool call_glib_init(int * p_argc, char *** p_argv); // or your main loop owner ...

	virtual bool call_gtk_or_glib_quit(); // or your main loop owner ...
 //	virtual bool call_gtk_quit(); // or your main loop owner ...
	virtual bool call_glib_quit(); // or your main loop owner ...

	int call_gx_main_and_exit(); // or return int 
	virtual bool call_glib_main();
	virtual bool Call_gtk_main();

	virtual bool window_attach( w_win_top * window );
	virtual bool window_detach( w_win_top * window );


	void task_attach( TOPAPP_Task * task );
	void task_detach( TOPAPP_Task * task );
	void wait_more_for( obj_ref0 * obj );
	void wait_less_for( obj_ref0 * obj );
	void keep_that( obj_ref0 * obj );
	void drop_that( obj_ref0 * obj );
	void debug_show_lists();
//	bool call_gtk_init( int * p_argc, char *** p_argv);
	int call_gtk_main();
};
}; // NS

/*
void C_exit_win_requested( GtkWidget * w, TOPAPP_Holder * m_a_h )
{
}
*/

#include "TOPAPP_Task.h" // was declared here
#endif
