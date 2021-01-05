#include "c_tree_tbl_one.h" // ie many things
#include "tbl_col_spec.h" 
#include "tree_build_1B.h"
#include "assert.h"

#include "enum_flags.h"

#define ASSERT(x) assert(x)

/*!
	destructor - all automatic
*/
tree_node::~tree_node()
{
	// e_print("~tree_node()\n");
}

/*!
	constructor for a locn not a leaf
*/
tree_node::tree_node( tree_node * par, T_LOCN * r )
{
	group = r;
	if( par )
	{
		parent = par;
		par->children.append( this );
	}
	// e_print("+tree_node()\n");
}

/*!
	constructor - init root and hash tables
*/
tree_build_0::tree_build_0()
: map_node_for_group( ht_type_optr, ht_type_optr )	// grouprow -> node
{
	root = new tree_node( NULL, NULL );
	root->parent = root;
	null_branch = new tree_node( root, NULL );
}

/*!
	constructor
*/
tree_build_1::tree_build_1(
	col_spec * _sort_spec,
	c_tree_tbl_one * _ctree
)
: map_gui_for_node( ht_type_optr, ht_type_void_g_object )	// node-> gui
{
	tree_spec = _sort_spec;
	tree_table = tree_spec->get_linked_table();
	ctree = _ctree;

	// set the colours
	bg_leaf   = Colour_Map->create_new_colour("RPT1:bg_leaf",   "#FFFFFF" );
	bg_group  = Colour_Map->create_new_colour("RPT1:bg_group",  "#FFE0E0" );
	bg_subtot = Colour_Map->create_new_colour("RPT1:bg_subtot", "#E0FFE0" );
}

tree_build_2::
tree_build_2(
	col_spec * _sort_spec,
	c_tree_tbl_one * _ctree
)
: tree_build_1( _sort_spec, _ctree )
{
}

//! find_parent_treenode_of_leaf
tree_node * tree_build_1::find_parent_group_of_leaf( T_LEAF * r )
{
/*
	r is a TRANS ROW (usually a leaf in the table)

	When grouped by tree_spec, r has a parent group-row r2
	(often a node from another table)

	The r2 data has a corresponding tree_node n2 (returned)

	map_node_for_group (hash table cache), maps from the
	row to its own tree_node.
*/

	// DATA --> (parent) GROUP DATA // r2: a row in another table tree_table
	T_LOCN * r2 = (T_LOCN*) tree_spec->get_linked_row( r );

	// DATA r2 --> NODE n2
	tree_node * n2;
	if( r2 ) {
		n2 = find_node_of_group( r2 );
	} else {
		if(1)
			// items with no parent get a fake parent below root
			n2 = null_branch;
			// but later make a sole NULL branch the root
		else
			// otherwise you get 5 groups and 200 leaf siblings
			n2 = root;
	}

	// ALL GROUP items are in tree (data isnt)
	// ie tree_node does NOT include leaves, only nodes
	// but hash table records r->n2 for all leaves

	return n2;
}

// find the group node of a group row_inst (not parent or grandparent)
tree_node * tree_build_1::find_node_of_group( T_LOCN * r2 )
{
	// top-level group nodes have NULL r2, so return root
	if(!r2) return root;;
	// NOT // if(!r2) return null_branch;;

	// cached value (one group is found several times)
	tree_node * node2 = NULL;
	node2 = map_node_for_group.lookup( r2 );
	if( node2 ) return node2;

	// to create node2, we need node3 its parent
	tree_node * node3 = NULL;
	node3 = find_parent_group_of_group( r2 );

	// build the new node and hold in map_node_for_group
	node2 = new tree_node( node3, r2 );
	map_node_for_group.add( r2, node2 );
	return node2;
}

tree_node * tree_build_1::find_parent_group_of_group( T_LOCN * r2 )
{
	// find the GROUPs parent row
	// Thats only supported within the same table (local)
	// or is it ?? non local table happens OK
	T_LOCN * r3;
	if(tree_table) {
		r3  = (T_LOCN*) tree_table->get_local_group_of_row( r2 );
		if(0) if( r3 ) {
			fprintf(stderr,"r3 found %s->%s\n",(STR0) r2->debug_row_name(), (STR0) r3->debug_row_name() );
		} else {
			fprintf(stderr,"r3 absent %s->ROOT\n",(STR0) r2->debug_row_name() );
		}
	} else {
		// this never happens - tree_table is never NULL - or ..?
		fprintf(stderr,"tree_table NULL %s->NULL\n",(STR0) r2->debug_row_name() );
		r3 = NULL;
	}
	if(r3==r2)
		r3 = NULL;

	// convert group row to group node
	tree_node * node3 = NULL;
	node3 = find_node_of_group( r3 );
	assert(node3);
	return node3;
}

GtkCTreeNode * tree_build_1::upgrade_group_with_data(
	tree_node * data_as_group,
	T_LOCN * r2
)
{
	e_print("ERROR  tree_build_1::upgrade_group_with_data() \n");
	return NULL;
}

GtkCTreeNode * tree_build_1::find_add_gui_of_leaf( T_LEAF * r1 )
{
/*
	A data row leaf can also be a group row parent of another row
	WHEN: group_spec->get_group_table() == this_table
	Plus there are two (row_inst *) nil roots, root and null_branch
	To resolve the nil, use ht<row_inst)
	To resolve the two, use ht<tree_node>

	Want to avoid keeping a ht of each data leaf,
	but can look for a data-row in the tree in	map_node_for_group
	(only when tree_spec-something)
*/
	GtkCTreeNode * gui1;

	// data row is not often an interior group node, but it might be
	// EG CAT tree is its own group
	tree_node * data_as_group;
	data_as_group = map_node_for_group.lookup( (T_LOCN*) r1 );
	if( data_as_group ) {
		return upgrade_group_with_data( data_as_group, (T_LOCN*) r1 );
	}

	// data(A) has a group parent (level-B)
	// group can be a leaf row
	tree_node * n2 = find_parent_group_of_leaf( r1 );
	assert( n2 );

	// gtk uses a root gui parent of NULL
	GtkCTreeNode * gui2;
	gui2 = find_add_gui_of_group( n2 );

	// prep strs[] for data_row
	ctree->prep_data_row( r1 );
	ctree->parent = gui2;
	ctree->sel_row_data = r1; // tbl_base
	ctree->do_insert_1();
	if( r1->row_flags & ROW_show_sums )
		ctree->set_bg( bg_subtot );
	else
		ctree->set_bg( bg_leaf );
	gui1 = ctree->new_node;

	return gui1;
}

GtkCTreeNode * tree_build_1::find_add_gui_of_group( tree_node * n2 )
{
	GtkCTreeNode * gui2;

	// has it already been created ?
	gui2 = map_gui_for_node.lookup( n2 );
	if( gui2 ) return gui2;

	row_inst * r2 = n2->group;
	if( !r2 ) {
		// a node with no row data is either root or null_branch
		ctree->parent = NULL;
		if( n2 == root ) {
			return NULL; // a minor imporvement
			static obj_hold<str2> s_ROOT = new_str2_static( "s_ROOT" );
			ctree->prep_group_row( tree_spec, r2, s_ROOT );
		} else if (n2 == null_branch ) {
			/*
				null_branch is a child of root
				but if there are no others
				AND SINCE any leaf must have a non null branch
				(or be under the null_branch)
				if n_branch==1 there are no other branches
				and no leaves beneath root, so NULL is ROOT
				or at least draw it that way
				OPTIONALLY have a rename for NULL_ROOT
			*/
			int n_branch = root->children.N();
			if( 1==n_branch ) {
				return NULL;
			}
			// null_branch has siblings, give it a name
			static obj_hold<str2> s_NULL = new_str2_static( "s_NULL" );
			ctree->prep_group_row( tree_spec, r2, s_NULL );
			// caller keeps this in cache anyway
			if(0) e_print("n_branch == %d\n", n_branch );
		} else {
			fprintf(stderr,"ROOT ROTT\n" );
			return NULL;
		}

		// GROUP is NOT DATA // ctree->sel_row_data = r2; // tbl_base
		if(1) e_print("UNREACHED\n" );
		ctree->sel_row_data = NULL;
		ctree->do_insert_1();
		ctree->set_bg( bg_group );
		gui2 = ctree->new_node;
		map_gui_for_node.add( n2, gui2 );
		return gui2;
	}
	
	// find gui3 parent of the soon to be added gui2
	tree_node * n3 = n2->parent;
	GtkCTreeNode * gui3;
	gui3 = find_add_gui_of_group( n3 );

	// create gui2 for n2 - from r1 + col_spec
	col_spec * spec_group_key = tree_table->key_spec;
	ctree->prep_group_row( spec_group_key, n2->group, NULL );
	ctree->parent = gui3;
	ctree->sel_row_data = NULL; // 
	ctree->do_insert_1();
	ctree->set_bg( bg_group );
	gui2 = ctree->new_node;

	map_gui_for_node.add( n2, gui2 );
	return gui2;
}

// r2 has been found in map_node_for_group (row->group_node)
// upgrade the gui item to show the row data
// return gui
GtkCTreeNode * tree_build_2::upgrade_group_with_data(
	tree_node * data_as_group,
	T_LOCN * r2
)
{
	if(0) fprintf(stderr,"LEAF == NODE upgrade %s\n", (STR0) r2->debug_row_name() );
	//
	GtkCTreeNode * gui2b;	// updated and returned
	GtkCTreeNode * gui2;	//
	// this probably adds the group then upgrades
	gui2 = find_add_gui_of_group( data_as_group );
	//
	ctree->new_node = gui2; // old_node really sel_node ? curr_
	ctree->sel_row_data = r2; // tbl_base
	ctree->prep_data_row( r2 );
	ctree->do_update_1();
	ctree->set_bg( bg_leaf );
	gui2b = ctree->new_node;
	return gui2b;
}

tree_build_2::
~tree_build_2()
{
	if(0) e_print("~tree_build_2()\n");
}
