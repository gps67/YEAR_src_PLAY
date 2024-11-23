#ifndef TCL_ObjType_plain_H
#define TCL_ObjType_plain_H

#include "TCL.h"
#include "TCL_STUBS.h" // INFO WARN FAIL
#include "TCL_HELP.h" // PTR1 PTR2

namespace TCL {

	/*
		TCL_ObjType_plain 
		Tcl_ObjType

		NO ADDED FIELDS
		USE WITH _plain Tcl types - limited USAGE

		Anything that a plain Tcl_ObjType might need

		NOTE that there is nothing PLAIN about _plain
		because it is (maybe) a UDEF Tcl_ObjType
		so some code is doing something with it
		HOWEVER we may encounter _builting types
		HOWEVER we may encounter other extension's types
	*/
 struct TCL_ObjType_plain : Tcl_ObjType
 {

	void set_funcs_NULL()
	{
	 #if 0	
	 	// RTFM Tcl_ObjType
		Tcl_FreeInternalRepProc *freeIntRepProc;
		Tcl_DupInternalRepProc *dupIntRepProc;
		Tcl_UpdateStringProc *updateStringProc;
		Tcl_SetFromAnyProc *setFromAnyProc;
	 #endif
		freeIntRepProc = NULL;
		dupIntRepProc = NULL;
		updateStringProc = NULL;
		setFromAnyProc = NULL;
	#if TCL_MAJOR_VERSION >= 9
		// Tcl has awarded itself an extra field mechanism
		// and claimed it for itself, but how do you use it for yours ?
		// There is some OPTIMISATION JOY when using the AS_LIST shimmer
		// TODO lucky you
	#endif

	}

 };

}; // namespace
#endif

