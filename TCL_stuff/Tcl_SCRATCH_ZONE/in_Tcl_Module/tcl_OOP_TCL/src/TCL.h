#ifndef TCL_H
#define TCL_H
// optimise this include
#include <tcl.h>


#if TCL_MAJOR_VERSION < 9
# error needs TCL 9
#endif

//     TCL_TYPE.txt 
// see TCL_TYPE.txt - we manipulate the struct Tcl_ObjType 
// see TCL_TYPE.txt - we tag is with obj -> typePtr -> dupIntRepProc
// == KEPT_PTR
// then KNOW using at least obj -> type == UDEF_BASED_ON_PLUS
// that then has type CXX TYPE SPEC and adds list_of_fields to TYPE_SPEC
// extends the struct Tcl_ObjType 

// we need a FAST TEST of Tcl_Obj * obj
// we need a FAST TEST of Tcl_Obj * obj -> typePtr -> dupIntRepProc 
// NULL_MEANS_goto_NOT //
// we need a FAST FETCH of PLUS_MYTYPE_DupInternalRepProc
// OBJ_module brings its own minimum API
// TCL.h is to include <tcl.h> or fake it just enough
// this smaller file is parsed much quicker // on repeat #include



#endif
