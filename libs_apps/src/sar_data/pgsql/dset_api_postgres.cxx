#include "dset_api_postgres.h"
#include "sql_link_postgres.h"




// EXCLUDE THESE RSN
#include "pdb_root_type.h"
#include "PDB_app.h"
#include "dgb_fail.h"

/*!
	constructor - connects to specific DB
*/
dset_api_postgres::dset_api_postgres(
	// const char * filename
)
: enq( 1000 )
, table()
{
	pgsql = new SQL_link_Postgres();
	if(! pgsql -> connect (
		"localhost",
		0, // PORT
		"obj_stat_gather",
		"gather99",
		"obj_stat_1"  
	))
		throw dgb_fail("SQL connect failed");
}

/*!
	destructor - disconnect
*/
dset_api_postgres::~dset_api_postgres()
{
	pgsql->disconnect();
	delete pgsql;
}

#include "time_calc_zone_week.h"

#include "p_list_segs_to_dset.fn"
#include "p_merge_floats.fn"
#include "p_select_ALL_specs_to_dset.fn"
#include "p_select_segs_to_dset.fn"

