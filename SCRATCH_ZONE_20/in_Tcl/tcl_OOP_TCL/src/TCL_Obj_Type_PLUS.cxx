
#include "TCL_Obj_Type_PLUS.h" // error in "_"

void SP1_FreeInternalRepProc( 
  Tcl_Obj *obj
) {
	/*
		A SYSTEM EXIT is happening
		the Literal is being deleted
	*/
	if( obj->bytes ) {
		WARN("DELETE SP1 '%s'", obj->bytes );
	} else {
		WARN("DELETE SP1 '(NULL)'" );
	}
	TCL_set_PTR1( obj, NULL );
	TCL_set_PTR2( obj, NULL );
}


void SP1_DupInternalRepProc(
  Tcl_Obj *srcPtr,
  Tcl_Obj *dupPtr
){
	FAIL("SP1 THIS IS BEYOND ME ATM");
	FAIL("SP1 WHY WAS THIS CALLED");

	// WHY copy from src to dst
	// MAYBE LIST COPY then change ??

	TCL_set_PTR1( dupPtr, NULL );
	TCL_set_PTR2( dupPtr, NULL );
}



void SP1_UpdateStringProc(
  Tcl_Obj *obj
) {
	WARN("SP1 This should never be called");
	if( !obj -> bytes ) {
		// TCL_FAIL // write FAIL text to TCL val
		FAIL("obj bytes == NULL Literal");
		return;
//		return TCL_ERROR;
	}
};


int SP1_SetFromAnyProc(
 Tcl_Interp *interp,
 Tcl_Obj *obj
)
{
	/*
		to upgrade "Literal" to TYPE SP1 {"Literal"}

		simply set TYPE
		everything else should already be NULL
	*/
	TCL_ObjType_SP1 * TYPE_SP1 = get_TYPE_SP1();
	if( !TYPE_SP1 ) {
		return TCL_ERROR; // already reported
	}
	if( !TYPE_SP1 -> name ) {
		FAIL("TYPE_SP1 has no name");
		return TCL_ERROR;
	}
	if( !obj -> bytes ) {
		// TCL_FAIL // write FAIL text to TCL val
		FAIL("obj bytes == NULL Literal");
		return TCL_ERROR;
	}

	INFO("UPGRADING LITERAL %s", obj->bytes );

	obj -> typePtr = get_TYPE_SP1();
	TCL_set_PTR1( obj, NULL );
	TCL_set_PTR2( obj, NULL );
	return TCL_OK;
}



void TCL_ObjType_SP1:: set_funcs_SP1()
{
 #if 0
	Tcl_FreeInternalRepProc *freeIntRepProc;
	Tcl_DupInternalRepProc *dupIntRepProc;
	Tcl_UpdateStringProc *updateStringProc;
	Tcl_SetFromAnyProc *setFromAnyProc;
 #endif
	freeIntRepProc = SP1_FreeInternalRepProc;
	dupIntRepProc = SP1_DupInternalRepProc;
	updateStringProc = SP1_UpdateStringProc;
	setFromAnyProc = SP1_SetFromAnyProc;
	INFO("CALLED");
}

TCL_ObjType_SP1 * get_TYPE_SP1()
{
	//
	static TCL_ObjType_SP1 * ObjType_SP1 = NULL;
	if( ObjType_SP1 ) return ObjType_SP1;
	if( 1 )
		INFO("# CALL # new ObjType_SP1" );
	INFO("building SP1");

	// I want to call the objects of type SP1_t SP1
	// using a temp SP1 so that it is fully init'd before not NULL
	// 
	TCL_ObjType_SP1 * SP1 =
		new TCL_ObjType_SP1(); // _PLUS("SP1")
	if( !SP1 ) {
		FAIL("NULL from new ObjType_SP1" );
		return NULL;
	}
	// SP1->set_funcs_SP1(); // done in CTOR


	ObjType_SP1 = SP1;
	// registering is optional
	PASS("Registering %s", SP1->name );
	Tcl_RegisterObjType( SP1 );
	return ObjType_SP1;
};

