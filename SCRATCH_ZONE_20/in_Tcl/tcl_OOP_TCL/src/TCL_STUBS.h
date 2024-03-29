#ifndef TCL_STUBS_H
#define TCL_STUBS_H

	#include "dgb.h" // INFO WARn FAIL

	// Tcl_Proper types (standard)
	// TCL_related types (this project)
	// 

	struct Tcl_Obj;
	struct Tcl_Interp;

 namespace TCL {

	class OBJ_module;

	class TCL_PLUS_LEX1;
	class TCL_PLUS_BASE;

	struct TCL_REF;
	struct TCL_LIST;
	struct TCL_DICT;
 };

	// usually defined elsewhere
	// see stdint.h
	typedef unsigned int u32;
	typedef unsigned char u8;
	typedef  u8  u8_idx;
	typedef u32 u32_lohi;


#endif

