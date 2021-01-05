#include "g_adjustment.h"
#include "w_widget0.h"

/*!
	destructor - release obj (should be a base class?)
*/
g_adjustment::~g_adjustment()
{
	if(obj) gtk_object_unref( obj );
	obj = NULL;
}

/*!
	constructor - lots of steps settings
*/
g_adjustment::g_adjustment(
	gdouble val,
	gdouble lo,
	gdouble hi,
	gdouble one,
	gdouble many,
	gdouble page
)
{
	obj = gtk_adjustment_new( val, lo, hi, one, many, page );
	if(1) gtk_object_ref( obj ); // LURK
	if(1) gtk_object_ref( obj ); // LURK
	gtk_signal_connect(                                                     
		obj,
		"value-changed",                                              
		(SignalFunc) C_event_value_changed,                              
		this                                                            
	);                                                                      
}

/*!
	C --> C++
*/
gint g_adjustment::C_event_value_changed(
	GtkAdjustment * self_base,
	g_adjustment * self
)
{
	self->debug_check_pointer();
	self->event_value_changed();
	return 0;
}

/*!
	get the floating point value
*/
gdouble g_adjustment::get_val()
{
#ifdef WITH_GTK2
	return gtk_adjustment_get_value( Adjustment() );
#else
	return Adjustment()->value;
#endif
}

/*!
	set the value
*/
void g_adjustment::set_val( gdouble val )
{
	gtk_adjustment_set_value( Adjustment(), val );
}

/*!
	get the GtkAdjustment that this C++ class wraps
*/
GtkAdjustment * g_adjustment::Adjustment() {
	if(!obj) {
		e_print("NULL obj g_adj\n");
	}
	return (GtkAdjustment *) obj;
}

/*!
	VIRTUAL - app should do something
*/
void g_adjustment::event_value_changed()
{
	e_print("Adjustment::event_value_changed to %f (BASE)\n", get_val() );
}


/*
The g_adjustment gets event_value_changed(v)
the w_scale gets event_value_changed(v)
*/

/*!
	Constructor - A g_adjustment_silent is a g_adjustment,
	but makes less noise.

	Allocates own new g_adjustment
*/
g_adjustment_silent::g_adjustment_silent(
	gdouble val,
	gdouble lo,
	gdouble hi,
	gdouble one,
	gdouble many,
	gdouble page
)
: g_adjustment( val, lo, hi, one, many, page )
{
}

/*!
	The app should replace this function, so that something actually
	happens. Note that tv_panel line_factor_slider : public w_scale_fix
	uses a g_adjustment_silent, (throws its value away), and uses the
	GUI sliders event_value_changed

*/
void g_adjustment_silent::event_value_changed()
{
	gdouble val = get_val();
	if(0) e_print("Adjustment::event_value_changed to %f (SILENT)\n", val );
}

