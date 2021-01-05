#ifndef w_win_scrolled_H
#define w_win_scrolled_H

#include "w_container_.h"
#include "obj_hold.h"

/*!
	A scrolled window holds a self-scrolling widget,
	or an assisted scrolling widget
*/
class w_win_scrolled : public w_container_1
{
 public:

	~w_win_scrolled() ;

	 w_win_scrolled(
		// GtkPolicyType h = GTK_POLICY_AUTOMATIC,
		// GtkPolicyType v = GTK_POLICY_ALWAYS
		GtkPolicyType h = GTK_POLICY_AUTOMATIC,
		GtkPolicyType v = GTK_POLICY_AUTOMATIC
	) ;

	GtkScrolledWindow * Scrolled_Window()
	{
		return GTK_SCROLLED_WINDOW (w);
	}

	void get_canvas_size( int & w_width,  int & w_height ) ;
	void pack_unscrollable( w_widget0 * sub ) ;
	void event_destroyed();
 virtual
	void event_size_request( GtkRequisition *requisition );


};

#endif
