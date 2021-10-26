
#include "OBJ_decoder.h"
#include "TCL_HELP.h"
#include <string.h>
#include "TCL_MATCHER.h"

#define CXX_PROTO_T( func_name, parameter_line )	\
				\
  int func_name(		\
   parameter_line, 		\
   Tcl_Interp *interp,		\
   int objc,			\
   Tcl_Obj *const objv[]	\
  )				\
// CALLED by TCL we must return TCL_OK or _ERROR or ...

/*!
	Store a "Literal" in a LITERAL_MATCHER("GET")
	ask MATCHER_get.MATCHES( objv[i] )
	ask MATCHER_set.MATCHES( objv[i] )
	ask MATCHER_array_set.MATCHES( objv[i] )

	Use a {proc _anon_ {} { return {Literal} } to internalise it
	That means it WILL have a bytes value, but test for dafe zone

	Match obj against LITERAL_MATCHER, aiming for single PTR == PTR

	Match obj against _one _two # accept possible str not SPELLING

	That was inline, now call func

	Now do strcmp (against match_one) to confirm match or different

	KEEP first 4 competing differents, cached, false on PTR == PTR

	Too many printfs, making this look a bit naff

*/

#define CRLF "\n"

int OBJ_usage_error( Tcl_Interp * interp, int objc, Tcl_Obj *const* objv )
{
	FAIL("USAGE");
	objc = 0;
	Tcl_WrongNumArgs(interp, objc, objv, 
	"..." CRLF
	"eg OBJ $id GET field" CRLF
	"eg OBJ $id SET field $value" CRLF
	"eg OBJ $id array_get" CRLF
	"eg OBJ $id array_set $pairs_list" CRLF
	);
	return TCL_ERROR;
}

CXX_PROTO_T( OBJ_OBJ, OBJ_decoder * decoder )
  {


  	if( decoder->test(interp) ) return TCL_OK;

	INFO( "OBJ objc == %d", objc );
//  gdb_invoke(false);
	INFO( "and on" );
//  gdb_break_point();

  	// OBJ $obj GET fielname
  	// OBJ $obj SET fielname val
  	// OBJ $obj CALL ...
  	// OBJ $obj NORMALISE
	//
	// OPTION $obj might have type specific OPCODE
	// OPTION look at OPCODE before $obj

	if( objc < 3 ) {
		return OBJ_usage_error( interp, objc, objv );
	}

	// objv[0] == ~ "OBJ" or something
	Tcl_Obj * obj_id = objv[1];
	Tcl_Obj * cmd = objv[2];

	/*
		LOOK at obj_id
		FETCH obj from OBJ sto
		TODO
	*/

	/*
		LOOK at cmd
		cmd REALLY should be a Literal OPCODE
		TODO
	*/

	static LITERAL_MATCHER match_GET( interp, "GET" );
	static LITERAL_MATCHER match_SET( interp, "SET" );
	static LITERAL_MATCHER match_array_set( interp, "array_set" );
	static LITERAL_MATCHER match_array_get( interp, "array_get" );
	static LITERAL_MATCHER match_LIST_ALL( interp, "LIST_ALL_OBJ_TYPE" );


	switch( objc ) {
	 case 0:	// impossible
	 case 1:	// OBJ
	 case 2:	// OBJ $id
		return OBJ_usage_error( interp, objc, objv );
	 break;
	 case 3:	// OBJ $id OPCODE

		if( match_array_get.MATCHES(cmd) ) {
			INFO("GOT array_get\n");
			return TCL_OK;
		}

		if( match_LIST_ALL.MATCHES(cmd) ) {
		//	return list of all objType names
		//	boolean double end-offset regexp list cmdName bytecode
		//	procbody bytearray int dict {array search} string
		if(0)
			INFO("GOT LIST_ALL\n");
			Tcl_Obj * retlist = Tcl_NewListObj( 0, NULL );
			if( TCL_OK!=
				Tcl_AppendAllObjTypes( interp, retlist )
			) {
				return false;
			}
			Tcl_SetObjResult( interp, retlist );
		if(0)
			INFO("GOT LIST_ALL ==> %s\n",
				Tcl_GetString( retlist ));
			return TCL_OK;
		}

		WARN("not recognised Literal %s \n",cmd->bytes );
		return OBJ_usage_error( interp, objc, objv );

	 break;
	 case 4:	// OBJ $id OPCODE fieldname

		if( match_GET.MATCHES(cmd ) ) {
			INFO("GOT GET\n");
			return TCL_OK;
		}
	
		// GET fieldname // 

	 break;
	 case 5:	// OBJ $id OPCODE fieldname value

		if( match_SET.MATCHES(cmd ) ) {
			INFO("GOT SET\n");
			return TCL_OK;
		}

		if( match_array_set.MATCHES(cmd ) ) {
			INFO("GOT array_set\n");
			return TCL_OK;
		}
	 break;
	default:
		return OBJ_usage_error( interp, objc, objv );
	}

	  INFO("not GOT %s \n",cmd->bytes );

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

