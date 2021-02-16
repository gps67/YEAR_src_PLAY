#include "XML_DTD.h"

/*
	I now think that HT should do this
	possibly as a side effect of the first lookup
	the HT will need to be initialised with the threads pool
	(if not global)
*/
#include "stbl_spelling.h"                                                      

using namespace XML_DTD;

bool T_NS_POOL::init_global_pool()
{
	anon_name_space = new T_NS();
	return true;
}

T_NS * T_NS_POOL::lookup_add_namespace( const p0p2 & name_space )
{
	obj_hold<str2> str_name_space = STR2_TOKENISE( name_space );
	T_NS *	ns = ht_namespace.lookup( str_name_space );
	if( ns ) return ns;

	/*
		actually need a lookup mechanism via SPIN_TOP
		so that nested queries go back to the top
	*/
	ns = new T_NS();
	ns->ns_pool = this;
	ns->prefix = str_name_space; // hmm
	ns->dtd = new T_DTD(); // hmm
	ns->dtd->NS = ns;
	return ns;
}


T_NS * 	T_DTD::lookup_add_namespace( p0p2 & name_space ) {
	if( name_space == NS->prefix ) return NS;
	return NS->ns_pool->lookup_add_namespace( name_space );
}

T_elem_spec * T_DTD::lookup_elem( p0p2 & name )
{
	tmp_name.set( name );
	T_elem_spec * spec = ht_elem_spec.lookup( tmp_name );
	if(spec) return spec;

	/*
		MOVE THIS
	*/
	spec = new T_elem_spec( this );
	spec->name = STR2_TOKENISE( name );
//	WARN("WAS MISSING RET_VAL");
	return spec;
}

T_elem_spec * T_DTD::lookup_elem( p0p2 & space, p0p2 & name )
{
	// LURK - befuddled
	if( space == name_space ) 
		return lookup_elem( name );

	T_NS * ns_dtd = lookup_add_namespace( space );
	return ns_dtd->dtd->lookup_elem( name );
}

