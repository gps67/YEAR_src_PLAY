#ifndef c_tree_tbl_one_H
#define c_tree_tbl_one_H

#include "w_ctree.h"
#include "table.h"

namespace ACCTS {

// based on a c_tree (which isnt really a 'tbl')
// this adds table and row_inst

//! Merges a generic c_tree_base and the apps table
/*!
	c_tree is a Gtk wrapper with a few extras but no knowledge of the app.
	This class adds the link to the data table, and the ability
	to present the data sorted and grouped.

	The app uses a sub-class of this to act on signals.

	A lot of work is done by tree_build_1.
*/
class c_tree_tbl_one : public w_ctree
{
 public:
	//! The data being viewed
	obj_hold<table>	tbl;

	//! The selected columns can be in any order, omit some
	obj_list<col_spec> col_list;

	//!rebuild the col_list from the tbl
	virtual void build_col_list();

	//! constructor - with one col selected for sorting
	c_tree_tbl_one( table * _tbl, int treecol );

	//! rebuild the GUI tree from the data_rows
	void reload_data();

	//! build the strs[] for this row
	void prep_data_row( row_inst * r );

	//! build the strs[] for a group row, not a data row - (tree node not leaf)
	void prep_group_row( col_spec * tree_spec, row_inst * row, str2 * astr );

	//! which row is currently selected ?
	row_inst * get_sel_row_inst();

	//! creator needs to find tree col, by visible position, -1 = NONE
	int find_visible_col( col_spec * spec );

	// event_click_column - serived class uses

};

}; // NAMESPACE

#endif
