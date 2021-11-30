
// #include "TCL_ObjType_PLUS.h" // error in "_"
// #include "TCL_PLUS_BASE.h" // error in "_"
#include "TCL_PLUS_LEX.h" // error in "_"
#include "TCL_HELP.h" // P64
#include "dgb.h" // FAIL
#include "TCL_PLUS_LEX.h" // 

using namespace TCL ; // lots of C funcs - each needs TCL:: namespace

int PLUS_LEX_SetFromAnyProc(
 Tcl_Interp *interp,
 Tcl_Obj *obj
)
{
	// look at incoming
	// it is ALWAYS NULL // so far
	if( obj -> typePtr ) {
		INFO("old typePtr -> %s", TCL_get_type_name( obj ));
	}
	
	#if 1 // extra checks
	/*
		to upgrade "Literal" to TYPE LEX1 {"Literal"}
	
		simply set TYPE
		everything else should already be NULL
	*/
	TCL_PLUS_LEX1 * TYPE_LEX1 = get_TYPE_LEX1();
	if( !TYPE_LEX1 ) {
		return TCL_ERROR; // already reported
	}
	if( !TYPE_LEX1 -> name ) {
		// this never happens
		FAIL("TYPE_LEX1 has no name");
		return TCL_ERROR;
	}
	if( !obj -> bytes ) {
		// this never happens / because Tcl cant call to convert a {}
		// hmm or can it - an empty string // points ot ""
		// TCL_FAIL // write FAIL text to TCL val
		FAIL("obj bytes == NULL Literal");
		return TCL_ERROR;
	}
	#endif
	
	INFO("UPGRADING LITERAL %s", obj->bytes );
	
	obj -> typePtr = get_TYPE_LEX1();
	TCL_set_PTR1( obj, NULL );
	TCL_set_PTR2( obj, NULL );
	return TCL_OK;
} // LEX1 set From Any

/*
	all these funcs need reworking to VTBL ones
*/

extern
int LEX2_SetFromAnyProc(
 Tcl_Interp *interp,
 Tcl_Obj *obj
);


TCL_PLUS_LEX1 * TCL:: get_TYPE_LEX1()
{
	// build the TYPE, or cached
	// build it and register it
	//
	// cache var //
	static TCL_PLUS_LEX1 * PLUS_LEX1 = NULL;
	if( PLUS_LEX1 ) return PLUS_LEX1;

	if( 1 )
	INFO("building TYPE_LEX1");

	// I want to call the objects of type LEX1_t LEX1
	// using a temp LEX1 so that it is fully init'd before not NULL
	// 
	TCL_PLUS_LEX1 * LEX1
	= new TCL_PLUS_LEX1(); // _PLUS("LEX1")
	if( !LEX1 ) {
		FAIL("NULL from new PLUS_LEX1" );
		return NULL;
	}
	// LEX1->set_funcs_LEX1(); // done in CTOR
	// CTOR also did KEPT_PTR into DUP field
	// hence derived class for PLUS/LEX1

	LEX1 -> bytes_never_NULL = true; // LEX1 is LEX1 -> bytes
	LEX1 -> PTR2_is_Tcl_Obj = false; // default, but for clarity

	// save the cached local value

	if(! LEX1->Register_ObjType()) {
		WARN("proceeding anyway");
	}

	PLUS_LEX1 = LEX1;
	return PLUS_LEX1;
};

////////////////////////
// LEX2
// LEX2 uses PTR2 -> LEX1


int LEX2_SetFromAnyProc(
 Tcl_Interp *interp,
 Tcl_Obj *obj
)
{
	/*
		to upgrade "Literal" to TYPE LEX2 {"Literal"}

		simply set TYPE
		everything else should already be NULL
	*/
	TCL_PLUS_LEX2 * TYPE_LEX2 = get_TYPE_LEX2();
	if( !TYPE_LEX2 ) {
		return TCL_ERROR; // already reported
	}
	if( !TYPE_LEX2 -> name ) {
		FAIL("TYPE_LEX2 has no name");
		return TCL_ERROR;
	}
	if( !obj -> bytes ) {
		// TCL_FAIL // write FAIL text to TCL val
		FAIL("obj bytes == NULL Literal");
		return TCL_ERROR;
	}

	INFO("UPGRADING LITERAL %s", obj->bytes );

	obj -> typePtr = get_TYPE_LEX2();
	TCL_set_PTR1( obj, NULL );
	TCL_set_PTR2( obj, NULL );
	return TCL_OK;
}



TCL_PLUS_LEX2 * TCL:: get_TYPE_LEX2()
{
	// return the TYPE , cached or new
	// build it and register it
	//

	// CACHED
	static TCL_PLUS_LEX2 * PLUS_LEX2 = NULL;
	if( PLUS_LEX2 ) return PLUS_LEX2;

	// TRACER
	if( 1 )
		INFO("# CALL # new PLUS_LEX2" );

	// NEW
	TCL_PLUS_LEX2 * LEX2 =
		new TCL_PLUS_LEX2(); // _PLUS("LEX2")
	if( !LEX2 ) {
		FAIL("NULL from new PLUS_LEX2" );
		return NULL;
	}

	LEX2 -> bytes_never_NULL = true; // LEX1 is LEX1 -> bytes
	LEX2 -> PTR2_is_Tcl_Obj = true; // PTR2 -> LEX1
	// that is PTR2 of INST

	if(! LEX2->Register_ObjType()) {
		WARN("proceeding anyway");
	}

	PLUS_LEX2 = LEX2;
	return PLUS_LEX2;
}

TCL_PLUS_DICT * TCL:: get_TYPE_DICT()
{
	// return the TYPE , cached or new
	// build it and register it
	//

	// CACHED
	static TCL_PLUS_DICT * PLUS_DICT = NULL;
	if( PLUS_DICT ) return PLUS_DICT;

	// TRACER
	if( 1 )
		INFO("# CALL # new PLUS_DICT" );

	// NEW
	TCL_PLUS_DICT * DICT =
		new TCL_PLUS_DICT(); // _PLUS("DICT")
	if( !DICT ) {
		FAIL("NULL from new PLUS_DICT" );
		return NULL;
	}

	// ATM TYPE_DICT is an OBJ_obj_idx with idx stored in bytes
	DICT -> bytes_never_NULL = true; // LEX1 is LEX1 -> bytes
	DICT -> PTR2_is_Tcl_Obj = true; // PTR2 -> TCL_DICT.dict

	if(! DICT->Register_ObjType()) {
		WARN("proceeding anyway");
	}

	PLUS_DICT = DICT;
	return PLUS_DICT;
}

TCL_PLUS_VECT * TCL:: get_TYPE_VECT()
{
	// return the TYPE , cached or new
	// build it and register it
	//

	// CACHED
	static TCL_PLUS_VECT * PLUS_VECT = NULL;
	if( PLUS_VECT ) return PLUS_VECT;

	// TRACER
	if( 1 )
		INFO("# CALL # new PLUS_VECT" );

	// NEW
	TCL_PLUS_VECT * VECT =
		new TCL_PLUS_VECT(); // _PLUS("VECT")
	if( !VECT ) {
		FAIL("NULL from new PLUS_VECT" );
		return NULL;
	}

	VECT -> bytes_never_NULL = true; // LEX1 is LEX1 -> bytes
	VECT -> PTR2_is_Tcl_Obj = true; // PTR2 -> TCL_VECT . vect // list
	// that is PTR2 of INST

	if(! VECT->Register_ObjType()) {
		WARN("proceeding anyway");
	}

	PLUS_VECT = VECT;
	return PLUS_VECT;
}

////////////////////////

Tcl_Obj * TCL:: mk_LEX1( Tcl_Interp * interp, const char * str )
{
	// called by MATCHER.MATCHES CTOR
	Tcl_Obj * lex1 = mk_common_spelling( interp, str );
	if( lex1 -> typePtr ) {
		WARN("already a TYPE_XXXX %s %s",
		  str_not_NULL(lex1 -> typePtr -> name),
		  str
		);
		print_tcl_obj( lex1, "from mk_common_spelling" );
	}
	TCL_PLUS_BASE * TYPE_LEX1 = get_TYPE_LEX1();
	TYPE_LEX1 -> setFromAnyProc( interp, lex1 );
	print_tcl_obj( lex1, "after set_from_any" );
	return lex1;
}

// Tcl_Obj * mk_LEX2( Tcl_Interp * interp, const char * str );// MAKES NO SENSE
// LEX2 is the forced use of a fixed Tcl_Obj when a LEX1 would be better
// LEX2 comes from NOT using proc, or from other clever construction of obj
// LEX2 holds a PTR2 -> a_LEX1
// LEX2 may even be multiple instances

bool TCL:: upgrade_to_LEX2( Tcl_Obj * obj,  Tcl_Obj * LEX1 ) {
#if 1
	
#endif
	INFO("%s %s", obj->bytes, LEX1->bytes);
	obj -> typePtr = /* AUTO CAST from _plus to PLAIN */ get_TYPE_LEX2();
	TCL_set_PTR1( obj, NULL );
	TCL_set_PTR2( obj, LEX1 );
	Tcl_IncrRefCount( LEX1 );
	return TCL_OK;
}
