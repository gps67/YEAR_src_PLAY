#include "X_ALL.h"
// ALL // #include <stdlib.h>
// ALL // #include <string.h>
using namespace WAX;

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

/*!
	child classes use this to create the object
*/
X_Window::
X_Window(
	X_Window * _parent,	// provides display ;-)
	Window _window,		// possibly 0 or XXXX
	const char * _name	// usually set
)
: X_Drawable_Surface( _parent->disp->display, _window )
, parent( _parent )
, disp( _parent->disp )
, name(0)
//	, xft_draw( *this )
{
	set_name( _name );
	disp->add( this );
}

/*!
	NULL_parent
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
}

/*!
	create a simple window, NB-TODO some unsigned work 
*/
X_Window::X_Window(
	const char * _name,
	X_Display & disp_,
	A_Rectangle xywh, // i16 // A_SCREEN_Rectangle // A_3D_TANLGE
	int borderwidth
)
: X_Drawable_Surface( disp_.display, 0 )
, parent(NULL)
, disp( & disp_ )
, name(0)
//, xft_draw() // requires following create()
{
	set_name( _name );
	ulong col_border = BlackPixel( display, 0 );
	ulong col_background = BlackPixel( display, 0 );
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
	drawable = ::XCreateSimpleWindow(
		display,
		parent->get_window(),
		xywh.x, xywh.y, xywh.width, xywh.height,
		borderwidth,
		col_border,
		col_background
	);
	xywh.get_WH( WH );
//	WH   = xywh.WH; // avoiding struct return // why ?
//	WH.w = xywh.width; // xywh is not built from WH but { width height }
//	WH.h = xywh.height; // maybe inline is a good alias

//	xft_draw.Xft_DrawCreate( *this );

	disp->add( this );
	map();

	printf( "window = %ld\n", get_window() );
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

/*!
	name gets displayed in the title bar, and wman lists

	https://tronche.com/gui/x/icccm
	https://tronche.com/gui/x/icccm/sec-4.html
	says WM_NAME also WM_ICON_NAME 
*/
void X_Window:: set_title( const char * name )
{
	::XStoreName( display, get_window(), name );
}



/*
*/
struct X_Window_Root : public X_Window
{
	virtual void event_expose( A_Rectangle & xywh )
	{
		INFO("UNUSED");
		// we provide an X_Window based class wrap
		// and it requires some virtual functions
		// they never get called!
	}

#define NULL_parent NULL
#define ZERO_window 0

	// LURK no check on DefaultRootWindow() - see syntax

	X_Window_Root(
		X_Display & disp_,
		const char * _name
	)
	: X_Window( NULL_parent, &disp_, DefaultRootWindow( disp_.display ), _name )
	{
		printf("# X_Window_Root( disp, '%s' ) -- window(%ld)\n", name, get_window() );
	}
};

/*!
	create an internal object for the already existing root window

	no functions are here for ROOT
	no static singleton for root (TODO)
	called once
 */
X_Window * X_Window:: register_root(
	X_Display & disp_,
	const char * name
)
{
	X_Window * w = new X_Window_Root( disp_, name );
	return w;
}
