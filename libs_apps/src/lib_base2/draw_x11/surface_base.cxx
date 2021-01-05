#include "surface_base.h"

surface_base::
surface_base()
: ht_gc( 
	ht_type_str2,
	ht_type_obj
)
, ht_text( 
	ht_type_str2,
	ht_type_obj
)
{
	gc = NULL;
	text = NULL;
}

surface_base::
~surface_base()
{
}

bool surface_base::set_gc( str0 name_ )
{
	obj_hold<str2> name; name = new str2( name_ );
	return set_gc( name );
}

bool surface_base::set_font( str0 name_ )
{
	obj_hold<str2> name; name = new str2( name_ );
	return set_font( name );
}

bool surface_base::set_gc( str2 * name )
{
	d_gc * gc2;
	gc2 = ht_gc.lookup( name );
	if(gc2) {
		gc = gc2;
		return true;
	}
	gc2 = new d_gc( drawable );
	if(! gc2 ) return false; // THROW
	ht_gc.add( name, gc2 );
	gc = gc2;
	return setup_gc( name );
}

bool surface_base::set_font( str2 * name )
{
	return false;
}

// virtual
bool surface_base::setup_gc( str2 * name )
{
	return false;
}

// virtual
bool surface_base::setup_font( str2 * name )
{
	return false;
}




