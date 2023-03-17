#include "X_Display.h"
#include "X_Display_UDEF.h"
// #include "X_ALL.h" // X_Display.cxx:#include <X11/keysym.h>
#include <X11/keysym.h>


using namespace WAX;

void X_Display_UDEF::test1()
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

