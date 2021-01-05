#include "ht1.h"
#include "glib.h"
#include <glib-object.h>
#include "e_print.h"

// why is this missing today?
// extern "C" void g_object_unref(gpointer object);
// here it is // #include <gtk/gtk.h>
// as if I care
//#include <gtk/gtk.h>


/* glib is a strange beast
extern
GHashTable *
g_hash_table_new_full(
	GHashFunc       hash_func,
	GCompareFunc    key_equal_func,
	GDestroyNotify  key_destroy_func,
	GDestroyNotify  value_destroy_func
);

*/

/*
	Currently key must be a str2
	To fix ...sort out is_same_as, get_hash_key and ...
*/
void ht1::G_destroy_optr( obj_ref * obj )
{
	if(0) {
		e_print("ht1::G_destroy_optr()");
		obj->debug_print_ln(); // incl nl
	}
	obj->ref_decr();
}

/*!
	constructor - key_type defines comparisons, val_type define deletion
*/
ht1::ht1(
	ht_type key_type,
	ht_type val_type
) {
	key_is_ref_counted = TRUE;
	val_is_ref_counted = TRUE;
	key_is_str2 = FALSE;
	key_reused = new str2_reused();
	key_reused->ref_incr();

	ht = NULL;

	// unused 
	GDestroyNotify F_del_key = (GDestroyNotify) G_destroy_optr;
	GDestroyNotify F_del_val = (GDestroyNotify) G_destroy_optr;

	GHashFunc           HASH_FUNC = (GHashFunc)    ht1::G_HashFunc_str2;
	GCompareFunc KEY_COMPARE_FUNC = (GCompareFunc) ht1::G_CompareFunc_str2;

 switch( key_type ) {
  case ht_type_str2: {
	key_is_ref_counted = TRUE;
	key_is_str2 = TRUE;
	F_del_key        = (GDestroyNotify) G_destroy_optr;
	HASH_FUNC        = (GHashFunc)    ht1::G_HashFunc_str2;
	KEY_COMPARE_FUNC = (GCompareFunc) ht1::G_CompareFunc_str2;
  } break;
   case ht_type_optr: {
	key_is_ref_counted = TRUE;
	F_del_key        = (GDestroyNotify) G_destroy_optr;
	HASH_FUNC        = (GHashFunc)    ht1::G_HashFunc_optr;
	KEY_COMPARE_FUNC = (GCompareFunc) ht1::G_CompareFunc_optr;
  } break;
   case ht_type_obj: {
	key_is_ref_counted = TRUE;
	F_del_key        = (GDestroyNotify) G_destroy_optr;
	HASH_FUNC        = (GHashFunc)    ht1::G_HashFunc_obj;
	KEY_COMPARE_FUNC = (GCompareFunc) ht1::G_CompareFunc_obj;
  } break;
   case ht_type_void_g_object:
	/*
		use as key needs more thought
		F_del_key = g_object_unref
		... incr where?
		fall thru to next case
	*/
   case ht_type_void: {
	e_print("key_type::ht_type_void\n");
	gdb_break_point();
	key_is_ref_counted = FALSE;
	F_del_key        = (GDestroyNotify) NULL; // not even free()
	HASH_FUNC        = (GHashFunc)    ht1::G_HashFunc_optr;
	KEY_COMPARE_FUNC = (GCompareFunc) ht1::G_CompareFunc_optr;
  } break;
  default:
	ht = NULL;
	// throw something
	return;
 }

 switch( val_type ) {
  case ht_type_str2: {
	val_is_ref_counted = TRUE;
	F_del_val        = (GDestroyNotify) G_destroy_optr;
  } break;
   case ht_type_optr: {
	val_is_ref_counted = TRUE;
	F_del_val        = (GDestroyNotify) G_destroy_optr;
  } break;
   case ht_type_void: {
	val_is_ref_counted = FALSE;
	e_print("val_type::ht_type_void\n");
	gdb_break_point();
	F_del_val        = (GDestroyNotify) NULL;
  } break;
   case ht_type_void_g_object: {
	val_is_ref_counted = FALSE; // by GLIB not by obj_ref
	if(0) e_print("OK val_type::ht_type_void_g_object\n");
	if(0) gdb_break_point();
	F_del_val        = (GDestroyNotify) g_object_unref;
	F_del_val        = (GDestroyNotify) NULL;
  } break;
  default:
	ht = NULL;
	return;
	// throw something
 }

#ifdef WITH_GTK1
	static bool warned = false;
	if(!warned) {
		e_print("g_hash_table_new() - without item delete\n");
		warned = true;
	}
 /*
	For some reason I was using the plain version - without deletes!
	I really dont understand how, as I thought the ref_count had
	been debugged! 

	There are also new-code (unchecked) issues with obj_ref::ht_*
 */

	ht = g_hash_table_new(
		HASH_FUNC,
		KEY_COMPARE_FUNC
	);
#else
	ht = g_hash_table_new_full(
		HASH_FUNC,
		KEY_COMPARE_FUNC,
		F_del_key,
		F_del_val
	);

#endif
}

VAL_T * ht1::add( KEY_T * key,VAL_T * val ) {
	if( ! ht ) {
		// never happens
		fprintf(stderr,"ht_add NULL ht\n" );
		// return NULL;
	}
	if( ! val ) {
		// you can have a null value but was it found?
		fprintf(stderr,"ht_add NULL val\n" );
		// return NULL;
	}
	if( ! key ) {
		// never happens
		fprintf(stderr,"ht_add NULL key val.%p\n", val );
		return NULL;
	}
/*
http://127.0.0.1/gtk/glib/glib-hash-tables.html#G-HASH-TABLE-INSERT
If the key already exists in the GHashTable its current value is replaced
with the new value. If you supplied a value_destroy_func when creating the
GHashTable, the old value is freed using that function. If you supplied a
key_destroy_func when creating the GHashTable, the passed key is freed
using that function.
*/
	if( key && key_is_ref_counted )
		key->ref_incr();
	if( val && val_is_ref_counted )
		val->ref_incr();
	g_hash_table_insert (ht, key, val );
	return val;
}

ht1::~ht1() {
	if(0) e_print("delete ht1\n");
	// delete key_reused; // mixed up metaphor
	if( key_reused) {
		key_reused->set_NULL(); //
		key_reused->ref_decr(); // should never be shared !
		key_reused = NULL;
	}

	// destroy each item in table
	remove_all();
	g_hash_table_destroy( ht );
}

VAL_T * ht1::lookup( KEY_T * key ) {
	return (VAL_T *) g_hash_table_lookup (ht, key);
}

VAL_T * ht1::lookup( const char * key ) {
	/*
		dirty trick to avoid copying key
		splice its value into an existing str2
		and NEVER free the key
	*/
	if( !key_is_str2 ) {
		throw "key_is_not_str2";
	}
	key_reused->set_fixed( key );
	// LURK // not thread safe here
	return lookup( key_reused );
}

bool ht1::remove( const char * key ) {
	/*
		dirty trick to avoid copying key
		splice its value into an existing str2
		and NEVER free the key
	*/
	if( !key_is_str2 ) {
		throw "key_is_not_str2";
	}
	key_reused->set_fixed( key );
	// LURK // not thread safe here
	return remove( key_reused );
}

/*
	C functions for glib
*/
guint ht1::G_HashFunc_str2( KEY_T * key_ )
{
	str2 * key = (str2 *) key_;
	return (guint) key->get_hash_key();
}

gint ht1::G_CompareFunc_str2(
	KEY_T * key_a_,
	KEY_T * key_b_
) {
	str2 * key_a = (str2 *) key_a_;
	str2 * key_b = (str2 *) key_b_;
	return (guint) key_a->is_same_as( * key_b );
}

/*
	This is where 'any' object can be used as hash keys.
	The obj_ref must provide VIRTUAL functions that do this.
*/

guint ht1::G_HashFunc_obj( KEY_T * _key )
{
	return _key -> ht_get_hash();
}

gint ht1::G_CompareFunc_obj(
	KEY_T * key_a,
	KEY_T * key_b
) {
	// VIRTUAL but no knowledge of type of key_b
	return key_a -> ht_equals( key_b );
}

guint ht1::G_HashFunc_optr( KEY_T * _key )
{
	return (guint) reinterpret_cast<long long>( _key );
}

gint ht1::G_CompareFunc_optr(
	KEY_T * key_a_,
	KEY_T * key_b_
) {
	return key_a_ == key_b_;
}

gboolean ht1::G_does_delete( 
	KEY_T * key,
	VAL_T * val,
	ht1 * self
) {
	if(0) fprintf(stderr,"remove_all - one\n" );
	if(!key) {
		fprintf(stderr, "** WARNING ** ht1::remove() NULL key\n");
		// return FALSE;
	}
	if(!val) {
		fprintf(stderr, "** WARNING ** ht1::remove() NULL val\n");
		// return FALSE;
	}
#ifdef WITH_GTK1
	// with myapp -most things are also in a obj_list
	// so they wont get deleted  here
	// else put into a global holder for one cycle
	// NOW DONE BY GLIB - callback
	if( key && self->key_is_ref_counted )
		key->ref_decr();
	if( val && self->val_is_ref_counted )
		val->ref_decr();
#endif
	return TRUE; // remove from table
}

// here key value and file are pre-type-cast (ie secretly when func saved)

void ht1_my_foreach_print( 
	KEY_T * key,
	VAL_T * val,
	FILE * file
) {
	if(val) val->prints();
}

// void ht1::print_all( FILE * f ) {
// 	GHFunc printer = (GHFunc) ht1_my_foreach_print ;
// 	g_hash_table_foreach (ht, printer, f);
// }
// void ht1::store_all( DB_sar * sql ) {
// 	GHFunc printer = (GHFunc) ht1_my_foreach_store ;
// 	g_hash_table_foreach (ht, printer, sql );
// }
//
void ht1::remove_all() {
	if(0) fprintf(stderr,"remove_all - start\n" );
	GHRFunc remove = (GHRFunc) ht1::G_does_delete;
	g_hash_table_foreach_remove (ht, remove, this);
	if(0) fprintf(stderr,"remove_all - end\n" );
}
bool ht1::remove( KEY_T * key2 )
{
	KEY_T * key;
	VAL_T * val;
	if( !g_hash_table_lookup_extended(
		ht,
		key2,
		(void **) &key,
		(void **) &val
	))
	{
		e_print("ht1::remove - lookup missing key\n");
		return false;
	}
	// e_print(".");
	g_hash_table_remove( ht, (void *)key2 );
#ifdef WITH_GTK1
	if( key && key_is_ref_counted )
		key->ref_decr();
	if( val && val_is_ref_counted )
		val->ref_decr();
#endif
	return true;
}

bool ht1::rename( KEY_T * key1, KEY_T * key2 ) // but not NULL
{
	VAL_T * val1 = lookup( key1 );
	if( !val1 ) return FALSE;
	VAL_T * val2 = lookup( key2 );
	if( val2 ) return FALSE;
	add( key2, val1 );
	remove( key1 );
	return TRUE;
}
