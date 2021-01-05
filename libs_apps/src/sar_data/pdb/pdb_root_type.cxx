
#include "e_print.h"

#include "pdb_root_type.h"
REGISTER( pdb_root_base );
REGISTER( pdb_root_type );

/*!
	constructor - creating a NEW file, not mounting
	an existing one.

	Create a new pdb_dset.
*/
pdb_root_base::pdb_root_base( NS_POST::storage * sto )
{
	e_print( "NEW ROOT database created\n INIT HERE\n");
	
	mount_count = 0; // testing a mount-count
}

/*!
	always call mk_XXX
*/
pdb_root_base * pdb_root_base::mk_new( NS_POST::storage * sto )
{
        return new (self_class, *sto) pdb_root_base( sto );
}

// REGISTER( pdb_root_type );


/*!
	constructor - creating a NEW file, not mounting
	an existing one.

	Create a new pdb_dset.
*/
pdb_root_type::pdb_root_type( NS_POST::storage * sto )
: pdb_root_base( sto )
{
	dset = pdb_dset::mk_new( sto );
}

/*!
	always call mk_XXX
*/
pdb_root_type * pdb_root_type::mk_new( NS_POST::storage * sto )
{
        return new (self_class, *sto) pdb_root_type( sto );
}

