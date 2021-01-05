#include "mem_line_tree.h"

str0
mem_line_tree_node::
get_attr()
const
{
	spec_attr_mapper * mapper = & treetop->mapper;
	spec_attr_raw attr_raw = mapper->un_map( depth );
	// const mem_line_spec * spec = (const mem_line_spec *) & leaf.ptr->spec;
	const mem_line_spec * spec = (const mem_line_spec *) & leaf->spec;

	// mem_line_spec * spec1 = (mem_line_spec *) & leaf->spec;
	// const mem_line_spec * spec = (const mem_line_spec *) spec1;

	return spec->get_attr( attr_raw );
	// return leaf->spec.get_attr( attr_raw );

	// if( ! treetop  ) return "";
	// if( ! spec  ) return "";
	// return treetop->get_attr( spec,  attr );
}

/*
str0
mem_line_tree_node::
get_attr(
	const mem_line_spec * spec,
	const spec_attr_mapped attr
) const
{
	if( ! treetop  ) return "";
	if( ! spec  ) return "";
	return treetop->get_attr( spec,  attr );
}

str0
mem_line_tree_node::
get_attr_name( const spec_attr_raw & attr_raw )
{
	return leaf->spec.get_attr_name( attr_raw );
}
*/

str0
mem_line_tree_node::
get_attr_name() 
{
	// I'm sure this is mad - but sort out when docs
	//return "Label";
	spec_attr_mapper * mapper = & treetop->mapper;
	spec_attr_raw attr_raw = mapper->un_map( depth );
	return leaf->spec.get_attr_name( attr_raw );
}

str0
mem_line_tree_node::
name()
{
	if( ! leaf  ) return "";
	return get_attr();
	// return get_attr( & leaf->spec,  depth );
}

mem_line_tree_node::
mem_line_tree_node(
	mem_line_tree * tree,
	const spec_attr_mapped _depth,
	const mem_line_spec & _spec
)
: treetop( tree )
, depth( _depth )
, leaf( NULL )
, branches()
{
	sel = -1;
	// is_leaf_node() logic in constructor
	// if( depth == mem_line_spec::N_depths() )
	if( is_leaf_node())
	{
		leaf = new mem_line_tree_leaf(
			tree->new_id(),
			_spec
		);
	} else {
		mem_line_tree_node * node1 = 
		  new mem_line_tree_node(
			tree,
			depth.next(),
			_spec
		  );
		// each INTERIOR-NODE holds a leaf. which has common prefix
		leaf = node1->leaf;
		branches.append( node1 );
		// The selected path down this sub-tree (set by GUI)
		sel = 0;
	}
}

mem_line_tree_node *
mem_line_tree_node::
find_prefix(
	const spec_attr_mapped deep,
	const mem_line_spec * search
) {
	if( depth == deep )
		return this;
	if( deep.is_root_node() )
		return branches[0]->find_prefix( deep, search );
//
//	str0 key = get_attr( search, depth.next() );
	str0 key = treetop->get_attr( search,  depth.next() );
	mem_line_tree_node * branch;
	int n = branches.N();
	for( int b=0; b<n; b++ )
	{
		branch = branches[b];
		// if( 0==strcmp( key, branch->name()) ) {
		if( key == branch->name() ) {
			return branch->find_prefix( deep, search );
		}
	}
	e_print("find_prefix() not found \n");
	return NULL;
}

mem_line_tree_node * // returns the node that holds the leaf
mem_line_tree_node::
find_add_2(
	const mem_line_spec & _spec
) {
	/*
		THIS is the parent node of the search area
		THIS might be the leaf-holder
	*/

	if(is_leaf_node()) {
		if(0) e_print("## find_add_2() is_leaf_node()\n");
		return this;
	}

	/*
		searching for leaf beneath this node,
		subkey is one step in that direction from here
	*/
	// str0 subkey = get_attr( & _spec, depth.next() );
	str0 subkey = treetop->get_attr( &_spec,  depth.next() );
	if(0) e_print(" .. %s ", subkey.sget() );

	// try each branch, (recursive down one)
	for( int b = branches.N()-1; b>=0; b-- )
	{
		mem_line_tree_node * branch = branches[b];
		if( subkey == branch->name() )
		{
			return branch->find_add_2( _spec );
		}
	}

	// add missing branch
	mem_line_tree_node * extra = new mem_line_tree_node(
		treetop,
		depth.next(),
		_spec
	);
	branches.append(  extra );
	return extra;
}

mem_line_tree_leaf *
mem_line_tree::find_add( const mem_line_spec & _spec )
{
	// first use primes the tree
	// searching again is a waste but simple, once
	if( !root ) {
		root = new mem_line_tree_node( this, spec_attr_mapped::root(), _spec );
	}
	return root->find_add_2( _spec )->leaf;
}

