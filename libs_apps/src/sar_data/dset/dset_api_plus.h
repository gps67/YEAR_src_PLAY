#ifndef dset_api_plus_H
#define dset_api_plus_H

#ifndef dset_api_H
// #include "misc.h"
// #include "mem_line_spec.h"
// #include "mem_line_tree.h"
// #include <stdio.h>
// #include "obj_ref.h"
#include "dset_api.h"
#endif

/*!
	a debugging class, and possible data dump (restorable?) (tdb)
*/
class dset_api_print : public dset_api
{
 public:
	FILE * f;
	int ncols;
	//! constructor - setup to write to FILE with _ncols width
	dset_api_print(
		FILE * _file,
		int _ncols
	);

  virtual
	bool merge_floats(
		mem_line_spec * spec,
		time_t t0,
		int n,
		float * flts
	);
	~dset_api_print() {}
};

/*!
	used by sar_top_layout to build a mem_line_tree (line specs not data)

	each spec given to merge_spec is added to the tree.
*/
class dset_api_line_tree : public dset_api
{
 public:
 	mem_line_tree * root;

	dset_api_line_tree( mem_line_tree * _root )
	{
		root = _root;
	}
	bool merge_spec( mem_line_spec * spec )
	{
		if(!spec) return FALSE;
		root->find_add( *spec );
		return TRUE;
	}
	~dset_api_line_tree()
	{
	};

};

#endif
