
#include "x11_dpy.h"
#include "e_print.h"
#include <X11/keysym.h>
#include "dgb_fail.h"
#include "fd_selectable.h"
#include "gtk_text_top_app.h"

#include "gdb_invoke.h"

/*!
	constructor - must open a valid display, or be given one
*/
x11_dpy::x11_dpy(
	char * dpyname,
	Display * _dpy
)
// : fd_holder( this )
{
// 	gdb_invoke(true);
 	gdb_break_point();
	dpy_opened = false;
	dpy = _dpy;

	if(!dpy) {
		dpy = XOpenDisplay(dpyname);
		dpy_opened = true;
	}
	if (! dpy ) {
		e_print("can't open display %s\n", dpyname?dpyname:"");
		exit(1);
	}
	XFlush();
	// fd is now available - must tell fd_holder if it ever changes!
	//fd_holder = new fd_x11( this );
	fd_holder = new fd_x11();
	fd_holder->debug_name_set( "x11_dpy_FD" );
}

/*!
	destructor - closes this link not GTK's link
*/
x11_dpy::~x11_dpy()
{
	if( dpy && dpy_opened )
		XCloseDisplay( dpy);
}

/*!
	used to resolve the X11 atom XAWTV_STATION
*/
Atom x11_dpy::mk_atom( const char * name )
{
	return XInternAtom( dpy, name, False);
}

/*!
	XFlush() ensures all queued data is sent
*/
void x11_dpy::XFlush()
{
	::XFlush( dpy );
}

// someone references this
// something about not knowing XFlush is enoughm or is X11
void x11_dpy::flush(){
	::XFlush( dpy );
	// XFlush();
}

/*!
	tells if window has the XAWTV_STATION property
*/
bool x11_dpy::window_has_atom(
	Window w,
	Atom atom_station
)
{
	Atom            type;
	int             format;
	unsigned long   nitems, bytesafter;
	unsigned char  *args = NULL;

	// search for atom_station in w
	XGetWindowProperty(
		dpy,
		w,
		atom_station,	// _XAWTV_STATION
		0,			// long_offset
		(65536 / sizeof(long)),	// long_length
		False,			// delete
		XA_STRING,		// req_type
		&type,			// (out) actual_type
		&format,		// (out) actual_format
		&nitems,		// (out) 
		&bytesafter,
		&args
	);
	// if args is NULL, atom_station wasnt found on w
	if (!args)
		return false;
/*
	if (debug) {
		printf("query 0x%08lx: ",w);
		for ( int i = 0; i < nitems; i += strlen((char *)args + i) + 1)
			printf("%s ", args + i);
		printf("\n");
	}
*/
	XFree(args);
	return true;
}

/*!
	Used to find a window that defines XAWTV_STATION, else 0
*/
Window
x11_dpy::find_window_with_atom( Atom atom_station)
{
	int             n;
	Window          root2, parent, *kids;
	unsigned int    nkids;
	Window          result = 0;
	Window          root;

	/*
		get a list of toplevel kids of the root desktop
	*/
	root = RootWindowOfScreen(DefaultScreenOfDisplay(dpy));
	if (!XQueryTree(dpy, root, &root2, &parent, &kids, &nkids)) {
		e_print( "XQueryTree failed on display %s\n",
			DisplayString(dpy)
		);
		return 0;
	}

	// this is unlikely
	if (!(kids && nkids)) {
		e_print( "root window has no children on display %s\n",
			DisplayString(dpy));
		return 0;
	}

	// search thrugh the list - multiple finds ?
	for (n = nkids - 1; n >= 0; n--) {
		/*
			Each top level window has been wrapped by the WM
			(given borders, title bar and new parent frame)

			XmuClientWindow returns the window which has a WM_STATE property

			(This is better than searching all sub-branches + leaves)
		*/
		Window w = XmuClientWindow( dpy, kids[n] );

		if(window_has_atom( w, atom_station ) )
		{
			// found it, keep looking for another or break
			result = w;
			break;
		} else {
			 continue;
		}
	}
	// added because man XQueryTree says so
	XFree(kids);
	return result;
}
