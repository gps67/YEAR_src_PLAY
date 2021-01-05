#ifndef c_tree_tbl_two_H
#define c_tree_tbl_two_H


// based on a table c_tree
// this adds a link to app and specific handlers

#include "c_tree_tbl_one.h"
#include "nb_page_tbl.h" 	// strange signal routing

namespace ACCTS {

// class top_app;

/*!
	c_tree_tbl_two adds the link to the notebook page "up"
	that owns this ctree, and catches signals that need
	to be relayed to it.

		 nb_page_tbl * up;
*/
class c_tree_tbl_two : public c_tree_tbl_one
{
 public:
	nb_page_tbl * up;
	~c_tree_tbl_two();

	c_tree_tbl_two( nb_page_tbl * _app, table * _tbl, int treecol );

 // virtual
	void event_click_column();

 // virtual
	void event_tree_select_row();

};

}; // NAMESPACE

#endif
