#ifndef table_col_spec_plus_H
#define table_col_spec_plus_H

#include "tbl_col_spec.h"

// previously col_spec was insulated from types by cell_
#include "date_glib.h"
#include "amnt_t.h"
#include "per_cent.h"


namespace ACCTS {


// - amnt -

class col_spec_amnt : public col_spec
{
 public:
	bool show_null_as_error; // default
	bool show_zero_as_empty; // NULL is empty

	col_spec_amnt( str0 name );
	col_spec_amnt( str2 * name );
	virtual obj_ref * new_cell_from_str( str0 s );
	virtual str2 * get_str2_BASE( row_inst * row );
	virtual IS_DIFF_t cmp_rr( row_inst * r1, row_inst * r2 );
 	virtual col_spec * col_spec_clone( table * t2 );
	virtual T_field get_field_type() { return T_amnt; }
	virtual str2 * get_str2_for_display( row_inst * row );

	virtual bool get_amnt( row_inst * row, amnt_t & amnt );
	virtual bool set_amnt( row_inst * row, amnt_t amnt );
};

// - percent -

class col_spec_percent : public col_spec
{
 public:
	col_spec_percent( str2 * name );
	virtual obj_ref * new_cell_from_str( str0 s );
	virtual str2 * get_str2_BASE( row_inst * row );
	virtual IS_DIFF_t cmp_rr( row_inst * r1, row_inst * r2 );
 	virtual col_spec * col_spec_clone( table * t2 );
	virtual T_field get_field_type() { return T_percent; }

	virtual bool get_percent( row_inst * row, per_cent_t & amnt );
	virtual bool set_percent( row_inst * row, per_cent_t amnt );
};

// - date -

class col_spec_date : public col_spec
{
	table * era_tbl;
 public:
	col_spec_date( str2 * name, table * _era_tbl );
	virtual obj_ref * new_cell_from_str( str0 s );
	str2 * get_str2_BASE( row_inst * row );
	table * get_linked_table();
	// the linked row is - the era group ?
	row_inst * get_linked_row( row_inst * r );
	w_widget0 * mk_edit( row_inst * r );
	virtual IS_DIFF_t cmp_rr( row_inst * r1, row_inst * r2 );
 	virtual col_spec * col_spec_clone( table * t2 );
	virtual T_field get_field_type() { return T_date; }

	virtual bool get_date( row_inst * row, date_glib & amnt );
	virtual bool set_date( row_inst * row, date_glib amnt );
};

// - str2 -

class col_spec_str2 : public col_spec
{
 public:
	col_spec_str2( str2 * name );
 // virtual
	obj_ref * new_cell_from_str( str0 s );
	str2 * get_str2_BASE( row_inst * row );
	virtual IS_DIFF_t cmp_rr( row_inst * r1, row_inst * r2 );
 	virtual col_spec * col_spec_clone( table * t2 );
	virtual T_field get_field_type() { return T_str_desc; }
};

// - str2_key -

class col_spec_str2_key : public col_spec
{
 public:
	col_spec_str2_key( str2 * name );
 // virtual
	obj_ref * new_cell_from_str( str0 s );
// str2	obj_ref * new_cell_from_str2( str2 * s );
	str2 * get_str2_BASE( row_inst * row );
	str2 * get_str2_for_csv( row_inst * row );
	table * get_linked_table();
	row_inst * get_linked_row( row_inst * r );
	virtual IS_DIFF_t cmp_rr( row_inst * r1, row_inst * r2 );
	virtual bool set_BASE( row_inst * row, str0 s );
 	virtual col_spec * col_spec_clone( table * t2 );
	virtual T_field get_field_type() { return T_str_key; }
};

// - row_link - into same tbl

class col_spec_link : public col_spec
{
 public:
	col_spec_link( str2 * name );
 // virtual
	obj_ref * new_cell_from_str( str0 s );
	str2 * get_str2_BASE( row_inst * row );
///	table * get_linked_table();
	row_inst * get_linked_row( row_inst * r );
	str2 * get_linked_desc_for_display( row_inst * row );

	w_widget0 * mk_edit( row_inst * r );
	virtual IS_DIFF_t cmp_rr( row_inst * r1, row_inst * r2 );
 	virtual col_spec * col_spec_clone( table * t2 );
	virtual T_field get_field_type() { return T_link_grp; }
};

class col_spec_link_dot_dot : public col_spec_link
{
 public:
	col_spec_link_dot_dot( str2 * name );
 // virtual
//	obj_ref * new_cell_from_str( str0 s );
//	str2 * get_str2_BASE( row_inst * row );
	table * get_linked_table() { return tbl; }
//	row_inst * get_linked_row( row_inst * r );
//	widget0 * mk_edit( row_inst * r );
 	virtual col_spec * col_spec_clone( table * t2 );
	virtual T_field get_field_type() { return T_link_grp; }
};

// - row_link - into tbl2

class col_spec_row_link_key : public col_spec_link
{
 public:
	obj_hold<table> tbl2;
	col_spec_row_link_key( str2 * name, table * _tbl2 );
 // virtual
//	obj_ref * new_cell_from_str( str0 s );
//	str2 * get_str2_BASE( row_inst * row );
	table * get_linked_table() { return tbl2; }
	row_inst * get_linked_row( row_inst * r );
//	widget0 * mk_edit( row_inst * r );
 	virtual col_spec * col_spec_clone( table * tb );
	virtual T_field get_field_type() { return T_link_row; }
};

// - img_id -

class col_spec_img_id : public col_spec_str2
{
 public:
	col_spec_img_id( str2 * name );
 // virtual
	w_widget0 * mk_edit( row_inst * r );
	// obj_ref * new_cell_from_str( str0 s );
	// str2 * get_str2_BASE( row_inst * row );
	// virtual IS_DIFF_t cmp_rr( row_inst * r1, row_inst * r2 );
 	// virtual col_spec * col_spec_clone( table * t2 );
	// virtual T_field get_field_type() { return T_str_desc; }
};


/*

	EXPRESSIONS or VALUES ??

	see ..._EXTRA.cxx and ..._date_rel.h
*/

}; // NAMESPACE
#endif


