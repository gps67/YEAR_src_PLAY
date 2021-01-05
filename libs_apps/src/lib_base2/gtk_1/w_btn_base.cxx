#include "w_btn_base.h"
#include "dgb.h"

/*!
	The C --> C++ vector
*/
void w_btn_base::C_event_clicked( 
	GtkWidget * _w,
	w_btn_base * self
) {
	obj_hold<w_btn_base> keeper = self; // stack ref in case of self-destruct !

	if(0) STEP("OK");
	self->debug_track_pointer();
	self->event_clicked();
}

/*!
	VIRTUAL - do the buttons action
*/
void w_btn_base::event_clicked()
{
	e_print("w_btn_plus::event_clicked() - app should define\n" );
	// derived class does somethig here
}

/*!
	set xalign property of the child label
*/
void w_btn_base::set_xalign( float h )
{
	// NO // gtk_misc_set_alignment( GTK_MISC(GTK_BUTTON(w)->child), 0.0, 0.5);
	// OK // gtk_misc_set_alignment( GTK_MISC(GTK_BIN(w)->child), h, v );
	gtk_object_set( (GtkObject*) CHILD_LABEL(), "xalign", h , NULL );              
}

/*!
	set yalign property of the child label
*/
void w_btn_base::set_yalign( float v )
{
	gtk_object_set( (GtkObject*) CHILD_LABEL(), "yalign", v , NULL );
}


// --------------------------------------------


