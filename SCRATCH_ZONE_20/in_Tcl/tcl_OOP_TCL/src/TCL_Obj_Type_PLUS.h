#ifndef TCL_Obj_Type_PLUS_H
#define TCL_Obj_Type_PLUS_H
//	#include "TCL_Obj_Type_PLUS.h"

#include <tcl.h>

struct TCL_Obj_Type_PLUS : Tcl_ObjType
{
 // pretty printing require CTOR to set up good defaults

	const char * alias_one_ABB; // when not UDEF_25 UDEF_SP1 or "SP1"
	// "SP1"

	const char * alias_one_LONG;
	// "UDEF_25_SP1" // SP1 is also an STR4 // with NUL

//	const char * alias_two_ABB;
	// "SPELLING_ONE"

//	const char * alias_two_LONG; // when not UDEF_25 UDEF_SP1 or "SP1"
	// { SP1_t "SP1" }

	// when not UDEF_25 UDEF_SP1 or "SP1"

	bool has_DICT_of_KEY_VAL; // ie user added fields { KEY VAL }

	TCL_Obj_Type_PLUS( const char * ABB )
//	: Tcl_ObjType()
	{
		alias_one_ABB = ABB;
		alias_one_LONG = ABB;
//		alias_two_ABB = NULL;
//		alias_two_LONG = NULL; 

		has_DICT_of_KEY_VAL = false; // SP1 no extra KEY_VAL fields
	}

	TCL_Obj_Type_PLUS()
	: Tcl_ObjType()
	{
		alias_one_ABB = "UNSET";
		alias_one_LONG = "UNSET";
	}

}; // struct

struct TCL_Obj_Type_SP1 : TCL_Obj_Type_PLUS
{
	TCL_Obj_Type_SP1()
	: TCL_Obj_Type_PLUS("SP1")
	{
	}
};

#endif
