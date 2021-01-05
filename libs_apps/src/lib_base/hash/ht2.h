#ifndef ht2_h
#define ht2_h

/*
	ht2 provides a typed wrap around ht1
*/

#include "obj_ref.h"
#include "ht1.h"

#define OBJ(sub_type) ((obj_ref *)sub_type) // though it could be (VOID *)

/*!
	Hash Table (over ht1)
*/
template < class KEY, class VAL >
class ht2 : public ht1 {
 public:

	 ht2(
		ht_type key_type = ht_type_str2,
		ht_type val_type = ht_type_optr
	) : ht1( key_type, val_type ) { };
	~ht2() {};

	VAL * add( KEY * key, VAL * val )
	{
		return (VAL *) ht1::add( OBJ(key), OBJ(val ) );
	}
	VAL * lookup( KEY * key )
	{
		return (VAL *) ht1::lookup( OBJ(key) );
	}
	VAL * lookup( const char * key )	// special case
	{
		return (VAL *) ht1::lookup( key );
	}
	bool remove( const char * key )	// special case
	{
		return ht1::remove( key );
	}
	bool rename( KEY * key1, KEY * key2 )
	{
		return (VAL *) ht1::rename( OBJ(key1), OBJ(key2) );
	}
	void remove( KEY * key )
	{
		ht1::remove( OBJ(key) );
	}

	// OK // void remove_all();
};

class str2;

namespace ACCTS {
	class row_inst;
	class col_spec;
	class table;
};
// using namespace ACCTS;

// WAS: key must be str2 - or sub_type
// NOW: key may be an obj_ref hashed by address
// NOW: key may be a VOID pointer - but caller must do own ref_count issues

typedef ht2<str2,::ACCTS::row_inst>	ht_row;
typedef ht2<str2,::ACCTS::col_spec>	ht_specs;
typedef ht2<str2,::ACCTS::table>		ht_tables;

// -X-

typedef ht2<void,::ACCTS::row_inst>		ht_V_row;

#define NEW_ht_v_row() new ht_V_row( ht_type_void, ht_type_optr )

#undef OBJ

#endif
