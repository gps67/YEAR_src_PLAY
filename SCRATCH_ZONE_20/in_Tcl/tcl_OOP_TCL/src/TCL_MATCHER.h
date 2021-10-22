#ifndef TCL_MATCHER_H
#define TCL_MATCHER_H

#include "TCL_STUBS.h"
#include "TCL_PTR.h"
#include "TCL_HELP.h"
// #include "TCL_MATCHER.h"

extern
bool CMP( Tcl_Obj * obj, const char * str );


struct LITERAL_MATCHER {
	//
	//
	//


	// LITERAL_MATCHER // <--HERE-
	// FIELDNAME_MATCHER // <-- ALSO HERE
	// obi_id // NO NO // that is decoder // will GET_STRING it 



	// how many different Tcl_Obj's do we cache

	static const int N_different = 4;

	// these auto DTOR CTOR NULL refcount 
	TCL_PTR match_one;
	TCL_PTR match_two;
	TCL_PTR differents[N_different];

	~LITERAL_MATCHER()
	{
	}

	LITERAL_MATCHER( Tcl_Interp * interp, const char * str )
	{
		match_one = mk_common_spelling( interp, str );
	}

	/*!
		inline test that obj is already cached
	*/
	bool MATCHES( Tcl_Obj * obj ) {

		// match cache
		if( obj == match_one ) return true;
		if( obj == match_two ) return true;

		// different cache
		// rewrite as ASM word compare macro
		for( int i = 0; i<N_different; i++ ) {
			if( obj == differents[i] ) {
				return false;
			}
		}

		// not finding obj means strcmp and update cache required
		return MATCHES_fn( obj );
	}

	bool MATCHES_fn( Tcl_Obj * obj );

	bool upgrade_to_SP1( Tcl_Obj * obj ); // no interp nearby
	bool upgrade_to_SP2( Tcl_Obj * obj ); // no interp nearby

};

typedef LITERAL_MATCHER xTCL_MATCHER;

#endif
