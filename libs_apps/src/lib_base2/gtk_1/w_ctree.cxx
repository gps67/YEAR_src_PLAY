#include "w_ctree.h"

/*!
	STATIC C --> C++ callback: row selected
*/
void w_ctree::C_event_tree_select_row(
        GtkCTree * tree,
        GtkCTreeNode * node,
        gint col,
        w_ctree * app
) {
	if(!app) return;
	app->init_sel();
	app->sel_col = col;
	app->sel_row_gtk = node;
	app->sel_row_data = app->get_row_data( node );
	if(! app->sel_row_data )
	{
		e_print("sel_row_data is NULL\n"); // this is normal for interio
		return;
	}
	app->event_tree_select_row();
	app->init_sel();
}

/*!
	STATIC C --> C++ callback
*/
void w_ctree::C_event_click_column(
        GtkCTree * tree,
        gint col,
        w_ctree * app
) {
        obj_hold<w_ctree> app_keeper; app_keeper = app;
	if(!app) return;
	if(app->sel_row_data) gdb_break_point();
	app->init_sel();
	app->sel_col = col;
	app->event_click_column( );
	app->init_sel();
}

/*!
	clear the selection
*/
void w_ctree::init_sel()
{
        sel_row_gtk = NULL;
        sel_row_data = NULL;
        sel_col = 0;
}

/* --- DEL
void w_ctree::event_tree_select_row()
{
	e_print("event_tree_select_row - subclass should handle\n");
}

void w_ctree::event_click_column()
{
	e_print("event_click_column - subclass should handle\n");
}
*/



/*!
	calc and set the width of column(c)
*/
void w_ctree::recalc_col_width(int c)
{
	int x = optimal_column_width( c );
	set_column_width( c, x );
}

/*!
	calc and set the width of each column
*/
void w_ctree::recalc_col_widths()
{
	int nc = ncol;
	for( int c = 0; c<nc; c++ )
	{
		recalc_col_width( c );
	}
}

/*!
	destructor
*/
w_ctree::~w_ctree()
{
	// e_print("~ w_ctree() - %p\n", this );
}

/*!
	constructor - titles are available - late_init does the work
*/
w_ctree::w_ctree( int n, int treecol, const char * titles[] )
: strs( n )
{
	late_init( n, treecol, titles );
}

/*!
	constructor - dont allocate w yet - titles not available - late_init
*/
w_ctree::w_ctree()
: strs( 10 )
{
	init();
}

/*!
	common code to set all fields to vanilla, and w = NULL
*/
void w_ctree::init()
{
	w = NULL;
	new_node = NULL;

	parent = NULL;
	sibling = NULL;
	// text = strs.get_argv(); // probably never changes

	spacing = 10;	// no effect
	pixmap_closed = NULL;
	mask_closed = NULL;
	pixmap_opened = NULL;
	mask_opened = NULL;

	is_leaf = 0;
	expanded = 1;
	expanded = 0;
	TREE_COLUMN = 0;

	init_sel();
}

/*!
	now that the titles are available, allocate the widget
*/
void w_ctree::late_init( int n, int treecol, const char * titles[] ) // LATE not repeatable
{
	if(w)
	{
		e_print("w_ctree::late_init() but w is not NULL\n");
		return;
	}
	// e_print("w_ctree::late_init() - %p\n", this );
	strs.re_init( n );
	ncol = n;
	init();
	w =  gtk_ctree_new_with_titles(
		ncol,
		treecol,
		(gchar **) titles
	);
	TREE_COLUMN = treecol;
        connect(
                "tree-select-row",
                (SignalFunc) C_event_tree_select_row
        );
        connect(
                "click-column",
                (SignalFunc) C_event_click_column
        );

}

/*!
	Create a new node
	OUTPUT:
		new_node
	INPUTS:
		sel_row_data
		parent
		sibling
		strs
		...
*/
bool w_ctree::do_insert_1()
{
	new_node = gtk_ctree_insert_node(
		CTree(),
		parent, sibling,
	 (gchar**)
		strs.get_argv(),
		spacing, 
		pixmap_closed, mask_closed, 
		pixmap_opened, mask_opened,
		is_leaf,
		expanded
	);

	if(!new_node) return FALSE;
	set_row_data( new_node, sel_row_data );                                 

	// set signals on new node
	return TRUE;
}

/*!
	Change the text of each cell,
	new_node is ALREADY set
	INPUTS:
		new_node
		strs
		sel_row_data
		rows attached data row
	UNUSED:
		...
		sibling
		parent
*/
bool w_ctree::do_update_1()
{
	set_row_data( new_node, sel_row_data );                                 
	int nc = ncol;
	const char ** argv = strs.get_argv();
	for(int col = 0; col<nc; col++)
	{
		gtk_ctree_node_set_text(
			CTree(),
			new_node,	// curr
			col,
			argv[col]
		);
	}
	return TRUE;
}

/*!
	RENAME: connect_select_row()
*/
void w_ctree::on_select_row_tell( FN_tree_select_row * func, w_widget0 * app )
{
	// LURK this is 4 parameters not 2
	connect(
		"select_row",
		(SignalFunc) func,
		(obj_ref *) app
	);
	// this is overdone by c_tree_tbl_base
}

/*!
	set obj as the data on the node row
*/
void w_ctree::set_row_data_full( GtkCTreeNode * node, obj_ref * obj )
{
	if( ! node ) return;
	if( obj ) obj->ref_incr();
	if(0) if( obj ) {
		e_print("set_row_data_full ");
		obj->debug_track_pointer();
	}
	// presume that gtk destroys the previous value 
	gtk_ctree_node_set_row_data_full(
		CTree(),
		node,
		obj,
		(GtkDestroyNotify) C_ref_decr // delete on delete
	);
}

/*!
	set obj as the data on the node row
*/
void w_ctree::set_row_data( GtkCTreeNode * node, obj_ref * obj )
{
	set_row_data_full( node, obj );
}	

/*!
	get obj for the node row
*/
obj_ref * w_ctree::get_row_data( GtkCTreeNode * node )
{
	return (obj_ref *) gtk_ctree_node_get_row_data( CTree(), node );
}

/*!
	select the row corresponding to obj
*/
void w_ctree::re_select_by_data( obj_ref * data )
{
	if(!data) return;
	GtkCTreeNode * node;
	node = gtk_ctree_find_by_row_data( CTree(), NULL, data );
	if( ! node )
	{
		e_print("re_select - node not found(--)\n");
		return;
	}
	// gtk_ctree_node_moveto( CTree(), node, 0, 0.5, 0.0 );
	gtk_ctree_select( CTree(), node ); // emits own signal
}

/*!
	This sometimes works - used to
*/
void w_ctree::make_row_visible( obj_ref * data )
{
	if(!data) return;
	GtkCTreeNode * node;
	node = gtk_ctree_find_by_row_data( CTree(), NULL, data );
	if( ! node )
	{
		e_print("make_row_visible - node not found(--)\n");
		return;
	}
	/*
		this doesnt quite work when the edit-grid first appears
		as 0.5 of the full height is still off, so setting to 0.2
		make it just visible.

		The better alternative is to get this ctree resized
		THEN call this
	*/
	gtk_ctree_node_moveto( CTree(), node, 0, 0.4, 0.0 );
}

/*!
	set the background of the previously selected new row
*/
void w_ctree::set_bg( d_colour * col )
{
	set_bg( new_node, col );
}

/*!
	set the background of the any row
*/
void w_ctree::set_bg( GtkCTreeNode *node, d_colour * col )
{
	gtk_ctree_node_set_background( CTree(), node, & col->colour );
}

