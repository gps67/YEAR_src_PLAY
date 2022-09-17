#include "w_widget0.h"

#include "d_colour.h"
#include "obj_hold.h"
#include "dgb_fail.h"
#include "dgb.h"
#include "buffer1.h" // only for type_name() !

#define DBG0 if(1)

//
// static C_ callbacks
//

/*!
	Gtk tells the widget its PRE-allocated size using size_request.
	The widget can overrule the width/height values.

	Once decided, the widget gets a configuration or size_allocated call.
*/
void C_event_size_request(
        GtkWidget *widget,
        GtkRequisition *requisition,
        w_widget0 * self
) {
	if(!self) {
		return;
	}
	// e_print_view // changes size every time it prints ...
#warning "TODO: debug_check_valid_pointer() - prints nothing when OK"
if(0)	self->debug_track_pointer();
if(0)	self->debug_track_pointer();
if(1)	self->debug_check_pointer();
	self->event_size_request( requisition );
}

/*!
	The mouse has moved, call the VTBL function
*/
gboolean C_event_mouse_moved( GtkWidget * widget, GdkEventMotion * ev, w_widget0 * self )
{
	int x = (int) ev->x;
	int y = (int) ev->y;
	self->event_mouse_moved( x, y );
	return TRUE; // ? handled
}

/*!
	'configure' means size decided and widget exists
*/
gint C_event_configure(
	GtkWidget         *widget,
	GdkEventConfigure *event,
	w_widget0 * self )
{
	if(!self) {
		FAIL("NULL self - this never happens");
		return true;
	}
	self->debug_track_pointer();
	self->event_configure( event );
	return TRUE;
}

/*!
	redraw
*/
gint C_event_expose(
	GtkWidget         *widget,
	GdkEventExpose *event,
	w_widget0 * self )
{
	self->debug_track_pointer();
	self->event_expose( event );
	return TRUE;
}

/*!
	This is a vital component of a C++ widgets lifecycle.

	When (after) Gtk deletes a widget (such as when it is removed from
	a container), it calls this (provided you requested the callback).

	No further use of self->w is allowed, so the C function clears w
	before calling the C++ callback.

	NB one valid use of (w) is to remove it by value from tables,
	and possibly check its reference count. This is because
	(supposedly) w deletion is not divided into destroy and finalise.

	Child widgets of self are already deleted, and you use the NULL w
	to avoid calling gtk_widget_remove() or other. Those subs will
	also get their own event_destroy() called, the order is a bit
	slippery, but the C++ object should outlive the gtk widget.

	Parents of w ... dunno

	Some code manually calls event_destroy(), which will only be bad,
	because when GTK does its delete, there will be a callback on
	a deleted (free(mem)) object.

	Cannot call destroy
*/
static gboolean C_event_destroy(
	GtkWidget * widget,
	w_widget0 * self
)
{
	self->debug_track_pointer();

	obj_hold<w_widget0> keeper = self;
	if(self->w)
	{
		if( self->w != widget ) {
			INFO( "widget=%p self=%p self->w=%p ** SHOULD BE SAME **",
				widget,
				self,
				self->w
			);
		}
		self->w_set_NULL();
		self->event_destroyed();
	} else {
		STEP( "already NULL" );
	}
	return TRUE; // ? handled
	// keeper releases the C++ widget here
}

//
// connect the callbacks
//

/*!
*/
void w_widget0::connect_event_mouse_moved()
{
	if(!w) {
		FAIL("NULL w");
		THROW_dgb_fail("NULL w");
		return;
	}
	connect(
		"motion-notify-event",
		(SignalFunc) C_event_mouse_moved
	);
	gtk_widget_add_events(w, GDK_POINTER_MOTION_MASK );
}

/*!
*/
void w_widget0::connect_event_size_request()
{
	if(!w) {
		FAIL("NULL w");
		THROW_dgb_fail("NULL w");
		return;
	}
	connect(
		"size-request",
		(SignalFunc) C_event_size_request
	);
}

/*!
*/
void w_widget0::connect_event_configure()
{
	if(!w) {
		FAIL("NULL w");
		THROW_dgb_fail("NULL w");
		return;
	}
	buffer1 tn;
	INFO("%s.%p %s.%p", type_name(tn), this, G_OBJECT_TYPE_NAME(w), w );
if(0) {
	debug_track_pointer();
}
	connect(
		"configure_event",
		(SignalFunc) C_event_configure
	);
	// http://www.gtk.org/tutorial1.2/gtk_tut-23.html
	gtk_widget_add_events(w, GDK_STRUCTURE_MASK );
	// warning w_fixed - does not have a window ?? some dont
}

/*!
*/
void w_widget0::connect_event_expose()
{
	connect(
		"expose_event",
		(SignalFunc) C_event_expose
	);
}

/*!
*/
void w_widget0::connect_event_destroyed()
{
	if(!w) {
		WARN("w == NULL");
		return;
	}
	connect(
		"destroy",
		(SignalFunc) C_event_destroy
	);
}

/*!
*/
void w_widget0::disconnect_event_destroyed()
{
	if(!w) {
		WARN("w == NULL");
		return;
	}
	connect(
		"destroy",
		(SignalFunc) C_event_destroy
	);
}

//
// C++ virtual callbacks - these arent used if the sub-class defines them
//

/*!
	mouse has moved
*/
void w_widget0::event_mouse_moved( int x, int y )
{
	e_print("w_widget0::event_mouse_moved(%d,%d)\n", x, y );
}

/*!
	size has changed (or been decided)
*/
void w_widget0::event_configure( GdkEventConfigure *event )
{
	buffer1 tn;
	WARN("not handled by %s.%p", type_name(tn), this );
	if(0) {
		debug_print();
		e_print("w_widget0++:event_configure()\n" );
	}
	// w_drawing_area does:: reallocate_pixmap(); // resize
}

/*!
	redraw screen
*/
void w_widget0::event_expose( GdkEventExpose *event )
{
	e_print("w_widget0:event_expose()\n" );
	// w_drawing_area does:: copy pixmap to screen-window();
}

/*!
	calc size

	gtk is asking the widget for its initial size request
	(see w_segs_map, which calls calc_layout_1)

	This function gets the chance of changing that guess,
	and setting a minimum, or additional area.

*/
void w_widget0::event_size_request( GtkRequisition *requisition )
{
	debug_print();
	e_print("w_widget0:event_size_request()\n" );
}


//
//  --
//

/*!
	construct widget that holds GTK widget

	it is tricky for it to make a virtual call to set_w
	so things that need their own class to be called must do late_init
*/
w_widget0::w_widget0( GtkWidget * w1 )
{
	DEBUG_print_stack();
	w_set( w1 );
}

/*!
	destructor - call_destroy() does the real work, and it should
	have been called earlier. If this prints anything, look for static
	members which were not ref_count deleted, just dtor().
*/
w_widget0::~w_widget0()
{
	// this blows up - only call where w-still exists
	if(w) 
	{
		e_print("~w_widget0() destructor - w NOT NULL" );
		debug_track_pointer();
		call_destroy();
	}
	// it is already printed by ref_zero() - with much better VTBL + state
	if(0) debug_track_pointer();
}

/*!
	the underlying gtk object has disappeared,
	and gtk has told us about it.

	To avoid trusting each implementation of event_destroy(),
	w is already set to NULL by the C caller. (GTK has
	already deleted w, and I dont use it as a hash key).

	There are a few straggling manual calls to event_destroy,
	which are to be rerouted through call_destroy()

	There is no NEED for th class to do any more than was done by C_
*/
void w_widget0::event_destroyed()
{
//	DBG0 e_print("w_widget0::event_destroyed() -- CLASS SHOULD REPLACE -- ");
	if(w)
	{
		DBG0 debug_print();
		DBG0 e_print(" %s \n", G_OBJECT_TYPE_NAME( w ) );
		w_set_NULL();
	} else
	{
		INFO("BYE");
		DBG0 debug_track_pointer();
	}
}

// virtual
/*!
*/
void w_widget0::w_set( GtkWidget * w1 )
{
	if(w) debug_track_pointer();
	w = w1;
	debug_track_pointer();
}

void w_widget0::w_set_NULL()
{
	if(!w) {
		STEP("possibly pointless or occasionally so");
	}
	w_set(NULL);
}

/*!
*/
void w_widget0::show_all()
{
	gtk_widget_show_all( outer_packable() );
}

/*!
*/
void w_widget0::hide()
{
	gtk_widget_hide( outer_packable() );
}

/*!
*/
void w_widget0::show()
{
	gtk_widget_show( outer_packable() );
//	debug _everything_ !! OOPS NOT !!
//	if(w) connect_event_configure();
}

// VIRTUAL
/*!
	Rework to call this 
*/
void w_widget0::call_destroy()
{
	debug_track_pointer();
	obj_hold<w_widget0> keeper; keeper = this;
	GtkWidget * w1 = w;
	GtkWidget * outer = outer_packable();
	if(!w1) WARN("OUTER but not w");
	if( w1 == outer ) outer = NULL;
	if( outer )
	{
		const char * type_name = G_OBJECT_TYPE_NAME( outer );
		INFO("OUTER this=%p w=%p outer=%p %s", this, w, outer, type_name );
		gtk_widget_destroy( outer );
		// leave w1 alone!
		return;
	}

	w_set_NULL();
	if(w1)
	{
		gtk_widget_destroy( w1 );
	}

// disconnect other stuff ?
//		if(0)
//			signal_disconnect_by_data();
//		gtk_widget_destroy( outer_packable() );
//		/*
//			since this OFTEN happens in the dtor, class==w_widget0
//		*/
//		if(1) { e_print("Destroy: "); debug_print_ln(); }
//		w = NULL;
//		// gtk_widget_destroy( w1 );
//		// gtk_widget_unref( w1 );
//		/*
//			MAYBE: if the object was packed, gtk owns the recount
//			g_object_unref( w1 );
//
//			That leaves where gtk holds a pointer to obj_ref data
//			which is supposed to be cleared by C_ref_decr(obj)
//		*/
}

/*!
	A simple wrap around gtk_signal_connect, you probably still need to
	cast (SignalFunc) func, because of the type of app.
*/
void w_widget0::connect(
	str0 sig,
	SignalFunc func,	// Simply delete Gtk from (GtkSignalFunc) cast
	obj_ref * app		// NULL defaults to this
) {
	if(!app) app = this;
	if(!w) {
		FAIL("cannot connect_event '%s', w is NULL", (STR0) sig );
		debug_track_pointer();
		THROW_dgb_fail("NULL w");
		// return;
	}
	gtk_signal_connect(
		GTK_OBJECT( w ),
		sig,
		(GtkSignalFunc) func,
		app
	);
}

/*!
	return this->w or for special wrappers the outermost frame.

	outer_packable is rare, used by: (eg) NOWHERE!

	The idea is (eg) to allow a PLAIN class to wrap several
	components, with the INNER frame as the default target,
	but the OUTER frame to be the packable bit.

	Still only makes sense where 'plain' class is derived from w_widget0
	and w is borrowed, NULL, or rewrite without w ?
*/
GtkWidget * w_widget0::outer_packable()
{
	return w;
}

/*!
	return the height of the widget in text-lines, or fields
*/
int w_widget0::approx_line_height() // or maybe two
{
	return 1;	// most are one line
}

/*!
	associate ref_counted obj with this->w g_object (under key)
*/
void w_widget0::set_data( str0 key, obj_ref * obj )
{
	if( !w ) return;
	if( obj ) obj->ref_incr();
	if(1) if (!obj ) {
		INFO("set_data('%s',NULL) ", (STR0) key );
		debug_track_pointer();
	}
	gtk_object_set_data_full(
		(GtkObject *) w ,
		key,
		obj,
		(GtkDestroyNotify) C_ref_decr
	);
}

/*!
	get the named associated data
*/
obj_ref * w_widget0::get_data( str0 key )
{
	return (obj_ref *) gtk_object_get_data(
		(GtkObject *) w ,
		(STR0) key
	);
}

/*!
	each sub-class returns a string for the fields value
*/
str0 w_widget0::get_edited_value_as_cstr()
{
	e_print("get_edited_value_as_cstr=>'EDITED' - mk_edit didnt go far\n");
	return "EDITED";
}

// g_object_set docs are sparse (ready to be deprecated) 
// but its a fair bet that val is copied

/*!
	set a named property (normally a gtk attribute) to a string value

	GTK usually recognises specific properties, eg button."label"
	and manages the memories.
*/
void w_widget0::set_prop( str0 name, str0 val )
{
	gtk_object_set( (GtkObject *) w, name, (STR0) val, NULL );
}

/*!
	set a named property (normally a gtk attribute) to an INTEGER value
*/
void w_widget0::set_prop( str0 name, int val )
{
	gtk_object_set( (GtkObject *) w, name, val, NULL );
}

/*!
	set a named property (normally a gtk attribute) to an g_object value
*/
void w_widget0::set_prop_obj( str0 name, GtkObject * val )
{
	gtk_object_set( (GtkObject *) w, name, val, NULL );
}

/*!
	get a named property which is a string
*/
str0 w_widget0::get_str_prop( str0 name )
{
	char * str = NULL;
	if(w) gtk_object_get( (GtkObject *) w, name, &str, NULL );
	return str;
}

/*!
	get a named property which is a g_object
*/
GtkObject * w_widget0::get_obj_prop( str0 name )
{
	GtkObject * ob = NULL;
	if(w) gtk_object_get( (GtkObject *) w, name, &ob, NULL );
	return ob;
}

/*!
	get a named property which is a int
*/
int w_widget0::get_int_prop( str0 name )
{
	glong val;
	if(w) gtk_object_get( (GtkObject *) w, name, &val, NULL );
	return (int) val;
}

/*!
*/
void w_widget0::e_print_str_prop( str0 name )
{
	buffer1 tn;
	e_print( "TYPE %s NAME %s PROP %s STR %s\n",
		(STR0) type_name(tn),
		(STR0) debug_name_get(),
		(STR0) name,
		(STR0) get_str_prop( name )
	);
}

/*!
*/
void w_widget0::e_print_int_prop( str0 name )
{
	buffer1 tn;
	e_print( "TYPE %s NAME %s PROP %s INT %d\n",
		(STR0) type_name(tn),
		(STR0) debug_name_get(),
		(STR0) name,
		       get_int_prop( name )
	);
}


/*!
	synthesise a redraw request
*/
void w_widget0::request_redraw( int x, int y, int w1, int h )
{
	GdkRectangle update_rect;
	update_rect.x = x;
	update_rect.y = y;
	update_rect.width = w1;
	update_rect.height = h;
	gtk_widget_draw( w, &update_rect);
}

/*!
	get the default drawable
*/
GdkDrawable * w_widget0::get_drawable()
{
	if( !w ) return NULL;
	return w->window; 
}

/*!
	set the minimum usual size
*/
void w_widget0::set_usize( int w_width,  int w_height )
{
	gtk_widget_set_usize( w, w_width, w_height );
}

/*!
	get the current size
*/
void w_widget0::get_size( int & w_width,  int & w_height )
{
	if(!w) {
		w_width = 0;
		w_height = 0;
		return;
	}
	GtkRequisition requisition;
	int TODO_fix_this;
	gtk_widget_size_request ( w, &requisition );
	w_width = requisition.width;
	w_height= requisition.height;
}

/*!
	set the GTK_STATE_NORMAL background colour (not gtk1 though)

	CHANGE to bool - colour might not be known, etc
*/
void w_widget0::set_bg( str0 colour_name )
{
	d_colour * colr = Colour_Map->lookup( colour_name );
	if(!colr)
		colr = Colour_Map->create_new_colour( colour_name );
	if(!colr) return;

	if(!w) {
		FAIL("NULL w");
		return;
	}

#ifdef WITH_GTK2
	// see also gtk_fixed_set_has_window - to make THIS w hava a bg
	debug_track_pointer();
	buffer1 tn;
	INFO("%p %-10s %s", this, type_name(tn), (STR0) colour_name );

	GtkWidget * w2 = gtk_widget_get_parent( w );
	if( w2 == w ) {
		WARN("w2 == w");
		// this never happens, even when framing is to parents w
		// somehow it has its own w, but shared X11 object
		// maybe some other incantation (get gdk window) ...
	}
	gtk_widget_modify_bg(
	//	w2, // big difference for vbox things
	//	With w2 the colours get used
	//	With w, the colours get ignored (for w without own x11 widget)
		w,
		GTK_STATE_NORMAL,
		& colr->colour
	);
#else
	FAIL("w_widget0::set_bg() - not WITH_GTK2\n");
#endif
}


// virtual
void w_widget0::ref_zero()
{
	// seems the problem is w not being valid, this/VTBL is OK
	// INFO("%s this=%p w=%p", type_name(tn), this, w );
	// done by ref_zero // DBG0 debug_track_pointer();
	if(!w)
	{
		// INFO("w already NULL");
	} else {
		ref_incr(); // protect during destroy
		call_destroy();
		ref_decr_private(); // protected during destroy
	}
	obj_ref::ref_zero();
	// delete this;
}

/*!
	save a widgets pixel dump as a (png) file

	The window must be fully visible and not obscured,
	otherwise you get the window in-front (or black for offscreen).

	The idea is to replace 'gd' with standard X11 api (gdk)
	for the production of button gifs for web pages (and other),
	with the small price that the app has to have X11 open
	and a big enough screen. It might be possible to figure
	a 'null' X11 (printer) and use non-screen pixbuf, but this
	is a start.

	Perhaps create pubbuf the same size as window,
	and get it to redraw, somehow.
*/
bool w_widget0::save_png( str0 filebase, str0 comment )
{
	if(!w) return false;

	GdkPixbuf *pixbuf;
	pixbuf = gdk_pixbuf_get_from_drawable(
		NULL,
		w->window,
		gtk_widget_get_colormap(w),
		0,0,
		0,0,
		width(),
		height()
	);
	if( !pixbuf )
	{
		e_print("pixbuf NULL in save_png\n" );
		return false;
	}

	if( !gdk_pixbuf_save(
		pixbuf,
		"drawingarea.png",
		"png",
		NULL,	// &error
		"tEXt::Comment",
		(STR0) comment,
		NULL	// end list of pairs
	))
	{
		e_print("gdk_pixbuf_save failed \n" );
		e_print("Check the contents of PREFIX/etc/gtk-2.0/gdk-pixbuf.loaders \n");
		return false;
	}

	g_object_unref(pixbuf); /* freeing pixbuf */

	return true;
}

/*!
	save a widgets pixel dump as a (jpeg) file
*/
bool w_widget0::save_jpeg( str0 filebase, str0 comment )
{
	if(!w) return false;

	GdkPixbuf * pixbuf;
	pixbuf = gdk_pixbuf_get_from_drawable(
		NULL,
		w->window,
		gtk_widget_get_colormap(w),
		0,0,
		0,0,
		width(),
		height()
	);
	if( !pixbuf )
	{
		e_print("pixbuf NULL in save_jpeg\n" );
		e_print("Check the contents of PREFIX/etc/gtk-2.0/gdk-pixbuf.loaders \n");
		return false;
	}

	if( !gdk_pixbuf_save(
		pixbuf,
		"drawingarea.jpeg",
		"jpeg",
		NULL,
		"quality",
		"100",
		NULL
	)) {
		e_print("gdk_pixbuf_save failed \n" );
		return false;
	}

	g_object_unref(pixbuf); /* freeing pixbuf */

	return true;
	return false;
}



/*
	Note the underscore and moved parameter to this
*/
const char * w_widget0::_G_OBJECT_TYPE_NAME()
{
	if(!w) return "NULL_w";
	return _G_OBJECT_TYPE_NAME();
	return G_OBJECT_TYPE_NAME( w );
}

const char * w_widget0::debug_widget_type_name() // callable as obj_ref0
{
	if(!w) return "w==NULL";
	// This is not an attampt to read private fields from gtk
	// but gtk crashes, so I have to not ask the question
	// if gtk changes, change this
	if(!w->object.parent_instance.g_type_instance.g_class)
		return "w-g_class-NULL";
	return G_OBJECT_TYPE_NAME( w );
}

void w_widget0:: signal_disconnect_by_data()
{
	return; // BLOWS UP ON ALREADY DELETED gtk-w
	// this is happening AFTER the gtk widget has been destroyed
	// need to rethink this, with destroy vector maybe
	if(w)
	 gtk_signal_disconnect_by_data( GTK_OBJECT( w ), this );
}
