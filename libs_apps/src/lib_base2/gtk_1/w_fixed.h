#ifndef w_fixed_H
#define w_fixed_H

#include "w_widget0.h"
#include "obj_hold.h"
#include "w_window.h"

#include "w_container_.h"
#include "w_frame.h" // w_gtk_bin

/*!
	
*/
class w_fixed : public w_gtk_bin
{
 public:
	GtkFixed * Fixed() { return (GtkFixed*)w; }
	~w_fixed();
	w_fixed();
//	w_fixed( str0 label );
	void put( w_widget0 * child, int x, int y );
	void move( w_widget0 * child, int x, int y );

	bool has_window();
	void set_has_window( bool own = true );

};

#endif
