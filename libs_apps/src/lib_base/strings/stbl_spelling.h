#ifndef Stbl_spelling_H
#define Stbl_spelling_H

#define STR2_TOKENISE( s ) STBL::Spelling_Pool_lookup_add( s )

struct buffer1;
struct p0p2;

// #include "HT_table.h"
#include "obj_ref.h"

/*!
	STBL = Symbol Table - and all thats based on it

	nb HT is probably in another namespace,
	STBL is how HT is used, and what its used for ...!

	TODO: HT needs yet another version, but from the STBL point of view,
	which only becomes apparent when client code is written ...

	Spelling Pool can be set as default SECONDER for each HT
	THEN - upgrade str2 from char *

	HT_lookup_helper

		retains a buffer1 ?
		retain a SPIN lock - for a while ?
		or single thread cache - retain LI_anchor until ... other loses

*/
namespace STBL {

	struct GRP_STBL {};
	struct GRP_STBL_obj : public obj_ref {};

	/*!
		convert const char * str to shared str2

		str2 * token = STBL::Spelling_Pool_lookup_add( "token" );

		that is then guaranteed to be the same pointer for
		all identical spellings of "token" (which must also have
		been resolved), but that doesnt mean that they are the
		same thing!
	*/
	str2 * Spelling_Pool_lookup_add( const u8 * s );
	str2 * Spelling_Pool_lookup_add( const p0p2 & s );
	str2 * Spelling_Pool_lookup_add_poke( const p0p2 & s );
	str2 * Spelling_Pool_lookup_add( buffer1 & buf );

#if 0
	str2 * Spelling_Pool_lookup_add( const char * s )
	{
		// return STBL::Spelling_Pool_lookup_add( (const u8 *) s );
		return Spelling_Pool_lookup_add( (const u8 *) s );
	}
#endif

}; // namespace

/*
	theres an additional option: new_str2_static()
	it sets ref_count, and might be modified to not delete
	str2 (but add it to a current list). That speeds up exit()
	by not calling free() on every thing ever seen.
*/
#endif
