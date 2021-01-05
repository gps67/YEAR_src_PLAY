#ifndef w_graph3_H
#define w_graph3_H

#include "w_graph2.h"

class sar_top_app;

/*!
	w_graph3 is a graph2 for use with sar_top_app

	It passes the crosshairs position back
*/
class w_graph3 : public w_graph2
{
 public:
	sar_top_app * owner; // WEAK ref to the owner of this layout
/*
 virtual
	void cross_hairs_at( time_t tval, flt80  yval );
*/
 virtual
	void event_mouse_moved(int x, int y);

	w_graph3( sar_top_app * _owner );

};

#endif
