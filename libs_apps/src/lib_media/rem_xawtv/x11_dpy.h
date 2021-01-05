#pragma interface "x11_dpy.h"
#ifndef x11_dpy_H
#define x11_dpy_H

#ifdef WIN32
	typedef int Display;
	typedef int Atom;
	typedef int Window;
#else

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xmu/WinUtil.h>    /* for XmuClientWindow() */
#endif

#include "ints.h"
#include "fd_selectable.h"

class x11_dpy;

/*!
	every X11 connection has an FD to poll() or select()
*/
class fd_x11 : public fd_selectable_base
{
	x11_dpy * dpy;
 public:
	~fd_x11();
	//! create a selectable fd over Dispay 
	fd_x11( x11_dpy * _dpy );
	//! blank CTOR and late_init
	fd_x11();
	void late_init( x11_dpy * _dpy );
	//! tell x11_dpy that it is readable
	virtual bool select_says_read();
};


/*!
	x11_dpy is an X11 display, as used by rem_xawtv to find its window.
*/
struct x11_dpy : public GRP_lib_media
{
	// was dpy borrowed or was it opened
	bool dpy_opened;
	// the X11 display connection (shared or new)
	Display * dpy;

	// the select()able fd
	// its not obj_hold<> so .. call static ?
	obj_hold<fd_x11> fd_holder;
	virtual bool select_says_read();

	virtual ~x11_dpy();
	x11_dpy(
		char * dpyname = NULL,
		Display * _dpy = NULL
	);
	Atom mk_atom( const char * name );
	void XFlush();//  { ::XFlush( dpy ); }
	void flush();
	Window find_window_with_atom( Atom atom_station );
	bool window_has_atom( Window w, Atom atom_station );

	bool lookup_Key_by_name(
		const char * keyname, 
		int & keysym,
		int & keycode 
	);

	bool lookup_modifier_mask_by_name( const char * _mod, int & var );

	bool Grab_Key_ROOT (
		const char * str_key, 
		const char * str_mod
	);

	bool Grab_Key_ROOT_ITERS (
		const char * str_key, 
		const char * str_mod
	);

	bool Grab_Key_ROOT (
		int keycode, 
		int modifiers
	);

	bool Grab_Key (
		Window grab_window,
		int keycode, 
		int modifiers
	);

	void test1();

} ;

#endif
