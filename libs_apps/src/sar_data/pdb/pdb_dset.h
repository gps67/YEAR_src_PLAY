#ifndef pdb_dset_H
#define pdb_dset_H

#include "pdb_line_list.h"
#include "pdb_string_pool.h"

/*!
	pdb_dset is the DATASET in the NS_POST file,
	it holds a set of lines, which hold their own data.

	String pool is used to hold some spellings of strings,
	(host names, attr names, etc). Its a good experiment
	but do be careful as there is no GC of unused names.
	Its benefit is that strings are localised onto 
	shared page(s), and dont have lots of object headers.
	That stops stray page-hits.
*/
class pdb_dset : public NS_POST::object
{
 public:
	pdb_string_pool * string_pool;
	pdb_line_list * line_list;

	bool merge_floats(
		mem_line_spec * mem_spec,
		time_t t0,
		int n,
		float * flts
	);

	CLASSINFO( pdb_dset,
		REF( line_list )
		REF( string_pool )
	);
  static
	pdb_dset * mk_new( NS_POST::storage * sto );
	pdb_dset( NS_POST::storage * sto, int n );

	pdb_line_and_segs * find_line( mem_line_spec * spec )
	{
		int l =  line_list->find_add_line( string_pool, spec );
		return (*line_list)[l];
	}

};

#endif
