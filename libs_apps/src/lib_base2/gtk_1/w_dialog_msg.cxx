#include "w_dialog_msg.h"
#include "dgb_fail.h"
#include "dgb.h"

/*
	This is a bit messed up,
	the gtk base is supposed to return a "response" (int/enum)
	and you set values for extra buttons.

	There was some issue with it not working on the old/new version.
	Currently it is only good for the Copyright popup, and standalone.

	Theres also a problem with ref_incr - who holds this dialog?
	Currently ref_count is usually ZERO {ref_incr(); ref_decr();}
*/


/*!
	The dialog box gets deleted by MOST of the buttons, calling this
	(connected to "response" "cancelled" ...)
*/
// static
void w_dialog::C_destroy_counting(
	GtkWidget * _w, // possibly the button
	w_dialog * self
) {
	INFO("(_w,self) = (%p,%p)", _w, self );
	/*
		w_dialog is not often registered with the holder
		so treat it as a refcounted thing with no references!
	*/
	self->debug_track_pointer();
	INFO("HERE is HERE");
	INFO("HERE is HERE");
	INFO("HERE is HERE");
	INFO("HERE is HERE");
	INFO("HERE is HERE");
	INFO("HERE is HERE");
	INFO("HERE is HERE");
	INFO("HERE is HERE");
	// gdb_invoke(true);
	if(self->get_ref_count() < 1) {
		// special case - avoid OBJ_REF bad count -1
		FAIL("WHY ref_incr()");
		FAIL("WHY ref_incr()");
		FAIL("WHY ref_incr()");
		FAIL("WHY ref_incr()");
		FAIL("WHY ref_incr()");
		FAIL("WHY ref_incr()");
		STEP("WHY ref_incr()");
		 self->ref_incr();
	}
	self->ref_decr();
	int LURK_BUG_this_segfailts_if_you_close;
	/* see w_container where something similar happened
	refcount is OK, its destroy being called on the already free()d gtk w
	which generates a callback on (void *)0xFFFFc
	*/
}

/*
// virtual
void w_dialog::destroy()
{
	FAIL("HERE WE ARE");
	FAIL("HERE WE ARE");
	FAIL("HERE WE ARE");
	FAIL("HERE WE ARE");
	FAIL("HERE WE ARE");
	FAIL("HERE WE ARE");
	FAIL("HERE WE ARE");
	call_destroy();
	// if(w) gtk_widget_destroy( w );
	// base should do this ? // w = NULL;
}
*/

/*!
	destructor for the dialog - should call_destroy() ???
*/
w_dialog::~w_dialog()
{
	/*
		we kept a second reference to the sub-area widgets w

		we did not doa glib obj incr, nor anything else special

		our container objects will disappear soon enough
	*/
	if(!debug_track_pointer()) {
		FAIL("abandoning"); // or throw ?
		return;
	}
	// print some irrelevent stuff
	vbox->debug_track_pointer();
	action_area->debug_track_pointer();
	// we sneaked in copies of these, so unsneak them
	vbox->w_set_NULL();
	action_area->w_set_NULL();
	/*
		I DONT THINK THIS SHOULD BE DONE HERE!!
	*/
	//
	STEP("Calling call_destroy() - dont all widgets do that anyway?");
	call_destroy();
}

/*!
	constructor - connect the signals and build zones (not packed?)
*/
w_dialog::w_dialog()
: w_window()
, vbox( NULL )
, action_area( NULL )
{
	// gdb_invoke(true);
	/*
		allocate the basic layout - ready for embellishments
	*/
	w_set( gtk_dialog_new() ); 

	/*
		This snipped appeared somewhere, pasted for luck
	*/
if(0) // "response" means nothing, version ?
	gtk_signal_connect_object(
		GTK_OBJECT (w),
		"response",
		GTK_SIGNAL_FUNC (gtk_widget_destroy),
		GTK_OBJECT (w)
	);
	// connect( "close", (SignalFunc)C_destroy_counting, this );
	connect( "response", (SignalFunc)C_destroy_counting );

	/*
		gtk has already created action_area and vbox in its world.

		Create C++ wrappers on those existing objects,
		so that we can access them with similar names.
	*/
	action_area = new w_hbox( Dialog()->action_area );
	vbox = new w_vbox( Dialog()->vbox );

	connect_event_destroyed();
}

/*!
	This dialog is a transient for (associated with) mainwin.
	So when mainwin gets minimised, so does this.

	TODO: mainwin->claim_transient( this )
	TODO: mainwin->get_toplevel()
*/
void w_dialog::set_transient_for( w_widget0 * mainwin )
{
	if(!mainwin) return;
	GtkWidget * top = gtk_widget_get_toplevel( mainwin->w );
	gtk_window_set_transient_for( Window(), GTK_WINDOW( top ) );
}

/*!
	UNUSED NOOP
*/
void w_dialog::set_default_response( int resp_id )
{
	INFO("resp_id = %d", resp_id );
	gtk_dialog_set_default_response( Dialog(), resp_id ); 
}

/*!
	add a message label to the dialogs area

	TODO: rewrite as w_widget ?
*/
GtkWidget * w_dialog::add_message( str0 str )
{
	GtkWidget * label = gtk_label_new (str);
	int TODO_use_w_container; // and below
	gtk_container_add(
		GTK_CONTAINER (GTK_DIALOG(w)->vbox),
		label
	);
	gtk_widget_show( label );
	return label;
}

/*!
	add a button to the dialogs action_area
*/
GtkWidget * w_dialog::add_button( bool deflt, str0 str, int resp_id )
{
	GtkWidget * btn = gtk_button_new_with_label( str );
	// response is not closing
	connect( 
		"response",
		(SignalFunc) C_destroy_counting,
		this
	);
 if (0) 
	gtk_signal_connect(
		GTK_OBJECT (btn),
		"clicked",
		// GTK_SIGNAL_FUNC (gtk_widget_destroy),
		GTK_SIGNAL_FUNC( C_destroy_counting ),
		//(GtkObject *)Dialog()
		(GtkObject *)this
	);
	gtk_container_add(
		GTK_CONTAINER( GTK_DIALOG(w)->action_area),
		btn
	);
	gtk_widget_show( btn );
	return btn;
	// gtk_dialog_add_button( Dialog(), str, resp_id );
	if( deflt )
		set_default_response( resp_id );
}

/*!
	Add an OK button, using whatever language str holds
*/
GtkWidget * w_dialog::add_button_ok( bool deflt, str0 str )
{
	return add_button( deflt, str, -1 );
	// return add_button( deflt, str, GTK_REPONSE_OK );
}

/*!
	Add a CANCEL button, using whatever language str holds
*/
GtkWidget * w_dialog::add_button_cancel( bool deflt, str0 str )
{
	return add_button( deflt, str, -1 );
	// return add_button( deflt, str, GTK_REPONSE_CANCEL );
}

/*!
	Dialogs are usually deleted when not visible, but are run()
	after construction
*/
int w_dialog::run()
{
	// return gtk_dialog_run( Dialog() );
	show();
	return 0;
}

/*!
	Generate a dialog box because of some serious error.

	DEMOS ome of the above code. TODO: show e_print window 
*/
// static
void w_dialog::msg_failed( w_widget0 * parent, str0 title, str0 message )
{
	INFO("TITLE: %s MESSAGE %s", (STR0) title, (STR0) message );
#warning "LURK - static reference to a dialog_box - it is all wrong"
	static obj_hold<w_widget0> one_keeper;

	w_dialog * msg  = new w_dialog();
	msg->ref_static();
	WARN("putting STATIC guard on dialog box %p", msg);
	INFO("setting w_dialog %p w = %p", msg, msg->w );
if(1)	one_keeper->debug_track_pointer(); // will FAIL first time
	one_keeper = msg;
	msg->debug_track_pointer();

	msg->set_title( title );
	msg->set_transient_for( parent );
	msg->add_message( message );
	msg->add_button_cancel( TRUE, "Close" );
	msg->run();
}

