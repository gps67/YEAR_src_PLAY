#ifndef ht1_h
#define ht1_h

/*!
	The type of the key or field is ...
*/
// typedef
 enum ht_type {
	ht_type_str2 = 0,
	ht_type_optr = 1,
	ht_type_void = 2,
	ht_type_obj = 3,
	ht_type_void_g_object = 4
};

class ht1;


#include <glib.h> // GHashTable

#include <stdio.h> // FILE *
#include "str2.h"
#include "str2_reused.h"

/*
 * 	LURK KEY_T MUST be str2
 * 	until cmp funcs seorted
*/
// #define KEY_T str2
#define KEY_T obj_ref
#define VAL_T obj_ref

/*!
	A hash table of KEY --> VAL
*/
class ht1 : public GRP_lib_base
{
	GHashTable * ht;
	bool	key_is_ref_counted;
	bool	val_is_ref_counted;
	bool	key_is_str2;	// allow str2_reused
	str2_reused * key_reused;
 public:

	 ht1(
		ht_type key_type,
		ht_type val_type
	);
	~ht1();

	VAL_T * add( KEY_T * key, VAL_T * val );
	VAL_T * lookup( KEY_T * key );
	bool rename( KEY_T * key1, KEY_T * key2 );

	VAL_T * lookup( const char * key );	// special case 
	bool remove( const char * key );	// sibling special case

	bool remove( KEY_T * key );
	void remove_all();

 static
	void G_destroy_optr( obj_ref * obj );
 static
	guint G_HashFunc_str2( KEY_T * key );
 static
	guint G_HashFunc_optr( KEY_T * key );
 static
	guint G_HashFunc_obj( KEY_T * key );
 static
	gint G_CompareFunc_str2( KEY_T * key, KEY_T * key2 );
 static
	gint G_CompareFunc_optr( KEY_T * key, KEY_T * key2 );
 static
	gint G_CompareFunc_obj( KEY_T * key, KEY_T * key2 );
 static
	gboolean G_does_delete( 
		KEY_T * key,
		VAL_T * val,
		ht1 * self
	);
	
};

#endif
