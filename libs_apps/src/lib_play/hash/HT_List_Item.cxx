
#include "HT_List_Item.h"
#include "obj_hold.h"

#if 0
// virtual 
/*!
	This is unused
*/
uint HT_List_Item::ht_get_hash()
{
	if( hash ) return hash;
	if( !key ) return 0;
	hash = key -> ht_get_hash();
	return hash;
}

// virtual 
/*!
	This is unused
*/
bool HT_List_Item::ht_equals( obj_ref * key2 )
{
	if( !key ) return false;
	return key -> ht_equals( key2 );
}
#endif

