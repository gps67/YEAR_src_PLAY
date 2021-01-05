
#include "pdb_string_pool.h"
/*
 * 	string pool converts strings to ints
*/

/*!
	NS_POST requires all types to be registered, naff name though.
*/

REGISTER( pdb_string_pool );

/*!
	construct a new string pool, with initial space for n1 strings

	String data is held in one large text area, offsets are held
	in an array. Strings are referenced by idx, are shared and are
	never deleted.  This is to reduce page-hits.

	The idea is that from time-to-time you delete and start again,
	possibly exporting all data to SQL. That way unwanted strings
	dont get re-created, plus data-fragmentation is reworked.
*/
pdb_string_pool::pdb_string_pool( int n1 )
:	text( n1 * 10 )
,	offs( n1 )
{
	// NOTE: Mandatory NON-DEFAULT constructors of fields
}

/*!
	This is the static mk_XXX function, dont use the constructor directly
*/
pdb_string_pool * pdb_string_pool::create( NS_POST::storage * sto, int n1 )
{
	return new (self_class, *sto ) pdb_string_pool( n1 );
}

/*!
	get a direct pointer to nul terminated string_pool[idx]
*/
str0 pdb_string_pool::get_str( int idx )
{
	if( idx < 0 ) return "PDB-ERROR(<)";
	if( idx > N() ) return "ERROR(>)";
	const char * ptr = text.body() + offs[idx];
	return (str0) ptr;
}

/*!
	find/add str to the string pool, return its idx.

	NB: strings are shared, and never deleted
*/
int pdb_string_pool::find( str0 str )
{
	int n = N();
	for( int i = 0; i < n; i++ )
	{
		str0 s = get_str( i );
		if( s == str )
			return i;
	}
	// int len1 = strlen( str );
	int len1 = str.str_len();
	int len2 = text.get_size();
	int len3 = len2 + len1 + 1;
	text.set_size( len3 );
	strcpy( text.body()+len2, str.sget() );
	offs.push( len2 );
	return n;
}

