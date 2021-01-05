#include "grid_tbl.h"

/*!
	constructor - table data already exists
*/
grid_tbl::grid_tbl( nb_page_tbl * _up, table * _tbl, int _sort_col )
: GRID::grid_window_0()
{
	up = _up;
	tbl = _tbl;
	TREE_COLUMN = _sort_col; // grid_window_0
}

/*!
	select the row which has data
*/
void grid_tbl::re_select_by_data( obj_ref * data )
{
}

/*!
*/
void grid_tbl::make_row_visible( obj_ref * data )
{
}

/*!
*/
int grid_tbl::find_visible_col(  col_spec * spec )
{
	return -1;
}


