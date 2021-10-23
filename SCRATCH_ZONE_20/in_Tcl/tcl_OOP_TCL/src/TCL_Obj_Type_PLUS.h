#ifndef TCL_ObjType_PLUS_H
#define TCL_ObjType_PLUS_H
//	#include "TCL_ObjType_PLUS.h"

#include <tcl.h>
#include "TCL_STUBS.h" // INFO WARN FAIL
#include "TCL_HELP.h" // PTR1 PTR2

struct TCL_ObjType_PLUS : Tcl_ObjType
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

	TCL_ObjType_PLUS( const char * ABB )
//	: Tcl_ObjType()
	{
		set_funcs_NULL();
		TODO("name UDEF_00_UNSET - alloc TCL_TYPE or u8_xFF or ...");
	name = "UDEF_00_unset";
		alias_one_ABB = ABB;
		alias_one_LONG = ABB;
//		alias_two_ABB = NULL;
//		alias_two_LONG = NULL; 
		name = ABB;

		has_DICT_of_KEY_VAL = false; // SP1 no extra KEY_VAL fields
	}

	TCL_ObjType_PLUS()
	: Tcl_ObjType()
	{
		alias_one_ABB = "UNSET";
		alias_one_LONG = "UNSET";
	}

	void set_funcs_NULL()
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


}; // struct

struct TCL_ObjType_SP1 : TCL_ObjType_PLUS
{
	TCL_ObjType_SP1()
	: TCL_ObjType_PLUS("SP1")
	{
		set_funcs_SP1();
	}

	void set_funcs_SP1();

};

// extern TCL_ObjType_SP1 * ObjType_SP1; // = NULL;
// Tcl_ObjType * get_SP1();

extern
TCL_ObjType_SP1 * get_TYPE_SP1(); // does not need interp

#endif
