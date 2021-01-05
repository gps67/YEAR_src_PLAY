#ifndef w_frame_H
#define w_frame_H

#include "w_widget0.h"
#include "obj_hold.h"
#include "w_window.h"

#include "w_container_.h"

/*!
	sub class MUST detatch the contained object
*/
class w_gtk_bin : public w_container_1
{
 public:
	// BASE class - no constructor ...
	GtkBin * Gtk_Bin() { return GTK_BIN( w ); }
};

/*!
*/
class w_frame : public w_gtk_bin
{
 public:
	~w_frame()
	{
		debug_track_pointer();
		remove_all();
	}
	w_frame( str0 label )
	{
		w_set( gtk_frame_new( label ));
	}
};

#endif
