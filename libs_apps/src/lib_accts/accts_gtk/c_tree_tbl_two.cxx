
#include "nb_page_tbl.h" 	// strange signal routing
#include "c_tree_tbl_two.h"
#include "tbl_row_inst.h"

namespace ACCTS {

/*!
	destructor - plain
*/
c_tree_tbl_two::~c_tree_tbl_two()
{
	if(0) e_print("~c_tree_tbl_two(%s)\n", (STR0) tbl->get_name_str() );
}

/*!
	constructor - initialise baseclass (_tbl,treecol) and record app
*/
c_tree_tbl_two::c_tree_tbl_two(
	nb_page_tbl * _app,
	table * _tbl,
	int treecol
)
: c_tree_tbl_one( _tbl, treecol )
{
	if(0) e_print("c_tree_tbl_two(%s)\n", (STR0) tbl->get_name_str() );
	up = _app;
}

/*!
	clicking on the column (header) causes the app to regenerate
	the table sorted by that column.
*/
void c_tree_tbl_two::event_click_column()
{
	if(0) fprintf(stderr,"c_tree_tbl_two::event_click_column() col %d\n",  sel_col );
	if( sel_col == TREE_COLUMN ) return; // ctree
	up->change_col_sort( sel_col );
}

/*!
	clicking on a row causes the app to make an edit form for that row
*/
void c_tree_tbl_two::event_tree_select_row()
{
	row_inst * row = get_sel_row_inst();
	if(0) e_print("c_tree_tbl_two::event_tree_select_row(ctree,row[%s],col=%d,app)\n",
	(STR0) 	row->debug_row_name(),
		sel_col
	);
	up->row_selected( row );
}


}; // NAMESPACE
