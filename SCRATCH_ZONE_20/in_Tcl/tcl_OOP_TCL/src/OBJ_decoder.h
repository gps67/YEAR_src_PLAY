#ifndef OBJ_decoder_H
#define OBJ_decoder_H

#include "TCL_REF.h"
#include "TCL_LIST.h"
#include "TCL_DICT.h"
#include "TCL_HASH.h"
#include "TCL_MATCHER.h"
#include "TCL_TYPE.h"

#include "TCL_Obj_Type_PLUS.h" // LEX1


#if 0
class OBJ_MODULE // actually any kind of module
{
 public:
	OBJ_MODULE( Tcl_Interp * interp )
	{
	}
	~OBJ_MODULE()
	{
	}

};
#endif

/*!
	OBJ_decoder holds a TABLE[ obj_id ] of Tcl_Obj *

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
class OBJ_decoder
{
 //	TCL_TYPE type_decoder;
 	KEPT_PTR_type KEPT_PTR; 
	TCL_LIST objs;
//	TCL_DICT dict;
	// the _PLUS types are not directly usable in typePtr !!! because VTBL
 public:
	TCL_ObjType_LEX1 * TYPE_LEX1;
	TCL_ObjType_LEX2 * TYPE_LEX2;
//	TCL_ObjType_PLUS * TYPE_PLUS;
	TCL_ObjType_DICT * TYPE_DICT;
	TCL_ObjType_VECT * TYPE_VECT;
 public:
 	OBJ_decoder( Tcl_Interp * interp )
	: objs( interp )
//	, dict( interp )
//	, hash( interp )
 	, KEPT_PTR( NULL )
	, TYPE_LEX1( NULL )
	, TYPE_LEX2( NULL )
	, TYPE_DICT( NULL )
	, TYPE_VECT( NULL )
	{
		// KEEP the KEPT_PTR
		KEPT_PTR = GET_KEPT_PLUS_PTR_GLOBAL();

		// do INIT this thing, DONT keep it
		// TODO remove
		TYPE_LEX1 = get_TYPE_LEX1(); // build it
		TYPE_LEX2 = get_TYPE_LEX2(); // build it
		TYPE_DICT = get_TYPE_DICT(); // build it
		TYPE_VECT = get_TYPE_VECT(); // build it
		// a TYPE_obj is not a Tcl_Obj just a STRUCT*
	}

 	~OBJ_decoder()
	{
	}

	bool new_OBJ_DICT( Tcl_Interp * interp, Tcl_Obj ** RET_VAL );
	bool new_OBJ_VECT( Tcl_Interp * interp, Tcl_Obj ** RET_VAL );
	bool new_OBJ_type_nam(
		Tcl_Interp * interp,
		Tcl_Obj ** RET_VAL,
		Tcl_ObjType * typ,
		const char * NAME
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
int declare_OBJ_functions( Tcl_Interp * interp, OBJ_decoder * decoder );

///////////////////////////////////////////////

#endif
