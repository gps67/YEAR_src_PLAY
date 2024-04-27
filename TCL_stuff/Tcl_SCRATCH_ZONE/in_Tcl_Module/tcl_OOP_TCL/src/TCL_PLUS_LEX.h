#ifndef TCL_PLUS_HEX_H
#define TCL_PLUS_HEX_H
//	#include "TCL_ObjType_PLUS.h"

#include "TCL.h"
#include "TCL_STUBS.h" // INFO WARN FAIL
#include "TCL_HELP.h" // PTR1 PTR2
#include "TCL_PLUS_BASE.h" // PTR1 PTR2
// #include "dgb.h" // FAIL

namespace TCL { // ##################################### TCL ####

struct TCL_PLUS_LEX1 : TCL_PLUS_BASE
{
	TCL_PLUS_LEX1()
	: TCL_PLUS_BASE("LEX1")
	{
		// see get_TYPE_LEX1
//		bytes_never_NULL = true; // LEX1 is LEX1 -> bytes
//		PTR2_is_Tcl_Obj = false; // LEX1 is just a typePtr tag
	}
};


struct TCL_PLUS_LEX2 : TCL_PLUS_BASE
{
	TCL_PLUS_LEX2()
	: TCL_PLUS_BASE("LEX2")
	{
		// see get_TYPE_LEX2() for details
		bytes_never_NULL = true; // LEX2 is LEX1 -> bytes
	}
};

struct TCL_PLUS_TCL2 : TCL_PLUS_BASE
{
	TCL_PLUS_TCL2( const char * name )
	: TCL_PLUS_BASE(name)
	{
		PTR2_is_Tcl_Obj = true; // TCL2
	}
};

struct TCL_PLUS_DICT : TCL_PLUS_TCL2
{
	TCL_PLUS_DICT()
	: TCL_PLUS_TCL2("DICT")
	{
	}
};

struct TCL_PLUS_VECT : TCL_PLUS_TCL2
{
	TCL_PLUS_VECT()
	: TCL_PLUS_TCL2("VECT")
	{
	}
};


// NB this is where GLOBAL meets parametised LAYERS
// mk_LEX1("lookup") needs global TYPE_LEX1 and global LEX1_SPELLING_POOL

extern
TCL_PLUS_LEX1 * get_TYPE_LEX1(); // does not need interp
extern
TCL_PLUS_LEX2 * get_TYPE_LEX2(); // does not need interp
extern
TCL_PLUS_DICT * get_TYPE_DICT(); // does not need interp
extern
TCL_PLUS_VECT * get_TYPE_VECT(); // does not need interp

Tcl_Obj * mk_LEX1( Tcl_Interp * interp, const char * str );
bool upgrade_to_LEX2( Tcl_Obj * obj,  Tcl_Obj * LEX1 );


}; // namespace
#endif
