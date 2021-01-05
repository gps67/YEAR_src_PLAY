
#include "pdb_line_list.h"
#include "mem_line_spec.h"

namespace NS_POST {
	// gcc-4.1 demans that this is in the POST namespace
	REGISTER_TEMPLATE( array_of_ptr<pdb_line_and_segs> );
};

REGISTER(pdb_line_list);

/*!
	Create a new list of lines
*/
//static
pdb_line_list * pdb_line_list::mk_new( NS_POST::storage * sto, int n_free )
{
	return new (self_class, *sto) pdb_line_list( n_free);
}

/*!
	constructor - use the mk_XXX instead

	ASIDE: When this class want registered with NS_POST, I got
	a linkage error
*/
pdb_line_list::pdb_line_list( size_t n_free )
: NS_POST::dyn_array_of_ptr<pdb_line_and_segs>( n_free, 0 )
{
}

/*!
	Add a NEW line (mustn't already exist), see find_add_line

	NOT SURE WHERE THIS IS CALLED - see find_add_line
*/
bool pdb_line_list::add_line( 
	pdb_string_pool * string_pool,
	mem_line_spec * spec
) {
	int id = N();
	// PRESUME line_id doesnt already exist
	pdb_line_and_segs * line = pdb_line_and_segs::mk_line(
		get_storage(),
		string_pool,
		id,
		spec
	);
	/*
		push is an STL/NS_POST that adds line to the dyn_array_of_ptr<T>
	*/
	push( line );
	if( line->DB_ID != id )
	{
		e_print("line->DB_ID(%d) != id\(%d)n", line->DB_ID, id );
	}
	return TRUE;
}

/*!
	returns the line-id if the new/existing line (always succeeds)
*/
int pdb_line_list::find_add_line( 
	pdb_string_pool * string_pool,
	mem_line_spec * mem_spec
) {
	pdb_line_spec pdb_spec;
	pdb_spec.set_from( string_pool, mem_spec );
	int n = N();
	int id;
	for( id=0; id<n; id++ )
	{
		if( pdb_spec.equals( & this->get_at( id )->spec ) )
			return id;
	}
	id = n;
	pdb_line_and_segs * line = pdb_line_and_segs::mk_line(
		get_storage(),
		string_pool,
		id,
		mem_spec
	);
	push( line );
	return id;
}
