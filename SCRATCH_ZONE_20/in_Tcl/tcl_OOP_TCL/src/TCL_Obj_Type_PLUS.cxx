
#include "TCL_Obj_Type_PLUS.h" // error in "_"


int SP1_SetFromAnyProc(
 Tcl_Interp *interp,
 Tcl_Obj *obj
)
{
	TCL_ObjType_SP1 * TYPE_SP1 = get_TYPE_SP1();
	if( !TYPE_SP1 ) {
		return TCL_ERROR;
	}

	obj -> typePtr = get_TYPE_SP1();
	return TCL_ERROR;
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
	freeIntRepProc = NULL;
	dupIntRepProc = NULL;
	updateStringProc = NULL;
	setFromAnyProc = NULL;
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
	if( !ObjType_SP1 ) {
		INFO("NULL from new ObjType_SP1" );
		return NULL;
	}
	// SP1->set_funcs_SP1(); // done in CTOR


	ObjType_SP1 = SP1;
	// registering is optional
	Tcl_RegisterObjType( SP1 );
	return ObjType_SP1;
};

