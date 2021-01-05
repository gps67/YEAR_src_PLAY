#ifndef w__window_H
#define w__window_H

#include "w_container_.h"
#include "obj_hold.h"
#include <gtk/gtkwidget.h>

extern "C" {
	struct xGtkWindow;
};

/*!
	w_window is a base class for w_win_top which calls new TOPLEVEL,
	and other things like app->callbacks(). Hence a bit slim here.
*/
class w_window : public w_container_1
{
 public:

	GtkWindow* Window() { return GTK_WINDOW( w ); }

	~w_window()
	{
	}
	w_window()
	{
	}
/*
	// following failed for w_dialog - why??
	w_window( GtkWindow * w1 )
	: w_widget0( GTK_WIDGET( w1 ))
	{
	}
*/
	void set_title( str0 title )
	{
		gtk_window_set_title( Window(), title );
	//	gtk_window_set_title( (GtkWindow *) w, title );
	}

	void set_title_upper( str0 title );

	void raise_window()
	{
		if(!w) return;
		gdk_window_raise( w->window );
	}

	void w_window_dismantle()
	{
		remove_all();
	}

	void hide()
	{
		// gtk_widget_hide( w->window );
		gtk_widget_hide( w );
	}

	void hide_window_now_until_idle()
	{
		// clever trick to hide a window IMMEDIATELY
		// and it reappears when gtk is idle

		// I use this within a callback
		// hide the window NOW
		// force it to queue jump with unrealise
		// or maybe stop errors by calling hide()
		// then call show() - which will be queued
		// the only other thing that worked immediately was destroy
		// but this regrows, on next idle loop

		//
		// you have to hide and show otherwise you get
		// Gdk-CRITICAL **: gdk_window_set_geometry_hints:
		// assertion `GDK_IS_WINDOW (window)' failed
		//
		// no need for unmap process_updates or realise
		// realise() called when next idle loop
		// the components are still there!
		// maybe its a bones taken out of the body
		// and the next realise, does what is initially done
		// but its not obvious, so suprising when it happens again

		// if this starts mis-behaving, hide somewhere else
		// eg single threaded GUI -vs- 

		// not sure what happens with sub-widgets, all of top for now

		gtk_widget_hide(w); // queued
		gtk_widget_unrealize(w); // pushes hide() through NOW
		gtk_widget_show(w); // queued for when almost idle
	}

	bool set_icon_from_file( const char * filename );

};

#endif
