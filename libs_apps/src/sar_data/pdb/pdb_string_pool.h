#ifndef pdb_string_pool_H
#define pdb_string_pool_H
/*
 * 	string pool converts strings to ints
*/

#include "post_storage.h"
// #include "post_object.h"
#include "post_array.h"

// use const char * or str0
#include "str0.h"

/*!
	The string pool converts a SPELLING to and from an int.

	This reduces page hits when looking at pdb_line_spec,

	Although the storage * sto is known at create time,
	it is self-finding for creating new strings and array reallocs
*/
class pdb_string_pool : NS_POST::object
{
 public:
	NS_POST::DynArrayOfChar	text;
	NS_POST::DynArrayOfInt	offs;

	CLASSINFO( pdb_string_pool, NO_REFS );
	// REF( text ) REF( offs ) );

	int N() { return offs.get_size(); };
	pdb_string_pool ( int n1 );

 static
	pdb_string_pool * create( NS_POST::storage * sto, int n1 = 90 );

	str0 get_str( int idx );
	int find( str0 str );
};

#endif
