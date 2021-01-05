#ifndef dset_api_postgres_H
#define dset_api_postgres_H

#include "dset_api.h"
// DONE //#include "misc.h"
// DONE //#include "mem_line_spec.h"
// DONE //#include "mem_line_tree.h"
// DONE //#include <stdio.h>
// DONE //#include "obj_ref.h"

#include "buffer2.h"
#include "mem_line_hash.h"

class SQL_link_Postgres;

/*!
	dset_api_postgres is a dset_api consumer and producer.

	TODO: merge_floats doesnt delete the later half
*/
class dset_api_postgres : public dset_api
{
	SQL_link_Postgres * pgsql;
	buffer2 enq;
	mem_line_sql_id table;
 public:
	~dset_api_postgres();
	dset_api_postgres(
	//	const char * filename
	);

  virtual
	bool merge_floats(
		mem_line_spec * spec,
		time_t t0,
		int n,
		float * flts
	);

  virtual
	bool select_segs_to_dset(
		dset_api * dset,
		mem_line_spec * spec,
		time_t t0,
		time_t t2
	);

  virtual
	bool select_ALL_specs_to_dset(
		dset_api * dset
	);

  virtual
	bool list_segs_to_dset(
		dset_api * dset,
		mem_line_spec * spec,
		time_t t0,
		time_t t2
	);
};

#endif
