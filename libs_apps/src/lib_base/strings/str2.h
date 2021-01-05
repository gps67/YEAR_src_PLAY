#ifndef str2_H
#define str2_H

#include <string.h>
#include <stdlib.h> // malloc
#include "misc.h"
#include "obj_ref.h"
#include "str1.h"

/*!
	a string object with a hash, flags and a ref_count

	LURK: copied strings dont copy the flags!
	(could copy the hash)

	FLAGS-POSS-MAYBE:

		is_readonly
		is_not_malloced_like_it_should_be // default _is_

		is_cset_ascii
		is_cset_8859_9
		is_cset_utf8
		is_cset_UDEF_BYTE
		is_cset_OTHER

		is_word_edge_left
		is_word_edge_inside
		is_word_edge_right

		has_ML_inside	// ie decode required
		is_ML_emcoded // &amp; done <POSS/> 
		is_XX_encoded // groff, shell, tcl, ....

	VTBL-FLAGS-AND-ACTIONS-MAYBE:

		get_cset_flags( info_zone & info )
		get_LANG()
		get_SPEC()

*/
class str2 : public obj_ref, public str1
{
	int hash_stored;
 public:
	void clear_hash()
	{
		hash_stored = 0;
	}

	void prints();

	~str2();

	str2()
	: obj_ref()
	, str1()
	{
		clear_hash();
	}

/*
	str2( const uchar * s )
	: obj_ref()
	, str1( s )
	{
		clear_hash();
	}

	str2( const char * s )
	: obj_ref()
	, str1( (const uchar *) s )
	{
		clear_hash();
	}
*/

	str2( const str0 & s0 )
	: obj_ref()
	, str1( s0 )
	{
		clear_hash();
	}

	str2( const str2 & s2 )
	: obj_ref()
	, str1( s2 )
	{
		clear_hash();
	}


/* INHERIT but - now override with str2 clear_hash() */

/*
	void set( const uchar * s ) {
		str1::set( (str0) s );
		clear_hash();
	}

	void set( const char * s ) {
		str1::set( (str0) s );
		clear_hash();
	}
*/

	void set( const str0 & s ) {
		str1::set( s );
		clear_hash();
	}

	void set( const str2 & s2 ) {
		str1::set( s2 );
		clear_hash();
	}

	str2 & operator=( str0 s0 )
	{
		str1::set( s0 );
		clear_hash();
		return *this;
	}
	str2 & operator=( const str2 & s2 )
	{
		str1::set( (str0) s2 );
		clear_hash();
		return *this;
	}

	// INHERIT // void set( const char * s );
	// const char * get() { return str ; }
/*

	bool is_same_as( const str2 * s2 ) {
		return is_same_as( *s2 );
	}

	bool is_same_as( const str2 & s2 ) {
		return (0==strcmp( (char *) str, (char *) s2.str ));
	}
*/

/*
	int str_cmp( const str2 & s2 ) {
		return strcmp( str, s2.str );
	}
	bool is_same_as( const str2 * s2 ) {
		if(!s2) return FALSE;
		return (0==strcmp( (char *) str, (char *) s2->str ));
	}
	bool is_same_as( const char * s2 ) {
		// NULL ??
		return (0==strcmp( (char *) str, (char *) s2 ));
	}
*/

	// INHERIT // uint get_hash_key(); but use cache

	uint get_hash_key()
	{
		if( !hash_stored )
			hash_stored = str1::get_hash_key();
		return hash_stored;
	}

	// virtual obj_ref
	uint ht_get_hash()
	{
		return get_hash_key();
	}

	// virtual obj_ref
	bool ht_equals( obj_ref0 * key2 )
	{
		// LURK CAST
		str2 * s2 = (str2 *) key2;
		return is_same_as( s2 );
	}
	
//	IS_DIFF_t cmp( str2 * s2 );

};

extern str2 * new_str2_static( const char * s ); // simply sets n_static++
// inline str2 * new_str2_static( const char * s ) { return new_str2_static((const uchar *)s);}
#endif
