#ifndef v_paned_H
#define v_paned_H

#include "w_widget0.h"
#include "w_container_.h"
#include "obj_hold.h"

/*!
*/
class w_xpaned : public w_container_
{
 public:
	GtkPaned * Paned() { return (GtkPaned *) w; }
	obj_hold<w_widget0> panelA;
	obj_hold<w_widget0> panelB;
	w_xpaned()
	{
		panelA = NULL;;
		panelB = NULL;;
	}
	~w_xpaned()
	{
		dismantle();
	}
	
	void dismantle()
	{
		remove_sub( panelA );
		remove_sub( panelB );
	}
	void pack1( w_widget0 * A, bool resize, bool shrink )
	{
		if(!A) return;
		panelA = A;
		gtk_paned_pack1( Paned(), A->w, resize, shrink );
		panelA->show();
	}
	void pack2( w_widget0 * B , bool resize, bool shrink )
	{
		if(!B) return;
		panelB = B;
		gtk_paned_pack2( Paned(), B->w, resize, shrink );
		panelB->show();
	}
};


/*!
*/
class w_vpaned : public w_xpaned
{
 public:
	w_vpaned()
	: w_xpaned()
	{
	}
	~w_vpaned()
	{
		dismantle();
	}

	void late_init(
		w_widget0 * A,
		w_widget0 * B,
		bool big_A = true
	)
	{
		w_set( gtk_vpaned_new());
		if( big_A) {
			pack1( A, TRUE, FALSE );
			pack2( B, FALSE, FALSE );
		} else {
			pack1( A, FALSE, FALSE );
			pack2( B, TRUE, FALSE );
		}
	}
	w_vpaned(
		w_widget0 * A,
		w_widget0 * B,
		bool big_A = true
	)
	: w_xpaned()
	{
		late_init( A, B, big_A );
	}
};

/*!
*/
class h_paned : public w_xpaned
{
 public:
	h_paned() : w_xpaned() { }
	void late_init(
		w_widget0 * A,
		w_widget0 * B
	)
	{
		w_set( gtk_hpaned_new());
		pack1( A, TRUE, FALSE );
		pack2( B, FALSE, FALSE );
	}
	
	h_paned(
		w_widget0 * A,
		w_widget0 * B
	)
	: w_xpaned()
	{
		late_init( A, B );
	}
	~h_paned()
	{
		dismantle();
	}
};

#endif
