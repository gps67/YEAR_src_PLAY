#ifndef nb_page_tbl_H
#define nb_page_tbl_H

#define OLD_WAY

#include "obj_list.h"
#include "w_vpaned.h"	// base class
#include "w_btn_base.h"

class w_win_scrolled;
class w_table;

namespace ACCTS {
	class accts_top_app;
	class table;
	class row_inst;
	class c_tree_tbl_two;
};

namespace ACCTS {

class nb_page_tbl;

	class grid_tbl;

// doxygen complains
//  Warning: member with no name found.

typedef void (FN_callback_nb_page_tbl)( GtkWidget *, nb_page_tbl *);

/*!
	Each page of the parent notebook (this) is a v_paned pair
	of (ctree, edits ). The initial edits tends to be empty,
	but resizes when a row is selected and it is allocated.

	Some signals are routed through the top app.

	The CLIST is iself held in a scrolled container

*/
class nb_page_tbl : public w_vpaned
{
	//! the accts_top_app needs to know a few signals
	accts_top_app		*		top; // weak

	//! the table viewed in the notebook page
	table		*		tbl; // weak
	//! the currently selected row (data)
	obj_hold<row_inst>		curr_row;

/*
	The parent environment looks like:

	+-----------	w_top_level
	|+----------	vbox
	||( QUIT )	btn
	||( File Edit )	mbar
	||+-----------	vbox	"app_area"
	|||+----------	notebook
	|||| [A] [B]
	||||+--------	notbook inner
	||||| +------	w_vpaned		THIS
	||||| |
	||||| |
	||||| +-----------
	||+----------
	||( Status line
	|+----------
	+----------------------
*/

/*
	+-----------	w_vpaned		this
	| +--------- 	w_scrolled		(anon)
	| | +-------	ctree_tbl_two		ctree
	| | |
	| | +---------
	| +-----------
	|  - - - - 
	| +-----------	w_table			edits
	| | +-+ +-+
	| | | | | |				(anon)
	| | +-+ +-+				(keeplist=edit_field_widgets)
	| +-------------
	+---------------
*/

	//! the scrolled tree is the main part of the view
#ifdef OLD_WAY
#else
#endif
#ifdef OLD_WAY
	obj_hold<c_tree_tbl_two>	ctree;
#else
	obj_hold<grid_tbl>		ctree;
#endif

	//! edits holds scrolled and a few surrounding things
	obj_hold<w_table> 		edits;

	//! view for editing a row
	obj_list<w_widget0>		edit_field_widgets;
 public:

	//! constructor to (re-)build view with a row already selected
	nb_page_tbl(
		accts_top_app * _app,
		table * _tbl,
		int sort_col,
		row_inst * sel
	);
	 ~nb_page_tbl();
	void event_destroyed();
	
	 void row_selected( row_inst * row );
	 void rebuild_edit_table( row_inst * row );
	 void change_col_sort( int col_num );
	 w_btn_base * mk_btn( str0 s, FN_callback_nb_page_tbl * func );
	// buttons
	 void delete_current_row();
	 void replace_current_row();
	 void insert_new_row();
	 void reload_this_table();

	 void change_col_sort( str0 specname );

};

}; // NAMESPACE

#endif
