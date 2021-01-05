#ifndef pdb_root_type_H
#define pdb_root_type_H

#include "pdb_dset.h"

/*!
	pdb_root_base is the BASE type of the root node in the NS_POST file

	Its tricky to change the type of any existing NS_POST object,
	but you could always set a new root of a different type.
*/
class pdb_root_base : public NS_POST::object
{
 public:
	//! NS_POST template: register the class and (some of) its fields
	CLASSINFO( pdb_root_base, NO_REFS );

	//! Completely pointless action by the baseclass, track mount_count
	int mount_count;

	//! NON-DEFAULT constructor-with-parameter (any parameter)
	pdb_root_base( NS_POST::storage * sto );

	//! Call this unless you want a sub-class of this obj
  static
	pdb_root_base * mk_new( NS_POST::storage * sto );
};


/*!
	pdb_root_type is the type of the root node in the NS_POST file

	It adds app_specific things such as '.dset'

	Temp vars such as '.all_lines_loaded' is to be moved to MEM
*/
class pdb_root_type : public pdb_root_base 
{
 public:
	//! NS_POST template: register the class and (some of) its fields
	CLASSINFO( pdb_root_type, REF( dset ) );

	//! TODO: move this to temp MEM
	bool all_lines_loaded;	// STILL USED ???
	//! The data held by this (part of) pdb file
	pdb_dset * dset;

	//! NON-DEFAULT constructor-with-parameter( any will do)
	pdb_root_type( NS_POST::storage * sto );

	//! Call this to create new root object(s)
  static
	pdb_root_type * mk_new( NS_POST::storage * sto );
};

#endif
