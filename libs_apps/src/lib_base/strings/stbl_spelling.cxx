#include "HT_table.h"
#include "stbl_spelling.h"

// str2_reused is necessary, but not overtly here
// it is done by ht1.cxx (I hope) (deprec?)
// it is done by HT_table_base.cxx (I hope)
// which stores ONE LOCKED (I wish) str2_reused per HT
// 
// #include "str2_reused.h"

#include "buffer1.h"

//#include <string.h> // memcpy

using namespace STBL;

static HT_table<str2,str2> STBL_Spelling_Pool;


/*
	TODO REWORK:

		this moves to HT_table_base
		ALL tables with str2 keys (of the main_mem persuasion)
		use a common pool of str2 keys (why - hash splits enough!)

		the first call lookup( const char * k ) finds k
		in the local STBL (local vars, elem_pool, etc),
		and upgrades to str2 * key (somehow, maybe HT_PAIR)
		If not found in local DRY (repeat), then hop to
		the central string pool, where a self_deleting_str2
		is created (zero_ref_count kept by POOL, but temp,
		but one_in_list, but on_dispose--remove_from_dry)

		SO: when creating a HT - name the central pool
		SO: its the HT that allocate the str2_key from const char *
		
		
	LURK THREADS

		the table and also str2_reused must both be locked

		a simple spinning lock is OK as its usually V.fast
		and occasionally waiting for malloc.

		that needs to be done in the HT_table_base class
		or in this level function

	LURK NUL
		not sure what happens in sub-classes

 //	LURK overwrite
 //		_poke p0p2 overwrites the pointed string!
 //
 //		in an mmap file, that will cause (copy on write)
 //		or write-on-close

		I'd like to make this class better for p0p2
		as it is clearly how a parser might work over words
		(most of which are less than 8 bytes!)

	POSS ROM str0 - dont malloc
	POSS ROM store - use own area ...  (disk rom chenges everything)

	LURK cset
		you must be consistent to make u8 * s the same
		so 8859 or utf8 but not the same (different spellings)
		
*/

str2 * STBL::Spelling_Pool_lookup_add( const u8 * s )
{
	if(!s) s = (const u8 *) "";

//	str2_reused s2_head;
//	str2 * s2 = & s2_head;
//	s2_head.set_fixed( s );

	str2 * s2_ret = STBL_Spelling_Pool.lookup( (const char *) s );
	if( s2_ret ) return s2_ret;

	s2_ret = new str2( s );
	return STBL_Spelling_Pool.add( s2_ret, s2_ret );
}

// #define NUL 0
#include "CR_LF_NUL.h"

/*!
	... ditto .. but poke a NUL byte at the end of p0p2 (and undo)
*/
str2 * STBL::Spelling_Pool_lookup_add_poke( const p0p2 & s )
{
//	const char * P0 = (char * ) s.p0;
	char * P2 = (char * ) s.p2;
	const char C2 = * P2;
	*P2 = NUL;
	str2 * s2 = STBL::Spelling_Pool_lookup_add( s.p0 );
	*P2 = C2;
	return s2;
}

/*!
	... ditto ... but readonly p0p2 buffer, so copy into a buffer1
*/
str2 * STBL::Spelling_Pool_lookup_add( const p0p2 & s )
{
	buffer1 buf;
	buf.set( s );
	// DONE by set() // buf.trailing_nul();
	return STBL::Spelling_Pool_lookup_add( buf.buff );

//	// I like the idea of not using malloc for buffer1
//	u8 buff[ len + 1 ];
//	memcpy( buff, s.p0, len );
//	buff[len] = NUL;
//
//	str2 * s2 = STBL::Spelling_Pool_lookup_add( buff );
//	return s2;
}


str2 * STBL::Spelling_Pool_lookup_add( buffer1 & buf )
{
	buf.trailing_nul();
	return STBL::Spelling_Pool_lookup_add( buf.buff );
	
}


