#include "str2.h"

#include "stbl_spelling.h"

// str2::str2( const char * s ) : str1(s) {}

//! AVOID - OK - debugging
void str2::prints()
{
	e_print( "'%s'", sget() );
}

str2::~str2()
{
}

//! construct and flag an object for a static var
str2 * new_str2_static( const char * s )
{
	// str2 * s2 = new str2( s );
	str2 * s2 = ::STBL::Spelling_Pool_lookup_add( (const u8 *) s );
//
//	the str2 must still be held in a obj_hold
//	s2->ref_static();	// not enforced but it helps with the counting

	return s2;
}
