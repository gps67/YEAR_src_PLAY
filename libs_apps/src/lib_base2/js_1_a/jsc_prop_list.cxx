#include "jsc_prop_list.h"


// using namespace JSSM;
namespace JSSM {

bool jsc_prop_list::lookup( str0 _name, int & pos )
{
	int n = props.N();
	for( int i=0; i<n; i++ )
	{
		if( !props[i].name->is_same_as( _name ) )
			continue;
		pos = i;
		return true;
	}
	return false;
}

int jsc_prop_list::alloc_new_prop( str0 _name )
{
	int pos;
	if( lookup( _name, pos ) ) return pos;
	int n = props.N();
	props.extend_by( 1 );
	props[n].name = new str2(_name);
	return n;
}

jsc_prop_list_item * jsc_prop_list::get( int jsid )
{
	if( jsid < props.N() )
	{
		return & props[ jsid ];
	}
	e_print(" jsc_prop_list::get( %d ) out of range\n", jsid );
	gdb_break_point();
	return NULL;
}

/*!
	idx is a pre-allocated id, not necessarily what JSSM uses
*/
bool jsc_prop_list::alloc_prop( int & jsid, const char * name, jsc_prop_list_item * & prop )
{
	if( props.N() > 255 ) return false;
	jsid = alloc_new_prop( name );
	prop = & props[jsid];
	return true;
}

/*!
	idx is a pre-allocated id, not necessarily what JSSM uses
*/
bool jsc_prop_list::alloc_prop( int & jsid, const char * name, int idx )
{
	jsc_prop_list_item * prop;
	if( ! alloc_prop( jsid, name, prop ) ) return false;
	prop->idx = idx;
	return true;
}


/*!
	Whatever id is used by JSSM, this object is the jsc's handler
	for that property (which may be the instances prop value
	when each inst has a class of its own )
*/
bool jsc_prop_list::alloc_prop( int & jsid, const char * name, obj_ref * obj )
{
	jsc_prop_list_item * prop;
	if( ! alloc_prop( jsid, name, prop ) ) return false;
	prop->obj = obj;
	return true;
}


} // NAMESPACE

