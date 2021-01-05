#ifndef dset_api_MySQ_H
#define dset_api_MySQLH

#include "dset_api.h"
// DONE //#include "misc.h"
// DONE //#include "mem_line_spec.h"
// DONE //#include "mem_line_tree.h"
// DONE //#include <stdio.h>
// DONE //#include "obj_ref.h"

#include "buffer2.h"
#include "mem_line_hash.h"

class sql_link_MySQL;

/*!
	dset_api_MySQL - is a MySQL oriented dset_api.
	It can be a consumer or a producer.

	TODO: At present the 'merge' part is incomplete, as it doesnt
	properly delete overlapping segments. This can be resolved
	using an additional cleanup script (believe the earliest T0).
*/
class dset_api_MySQL : public dset_api
{
	buffer2 enq;
	mem_line_sql_id table;
 public:
	sql_link_MySQL * mysql; // public to open it
	~dset_api_MySQL();
	dset_api_MySQL(
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
