
#include "TCL_Obj_Type_PLUS.h" // error in "_"

void LEX1_FreeInternalRepProc( 
  Tcl_Obj *obj
) {
	/*
		A SYSTEM EXIT is happening
		the Literal is being deleted
	*/
	if( obj->bytes ) {
		WARN("DELETE LEX1 '%s'", obj->bytes );
	} else {
		WARN("DELETE LEX1 '(NULL)'" );
	}
	TCL_set_PTR1( obj, NULL );
	TCL_set_PTR2( obj, NULL );
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



void LEX1_UpdateStringProc(
  Tcl_Obj *obj
) {
	WARN("LEX1 This should never be called");
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
		FAIL("TYPE_LEX1 has no name");
		return TCL_ERROR;
	}
	if( !obj -> bytes ) {
		// TCL_FAIL // write FAIL text to TCL val
		FAIL("obj bytes == NULL Literal");
		return TCL_ERROR;
	}

	INFO("UPGRADING LITERAL %s", obj->bytes );

	obj -> typePtr = get_TYPE_LEX1();
	TCL_set_PTR1( obj, NULL );
	TCL_set_PTR2( obj, NULL );
	return TCL_OK;
}



void TCL_ObjType_LEX1:: set_funcs_LEX1()
{
 #if 0
	Tcl_FreeInternalRepProc *freeIntRepProc;
	Tcl_DupInternalRepProc *dupIntRepProc;
	Tcl_UpdateStringProc *updateStringProc;
	Tcl_SetFromAnyProc *setFromAnyProc;
 #endif
	freeIntRepProc = LEX1_FreeInternalRepProc;
	dupIntRepProc = LEX1_DupInternalRepProc;
	updateStringProc = LEX1_UpdateStringProc;
	setFromAnyProc = LEX1_SetFromAnyProc;
	INFO("CALLED");
}

TCL_ObjType_LEX1 * get_TYPE_LEX1()
{
	// return the TYPE , cached or new
	// build it and register it
	//
	static TCL_ObjType_LEX1 * ObjType_LEX1 = NULL;
	if( ObjType_LEX1 ) return ObjType_LEX1;
	if( 1 )
		INFO("# CALL # new ObjType_LEX1" );
	INFO("building LEX1");

	// I want to call the objects of type LEX1_t LEX1
	// using a temp LEX1 so that it is fully init'd before not NULL
	// 
	TCL_ObjType_LEX1 * LEX1 =
		new TCL_ObjType_LEX1(); // _PLUS("LEX1")
	if( !LEX1 ) {
		FAIL("NULL from new ObjType_LEX1" );
		return NULL;
	}
	// LEX1->set_funcs_LEX1(); // done in CTOR


	ObjType_LEX1 = LEX1;
	// registering is optional
	PASS("Registering %s", LEX1->name );
	Tcl_RegisterObjType( LEX1 );
	return ObjType_LEX1;
};

////////////////////////



void LEX2_FreeInternalRepProc( 
  Tcl_Obj *obj
) {
	/*
		A SYSTEM EXIT is happening
		the Literal is being deleted
	*/
	if( obj->bytes ) {
		WARN("DELETE LEX2 '%s'", obj->bytes );
	} else {
		WARN("DELETE LEX2 '(NULL)'" );
	}
	TCL_set_PTR1( obj, NULL );
	TCL_set_PTR2( obj, NULL );
}


void LEX2_DupInternalRepProc(
  Tcl_Obj *srcPtr,
  Tcl_Obj *dupPtr
){
	FAIL("LEX2 THIS IS BEYOND ME ATM");
	FAIL("LEX2 WHY WAS THIS CALLED");

	// WHY copy from src to dst
	// MAYBE LIST COPY then change ??

	TCL_set_PTR1( dupPtr, NULL );
	TCL_set_PTR2( dupPtr, NULL );
}



void LEX2_UpdateStringProc(
  Tcl_Obj *obj
) {
	WARN("LEX2 This should never be called");
	if( !obj -> bytes ) {
		// TCL_FAIL // write FAIL text to TCL val
		FAIL("obj bytes == NULL Literal");
		return;
//		return TCL_ERROR;
	}
};


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



void TCL_ObjType_LEX2:: set_funcs_LEX2()
{
 #if 0
	Tcl_FreeInternalRepProc *freeIntRepProc;
	Tcl_DupInternalRepProc *dupIntRepProc;
	Tcl_UpdateStringProc *updateStringProc;
	Tcl_SetFromAnyProc *setFromAnyProc;
 #endif
	freeIntRepProc = LEX2_FreeInternalRepProc;
	dupIntRepProc = LEX2_DupInternalRepProc;
	updateStringProc = LEX2_UpdateStringProc;
	setFromAnyProc = LEX2_SetFromAnyProc;
	INFO("CALLED");
}

TCL_ObjType_LEX2 * get_TYPE_LEX2()
{
	// return the TYPE , cached or new
	// build it and register it
	//
	static TCL_ObjType_LEX2 * ObjType_LEX2 = NULL;
	if( ObjType_LEX2 ) return ObjType_LEX2;
	if( 1 )
		INFO("# CALL # new ObjType_LEX2" );
	INFO("building LEX2");

	// I want to call the objects of type LEX2_t LEX2
	// using a temp LEX2 so that it is fully init'd before not NULL
	// 
	TCL_ObjType_LEX2 * LEX2 =
		new TCL_ObjType_LEX2(); // _PLUS("LEX2")
	if( !LEX2 ) {
		FAIL("NULL from new ObjType_LEX2" );
		return NULL;
	}
	// LEX2->set_funcs_LEX2(); // done in CTOR


	ObjType_LEX2 = LEX2;
	// registering is optional
	PASS("Registering %s", LEX2->name );
	Tcl_RegisterObjType( LEX2 );
	return ObjType_LEX2;
};

////////////////////////


