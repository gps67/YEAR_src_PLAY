#include "c_tree_tbl_one.h" // ie many things
#include "tbl_col_spec.h" 
#include "tree_build_1.h"
#include "assert.h"

#include "enum_flags.h"

#define ASSERT(x) assert(x)

#include "sort_spec_base.h"

namespace ACCTS {

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
		par->children_locn.append( this );
	}
	// e_print("+tree_node()\n");
}


/*!
	append all leaves to list
*/
void tree_node::flat_list_leaf( obj_list<T_LEAF> & list )
{
	int nn = children_locn.N();
	for(int i=0; i<nn; i++ )
	{
		children_locn[i] -> flat_list_leaf( list );
	}
	int n = children_leaf.N();
	for(int i=0; i<n; i++ )
	{
		list.append( children_leaf[i] );
	}
}

/*!
	append all child nodes to list (recursively)

	The usual question - is this node itself to be added?
	Here I think that there must be a fake root node, so no

	Option on tree depth/bredth - keep siblings together or parents
*/
void tree_node::flat_list_locn( obj_list<tree_node> & list, bool depthwise )
{
	int n = children_locn.N();
	for(int i=0; i<n; i++ )
	{
		tree_node * node = children_locn[i];
		if(! node )
		{
			e_print("flat_list_locn() NULL node\n");
			continue;
		}
		list.append( node );
		if( depthwise)
			node -> flat_list_locn( list, depthwise );
	}
	if( !depthwise)
	for(int i=0; i<n; i++ )
	{
		tree_node * node = children_locn[i];
		if(node) node -> flat_list_locn( list, depthwise );
	}
}

/////////////////////////////////

/*!
	constructor - init root and hash tables
*/
void tree_build_0::re_init_0()
// : map_node_for_group( ht_type_optr, ht_type_optr )	// grouprow -> node
{
	map_node_for_group.remove_all();
	root = new_tree_node( NULL, NULL );
	root->parent = root;
	null_branch = new_tree_node( root, NULL );
}

/*!
	constructor - init root and hash tables
*/
tree_build_0::tree_build_0()
: map_node_for_group( ht_type_optr, ht_type_optr )	// grouprow -> node
, root( NULL )
, null_branch( NULL )
{
	re_init_0();
}

/*!
*/
// virtual
tree_node * tree_build_0::new_tree_node( tree_node * par, T_LOCN * r )
{
	return new tree_node( par, r );
}

/*!
	starting from tree.root, add all leaves to list
*/
void tree_build_0::flat_list_leaf( obj_list<T_LEAF> & list )
{
	root->flat_list_leaf( list );
 return; // duplication for VAT table - fixup so that only root is relevent EVER
	null_branch->flat_list_leaf( list );
}

/*!
*/
void tree_build_0::flat_list_locn( obj_list<tree_node> & list, bool depthwise )
{
	root->flat_list_locn( list, depthwise );
 return; // duplication for VAT table - fixup so that only root is relevent EVER
	null_branch->flat_list_locn( list, depthwise );
}

/////////////////////////////////

/*!
*/
tree_build_1::tree_build_1()
{
}

/*!
	constructor
*/
tree_build_1::tree_build_1(
	col_spec * _tree_spec
)
: tree_build_0()
, tree_spec()
, tree_table()
{
	set_tree_spec( _tree_spec );
}

/*!
*/
void tree_build_1::re_init_1( col_spec * _tree_spec )
{
	re_init_0();
	tree_spec = NULL;
	tree_table = NULL;
	set_tree_spec( _tree_spec );
}

/*!
*/
void tree_build_1::set_tree_spec( col_spec * _tree_spec )
{
	tree_spec = _tree_spec;
	tree_table = NULL;
	if( tree_spec )
		tree_table = tree_spec->get_linked_table();
}

//! find_parent_treenode_of_leaf
tree_node * tree_build_1::find_parent_treenode_of_leaf( T_LEAF * r )
{
/*
	r is a TRANS ROW (usually a leaf in the table)

	When grouped by tree_spec, r has a parent group-row r2
	(often a node from another table)

	The r2 data has a corresponding tree_node n2 (returned)

	map_node_for_group (hash table cache), maps from the
	row to its own tree_node.

	r --> r2 --> n2

	The returned n2 is NEVER NULL
*/

	/*
		r --> r2

		The 'linked_row' of a row is its parent (in another table)
		tree_spec decides this.

		if tree_spec cant find a parent it returns NULL
	*/
	T_LOCN * r2 = (T_LOCN*) NULL;
	if( tree_spec ) r2 = (T_LOCN*) tree_spec->get_linked_row( r );

	/*
		This is a leaf. If the parent is NULL, move them into a
		'fake' subdir, by returning null_branch.

		That cant be done by find_node_of_group(r2) because it
		returns 'root' (real groups dont have fake group parents)
	*/
	if( !r2 ) return null_branch;

	/*
		r2 --> n2
		
		convert the linked row to a node in the interior tree.
	*/
	return find_node_of_group( r2 );
}

/*!
	Finding the parent of r2 is not like finding the parent of r1
*/
tree_node * tree_build_1::find_parent_treenode_of_group( T_LOCN * r2 )
{
	if(!tree_table)
	{
		// this never happens - tree_table is never NULL - or ..?
		fprintf(stderr,"tree_table NULL %s->NULL\n",(STR0) r2->debug_row_name() );
		return root;
	}
	
	// find the GROUPs parent row
	// Thats only supported within the same table (local)
	// or is it ?? non local table happens OK
	T_LOCN * r3;	// parent of r2
	r3  = (T_LOCN*) tree_table->get_local_group_of_row( r2 );
	if(0) {
	 if( r3 ) {
		fprintf(stderr,"r3 found %s->%s\n",(STR0) r2->debug_row_name(), (STR0) r3->debug_row_name() );
	 } else {
		fprintf(stderr,"r3 absent %s->ROOT\n",(STR0) r2->debug_row_name() );
	 }
	}
	
	if(r3==r2)
		r3 = NULL;
	
	// convert group row to group node
	tree_node * node3 = NULL;
	node3 = find_node_of_group( r3 );
	assert(node3);
	return node3;
}

/*!
	r2 is the "parent" linked row of a leaf (BUT NOT NULL)
	or the "grand-parent" (possibly NULL - meaning root).

	return the node that corresponds to the (group) row.
*/
tree_node * tree_build_1::find_node_of_group( T_LOCN * r2 )
{
	/*
		If the group (parent) is NULL, the node is root.
		See also the null_branch fixups, which move top-level
		leaves into a special fake branch (and then out again
		if there are no other branches)

		// NOT // if(!r2) return null_branch;
	*/
	if(!r2) return root;

	tree_node * node2 = NULL; // this function never returns NULL

	/*
		If the value is cached, return it
		(one group is found several times)
	*/
	node2 = map_node_for_group.lookup( r2 );
	if( node2 ) return node2;

	/*
		to create node2, we need its parent node3: (never NULL)
	*/
	tree_node * node3 = NULL;
	node3 = find_parent_treenode_of_group( r2 );

	/*
		build the new node2 under node3
	*/
	node2 = new_tree_node( node3, r2 );

	/*
		 and cache it in map_node_for_group
	*/
	map_node_for_group.add( r2, node2 );

	return node2;
}

/*!
	should have reused sort_spec_one, but that was embedded in table_THING.cxx
	so copy + paste
*/
class sort_spec_node : public sort_spec_base
{
public:
	IS_DIFF_t do_cmp( tree_node * n1, tree_node * n2 )
	{
		if(n1==n2) return IS_SAME; // should not happen
		if(!n1) return IS_LESS;
		if(!n2) return IS_MORE;
		row_inst * g1 = n1->group;
		row_inst * g2 = n2->group;
		if(g1==g2) return IS_SAME; // should never happen
		if(!g1) return IS_LESS;
		if(!g2) return IS_MORE;
		if( g1->get_row_sort() < g2->get_row_sort()) return IS_LESS;
		if( g1->get_row_sort() > g2->get_row_sort()) return IS_MORE;
		return IS_SAME; // ISH
	}
	
	MFN_do_cmp get_do_cmp()
	{
		return (MFN_do_cmp) & sort_spec_node::do_cmp;
	}
};
	
/*!
*/
void tree_node::sort_interior_here( sort_spec_base * sort_spec )
{
	children_locn.sort( sort_spec );
}
	
/*!
*/
void tree_node::sort_interior_recursive( sort_spec_base * sort_spec )
{
	sort_interior_here( sort_spec );
	
	int nl = children_locn.N();
	for( int i=0; i<nl; i++ )
	{
		tree_node * _child = children_locn[i];
		tree_node * child = (tree_node *) _child;
		child->sort_interior_recursive( sort_spec );
	}
}

/*!
*/
void tree_build_1::sort_interior()
{
	obj_hold<sort_spec_node> sort_spec = new
		sort_spec_node();
	root->sort_interior_recursive( sort_spec ); // 
}

/////////////////////////////////

/*!
	constructor

	Without the HT constructor, you get a sigsegv because obj is gobject not obj_ref
*/
tree_build_2::tree_build_2(
	col_spec * _tree_spec,
	c_tree_tbl_one * _ctree
)
: tree_build_1( _tree_spec )
, map_gui_for_node( ht_type_optr, ht_type_void_g_object )       // node-> gui   
{
	ctree = _ctree;

	// set the colours
	bg_leaf   = Colour_Map->create_new_colour("RPT1:bg_leaf",   "#FFFFFF" );
	bg_group  = Colour_Map->create_new_colour("RPT1:bg_group",  "#FFE0E0" );
	bg_subtot = Colour_Map->create_new_colour("RPT1:bg_subtot", "#E0FFE0" );
}


/*!
*/
GtkCTreeNode * tree_build_2::upgrade_group_with_data(
	tree_node * data_as_group,
	T_LOCN * r2
)
{
	e_print("ERROR  tree_build_2::upgrade_group_with_data() \n");
	return NULL;
}

/*!
*/
GtkCTreeNode * tree_build_2::find_add_gui_of_leaf( T_LEAF * r1 )
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

	/*
		Forward reference - RESOLVED

		A leaf is not often an interior group node, but it can be
		EG CAT tree is its own group

		It if is found in the interior tree, it was (almost certainly)
		created as a forward reference, which is now resolved.
		IE The new value overwrites the old blank placeholder.
	*/
	tree_node * data_as_group;
	data_as_group = map_node_for_group.lookup( (T_LOCN*) r1 );
	if( data_as_group ) {
		return upgrade_group_with_data( data_as_group, (T_LOCN*) r1 );
	}
	// does that make it a LEAF or a LOCN - is it in the children_leaf list?

	d_colour * row_colour = bg_leaf;
	if( r1->row_flags & ROW_show_sums )
		row_colour = bg_subtot;

	// Get Interior tree node gui2 parent of gui1
	tree_node * n2 = find_parent_treenode_of_leaf( r1 );
	assert(n2); // root node isnt NULL

/*
// QUICK + DIRTY HACK - should check that it isnt already there ...
// you have to do this manually
	int t_pos = 0;
	n2->children_leaf.append_if_absent( r1 );
*/

	gui1 = MK_20(
		n2,	// parent node of leaf
		// tree_col_spec is NULL or not used
		r1,
		row_colour,
		0
	);

	return gui1;
}

GtkCTreeNode * tree_build_2::find_add_gui_of_group( tree_node * n2 )
{
	/*
		n2 --> r2	- can be NULL
		n2 --> n3	- never NULL
		n2.group
	*/

	GtkCTreeNode * gui2;

	// has it already been created ?
	gui2 = map_gui_for_node.lookup( n2 );
	if( gui2 ) return gui2;

	str2 * keystr = NULL;
	static obj_hold<str2> s_ROOT = new str2( "s_ROOT" );
	static obj_hold<str2> s_NULL = new str2( "s_NULL" );
	GtkCTreeNode * NULL_gui_root = NULL; // Gtk uses NULL
	col_spec * t_spec;
	tree_node * n3;

	/*
		does this group have a defining row ?
		a node with no row data is either root or null_branch
	*/
	row_inst * r2 = n2->group;
	if( !r2 )
	{
		/*
			the root node has NULL as its gui parent
		*/
		if( n2 == root )
		{
			return NULL_gui_root;
		}

		if (n2 != null_branch ) // it must be
		{
			// this neer happens, its either root or null_branch
			fprintf(stderr,"ROOT ROTT\n" );
			return NULL;
		}

		/*
			This n2 is the null_branch.

			Act as either a branch or as the root node.
			It is a child of root but if there are no others
			its children_locn become roots children_locn. invisibly.
		*/
		if( 1 == root->children_locn.N() )
		{
			return NULL_gui_root;
		}

		/*
			so now we have the rare case of creating the NULL_BRANCH
		*/

		/*
			This strange node has a strange name.
		*/
		keystr = s_NULL;

		/*
			The parent of s_NULL is root
		*/
		n3 = root;

		/*
			Dont use tree_table->key_spec, use the leafs (tree_buid's)
		*/
		t_spec = tree_spec;

	} else {
		/*
			n3 already exists (but its gui might not)
		*/
		n3 = n2->parent;

		/*
			Dont use leaf's spec, use tree_table->key_spec
		*/
		t_spec = tree_table->key_spec;
	}
	
	gui2 = MK_10(
		n3,
		n2,			// tree_node

		t_spec,
		keystr,			// key str NULL defaults

		bg_group		// row_colour
	);
	map_gui_for_node.add( n2, gui2 );

	return gui2;
}

/////////////////////

tree_build_3::
~tree_build_3()
{
	if(0) e_print("~tree_build_3()\n");
}

tree_build_3::
tree_build_3(
	col_spec * _tree_spec,
	c_tree_tbl_one * _ctree
)
: tree_build_2( _tree_spec, _ctree )
{
}

// r2 has been found in map_node_for_group (row->group_node)
// upgrade the gui item to show the row data
// return gui
GtkCTreeNode * tree_build_3::upgrade_group_with_data(
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
	ctree->set_bg( bg_group );
	gui2b = ctree->new_node;
	return gui2b;
}
}; // NAMESPACE
