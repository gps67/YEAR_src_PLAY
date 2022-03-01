#ifndef TCL_MATCHER_H
#define TCL_MATCHER_H

#include "TCL_STUBS.h"
#include "TCL_REF.h"
#include "TCL_HELP.h"
#include "TCL_PLUS_BASE.h"
#include "TCL_PLUS_LEX.h"
// #include "TCL_MATCHER.h"

namespace TCL {
extern
bool CMP( Tcl_Obj * obj, const char * str );


/*!
	LITERAL_MATCHER * match_GET = new LITERAL_MATCHER(interp,"GET");
	// keep match_GET forever //
	if( match_GET.matches( argv[i] ) { /+ objv[i] is "GET" +/ }

	When objv[i] is a const "GET" in a proc, it is a retained const
	and normally belongs to Tcl
*/
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
	TCL_REF match_one; // LEX1 -or- TCL_SPECIFIC_TOKENISED
	TCL_REF match_two; // LEX2 from outside of proc
	TCL_REF match_three; // another LEX2, similar ways
	TCL_REF differents[N_different];

	~LITERAL_MATCHER()
	{
	}

	LITERAL_MATCHER( Tcl_Interp * interp, const char * str )
	{
		match_one = mk_LEX1( interp, str );
	//	match_one = mk_common_spelling( interp, str );
	}

	/*!
		inline test that obj is already cached
	*/
	bool MATCHES( Tcl_Obj * obj ) {

		// match cache
		if( obj == match_one ) return true;
		if( obj == match_two ) return true;
		if( obj == match_three ) return true;

		// we do NOT KNOW match_one->typePtr is TYPE_LEX1
		// it is not NULL, but might be a Tcl object
		// as the CTOR guarantees that
		//( match_one && match_one->type_Ptr && ... )
		if( obj->typePtr ) {
			// this is FINGERS CROSSED
			// there could be several TYPED tcl types
			// eg VECT is varname is funcname is bytecode is ...
			if( obj->typePtr == match_one->typePtr ) {
			// KNOW match_one not null, typePtr == LEX1
			// KNOW obj is not NULL (semi guarantee)
			// 
			// so BOTH are LEX1 // so no match
			// 
			// else LEX2 goes to fn, plain goes to fn
			// else STRANGE list int other goes to fn
			// we have a majority of LEX1
			// for scripts using proc // 
			}
		#if 1
			// visual tracer
			INFO("FAST MATCH LEX1 != LEX1");
			INFO("FAST MATCH %s != %s",
			 obj->bytes,
			 match_one->bytes
			);
		#endif
			return false;
		}
	/*
		obj -> PTR2 == match_one
		obj -> TYPE == LEX2

		LEX2 catpured in match_two does as good as this already
		but the MATCH_FALSE path should nned to call
		particularly if obj is already LEX1 or LEX2->LEX1
		so fater return of FALSE
	*/

#if 0
		// actually I think that different list is waste of time
		// different cache
		// rewrite as ASM word compare macro
		for( int i = 0; i<N_different; i++ ) {
			if( obj == differents[i] ) {
				return false;
			}
		}
#endif

		// not finding obj means strcmp and update cache required
		return MATCHES_fn( obj );
	}

	bool MATCHES_fn( Tcl_Obj * obj );

//	bool upgrade_to_LEX1( Tcl_Obj * obj ); // no interp nearby
	bool SEE_C_upgrade_to_LEX2( Tcl_Obj * obj, Tcl_Obj * LEX1  ); // 

 }; // class
}; // namespace


#endif
