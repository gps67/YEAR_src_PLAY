#ifndef rep_html_H
#define rep_html_H

#include "dgb.h"
#include "dgb_fail.h"
#include "fd_hold.h"
#include "dset.h"
#include "sum_tree.h"
#include "rpt_help_tbl.h"

class col_spec;

namespace ACCTS {

class rep_html  : public GRP_lib_accts
{
 public:
	data_set * dset;
	obj_hold<fd_hold_1> fd_;
	fd_hold_1 & fd;
	help_INFO info; // INFO is a new MACRO

	table * tbl;
	obj_hold<sum_tree_build> p_tree;
	sum_tree_build & tree;

	col_spec * img_spec;

	int ncol; // all columns are visible
	obj_list<col_spec> col_list;
	dyn_array<align_val> just_align;

	void build_col_list();
	void build_tree();

	rep_html( data_set * _dset );
	~rep_html();

	str0 href_for_row( row_inst * row );

	bool opens_titles( str0 filename );
	bool opens( str0 filename, str0 title );
	void draw_td( str0 data, str0 align, str0 clas, str0 href );
	void draw_td( str2 * data, str0 align, str0 clas, str0 href );
	void draw_title_page();
	void draw_tr_th();
	void draw_tr_td( row_inst * row );
	void draw_tr_group_heading( tree_node * grp );
	bool draw_table(
		str0 tablename,
		str0 treecol
	);
	bool closes();
	
};

}; // NAMESPACE
#endif
