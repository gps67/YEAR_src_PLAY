/*
	replaces c_tree_tbl_two
*/

#include "grid_base.h" // GRID::grid_window_0

/*
*/

namespace ACCTS {
//	class row_inst;
	class nb_page_tbl;
	class table;
	class col_spec;
};
using namespace ACCTS;

class grid_tbl : public GRID::grid_window_0
{
 public:
	nb_page_tbl * up;
	table * tbl;

	grid_tbl( nb_page_tbl * _up, table * _tbl, int _sort_col );
	void re_select_by_data( obj_ref * data );          
	void make_row_visible( obj_ref * data );          
	int find_visible_col(  col_spec * spec );
};

