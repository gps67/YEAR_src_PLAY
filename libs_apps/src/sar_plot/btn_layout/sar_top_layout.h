#ifndef sar_plaot_layout_H
#define sar_plaot_layout_H

#include "w_vbox.h"
#include "btn_col.h"
// #include "w_hbox.h"
#include "mem_line_tree.h"

// #include "w_btn_plus.h"

class dset_api; // #include "dset_api.h"

class sar_top_app;
class btn_sar_layout;

class sar_top_layout : public w_hbox
{
	friend struct btn_sar_layout;	// a single btn in this layout

	/*
		The btn created now belong to the w_container_N baseclass
		accessed via 'keeper' which contains:

			w_vbox "col_1"
				w_frame
					site[*]
				w_frame
					host[*]
				w_frame
					dt[*]
			w_frame
				attr4[*]
			w_frame
				attr5[*]

	*/

	//! cols is used to find attr[i] - beware it holds a reference
	obj_list<w_widget0> cols;

	w_vbox * col_1;

	//! layout  OWNS  the tree of btn_nodes 
	obj_hold<mem_line_tree> line_tree;

	//! sar_top_app OWNS THIS - weak link back up tree
	sar_top_app * top_app;

	// circular .h
	void tell_top_app_layout_selected_leaf( mem_line_spec * spec );
	static void C_del1( GtkWidget *wid, gpointer data );
	void empty_from_depth( int col );
	void do_single_depth( mem_line_tree_node * node0 );
	void redo_subtree( mem_line_tree_node * node0 );

 public:
	sar_top_layout( sar_top_app * _top_app );
	~sar_top_layout();
	void dismantle();

	void set_line_tree_from_dset( dset_api * dset );

};

#endif
