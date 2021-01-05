
#include "c_tree_tbl_one.h"
#include "tree_build_1.h"
#include "tbl_col_spec.h"

#include "g_timer.h"

namespace ACCTS {

// one knows the type of row_data (base doesnt)

/*!
	constructor - tbl is the data, treecol is to be a tree

	NB The treecol doesnt change, a new ctree is generated on change.
*/
c_tree_tbl_one::c_tree_tbl_one( table * _tbl, int treecol )
: w_ctree()
{
	tbl = _tbl;
	if(0) fprintf(stderr,"c_tree_tbl_one(%s)\n", (STR0) *tbl->name );
	if(0) gdb_break_point();

	build_col_list();

	// obtain an argv of column names
	int nc = col_list.N();
	strs.extend_to( nc );
	for( int i = 0; i<nc; i++ )
	{
		strs.set( i, col_list[i]->get_name_hdr() ); 
	}

	// create a new Gtk CTree, using the headers
	// since THIS is derived from the CTree holder, its a late init
	late_init( nc, treecol,  strs.get_argv() );

	// set each columns left/right alignment
	for( int i = 0; i<nc; i++ )
	{
		col_spec * spec = col_list[i];
		set_column_align( 
			i,
			spec->get_align()
		);
	}

	// file the table now
	reload_data();

}

/*!
	col_list is a copy of the tbls col_list
	but skipping spec.is_hidden

	You can change the order, as long as you write the hooks
	to munge this list at the right time, for now RPT1 simply
	adds the columns in the desired sequence.
*/
void c_tree_tbl_one::build_col_list()
{
	col_list.clear();

	int nc = tbl->col_specs.N();
	for( int i = 0; i<nc; i++ )
	{
		col_spec * spec = tbl->col_specs[i];
		if( !spec ) continue; // shouldnt happen
		if( spec->is_hidden ) continue;
		col_list.append( spec );
	}
}

/*!
*/
int c_tree_tbl_one::find_visible_col( col_spec * spec )
{
	int nc = col_list.N();
	for( int i=0; i<nc; i++ )
	{
		if( col_list[i] == spec ) return i;
	}
	return -1;
}


/*!
*/
row_inst * c_tree_tbl_one::get_sel_row_inst()
{
	return (row_inst *) (obj_ref *)sel_row_data;
}

/*!
*/
void c_tree_tbl_one::prep_data_row( row_inst * row )
{
	// for each column ...
	int nc = col_list.N();
	strs.extend_to( nc );
	for( int c = 0; c<nc; c++ )
	{
		col_spec * spec = col_list[c];
		str2 * s =  spec->get_str2_for_display( row );
		strs.set( c, s );
	}
}

/*!
*/
void c_tree_tbl_one::prep_group_row(
	col_spec * tree_spec,
	row_inst * row,
	str2 * astr
)
{
	// row can be from an alien table - the group table
	// row CAN be from same table, but that is handled elsewhere
	int nc = col_list.N();
	strs.extend_to( nc );
	obj_hold<str2> s;
	for( int c = 0; c<nc; c++ )
	{
		/*
			this function is ONLY called for group rows
			so leaf rows get the abbreviated value.
			That includes group nodes within the dame table
			eg CAT table doesnt call this
		*/
		if( c == TREE_COLUMN ) {
			if( astr ) {
				strs.set( c, astr );
			} else {
			// OK-ISHs = mk_str_key_desc( this, row, tree_spec );
			// OK	s = tree_spec->get_str2_for_display( row );
			//	s = tree_spec->get_key_desc_for_display( row );
			// OVER	s = tree_spec->get_linked_key_desc( row );
				s = tree_spec->get_key_desc( row );
				strs.set( c, s );
			}
		} else {
			static obj_hold<str2> s2 = new_str2_static(".");
			strs.set( c, s2 );
		}
	}
}

/*!
*/
void c_tree_tbl_one::reload_data()
{
	// get the spec1 for TREE_COLUMN  used to sort the table,
	col_spec * spec1 = col_list[TREE_COLUMN] ;

	// pre-sorted data leads to sorted tree (do as 2 pass anyway)
	tbl->sort_by_col_spec( spec1 );

	// the number of rows remains fixed
	int nr = tbl->rows_list.N();

	obj_hold<g_timer> timer; timer = new g_timer();

	/*
		This code could be moved to tree_build_1,
		we call it with tree->find_add_gui_of_leaf( row );
		and it calls us with prep_group_row(), do_insert()

		tree is deleted at the end of this function,
		after it has clear()ed and rebuilt this 
	*/
	obj_hold<tree_build_3>	tree; tree = new tree_build_3( spec1, this );
	if(1) timer->step_print( *tbl->name );


	// build the non-gui tree of groups (hierarchy)
	for(int r = 0; r<nr; r++ )
	{
		row_inst * r1 = tbl->rows_list[r];
		T_LEAF * row = (T_LEAF*) r1;
		// tree->pass_1_include_leaf( row );
		tree_node * n2 = tree->find_parent_treenode_of_leaf( row );
		n2->children_leaf.append_if_absent( row );
	}
	tree->sort_interior();
#if 0

#if 0
	obj_list<tree_node> list1;
	tree->root->flat_list_locn( list1 );
	int nn = list1.N();
	for(int i=0; i<nn; i++ )
	{
		/*
			This causes a subsequent crash when setting the row data
		*/
		// tree->find_add_gui_of_group( list1[i] );
		/*
			This only works if you added leaves to tree
			(normally there is no need)
		*/
		tree_node * n3 = list1[i];
		if( !n3->children_leaf.N() ) continue;
		row_inst * r1 = n3->children_leaf[0];
		tree->find_add_gui_of_leaf( r1 );
	}
#endif

	if(1) timer->step_print( "add parent groups" );

	// now sort each part of the tree
	// tree->sort_children();

	// build the gui CTree using the cached hierarchy (nodes+leaves)
	clear();
	for(int r = 0; r<nr; r++ )
	{
		row_inst * r1 = tbl->rows_list[r];
		T_LEAF * row = (T_LEAF*) r1;
		tree->find_add_gui_of_leaf( row );
	}
#else
	obj_list<row_inst> list1;
	tree->root->flat_list_leaf( list1 );
	int nrow = list1.N();
	for( int i=0; i<nrow; i++ )
	{
		row_inst * r1 = list1[i];
		tree->find_add_gui_of_leaf( r1 );
	}

#endif

	// node leaves took .4+ sec
	if(1) timer->step_print( "add node leaves" ); // reduce: upto 1 second
	
	// yes it takes 2 seconds - pango strikes here
	// now its 1 minute for not many rows of not many cols, is it me ?
	int nc = ncol;
	for( int c=0; c<nc; c++ )
	{
		col_spec * spec2 = col_list[ c ] ;
		if( c == TREE_COLUMN )
		{
			if( !spec2->display_width_expanded )
				spec2->display_width_expanded = optimal_column_width( c );
			set_column_width( c, spec2->display_width_expanded );
#ifdef WITH_GTK2
/*
	 still here with 2.2

	To see this bug click on the 'amnt' col header
	Its right aligned
*/
			int LURK_gtk_bug_right_aligned_off_right;
			set_column_align_left( c );
#endif
		} else {
			if( !spec2->display_width )
				spec2->display_width = optimal_column_width( c );
			set_column_width( c, spec2->display_width );
		}
	//	set_column_align_right( c ); // demos bug in gtk2
	}
	// recalc_col_widths();
	// recalc_col_width(TREE_COLUMN);
	if(1) timer->step_print( "recalc col widths" );
}



}; // NAMESPACE
