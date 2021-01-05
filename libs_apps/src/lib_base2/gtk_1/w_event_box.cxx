#include "w_event_box.h"

/*
	////////////////////////////////////////
	w_event_box 
	////////////////////////////////////////
*/
w_event_box:: w_event_box()
{
	w_set( gtk_event_box_new());
}

void w_event_box:: set_visible_window( bool visible_window ) {
	return gtk_event_box_set_visible_window( EventBox(), visible_window );
}

void w_event_box:: set_above_child( bool above ) {
	//
	return gtk_event_box_set_above_child( EventBox(), above );
}

bool w_event_box:: get_above_child() {
	return gtk_event_box_get_above_child( EventBox() );
}

// move this to container_1 ??
bool w_event_box:: set_child( w_widget0 * _child ) { 
	pack_solo( _child);
	return true;
}
w_widget0 * w_event_box:: get_child() { 
	return get_sub_kept();
}

bool w_event_box:: child_must_not_be_null() {
	if(!child_is_null() ) return true;
	FAIL("NULL");
	THROW_dgb_fail("NULL");
	return false;
}


