#ifndef HT_table_base_H
#define HT_table_base_H


#include "HT_List_Item.h"

struct p0p2;

/*!
	A hash table base class for use in derived templates.

	HT_List_Item_base does lots of the work, and holds a copy 
	of the u32/u64 hash, which could reduce page hits (if they
	were all allocated adjacently).

	The key and val types are flexible enough for the derived template
	code to cast things. key doesnt have to be a str2, but when it is,
	its useful to have "str2_reused * key_reused;" which doesnt use
	malloc, for searches but is very temp.

	This HT is slightly different from other peoples, as items do
	not bounce from slot to slot, but build a list held by each slot.
	The items hash (u32) is not recalculated when the slot count changes,
	and it is likely that all items in a slot have different hash values,
	which speeds things up a lot. That allows N_items to exceed N_slots,
	and still be optimal (you dont need 1:2) as long as the lists dont
	get too long.
*/
class HT_table_base : public GRP_lib_play
{
	//! A flat array of slots
	obj_list<HT_List_Item_base>  slots;

	//! The number of slots - faster than N() calc from nbytes
	int N_slots;

	//! The number of items - rebuild the HT at hi/lo tide
	int N_items;

	HT_Type key_type;	//<! the key is a str2 or ...
	HT_Type val_type;	//<! the val is a str2 or ...

	//! fast? flag
	bool key_is_str2;

	//! temp key string holder
        str2_reused * key_reused;

	/*!
		add 1 to N_incr and maybe expand the table
	*/
	void N_items_incr()
	{
		N_items ++;
		if( N_items > (N_slots*2) )
		{
			expand();
		}
	}

	/*!
	*/
	void anchor_add( 
		 obj_hold<HT_List_Item_base>  * anchor,
		 HT_List_Item_base  * LI
	)
	{
		assert( LI );
		LI->anchor_add( anchor );
		N_items_incr();
	}

	/*!
	*/
	void anchor_del( 
		 obj_hold<HT_List_Item_base>  * anchor
	)
	{
		HT_List_Item_base  * LI = * anchor;
		anchor_del( anchor, LI ); // this is silly - here !
	}

	/*!
	*/
	void anchor_del( 
		 obj_hold<HT_List_Item_base>  * anchor,
		 HT_List_Item_base  * LI
	)
	{
		assert( LI );
		LI->anchor_del( anchor );
		N_items --;
		if( N_items < (N_slots/4) )
		{
			unexpand();
		}
	}

	static
	str0 str_HT_Type( HT_Type t );

	
	// DEBUG ONLY - this is single threaded, and AVOID recalling
	// if that does happen ... ? defer somehow using a keep
	HT_lockable lock;
	void lock_check_1() { lock.lock_check_1(); } // DEBUG only

	bool LI_locate(
		obj_hold<HT_List_Item_base> ** returned_anchor,
		uint h1,
		obj_ref * key,
		bool add_first
	);

	void LI_add( HT_List_Item_base * LI );

 public:

	/*!
	*/
	void expect_key_type( HT_Type expected )
	{
		if( expected != key_type ) fn_expect_key_type( expected );
	}
	/*!
	*/
	void expect_val_type( HT_Type expected )
	{
		if( expected != val_type ) fn_expect_val_type( expected );
	}
	void fn_expect_key_type( HT_Type expected );
	void fn_expect_val_type( HT_Type expected );


	virtual ~HT_table_base();
	HT_table_base(
                HT_Type key_type_ = HT_Type_str2,
                HT_Type val_type_ = HT_Type_obj,
		int sz = 1
	);

	/*
		MAYBE - there should be a lookup system
		which holds a buffer and a lock and ....
		OR - p0p2 is enough, as its usually below 5 ?

	obj_ref * lookup( p0p2 key );
	obj_ref * lookup( buffer1 & key );

	*/

	obj_ref * lookup( obj_ref * key );
	obj_ref * lookup( str0 key );
	obj_ref * lookup( buffer1 & key );
	obj_ref * add( obj_ref * key, obj_ref * val );
	bool remove( obj_ref * key );
	bool rename( obj_ref * k1, obj_ref * k2 );
	void unexpand();
	void expand();
	void remove_all();

};

#endif
