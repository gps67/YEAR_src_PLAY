#include "X_ALL.h"
// ALL // #include <stdlib.h>
// ALL // #include <string.h>
using namespace WAX;

void X_Window::set_name( const char * _name )
{
	if( name ) free( (void *)name );
	name = strdup( _name );
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
	: parent( _parent )
	, display( _parent->disp->display )
	, window( _window )
	, disp( _parent->disp )
	, name(0)
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
	: parent( _parent )
	, display( _disp->display )
	, window( _window )
	, disp( _disp )
	, name(0)
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
	A_Rectangle xywh,
	int borderwidth
)
: parent(NULL)
, disp( & disp_ )
, display( disp_.display )
, window(0)
, name(0)
{
	set_name( _name );
	ulong col_border = BlackPixel( display, 0 );
	ulong col_background = BlackPixel( display, 0 );
	window = ::XCreateSimpleWindow(
		display,
		RootWindow( display, 0),
		xywh.x, xywh.y, xywh.width, xywh.height,
		borderwidth,
		col_border,
		col_background
	);
	disp->add( this );
	printf( "window = %ld\n", window );
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
: parent( parent_ )
, disp( parent_->disp )
, display( parent_->display )
, window(0)
, name(0)
{
	set_name( _name );
	ulong col_border = BlackPixel( display, 0 );
	ulong col_background = BlackPixel( display, 0 );
	window = ::XCreateSimpleWindow(
		display,
		parent->window,
		xywh.x, xywh.y, xywh.width, xywh.height,
		borderwidth,
		col_border,
		col_background
	);

	disp->add( this );
	map();

	printf( "window = %ld\n", window );
}


struct X_Window_Root : public X_Window
{
	virtual void event_expose( A_Rectangle & xywh )
	{
		// never gets called!
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
		printf("# X_Window_Root( disp, '%s' ) -- window(%ld)\n", name, window );
	}
};

/*!
	create an internal object for the already existing root window
 */
X_Window * X_Window::register_root(
	X_Display & disp_,
	const char * name
)
{
	X_Window * w = new X_Window_Root( disp_, name );
	return w;
}

