#ifndef  rem_xawtv_H
#define  rem_xawtv_H

#include "x11_dpy.h"
#include "rem_xawtv_cmd.h"
#ifdef WIN32
#include "gdk/gdkwin32.h" // GdkNativeWindow
typedef long long XErrorEvent;
#else
#include "gdk/gdkx.h" // GdkNativeWindow
#endif
// #include "misc.h"	// sleep_10
#include "sleep_ms.h"	// sleep_10
#include "e_print.h"

#ifdef WITH_GTK1
	typedef unsigned int GdkNativeWindow;
#endif

/*!
	rem_xawtv sends control commands to xawtv by sending ATOM values to X11

	It knows how to find a running XAWTV window. see xawtv_remote

	It caches its window_id and when that window is closed, it has to be
	asked to do (someaction) again.

	Some of these operations (wait_win_id) are time expensive.

	This needs to be turned inside out, (or a thread added),
	so that it never block/sleeps. That would require a hook
	into the select-loop (call_back_in_10( 2, function )).

	Alternatively, allow the sleep_10(2) function to do fd_selectable
	callbacks (which it already does?)
*/
class rem_xawtv : public x11_dpy
{
	Window win_id;
 public:
	~rem_xawtv();
	rem_xawtv(
		char * dpyname = NULL,
		Display * _dpy = NULL,
		Window _win_id = 0
	);

	void clear_win_id();
	bool have_win_id() { return win_id != 0 ; }

	bool find_win_id();
	GdkNativeWindow get_win_id();
	void send_cmd_line( int argc, char ** argv );
	void send_cmd_line( const char * buff );
	void send( const rem_xawtv_cmd & cmd );
	bool wait_and_quit();
	bool wait_win_id();
	bool recover( const char * line = NULL );
};
#endif
