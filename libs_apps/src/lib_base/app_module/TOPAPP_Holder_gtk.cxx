#include "TOPAPP_Holder_gtk.h"
#include "dgb.h"
#include "dgb_fail.h"
#include <gtk/gtk.h>
#include "w_win_top.h"
#include "d_colour.h"
using namespace APPS;

#define is_non_gui_FALSE false

TOPAPP_Holder_gtk::
TOPAPP_Holder_gtk( int * p_argc, char *** p_argv )
: TOPAPP_Holder_glib( p_argc, p_argv, is_non_gui_FALSE )
{
	STEP("HERE");
	call_gtk_or_glib_init( p_argc, p_argv ); // OR glib_init
}

bool TOPAPP_Holder_gtk::
call_gtk_or_glib_init(int * p_argc, char *** p_argv)
{
	STEP("HERE");
	if( g_not_gtk ) {
		// a gtk application can still choose to be non-gui (pdf gen)
		return call_glib_init(p_argc,p_argv);
	} else {
		return call_gtk_init(p_argc,p_argv);
	}
}


bool TOPAPP_Holder_gtk::
call_gtk_init(int * p_argc, char *** p_argv)
{
	STEP("HERE");
	if( g_not_gtk ) {
		WARN("GTK is linked in, GTK class, set to GLIB, still here!");
	}
	/*VOID*/ gtk_init(p_argc,p_argv);
	Colour_Map = new d_colour_map();
	return true;
}


/*!
	add a sub-window to the list of toplevels
*/
bool TOPAPP_Holder_gtk::window_attach( w_win_top * win )
{
	win->debug_track_pointer();
	wait_more_for( win );
	return true;
	return FALSE_dgb_fail("WRITEME");
}

/*!
	remove top-level window from keeplist
*/
bool TOPAPP_Holder_gtk::window_detach( w_win_top * win )
{
	win->debug_track_pointer();
	wait_less_for( win );
	return true;
	return FALSE_dgb_fail("GLIB not GTK");
}

bool TOPAPP_Holder_gtk::Call_gtk_main()
{
	gtk_main();
	return true;
}
