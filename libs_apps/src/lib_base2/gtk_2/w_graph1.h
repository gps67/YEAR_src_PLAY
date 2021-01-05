#ifndef w_graph1_H
#define w_graph1_H

#include "w_drawing_area.h"

/*!
*/
class w_graph1 :  public w_drawing_area
{
 public:
	w_graph1();
	~w_graph1();
  virtual
	void resize_configure();
	void size_request( GtkRequisition *requisition );
};

#endif
