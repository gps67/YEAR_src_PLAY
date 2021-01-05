#ifndef w_container__H
#define w_container__H

#include "w_widget0.h"
#include "obj_hold.h"
// #include "w_window.h"
#include "obj_list.h"

/*!
	container_ is a partially complete (hence the _) baseclass
	that introduces the virtual remove_all() ... API of containers

	Caller must track appends/deletes to match refcounts.

	TODO:
		w_table should be a container

	HISTORY:
		there was some problem with GTK widgets being deleted
		when C++ objects still point to them (vice-verse).
		This is supposed to be fixed, but remnets of old code might
		remain.
*/
class w_container_ : public w_widget0
{
 public:

	w_container_();
	~w_container_();

// C //	struct GtkContainer;
	GtkContainer * Container();

/*
	Try to stop unwary callers using these thinking they are complete
	Direcly derived classes must do this (hence no w_widget * parameters)
*/
	void add( GtkWidget * sub );
	void remove( GtkWidget * sub );
	void remove_sub( obj_hold<w_widget0> & sub );
	int get_border_width();
	void set_border_width(int wide);
	virtual void remove_all();
	void event_destroyed();
	void w_set( GtkWidget * sub );
};

/*!
	w_container_1 holds a single child widget
*/
class w_container_1 : public w_container_
{
	obj_hold<w_widget0> sub_kept; // keep alive
 public:
	~w_container_1();
	w_container_1();
	void pack_solo( w_widget0 * sub );
	void pack_full( w_widget0 * sub ) { pack_solo( sub ); }
	void pack( w_widget0 * sub ) { pack_solo( sub ); }
	void packed( w_widget0 * sub );
	virtual void remove_all();
	w_widget0 * get_sub_kept();
};

/*!
	w_container_N holds a list of widgets
*/
class w_container_N : public w_container_
{
 public:
	obj_list<w_widget0> keeper;
 public:
	~w_container_N();
	w_container_N();
	//! derived class did the packing
	void packed( w_widget0 * sub );
	void pack( w_widget0 * sub );
	void remove( w_widget0 * sub );
	virtual void remove_all();
};

#endif
