#ifndef ACCTS_sum_tree_H
#define ACCTS_sum_tree_H
// only included by report1_pdf.cxx - currently

#include "obj_hold.h"
#include "dset.h"
#include "tbl_col_spec_plus.h"
#include "dgb_fail.h"
#include "tbl_col_spec_date_rel.h"

#include "e_print.h"
#include "tree_build_1.h" // this will replace tree_build_1
#include "enum_flags.h" // ROW_show_sums

#include "dyn_array.h" // ROW_show_sums


namespace ACCTS {

/*!
*/
class sub_tot_spec : public GRP_lib_accts
{
 public:
	int col_pos;		// find left etc
	col_spec_amnt * spec;

	sub_tot_spec()
	: col_pos(0)
	, spec( NULL )
	{
	}
};

/*!
	adds some data to the tree_node

	ACTUALLY theres no need for this:
	The RPT table can hold the data, and the tree built, used twice
*/
class sum_tree_node : public tree_node
{
 public:
	int depth;

	dyn_array<amnt_t> subtotal;

	sum_tree_node( tree_node * par, T_LOCN * r, int ntotals );
};

/*!
*/
class subtot_spec : public GRP_lib_accts
{
 public:
	int vis_col_pos;
	col_spec_amnt * spec;
};

/*!
	This extends tree_build_1 to add subtotals, on a per-node basis
*/
class sum_tree_build : public tree_build_1
{
 public:
	table * table1;
	dyn_array<subtot_spec> subtotal_specs;
	//subtotal (amnt) is held in individual tree_nodes ;-)

	sum_tree_build();
	sum_tree_build( col_spec * _tree_spec );
	void re_init_2( col_spec * _tree_spec );

// virtual
	tree_node * new_tree_node( tree_node * par, T_LOCN * r );
	str2 * get_key_desc( tree_node * node );
	str2 * get_key( tree_node * node );
	str2 * get_desc( tree_node * node );

	void add_all_subtotals();
	void add_all_subtotals( tree_node * locn );
	void add_one_row( row_inst * row );
	void add_all_rows(); // add all data from selected table1
	int find_idx_spec( col_spec * spec );
	bool get_total( tree_node * node, col_spec * spec, amnt_t & amnt );
	bool get_total_of_visible_row( tree_node * node, amnt_t & amnt );

	bool spec_is_tree_spec( col_spec * spec )
	{
		return spec == s1_tree;
	}
	bool spec_is_desc( col_spec * spec )
	{
		return spec == s1_desc;
	}

	col_spec * s1_tree;	// spec_is_tree_spec()
	col_spec * s1_desc;	// spec_is_desc()
	col_spec * s2_tree;	// UNUSED ?
	col_spec * s2_desc;	// UNUSED ?
};


}; // namespace
#endif
