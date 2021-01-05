#ifndef w_scale_H
#define w_scale_H

#include "w_widget0.h"                                                          
#include "g_adjustment.h"                                                          

/*!
	problems with g_adjustment - both get event_value_changed(...)

	This base class needs another (derived) class to hold the
	g_adjustment. I didnt sort this out properly, and getting
	angry with the inverted slider design, I fled when it stopped
	failing, and havent been back. (Its probably obvious).

	This can be either horizontal or vertical, and is the GUI aspect.
	The stored data is elsewhere (several controls might change one
	changeable). see "w_scale_fix" in tv_panel.h (move that here?)
*/
class w_scale : public w_widget0
{
 public:
	GtkScale * Scale() { return (GtkScale *) w; }
 // GtkRange is a base class of GtkScale
	GtkRange * Range() { return (GtkRange *) w ; }

	//! get the GTK changeable data (glib)
  virtual
	GtkAdjustment * get_adjustment();

  virtual
	~w_scale();

	w_scale( bool horiz, g_adjustment * adj );
	
	w_scale(
		bool horiz,
		double lo,
		double hi,
		double step
	) ;
	void set_digits( int digits );
	int get_digits();
	void set_draw_value( bool show );
	// bool get_draw_value()
	// get_value_pos()
	void set_value_pos( GtkPositionType pos );
	/*
		These make the caller independent of GTK names
	*/
	void set_value_pos_left()   { set_value_pos( GTK_POS_LEFT ); }
	void set_value_pos_right()  { set_value_pos( GTK_POS_RIGHT ); }
	void set_value_pos_top()    { set_value_pos( GTK_POS_TOP ); }
	void set_value_pos_bottom() { set_value_pos( GTK_POS_BOTTOM ); }
  static
	void C_event_value_changed(
		GtkAdjustment *adj,
		w_scale * scale
	);
  virtual
	void event_value_changed( double val );
	void set_value( double val );
	double get_value();
	void set_inverted(bool corrected = TRUE ); // which IS the right way up

};

#endif
