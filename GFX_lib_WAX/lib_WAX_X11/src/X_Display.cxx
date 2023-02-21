#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Xatom.h>
#include <X11/keysym.h>

#include <string.h>

#include "X_STUBS.h"
#include "A_point_plus.h"
#include "X_Colours.h"
#include "X_Window.h"
#include "A_Map_W.h"
#include "X_Display.h"
#include "X_Draw.h"

#include "X11/XKBlib.h"

// #include "X_Display.h"
#include "e_print.h"

using namespace WAX;

/*
	each open window is a reason to stay
*/
int X_Display_One:: reasons_to_stay = 0;

Atom X_Display_One:: atom_wm_delete_window = 0;
Atom X_Display_One:: atom_wm_state = 0;
Atom X_Display_One:: atom_wm_state_above = 0;
Atom X_Display_One:: atom_wm_state_add = 0;
Atom X_Display_One:: atom_wm_state_remove = 0;

/*!
	create a connection to an X11 server

	There should only be one, but you might have a second, eg from tcl/tk
	so this does the one-time initialisation of some constants,
	and does not check that it is the only one
*/
X_Display_One :: 
X_Display_One( const char * display_name )
: cmap( NULL ) // what is the correct C++ of doing this ?
{
	display = ::XOpenDisplay( display_name );
	if(!display) {
		FAIL("NULL display from ::XOpenDisplay(%s)", display_name );
		return; // throw
	}
	cmap.late_init( display );
	// only_if_exists == false // NULL might match NULL
	atom_wm_delete_window = XInternAtom( display, "WM_DELETE_WINDOW", False);
	atom_wm_state = XInternAtom( display, "_NET_WM_STATE", False);
	atom_wm_state_above = XInternAtom( display, "_NET_WM_STATE_ABOVE", False);
	atom_wm_state_add = XInternAtom( display, "_NET_WM_STATE_ADD", False);
	atom_wm_state_remove = XInternAtom( display, "_NET_WM_STATE_REMOVE", False);
}

bool X_Display_One :: test_list_depths()
{
	int screen_number = 0; // screen 1 retval NULL
	int count_return = 0; // unchanged if NULL was returned
	int * ary_of_int;


	int n = ScreenCount( display );
	INFO("%d == ScreenCount(display)", n ); // always 1 ??

	INFO("ServerVendor(display) == %s", ServerVendor(display) );
	INFO("VendorRelease(display) == %d", VendorRelease(display) );

	Screen * scn = DefaultScreenOfDisplay(display);

	INFO("WidthOfScreen( Screen * screen ) == %d", WidthOfScreen( scn ) );
	INFO("HeightOfScreen( Screen * screen ) == %d", HeightOfScreen( scn ) );

	// xwininfo
	// 3286x1080+0+0	// depth 0 // pretty sure it is 1920 x 1080
	// 1366x768+0-0		// depth 32
	// probably virtual sum of 2 screens


	// 24 1 4 8 15 16 32 // no repettition default first then sorted
	ary_of_int = XListDepths(display, screen_number, &count_return);
	{
		return FAIL("RETVAL NULL");
	}
	for( int i = 0; i < count_return; i++ ) {
		INFO("[%d] depth %d", i, ary_of_int[i] );
	}
	XFree( ary_of_int );
	return true;
// screen 0
// # INFO # bool X_test_png::list_depths() # [0] depth 24
// # INFO # bool X_test_png::list_depths() # [1] depth 1
// # INFO # bool X_test_png::list_depths() # [2] depth 4
// # INFO # bool X_test_png::list_depths() # [3] depth 8
// # INFO # bool X_test_png::list_depths() # [4] depth 15
// # INFO # bool X_test_png::list_depths() # [5] depth 16
// # INFO # bool X_test_png::list_depths() # [6] depth 32
// screen 1
// .. RETVAL NULL ..
}

bool X_Display_One :: guess_screen_size( A_WH & WH )
{
	Screen * screen = DefaultScreenOfDisplay(display);
	WH.w = WidthOfScreen( screen );
	WH.h = HeightOfScreen( screen );
	INFO(" WH( %d, %d )", WH.w, WH.h );
	// I think "screen" is 2 monitors // widest doubled
	// or sum or both //
	// 1920 x 1080
	// 1366 x  768
	// 3286 x 1848
	// 3286 x 1080 // sub both sideways // widest vertically // arranged
	if( WH.w > 1920 ) WH.w =  1920;
	if( WH.h > 1080 ) WH.h = 1080;
	if( WH.w > 1920 ) WH.w /= 2;
	if( WH.h > 1080 ) WH.h /= 2;
	INFO(" WH( %d, %d )", WH.w, WH.h );
	return true;
}


void X_Display:: process_events_forever() // loop
{
	XEvent report;
	while (1)  {
		/* disp. */ XFlush();
		/* disp. */ XNextEvent( report );
		/* disp. */ process_event( report );
	//	INFO("reasons_to_stay %d", reasons_to_stay );
		if(!reasons_to_stay_test())
			return;
	}
}

bool X_Display:: process_event( XEvent & event )
{
	X_Window * W1 = find_Window( event.xany.window );
	X_Window * W2 = W1;
	// X_test_box * W2 = (X_test_box *) W1;
	if(!W2) {
		printf("ERROR WINDOW not found\n");
		// return 0;
	}
	switch  (event.type) {
	case Expose:   {
		INFO("case EXPOSE %s %d %d %d %d",
			W2->name,
			event.xexpose.x,
			event.xexpose.y,
			event.xexpose.width,
			event.xexpose.height
		);
		A_Rectangle rect(
			event.xexpose.x,
			event.xexpose.y,
			event.xexpose.width,
			event.xexpose.height
		);
		W2->event_expose( rect );
		// return 0;
	} break;
	case KeyRelease:
		printf("KeyRelease: ");
	case KeyPress: {
		int index = 0;
		// KeySym symb = XKeycodeToKeysym( display, event.xkey.keycode, index);
#define z_group 0
#define z_level 0
		KeySym symb = XkbKeycodeToKeysym(
			display,
			event.xkey.keycode,
			z_group,
			z_level
		);
		const char * str = XKeysymToString( symb );
	//	e_print(" KEY Up = %d \n", Up );
	//	e_print(" KEY KEY_Up = %d \n", XK_Up ); // 65362
	//	e_print(" KEY symb = %ld = XkbKeycodeToKeysym(...) \n", symb );


		printf("Keysym %ld keycode %3d state %2x type %d name %9s win.name %s \n",
			symb,
			event.xkey.keycode,
			event.xkey.state,
			event.xkey.type,
			str,
			W2->name
		);
		if( 24 == event.xkey.keycode ) {
			// exit(0);
			INFO("Close the program if q is pressed.");
			W2->call_XDestroyWindow_top();
			INFO("q is pressed.");
		}

		switch(symb) {
		 
		 /**/   case XK_space:
		 	e_print("ON XK_ space()\n");
		 break; case XK_Up:
		 	e_print("ON XK_ Up()\n");
		 break; case XK_Down:
		 	e_print("ON XK_ Down()\n");
		 break; case XK_Left:
		 	e_print("ON XK_ Left()\n");
		 break; case XK_Right:
		 	e_print("ON XK_ Right()\n");
		 break; case XK_Prior:
		 	e_print("ON XK_ Prior() { PG_UP} \n");
		 break; case XK_Next:
		 	e_print("ON XK_ Next() { PG_DOWN} \n");
		 break; case XK_Home:
		 	e_print("ON XK_ Home()\n");
		 break; case XK_End:
		 	e_print("ON XK_ End()\n");
		 break; case XK_Return:
		 	e_print("ON XK_ Return()\n");
		 break; case XK_BackSpace:
		 	e_print("ON XK_ BackSpace()\n");
		 break; case XK_Delete:
		 	e_print("ON XK_ Delete()\n");
		 break; case XK_Escape:
		 	e_print("ON XK_ Escape()\n");
		 break; case XK_F1:
		 	e_print("ON XK_ F1()\n");
		 break; case XK_F2:
		 	e_print("ON XK_ F2()\n");

		 break; case XK_Menu:
		 	e_print("ON XK_ Menu()\n");
		 break; case XK_Print:
		 	e_print("ON XK_ Print()\n");
		 break; case XK_Scroll_Lock:
		 	e_print("ON XK_ Scroll_Lock()\n");
		 break; case XK_Pause:
		 	e_print("ON XK_ Pause()\n");
		 break; case XK_Break:
		 	e_print("ON XK_ Break()\n");
		 break;

		}
		// return 0;
	}
	break;
// TODO
	case ResizeRequest: {
		e_print( "EVENT ITEM '%s' WH %d %d '%s'\n",
			"ResizeRequest",
			event.xresizerequest.width,
			event.xresizerequest.height,
			W2->name
		); break;
		// XConfigureWindow(), XResizeWindow(), or XMoveResizeWindow().
		// called by WMAN - a DIFFERENT client

	} break;
	case ConfigureNotify: {
		e_print( "EVENT ITEM '%s' WH %d %d '%s'\n",
			"ConfigureNotify",
			event.xconfigure.width,
			event.xconfigure.height,
			W2->name
		);
	//	W2->resize(event.xconfigure.width, event.xconfigure.height);

	} break;

	case ClientMessage: {
		Atom prop = event.xclient.data.l[0];
		char * prop_name = XGetAtomName( display, prop );
		e_print( "EVENT %s prop %s win=%s\n",
			"ClientMessage",
			prop_name,
			W2->name
		);
		if((Atom)event.xclient.data.l[0] == atom_wm_delete_window) {
			INFO("WM_DELETE_WINDOW - needs to be actioned");
			FAIL("WM_DELETE_WINDOW");
		} else {
			FAIL("OTHER ClientMessage");
		}
		XFree( prop_name );
	} break;

	case PropertyNotify: {

// https://tronche.com/gui/x/xlib/window-information/XGetWindowProperty.html

		Atom prop = event.xproperty.atom;
		const char * state = "UNSET";
		switch( event.xproperty.state ) {
		 case PropertyNewValue: state = "NEW"; break;
		 case PropertyDelete: state = "DEL"; break;
		}
		char * prop_name = XGetAtomName( display, prop );

		e_print( "EVENT %s %s %s win=%s\n",
			"PropertyNotify",
			state,
			prop_name,
			W2->name
		);
		XFree( prop_name );
	} break;


#define ITEM(nme) case nme: e_print( "# >>>> # win %-7s EVENT '%s'\n", W2->name, #nme ); break;
	ITEM(ButtonPress)
	ITEM(ButtonRelease)
	ITEM(MotionNotify)
	ITEM(EnterNotify)
	ITEM(LeaveNotify)
	ITEM(FocusIn)
	ITEM(FocusOut)
	ITEM(KeymapNotify)
	ITEM(GraphicsExpose)
	ITEM(NoExpose)
	ITEM(CirculateRequest)
	ITEM(ConfigureRequest)
	ITEM(MapRequest)

	ITEM(CirculateNotify)
	ITEM(CreateNotify)
	ITEM(DestroyNotify) // after this several events refer to win1
	ITEM(GravityNotify)
	ITEM(MapNotify)
	ITEM(MappingNotify)
	ITEM(ReparentNotify)
	ITEM(UnmapNotify)
	ITEM(VisibilityNotify)
	// ITEM() // Circulate items
	ITEM(ColormapNotify)
	ITEM(SelectionClear)
	ITEM(SelectionNotify)
	ITEM(SelectionRequest)

	default:
		printf( "EVENT type %d in %s - default\n", event.type, W2->name );
		// return 0;
	}
	return true;
}

void X_Display::test1()
{
	INFO("CALLED");
	// pointless play code
	int keysym = XK_KP_Add;

	const char * keyname = "KP_Add"; // without the XP_prefix
	keysym = XStringToKeysym( keyname );
	if( keysym == NoSymbol ) {
		keysym = XK_KP_Add;
		printf( "# FAIL # XStringToKeysym('%s')\n", keyname );
	}

	int keycode = 0;
	int modifiers = 0;
	Window grab_window =  RootWindow( display, 0);
	Bool owner_events = False;
	int pointer_mode = GrabModeAsync;
	int keyboard_mode = GrabModeAsync;

	keycode = XKeysymToKeycode( display, keysym );
	modifiers = XK_Alt_L;
	modifiers = XK_Alt_R; // not a modifier ?
	modifiers = AnyModifier;
	owner_events = False;
	owner_events = True;
	grab_window = DefaultRootWindow( display );
// return;

	printf(" XKeyGrab( %s, %d, %x, %ld, %d, %d, %d ); \n",
		"display",
		keycode,
		modifiers,
		grab_window,
		owner_events,
		pointer_mode,
		keyboard_mode
	);

	int t = XGrabKey(
		display,
		keycode,
		modifiers,
		grab_window,
		owner_events,
		pointer_mode,
		keyboard_mode
	);
	if( t == NoSymbol ) {
		printf( "# FAIL # XGrabKey(...)\n" );
	}
	/*
		KEY is now active when window is not top
		KEY is reported on "R-O-O-T" # whatever that is
		CALL is to X_display event loop handler
	*/

}
