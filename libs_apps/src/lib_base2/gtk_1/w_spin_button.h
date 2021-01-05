#ifndef w_spin_button_H
#define w_spin_button_H

/*
	could have been dual inherit
	could be ... lots of things ...
*/
#include "w_widget0.h"
#include "g_adjustment.h"

/*!
*/
class w_spin_button : public w_widget0
{
	obj_hold<g_adjustment> adj;
 public:
	w_spin_button( g_adjustment * _adj )
	: adj( _adj)
	{
		w_set( gtk_spin_button_new( adj->Adjustment(), 1,0 ));
	}
};

#endif
