// only included by report1_pdf.cxx - currently

#include "sum_tree.h"

using namespace ACCTS;

sum_tree_node::sum_tree_node( tree_node * par, T_LOCN * r, int ntotals )
: tree_node( par, r )
, subtotal()
{
	if(!par)
		depth = -1;
	else
		depth = ((sum_tree_node *) par)->depth + 1;
	subtotal.extend_to( ntotals ); // init 0
}


/////////

sum_tree_build::sum_tree_build()
: tree_build_1()
{
}

sum_tree_build::sum_tree_build( col_spec * _tree_spec )
: tree_build_1()
, subtotal_specs()
{
	re_init_2( _tree_spec );
}

/*
	prepare tree to be sorted by _tree_spec
	find ALL summable columns (even if hidden)
*/
void sum_tree_build::re_init_2( col_spec * _tree_spec )
{
	/*
		This is tricky, as root and null_branch must be created
		with the CORRECT number of totals fields
	*/
	subtotal_specs.clear();
	table1 = NULL;
	if( !_tree_spec )
		return;
	// assert table
	table1 = _tree_spec->tbl;
	int n = table1->col_specs.N();
	int vis_col_pos = -1;
	for( int i=0; i<n; i++ )
	{
		col_spec * spec = table1->col_specs[i];
		if( ! spec->is_hidden )
			vis_col_pos ++;
		if( ! spec->is_summable )
		{
			continue; // ok no request from report/table
		}
		if( spec->get_field_type() != T_amnt )
		{
			e_print("summable but not amnt\n");
			continue;
		}
		int idx = subtotal_specs.N();
		subtotal_specs.extend_by(1);
		subtot_spec & sub = subtotal_specs[idx];
		sub.vis_col_pos = vis_col_pos;;
		if( spec->is_hidden )
			sub.vis_col_pos = -1;
		sub.spec = (col_spec_amnt *) spec;
	}
	re_init_1( _tree_spec );

	// there is field duplication here ...

	table * t1 = table1;
	table * t2 = tree_table;

	s1_tree = t1->col_spec_tree;
	s1_desc = t1->desc_spec;
	s2_tree = s1_tree; // LURK
	s2_desc = s1_desc; // LURK
	if(t2) s2_tree = t2->key_spec; // t2->col_spec_tree;
	if(t2) s2_desc = t2->desc_spec;
	
}

// virtual
tree_node * sum_tree_build::new_tree_node( tree_node * par, T_LOCN * r )
{
	// subtotals is allocated but set to zero
	return new sum_tree_node( par, r, subtotal_specs.N() );
}

str2 * sum_tree_build::get_key( tree_node * node )
{
	if( node && node->group && tree_table )
		return tree_table->get_key_of_row( node->group );
	if(!node) return new str2("NULL NODE");
	if(!node->group)
	{
		return NULL;
		return new str2("Entire Table");
		if( table1 )
			return table1->name;
		return new str2("NULL node->group");
	}
	return new str2("ERROR");
}

str2 * sum_tree_build::get_desc( tree_node * node )
{
	if( node && node->group && tree_table )
		return tree_table->get_desc_of_row( node->group );
	if(!node) return new str2("NULL NODE");
	if(!node->group)
	{
		return NULL;
		return new str2("Entire Table");
		if( table1 )
			return table1->name;
		return new str2("NULL node->group");
	}
	return new str2("ERROR");
}

str2 * sum_tree_build::get_key_desc( tree_node * node )
{
	if( node && node->group && tree_table )
		return tree_table->get_key_desc_of_row( node->group );
	if(!node) return new str2("NULL NODE");
	if(!node->group)
	{
		return new str2("table");
		if( table1 )
			return table1->name;
		return new str2("NULL node->group");
	}
	if( tree_spec )
		return tree_spec->get_key_desc( node->group );
	else
		return new str2( "NULL tree_spec" );
}

/*
	add one leaf to the tree, update all immediate subtotals
	
	parent_node = find_parent_treenode_of_leaf( row );
	parent_node.children_leaf.append( row )
	parent_node.subtotal[FOREACH] += row.CORRESPONDING value

	see rep_pdf_0::build_tree() -- tree.used
*/
void sum_tree_build::add_one_row( row_inst * row )
{
	sum_tree_node * node = (sum_tree_node *) find_parent_treenode_of_leaf( row );
	if( !node ) {
		e_print("# ERROR in add_one_row() NULL node\n");
		return;
	}
	node->children_leaf.append( row );
	int ns = subtotal_specs.N();
	// this isnt quite right as it dos add UP-tree
	for( int s=0; s<ns; s++ )
	{
		subtot_spec & sub = subtotal_specs[s];
		amnt_t val;
		if(! sub.spec->get_amnt( row, val ) )
		{
			e_print("# ERROR in add_one_row() get_amnt() returned FALSE (null field?) column: %s\n",
				(STR0) sub.spec->name->str
			);
			/*
				This happens when rows are auto-created
				eg ACCT.h.amount == NULL
				eg ACCT.VAT.amnt_closing == NULL
				The following debug helps,
				but makes output verbose
			*/
			if(1) row->prints();
			continue;
		}
		node->subtotal[s].add( val );
	}
}

void sum_tree_build::add_all_rows()
{
	if(!table1) return;
	int nr = table1->rows_list.N();
	for( int r=0; r<nr; r++ )
	{
		add_one_row( table1->rows_list[r] );
	}
}

/*!
	Add up the sub-location totals
	leaves were calculated during add (albeit at cache hits)

*/
void sum_tree_build::add_all_subtotals()
{
	add_all_subtotals( root );
	add_all_subtotals( null_branch );
}

void sum_tree_build::add_all_subtotals( tree_node * _locn )
{
	if(!_locn) return;
	sum_tree_node * locn = (sum_tree_node *) _locn;
	/*
		tree-walk down children_locn
		for each coumn added to subtotals[]
		add sub-values to this parent node

		NB: the ROWS were previously added to a sum_tree_node :
			add_one_row( row )
	*/
	int nl = locn->children_locn.N();
	for( int i=0; i<nl; i++ )
	{
		tree_node * _child = locn->children_locn[i];
		sum_tree_node * child = (sum_tree_node *) _child;
		add_all_subtotals( child );
		int ns = locn->subtotal.N();
		for( int s=0; s<ns; s++ )
		{
			locn->subtotal[s].add( child->subtotal[s] );
		}
	}
}


int sum_tree_build::find_idx_spec( col_spec * spec )
{
	if(!spec) return -1;
	int ns = subtotal_specs.N();
	for( int s=0; s<ns; s++ )
	{
		subtot_spec & sub = subtotal_specs[s];
		if( sub.spec == spec ) return s;
	}
	e_print("Failed to find %s in subtotal_specs\n", (STR0) spec->name->str );
	return -1;
}


/*!
	add the VISIBLE fields
*/
bool sum_tree_build::get_total_of_visible_row( tree_node * _node, amnt_t & amnt )
{
	sum_tree_node * node = (sum_tree_node *) _node;
	amnt_t sum;
	int ns = subtotal_specs.N();
	if( !node ) ns = 0;
	for( int s=0; s<ns; s++ )
	{
		if( !subtotal_specs[s].spec->is_hidden )
			sum.add( node->subtotal[s] );
	}
	amnt = sum;
	return true;
}

/*
	get the accumulated total for the specified tree-row-grp, column 
*/
bool sum_tree_build::get_total(
	tree_node * _node,	//!< the group row
	col_spec * spec,	//!< the column
	amnt_t & amnt		//!< the returned amount
)
{
	sum_tree_node * node = (sum_tree_node *) _node;
	if( !node ) return false;
	if( !spec ) return false;
	int pos = find_idx_spec( spec );
	if( pos < 0 ) return false;
	amnt = node->subtotal[pos];
	return true;
}

