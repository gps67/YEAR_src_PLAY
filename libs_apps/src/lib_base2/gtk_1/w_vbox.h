#ifndef w_vbox_H
#define w_vbox_H

#include "obj_hold.h"
#include "obj_list.h"
#include "w_btn_base.h" // some user needs this - remove it later

#include "w_container_.h"

#define CHECK_NULL_SELF if(!self) return

// class frame_set_app;
// typedef void (btn_4_func)( GtkWidget *, obj_ref *);
// doxygen says: Warning: member with no name found.
// typedef void (btn_4_func)( GtkWidget *, frame_set_app *);

/*!
*/
class w_box : public w_container_N
{
 public:

	~w_box()
	{
		remove_all();
	}
	w_box( GtkBox * W0 ) { w_set( (GtkWidget *) GTK_BOX(W0)); }
	w_box( ) {}
	GtkBox * Box() { return GTK_BOX(w); }
	// GtkContainer * Container() { return GTK_CONTAINER(w); }

	void pack_start(
		w_widget0 * sub,
		gboolean expand = FALSE,
		gboolean fill = TRUE,
		guint padding = 0
	) {
		gtk_box_pack_start(
			Box(),
			sub->outer_packable(),
			expand,
			fill,
			padding
		);
		packed( sub );
		sub->show();
	}

	/*!
		_end is last, bottom, right, of vbox, hbox
	*/
	void pack_end(
		w_widget0 * sub,
		gboolean expand = FALSE,
		gboolean fill = TRUE,
		guint padding = 0
	) {
		gtk_box_pack_end(
			Box(),
			sub->outer_packable(),
			expand,
			fill,
			padding
		);
		packed( sub );
		sub->show();
	}

	void pack( w_widget0 * sub )
	{
		pack_big(sub);
	}

	void pack_big( w_widget0 * sub )
	{
		pack_start(sub, TRUE, TRUE, 0);
	}

	void pack_asis( w_widget0 * sub )
	{
		pack_start(sub, FALSE, TRUE, 0);
	}

	void pack_expand( w_widget0 * sub )
	{
		pack_start(sub, TRUE, FALSE, 0);
	}

	void pack_fill( w_widget0 * sub ) // same as asis
	{
		pack_start(sub, FALSE, TRUE, 0);
	}

	/*
		If you dont FILL, the extra space remains unused, untidy

		If you do FILL, any leftover space is used

		If you EXPAND, the widget
	*/


};

/*!
*/
class w_hbox : public w_box
{
 public:
	w_hbox(
		bool equal = FALSE,
		int spacing = 0
	) {
			w_set( gtk_hbox_new( equal, spacing ));
	}

	w_hbox( GtkBox * W0 ): w_box(W0) {}

	static w_hbox * new_w_hbox()
	{
		w_hbox * h = new w_hbox(); 
		h->w=gtk_hbox_new(FALSE, 0);
//		h->set_border_width(5);
		return h;
	}
};


/*!
*/
class w_vbox : public w_box
{
 public:
	w_vbox( GtkBox * W0 ): w_box(W0) {}
	w_vbox(
		bool equal = FALSE,
		int spacing = 0
	) {
		w_set( gtk_vbox_new( equal, spacing ));
	}
	static w_vbox * new_vbox()
	{
		w_vbox * v = new w_vbox(FALSE, 0); 
	//	v->set_border_width(5);
		return v;
	}
};

#endif
