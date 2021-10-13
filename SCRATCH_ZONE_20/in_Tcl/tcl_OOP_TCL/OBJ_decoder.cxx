
#include "OBJ_decoder.h"

  int OBJ_test1(
   OBJ_decoder * decoder,
   Tcl_Interp *interp,
   int objc,
   Tcl_Obj *const objv[]
  )
  {
  	
  	if( decoder -> test(interp)) {
		return TCL_ERROR;
	}
	fprintf(stderr,"OBJ_test1\n");
	return TCL_OK;
  }


// extern 
int declare_OBJ_functions( Tcl_Interp * interp, OBJ_decoder * decoder )
{
	Tcl_CmdDeleteProc * deleteProc = NULL; // for now

	if(TCL_OK!=Tcl_CreateObjCommand(
	 interp,
	 "test1",
/* CAST */ (Tcl_ObjCmdProc*)
	 OBJ_test1,
/* CAST */  (ClientData)
	 decoder,
	 deleteProc
	)) {
	}
	return TCL_OK;
}

