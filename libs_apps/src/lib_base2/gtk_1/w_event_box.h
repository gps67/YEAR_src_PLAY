#ifndef w_event_box_H
#define w_event_box_H

#include "dgb_fail.h"
#include "w_container_.h"
#include "w_frame.h"
#include "w_fixed.h"

/*!
	////////////////////////////////////////
	w_event_box 
	////////////////////////////////////////
	an actually realised window that can receive CONFIGURE (and other) events
*/
class w_event_box : public w_container_1
{
 public:
	GtkEventBox * EventBox() { return GTK_EVENT_BOX(w); }

	w_event_box();

	void set_visible_window( bool visible_window = true );
	void set_above_child( bool above );
	bool get_above_child();

	// move this to container_1 ??
	bool set_child( w_widget0 * _child );
	w_widget0 * get_child();
	bool child_must_not_be_null();

	bool child_is_null() { return NULL==get_child(); }
	void set_above_child_false() {
		// this is the normal situation
		set_above_child(false);
	}
};

#endif
