#ifndef table_col_spec_H
#define table_col_spec_H

#include <gtk/gtk.h> // only for GTK_JUSTIFY_XXX enum type

#include "T_field.h"


#include "obj_ref.h"
#include "obj_hold.h"
// ERR // class ::ACCTS::row_inst;
#include "tbl_row_inst.h"
#include "tbl_cell_base.h"
#include "ints.h" // is_DIFF_t bool and int32
// ERR // class ::ACCTS::table;
#include "table.h" 

#include "align_val.h" 
using namespace BASE1;

// col_spec can generate widgets
class w_widget0;

namespace ACCTS {

//! A col_spec describes a column of a table
/*!
	Each column of a table has a col_spec, which describes the
	type of the data, and the position in the table.

	col_spec is how fields are accessed from rows, how cells
	are sort-compared, and even how cells are 'grouped'
	into a heirarchy.

	There are sub-classes (col_spec_date, col_spec_amnt, ... ) for different types, using virtual functions
	to handle display, loading to/from CSV, and for linking to other tables.

	In future, col_spec's might be used to access sub-fields
	or calculated values, then the 'col_pos' field will be used
	differently.

	The basic stored cell value is obj_ref (almost anything),
	but by controlling init/set you can expect a datatype for get.

	Some derived link types, resolve their cells values in (another)
	table and store the referenced row as a cell value. This allow
	global renames, but currently prevents deletions! ((TODO:
	allow deleted row to be replaced on detect-not-in-table))
*/
class col_spec : public obj_ref
{
	//! col_spec does some of tables work, it  can access members
	friend class table;
	friend class coll_spec;
	friend class cell_base;

	// private prevents copy construction (things need to be customised)
	col_spec( const col_spec * spec2 );

 protected:
 public:
	//! the table that owns this column
	table *		tbl;

	//! the name of the column in the table
	obj_hold<str2>	name;

	//! the column header - if not the name
	obj_hold<str2>	name_hdr;

	//! the store position of the column in the table
	int		col_pos;	// TODO - remove this to subclass

	//! right or left alignment in files and display
//	GtkJustification justification; // plain enum
	align_val align_just; // wrapped enum

// funcs

	//! add this col_spec to tbl - used during table build
	void bind_as_tbl_col( table * t, int col );

// public:

	//! get a cell value from a row
	obj_ref *	get_obj( row_inst * row );

	//! put a cell value into a row
	void 		set_obj( row_inst * row, obj_ref * cell );

 public:

	bool	is_hidden;	// single VIEW of table, omits this col
	bool	is_summable;	// ie reports WILL show sums
	bool	is_blankable;	// ie reports WILL show nothing when zero

	//! some general initialisation
	void init0();
	//! create a new col_spec - still needs to be placed into context
	col_spec( str0 name_ );
	col_spec( str2 * name_ );

	// how you like the display field formatted
	align_val get_align();

	void set_align_left();
	void set_align_right();
	void set_align_center();

	str2 * get_name();
	str0 get_name_str();	// mostly for debugging

	str2 * get_name_hdr();	// name_hdr fallback on name
	void set_name_hdr( str2 * hdr );

	//! Compare two rows by this cell (must be same table)
	virtual IS_DIFF_t cmp_rr( row_inst * r1, row_inst * r2 );

	/*!
		create a new data cell from a plain string

		This can return a linked ROW, when the cell
		is just a key. Usually a cell is a date, anmt
		or string. Commas are already plain 8859.
	*/
	virtual obj_ref * new_cell_from_str( str0 s );

	virtual obj_ref * new_cell_from_NULL();

	//! create a COPIED col_spec - FUTURE WORK
 	virtual col_spec * col_spec_clone( table * tbl2 ) = 0;

	//! set the cells value from an 8859 string
	virtual bool set_BASE( row_inst * row, str0 s );

	//! set the cells value from an 8859 string
	virtual bool set_from_csv( row_inst * row, str0 s );

	//! set the cells value from an 8859 string
	virtual bool set_from_edit( row_inst * row, str0 s );

	/*
		It would probably be better to only have _BASE
		but a cell might have a human readable abbreviation
		for use in displays, with a full data-str in csv files.
	*/
	virtual str2 * get_str2_BASE( row_inst * row );
	virtual str2 * get_str2_for_display( row_inst * row );
	virtual str2 * get_str2_for_edit( row_inst * row );
	virtual str2 * get_str2_for_csv( row_inst * row );

	//!	linked keys point to a (second) table
	virtual table * get_linked_table();

	/*!
		The group of a row is the 'parent' when that
		column is the TREE_COLUMN in a Gtk tree. For example

		A cell is: TRANS[thisrow].acct1

		The group is: ACCT[thatrow] -- a row_inst not a cell

		acct1_spec->get_group_row(thisrow) --returns--> thatrow
	*/
	virtual row_inst * get_linked_row( row_inst * r );

	/*
		special call for linked fields
	virtual str2 * get_key_desc_for_display( row_inst * r );
	virtual str2 * get_linked_desc_for_display( row_inst * r );
	*/

	//! Here for when it makes sense
	str2 * get_linked_key_desc( row_inst * r );

	//! For when the link has already been resolved
	str2 * get_key_desc( row_inst * r );

	virtual w_widget0 * mk_edit( row_inst * r );
	virtual w_widget0 * mk_edit_combo_link( row_inst * row );

	virtual T_field get_field_type() = 0;

	void prints(); // debug thing

	int display_width_expanded; //!< guess or cache the display width
	int display_width;	//!< guess or cache the display width

	bool is_connected()
	{
		return this && tbl;
	}

};
}; // NAMESPACE

#endif

