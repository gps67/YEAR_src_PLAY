#ifndef w_menu1_H
#define w_menu1_H

#include "w_widget0.h"
#include "obj_list.h"

/*!
*/
class w_menu_item : public w_widget0
{
 public:
	GtkMenuItem * MENU_ITEM() { return (GtkMenuItem *) w; }
	w_menu_item(
		str0 s,
		 SignalFunc func,
		obj_ref * app
	); 
	void right_justify();
};

/*!
*/
class w_menu : public w_widget0
{
//	obj_list<w_menu_item> ref_keeper;
	obj_list<w_widget0> ref_keeper;
 public:
	GtkMenu * pop_up;
	w_menu( str0 s );
//	void append( w_menu_item * item );
	void append( w_widget0 * item );
	w_menu_item * mk_item(
		str0 s,
		 SignalFunc func,
		obj_ref * app
	);
};

/*!
*/
class w_menu_bar : public w_widget0
{
//	obj_list<w_menu> ref_keeper;
	obj_list<w_widget0> ref_keeper;
 public:
	w_menu_bar();
	void append( w_menu * root_menu );
	w_menu * mk_menu( str0 s);
	w_menu * mk_menu_right( str0 s);
// VIRTUAL 
	void call_destroy();

};
#endif
