/*
 TODO:

	Rearrange the column sequences
		dev/attr --  attr/dev
		Put dt/avg at top
	
	Use different selectors for site, 
*/

# include "sar_top_layout.h"
#include "dset_api.h"
#include "w_btn_plus.h"
#include "w_frame.h"

#define COL3 3

/////////////////////// btn_sar_layout //////////////////////////////

class btn_sar_layout : public w_btn_plus
{
 public:
	obj_hold<mem_line_tree_node>  node;
	// mem_line_tree_node * node;
	//
	btn_sar_layout(
		const char * label,
		mem_line_tree_node * _node
	)
	: w_btn_plus( label )
	{
		node = _node;
	}
	~btn_sar_layout()
	{
		// why are all these called nothing
		if(0) {
			e_print("# DEL btn '%s' ", (STR0) node->name() );
			debug_print_ln();
		}
	}
	//
	void event_clicked();
	//
	void set_selected()
	{
		set_bg( "green" );
	}
};

void  btn_sar_layout::event_clicked()
{

	if(0) e_print( "BTN %s\n", (STR0) node->name() );
	mem_line_tree * tree = node->treetop;

	mem_line_tree_node * parent = tree->root->find_prefix(
		node->depth.prev(),
		& node->leaf->spec
	);
	if(!parent) {
		return;
	}
	parent->sel_branch( node );

	sar_top_layout * layout = (sar_top_layout * ) tree->client_data;
	if(!layout) e_print("NULL layout\n"); else // never happens
	layout->redo_subtree( parent );
}

/////////////////////// sar_top_layout //////////////////////////////

#include "sar_top_app.h"

// static
void sar_top_layout:: C_del1( GtkWidget *wid, gpointer data )
{
	sar_top_layout * that = (sar_top_layout*) data;
	gtk_container_remove( GTK_CONTAINER(that->w), wid );
	
}

void sar_top_layout::tell_top_app_layout_selected_leaf( mem_line_spec * spec )
{
	if( !top_app ) {
		e_print("top_app is NULL\n");
		if(1) spec->print1(stderr);
		return;
	}
	top_app->layout_selected_leaf( spec );
}

sar_top_layout:: sar_top_layout( sar_top_app * _top_app )
: w_hbox()
, cols() // init empty
{
	top_app = _top_app;
	// cols = keeper;
	// GTK_POLICY_NEVER - encourages gtk to make it full width
	// otherwise its absurd
	w_win_scrolled * col_1_holder = new w_win_scrolled(
		GTK_POLICY_NEVER,
		GTK_POLICY_AUTOMATIC
	);
	pack_asis( col_1_holder );

	col_1 = new w_vbox() ;
	col_1_holder->pack_unscrollable( col_1 );
}

sar_top_layout:: ~sar_top_layout()
{
	/*
		There are issues with circular refs, so manually
		clear down the cols and line_tree, then let
		the default destructor delete what is empty
	*/
	if(0) e_print("~sar_top_layout() - start\n");
	if(0) e_print(" empty-from depth \n");
	empty_from_depth(0);
	if(0) e_print("~sar_top_layout() - end .. now subs\n");
}

void sar_top_layout::dismantle()
{
	empty_from_depth(0);
	col_1 = NULL;
	cols.rezero();
	if(0) e_print(" line_tree=NULL \n");
	// destroy(); // should delete the outer frame then the inners?
	line_tree = NULL;
	if(0) e_print(" dismantled() \n");
}

void sar_top_layout:: empty_from_depth( int col )
{
//  gtk_container_foreach( GTK_CONTAINER(w), C_del1, this );
    int n = cols.N();
    for( int i=n-1; i>=col; i-- )
    {
	if(0) e_print("empty_from_depth(%d) at %d\n", col, i );
	/*
		I've not been counting pointer from w_widget to w
		so when item is removed from container, it is also
		deleted, even though I still have a pointer to w.

		((adding that counter might prevent GTK from
		telling me when object is off-screen-deleted ))

		Any use of w in destructor (or other subsequent)
		is a tad illegal
	*/
	// delete duplicate reference first
	w_widget0 * item = cols[i];
	cols.del(i);
	if( i < COL3 )
	{
		col_1 -> remove( item );
	    // gtk_container_remove( GTK_CONTAINER(col_1->w), cols[i]->w );
	} else {
		this  -> remove( item );
	    // gtk_container_remove( GTK_CONTAINER(w), cols[i]->w );
	}
    }
}

void sar_top_layout:: do_single_depth( mem_line_tree_node * node0 )
{
	int b_sel = node0->sel;

	int n = node0->branches.N();
	if(0) e_print("set_line_tree() %d branches\n", n );
	if( n==0) {
		e_print("set_line_tree() ZERO branches\n" );
		return;
	}
	if(!n) return;

	obj_hold<btn_col> col;
	obj_hold<w_vbox> col_plain;
	int column_depth = node0->depth.map_attr + 1;

	const char * attr_name = node0->branches[0]-> get_attr_name();
	// pango has ruined label, bring back Tk
	// w_label * lbl = new w_label( attr_name );

	/*
		frm is a frame with a label
	*/
	w_frame * frm = new w_frame( attr_name );

	if( column_depth < COL3 )
	{
		col_plain = new w_vbox();
		frm->pack( col_plain );		// holds a ref to col_plain
		col_1->pack_asis( frm );
		int LURK_this_is_all_wrong;
	} else {
		col = new btn_col();
		frm->pack( col );
		this->pack_asis( frm );
	}
	// cols is a second list of widgets, used to find col[depth]
	cols.append( frm );

	mem_line_tree_node * branch;

	for(int b=0; b<n; b++)
	{
		branch = node0->branches[b];
		assert( branch );
		const char * name = branch->name();
		obj_hold<btn_sar_layout> btn = new btn_sar_layout(
			name,
			branch
		);
		if( column_depth < COL3 )
			col_plain->pack( btn );
		else
			col->append( btn );

		// this evaporates on OMO
		// gtk_widget_set_state( btn->w, GTK_STATE_SELECTED );

		if( b == b_sel )
		{
			btn->set_selected();
		}
	}

}

void sar_top_layout:: redo_subtree( mem_line_tree_node * node0 )
{

	if(!node0)
	{
		e_print("redo_subtree(NULL)\n");
		e_print("Happens when tree is NULL");
		return;
	}

	// node0 is the PARENT of the first col drawn
	// root has depth -1, col is depth+1

	int column_depth = node0->depth.map_attr + 1;
	empty_from_depth( column_depth );

	if( !node0 ) {
		e_print("sar_top_layout::() NULL node0\n" );
		return;
	}
	
	if( node0->is_leaf_node() ) // building list of branches
	{
		tell_top_app_layout_selected_leaf( 
			& node0->leaf->spec
		);
		return;
	}

	do_single_depth( node0 );

	int sel = node0->sel;
	if( sel < 0 ) {
		e_print("redo_subtree() sel=%d\n", sel );
		return;
	}

	if(0) e_print("# OK # redo_subtree() sel=%d\n", sel );

	redo_subtree(
		node0->branches[sel]
	);
}

void sar_top_layout:: set_line_tree_from_dset( dset_api * dset )
{
	/*
		? delete old tree ?
		This fixes deletion of some weak circular links
		Call this BEFORE deleting the line_tree
	*/
	empty_from_depth(0);

	/*
		This list can be in any order, maybe loaded from script
	*/
	str0 mymaps[8] = {
		"site",
		"host",
		"cmd",
		"dt",
		"avg",
		"opt",
		"attr",
		"dev"
	};

	// allocate line_tree (or clear-down old ?)
	line_tree = new mem_line_tree( mymaps );
	line_tree->client_data = this; // used by event_clicked

	// use a TEMP dset_api tool to wrap tree to load data
	// select causes root to become non-NULL - redesign?
	dset_api_line_tree dset2( line_tree ); 
	dset->select_ALL_specs_to_dset( & dset2 );

	redo_subtree( line_tree->root );
}
