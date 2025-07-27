#ifndef rep_pdf_0_H
#define rep_pdf_0_H
// only included by report1_pdf.cxx - currently

#include "pdf_base.h"
#include "pdf_pen_base.h"




#include "run_report.h"

#include "obj_hold.h"
#include "dset.h"
#include "tbl_col_spec_plus.h"
#include "dgb_fail.h"
#include "tbl_col_spec_date_rel.h"

#include "e_print.h"
#include "tree_build_1.h"
#include "enum_flags.h" // ROW_show_sums

#include "sum_tree.h" // replaceing rep_tree_node

#include "align_val.h" // left-center-right
using namespace BASE1;
using namespace PDF_GEN;

namespace ACCTS {

/*!
	layout column headers - centralise 3 line headers, split_at_underline
*/
class Layout_Hdrs : public GRP_lib_accts
{
 public:
	flt80 width;
	flt80 height;

	void layout_a_b_c( pdf_pen_text * pen, str0 str );
	void draw_a_b_c(
		pdf_pen_text * pen,
		flt80 x_l,
		flt80 x_r,
		flt80 y_t,
		flt80 y_b,
		str0 str
	);
	Layout_Hdrs()
	{
		width = 0;
		height = 0;
	}
};

/*!
	lots of table drawing functionality
*/
class rep_pdf_0 : public PDF_GEN::pdf_base
{
 public:
	str0 filenamepdf;
	data_set * dset;

        table * tbl;	//  = dset->get_table( "Categorised" );                 
	col_spec_amnt * spec_amnt;
	// I dont want it to be an obj_ref_0 but it is,
	obj_hold<sum_tree_build> tree;

	col_spec * s1_tree;
	col_spec * s1_desc;
	col_spec * s2_tree;
	col_spec * s2_desc;

	obj_list<col_spec> col_list;
	dyn_array<flt80>  widths;
	dyn_array<align_val> just_align;
	int ncol;
	dyn_array<flt80>  x_left;
	dyn_array<flt80>  x_title_left;
	dyn_array<flt80>  x_title_right;
	dyn_array<flt80>  x_right;
	dyn_array<flt80>  y_horiz_lines;
	dyn_array<flt80>  x_vert_lines;

	Layout_Hdrs	layout_hdrs;
	flt80		titles_height;

	obj_hold<pdf_pen_draw> line_borders;
	obj_hold<pdf_pen_draw> fill_titles;
	obj_hold<pdf_pen_draw> fill_titles_small;
	obj_hold<pdf_pen_draw> fill_totals;

// HOLD is a PTR or a REF or a SYNTAX REWRAP
#define	HOLD_pdf_pen_text obj_hold<pdf_pen_text>

//	obj_hold<pdf_pen_text> pen_titles;
//	obj_hold<pdf_pen_text> pen_titles_small;
//	obj_hold<pdf_pen_text> pen_normal;
//	obj_hold<pdf_pen_text> pen_subtots;

	HOLD_pdf_pen_text pen_titles;
	HOLD_pdf_pen_text pen_titles_small;
	HOLD_pdf_pen_text pen_normal;
	HOLD_pdf_pen_text pen_subtots;

	dyn_array<sub_tot_spec>	subtotals;

	void x_out();

	flt80 x_margin_left;
	flt80 x_gap_left;
	flt80 x_gap_right;
	flt80 x_side_left;
	flt80 x_side_right;
	flt80 y_page_top;
	flt80 y_page_end;
	flt80 y_page_min_break;
	flt80 y_min_orphan;
	flt80 y_normal_line_height;
	flt80 y_gap_above;
	flt80 y_gap_below;
	flt80 y_pos;
	flt80 y_gap_between_nodes;

	void size_col_request( int vis_col, flt80 w );

	bool full_key_path; 

	rep_pdf_0( pdf_ctxt & _pdf, data_set * _dset, str0 filenamepdf );

	virtual void on_page_top();


	void build_col_list();
	void build_tree();
	void size_row( row_inst * r );
	void set_x_left();
	void draw_titles_small();
	void draw_tree_titles_small();
	void mid_title( str0 title, int gap_pre = 0, int gap_post = 0 );
	void mid_title_gap( int gap = 1 );
	void draw_row( row_inst * row );
	void draw_row_node( tree_node * locn );
	void do_page_break();
	void sub_each( dyn_array<flt80> & ary, flt80 sub );
	void add_each( dyn_array<flt80> & ary, flt80 sub );

	void show_EUROVAT( bool show ); // too wide for summary
	virtual bool open_up( str0 filenamepdf, str0 title, bool _landscape );
	virtual bool draw_table(
		str0 tablename,
		str0 treecol,
		str0 totcol,
		bool _landscape,
		bool full_key_path = true, // looks shabby for era keys
		int interior_depth = -1,
		bool interleave = true,
		bool show_leaves = true,
		str0 sum_tablename = (STR0) NULL
	);
	virtual void create_pens();
	virtual void draw_tree_node( sum_tree_node * node );
	virtual void draw_node( sum_tree_node * node );
	virtual void draw_summary_line( sum_tree_node * node );
	virtual void draw_group_title( tree_node * node );

	void draw_subtotals( sum_tree_node * node );
	void size_subtotals( sum_tree_node * node );
	void size_subtotals( tree_node * node );

	void draw_grid_lines();

	virtual bool run_wrapped_report() = 0; 

	static bool pdf_doc_from_table( table * tbl, str0 name );

	bool show_hidden_cols;
	bool show_hidden_tbls;

	bool sum_table(
		table * tbl,
		col_spec * spec_totcol,
//		bool _landscape,
//		bool full_key_path = true, // looks shabby for era keys
//		int interior_depth = -1,
//		bool interleave = true,
//		bool show_leaves = true,
		str0 sum_tablename
	);


};

}; // NAMESPACE
#endif
