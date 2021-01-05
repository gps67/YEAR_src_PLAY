#ifndef pdb_line_list_H
#define pdb_line_list_H

#include "pdb_line_and_segs.h"
#include "pdb_string_pool.h"

/*!
*/
class pdb_line_list : public NS_POST::dyn_array_of_ptr<pdb_line_and_segs>
{
 public:
	CLASSINFO( pdb_line_list, NO_REFS );
	pdb_line_list( size_t n_free );
	int N()
	{
		return get_size();
	}
	int find_add_line(
		pdb_string_pool * string_pool,
		mem_line_spec * spec
	);
	bool add_line( 
		pdb_string_pool * string_pool,
		mem_line_spec * spec
	);
 static
	pdb_line_list * mk_new(
		NS_POST::storage * sto,
		int n_free
	);
};

#endif
