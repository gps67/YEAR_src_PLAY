
#include "OBJ_decoder.h"
#include "TCL_HELP.h"
#include <string.h>

#define CXX_PROTO_T( func_name, parameter_line )	\
				\
  int func_name(		\
   parameter_line, 		\
   Tcl_Interp *interp,		\
   int objc,			\
   Tcl_Obj *const objv[]	\
  )				\
// CALLED by TCL we must return TCL_OK or _ERROR or ...

struct MATCHER {
	static const int N_different = 4;

	// these auto DTOR CTOR NULL refcount 
	TCL_PTR match_one;
	TCL_PTR match_two;
	TCL_PTR differents[N_different];

	~MATCHER()
	{
	}

	MATCHER( const char * str )
	{
		match_one = mk_common_spelling( str );
	}

	bool MATCHES_fn( Tcl_Obj * obj )
	{
		if( obj == match_one ) return true;
		if( obj == match_two ) return true;
		for( int i = 0; i<N_different; i++ ) {
			if( obj == differents[i] ) {
				// add to counters
				return false;
			}
		}
		if( !match_one ) {
			// match_one is supposed to exist
			fprintf(stderr,"**** // NULL match_one\n");
			return false;
		} else if( !match_one->bytes ) {
			// match_one is supposed to exist
			fprintf(stderr,"**** // NULL match_one bytes\n");
			return false;
		} else { // OK
		if( CMP( 
	if( CMP( cmd, "array_get" )) {
		return false;
	}

	bool MATCHES( Tcl_Obj * obj )
	{
		if( obj == match_one ) return true;
		if( obj == match_two ) return true;
		return MATCHES_fn( obj );

	}
};

#define CRLF "\n"

int OBJ_usage_error( Tcl_Interp * interp, int objc, Tcl_Obj *const* objv )
{
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

bool CMP( Tcl_Obj * obj, const char * str )
{
	const char * str_word = Tcl_GetString( obj );
	return 0 == strcmp( str, str_word );
}

CXX_PROTO_T( OBJ_OBJ, OBJ_decoder * decoder )
  {
  	if( decoder->test(interp) ) return TCL_OK;

	fprintf(stderr, "OBJ objc == %d \n", objc );

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

	// objv[0] == ~ "OBJ" as something
	Tcl_Obj * obj_id = objv[1];
	Tcl_Obj * cmd = objv[2];

	/*
		LOOK at obj_id
		FETCH obj from OBJ sto
		TODO
	*/

	/*
		LOOK at cmd
		It REALLY should be a Literal OPCODE
		TODO
	*/

	if( CMP( cmd, "array_get" )) {
	  	fprintf(stderr,"GOT array_get\n");
		return TCL_OK;
	}

	switch( objc ) {
	 case 0:	// impossible
	 case 1:	// OBJ
	 case 2:	// OBJ $id
		return OBJ_usage_error( interp, objc, objv );
	 break;
	 case 3:	// OBJ $id OPCODE
	
		return OBJ_usage_error( interp, objc, objv );
	 break;
	 case 4:	// OBJ $id OPCODE fieldname
	 break;
	 case 5:	// OBJ $id OPCODE fieldname value
	 ;
	default:
		return OBJ_usage_error( interp, objc, objv );
	}

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

