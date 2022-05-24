#ifndef TCL_MATCH_H
#define TCL_MATCH_H

	// TCL_MATCH is like TCL_MATCHER but shorter
	// LITERAL_MATCH is like LITERAL_MATCHER but shorter
	// drop all LEX1 nonsense
	// go directly for strcmp
	// 

#include "TCL_STUBS.h"
#include "TCL_REF.h"
#include "TCL_HELP.h"
#include "TCL_PLUS_BASE.h"
#include "TCL_PLUS_LEX.h"
// #include "TCL_MATCHER.h"

#include <string.h> // and hopefully its inline version of strcmp or strncmp

namespace TCL {


/*!
	LITERAL_MATCH * match_GET = new LITERAL_MATCH(interp,"GET");
	// keep match_GET forever //
	if( match_GET.matches( argv[i] ) { /+ objv[i] is "GET" +/ }

	When objv[i] is a const "GET" in a proc, it is a retained const
	and normally belongs to Tcl
*/
struct LITERAL_MATCH {

	// auto DTOR CTOR NULL refcount 
	TCL_REF match_one; // plain TCL str

	~LITERAL_MATCH()
	{
	}

	LITERAL_MATCH( Tcl_Interp * interp, const char * str )
	{
		match_one = mk_common_spelling( interp, str );
	}

	/*!
		inline test that obj is already cached
	*/
	bool MATCHES( Tcl_Obj * obj ) {

		// CODE ERROR
		if( !obj ) return FAIL("CODE_ERROR"); 
		if( !match_one ) return FAIL("CODE_ERROR"); 

	 INFO("match_one %s argv[i] %s", match_one->bytes, obj->bytes );

		// cached result - is usually LEX1 in match_one
		// experiments show that TCL internalises
// LOG		if( obj == match_one ) return PASS("match_one");
// there is no match_two
//		if( obj == match_two ) return PASS("match_two");

		if( obj == match_one ) return true;

		if(!obj->bytes) {
			FAIL("WE SHOULD EXPECT A STRING OR post STRING");
			const char * str_obj = Tcl_GetString( obj );
		}
		if(!match_one->bytes) {
			FAIL("CODE_ERROR match_one bytes");
			const char * str_one = Tcl_GetString( match_one );
		}
		return 0 == strcmp( obj->bytes, match_one->bytes );

	}

 }; // class
}; // namespace


#endif
