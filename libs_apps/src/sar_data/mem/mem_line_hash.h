#ifndef mem_line_hash_H
#define mem_line_hash_H

#include "mem_line_spec.h"	// not an obj_ref
#include "ht2.h"

/*!
	hash_key_val means both key and val in a hash table
	Adds sql_id, which SQL depends on the table

	mem_line_spec is not an obj_ref (why?)

	a mem_line_spec and its sql_id

	The actual hash key IS held by the mem_line_spec
*/
class mem_line_hash_key_val : public obj_ref
{
 public:
	mem_line_spec spec; // not an obj_ref
	long sql_id;

	uint ht_get_hash()
	{
		return spec.get_hash_key();
	}
	bool ht_equals( obj_ref * key2_ )
	{
		mem_line_hash_key_val * key2 = (mem_line_hash_key_val *) key2_;
		return spec.is_same_as( & key2->spec );
	}

	mem_line_hash_key_val()
	: spec()
	, sql_id( -1 )
	{
	}

	mem_line_hash_key_val( const mem_line_spec & _spec, long id )
	: spec( _spec )
	, sql_id( id )
	{
	}

 virtual
	~mem_line_hash_key_val()
	{
	}
};

typedef ht2<mem_line_hash_key_val,mem_line_hash_key_val> ht_spec1;
class dset_api_MySQL;

/*!
	a hash table of line_spec
*/
class mem_line_sql_id : public GRP_sar_data
{
	ht_spec1 hash;
	mem_line_hash_key_val temp_key;
 public:

	mem_line_spec * get_temp_spec()
	{
		return & temp_key.spec;
	}
	void find_set( long id )
	{
		temp_key.spec.recalc_hash_key();
		mem_line_hash_key_val * val1;
		val1 = hash.lookup( &temp_key );
		if( val1 )
		{
			if( val1->sql_id != id )
			{
				if( val1->sql_id != -1 )
					e_print("HASH sql_id %ld != id %ld\n",
						val1->sql_id, id );
				val1->sql_id = id;
			}
		} else {
			val1 = new mem_line_hash_key_val( temp_key.spec, id );
			hash.add( val1, val1 );
		}
	}

	bool find_line( mem_line_spec * spec, long & id )
	{
		temp_key.spec.copy_from( spec );
		temp_key.spec.recalc_hash_key();
		mem_line_hash_key_val * val1;
		val1 = hash.lookup( &temp_key );
		if( !val1 )
			return FALSE;
		id = val1 -> sql_id;
		return TRUE;
	}

	mem_line_sql_id()
	: hash( ht_type_obj, ht_type_optr )
	, temp_key()
	{
		temp_key.ref_fixed();
	}

	~mem_line_sql_id()
	{
	}

};

#endif
