#include "w_scale.h"

/*!
	VIRTUAL: get the GtkAdjustment. w_scale_fix from tv_panel.h
	hold it externally, and connects it at constructor time.
	Probably only works with GTK2 

	TODO: fix this
*/
GtkAdjustment * w_scale::get_adjustment()
{
#ifdef WITH_GTK2
	return gtk_range_get_adjustment( GTK_RANGE(w) );
#else
	/*
		THIS IS WRONG

		it returns a VScale (HScale)
		why ... dunno

		if only they retro ported gtk_range_get_adjustment to GTK1
	*/
	GtkAdjustment * adj = NULL;
	gtk_object_get( (GtkObject*)w, "adjustment", &adj, NULL );
	if(!adj) e_print("adjustment = %p\n", adj );
	if(0) e_print("adjustment = %p\n", adj );
	return GTK_ADJUSTMENT( adj ); // LURK V_scale [H]
#endif
}

/*!
	destructor - signal_disconnect_by_data() - GOOD/BAD?
*/
w_scale::~w_scale()
{
	/*
		The base widget is supposed to (NOT) do this,
		and IT used to crash, so maybe this is out of place?
	*/
	signal_disconnect_by_data();
}

/*!
	Paper Page Scrollers increase downwards (TRUE), volume upwards (FALSE)
*/
void w_scale::set_inverted( bool corrected )
{
#ifdef WITH_GTK2
	gtk_range_set_inverted( Range(), corrected );
#endif
}

/*!
	constructor - attach to an existing g_adjustment
*/
w_scale::w_scale( bool horiz, g_adjustment * adj )
{
	// adj->ref_incr(); // NONEED ??
	// this->ref_incr(); // NONEED ??
	// this->ref_incr(); // NONEED ??

	if( horiz )
		w_set( gtk_hscale_new( adj->Adjustment() ));
	else
		w_set( gtk_vscale_new( adj->Adjustment() ));
	set_inverted( TRUE );

#ifdef WITH_GTK2
	g_signal_connect(
		G_OBJECT (get_adjustment() ),
		"value_changed",
		G_CALLBACK( C_event_value_changed ),
		this
	);
#else
	gtk_signal_connect(
		GTK_OBJECT( adj->Adjustment()),
		"value_changed",
		(GtkSignalFunc) C_event_value_changed,
		this );
#endif
}

/*!
	constructor - without an external g_adjustment
*/
w_scale::w_scale(
	bool horiz,
	double lo,
	double hi,
	double step
) {
#ifdef WITH_GTK2
	if( horiz )
		w_set( gtk_hscale_new_with_range( lo, hi, step ));
	else
		w_set( gtk_vscale_new_with_range( lo, hi, step ));
	g_signal_connect(
		G_OBJECT (get_adjustment() ),
		"value_changed",
		G_CALLBACK( C_event_value_changed ),
		// (SignalFunc) C_event_value_changed,
		this
	);
#else
	GtkAdjustment * adj = (GtkAdjustment *) gtk_adjustment_new( 0, lo, hi, step, step, 0 );
	if(0) gtk_object_ref( GTK_OBJECT(adj) );
	if( horiz )
		w_set( gtk_hscale_new( adj ));
	else
		w_set( gtk_vscale_new( adj ));
	gtk_signal_connect(
		(GtkObject *) adj,
		"xvalue_changed",
		(GtkSignalFunc) C_event_value_changed,
		this
	);
#endif
}

/*!
	how many digits to show in the value
*/
void w_scale::set_digits( int digits )
{
	gtk_scale_set_digits( Scale(), digits );
}

/*!
	do or dont draw the VALUE
*/
void w_scale::set_draw_value( bool show )
{
	gtk_scale_set_draw_value( Scale(), show );
}

/*!
	where the value digits are drawn (top)
*/
void w_scale::set_value_pos( GtkPositionType pos )
{
	gtk_scale_set_value_pos( Scale(), pos );
}

/*!
	returns the number of decimal places in the value
*/
int w_scale::get_digits()
{
#ifdef WITH_GTK2
	return gtk_scale_get_digits( Scale() );
#else
#warning "GTK1 doesnt have gtk_scale_get_digits"
	e_print("w_scale::get_digits() dummy()\n");
	return 1;
#endif
}

/*!
	When user clicks on slider, gtk auto changes Ajdustment.

	When prog calls set_value() Adjustment gets changed

	When Adjustment get changed value_changed gets emitted

	TODO: rename event_value_changed()

	NB I only get integer values ... though floats shown on screen
	for tv_remote slider.

*/
// static
void w_scale::C_event_value_changed(
	GtkAdjustment *adj,
	w_scale * scale
)
{
	scale -> event_value_changed( adj->value ) ;
}

/*!
	VIRTUAL - slider has moved, tell app to do something
*/
void w_scale::event_value_changed( double val )
{
	e_print("A_Scale at %f\n", (float) val );
}

/*!
	Change slider (and maybe app?) to show new value
*/
void w_scale::set_value( double val )
{
#ifdef WITH_GTK2
	gtk_range_set_value( Range(), val );
#else
	gtk_adjustment_set_value(  get_adjustment(), (gfloat) val );
#endif
}

/*!
	get value from the Range/adjustment
*/
double w_scale::get_value()
{
#ifdef WITH_GTK2
	return gtk_range_get_value( Range() );
#else
	GtkAdjustment * adj = get_adjustment();
	return adj->value;
	// return gtk_adjustment_get_value( get_adjustment() );
#endif
}


