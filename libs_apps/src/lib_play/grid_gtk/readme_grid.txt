grid_base.h:class row_inst;
grid_base.h:	class nb_page_tbl;
grid_base.h:	class table;
grid_base.h:class grid_window_0;

grid_base.h:
-----------

grid_col_info
	width

grid_row_info
	height
	row_inst * data

	IF this is a tree - this should have a list of sub-rows

w_grid_area
	based on a w_drawing_area
	ncols nrows

w_grid_title - a one line grid
w_grid_cells - a grid

grid_window_0 - two scrolled joined grids, titles+cells

grid_tbl.h:
----------
grid_tbl : public grid_window_0
	adds table * tbl
