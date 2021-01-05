
#include "w_win_top.h"
#include "TOPAPP_Holder_gtk.h"
#include "TOPAPP_Task_gtk.h"
#include "dgb_fail.h"
#include "buffer1.h"

static w_win_top * old_ptr = NULL;
static GtkWidget * old_ptr_w = NULL;

/*!
*/
void w_win_top::event_destroyed() // virtual call-back
{
	// only called by gtk destructor, w already useless
	w_set_NULL();
	buffer1 tn;
	STEP(type_name(tn));
	STEP("remove this win from app wait-for list");
	debug_track_pointer();
	remove_all();
	if( app )
	{
		app -> window_detach( this );
		app = NULL;
	} else {
		//
		WARN("NULL app");
	}
}

bool C_event_delete_event( 
	GtkObject * w,
	w_win_top * that
) {
	STEP("C_event_delete_event(w,that) - did you press ALT-F4? ");
	INFO("GtkObject * w = %p", w );
	that->debug_track_pointer();
	w_win_top * cpy_old_ptr = old_ptr;
	GtkWidget * cpy_old_ptr_w = old_ptr_w;
	INFO("OLD->w %p %p", cpy_old_ptr, cpy_old_ptr_w );
//	INFO("w_win_top * that = %p %s", that, that->type_name() );
	INFO("w_win_top * that = %p %s", that, "TBA:that->type_name()" );
	INFO("GtkWidget * that.w = %p", that->w );
	if(!that->w) {
		INFO("that.w = %p may  have been cleared by XXX" );
	}
	if( that && that->w ) {
		if( that->w != (GtkWidget*) w ) {
			WARN("( that->w != w ) %p %p", that->w, w );
		}
	}
	that->debug_track_pointer();
	gdb_break_point();
//	gdb_invoke(true);
	if( that ) return that -> event_delete_event();
	return false; // we didnt handle it, ask Gtk to close us
}

/*!
	This is the only class that uses "delete_event", when it is debugged,
	it can be moved to the base class, or not ... It is a top-level-thing
*/
void w_win_top::connect_delete_event()
{
	old_ptr = this;
	old_ptr_w = this->w;
	STEP("connecting 'delete_event'for w_win_top/+ - to handle ALT-F4 or X ");
 if(1) {
	buffer1 tn;
 	INFO("this    = %p %s", this, type_name(tn) );
 	INFO("this->w = %p %s", this->w, debug_widget_type_name() );
 }
	debug_track_pointer();
	gdb_break_point();
 if(0)	gdb_invoke(true);
	if(0) {
		FAIL("refusing");
		return;
	}
	connect( "delete-event", SignalFunc( C_event_delete_event ) );
//	connect( "delete_event", SignalFunc( C_event_delete_event ) );
}

/*!
*/
bool w_win_top::event_delete_event() // virtual call-back
{
	/*
		if WE do not handle the event GTK will
		and the default action is to close the window (somehow)

		That is still a good idea, but if the window manager
		gets a X you can refuse to close:

			handle_event_here_do_not_close_b = true;

		If you have closed, you can return either
	*/
	bool handle_event_here_do_not_close_b = false;

	debug_track_pointer();
	STEP("derived class can override this");

	return handle_event_here_do_not_close_b;
}

/*!
*/
w_win_top::w_win_top( TOPAPP_Task_gtk * _app )
{
	// a marker to see if changing pointer is mad
	strcpy( marker, "gps");

	static TOPAPP_Task_gtk * app_defaulted = NULL;
	if( !app_defaulted ) app_defaulted = _app;
	if( !app_defaulted ) throw "First (MAIN) opened w_win_top cannot default";
	if(_app)
		app =  _app;
	else
		app = app_defaulted;
	w_set( gtk_window_new( GTK_WINDOW_TOPLEVEL ));
	connect_delete_event();
	app -> window_attach( this );

}

/*!
*/
w_win_top::~w_win_top()
{
	STEP("DTOR()");
	debug_track_pointer(); // ref_count
	if(app) app -> window_detach( this );
	/*
		app deletes this.
		this doesnt delete that
		and the obj_hold has PROBABLY/certainly gone out of scope
	*/
	app = NULL;
	remove_all();
	GtkWidget * w1 = w;
	w_set_NULL();
	/*
		Calling destroy after gtk_main_quit causes
		an error message, but not doing so causes
		data held in widgets to not be released!
// Gtk-CRITICAL **: ...  (gtk_main_quit): assertion `main_loops != NULL' failed
	*/
	if(w1) gtk_widget_destroy( w1 );
}

