#ifndef dset_api_H
#define dset_api_H

#include "misc.h"
#include "mem_line_spec.h"
#include "mem_line_tree.h"
#include <stdio.h>
#include "obj_ref.h"


/*!
	The api used to read/write parts of a dset

	dset is a dataset DESTINATION or SOURCE (possibly BOTH).
	It can also be a filter, cascading to another dset.

		C = CLIENT  = CONSUMER
		S = SERVER  = SUPPLIER

	A DB (such as mysql, or pgsql) might be used for both read and write.
	Commonly one direction is used at a time. A query causes callbacks.

	Virtual functions are used to request queries, and to receive
	responses.

	Queries can obtain:

		possible line specs
		segments available (without data)
		segments available (with data)

	The key difference between different classes (apart from the
	implementation code) will be the initial constructor, which
	will need database passwords, addresses, names etc.

	HISTORY: this started as time-series-segments only. 
	time-value-pairs was added later
*/
class dset_api : public obj_ref
{
 public:

  //! destructor - plain
  virtual ~dset_api() {}

  //! flush all data (already done) - compat with other API's
  virtual
	bool flush() { return TRUE; }

  /*
	To get a list of lines:

	CLIENT  --> SERVER REQUEST  S->select_ALL_specs_to_dset( dset )
	CLIENT <--  SERVER RESPONSE C->merge_spec( spec )

	TODO:
		select_specs_to_dset_like( dset, wildcardspec )
		select_specs_in_subtree( dset, treepoint )
  */

  //! list all line specs (not data) in this to target dset
  virtual
	bool select_ALL_specs_to_dset(
		dset_api * dset
	)
	{
		return TRUE;
	}

  //! receive a line spec
  virtual
	bool merge_spec( mem_line_spec * spec )
	{
		return TRUE;
	}

  /*

	To build a map of available segments, list segs without data

	CLIENT  --> SERVER REQUEST  S->list_segs_to_dset( dset, spec, t0, t2 )
	CLIENT <--  SERVER RESPONSE C->listed_seg( spec, t0, t2 )

  */

  //! to dset send LIST-NOT-DATA of all segs for SPEC line that touch [t0[t2 (can overshoot)
  virtual
	bool list_segs_to_dset(
		dset_api * dset,
		mem_line_spec * spec,
		time_t t0,
		time_t t2
	)
	{
		return TRUE;
	}

  //! receive info that seg exists for spec-line [t0[t2  (but not data)
  virtual
	bool listed_seg(
		mem_line_spec * spec,
		time_t t0,
		time_t t2
	)
	{
		return TRUE;
	}

  /*

	CLIENT  --> SERVER REQUEST  S->select_segs_to_dset( dset, spec, t0, t2 )
	CLIENT <--  SERVER RESPONSE C->merge_floats( spec, t0, n, flts[] )
	CLIENT <--  SERVER RESPONSE C->merge_point( spec, t0, flts )
  */

  //! to dset send DATA of all segs for SPEC line that touch [t0[t2 (can overshoot)
  virtual
	bool select_segs_to_dset(
		dset_api * dset,
		mem_line_spec * spec,
		time_t t0,
		time_t t2
	)
	{
		return TRUE;
	}

		
  //! receive DATA for spec-line (time series)
  virtual
	bool merge_floats(
		mem_line_spec * spec,
		time_t t0,
		int n,
		float * flts
	)
	{
		return TRUE;
	}

  //! receive DATA POINT for spec-line (time series or time-val pairs)
  virtual bool merge_point(
		mem_line_spec * spec,
		time_t t0,
		// int n, // == 1
		float flt
	)
	{
		return TRUE;
	}

  //! flush database and disconnect
  virtual bool disconnect() { return true; }

};

#include "dset_api_plus.h"

#endif
