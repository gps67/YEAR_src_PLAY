#ifndef OBJ_decoder_H
#define OBJ_decoder_H

// #include <tcl.h> // done by TCL_PTR_t.h
#include "TCL_PTR_t.h"
#include "TCL_LIST_t.h"


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
	TCL_LIST_t list;
 public:
 	OBJ_decoder( Tcl_Interp * interp )
	:list( interp )
	{
	}
 	~OBJ_decoder()
	{
	}
	bool test( Tcl_Interp * interp )
	{
		return list.test(interp);
	}

};

extern
int declare_OBJ_functions( Tcl_Interp * interp, OBJ_decoder * decoder );

#endif
