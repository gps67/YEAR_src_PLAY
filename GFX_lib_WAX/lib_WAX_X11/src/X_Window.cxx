#include "X_ALL.h"
// ALL // #include <stdlib.h>
// ALL // #include <string.h>
using namespace WAX;


/*
	this has to be in CXX because it uses details of X_Display
*/
X_Window_Root::
X_Window_Root(
	X_Display * _disp, // has root_window
	const char * _name
)
: X_Window(
	NULL_parent,
	_disp,
	0,
	_name
) {
	// X_Window_Root // singleton with Display singleton //
	if(_disp->root_window) {
		_disp->root_window = this;
	}
}



void X_Window::set_name( const char * _name )
{
	if( name ) free( (void *)name );
	name = strdup( _name );
}

X_Window::
~X_Window() // deregister
{
	disp->del( this );
}

bool
X_Window::
check_window() // true == OK
{
	if(!drawable) {
		WARN("ZERO window in '%s'", name );
		WARN("WANT_BACKTRACK '%s'", name ); // Q1 Q2 cident SUBLEX //
		DEBUG_print_stack() ; // 
		// dgb_api // dgb.h // SUBDIR src/lib_base/dgb/ lib_base subdir // dgb.h 

		return false;
	}
	return true;
}

/*!
	NULL_parent
	child classes use this to create the object
*/
X_Window::
X_Window(
	X_Window * _parent,	// possibly NULL
	X_Display * _disp,	// must have display
	Window _window,		// possibly 0 or XXXX
	const char * _name	// usually set
)
: X_Drawable_Surface( _disp->display, _window )
, parent( _parent )
, disp( _disp )
, name(0)
//	, xft_draw( *this )
{
	set_name( _name );
	disp->add( this );
	get_window(); // check
}

/*!
	create a simple window on the parent

	really - should create the local X_Window object
	without a display's window.
	Then do the tree-layout,
	then INSTANCIATE the window
*/
X_Window::X_Window(
	const char * _name,
	X_Window * parent_,
	A_Rectangle xywh,
	int borderwidth
)
: X_Drawable_Surface( parent_->disp->display, 0 )
, parent( parent_ )
, disp( parent_->disp )
, name(0)
//, xft_draw() // requires following create()
{
	set_name( _name ); // _dgb_
	ulong col_border = BlackPixel( display, 0 );
	ulong col_background = BlackPixel( display, 0 );
	INFO("Calling XCreateSimpleWindow()");
	Window _window =::XCreateSimpleWindow(
		display,
		parent->get_window(),
		xywh.x, xywh.y, xywh.width, xywh.height,
		borderwidth,
		col_border,
		col_background
	);
	if( _window )
		set_window( _window );
	else
		WARN("ZERO window=::XCreateSimpleWindow()"); // cant easily return in CTOR
	// WH = xywh.WH // GETTER uses RETVAR WH
	xywh.get_WH( WH );
//	WH   = xywh.WH; // avoiding struct return // why ?
//	WH.w = xywh.width; // xywh is not built from WH but { width height }
//	WH.h = xywh.height; // maybe inline is a good alias

//	xft_draw.Xft_DrawCreate( *this );

	disp->add( this );
	map();
	INFO( "window = %ld", get_window() );
	if(! X_WMProtocols_add_WM_DELETE_WINDOW() )
		FAIL_FAILED();
}

bool X_Window:: map()
{
 
	// was above everything else, but not the focussed window
 if(0)	::XMapRaised( display, get_window() ); // worked more
 else	::XMapWindow( display, get_window() );
	return true;
}

void X_Window:: XSelectInput_mask_one()
{
	long mask  = 0;
	mask |= ExposureMask ;
	mask |= KeyPressMask ;
	mask |= ButtonPressMask ;
	mask |= ButtonReleaseMask; // Pointer button up
//      mask |= ResizeRequest ; // NOT SURE WHERE THIS CAME FROM
	mask |= ResizeRedirectMask; // Redirect resize of this window
	mask |= StructureNotifyMask; // Destroy
//	mask |= -1;
	mask |= ((u32)-1) >> 8 ;
	XSelectInput( mask );
	// see also
	// Clients, usually those with multiple top-level windows,
	// whose server connection must survive the deletion of some
	// of their top-level windows,
	// should include the atom WM_DELETE_WINDOW
	// in the WM_PROTOCOLS property on each such window.
	// They will receive a ClientMessage event as described above
	// whose data[0] field is WM_DELETE_WINDOW.
}


//-----------------------------------------------------------------//


bool X_Window:: X_WMProtocols_add_WM_DELETE_WINDOW()
{
	// subscribe to be told when WM_ clicks on X button
	XSetWMProtocols(
		display,
		get_window(),
	      & X_Display_2:: atom_wm_delete_window,
		1
	);

	// return FAIL("reason");

	if(! INFO("LURK TODO") )
		WARN("INFO returned false");
	return true;
}

// has to go after decl of X_Window_Root

#if 0
/*!
	create an internal object for the already existing root window

	this is NOT an X11 thing,
	just keeping the singleton root w_id
	in a type, in a var,

	actually not even in a var,
	caller must do that

	WHY because we might have multiple DISPLAYS open ?
	WHY and need to set "name" at correct time

	EG move { X_Window_Root * x_window_root } to Display
	
	TODO functions are here for ROOT
	no static singleton for root (TODO)
	called once
 */
X_Window * X_Window:: register_root(
	X_Display & _disp,
	const char * name
)
{
	WARN("DONT CALL THIS");
	return disp.Root_Window;
	X_Window * w = new X_Window_Root( _disp, name );
	return w;
}
#endif

//-----------------------------------------------------------------//

/*!
	name gets displayed in the title bar, and wman lists

	https://tronche.com/gui/x/icccm
	https://tronche.com/gui/x/icccm/sec-4.html
	says WM_NAME also WM_ICON_NAME 
	
	NB moving to Top_Level
*/
void X_Window_Top_Level:: set_title( const char * name )
{
	::XStoreName( display, get_window(), name );
}


/*!
	create a simple window, NB-TODO some unsigned work 
*/
X_Window_Top_Level::X_Window_Top_Level(
	const char * _name,
	X_Display & _disp,
	A_Rectangle xywh, // i16 // A_SCREEN_Rectangle // A_3D_TANLGE
	int borderwidth
)
: X_Window_Frame( /*_parent*/ (X_Window *) NULL, & _disp, /*window*/ 0, _name )
{
	set_name( _name );
	ulong col_border = BlackPixel( display, 0 );
	ulong col_background = BlackPixel( display, 0 );
	// create a simple window, drawable // 
	drawable = ::XCreateSimpleWindow(
		display,
		RootWindow( display, 0),
		xywh.x, xywh.y, xywh.width, xywh.height,
		borderwidth,
		col_border,
		col_background
	);
//	xft_draw.Xft_DrawCreate( *this );
	disp->add( this );
	INFO("CTOR_ONE");
	INFO( "window = %ld", get_window() );
	if(! X_WMProtocols_add_WM_DELETE_WINDOW() ) {
		FAIL_FAILED();
	}
}

bool X_Window_Top_Level::
X_Raise_Window()
{
	// only Raises above siblings, not WM ...
	XRaiseWindow(display, get_window() );
	XFlush(display);

	INFO("but see XRestackWindows");
	return true;
	// MAYBE create as child of ROOT
	// https://stackoverflow.com/questions/4345224/x11-xlib-window-always-on-top
	// maybe ... _NET_WM_STATE_ABOVE
}

bool X_Window_Top_Level::
set_always_on_top() // _add // todo _remove + { bool toggle = off }
{
	Window win_root = DefaultRootWindow( display );
	XClientMessageEvent xclient;
	memset( &xclient, 0, sizeof (xclient) );
	//
	xclient.type = ClientMessage;
	xclient.window = get_window();
	xclient.message_type = X_Display_2:: atom_wm_state;
	xclient.format = 32;
	xclient.data.l[0] = X_Display_2:: atom_wm_state_add;	 // _remove
	xclient.data.l[1] = X_Display_2:: atom_wm_state_above; // _below
	xclient.data.l[2] = 0;
	xclient.data.l[3] = 0;
	xclient.data.l[4] = 0;
	XSendEvent(
		display,
		win_root,
		false,
		SubstructureRedirectMask | SubstructureNotifyMask,
		(XEvent *)&xclient
	);
	XFlush(display);
	PASS("CALLED TOP"); // was called not on top
	return True;
}

//-----------------------------------------------------------------//
