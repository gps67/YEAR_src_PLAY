#ifndef dset_t_H
#define dset_t_H

#include "obj_hold.h"
#include "date_glib.h"


#include "table.h"
#include "table_list.h"
//#include "obj_ref_js.h"

namespace ACCTS { class col_spec; };
namespace ACCTS {

//! each file is a collection of tables
/*!
	a data_set is a list of tables (each with a name)
*/
class data_set : public obj_ref_js
{
public:
	// The set of loaded tables
	obj_hold<table_list>	tbl_list;

	data_set();
	void reset();
	bool load_file( str0 filename )
	{
		return file_load( filename );
	}
	bool file_load( str0 filename );
	bool file_save( str0 filename, bool compressed=false );

	col_spec * guess_col_spec( table * tbl, str2 * colname );

	table * get_table( str2 * name );
	table * get_table(  str0 name );
	table * get_table_or_FAIL(  str0 name );
	table * find_add_table( str2 * name ) ;
	table * find_add( str2 * name );
	table * find_add(  str0 name );
	table * find_add_fake_table( str2 * name );
	table * find_add_fake_table(  str0 name );
	void hide_table( str2 * name );
	void hide_table( str2 * name, bool hide );

	str2 * get_table_row_col(
		str0 name_table,
		str0 name_row,
		str0 name_col
	);
};
}; // NAMESPACE
#endif
