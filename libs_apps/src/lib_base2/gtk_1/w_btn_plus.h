#ifndef w_btn_plus_H
#define w_btn_plus_H

#include "w_vbox.h"
#include "w_win_scrolled.h"


/*!
	Most buttons are w_btn_plus, with event_clicked.
	Maybe all should be (early design, led to SELF
	getting most events).

	A btn with a virtual callback to itself

	There are further issues with references,
	eg sar_plot_layout <--> mem_line_tree_node <--> w_btn_sar

	There are further issues with more events - particularly
	actions that take some time to complete, busy or disabled.

*/
class w_btn_plus : public w_btn_base
{
 public:

	w_btn_plus( str0 label );

  virtual
	~w_btn_plus();

};

#endif
