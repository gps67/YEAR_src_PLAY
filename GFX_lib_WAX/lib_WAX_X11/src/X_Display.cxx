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

void X_Display:: process_events_forever()
{
	XEvent report;
	while (1)  {
		/* disp. */ XFlush();
		/* disp. */ XNextEvent( report );
		/* disp. */ process_event( report );
	}
}

void X_Display::process_event( XEvent & event )
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
		/*Close the program if q is pressed.*/
		if( 24 == event.xkey.keycode ) exit(0);

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
		e_print( "EVENT ITEM '%s' WH %d %d'%s'\n",
			"ResizeRequest",
			event.xresizerequest.width,
			event.xresizerequest.height,
			W2->name
		); break;
		// XConfigureWindow(), XResizeWindow(), or XMoveResizeWindow().
		// called by WMAN - a DIFFERENT client

	} break;
	case ConfigureNotify: {
		e_print( "EVENT ITEM '%s' WH %d %d'%s'\n",
			"ConfigureNotify",
			event.xconfigure.width,
			event.xconfigure.height,
			W2->name
		);
	//	W2->resize(event.xconfigure.width, event.xconfigure.height);

	} break;
#define ITEM(nme) case nme: e_print( "win %7s EVENT '%s'\n", W2->name, #nme ); break;
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
	ITEM(DestroyNotify)
	ITEM(GravityNotify)
	ITEM(MapNotify)
	ITEM(MappingNotify)
	ITEM(ReparentNotify)
	ITEM(UnmapNotify)
	ITEM(VisibilityNotify)
	// ITEM() // Circulate items
	ITEM(ColormapNotify)
	ITEM(ClientMessage)
	ITEM(PropertyNotify)
	ITEM(SelectionClear)
	ITEM(SelectionNotify)
	ITEM(SelectionRequest)

	default:
		printf( "EVENT type %d in %s - default\n", event.type, W2->name );
		// return 0;
	}
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
