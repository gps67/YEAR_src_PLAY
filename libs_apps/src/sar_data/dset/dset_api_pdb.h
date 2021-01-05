#ifndef dset_api_pdb_H
#define dset_api_pdb_H

#ifndef dset_api_H
// #include "misc.h"
// #include "mem_line_spec.h"
// #include "mem_line_tree.h"
// #include <stdio.h>
// #include "obj_ref.h"
#include "dset_api.h"
#endif

// move this to STUBS
class PDB_app;

/*!
	a 2-way POST DB MMAP file
*/
class dset_api_pdb : public dset_api
{
	PDB_app * pdb;
 public:
	~dset_api_pdb();
	//! open or create a POST file (no search path?)
	dset_api_pdb(
		const char * filename
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
  //! flush the MMAP file through its stilegate (hence why dset_api has flush)
  virtual
	bool flush();

  virtual
	bool disconnect();
};
#endif

