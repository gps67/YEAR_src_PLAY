#include "dset_api_MySQL.h"
#include "sql_link_MySQL.h"

// EXCLUDE THESE RSN
#include "pdb_root_type.h"
#include "PDB_app.h"
#include "dgb_fail.h"

/*!
	constructor - connect to MySQL (could be a parameter).
*/
dset_api_MySQL::dset_api_MySQL(
	// const char * filename
)
: enq( 1000 )
, table()
{
	mysql = new sql_link_MySQL();
/*
	if(! mysql -> connect (
		"localhost",
		0, // PORT
		"obj_stat_gather",
		"gather99",
		"obj_stat_1"  
	))
		throw dgb_fail("SQL connect failed");
*/
}

/*!
	destructor - disconnect
*/
dset_api_MySQL::~dset_api_MySQL()
{
	delete mysql;
}

#include "m_merge_floats.fn"
#include "m_select_ALL_specs_to_dset.fn"
#include "m_select_segs_to_dset.fn"
#include "m_list_segs_to_dset.fn"

