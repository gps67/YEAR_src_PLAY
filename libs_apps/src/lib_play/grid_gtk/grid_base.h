/*
	replaces c_tree_tbl_two
*/

#include "dyn_array.h"
#include "w_drawing_area.h"
#include "w_scale.h"
#include "w_table.h"
#include "w_vbox.h"

#include "d_draw.h"

/*
*/

namespace ACCTS {
	class row_inst;
	class nb_page_tbl;
	class table;
};
using namespace ACCTS;

namespace GRID {

class grid_window_0;

/*!
	info about each column
*/
struct grid_col_info : public GRP_lib_play
{
	int width;
	grid_col_info();
};

/*!
	info about each row, and the data pointer
*/
struct grid_row_info : public GRP_lib_play
{
	int height;
	row_inst * data;

	grid_row_info();
};

/*!
	The basic grid drawing and data access

	As each cell is drawn the coords are placed in the base class
*/
class w_grid_area : public w_drawing_area
{
 public:
	int nrows;
	int ncols;
	d_text text1;
	d_draw draw;
	d_draw draw_white;

	int row_y0;
	int row_y1;
	int row_y2;
	int row_y3;
	int cell_x0;
	int cell_x1;
	int cell_x2;
	int cell_x3;
	int cell_row;
	int cell_col;

	w_grid_area( grid_window_0 * _up );
	grid_window_0 * up;	
	void pixmap_reallocated();
	void draw_some();
	virtual void draw_cell();
	void fill_cell( d_draw & draw_colr );
};

/*!
*/
class w_grid_titles : public w_grid_area
{
 public:
	w_grid_titles( grid_window_0 * _up );
 virtual                                                                        
	void event_size_request( GtkRequisition *requisition );                 
};

/*!
*/
class w_grid_cells : public w_grid_area
{
 public:
	w_grid_cells( grid_window_0 * _up );
 virtual                                                                        
	void event_size_request( GtkRequisition *requisition );                 
};

/*!
	two windows, titles and cells
	with horizontally connected scrolling
*/
class grid_window_0 : public w_table
{
 public:

	w_grid_titles * area_titles;
	w_grid_cells * area_cells;

	w_scale * vscroll;
	w_scale * hscroll;

	g_adjustment * hadj;
	g_adjustment * vadj;

	dyn_array<grid_col_info> col_info;
	dyn_array<grid_row_info> row_info;

	int TREE_COLUMN;

	grid_window_0();
};

}; // namespace
