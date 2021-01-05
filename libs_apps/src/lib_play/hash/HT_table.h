#ifndef HT_table_h
#define HT_table_h

/*
	HT_table provides a typed wrap around HT_table_base
*/

#include "obj_ref.h"
#include "HT_table_base.h"
class buffer1;

#define OBJ(sub_type) ((obj_ref *)sub_type) // though it could be (VOID *)

/*!
	Hash Table (over HT_table_base)
*/
template < class KEY, class VAL >
class HT_table : public HT_table_base {
 public:

	HT_table(
		HT_Type key_type = HT_Type_str2,
		HT_Type val_type = HT_Type_obj,
		int sz = 1
	)
	: HT_table_base( key_type, val_type, sz )
	{
	};
	~HT_table() {};

	VAL * add( KEY * key, VAL * val )
	{
		return (VAL *) HT_table_base::add( OBJ(key), OBJ(val ) );
	}
	VAL * lookup( KEY * key )
	{
		return (VAL *) HT_table_base::lookup( OBJ(key) );
	}
	VAL * lookup( const char * key )	// special case
	{
		return (VAL *) HT_table_base::lookup( key );
	}
	VAL * lookup( buffer1 & key )	// another special case
	{
		return (VAL *) HT_table_base::lookup( key );
	}
	bool rename( KEY * key1, KEY * key2 )
	{
		return (VAL *) HT_table_base::rename( OBJ(key1), OBJ(key2) );
	}
	void remove( KEY * key )
	{
		HT_table_base::remove( OBJ(key) );
	}

	// OK // void remove_all();
};

class str2;
namespace ACCTS {
class row_inst;
class col_spec;
class table;
};
using namespace ACCTS;

// WAS: key must be str2 - or sub_type
// NOW: key may be an obj_ref hashed by address
// NOW: key may be a VOID pointer - but caller must do own ref_count issues

typedef HT_table<str2,ACCTS::row_inst>	HT_row;
typedef HT_table<str2,ACCTS::col_spec>	HT_specs;
typedef HT_table<str2,ACCTS::table>		HT_tables;

class HT_table_str_obj : public  HT_table<str2,obj_ref>
{
 public:
	HT_table_str_obj()
	: HT_table<str2,obj_ref>( HT_Type_str2, HT_Type_obj, 1 )
	{
	}
};

// -X-

typedef HT_table<void,row_inst>		HT_V_row;

#define NEW_HT_v_row() new HT_V_row( HT_type_void, HT_type_optr )

#undef OBJ

#endif
