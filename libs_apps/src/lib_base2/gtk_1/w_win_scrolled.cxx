
#include "w_win_scrolled.h"
#include "dgb.h"

/*!
*/
w_win_scrolled::~w_win_scrolled()
{
	remove_all();
}

/*!
*/
w_win_scrolled::w_win_scrolled(
	GtkPolicyType h,
	GtkPolicyType v
) {
	errno_zero();
	STEP("HERE");
	w_set( gtk_scrolled_window_new(
		(GtkAdjustment *)NULL,
		(GtkAdjustment *)NULL
	));
	gtk_scrolled_window_set_policy (
		GTK_SCROLLED_WINDOW (w),
		h, // GTK_POLICY_AUTOMATIC,
		v // GTK_POLICY_ALWAYS
	);
	connect_event_size_request();
	errno_zero();
	STEP("DONE");
}

/*
	INLINE
	GtkScrolledWindow * Scrolled_Window()
	{
		return GTK_SCROLLED_WINDOW (w);
	}
*/

/*!
*/
void w_win_scrolled::event_destroyed()
{
	// gdb_break_point();
	w_set_NULL();
	remove_all();
}

/*!
*/
void w_win_scrolled::get_canvas_size( int & w_width,  int & w_height )
{
	w_widget0 * child = get_sub_kept();
	if(child)
		child->get_size( w_width, w_height );
	else
	{
		w_width = 0;
		w_height = 0;
	}
}

/*!
*/
void w_win_scrolled::pack_unscrollable( w_widget0 * sub )
{
	gtk_scrolled_window_add_with_viewport(
		GTK_SCROLLED_WINDOW(w),
		sub->outer_packable()
	);
	sub->show();
	packed( sub );
}

/*!
	gtk is asking this widget for its size request,

	ask the child for its request, and use the width,
	The height is left loose, so the parent should
	provide a load, that is then used.

	This is the sort of stupidity that Gtk fails to solve,
	along with the viewport vidget. The code is written anyway,
	but you have to juggle the outer-packable, etc

	It probably stems from the way other people dont have
	a scrollable column of buttons, only using scrollbars
	on pages, which has the viewport code in anyway.

	TODO: why is the scrollbar 25 allowance 25?

	NEWS: pygtk with gtk.POLICY_NEVER causes the widget
	to be full width, unfortunately AUTOMATIC is too minimum

*/
void w_win_scrolled::event_size_request( GtkRequisition *requisition )
{
	// i dont see why the basic scrolled widget was as thin as the scrollbars
	int h0;
	int w0;
	// get_canvas_size( requisition->width,  requisition->height );
	get_canvas_size( w0,  h0 );
	requisition->width = w0 + 25;
	// requisition->height = h0;
}

