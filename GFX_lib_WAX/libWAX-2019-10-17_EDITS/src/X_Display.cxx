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

void X_Display::process_event( XEvent & report )
{
	X_Window * W1 = find_Window( report.xany.window );
	X_Window * W2 = W1;
	// X_test_box * W2 = (X_test_box *) W1;
	if(!W2) {
		printf("ERROR WINDOW not found\n");
		// return 0;
	}
	switch  (report.type) {
	case Expose:   {
		A_Rectangle rect(
			report.xexpose.x,
			report.xexpose.y,
			report.xexpose.width,
			report.xexpose.height
		);
		W2->event_expose( rect );
		// return 0;
	}
	case KeyRelease:
		printf("KeyRelease: ");
	case KeyPress: {
		int index = 0;
		// KeySym symb = XKeycodeToKeysym( display, report.xkey.keycode, index);
#define z_group 0
#define z_level 0
		KeySym symb = XkbKeycodeToKeysym(
			display,
			report.xkey.keycode,
			z_group,
			z_level
		);
		const char * str = XKeysymToString( symb );
	//	e_print(" KEY Up = %d \n", Up );
		e_print(" KEY KEY_Up = %d \n", XK_Up );
		e_print(" KEY symb = %d \n", symb );


		printf("keycode %3d state %2x type %d name %9s win.name %s \n",
			report.xkey.keycode,
			report.xkey.state,
			report.xkey.type,
			str,
			W2->name
		);
		/*Close the program if q is pressed.*/
		if( 24 == report.xkey.keycode ) exit(0);

		switch(symb) {
		 
		 /**/   case XK_space:
		 	e_print("ON XK_ spave()\n");
		 break; case XK_Up:
		 	e_print("ON XK_ Up()\n");
		 break; case XK_Down:
		 	e_print("ON XK_ Down()\n");
		 break; case XK_Left:
		 	e_print("ON XK_ Left()\n");
		 break; case XK_Right:
		 	e_print("ON XK_ Right()\n");
		 break; case XK_Prior:
		 	e_print("ON XK_ Prior()\n");
		 break; case XK_Next:
		 	e_print("ON XK_ Next()\n");
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
	default:
		printf( "EVENT type %d in %s\n", report.type, W2->name );
		// return 0;
	}
}

void X_Display::test1()
{
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
return;

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

}
