
#include "w_win_scrolled.h"
#include "nb_page_tbl.h"
#include "table.h"
#include "tbl_row_inst.h"
#include "tbl_col_spec.h"
#include "accts_top_app.h"
#include "c_tree_tbl_two.h" // OLD
#include "grid_tbl.h"	// NEW
#include "w_table.h"
#include "w_label.h"
#include "w_dialog_msg.h"

namespace ACCTS {

/*!
*/
void C_insert(
	GtkWidget * w,
	nb_page_tbl * self
) {
	self->insert_new_row();
}

/*!
*/
void C_delete(
	GtkWidget * w,
	nb_page_tbl * self
) {
	if( !self ) return;
	self->delete_current_row();
}

/*!
*/
void C_replace(
	GtkWidget * w,
	nb_page_tbl * self
) {
	self->replace_current_row();
}

/*!
*/
w_btn_base * nb_page_tbl::mk_btn(
	str0 s,
	FN_callback_nb_page_tbl * func
)
{
	return new w_btn_base( s, this, (SignalFunc) func );
}

/*!
*/
 nb_page_tbl::nb_page_tbl(
		accts_top_app * _app,
		table * _tbl,
		int sort_col,
		row_inst * _sel_row
 )
 : w_vpaned() // this - late_init packs two widgets
// /*
 , top( _app )
 , tbl( _tbl )
 , curr_row()	// obj_hold = NULL
 , ctree()	// obj_hold = NULL
 , edits()	// obj_hold = NULL
 , edit_field_widgets()	// obj_list = empty
// */
 {
	// retain these settings
	tbl = _tbl;
 	if(0) e_print("nb_page_tbl::nb_page_tbl(%s)\n", (STR0) * tbl->name );
	top = _app;
	curr_row = _sel_row;

	edits = new w_table( 2, 4 );
	edits->set_bg("white");

#ifdef OLD_WAY
	ctree = new c_tree_tbl_two( this, tbl, sort_col );
	w_win_scrolled * scrolled = new w_win_scrolled();
	scrolled->pack( ctree );
	late_init( scrolled, edits  ); // v_paned
#else
	ctree = new grid_tbl( this, tbl, sort_col );
	late_init( ctree, edits  ); // v_paned
#endif

	rebuild_edit_table( NULL );
	ctree->re_select_by_data( curr_row );

	/*
		re_select_by_data causes Gtk to emit tree-select-row
		which calls C_tree_select_row 
		which calls (c_tree_tbl_two::) event_tree_select_row
		which calls (nb_page_tbl::) up->row_selected()
		which calls ctree->make_row_visible( row );
		which doesnt quite do it when changing columns

	col
		CALLS: c_tree_tbl_two::event_click_column()
		CALLS: (nb_page_tbl::) up->change_col_sort( sel_col );
		CALLS: (accts_top_app::) top->replace_page( idx, tbl, sel_col, curr_row );
		NB curr_row passed
		CALLS: 

	*/
 }

// VIRTUAL
/*!
	NORMALLY the widget has been removed from the notebook.
	Its 'w' is invalid

	However the C++ code PROBABLY has a reference to it,
	and we need to remove all 'w' pointers before the dtor is called.
*/
void nb_page_tbl::event_destroyed()
{
	debug_track_pointer();

	edits = NULL;
	dismantle();
	w = NULL; // done by dismantle ??
}


/*!
*/
 nb_page_tbl::~nb_page_tbl()
 {
	const char * s1 = "X"; if(tbl) s1=(STR0) *tbl->name;
 	if(0) e_print("nb_page_tbl::~nb_page_tbl(%s)\n", s1 );
	/*
		The page has been replaced, and now released.
		This (w) is probably junk, but still need to
		dismantle properly ...
	*/
	w = NULL;
 }

/*!
*/
 void nb_page_tbl::row_selected( row_inst * row )
{
	curr_row = row;
	rebuild_edit_table( row );
	// LURK - does this require an idle loop to get new widget height
	ctree->make_row_visible( row );
}

/*!
*/
 void nb_page_tbl::rebuild_edit_table( row_inst * row )
 {
	edits->clear();
//	const char * s1 = "NIL";
//	if( row ) s1 = row->key_str();
//	if(0) e_print("rebuild_edit_table(row-key=%s)\n", s1 );
	if(!row) return;
	int nc = tbl->col_specs.N();
	edits->resize( 2, nc );
//	edits->resize( 5, nc );
	// each col of tbl becomes a row of edit
	// w_table * edits = new w_table( 2, nc );
	int row_pos = 0;
	int col_pos = 3; // wide ones

	w_btn_base * btn1 = mk_btn( "Insert", C_insert );
	edits->add( 0, 0, btn1 );
	if( ! tbl->group_spec )
	{
		w_btn_base * btn2 = mk_btn( "Delete", C_delete );
		edits->add( 0, 1, btn2 );
	}
	w_btn_base * btn3 = mk_btn( "Replace", C_replace );
	edits->add( 0, 2, btn3 );
	// NO // w_btn_base * btn4 = new w_btn_base( "THING", this,  C_insert );
	// OK //w_btn_base * btn4 = new w_btn_base( "THING", this, (SignalFunc) C_insert );
	// edits->add( 0, 3, btn4 );

	edit_field_widgets.rezero();
	for( int c = 0; c < nc; c++ )
	{
		col_spec * spec = tbl->col_specs[c];

		str0 s_lbl = spec->get_name_str();
		w_label * lbl = new w_label( s_lbl );
		lbl->justify_right(); // NOOP ??

		w_widget0 * edit = spec->mk_edit( row );
		if(!edit)
			continue;
		edit_field_widgets.append( edit );
		edit->set_data("col_spec", spec );

		int lh = edit->approx_line_height();
		if( lh > 2 )
		{
			// auto // edits->resize( col_pos+1, h+1 );
			edits->add( col_pos, 0, lbl,  1, 1 );
			edits->add( col_pos, 1, edit, 1, lh );
			col_pos++;
		} else {
			// auto // edits->resize( w, h );
			edits->add( 1, row_pos, lbl );
			edits->add( 2, row_pos, edit );
			row_pos++;
		}
	}
 }

/*!
*/
 void nb_page_tbl::change_col_sort( int sel_col )
 {
	int idx = -1;
	if( top->book->get_idx_of_widget( this, idx ) )
	{
		top->replace_page( idx, tbl, sel_col, curr_row );
	}
 }

/*!
*/
 void nb_page_tbl::reload_this_table()
 {
	int old = ctree->TREE_COLUMN;
	ctree->TREE_COLUMN = -1;
	change_col_sort( old ); // special flag
 }

/*!
*/
 void nb_page_tbl::delete_current_row()
 {
	gdb_break_point();
//	if( curr_row ) e_print("delete %s\n", curr_row->key_str());
	tbl->delete_row( curr_row );
	curr_row  = NULL;
	reload_this_table();
 }

/*!
*/
 void nb_page_tbl::replace_current_row()
 {
//	if( curr_row ) e_print("replace%s\n", curr_row->key_str());
	if( !curr_row ) return;
	// who cares about renameing the key field ;-)
	int n = edit_field_widgets.N();
	for( int i = 0; i < n; i++ )
	{
		w_widget0 * edit =  edit_field_widgets[i];
		col_spec * spec = (col_spec *) edit->get_data("col_spec");
		if( !spec ) {
			e_print("NULL col_spec from edit widget\n" );
			continue;
		}
		str0 val = edit->get_edited_value_as_cstr();
		spec->set_from_edit( curr_row, val );
	}

	gdb_break_point();
	// tbl->delete_row( curr_row );
	reload_this_table();
 }

 void nb_page_tbl::insert_new_row()
 {
	e_print("Insert\n");
	gdb_break_point();
	int n = edit_field_widgets.N();
	if(!n) return;
	col_spec * key_spec = tbl->key_spec;
	if( !key_spec ) return;
	w_widget0 * key_edit = NULL;
	for(int i=0; i<n; i++)
	{
		w_widget0 * edit =  edit_field_widgets[i];
		col_spec * spec = (col_spec *) edit->get_data("col_spec");
		if( spec == key_spec ) {
			key_edit = edit;
			break;
		}
		if( !spec ) {
			e_print("NULL col_spec from edit widget\n" );
			continue;
		}
		str0 val = edit->get_edited_value_as_cstr();
		spec->set_from_edit( curr_row, val );
	}
	if( ! key_edit )
	{
		e_print("INSERT - no key field found\n");
		return;
	}
	str0 key_val = key_edit->get_edited_value_as_cstr();
	obj_hold<str2> key_str; key_str = new str2( key_val );
	if(!key_str) return;
	row_inst * key_row = tbl->find_row( key_str );
	if( key_row )
	{
		e_print("A row with key '%s' already exists\n", (STR0) key_val );
		w_dialog::msg_failed( this, "Insert Failed", "A row with that key already exists" );
		return;
	}
	key_row = tbl->find_row_add_fake( key_str );
	curr_row = key_row;
	for(int i=0; i<n; i++)
	{
		w_widget0 * edit =  edit_field_widgets[i];
		col_spec * spec = (col_spec *) edit->get_data("col_spec");
		if( spec == key_spec ) {
			// continue;
		}
		if( !spec ) {
			e_print("NULL col_spec from edit widget\n" );
			continue;
		}
		str0 val = edit->get_edited_value_as_cstr();
		spec->set_from_edit( curr_row, val );
	}
	reload_this_table();
 }

/*!
*/
 void nb_page_tbl::change_col_sort( str0 specname )
 {
	col_spec * spec = tbl->get_col_spec( specname );
	if(!spec) return;

	int vis_pos = ctree -> find_visible_col( spec );
	if( vis_pos < 0 ) return;

	change_col_sort( vis_pos );
 }


}; // NAMESPACE



