#include "w_graph1.h"
#include "dgb_fail.h"

/*!
*/
w_graph1::w_graph1()
{
	connect_event_size_request();
}

/*!
*/
w_graph1::~w_graph1()
{
}

/*!
*/
void w_graph1::size_request( GtkRequisition *req )
{
	if( req->height < 76 ) {
		req->height = 76;
	}
}

/*!
	I CANT SEE WHO CALLS resize_configure !!!

	resize_configure is sent AFTER the negociation, with the RESULT.

	You are supposed to (only) allocate the relevent pixmap, but this
	also generates a new size request.
*/
void
w_graph1::resize_configure()
{
	for(int i =0; i<5; i++ ) INFO("WHO CALLS THIS");
	debug_track_pointer();
	INFO("width() height() == (%d,%d)\n", width(), height());
	static int loop_detect = 0;
        reallocate_pixmap();

	// this shows that size_request worked!
	if( loop_detect ) {
		loop_detect = 0;
	} else {
		int h1 = height();
		int h2 = h1;
		if( h1 < 75 ) {
			h2 = 75;
		} else if( h1 > 750 ) {
			h2 = 750;
		}
		if( h1 != h2 ) {
			e_print("################################### \n" );
			e_print("################################### \n" );
			e_print("w_graph1::resize_configure: h2 = %d \n", h2);
			e_print("################################### \n" );
			e_print("################################### \n" );
			set_size( width(), h2 );
		}
	}
}

