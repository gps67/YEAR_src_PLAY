#include "w_btn_plus.h"

/*!
	constructor
*/
w_btn_plus::w_btn_plus( str0 label )
: w_btn_base( label, this, (SignalFunc) C_event_clicked )
{


// it was always zero
	static bool done = false;
	if(!done)
	{
		done = true;
		int wid = gtk_container_get_border_width( (GtkContainer*)w );
		e_print( "*** gtk_container_get_border_width() == %d \n", wid );
#if 0
		/*
			YES a GtkButton is a container (and has a border)
		*/
#warning no such property for GtkButton.
		GtkBorder * bdr = (GtkBorder *)get_obj_prop( "default-border" );
		if( bdr )
		e_print( "*** left = %d top = %d\n", bdr->left, bdr->top );
#endif
	}

	gtk_container_set_border_width( (GtkContainer*)w, 0 );

	if(0) e_print("w_btn_plus() %s\n", (STR0) label );
}

/*!
	destructor - plain
*/
w_btn_plus::~w_btn_plus()
{
	if(0) {
		e_print("#~w_btn_plus() w in DCTR\n" );
		// get_text raises error message
		// see sar_top_layout::empty_from_depth()
		return;
	}
	if(!w) {
		/*
			btn can be remove_all un-packed and w set to NULL
			then another holder (but not packer) release,
			eg btn that deletes its own container
		*/
		// e_print("# ?? # ~w_btn_plus() w is NULL\n" );
		return;
	}
	if(0) e_print("~w_btn_plus() %s\n", (STR0) get_text() );
	// destroy does this // signal_disconnect_by_data();
}
