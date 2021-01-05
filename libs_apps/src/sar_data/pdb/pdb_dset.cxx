

#include "pdb_dset.h"
REGISTER( pdb_dset );

// #include "line_spec.h"

/*!
	constructor - but always use the static function to create
	as it uses the correct NS_POST new invocation.

	Creates a new string_pool in the file. You ony call this
	when a new file is created, as it is persistant data
*/
pdb_dset::pdb_dset( NS_POST::storage * sto, int n )
{
	line_list = pdb_line_list::mk_new( sto, n );
	string_pool = pdb_string_pool::create( sto, 91 );
}

/*!
	Create a new pdb_dset in the file
*/
pdb_dset * pdb_dset::mk_new( NS_POST::storage * sto )
{
	return new (self_class, *sto) pdb_dset( sto, 89 );
}

/*!
	add some data to the file. This is NOT a dset_api call,
	but is similar
*/
bool pdb_dset::merge_floats(
	mem_line_spec * mem_spec,
	time_t t0,
	int n,
	float * flts
) {
	int id = line_list->find_add_line( string_pool, mem_spec );
	// check id >=0
	return line_list->get_at(id)->merge_floats( t0, n, flts );
}

