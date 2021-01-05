#ifndef w_btn_base_H
#define w_btn_base_H

#include "obj_hold.h"
#include "w_widget0.h"

// #define CHECK_NULL_SELF if(!self) return

// class frame_set_app;
// typedef void (btn_4_func)( GtkWidget *, obj_ref *);
// typedef void (btn_4_func)( GtkWidget *, frame_set_app *);

//doxygen says: Warning: member with no name found.

typedef void (FN_btn)( GtkWidget *, w_widget0 *);

/*!
	All buttons are based on this.

	TODO: Currently I only do boring text buttons, I really must
	sort out the interface for multiple things inside the CHILD.

	Starting with the fact that a GtkButton is a Container (has border)
	so rebase this class?
*/
class w_btn_base : public w_widget0
{
 public:
	/*!
		Constructor - label but no action (its a label)
	*/
	w_btn_base( str0 label) {
		w_set( gtk_button_new_with_label( label ));
		set_align_left();
	}

	/*!
		Constructor - label and action (This is the norm)
	*/
	w_btn_base( 
		str0 label,
		obj_ref * app,
		SignalFunc fn
	)
	{
		w_set( gtk_button_new_with_label( label ));
		set_align_left();
		connect_clicked( fn, app );
	};

	/*!
		Connect a C callback to the clicked signal
	*/
	void connect_clicked(
		SignalFunc fn,
		obj_ref * app
	)
	{
		connect( "clicked", fn, app );
	}

	/*!
		Connect the default event_clicked()
	*/
	void connect_event_clicked()
	{
		connect_clicked( (SignalFunc) C_event_clicked, this );
	}

	/*!
		C --> C++ callback
	*/
  static
	void C_event_clicked( 
		GtkWidget * _w,
		w_btn_base * self
	);

	/*!
		The action callback
	*/
  virtual
	void event_clicked();

	//! return the GTK widget 
	GtkButton * BUTTON()
	{
		return GTK_BUTTON( w );
	}

	//! return the GTK outside widget (default outer_packable)
	GtkBin * BIN()
	{
		return GTK_BIN( w );
	}

	//! return the GTK interior widget (TODO: pack graphics and stuff)
	GtkWidget * CHILD()
	{
		return BIN()->child;
	}

	//! return the GTK interior widget as a LABEL
	GtkLabel * CHILD_LABEL()
	{
		return GTK_LABEL( CHILD() );
	}

	void set_xalign( float h ); //!< align x start at pos [0[1
	void set_yalign( float v ); //!< align y start at pos [0[1

	void set_align_left() { set_xalign( 0.0 ); } //! how text is aligned
	void set_align_mid() { set_xalign( 0.5 ); } //! how text is aligned
	void set_align_right() { set_xalign( 1.0 ); } //! how text is aligned

	//! change the text
	void set_text( str0 s )
	{
		set_prop( "label", s );
		set_align_left(); // lost
	}

	//! get the label text
	str0 get_text()
	{
		return get_str_prop( "label" );
	}

/*
	void self_destruct()
	{
		fprintf(stderr,"self_destruct - should never be called\n");
		destroy();
	}
*/
};
#endif
