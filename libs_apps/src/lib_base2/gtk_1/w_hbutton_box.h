#ifndef w_hbutton_box_H
#define w_hbutton_box_H

#include "w_container_.h"
#include "dgb.h"

/*
	Horizontal box of equally spaced buttons
*/
class w_hbutton_box : public  w_container_N
{
 public:
	~w_hbutton_box()
	{
	}

	w_hbutton_box()
	{
		w_set( gtk_hbutton_box_new ());
		set_layout_spread();
	}

	bool set_layout( GtkButtonBoxStyle layout )
	{
		if(!w) return FAIL("null w");
		gtk_button_box_set_layout(GTK_BUTTON_BOX (w), layout);
		return true;
	}
	bool set_layout_spread()
	{
		return set_layout( GTK_BUTTONBOX_SPREAD );
	}

	bool set_layout_edge()
	{
		return set_layout( GTK_BUTTONBOX_EDGE );
	}

	bool pack_asis( w_widget0 * sub )
	{
		pack( sub ); // which is a wrap around add()
		return true;
	}

};


#endif

