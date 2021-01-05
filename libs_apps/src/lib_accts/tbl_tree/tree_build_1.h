#ifndef tree_build_1_H
#define tree_build_1_H

#include "obj_hold.h"
#include "table.h"
#include "d_colour.h"

#include "tbl_row_inst.h"
#include "c_tree_tbl_one.h"

class sort_spec_base;

namespace ACCTS {

/* ----

	This is confusing, badly evolved and needs new docs.
	I'm not sure if I ever understood what I was doing, and now ...

	I think that EVERY INPUT ROW is a 'leaf' row, with data in each field.

	These can then be GROUPED into a tree, by the APP (here) naming
	the PARENT of each item.

	That grouping can use parents from another table or the SAME table.
	EG each CAT can be both a node and a leaf (same table).
	EG each TRANS.acct has an ACCT.key+ as the parent (different table)

	The code tracks the PARENT NODES, and not the item leaves (unles...),
	and builds a tree of them.

	To add a row (of any type) GTK requires a GTK_GUI_parent and sibling,
	and there is a hash table of created GUI rows (for nodes at least-?-)

	The GTk_GUI is mostly opaque to this tree_builder code (this isnt
	the c_tree itself), apart from parent sibline and argv of strings.

	The class tree_build{_0,_1,_2} is one single class, built in layers
	for no clear reason (docs), probably on a 'need-to-know' the
	types of the application, to allow different apps later.

	Leaves (not  stored) and Nodes appear in the order of the input,
	obviously grouped by tree. So the app sorts them first.

	Once built (and displayed) the tree_build_ object disappears,
	and the ctree uses the rowdata==row_inst to talk to the app.
	If you want to change the tree, rebuild it from scratch (that
	was the original design, but Pango seems to make rebuilds too
	slow, when older prototypes showed semi- instant times).

---- */

// YEUK, labels to figure if they can be different
#ifndef extra_stronger_labels
	typedef row_inst T_LOCN; // most
	typedef row_inst T_LEAF; // few
#else
	class T_LEAF : public row_inst
	{
	 public:
		T_LEAF(): row_inst(1) {}
	};
	class T_LOCN : public row_inst
	{
	 public:
		T_LOCN(): row_inst(1) {}
	};
#endif

/*
	A simple tree of (row_inst*)

	Each tree-node is a T_LOCN * group,
	with a parent and list of children_locn.

	TERM: group is used for when its a node not a leaf.
	Leaves are not actually stored here, but they could be.
	The group rows are PROBABLY from a different table than
	the leaves (grouped by a col_spec with a common key).
*/

/*!
	a data tree node (row_inst not the GUI item)

	NB children_locn is almost completely unused! It is only
	used to detect if the NULL_BRANCH has any siblings!

	This happens because parent does whats needed, GUI nodes
	are created on demand, and because the data is already sorted.
*/
struct tree_node : public obj_ref
{
	tree_node * 	parent;
	// obj_hold<tree_node>	parent; // CIRCULAR ?
	obj_list<tree_node>	children_locn;
	obj_list<T_LEAF>	children_leaf;
	obj_hold<T_LOCN>	group;

	tree_node( tree_node * par, T_LOCN * r );
	virtual ~tree_node();

	void flat_list_locn( obj_list<tree_node> & list, bool depthwise = false );
	void flat_list_leaf( obj_list<T_LEAF> & list );

	void sort_interior_here( sort_spec_base * sort_spec );
	void sort_interior_recursive( sort_spec_base * sort_spec );
};

//! a tree builder to build data and GUI trees
/*!
	used by c_tree_one :: reload_data

	The tree is initially configured with the col_spec * tree_spec
	which specifies the sort and grouping tree structure.

	This tree_builder (and not the caller tree holder) figures the
	group-parentage layout of the tree. The caller
	effects the order of the child-leaves.

	The first pass (of the caller) calls find_parent_treenode_of_leaf(row)
	which adds the group-items to the tree.

	The second pass (in reload_data) calls find_add_gui_of_leaf(row)
	which either returns an new/existing group row (gui row) or
	creates a new data row, beneath the group row. The tree builder
	gets the callers c_tree to do the actual creation.

	When the group rows are in the same table as leaves, a group-row might
	need to be upgraded to also be a data-row. This handles when
	referred nodes are created before their data is completely available.

	When group rows are from a different table, it isnt easy to know
	what to display, and all other columns (other than the TREE_COLUMN)
	are blanked with dots. It would be nice to expand the group-key
	to include the description, but that requires a varient row,
	and would disrupt the column_widths.

	Requests are cached in a hash table (of gtk tree items).

	TERM: r1 - leaf - a data row from the table

	TERM: r2 - group - the parent of a leaf 

	TERM: n2 - node - the tree node which is r2

	VARS: r3, n3 grandparent(+) of r1, referred to by n2

	TERM: gui_of_data - the GUI row of a leaf data row
*/

class tree_build_0 : public obj_ref
{
 public:

	//! cached node of row, where row IS a group
	ht2<T_LOCN,tree_node>	map_node_for_group;

	//! The root of the tree
	obj_hold<tree_node>	root;

	//! Some/ALL rows might have a NULL parent-group
	obj_hold<tree_node>	null_branch;

	tree_build_0();

	void re_init_0();
	
	virtual tree_node * new_tree_node( tree_node * par, T_LOCN * r );

	void flat_list_leaf( obj_list<T_LEAF> & list );
	void flat_list_locn( obj_list<tree_node> & list, bool depthwise );
};

/*
	Interacts with TABLE logic, to find/add parent
*/
class tree_build_1 : public tree_build_0
{
 protected:
 public:
	//! The spec that sorts and parents leaf rows
	obj_hold<col_spec>	tree_spec;
	obj_hold<table>		tree_table;
 public:

	tree_build_1( col_spec * _tree_spec );
	tree_build_1();
	void re_init_1( col_spec * _tree_spec );
	void set_tree_spec( col_spec * _tree_spec );

	/*
		PASS1 finds every parent group (so they can be sorted)
		--PASS2 finds every GUI --
	*/
	//! returns n2, the node which is the parent group of r1
	tree_node * find_parent_treenode_of_leaf( T_LEAF * r );

	//! returns n3, the node which is the parent group of r2-group
	tree_node * find_parent_treenode_of_group( T_LOCN * n2 );

	//! returns n2, the node corresponding to r2-group
	tree_node * find_node_of_group( T_LOCN * r2 );

	//! add leaf to tree (except only interior group locn's are added)
	void pass_1_include_leaf( T_LEAF * r )
	{
		find_parent_treenode_of_leaf( r );
	}

	void sort_interior();
};

/*
	maps tree onto Gtk GUI tree,
	knows ctree,
	adds a HT of every leaf
*/
class tree_build_2 : public tree_build_1
{

	// add new group row
	GtkCTreeNode * MK_10(
		tree_node * n3,		// graft new node beneath n3's gui3
		tree_node * n2,		// the gui2 is for this n2
	
		col_spec * tree_col_spec,
		str2 * key,
	
		d_colour * row_colour,
	
		int unused = 0
	)
	{
		obj_ref * NULL_sel_row_data = NULL; // group rows have no gui daa
		T_LOCN * group_row = n2->group;

		GtkCTreeNode * gui3_parent;
		gui3_parent = find_add_gui_of_group( n3 );
		// create gui3 for n3 - from r1 + col_spec
		ctree->prep_group_row( tree_col_spec, group_row, key );
		ctree->parent = gui3_parent;
		ctree->sel_row_data = NULL_sel_row_data; // 
		ctree->do_insert_1();
		ctree->set_bg( row_colour );
		GtkCTreeNode * gui2_group;
		gui2_group =  ctree->new_node;
		// map_gui_for_node.add( n2, gui2_group );
		return gui2_group;
	}

	// add new leaf row
	GtkCTreeNode * MK_20(
		tree_node * n2,
	
	//	col_spec * tree_col_spec,
	//	T_LOCN * group_row,
	//	str2 * key,
	
		T_LEAF * r1,
		// obj_ref * sel_row_data, // current rows attached data
		d_colour * row_colour,
	
		int unused = 0
	)
	{
		obj_ref * sel_row_data = r1;
		GtkCTreeNode * gui_parent;
		gui_parent = find_add_gui_of_group( n2 );
		// create gui2 for n2 - from r1 + col_spec
//		ctree->prep_group_row( tree_col_spec, group_row, key );
		ctree->prep_data_row( r1 );
		ctree->parent = gui_parent;
		ctree->sel_row_data = sel_row_data; // 
		ctree->do_insert_1();
		ctree->set_bg( row_colour );
		GtkCTreeNode * gui1_leaf;
		gui1_leaf =  ctree->new_node;
		return gui1_leaf;
	}

 protected:

	c_tree_tbl_one	*		ctree;		// weak ref

        ht2<tree_node,GtkCTreeNode> map_gui_for_node;

 public:
        GtkCTreeNode * find_add_gui_of_group( tree_node * nB );

	tree_build_2(
		col_spec * _tree_spec,
		c_tree_tbl_one * _ctree
	);

        GtkCTreeNode * find_add_gui_of_leaf( T_LEAF * rA );
	/*
		The input can be T_LEAF or T_LOCN -- not certain yet
	*/

	//! colours
	obj_hold<d_colour> bg_leaf;
	obj_hold<d_colour> bg_group;
	obj_hold<d_colour> bg_subtot;

	//! DERIVED CLASS MUST PROVIDE THIS
 virtual
	GtkCTreeNode * upgrade_group_with_data(
		tree_node * data_as_group,
		T_LOCN * rA
	);

};

class tree_build_3 : public tree_build_2
{
 public:
	tree_build_3(
		col_spec * _tree_spec,
		c_tree_tbl_one * _ctree
	);
	~tree_build_3();

	//! DERIVED CLASS MUST PROVIDE THIS
 virtual
	GtkCTreeNode * upgrade_group_with_data(
		tree_node * data_as_group,
		T_LOCN * rA
	);
};

}; // NAMESPACE
#endif
