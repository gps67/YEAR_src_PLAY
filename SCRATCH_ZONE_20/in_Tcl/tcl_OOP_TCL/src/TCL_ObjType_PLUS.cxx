
#include "TCL_ObjType_PLUS.h" // error in "_"
#include "TCL_PLUS_BASE.h" // error in "_"
#include "TCL_HELP.h" // P64
#include "dgb.h" // FAIL

using namespace TCL ; // lots of C funcs - each needs TCL:: namespace

/*
	this is EXPORTED (others are static to file)
	this DUP func is used as a KEEP_PTR 
	all _PLUS types do this
	only _PLUS types do this
	so we KNOW that 
	src -> typePtr == A_PLUS_TYPE # subtype within group
	so bounce through to find and call the correct function

	this is very rarely called, so lacks testing
*/
void TCL:: PLUS_MYTYPE_DupInternalRepProc( Tcl_Obj *src, Tcl_Obj *dst )
{
	#if 1
	// we KNOW this is a PLUS type because this was called
	// KEEP_PTR == this function, get it to check TYPE -> DUP func
	Tcl_DupInternalRepProc *dupIntRepProc
	= PLUS_MYTYPE_DupInternalRepProc; // this_func so fast

	// some checks // supposedly guaranteed
	if(!src) { FAIL("NULL src in DUP"); return; }
	if(!src->typePtr) { FAIL("NULL src_type in DUP"); return; }
	if(!(src->typePtr->dupIntRepProc != dupIntRepProc)) {
		FAIL("NULL src_type in DUP");
		return;
	}
	#endif

	#if 1
	if(!dst) {
		FAIL("dst is NULL");
		return;
	}
	if(1)
		WARN("DUP SRC %s %s '%s'",
			P64( src ),
			src->typePtr->name, 
			str_not_NULL(src->bytes)
		);
	if(1)
		WARN("DUP DST %s %s '%s'",
			P64( dst ),
			dst->typePtr->name, 
			str_not_NULL(dst->bytes)
		);
	#endif

	// first we NULL out VAL1 and PTR2 
	TCL_set_PTR1( dst, NULL );
	TCL_set_PTR2( dst, NULL );
	// bytes[length] should already be NULL
	// but this is the INTERNAL REP func (Tcl is bad here, so bad)
	// maybe callee shoudl do this be we do
	dst -> typePtr = src -> typePtr;

	// so NOW we know that this is a _PLUS typePtr
	// we DID know that because that is the only way this is called
	// but belt and braces

	// cast from Tcl_TypeObj * to PLUS
	// because of VTBL requires FN to add to PTR*
	// we KNOW this is a PLUS type because this was called
	TCL_ObjType_PLUS * PLUS =
	get_PLUS_from_obj_RAW( src );
	#if 1
	if(!PLUS) {
		FAIL("NULL typePtr after cast, but we KNOW it is a PLUS type");
		return;
	}
	#endif

	if( !PLUS -> VTBL_DupInternalRepProc( src, dst )) {
		FAIL_FAILED();
	}
}

bool TCL_ObjType_PLUS::
VTBL_DupInternalRepProc(
	Tcl_Obj *src,
	Tcl_Obj *dst
)
{
	INFO("CALLED - which is unusual");
	WARN("CALLED - which is unusual");
	return true;
}


void PLUS_UpdateStringProc(
  Tcl_Obj *obj
) {
	TCL_ObjType_PLUS * PLUS
	= get_PLUS_from_obj_RAW( obj );

	if( !PLUS -> VTBL_UpdateStringProc( obj )) {
		FAIL_FAILED();
	}
	if( !obj -> bytes ) {
		FAIL("obj bytes == NULL Literal");
		// so maybe set it to something?
	}
}

bool TCL_ObjType_PLUS::
VTBL_UpdateStringProc( Tcl_Obj * obj )
{
	WARN("PLUS This should never be called");
	WARN("leaving bytes NULL");
	// when OBJ is in BIND as STRING // use KEY in TABLE
	return false;
};

int PLUS_SetFromAnyProc(
 Tcl_Interp *interp,
 Tcl_Obj *obj
)
{
	/*
		called with obj->typePtr == NULL
		should not call _RAW
		because we dont KNOW that obj really is a _PLUS type

		actually here we probably know it ISNT
		as we are converting a non-type to a PLUS type

		maybe, this Tcl_Obj TYPE system is naff beyond belief
		It does not achieve much
		and it is confusing whilst not doing it

	*/
	/*
		PLUS_SetFromAnyProc is set into a typePtr somewhere
		WE are bouncing the C_fn to a VTBL_CXX_fn
		but we dont know what that VTBL_CXX_class is !!
		because having picked this function, obj is plain string
	*/
	if(!obj) {
		FAIL("NULL obj");
		return TCL_ERROR;
	}
	if(!obj->typePtr) {
		FAIL("NULL obj->typePtr");
		gdb_break_point();
		return TCL_ERROR;
	}
	gdb_invoke(false);
	TCL_ObjType_PLUS * PLUS
	= get_PLUS_from_obj_RAW( obj );
	INFO( "PLUS %p obj %p", PLUS, obj );

	if( !PLUS -> VTBL_SetFromAnyProc( interp, obj )) {
		FAIL_FAILED();
		return TCL_ERROR;
	}
	return TCL_OK;
}

bool TCL_ObjType_PLUS::
VTBL_SetFromAnyProc(
 Tcl_Interp *interp,
 Tcl_Obj *obj
)
{
	return FAIL("UNWRITTEN");
}

// HMMM // now it is refusing because this code is within TCL:: { here }

#if 0
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
#endif

bool TCL_ObjType_PLUS:: Register_ObjType()
{
 //	set_funcs_BASE(); // ctor did this, do it again ?
 //	check_funcs_not_NULL(); // test and forget

	// registering is optional
	PASS("Registering %s", name );
	Tcl_RegisterObjType( plain() );
	// Tcl_RegisterObjType( this );
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
	= get_PLUS_from_obj_RAW( obj );

	#if 1
	if( obj -> bytes ) {
		WARN("( obj -> bytes ) - mis thought it would be NULL"); 
		// tcl actually deletes bytes BEFORE calling
	}
	#endif

	if(! PLUS -> VTBL_FreeInternalRepProc( obj ))
	{
		FAIL_FAILED();
		return;
	}

	#if 1
	if( obj -> bytes ) {
		WARN("( obj -> bytes ) - mis thought it would be NULL"); 
		// tcl actually deletes bytes BEFORE calling
	}
	if( TCL_get_PTR1( obj )) {
		WARN("( obj -> PTR1 ) - should be NULL by now"); 
	}
	if( TCL_get_PTR2( obj )) {
		WARN("( obj -> PTR2 ) - should be NULL by now"); 
	}
	if( obj -> typePtr ) {
		WARN("( obj -> typePtr ) - mis thought it would be NULL"); 
	}
	#endif

	// that should have set PTR2 to NULL
	// probably VAL1 even if unused

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

bool TCL_ObjType_PLUS::
VTBL_FreeInternalRepProc( 
  Tcl_Obj *obj
) {
	return FAIL("unwritten - all subtype have SOME free to do");
}


// ########################################

void TCL_ObjType_PLUS:: set_funcs_BASE()
{
	freeIntRepProc     = PLUS_FreeInternalRepProc;
	dupIntRepProc      = PLUS_MYTYPE_DupInternalRepProc; // CALLS ...
	updateStringProc   = PLUS_UpdateStringProc;
	setFromAnyProc     = PLUS_SetFromAnyProc;
	INFO("CALLED for class %s", name );
}


