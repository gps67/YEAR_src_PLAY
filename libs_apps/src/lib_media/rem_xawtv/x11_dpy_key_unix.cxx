#include "x11_dpy.h"
#include "dgb_fail.h"

bool x11_dpy::lookup_modifier_mask_by_name( const char * _mod, int & var ){
	/*
		X11 is messed up here
		it cant commmit to actual names
		which ARE used elsewhere.
		
		Theres probably a real mechanism
		to commit to ALT_L but ... aaaaghhh
	*/
	str0 mod(_mod);
	int val = 0;
	/**/ if( mod == "" ) val = 0;
	else if( mod == "NONE" ) val = 0;
	else if( mod == "ANY" ) val = AnyModifier;
	else if( mod == "SHIFT" ) val = ShiftMask;
	else if( mod == "LOCK" ) val = LockMask;
	else if( mod == "CONTROL" ) val = ControlMask;
	else if( mod == "CTRL" ) val = ControlMask;
	else if( mod == "MOD1" ) val = Mod1Mask;
	else if( mod == "ALT" ) val = Mod1Mask;
	else if( mod == "MOD2" ) val = Mod2Mask;
	else if( mod == "NUM-LOCK" ) val = Mod2Mask;
	else if( mod == "NUM" ) val = Mod2Mask;
	else if( mod == "MOD3" ) val = Mod3Mask;
	else if( mod == "MOD4" ) val = Mod4Mask;
	else if( mod == "WINDOWS" ) val = Mod4Mask;
	else if( mod == "MOD5" ) val = Mod5Mask;
	else {
		e_print("# FAIL # lookup_modifier_by_name( %s ) \n", _mod );
		return false;
	}
	var = val;
	return true;
}
bool x11_dpy::Grab_Key_ROOT (
	const char * str_key, 
	const char * str_mod
) {
	int keycode;
	int keysym;
	if( !lookup_Key_by_name( str_key, keysym, keycode ) ) { return false; }
	int mod_base; // eg CTRL NP_Add - any NUMLOCK
	if(!lookup_modifier_mask_by_name( str_mod, mod_base )) { return false; }
	if(!Grab_Key_ROOT( keycode, mod_base ) ) { return false; }
	return true;
}
bool x11_dpy::Grab_Key_ROOT_ITERS (
	const char * str_key, 
	const char * str_mod
) {
	int keycode;
	int keysym;

	int mod_NUM;
	int mod_CAP;

	if(!lookup_modifier_mask_by_name( "NUM", mod_NUM )) { return false; }
	if(!lookup_modifier_mask_by_name( "LOCK", mod_CAP )) { return false; }
	int mod_base; // eg CTRL NP_Add - any NUMLOCK
	if(!lookup_modifier_mask_by_name( str_mod, mod_base )) { return false; }

	if( !lookup_Key_by_name( str_key, keysym, keycode ) ) { return false; }
	if(!Grab_Key_ROOT( keycode, mod_base ) ) { return false; }
	if(!Grab_Key_ROOT( keycode, mod_base|mod_NUM ) ) { return false; }
	if(!Grab_Key_ROOT( keycode, mod_base|mod_CAP ) ) { return false; }
	if(!Grab_Key_ROOT( keycode, mod_base|mod_NUM|mod_CAP ) ) { return false; }
	return true;
}
bool x11_dpy::Grab_Key_ROOT (
	int keycode, 
	int modifiers
) {
	Window grab_window = DefaultRootWindow( dpy );
	// Window grab_window =  RootWindow( dpy, 0);
	return Grab_Key( grab_window, keycode, modifiers );
}

bool x11_dpy::Grab_Key (
	Window grab_window,
	int keycode, 
	int modifiers
) {
	Bool owner_events = True; // yes I handle them
	int pointer_mode = GrabModeAsync; // dont lock up
	int keyboard_mode = GrabModeAsync; // dont lock up

	int t = XGrabKey(
		dpy,
		keycode,
		modifiers,
		grab_window,
		owner_events,
		pointer_mode,
		keyboard_mode
	);

	printf("# LOG #  XKeyGrab( %s, %d, x%2.2x, %2.2x, %d, %d, %d ); --> %d \n",
		"display",
		keycode,
		modifiers,
	(int)	grab_window,
		owner_events,
		pointer_mode,
		keyboard_mode,
		t
	);
	if( t == NoSymbol ) {
		printf( "# FAIL # XGrabKey(...)\n" );
		return false;
	}
// LURK there is still a bug with multiple requests causing a ...
// blockage, or lack of processed items, or ... ?
//	XFlush();
	return true;
}

fd_x11::~fd_x11()
{
	e_print("# DTOR # fd_x11::~fd_x11()\n" );
}

fd_x11::fd_x11()
{
	// simple default CTOR in need of late_init( dpy )
}

fd_x11::fd_x11( x11_dpy * _dpy )
{
	late_init( _dpy );
}

void fd_x11::late_init( x11_dpy * _dpy )
{
	if(!dpy) {
		return_VOID_dgb_fail("null ddisplay");
		return;
	}
	dpy = _dpy;
	Display * display = dpy->dpy;
	int xfd = XConnectionNumber(display);
	fd_set_socket( xfd ); // LURK base class doesnt set any subscriptions seen etc
	subscribe_each(); // LURK set_non_block()
	//	seen_connected = true; // dont report much
	//	set_block(); // LURK what did X11 do ?
}

bool fd_x11::select_says_read()
{
	if(!dpy) {
		return FALSE_dgb_fail("fd_x11::select_says_read() - NULL dpy");
		return false;
	}
	dpy->select_says_read();
//	dpy->XFlush();
	return true; // keep subscription
}

bool x11_dpy::select_says_read()
{
	printf(" x11_dpy::select_says_read() \n" );
	XEvent ev;
	// if QLength doesnt read fd how does it proceed?
	while(QLength(dpy)>0) {
		// XFlush(); // done by XNextEvent
		XNextEvent( dpy, & ev );
		// skip over XIM stuff (see XXX for more information)
		if (ev.type != KeyPress && ev.type != KeyRelease) {
			if (XFilterEvent(&ev, None)) continue;
		}
        
		switch( ev.type ) {
		 case KeyPress: {
			int index = 0;
			KeySym symb = XKeycodeToKeysym(
				dpy,
				ev.xkey.keycode,
				index
			);
			const char * str =  XKeysymToString( symb );

			printf("keycode %3d state 0x%2x type %d name %s win.name %s \n",
				ev.xkey.keycode,
				ev.xkey.state,
				ev.xkey.type,
				str,
				"NONAME"
			);

		 }
		}
	}
	XFlush();
	return true; // keep subscription
}

bool  x11_dpy::lookup_Key_by_name(
	const char * keyname, 
	int & keysym,
	int & keycode 
){
	// name to symbol - universal table for X11R6
	keysym = XStringToKeysym( keyname );
	if( keysym == NoSymbol ) {
		printf( "# FAIL # XStringToKeysym('%s')\n", keyname );
		return false;
	}

	// keycode is specific to the hardware of the display (scan code)
	keycode = XKeysymToKeycode( dpy, keysym );
	if( keycode == 0 ) {
		printf( "# FAIL # XKeysymToKeycode() %s )\n", keyname );
		return false;
	}
	return true;
}

void x11_dpy::test1()
{

	if(!Grab_Key_ROOT( "KP_8", "CTRL" ) ) { return; }
	//	if(!Grab_Key_ROOT_ITERS( "KP_9", "NONE" ) ) { return; }
	if(!Grab_Key_ROOT_ITERS( "KP_9", "CTRL" ) ) { return; }
//	XFlush();
	return;

	/*
		modifiers is MAD
		probably needs enumerating through combinations
		particularly NUM-LOCK
		which MUST be exactly as specified for GRAB
	modifiers = XK_Alt_R; // not a modifier ? keysymdef.h L265
	modifiers = AnyModifier; // OK // X.h LINE 252
	modifiers = XK_Alt_L; // not a valid modifer seee X.h

	modifiers = Mod1Mask; // cant find it
	modifiers = Mod2Mask; // acted as 
	modifiers = 0xFF; 
	modifiers = AnyModifier; // OK // X.h LINE 252
	modifiers = 0x1FFF; 
	modifiers = 0x03FF; 
	modifiers = 0x00FF; 
	modifiers = 1<<0;
	const char * mod = "ANY";
	mod = "ANY"; // OK 
	mod = "SHIFT"; // OK SHIFT with CAPSLOCK NUMLOCK OFF
	mod = "LOCK"; // OK CapsLock (when NOT NUMLOCK!)
	mod = "CTRL"; // OK - (BUT no NUMLOCK allowed)
	mod = "MOD1"; // OK - LEFT-ALT
	mod = "MOD2"; // OK - NUM-LOCK
	mod = "MOD3"; // UNKNOWN 0x20
	mod = "MOD4"; // WINDOWS-KEY // left / right
	mod = "MOD5"; // ALT-GR

	// MISSING = MENU key

	mod = "CTRL"; // OK - (BUT no NUMLOCK allowed)
	*/

	int modifiers_combined = 0;
	int modifiers = 0;
	int keycode = 0;

	if(!lookup_modifier_mask_by_name( "CTRL", modifiers )) { return; }
	modifiers_combined |= modifiers;


	if(!lookup_modifier_mask_by_name( "NUM-LOCK", modifiers )) { return; }
	modifiers_combined |= modifiers;

	if(!Grab_Key_ROOT( keycode, modifiers_combined ) ) { return; }

	XFlush();
}


#if 0
int XX_main(int argc, char ** argv)
{
	x11_dpy X;
	TOPAPP_Task_Holder holder(&argc, &argv);
	//	gtk_init( &argc, &argv );
	new gtk_text_top_app( & holder, "FILENAME" ); 
	X.test1();
	if(0)	X.XFlush(); // if a sub function forgot this it fails!
	gtk_main();
	return 0;
}
#endif
