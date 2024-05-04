#ifndef OBJ_module_H
#define OBJ_module_H

#include "TCL_REF.h"
#include "TCL_LIST.h"
#include "TCL_DICT.h"
#include "TCL_HASH.h"
#include "TCL_MATCHER.h"

// #include "TCL_ObjType_PLUS.h" // LEX1
#include "TCL_PLUS_BASE.h" // remove LEX1 though !?
namespace TCL {

enum VALS_TYPE_TAG { 
	VAL_is_UNS,
	VAL_is_INT,
	VAL_is_FLOAT,	 // f32 or f64 ...
	VAL_is_BITFIELD,
	PTR_is_Tcl_Obj,
	PTR_is_Tcl_Obj_SPEC,	// PTR1_is_SPEC // PTR2_is_
	PTR_is_VTBL_ZERO,	// all VTBL have a basic VTBL
	PTR_is_VTBL_ONE,	// LIBR_one uses VTBL_one : VTBL_zero
	PTR_is_VTBL_TWO,	// LIBR_two uses VTBL_two : VTBL_zero
	PTR_is_C,		// UNKNOWN and OWNED by this reference
	PTR_is_C_plus,		// KNOWN and OWNED and handled by nearby code
};

/*
	Layouts

		STR0 - AKEY of OBJ
		PTR1 - SPEC 
		PTR2 - DICT // which is Tcl_Obj
	
	Layout

		STR0 - AKEY
		PTR1 - SPEC as Tcl_Obj
		PTR2 - 
*/


#if 0
// or put this isn _PLUS
enum OBJ_types
{
	UNSET = 0
	LEX1,
	LEX2,
	VECT,
	DICT,
};
/*
	SCHEME
		TYPE == LEX1
		VAL1 == 0
		PTR2 == 0

		TYPE == LEX2
		VAL1 == 0
		PTR2 => LEX1

		TYPE == DICT || VECT || ...
		VAL1 == u16_u8_u8
		PTR2 == Tcl_Obj  -> list dict

		TYPE == CXX
		VAL1 == u16_u8_u8
		PTR2 == C_VOID *

*/
#endif

/*!
	OBJ_module holds a TABLE[ obj_id ] of Tcl_Obj *

	Each OBJ that gets decoded will be a KEY 
	where PTR1 is obj_id * 256 + obj_op
	where PTR2 is CXX * OBJ

	This is very inefficient for trees of leaves

	This might add u8 _op ==> TYPE_SPEC // gentre

	On 64 bit machines there is a world of opportunity

		u48 u16
	
	But I am still hoping to have a useful system with ARM 32

		u24 u8
	
	Possibly

		u16 u8 u8
	
	That would limit to [64K] of bright objects on ARM32
	but then be consistent to DECODE u8_u8 
*/
class OBJ_module
{
	// a list of ALL the objs created in the system
	// only needed for the BIND command strings to find obj
	// each obj is it's own ID, bytes[length] is there IDX stored
	TCL_LIST objs;
	// the _PLUS types are not directly usable in typePtr !!! because VTBL

	// The idea is that OBJ_module * is a parameter in a CPU register
	// so this is the fastest way to find "KEPT_PTR"
	// ie even faster than loading a 32-bit constant pointer
	// LDA ADDR_of_magic_func
	// LDA 0xFFFF_FFFF // of 64_bit
	// 
	// ld A P->KEPT_PTR_type
	// LDA CPU_VAR + OFFS DEREF // KEPT_PTR is ADDR_of_magic_func
	// LDB obj +OFFS(typePtr) DEREF TESTNULL +OFFS(magic_func_vect) DEREF
	// if(A==B) { KNOW("is_PLUS_type") }
	typedef Tcl_DupInternalRepProc * KEPT_PTR_type;
 	/*
		static 
		 KEPT_PTR_type
		 KEPT_PTR; // init_done_at init_time // = C_FUNC_ADDR

		You must also repeat this in OBJ_modules.o .cxx

	*/
 static
	KEPT_PTR_type KEPT_PTR;

 // OK THE WEAKNESS //
 // KEPT_PTR // is in OBJ_module * clientData; // provided to FUNC OBJ_OBJ
 // PLUS knows more about itself than we do // 
 // SO // CACHE // via nearest local pointer //
 // a sensible thing would make interp a field of _Module


	// static // NOT static so SELF->KEPT_PTR is available
	//
	// NOT static so SELF->KEPT_PTR is available
	//
	bool is_PLUS_type( const Tcl_Obj * obj )
	{
		// NEED // KNOW // obj is not NULL
		return is_PLUS_type( obj->typePtr );
		// if(!obj->typePtr) return false;
		// return  obj->typePtr -> dupIntRepProc == KEPT_PTR;
	}

	bool is_PLUS_type( const Tcl_ObjType * typePtr )
	{
		if(!typePtr) return false;
		return  typePtr -> dupIntRepProc == KEPT_PTR;
	}

	bool get_PLUS_type( const Tcl_Obj * obj, TCL_PLUS_BASE *& PLUS )
	{
		return TCL_PLUS_BASE:: get_PLUS_type( obj, PLUS );
//		if( !is_PLUS_type( obj )) return false;
//		// see _PLUS.h
//		PLUS = get_PLUS_from_typePtr( obj->typePtr );
//		return true;
	}

	TCL_PLUS_BASE * get_PLUS_type( const Tcl_Obj * obj )
	{
		if( !is_PLUS_type( obj )) return NULL;
		// see _PLUS.h
		return (TCL_PLUS_BASE*) get_PLUS_from_typePtr( obj->typePtr );
	}

 public:
	TCL_PLUS_LEX1 * TYPE_LEX1;
	TCL_PLUS_LEX2 * TYPE_LEX2;
//	TCL_PLUS_PLUS * TYPE_PLUS;
	TCL_PLUS_DICT * TYPE_DICT;
	TCL_PLUS_VECT * TYPE_VECT;
 public:
 	OBJ_module( Tcl_Interp * interp );

 	~OBJ_module()
	{
	}

	bool new_OBJ_DICT( Tcl_Interp * interp, Tcl_Obj ** RET_VAL );
	bool new_OBJ_VECT( Tcl_Interp * interp, Tcl_Obj ** RET_VAL );
	bool new_OBJ_type(
		Tcl_Interp * interp,
		Tcl_Obj ** RET_VAL,
		TCL_PLUS_BASE * TYPE_PLUS
	);

	bool test( Tcl_Interp * interp )
	{
//		return hash.test(interp);
//		return dict.test(interp);
		return objs.test(interp);
	}


 //	// future inline // currently fn
 //	bool is_one_of_my_types( Tcl_Obj * obj ) {
 //		return type_decoder.is_one_of_my_types( obj );
 //	}

};

extern
int declare_OBJ_functions( Tcl_Interp * interp, OBJ_module * decoder );

///////////////////////////////////////////////

}; // namespace
#endif
