
#include "pdb_line_and_segs.h"
REGISTER( pdb_line_and_segs );



#include "mem_line_spec.h"

#define least(A,B) ((A)<(B) ? (A) : (B) )
#define most(A,B) ((A)>(B) ? (A) : (B) )

#include "time_calc_zone_week.h"

/*!
	Create a new line (and data seg storage) from line_spec;

	Always call mk_XXX as it does the proper NS_POST new invocation.
*/
// static, calls constructor via NS_POST
pdb_line_and_segs * pdb_line_and_segs::mk_line(
	NS_POST::storage * sto,
	pdb_string_pool * _string_pool ,
	int id,
	mem_line_spec * search_spec
) {
	return new (self_class, *sto) pdb_line_and_segs(
		_string_pool,
		id,
		search_spec
	);
}
	
/*!
	Constructor - creates object in the file, see mk_XXX
*/
pdb_line_and_segs::pdb_line_and_segs(
	pdb_string_pool * _string_pool ,
	int id,
	mem_line_spec * _spec
)
: spec(-1)
, ary_t0(2)
, ary_t2(2)
, ary_blob(2)
{
	/*
		The above constructore are mandatory!
		The default constructor is used by NS_POST (and exists)
		The non-default does 'the-right-thing',
		particularly needed for fields of larger objects,
		where the larger object DOES have a mk_new( sto ) func,
		but the field doesnt ...
	*/
	DB_ID = id;
	spec.set_from( _string_pool, _spec );
//	spec.print1(stderr);
}

/*!
	extract the mem_line_spec from this.
*/
bool pdb_line_and_segs::get_line_spec(
	pdb_string_pool * string_pool ,
	mem_line_spec * _spec
)
{
	// if(!spec) return FALSE;
	spec.get_into( string_pool, _spec );
	return TRUE;
}

/*!
	insert this_line[idx] = (t0,t2,blob)
*/
bool pdb_line_and_segs::ary_ins(
	int idx,
	time_t t0,
	time_t t2,
	pdb_blob_fltz * blob
){
	ary_t0.ins( idx, 1, t0 );
	ary_t2.ins( idx, 1, t2 );
	ary_blob.ins( idx, 1, blob );
	return TRUE; // once started
}

/*!
	replace this_line[idx] = (t0,t2,blob)
*/
bool pdb_line_and_segs::ary_rpl(
	int idx,
	time_t t0,
	time_t t2,
	pdb_blob_fltz * blob
){
	ary_t0.put_at( idx, t0 );
	ary_t2.put_at( idx, t2 );
	if( ary_blob[ idx ] ) delete ary_blob[ idx ];
	ary_blob.put_at( idx, blob );
	return TRUE; // once started
}

/*!
	delete this_line[idx] (and delete its) (t0,t2,blob)
*/
bool pdb_line_and_segs::ary_del( int idx )
{
	ary_t0.del( idx, 1 );
	ary_t2.del( idx, 1 );
	if( ary_blob[ idx ] ) delete ary_blob[ idx ];
	ary_blob.del( idx, 1 );
	/*
		to help with debugging, set the old values to 0
		KNOWING that one free slot is available
	*/
	int xs = ary_t0.get_size();
	ary_t0.body()[xs] = 0;
	ary_t2.body()[xs] = 0;
	ary_blob.body()[xs] = NULL;
	return TRUE; // once started
}

/*!
	This is not dset_api, but is similar, it properly merges
	overlapping segments (by overwriting old data with new),
	and stores the data in the file.
*/
bool pdb_line_and_segs::merge_floats(
	NS_POST::storage *sto,	// for new objects (could get from this)
	time_t T0,
	int n,
	float * flts
) {
	bool ok = TRUE;
	time_t T2 = T0 + (spec.dt * n);
	int LURK_fixed_era;
	time_calc_zone_week4 era1 ( T0 );
	if( era1.T2 < T2 ) { // data crosses era edge
		int T1 = era1.T2; // the splitting point
		int n_right = (T2 - T1) / spec.dt;
		int n_left = n - n_right;
		float * flts_right = &flts[n_left];
		if(! merge_floats( sto, T0, n_left,  flts ) ) ok=FALSE;
		if(! merge_floats( sto, T1, n_right, flts_right ) ) ok=FALSE;
		return ok;
	}
	int N1 = N_seg();
	int i=0;
	for( i=0; i<N1; i++ ) {
		// the first line catches most
		if( ary_t2[i] < T0 ) {
			// seg ends before new data begins
			continue; // next segment or append to list
		}
		if( ary_t2[i] == era1.T0 ) {
			// seg ends at era start
			continue; // no merge over era edge
		}
		// NOW segment ends after/at new data begins

		if( T2 < ary_t0[i] ) {
			// new data ends before seg starts
			break; // ready for insert before i
		}
		if( era1.T2 == ary_t0[i] ) {
			/*
				new data would merge exactly with seg start
				but we want to stop merges over era1
			*/
			// seg starts at era1 edge
			break; // for insert before i
		}
		/*
		NOW segment and new data join/overlap (possibly merge next!)
		So lift the old data into temp vars, delete, re-add.
		Do not break loop  - return
		*/
		time_t new_t0 = least( T0, ary_t0[i] );
		time_t new_t2 = most( T2, ary_t2[i] );
		int new_n = (new_t2 - new_t0) / spec.dt;
		float new_flts[ new_n ];
		int old_pos = (ary_t0[i] - new_t0) / spec.dt;
		int old_len = (ary_t2[i] - ary_t0[i]) / spec.dt;
		pdb_blob_fltz * old_blob = ary_blob[i];
		ok = old_blob->un_compress( old_len, &new_flts[old_pos] );
		if(!ok) return FALSE;
		int new_pos = (T0 - new_t0) / spec.dt;
		// overwrite data 
		for( int p = 0; p<n; p++ )
		{
			new_flts[new_pos+p] = flts[p];
		}
		// CTRL-C here loses data! - TRANSACTION issue
		ary_del( i );
		return merge_floats( sto, new_t0, new_n, &new_flts[0] );
	}
	/*
		the for loop reached the end (i)
		or found the segment (i) AFTER where the new segment goes
		No possible merge (eg era edge)
	*/
	pdb_blob_fltz * new_blob =
		pdb_blob_fltz::mk_blob_fltz ( sto, n, flts );
	return ary_ins( i, T0, T2, new_blob );
}
