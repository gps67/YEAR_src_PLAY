#ifndef HT_List_Item_H
#define HT_List_Item_H

#include "obj_hold.h"
#include "obj_list.h"
#include "str2_reused.h"
#include "HT_lock.h"

// struct GObject; 
struct g_object; 
// #include <gtk/gtk.h> // g_object
#include "g_obj.h"

#include "dgb_fail.h"

enum HT_Type {
        HT_Type_str2 = 0,	// most common key
        HT_Type_obj = 1,	// most general obj_ref key, common value
        HT_Type_void = 2,	// key is int, val is unusual
        HT_Type_void_g_object = 3 // key is void pointer, val gets obj decref
};

/*
	// code to paste
	switch( val_type )
	{
         case HT_Type_str2:
         case HT_Type_obj:
         case HT_Type_void:
         case HT_Type_void_g_object:
	}
*/


// typedef unsigned int uint;

class HT_table_base;

/*!
	a part of HT_table_base. A list of items in the same slot.

	Items in the list tend to have (2M:1) different hash values,
	(slot = hash % N_slots), and the keys are NOT touched to
	get the hash each time (assist paged out situations).

	SINCE THIS ALREADY HAS A VIRTUAL DESTRUCTOR (OBJ-REF)
	IT COULD BE REWORKED TO A SIBLING THAT HOLDS G_OBJECT
	TODO
*/
class HT_List_Item_base : public obj_ref
{
	friend class HT_table_base;

	obj_hold<HT_List_Item_base>  ht_next;
 public:

	//! The key supplies the hash, but it is repeated here for speed
	// uint hash;
	long long hash; // TODO use UNION to store VOID * where obj_hold was
	//! The key must provide ht_get_hash() ht_equals(key2) (eg str2)
	obj_hold<obj_ref> key;
	//! The val is ref_counted
	obj_hold<obj_ref> val;
	//! val can be GObject (kludged for a while) A Ref is kept
	g_obj_perm val_g_object; // manual ref counts

	/*!
		new LI items 
	*/
	HT_List_Item_base(
		uint h,
		obj_ref * k,
		obj_ref * v,
		GObject * g
	)
	: obj_ref()
	, ht_next()
	, hash( h )
	, key( k )
	, val( v )
	, val_g_object( g )
	{
	}

	/*!
		set_key to ptr, set hash, dont change place in table

		The key can have different types. key_type is supplied
		by the table.

		The normal key_type is str2 or an obj.

		VOID keys are only supported as ints (point to nothing).

		INT keys are supported as pointers (held in hash NOT key).

		GObject keys are not really needed.
	*/
	void set_key( HT_Type key_type, void * ptr )
	{
		switch( key_type )
		{
		 case HT_Type_str2:
		 case HT_Type_obj:
			key = (obj_ref *) ptr;
			hash = key->ht_get_hash();
		 break;
		 case HT_Type_void:
		 {
			/*
				ptr can be an INT or a pointer
				No ref_counting, or dereferencing
			*/
			if (sizeof( ptr ) > sizeof( hash ))
				throw "the pointer is stored in the unsigned int";

			key = NULL;
	//		hash = (uint) ptr;
			hash = reinterpret_cast<long long>( ptr );
		 }
		 break;
		 case HT_Type_void_g_object:
		 {
			THROW_dgb_fail( "# GObject cant be key" );
			// would need to get hash of string which is GObject
			// but GObject might be other things!
			// val_g_object = (GObject *) ptr;
		 }
		}
	}

	/*!
		set val to ptr
	*/
	void set_val( HT_Type val_type, void * ptr )
	{
		switch( val_type )
		{
		 case HT_Type_str2:
		 case HT_Type_obj:
			val = (obj_ref *) ptr;
		 break;
		 case HT_Type_void:
		 {
			/*
				The value cant be VOID, because I dont need that
				and there isnt a variable for it.

				Could maybe use val_g_object (renamed),
				but destructor would need to know val_type
			*/
			THROW_dgb_fail( "# void cant be val" );
		 }
		 break;
		 case HT_Type_void_g_object:
		 {
			/*
				whilst still using glib there is still a holder
			*/
			val_g_object = (GObject *) ptr;
		 }
		}
	}

	/*!
		get key cast as a void * pointer (eg from int)
	*/
	void * get_key( HT_Type key_type )
	{
		switch( key_type )
		{
		 case HT_Type_str2:
		 case HT_Type_obj:
		 {
			return key;
		 }
		 break;
		 case HT_Type_void:
		 {
			return (void *) hash;
		 }
		 break;
		 case HT_Type_void_g_object:
		 {
			THROW_dgb_fail( "# key cant be g_object" );
			return val_g_object.OBJ();
		 }
		}
		THROW_dgb_fail( "# key_type unknown" );
		return NULL;
	}

	/*!
		get val as a void * pointer (eg from int)
	*/
	void * get_val( HT_Type val_type )
	{
		switch( val_type )
		{
		 case HT_Type_str2:
		 case HT_Type_obj:
		 {
			return val;
		 }
		 break;
		 case HT_Type_void:
		 {
			THROW_dgb_fail( "# void cant be val" );
			return val_g_object.OBJ();
		 }
		 break;
		 case HT_Type_void_g_object:
		 {
			return val_g_object.OBJ();
		 }
		}
		THROW_dgb_fail( "# key_type unknown" );
		return NULL;
	}

	/*!
		destructor - ref_count DECR all things
	*/
	~HT_List_Item_base()
	{
		hash = 0;
		key = NULL;
		val = NULL;
		set_val( HT_Type_void_g_object, NULL );
	}

	/*!
		add THIS at the front of the list held at anchor
	*/
	void anchor_add( obj_hold<HT_List_Item_base>  * anchor )
	{
		if(!anchor)
		{
			throw "anchor_add(NULL)";
		} else {
			ht_next = * anchor;
			* anchor = this;
		}
	}

	/*!
		del THIS from the front of the list held at anchor
	*/
	void anchor_del( obj_hold<HT_List_Item_base>  * anchor )
	{
		if(!anchor)
		{
			throw "anchor_add(NULL)";
		}
		// debugging only
		if( * anchor != this )
		{
			throw "anchor_del() bad anchor";
		}
		* anchor = ht_next;
	}

// //	It makes almost no sense to use THIS as a key in a someone elses HT
// //	except maybe as ADDRESS
//
//	// virtual 
//	uint ht_get_hash();
//
//	// virtual 
//	bool ht_equals( obj_ref * key2 );

};


class HT_List_Item_o_o : public HT_List_Item_base
{
 public:

	/*!
	*/
	HT_List_Item_o_o(
		uint h,
		obj_ref * k,
		obj_ref * v
	)
	: HT_List_Item_base( h, k, v, NULL )
	{
	}
};

class HT_List_Item : public HT_List_Item_o_o
{
 public:

	/*!
	*/
	HT_List_Item(
		uint h,
		obj_ref * k,
		obj_ref * v
	)
	: HT_List_Item_o_o( h, k, v )
	{
	}

	/*!
	*/
	HT_List_Item(
		obj_ref * k,
		obj_ref * v
	)
	: HT_List_Item_o_o( 0, k, v )
	{
		if( k )
			hash = k->ht_get_hash();
	}

};

#endif
