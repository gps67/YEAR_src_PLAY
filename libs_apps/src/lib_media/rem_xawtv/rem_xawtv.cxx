
#include "rem_xawtv.h"
#include "buffer1.h"
#include "gdb_invoke.h"
#include "sleep_ms.h"	// sleep_10

// #include "x11_dpy.h"
// #include "rem_xawtv_cmd.h"

#define VERBOSE 1

	static rem_xawtv * rem_xawtv_recent_used = NULL;

#ifdef WIN32
#warning "# MISSING CODE for WIN32"
static int
x11_error_dev_null(Display * dpy, XErrorEvent * event)
{
	return 0;
}
#else

/*!
	Error handler doesnt have much context, use a global?
*/
static int
x11_error_dev_null(Display * dpy, XErrorEvent * event)
{

// gdb_invoke(true);
	e_print("# XErrorEvent->serial       %d\n", (int) event->serial);
	e_print("# XErrorEvent->error_code   %d\n", (int) event->error_code);
	e_print("# XErrorEvent->request_code %d\n", (int) event->request_code);
	e_print("# XErrorEvent->minor_code   %d\n", (int) event->minor_code);
	e_print("# XErrorEvent->resourceid   %d\n", (int) event->resourceid);

	char buf[64];//  = msg.gap_addr_signed_char();
	int len = 63; // msg.space_avail();
	int t = XGetErrorText( dpy, event->error_code, buf, 63 );

	e_print("(t==%d) '%s'\n", t, buf );

#if 0
	buffer1 msg;
	msg.get_space(4096);
	int t = XGetErrorText(
		dpy, // event->display,
		event->error_code,
		msg.gap_addr_signed_char(),		// maybe put other stuff first
		msg.space_avail()
	);
	// t is NOT strlen and trailing_nul places a nul at position 0
	if(t>0) {
		msg.nbytes_used += t;
	}
	e_print("(t==%d) '%s'\n", t, (STR0)str0(msg));
#endif
	if( rem_xawtv_recent_used )
	 if( rem_xawtv_recent_used->dpy == dpy )
	 {
		e_print("x11_error_dev_null - probably xawtv dis-appeared\n");
		rem_xawtv_recent_used->clear_win_id();
	 } else {
		e_print("x11_error_dev_null - but dpy different\n");
	 }
    return 0;
}
#endif

/*!
	xawtv's known window is now unknown
*/
void rem_xawtv::clear_win_id()
{
	if(VERBOSE) e_print("clear_win_id(): was %lx\n", win_id );
	win_id = 0;
}

/*!
	Destructor
*/
rem_xawtv::~rem_xawtv()
{
}

/*!
	Constructor - find_win_id() of xawtv and set error handler
*/
rem_xawtv::rem_xawtv(
	char * dpyname,
	Display * _dpy,
	Window _win_id
) : x11_dpy( dpyname, _dpy )
{
	win_id = _win_id;
	find_win_id();
	rem_xawtv_recent_used = this;
#ifdef WIN32
#warning "# MISSING CODE for WIN32"
#else
	XSetErrorHandler( x11_error_dev_null );
#endif
}

/*!
	search for X11 window with xawtv's STATION property.

	Return false if not found. The cache allows fast repeat checks.
*/
bool rem_xawtv::find_win_id()
{
	if( win_id ) return true;
	Atom station = mk_atom( "_XAWTV_STATION" );
	win_id = find_window_with_atom( station );
	if(VERBOSE) e_print("find_win_id() %lx \n", win_id );
	if(!win_id) {
		e_print("xawtv not running\n");
		return false;
	}
	return true;
}

/*!
	find_win_id() in a loop taking upto 20 seconds
*/
bool rem_xawtv::wait_win_id()
{
	for( int i=0; i<100; i++)
	{
		if( find_win_id() ) return true;
		sleep_10( 2 );
	}
	return false;
}

/*!
	find_win_id() and return it as an X11 window handle
*/
GdkNativeWindow rem_xawtv::get_win_id()
{
	find_win_id();
	if(VERBOSE) e_print("win_id: 0x%ld\n", win_id );
	return (GdkNativeWindow) win_id;
}

/*!
	build an xawtv command and send it
*/
void rem_xawtv::send_cmd_line( int argc, char ** argv )
{
	rem_xawtv_cmd  cmd( argc, argv );
	send( cmd );
	XFlush();
}
/*!
	build an xawtv command and send it
*/
void rem_xawtv::send_cmd_line( const char * buff )
{
	// there was a problem with X11 errors and gdk loop and second poll fd
	if(0==strcmp(buff, "clear_win_id")) {
		clear_win_id();
		return;
	}
	rem_xawtv_cmd  cmd( buff );
	send( cmd );
	XFlush();
}
/*!
	send an xawtv command by setting an X11 property
*/
void rem_xawtv::send( const rem_xawtv_cmd & cmd )
{
	if(VERBOSE) cmd.print();

	static Atom atom_remote = 0;
	if(!atom_remote)
		atom_remote =  mk_atom( "_XAWTV_REMOTE" );

	find_win_id(); // yet again ;-)

#ifdef WIN32
#warning "# MISSING CODE for WIN32"
#else
	XChangeProperty(
		dpy,
		win_id,
		atom_remote,
		XA_STRING,
		8,		// format 8 is char array (bits per array item)
		PropModeReplace,
		(unsigned char *) cmd.words,
		cmd.len_used
	);
	// XFlush();
#endif
}

/*!
	wait for xawtv to appear, then send it the quit command.
	Return false if it doesnt go as expected.
	Caller should have (just) started xawtv.
*/
bool rem_xawtv::wait_and_quit()
{
	/*
		wait for upto 5 seconds for xawtv to startup.
	*/
	for( int i=0; i<20; i++)
	{
		if( find_win_id() ) break;
		sleep_10( 5 );
	}
	/*
		if it started, send the quit command, and wait for
		it to exit (ready for the next to start).

		This is needed because on my system xawtv has to run
		twice. Strange but true.
	*/
	if( find_win_id() ) {
		send( "quit" );
		XFlush();
		// wait for xawtv to exit
		for( int i = 0; i<10; i++ )
		{
			if( !find_win_id() ) break;
			clear_win_id();
			sleep_10( 2 );
		}
		return true;
	}
	else
	{
		e_print( "wait_and_quit() NO XAWTV FOUND\n" );
		return false;
	}
}

/*!
	recover xawtv after being in radio mode. line selects channel

	xawtv suspended by 'capture off' and card reprogrammed for radio
	recover its active state and settings (UK specific)
	setnorm PAL is required, but if its too fast xawtv fails
	either delay or switch its value via secam
	still too fast ...
*/
bool rem_xawtv::recover(const char * line )
{
	if(!line) line = "keypad 8";	// CABLE
	if(!line) line = "keypad 6";
	e_print("rem_xawtv::recover()\n");
	send_cmd_line( line );
	send_cmd_line( "capture overlay" );
	send_cmd_line( "setnorm secam" );
	XFlush();
	sleep_10( 3 );
	send_cmd_line( "setnorm PAL" );
	XFlush();
	sleep_10( 8 );
	send_cmd_line( "setnorm PAL" );
	return TRUE;
}
