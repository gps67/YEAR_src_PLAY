
#include "HT_table_base.h"
#include "dgb_fail.h"
#include "p0p2.h"
#include "buffer1.h"


/*!
*/
HT_table_base::HT_table_base(
	HT_Type key_type_,
	HT_Type val_type_,
	int sz
)
{
	// if( sz < 1 ) sz = 1;
	if( sz < 1 ) sz = 0;

	// cant set a breakpoint in the CTOR! zone // gdb_invoke(1);

	key_type = key_type_;
	val_type = val_type_;
	slots.resize( sz );
	N_items = 0;
	N_slots = sz;
	key_is_str2 = false;
	key_reused = NULL;

	// THIS is not an obj // debug_set_obj_name("HT_table_base_ANON");

	switch( key_type ) {
	 case HT_Type_str2:
		key_is_str2 = true;
		key_reused = new str2_reused();
		key_reused->ref_incr();
	// gdb here is a tad early // gdb_invoke(true);
		key_reused->debug_set_obj_name("key_reused");
	 break;
         case HT_Type_obj:
		// OK
	 break;
         case HT_Type_void:
		// OK
	 break;
         case HT_Type_void_g_object:
		THROW_dgb_fail( "key cannot be of type g_object" );
	 break;
	 default:
		;
	}

	switch( val_type ) {
	 case HT_Type_str2:
         case HT_Type_obj:
		// OK
	 break;
         case HT_Type_void:
		THROW_dgb_fail( "val cannot be of type void" );
	 break;
         case HT_Type_void_g_object:
		// OK
	 break;
	 default:
		;
	}

}

/*!
*/
HT_table_base::~HT_table_base()
{
	if( key_reused) {
		key_reused->set_NULL(); //
		key_reused->ref_decr(); // should never be shared !
		key_reused = NULL;
	}
}

// static
/*!
*/
str0 HT_table_base::str_HT_Type( HT_Type t )
{
	switch( t )
	{
         case HT_Type_str2:	return "str2";
         case HT_Type_obj:	return "obj";
         case HT_Type_void:	return "void";
         case HT_Type_void_g_object: return "void_g_object";
	}
	return "HT_Type::NOT_RECOGNISED";

}

/*!
*/
void HT_table_base::fn_expect_key_type( HT_Type expected )
{
	// perfect match (hope not VOID)
	if( expected == key_type ) return;

	// allow str2 to be an obj
	if(( expected == HT_Type_obj )  && ( key_type == HT_Type_str2 )) return;

	// but obj is not a str2
	//if(( expected == HT_Type_str2 ) && ( key_type == HT_Type_obj )) return;
	FAIL("# source expected %s table wants %s\n",
		(STR0) str_HT_Type( expected ),
		(STR0) str_HT_Type( key_type )
	);
}

/*!
*/
void HT_table_base::fn_expect_val_type( HT_Type expected )
{
	if( expected == val_type ) return;
	if(( expected == HT_Type_obj )  && ( val_type == HT_Type_str2 )) return;
	FAIL("# source expected %s table wants %s\n",
		(STR0) str_HT_Type( expected ),
		(STR0) str_HT_Type( val_type )
	);
}

/*!
	key might be a temp - try to keep already used one

	Return bool-was-found.

	Return the link that points to the found LI,
	or should point to the (absent) LI.
*/
bool HT_table_base::LI_locate(
	obj_hold<HT_List_Item_base> ** returned_anchor,
	uint h1,
	obj_ref * key,
	bool add_first
)
{
	lock_check_1();

	if(!N_slots) expand();

	// THIS is the only place the slot is calculated !!
	int s1 = h1 % N_slots;

	obj_hold<HT_List_Item_base> *  anchor = & slots[ s1 ];
	obj_hold<HT_List_Item_base> *  front_anchor = anchor;
	while( true )
	{
		HT_List_Item_base * L1 = * anchor;
		if( !L1 ) break;
		if( L1->hash == h1 )
		 switch( key_type )
		 {
		 case HT_Type_str2:
		 case HT_Type_obj:

			assert( L1->key );
			// maybe (TEMP) key should provide comparison
			if( L1->key->ht_equals( key ) )
			{
				* returned_anchor = anchor;
				return true;
			}

		 break;
		 case HT_Type_void:
			// hash is the key
			// nb ptr cannot be deleted
			// but it is never used as anything

			* returned_anchor = anchor;
			return true;

		 break;
		 case HT_Type_void_g_object:
			// g_object NOT supported as key
		 default:
			throw "# CODE ERROR # illegal key type";

			* returned_anchor = anchor;
			return true;

		 }
		anchor = & L1->ht_next;
	}
	// not found, but should be located (at start/end of list)
	if( add_first )
	{
		anchor = front_anchor;
	}
	* returned_anchor = anchor;
	return false;
}

/*!
	This is a tricky private function (for expand).

	.hash is alreay set
	.key is already set
	.val is already set
	.ht_next will get overwritten

	caller must check that ht_next isnt lost by accident!
*/
void HT_table_base::LI_add( HT_List_Item_base * LI )
{

	obj_hold<HT_List_Item_base> * anchor = NULL;
	if( LI_locate( &anchor, LI->hash, LI->key, false ) )
	{
		/*
			actually the item is NOT expected to be found
			but if its already there, try to retain the
			OLD LI and the OLD key. ref counting deletes LI
		*/
		HT_List_Item_base * found = * anchor;
		switch( val_type )
		{
		 case HT_Type_str2:
		 case HT_Type_obj:

			found->val = LI->val;

		 break;
		 case HT_Type_void:

			THROW_dgb_fail( "# CODE ERROR # val_type cannot be void" );

		 break;
		 case HT_Type_void_g_object:

			found->set_val( HT_Type_void_g_object, LI->val_g_object );
		}

		// retain found->hash
		// retain found->key
		// retain found->ht_next
	} else {
		/*
			item not found,
			* anchor was pointer to (LI->ht_next)
			* anchor becomes pointer to (LI)
		*/
		anchor_add( anchor, LI );
	}
}

/*!
*/
obj_ref * HT_table_base::lookup( obj_ref * key )
{
	expect_val_type( HT_Type_obj );
	HT_lock_user lok( lock );
	uint h1 = 0;
	switch( val_type )
	{
	 case HT_Type_str2:
	 case HT_Type_obj:

		h1 = key -> ht_get_hash();

	 break;
	 case HT_Type_void:

		h1 = (uint) reinterpret_cast<long long>(key);
		key = NULL;

	 break;
	 case HT_Type_void_g_object:

		THROW_dgb_fail("# CODE ERROR # lookup() key is g_object" );
	}
	obj_hold<HT_List_Item_base> * anchor = NULL;
	if( LI_locate( &anchor, h1, key, false ) )
	{
		HT_List_Item_base * found = * anchor;
		if( val_type == HT_Type_void_g_object)
#warning G_OBJECT_CAST_INTO_obj_ref_PTR
			return (obj_ref *) found->val_g_object.OBJ();
		else
			return found -> val;
	} else {
		return NULL;
	}
}

/*!
*/
obj_ref * HT_table_base::lookup( str0 key )
{
	/*
		This function dirty trick to avoid copying key
		splice its value into an existing str2
		and NEVER free the key
	*/

	// table must use strings as keys (most do)
	expect_key_type( HT_Type_str2 );

	key_reused->set_fixed( key );
	return lookup( key_reused );
}

/*!
*/
obj_ref * HT_table_base::lookup( buffer1 & key )
{
	/*
		This function dirty trick to avoid copying key
		splice its value into an existing str2
		and NEVER free the key
	*/

	// table must use strings as keys (most do)
	expect_key_type( HT_Type_str2 );

	key_reused->set_fixed( (STR0) key );
	return lookup( key_reused );
}

/*!
obj_ref * HT_table_base::lookup( p0p2 key )
{
	/ *
		dirty trick to avoid copying key
		splice its value into an existing str2
		and NEVER free the key
	* /
	expect_val_type( HT_Type_str2 );

	key_reused->set_fixed( key );
	return lookup( key_reused );
}
*/

/*!
*/
obj_ref * HT_table_base::add( obj_ref * key, obj_ref * val )
{
	expect_key_type( HT_Type_obj ); // str2 is OK
	expect_val_type( HT_Type_obj );

	HT_lock_user lok( lock );
	obj_hold<HT_List_Item_base> * anchor = NULL;
	uint h1 = key -> ht_get_hash();
	if( LI_locate( &anchor, h1, key, false ) )
	{
		HT_List_Item_base * found = * anchor;
		found -> val = val;
		return val;
	}

	// LI_locate didnt find it, but decided where it should go

	// prepare new item at front of list
	HT_List_Item *
	L1 = new HT_List_Item( h1, key, val );
	
	anchor_add( anchor, L1 );

	return val;
}

/*!
*/
bool HT_table_base::remove( obj_ref * key )
{
	expect_key_type( HT_Type_obj ); // str2 is OK

	HT_lock_user lok( lock );
	uint h1 = key -> ht_get_hash();
	obj_hold<HT_List_Item_base> * anchor = NULL;
	if( !LI_locate( &anchor, h1, key, false ) )
		return false;

	// delete the item from the queue
	anchor_del( anchor );
	return true;
}

/*!
*/
bool HT_table_base::rename( obj_ref * k1, obj_ref * k2 )
{
	expect_key_type( HT_Type_obj ); // str2 is OK

	HT_lock_user lok( lock );
	uint h1 = k1 -> ht_get_hash();
	obj_hold<HT_List_Item_base> * anchor = NULL;
	if( !LI_locate( &anchor, h1, k1, false ) )
		return false; // not found

	HT_List_Item_base * found = * anchor;
	obj_hold<HT_List_Item_base> tempkeeper = found;

	// remove found from where it was found
	anchor_del( anchor );

	// where does k2 go
	found -> key  = k2;
	found -> hash = k2->ht_get_hash();

	LI_add( found );

	return true;
}

/*!
*/
void HT_table_base::unexpand()
{
	expand();
}

/*!
*/
void HT_table_base::expand()
{
	// table can be 1/part_full, upto over_full
	// thats slots per item and items per slot (avg)
	int part_full = 4;
	int over_full = 2;
	if( ( N_slots/part_full < N_items ) && ( N_items < N_slots * over_full ) )
	{
		e_print("expand - already OK\n");
		return;
	}

	// move data from slots to slots2, via temp list
	obj_list<HT_List_Item_base>  slots2;
	int N_slots_old = N_slots;
	slots2.resize( N_slots_old );
	for( int s = 0; s<N_slots_old; s++ )
	{
		slots2[s] = slots[s];
		slots[s] = NULL;
	}

	// allocate a new slots array
	N_slots = N_items + 10;
	slots.resize( N_slots );
	// use up every available byte (blk1 over allocated)
	N_slots += slots.space_avail();
	slots.resize( N_slots );
	assert( ! slots.space_avail() );

	// add each LI back to this array
	obj_hold<HT_List_Item_base> keep1;
	obj_hold<HT_List_Item_base> keep2;
	for( int s = 0; s<N_slots_old; s++ )
	{
		HT_List_Item_base * L1 = NULL;
		HT_List_Item_base * L2 = slots2[ s ];
		keep2 = L2; // setup loop tail

		while( (L1 = L2) ) // assigned not NULL
		{
			L2 = L1->ht_next;
#if 0
			keep1 = L1; // was kept in keep2
			keep2 = L2; // keep for a while
#endif
#if 0
			keep1.take( keep2 );
			keep2 = L2; // keep for a while
#endif
#if 1
			keep1.take2( keep2, L2 );
			// keep1 = keep2 == L1
			// keep2 = L2
#endif

			L1->ht_next = NULL; // L2 kept in keep2
			LI_add( L1 );
		}
		slots2[s] = NULL;
	}
}

/*!
*/
void HT_table_base::remove_all()
{
	HT_lock_user lok( lock );
	int N = N_slots; 
	for( int s1 = 0; s1 < N; s1++ )
	{
		slots[ s1 ] = NULL;
	}
	N_items = 0;
}


