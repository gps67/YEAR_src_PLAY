#ifndef mem_line_tree_H
#define mem_line_tree_H

// #include "glib.h" // GHashTable

#include <stdio.h> // FILE *
#include "str1.h"
#include "is_diff.h"
#include "my_hash.h"

#include "mem_line_spec.h"	// not an obj_ref
#include "obj_ref.h"
#include "obj_hold.h"
#include "obj_list.h"

class mem_line_tree;

//! each line_spec is a leaf
class mem_line_tree_leaf : public obj_ref
{ public:
	int mem_line_id;;
	mem_line_spec	spec;

	mem_line_tree_leaf( int _id, const mem_line_spec & _spec )
	: mem_line_id( _id )
	, spec( _spec )
	{
	}

	~mem_line_tree_leaf()
	{
	}
};

//! interior node with a representative leaf (upto depth)
class mem_line_tree_node : public obj_ref
{ public:

	mem_line_tree * treetop; // which holds GUI info
	spec_attr_mapped depth;
	int sel;	// selected branch (-1 for empty=leaf)
	obj_hold<mem_line_tree_leaf> leaf;
	obj_list<mem_line_tree_node> branches;

	~mem_line_tree_node()
	{
	}

	/*
		_spec was found absent, and _id allocated for its leaf.
		This constructor builds a subtree (from depth) and a new leaf.
	*/
	//! Create node, sub-tree and leaf (known absent)
	mem_line_tree_node(
		mem_line_tree * _treetop,
		const spec_attr_mapped _depth,
		const mem_line_spec & _spec
	);

	//! Leaves are held by a dedicated node, at fixed level 7
	bool is_leaf_node()
	{
		// depth-7 is a leaf 0..7 is 8 levels
		return depth.is_leaf_node() ;
	}

	//! The root node has depth -1 (it is above level[0]
	bool UNUSED_is_root_node()
	{
		return depth.is_root_node();
	}

	/*
		name is the distinguishing key of THIS and siblings,
		taken from a sample leafs spec (first added to tree)
		as all sub-nodes and leaves share a common prefix
		upto and including key[depth]

		XMAP SPEC will require spec to jumble the depths,
		or this tree to do so.
	*/
	str0 name();

	//! search for _spec in this subtree, add via root if missing
	mem_line_tree_node * // returns the node that holds the leaf
	find_add_2(
		const mem_line_spec & _spec
	);

	void sel_branch( mem_line_tree_node * br )
	{
		int pos;
		if( branches.find_ptr( br, pos ) ) {
			sel = pos;
			return;
		}
		e_print("sel_branch() not found \n");
	}

	mem_line_tree_node * find_prefix(
		const spec_attr_mapped deep,
		const mem_line_spec * search
	);

/*
	str0 get_attr(
		const mem_line_spec * spec,
		spec_attr_mapped attr
	) const;
*/

	str0 get_attr() const;
	str0 get_attr_name();
//	str0 get_attr_name( const spec_attr_mapped & attr );
//	str0 get_attr_name( const spec_attr_raw & attr );

};

//! A tree is a a single root node, with depth -1, and no key[-1], or siblings
class mem_line_tree : public obj_ref
{
 public:
	obj_hold<mem_line_tree_node> root;
	int mem_line_id_next;
	void * client_data;	// sar_layout
	spec_attr_mapper mapper;

	str0 get_attr(
		const mem_line_spec * spec,
		const spec_attr_mapped depth
	) const
	{
		return spec->get_attr( mapper.un_map( depth ) );
	}


	mem_line_tree( const spec_attr_mapper & _mapper )
	:mapper( _mapper )
	{
		root = NULL;
		mem_line_id_next = 0;
	}

	mem_line_tree_leaf * find_add( const mem_line_spec & spec );

	int new_id()
	{
		return mem_line_id_next++;
	}

	int xN_depths()
	{
		return mem_line_spec::N_depths();
	}
};

#endif
