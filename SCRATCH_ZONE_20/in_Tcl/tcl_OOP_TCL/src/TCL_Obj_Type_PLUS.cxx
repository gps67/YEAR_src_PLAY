
#include "TCL_Obj_Type_PLUS.h" // error in "_"
#include "TCL_HELP.h" // P64
#include "dgb.h" // FAIL

/*
	this DUP func is used as a KEEP_PTR 
	all _PLUS types do this
	only _PLUS types do this
	so we KNOW that 
	srcPtr -> typePtr == A_PLUS_TYPE # subtype within group
	so bounce through to find and call the correct function

	this is very rarely called, so lacks testing
*/
void PLUS_MYTYPE_DupInternalRepProc( Tcl_Obj *srcPtr, Tcl_Obj *dupPtr )
{
	#if 1
	// KEEP_PTR == this function, get it to check TYPE -> DUP func
	Tcl_DupInternalRepProc *dupIntRepProc
	= PLUS_MYTYPE_DupInternalRepProc; // this_func so fast

	// some checks // supposedly guaranteed
	if(!srcPtr) { FAIL("NULL src in DUP"); return; }
	if(!srcPtr->typePtr) { FAIL("NULL src_type in DUP"); return; }
	if(!(srcPtr->typePtr->dupIntRepProc != dupIntRepProc)) {
		FAIL("NULL src_type in DUP");
		return;
	}
	#endif

	// so NOW we know that this is a _PLUS typePtr
	// we DID know that because that is the only way this is called
	// but belt and braces

	// cast from Tcl_TypeObj * to PLUS
	// because of VTBL requires FN to add to PTR*
	TCL_ObjType_PLUS * PLUS =
	get_PLUS_from_typePtr( srcPtr->typePtr );
//	(TCL_ObjType_PLUS*) srcPtr->typePtr;
	if(!PLUS) {
		FAIL("NULL typePtr after cast");
		return;
	}
	// find and call the real dup function
	// PTR stored in the PLUS struct
	Tcl_DupInternalRepProc * plus_dup = PLUS-> dupIntRepProc_PLUS;
	if(!plus_dup) {
		FAIL("NULL dupIntRepProc_PLUS");
		return;
	}
	plus_dup( srcPtr, dupPtr );
}

bool TCL_ObjType_PLUS:: check_funcs_not_NULL()
{
	bool OK = true;

	// check they are not NULL
	if(! freeIntRepProc ) {
		OK = FAIL("NULL freeIntRepProc");
	}
	if(! dupIntRepProc ) {
		// maybe we should set it here ?
		// or that is all done by the CTOR and adjusts
		OK = FAIL("NULL dupIntRepProc");
	}
	if(! dupIntRepProc_PLUS ) {
		OK = FAIL("NULL dupIntRepProc_PLUS");
	}
	if(! updateStringProc ) {
		OK = FAIL("NULL updateStringProc");
	}
	if(! setFromAnyProc ) {
		OK = FAIL("NULL setFromAnyProc");
	}

	// check DUP is the specific rerouted one
	// DUP is the one that is easily forgotten // because _PLUS
	if( dupIntRepProc != PLUS_MYTYPE_DupInternalRepProc) {
		OK = FAIL("dupIntRepProc != PLUS_MYTYPE_DupInternalRepProc");
	}

	if(!OK) {
		FAIL("NULL function in TCL_ObjType");
	}

	return OK;
}

bool TCL_ObjType_PLUS:: Register_ObjType()
{

	set_funcs_BASE(); // sets some common ones
	set_funcs(); // sets the type specific ones
	if(! check_funcs_not_NULL() ) {
		FAIL("NULL funcs CODE error, registering type anyway");
	}

	// registering is optional
	PASS("Registering %s", name );
	Tcl_RegisterObjType( this );
	return true;
}
	
/////////////////////////////////////////////////////////////////////////

void PLUS_FreeInternalRepProc( 
  Tcl_Obj *obj
) {
	/*
		GENERIC for all _PLUS type instances
	*/
	TCL_ObjType_PLUS * PLUS
	= get_PLUS_from_obj( obj );

	if(1)
		WARN("DELETE %s %s '%s'",
			P64( obj ),
			obj->typePtr->name, 
			str_not_NULL(obj->bytes)
		);
	if( PLUS->PTR2_is_Tcl_Obj ) {
		TCL_set_PTR2_decr_NULL( obj );
	} else {
//	if( PLUS->PTR2_is_CXX_obj ) { call openssl_delete_x509 via fn }
//	PLUS->PTR2_clear_value( obj ) // VTBL
//	if( PTR2 )
//	ditto for VAL1
	}
//	TCL_set_PTR1( obj, NULL ); // expect it always was
//	TCL_set_PTR2( obj, NULL ); // expect it always was
}

void LEX1_DupInternalRepProc(
  Tcl_Obj *srcPtr,
  Tcl_Obj *dupPtr
){
	FAIL("LEX1 THIS IS BEYOND ME ATM");
	FAIL("LEX1 WHY WAS THIS CALLED");

	// WHY copy from src to dst
	// MAYBE LIST COPY then change ??

	TCL_set_PTR1( dupPtr, NULL );
	TCL_set_PTR2( dupPtr, NULL );
}



void PLUS_UpdateStringProc(
  Tcl_Obj *obj
) {
	TCL_ObjType_PLUS * PLUS
	= get_PLUS_from_obj( obj );

	WARN("PLUS This should never be called");
	if( !obj -> bytes ) {
		// TCL_FAIL // write FAIL text to TCL val
		FAIL("obj bytes == NULL Literal");
		return;
//		return TCL_ERROR;
	}
};


int LEX1_SetFromAnyProc(
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
	TCL_ObjType_LEX1 * TYPE_LEX1 = get_TYPE_LEX1();
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

TCL_ObjType_LEX1 * get_TYPE_LEX1()
{
	// build the TYPE, or cached
	// build it and register it
	//
	// cache var //
	static TCL_ObjType_LEX1 * ObjType_LEX1 = NULL;
	if( ObjType_LEX1 ) return ObjType_LEX1;

	if( 1 )
	INFO("building TYPE_LEX1");

	// I want to call the objects of type LEX1_t LEX1
	// using a temp LEX1 so that it is fully init'd before not NULL
	// 
	TCL_ObjType_LEX1 * LEX1
	= new TCL_ObjType_LEX1(); // _PLUS("LEX1")
	if( !LEX1 ) {
		FAIL("NULL from new ObjType_LEX1" );
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

	ObjType_LEX1 = LEX1;
	return ObjType_LEX1;
};

////////////////////////
// LEX2
// LEX2 uses PTR2 -> LEX1

void LEX2_DupInternalRepProc(
  Tcl_Obj *srcPtr,
  Tcl_Obj *dupPtr
){
	FAIL("LEX2 THIS IS BEYOND ME ATM");
	FAIL("LEX2 WHY WAS THIS CALLED");
	FAIL("LEX2 copy over PTR2 and Incr");

	// WHY copy from src to dst
	// MAYBE LIST COPY then change ??

	TCL_set_PTR1( dupPtr, NULL );
	TCL_set_PTR2( dupPtr, NULL ); // set it to src_ptr PTR2 ref_incr
}


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
	TCL_ObjType_LEX2 * TYPE_LEX2 = get_TYPE_LEX2();
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



TCL_ObjType_LEX2 * get_TYPE_LEX2()
{
	// return the TYPE , cached or new
	// build it and register it
	//

	// CACHED
	static TCL_ObjType_LEX2 * ObjType_LEX2 = NULL;
	if( ObjType_LEX2 ) return ObjType_LEX2;

	// TRACER
	if( 1 )
		INFO("# CALL # new ObjType_LEX2" );

	// NEW
	TCL_ObjType_LEX2 * LEX2 =
		new TCL_ObjType_LEX2(); // _PLUS("LEX2")
	if( !LEX2 ) {
		FAIL("NULL from new ObjType_LEX2" );
		return NULL;
	}

	LEX2 -> bytes_never_NULL = true; // LEX1 is LEX1 -> bytes
	LEX2 -> PTR2_is_Tcl_Obj = true; // PTR2 -> LEX1
	// that is PTR2 of INST

//	Register calls set_funcs()
//	LEX2 -> set_funcs(); // VTL not within CTOR

	if(! LEX2->Register_ObjType()) {
		WARN("proceeding anyway");
	}

	ObjType_LEX2 = LEX2;
	return ObjType_LEX2;
}

TCL_ObjType_DICT * get_TYPE_DICT()
{
	// return the TYPE , cached or new
	// build it and register it
	//

	// CACHED
	static TCL_ObjType_DICT * ObjType_DICT = NULL;
	if( ObjType_DICT ) return ObjType_DICT;

	// TRACER
	if( 1 )
		INFO("# CALL # new ObjType_DICT" );

	// NEW
	TCL_ObjType_DICT * DICT =
		new TCL_ObjType_DICT(); // _PLUS("DICT")
	if( !DICT ) {
		FAIL("NULL from new ObjType_DICT" );
		return NULL;
	}

	// ATM TYPE_DICT is an OBJ_obj_idx with idx stored in bytes
	DICT -> bytes_never_NULL = true; // LEX1 is LEX1 -> bytes
	DICT -> PTR2_is_Tcl_Obj = true; // PTR2 -> TCL_DICT.dict

//	Register calls set_funcs()

	if(! DICT->Register_ObjType()) {
		WARN("proceeding anyway");
	}

	ObjType_DICT = DICT;
	return ObjType_DICT;
}

TCL_ObjType_VECT * get_TYPE_VECT()
{
	// return the TYPE , cached or new
	// build it and register it
	//

	// CACHED
	static TCL_ObjType_VECT * ObjType_VECT = NULL;
	if( ObjType_VECT ) return ObjType_VECT;

	// TRACER
	if( 1 )
		INFO("# CALL # new ObjType_VECT" );

	// NEW
	TCL_ObjType_VECT * VECT =
		new TCL_ObjType_VECT(); // _PLUS("VECT")
	if( !VECT ) {
		FAIL("NULL from new ObjType_VECT" );
		return NULL;
	}

	VECT -> bytes_never_NULL = true; // LEX1 is LEX1 -> bytes
	VECT -> PTR2_is_Tcl_Obj = true; // PTR2 -> TCL_VECT . vect // list
	// that is PTR2 of INST

//	Register calls set_funcs()

	if(! VECT->Register_ObjType()) {
		WARN("proceeding anyway");
	}

	ObjType_VECT = VECT;
	return ObjType_VECT;
}

////////////////////////

Tcl_Obj * mk_LEX1( Tcl_Interp * interp, const char * str )
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
	TCL_ObjType_PLUS * TYPE_LEX1 = get_TYPE_LEX1();
	TYPE_LEX1 -> setFromAnyProc( interp, lex1 );
	print_tcl_obj( lex1, "after set_from_any" );
	return lex1;
}

// Tcl_Obj * mk_LEX2( Tcl_Interp * interp, const char * str );// MAKES NO SENSE
// LEX2 is the forced use of a fixed Tcl_Obj when a LEX1 would be better
// LEX2 comes from NOT using proc, or from other clever construction of obj
// LEX2 holds a PTR2 -> a_LEX1
// LEX2 may even be multiple instances

bool upgrade_to_LEX2( Tcl_Obj * obj,  Tcl_Obj * LEX1 ) {
#if 1
	
#endif
	INFO("%s %s", obj->bytes, LEX1->bytes);
	obj -> typePtr = /* AUTO CAST from _plus to PLAIN */ get_TYPE_LEX2();
	TCL_set_PTR1( obj, NULL );
	TCL_set_PTR2( obj, LEX1 );
	Tcl_IncrRefCount( LEX1 );
	return TCL_OK;
}

// edit these 

void TCL_ObjType_PLUS:: set_funcs_BASE()
{
	freeIntRepProc     = PLUS_FreeInternalRepProc;
	dupIntRepProc      = PLUS_MYTYPE_DupInternalRepProc; // CALLS ...
	dupIntRepProc_PLUS = LEX2_DupInternalRepProc;
	updateStringProc   = PLUS_UpdateStringProc;
	setFromAnyProc     = LEX2_SetFromAnyProc;
	INFO("CALLED");
}

// virtual // presume _NULL reviously called
void TCL_ObjType_PLUS:: set_funcs() {
	WARN("OVERRIDE EXPECTED");
}

void TCL_ObjType_LEX1:: set_funcs() {
//	freeIntRepProc     = LEX1_FreeInternalRepProc;
//	dupIntRepProc      = PLUS_MYTYPE_DupInternalRepProc;
	dupIntRepProc_PLUS = LEX1_DupInternalRepProc;
//	updateStringProc   = LEX1_UpdateStringProc;
	setFromAnyProc     = LEX1_SetFromAnyProc;
	INFO("CALLED");
}

void TCL_ObjType_LEX2:: set_funcs() {
	//
//	freeIntRepProc     = LEX2_FreeInternalRepProc;
//	dupIntRepProc      = PLUS_MYTYPE_DupInternalRepProc; // CALLS ...
	dupIntRepProc_PLUS = LEX2_DupInternalRepProc;
//	updateStringProc   = LEX2_UpdateStringProc;
	setFromAnyProc     = LEX2_SetFromAnyProc;
	INFO("CALLED");
}

void TCL_ObjType_VECT:: set_funcs()
{
//	freeIntRepProc     = LEX2_FreeInternalRepProc;
//	dupIntRepProc      = PLUS_MYTYPE_DupInternalRepProc; // CALLS ...
	dupIntRepProc_PLUS = LEX2_DupInternalRepProc;
//	updateStringProc   = LEX2_UpdateStringProc;
	setFromAnyProc     = LEX2_SetFromAnyProc;
	INFO("CALLED");
}

void TCL_ObjType_DICT:: set_funcs()
{
//	freeIntRepProc     = LEX2_FreeInternalRepProc;
//	dupIntRepProc      = PLUS_MYTYPE_DupInternalRepProc; // CALLS ...
	dupIntRepProc_PLUS = LEX2_DupInternalRepProc;
//	updateStringProc   = LEX2_UpdateStringProc;
	setFromAnyProc     = LEX2_SetFromAnyProc;
	INFO("CALLED");
}


