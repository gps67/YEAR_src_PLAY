#ifndef jsc_prop_list_H
#define jsc_prop_list_H
/*
	This class is needed for when the C++ side changes the
	list of properties available, eg add table to dset, or
	worse delete. (Before tbl_list[js_prop_id] but that cant be)

	This is only relevent for a few scripts that manually change
	table schema, (but all scripts use it).

	For generality, a name can be re-used, returning the old js_id
	(which is what JS thinks, as it was never told of the prop going)

	props are never deleted individually, as JS thinks the name still
	exists.
*/

#include "dyn_array.h"
#include "obj_hold.h"
// #include "obj_ref_js.h"
#include "str2.h"

namespace JSSM {


struct jsc_prop_list_item : public GRP_lib_base2_js
{
	obj_hold<str2> 	name;
 //	int	js_id;	// MUST be same as idx
	int	flags;
	int	idx;
	void *	ptr;
	obj_hold<obj_ref> obj;

	jsc_prop_list_item()
	: name( NULL )
 //	, js_id( -1 )
	, flags( 0 )
	, idx( -1 )
	, ptr( NULL )
	, obj()
	{
	}

	jsc_prop_list_item( str2 * _name, int id )
	: name( _name )
 //	, js_id( id )
	, flags( 0 )
	, idx( -1 )
	, ptr( NULL )
	, obj()
	{
	}

	void set_flag( int mask )
	{
		flags |= mask;
	}
	bool get_flag( int mask )
	{
		return flags & mask;
	}
	void reset_flag( int mask )
	{
		flags &= !mask;
	}
};

struct jsc_prop_list  : public GRP_lib_base2_js
{
	dyn_array<jsc_prop_list_item> props;

	bool lookup( str0 _name, int & pos ); // N() if absent
	int alloc_new_prop( str0 _name );
	jsc_prop_list_item * get( int jsid );

	bool alloc_prop( int & jsid, const char * name, jsc_prop_list_item * & prop );
	bool alloc_prop( int & jsid, const char * name, int idx );
	bool alloc_prop( int & jsid, const char * name, obj_ref * obj );
};

} // NAMESPACE
#endif
