#ifndef TCL_ObjType_PLUS_H
#define TCL_ObjType_PLUS_H
//	#include "TCL_ObjType_PLUS.h"

#include <tcl.h>
#include "TCL_STUBS.h" // INFO WARN FAIL
#include "TCL_HELP.h" // PTR1 PTR2

struct TCL_ObjType_PLUS : Tcl_ObjType
{
 // pretty printing require CTOR to set up good defaults

	const char * alias_one_ABB; // when not UDEF_25 UDEF_LEX1 or "LEX1"
	// "LEX1"

	const char * alias_one_LONG;
	// "UDEF_25_LEX1" // LEX1 is also an STR4 // with NUL

//	const char * alias_two_ABB;
	// "SPELLING_ONE"

//	const char * alias_two_LONG; // when not UDEF_25 UDEF_LEX1 or "LEX1"
	// { LEX1_t "LEX1" }

	// when not UDEF_25 UDEF_LEX1 or "LEX1"

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

		has_DICT_of_KEY_VAL = false; // LEX1 no extra KEY_VAL fields
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

struct TCL_ObjType_LEX1 : TCL_ObjType_PLUS
{
	TCL_ObjType_LEX1()
	: TCL_ObjType_PLUS("LEX1")
	{
		set_funcs_LEX1();
	}

	void set_funcs_LEX1();

};


struct TCL_ObjType_LEX2 : TCL_ObjType_PLUS
{
	TCL_ObjType_LEX2()
	: TCL_ObjType_PLUS("LEX2")
	{
		set_funcs_LEX2();
	}

	void set_funcs_LEX2();

};


// NB this is where GLOBAL meets parametised LAYERS
// mk_LEX1("lookup") needs global TYPE_LEX1 and global LEX1_SPELLING_POOL

extern
TCL_ObjType_LEX1 * get_TYPE_LEX1(); // does not need interp

extern
TCL_ObjType_LEX2 * get_TYPE_LEX2(); // does not need interp

Tcl_Obj * mk_LEX1( Tcl_Interp * interp, const char * str );
bool upgrade_to_LEX2( Tcl_Obj * obj,  Tcl_Obj * LEX1 );


#endif
