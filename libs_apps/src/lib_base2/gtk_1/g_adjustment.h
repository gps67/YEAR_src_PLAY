#ifndef g_adjustment_H
#define g_adjustment_H

#include "w_widget0.h"

/*!
	g_adjustment is a float usd by scroll bars and spin-buttons (?).

	Note that _silent IS used!
*/
class g_adjustment : public obj_ref
{
	GtkObject * obj;
 public:

	~g_adjustment();

	g_adjustment(
		gdouble val,
		gdouble lo,
		gdouble hi,
		gdouble one,
		gdouble many,
		gdouble page
	);

	static gint C_event_value_changed(
		GtkAdjustment * self_base,
		g_adjustment * self
	);

	gdouble get_val();

	void set_val( gdouble val );

	GtkAdjustment * Adjustment();
  virtual
	void event_value_changed();

};

/*!
	g_adjustment_silent throws away the events (except for
	debugging!), and is used when the GUI event is used,
	and the GLIB event is ignored -

	The g_adjustment gets event_value_changed(v)
	the w_scale gets event_value_changed(v)
*/
class g_adjustment_silent : public g_adjustment
{
 public:
	g_adjustment_silent(
		gdouble val,
		gdouble lo,
		gdouble hi,
		gdouble one,
		gdouble many,
		gdouble page
	);
  virtual
	void event_value_changed();
};

#endif
