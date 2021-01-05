#ifndef w_dialog_message_H
#define w_dialog_message_H

#include "w_widget0.h"
#include "w_win_top.h" // to gtk a dialog is a top win, with some similar funcs
#include "w_vbox.h" // to gtk a dialog is a top win, with some similar funcs

// docs and versions - buttons do nothing here !!

/*!
	w_dialog is a dialog box

	Its a bit odd as the code isnt retro-fitted to GTK1,
	and grew from some strange origins. (TODO: Rewrite)

*/
class w_dialog: public w_window
{
 public:
	obj_hold<w_vbox> vbox;		//!< same name as gtk
	obj_hold<w_hbox> action_area;	//!< same name as gtk

	//! underlying gtk GObject
	GtkDialog* Dialog() { return GTK_DIALOG( w ); }

	static void C_destroy_counting(
		GtkWidget * _w,
		w_dialog * self
	) ;

	~w_dialog() ;
	w_dialog() ;
//	virtual void destroy();
	void set_transient_for( w_widget0 * mainwin ) ;
	void set_default_response( int resp_id ) ;
	GtkWidget * add_message( str0 str ) ;
	GtkWidget * add_button( bool deflt, str0 str, int resp_id ) ;
	GtkWidget * add_button_ok( bool deflt = FALSE, str0 str = "ok" ) ;
	GtkWidget * add_button_cancel( bool deflt = FALSE, str0 str = "cancel" ) ;
	int run() ;
	static void msg_failed(
		w_widget0 * parent,
		str0 title,
		str0 message
	) ;
};

#endif
