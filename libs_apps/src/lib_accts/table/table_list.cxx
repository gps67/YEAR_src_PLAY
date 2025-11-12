#include "table_list.h"
#include "table.h"


namespace ACCTS {

table * table_list::find_add( str0 name )
{
	table * tbl = tbl_map.lookup( name );
	if( tbl ) return tbl;
	obj_hold<str2> name2; name2 = new str2( name );
	return find_add( name2.get_ptr() );
}

table * table_list::find_add_hidden( str2 * name )
{
	table * t = find_add( name );
	t->is_hidden = true;
	return t;
/*
	table * tbl = tbl_map.lookup( name );
	if( tbl) return tbl;
	tbl = new table( name, dset );
	tbl_map.add( tbl->name.get_ptr(), tbl );
	// HIDDEN // tbl_list.append( tbl );
	return tbl;
*/
}

table * table_list::find_add( str2 * name )
{
	if(!name) WARN("NULL tbl_name name"); 
	INFO("tbl_name '%s'", (STR0) *name);
//	INFO("tbl_name '%s' from*name", (STR0) *name);
//	INFO("tbl_name '%s' from name->str", (STR0) name->str);
	table * tbl = tbl_map.lookup( name );
	if( tbl) return tbl;
	tbl = new table( name, dset );
	tbl_map.add( tbl->name.get_ptr(), tbl );
	tbl_list.append( tbl );
	return tbl;
}

table * table_list::get( str2 * name )
{
	return tbl_map.lookup( name );
}

table * table_list::get( str0 name )
{
	return tbl_map.lookup( name );
}
}; // NAMESPACE
