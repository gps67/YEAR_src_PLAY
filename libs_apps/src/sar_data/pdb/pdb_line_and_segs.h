#ifndef pdb_line_and_segs_H
#define pdb_line_and_segs_H


class mem_line_spec;
class pdb_string_pool;

// #include "pdb_t0_t2_nblk.h"
// #include "pdb_ary_t0_t2_nblk.h"
// #include "post_object.h"
#include "pdb_string_pool.h"
#include "pdb_line_spec.h"
#include "pdb_blob.h"
#include "ints.h"

typedef NS_POST::DynArrayOfInt DynArrayOfInt;

typedef NS_POST::dyn_array_of_ptr<pdb_blob_fltz> DynArrayOfBlob_fltz;


/*!
	pdb_line_and_segs is a Time_Series type line the NS_POST file.

	To reduce page-hits, it uses three parrallel arrays and a top
	object, and provides functions that hide such detail from you.

	Always use the mk_XXX function, never the constructors.
*/
class pdb_line_and_segs : public NS_POST::object
{
 public:
	int		DB_ID;
	pdb_line_spec	spec;

	DynArrayOfInt	ary_t0;
	DynArrayOfInt	ary_t2;
	DynArrayOfBlob_fltz	ary_blob;
 
	CLASSINFO( pdb_line_and_segs, NO_REFS );

	int N_seg() {
		return ary_t0.get_size();
	}

  static
	pdb_line_and_segs * mk_line(
		NS_POST::storage * sto,
		pdb_string_pool * _string_pool ,
		int id,
		mem_line_spec * search_spec
	);
	
	pdb_line_and_segs(
		pdb_string_pool * _string_pool ,
		int id,
		mem_line_spec * spec
	);

	bool get_line_spec(
		pdb_string_pool * string_pool ,
		mem_line_spec * spec
	);

	bool ary_ins(
		int idx,
		time_t t0,
		time_t t2,
		pdb_blob_fltz * blob // then blob belongs to this
	);

	bool ary_rpl(
		int idx,
		time_t t0,
		time_t t2,
		pdb_blob_fltz * blob
	);

	bool ary_del( int idx );

        bool merge_floats(
		NS_POST::storage * sto,
		time_t T0,
		int n,
		float * flts
	);

	bool merge_floats(
		time_t T0,
		int n,
		float * flts
	) {
		return merge_floats( get_storage(), T0, n, flts );
	}

};

#endif
