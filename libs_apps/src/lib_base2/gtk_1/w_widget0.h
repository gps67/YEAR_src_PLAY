#ifndef w_widget0_H
#define w_widget0_H

class w_widget0; // for following declaration of SignalFunc

#ifdef WITH_GTK2
#include <gtk/gtk.h>
typedef GtkSignalFunc SignalFunc;
#else
// typedef GtkSignalFunc SignalFunc;

struct GtkWidget;
struct GdkEventConfigure;
struct GdkDrawable;
struct GtkObject;
struct GtkRequisition;
struct GdkEventExpose;
struct GdkRectangle;
struct GTK_OBJECT;
struct gtk_signal_disconnect_by_data;

typedef void (SignalFunc) (
	GtkWidget * w,		// not a pointer to w_widget !
	GdkEventConfigure *event,
	w_widget0 * self 	// or OTHER client data
);
#endif

#include "obj_ref.h"


/*!
	Prototype of a call_back_function( gtkwidget, eventdata, clientData )

	The eventdata varies, as does the rest, so its a placeholder
	rather than correct
	
*/
// typedef GtkSignalFunc SignalFunc;

#if 0
// struct obj_ref;
typedef void (SignalFunc) (
	GtkWidget * w,		// not a pointer to w_widget !
	GdkEventConfigure *event,
	w_widget0 * self 	// or OTHER client data
);
#endif

/*!
	this provides the full decl, when really the proto should be used
	remove this and fix other places that depend on it!
*/
#include "str0.h"


/*!
	w_widget0 is the base class of all wrapped (gtk) widgets,
	with a pointer to the GtkWidget * w;

	It provides various utility functions, and code-themes,
	which make the wrapped widgets easier to use, and quite simple
	to implement.

	I think virtual callbacks are the best signal mechanism (for C++),
	so for example w_button introduces a virtual clicked() function.
	That does then require you to derive a subclass, which costs
	a static VTBL, and a few lines of declaration but then its simple.

	The main case where an object doesnt have ITSELF as its target,
	would be something like a button in a form that adds a row to
	the adjacent tree-panel. (See weak-link for circular up-references).
	Thats handled by a specific class of button that has a pointer to
	something that coordinates all these things together.

	Most widgets have a non-null w, but a few are created null, and
	late_init() adds more code before calling gtk. During deconstruction
	a null w means that the GTK has been deleted.
	
*/
class w_widget0 : public obj_ref
{
 public:
	//! The GTK widget that this C++ class wraps
	GtkWidget * w;

	//! Every class casts w into the expected
	GtkWidget * Widget() { return w; }

	//! constructor - plain
	w_widget0()
	{
		w = NULL;
		debug_set_is_widget();
	}

	//! destructor
	~w_widget0();

	//! construct from an EXISTING gtk widget (RARE)
	w_widget0( GtkWidget * w ); // UNUSED but allowed

/*
	static w_widget0 * get_self( GtkWidget * w0 )
	{
		return (w_widget0 *) gtk_object_get_data(
			(GtkObject *) w0,
			"self"
		);
	}

	void set_self()
	{
		gtk_object_set_data(
			(GtkObject *) w,
			"self",
			this
		);
	}
*/

	void hide();	// outer_packable
	void show();	// outer_packable
	void show_all();	// outer_packable
	void connect(
		str0 sig,
		SignalFunc func,
		obj_ref * app = NULL // default becomes this
	);

	void signal_disconnect_by_data();

	virtual void call_destroy();
	virtual GtkWidget * outer_packable();;
	virtual int approx_line_height(); // ie use 5 rows of a table
	virtual GdkDrawable * get_drawable();
	virtual str0 get_edited_value_as_cstr();

	virtual void w_set( GtkWidget * _w );
	void w_set_NULL();
	GtkWidget * w_get() { return w; }
	void set_data( str0 key, obj_ref * obj );
	obj_ref * get_data( str0 key );
	void set_prop_obj( str0 name, GtkObject * obj );
	void set_prop( str0 name, str0 val );
	void set_prop( str0 name, int val );
	str0 get_str_prop( str0 name );
	void e_print_str_prop( str0 name );
	void e_print_int_prop( str0 name );
	GtkObject * get_obj_prop( str0 name );
	int         get_int_prop( str0 name );

#ifdef WITH_GTK2
	int width() { return w->allocation.width; }
	int height() { return w->allocation.height; }
#else
#endif

	//! Gtk negociates size, Widget can change requisition
 virtual
	void event_size_request( GtkRequisition *requisition );
 virtual
	void event_configure( GdkEventConfigure *event );
 virtual
	void event_expose( GdkEventExpose *event ); 
 virtual
	void event_mouse_moved( int x, int y );
 virtual
	void event_destroyed();

	// virtual void debug_event_on_delete(); // reserved by prefix

	void disconnect_event_destroyed();
	void connect_event_destroyed();
	void connect_event_mouse_moved();
	void connect_event_configure();
	void connect_event_expose(); // mostly by w_drawing_area
	void connect_event_size_request();

	void request_redraw( int x, int y, int w1, int h );
#ifdef WITH_GTK2
	void request_redraw()
	{
		request_redraw( 0,0, width(), height() );
	}

	//! request a redraw
	void draw( GdkRectangle * update_rect )
	{
		gtk_widget_draw( w, update_rect);
	}
#endif

	// a start
	void set_bg( str0 colour_name );

	// set widgets min/usual size
	void set_usize( int w_width,  int w_height ) ;
	void get_size( int & w_width,  int & w_height ) ;

	bool save_png( str0 filebase, str0 comment );
	bool save_jpeg( str0 filebase, str0 comment );

	// is this a macro or something
	const char * _G_OBJECT_TYPE_NAME();
	const char * debug_widget_type_name();

 virtual void ref_zero();

};

typedef void (*FN_wp) (GtkWidget *, obj_ref *);

#endif
