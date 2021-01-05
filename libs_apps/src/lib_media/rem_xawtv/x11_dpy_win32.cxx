// THIS FILE HAS EVAPORATED TO EMPTY

#include "x11_dpy.h"
#include "e_print.h"
// #include <X11/keysym.h>
#include "dgb_fail.h"
#include "fd_selectable.h"
#include "gtk_text_top_app.h"

x11_dpy::x11_dpy(
	char * dpyname,
	Display * _dpy
)
{
}

x11_dpy::~x11_dpy()
{
}

Atom x11_dpy::mk_atom( const char * name ) {return -1;}
void x11_dpy::XFlush() {}
bool x11_dpy::window_has_atom(
	Window w,
	Atom atom_station
)
{
	return false;
}


Window
x11_dpy::find_window_with_atom( Atom atom_station)
{
	return 0;
}

