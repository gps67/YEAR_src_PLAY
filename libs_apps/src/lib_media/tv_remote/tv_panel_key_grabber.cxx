#include "tv_panel.h"
//#include <gdkkeysym.h>
#include <gdk/gdkkeysyms.h>

using namespace TV_REM;

#ifdef WIN32
#warning WIN32 code = nonsense
#define GrabModeAsync 0
#endif

/*
	gtk key bindings are based on a per-widget basis
	but I want 'global' settings ( also want WM grab
	but thats another issue), hence this crude filter.
*/

//! C callback that calls the C++ tv_panel member function
static
bool C_snooper(
	GtkWidget *grab_widget,
	GdkEventKey *event,
	tv_panel * panel
)
{
	return panel->key_snooper( grab_widget, event );
}

bool tv_panel::grab_key( int keycode, int modifier )
{
#ifdef WIN32
#warning WIN32 code = nonsense
	return 0;
#else
	// GdkWindow * w1 = gdk_get_default_root_window();

	Display * display = GDK_DISPLAY();
	// int root_wnd = DefaultRootWindow( display );
	int root_wnd = GDK_ROOT_WINDOW();
	bool owner_events = false;
	int  pointer_mode = GrabModeAsync;
	int keyboard_mode = GrabModeAsync;

	if( w && w->window ) {
		root_wnd = DefaultRootWindow( display );
		// root_wnd = GDK_DRAWABLE_XID( w->window );
	} else {
		e_print( "w->window is NULL !\n");
		e_print( "root_wnd = %lx\n", root_wnd );
		root_wnd = DefaultRootWindow( display );
		e_print( "root_wnd = %lx\n", root_wnd );
	}

		e_print( "root_wnd = %lx\n", root_wnd );

	if(1) owner_events = True;
	// this causes the display to lockup ! (ungrab?)
	if(0) pointer_mode = GrabModeSync;
	if(0) keyboard_mode = GrabModeSync;
	if(1) modifier = 8000; // AnyModifier;
	if(1) modifier = AnyModifier;

	/*
		This isnt quite there yet
		I think maybe it is working
		and Gtk receives something
		but that isnt sent to any window in particular

		One answer is to setup a second X11 connection
		One answer is to get Gtk to send ... to ...
	*/


	int t = XGrabKey(
		display,
		keycode,
		modifier,
		root_wnd,
		owner_events,
		pointer_mode,
		keyboard_mode
	);
	const char * str = "NoValue";
	switch( t ) {
	 case BadAccess:	str = "BadAccess"; 	break;
	 case BadValue:		str = "BadValue"; 	break;
	 case BadWindow:	str = "BadWindow"; 	break;
	 default:
		 str = "default";
	}
	e_print("grab_key( %d, %d ) -> %d %s \n", keycode, modifier, t, str );
	return t;
#endif

	
}

gboolean C_key_pressed(
	GtkWidget * widget,
	GdkEventKey * ev,
	w_widget0 * self
)
{                                                                               
	printf("KEY ");
        return TRUE; // ? handled                                               
}                                                                               

//! some keys are GLOBALLY active
void tv_panel::init_key_snooper()
{
	GdkWindow * gdk_root = gdk_get_default_root_window();
	// "key-press-event" and "key-release-event" signals. 
	// gtk_signal_connect(
	g_signal_connect(
		G_OBJECT(gdk_root),
		"key-press",
	//	"key-press-event",
	//	(GtkSignalFunc) C_key_pressed,
		(GtkSignalFunc) C_snooper,
		this
	);

	gtk_key_snooper_install( (GtkKeySnoopFunc) C_snooper, this );
	// return;
	grab_key( GDK_KP_Add,		GDK_Alt_L );
	grab_key( GDK_KP_Subtract,	GDK_Alt_L );
	grab_key( GDK_KP_Multiply,	GDK_Alt_L );
	grab_key( 63, 8 );
	grab_key( 86, 8 );
	grab_key( 112, 8 );
	grab_key( 82, 8 );
}


void e_print_KEY( GdkEventKey *event )
{
	str0 press = "PRESS";
	if( event->type == GDK_KEY_RELEASE ) press = "RELEASE";

	e_print(" # KEY: %4x %9s %7s flags: %4.4x raw: %4.4x '%s'\n",
		event->keyval,
		gdk_keyval_name( event->keyval ),
		(STR0) press,
		event->state,
		event->hardware_keycode,
		event->string
	);
}

//! some keys are GLOBALLY active, hence snooped
bool tv_panel::key_snooper(
	GtkWidget *grab_widget,
	GdkEventKey *event
)
{
	static const bool used = true;
	static const bool unused = false;
	switch( event->type ) {
	 case GDK_KEY_PRESS:
e_print_KEY( event );
	 break;
	 case GDK_KEY_RELEASE:
		e_print_KEY( event );
		return unused;
	 break;
	 default:
		e_print("KEY: NEITHER PRESS NOT RELEASE\n" );
		e_print_KEY( event );
		return unused;
	}
	switch( event->keyval ) {
	 case GDK_Up:
		slider_vol->set_value( slider_vol->get_value() + 10 );
		return used;
	 break;
	 case GDK_Down:
		slider_vol->set_value( slider_vol->get_value() - 10 );
		return used;
	 break;
	 case GDK_Right:
		// e_print( "slider_factor KEY\n");
		slider_factor->set_value( slider_factor->get_value() + 0.1 );
		return used;
	 break;
	 case GDK_Left:
		// e_print( "slider_factor KEY\n");
		slider_factor->set_value( slider_factor->get_value() - 0.1 );
		return used;
	 break;
	 case GDK_Page_Up:
		if( radio.is_open() )
			radio_change_channel( channel_number_radio+1 );
		else
			tv_change_channel( channel_number_tv+1 );
		return used;
	 break;
	 case GDK_Page_Down:
		if( radio.is_open() )
			radio_change_channel( channel_number_radio-1 );
		else
			tv_change_channel( channel_number_tv-1 );
		return used;
	 break;
#warning "Hardcoded LIRCD commands"
// also why +- <> Page_Up ... should consolidate, know VCR||tune||radio
	 case GDK_KP_Add:
		ir_client->send_commands("CABLE","Channel+");
		ir_client->send_commands("DS608P","up");
		return used;
	 case GDK_KP_Subtract:
		ir_client->send_commands("CABLE","Channel-");
		ir_client->send_commands("DS608P","down");
		return used;
	 case GDK_KP_Divide:
	 case GDK_KP_Multiply:
		ir_client->send_commands("CABLE","1 0 1");
		ir_client->send_commands("DS608P","0 1");
		return used;
/*
	There is no such feature
	 case GDK_t:
		rem.send_cmd_line( "keypad t" );
		return used;
*/
	 case GDK_q:
	 case GDK_Escape:
		if( radio.is_open() ) {
			radio.mute();
		} else {
			rem.send_cmd_line( "quit" );
		}
		gtk_main_quit();
	 break;
	 case GDK_f:
		if( radio.is_open() ) {
                } else {
                        tv_fullscreen();
                }
	 break;

	 case GDK_KP_Enter:
		e_print_KEY( event );
		if( radio.is_open() )
			radio.mute_toggle();
		else
			rem.send_cmd_line( "volume mute" );
		return used; // otherwise enter presses the Exit button!
	 break;
	 case GDK_KP_Insert:
	 case GDK_KP_0:
		if( radio.is_open() )
			tv_change_channel( channel_number_tv );
		else
			radio_change_channel( channel_number_radio );
		return used;
	 case GDK_KP_End:
	 case GDK_KP_1: tv_change_channel( 0 ); return used;
	 case GDK_KP_Down:
	 case GDK_KP_2: tv_change_channel( 1 ); return used;
	 case GDK_KP_Page_Down:
	 case GDK_KP_3: tv_change_channel( 2 ); return used;
	 case GDK_KP_Left:
	 case GDK_KP_4: tv_change_channel( 3 ); return used;
	 case GDK_KP_Begin:
	 case GDK_KP_5: tv_change_channel( 4 ); return used;
	 case GDK_KP_Right:
	 case GDK_KP_6: tv_change_channel( 5 ); return used;
	 case GDK_KP_Home:
	 case GDK_KP_7: tv_change_channel( 6 ); return used;
	 case GDK_KP_Up:
	 case GDK_KP_8: tv_change_channel( 7 ); return used;
	 case GDK_KP_Page_Up:
	 case GDK_KP_9: tv_change_channel( 8 ); return used;
	 default:
		e_print_KEY( event );
		return unused;
	}
	return unused;
}
