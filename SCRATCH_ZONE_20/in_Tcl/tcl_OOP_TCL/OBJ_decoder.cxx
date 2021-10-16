
#include "OBJ_decoder.h"
#include "TCL_HELP.h"

#define CXX_PROTO_T( func_name, parameter_line )	\
				\
  int func_name(		\
   parameter_line, 		\
   Tcl_Interp *interp,		\
   int objc,			\
   Tcl_Obj *const objv[]	\
  )				\
// CALLED by TCL we must return TCL_OK or _ERROR or ...

CXX_PROTO_T( OBJ_OBJ, OBJ_decoder * decoder )
  {
  	if( decoder->test(interp) ) return TCL_OK;

	fprintf(stderr, "OBJ objc == %d \n", objc );

  	// OBJ $obj GET fielname
  	// OBJ $obj SET fielname val
  	// OBJ $obj CALL ...
  	// OBJ $obj NORMALISE

	if( objc < 3 ) {
		// Tcl provides objv[0]
		Tcl_WrongNumArgs(interp, 1, objv, "$obj CMD ... GET SET CALL etc");
		return TCL_ERROR;
	}

	// objv[0] == ~ "OBJ" as something
	Tcl_Obj * obj_id = objv[1];
	Tcl_Obj * cmd = objv[2];

#if 0
	print_tcl_obj( objv[0] );
	print_tcl_obj( cmd );

	// look at refCount and at pointer address
	Tcl_Obj * obj_GET = mk_common_spelling(interp, "GET");
	print_tcl_obj( obj_GET );
#endif
#if 0
	Tcl_GetString(cmd);
	print_tcl_obj( cmd );
#endif

	return TCL_OK;
  }

CXX_PROTO_T( OBJ_test1, OBJ_decoder * decoder )
  {
  	
  	if( ! decoder -> test(interp)) {
		return TCL_ERROR;
	}
	return TCL_OK;
  }


// extern 
int declare_OBJ_functions( Tcl_Interp * interp, OBJ_decoder * decoder )
{
	Tcl_CmdDeleteProc * deleteProc = NULL; // for now
	Tcl_Command token;

	token=Tcl_CreateObjCommand(
		interp,
		"test1",
/* CAST */ (Tcl_ObjCmdProc*)
		OBJ_test1,
/* CAST */  (ClientData)
		decoder,
		deleteProc
	);

	token=Tcl_CreateObjCommand(
		interp,
		"OBJ",
/* CAST */ (Tcl_ObjCmdProc*)
		OBJ_OBJ,
/* CAST */  (ClientData)
		decoder,
		deleteProc
	);

	return TCL_OK;
}

