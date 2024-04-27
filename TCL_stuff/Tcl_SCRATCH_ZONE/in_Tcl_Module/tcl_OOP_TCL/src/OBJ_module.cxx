
#include "OBJ_module.h"
#include "TCL_HELP.h"
#include <string.h>
#include "TCL_MATCH.h"
#include "buffer1.h" // buffer1 print

using namespace TCL;  

/*!
	OBJ_MODULE * obj_module == ClientData == a CPU register
	OBJ_ARGV_helper * ARGV_helper == a local var
	
	The key issues are:
	
		obj_module->some_kept_pointer == findable
		obj_module is NOT multi-thread variable
		OBJ_OBJ needs a local var to assist
	
	hoping for as much inline as poss
	so that uses the callers CPU_register
*/
struct OBJ_ARGV_helper 
{
	TCL_LIST list;
	TCL_DICT dict;

	OBJ_ARGV_helper( Tcl_Interp * interp )
	: list( interp )
	, dict( interp )
	{
	}
};

OBJ_module:: OBJ_module( Tcl_Interp * interp ) // CTOR
: objs( interp )
, TYPE_LEX1( NULL )
, TYPE_LEX2( NULL )
, TYPE_DICT( NULL )
, TYPE_VECT( NULL )
{
	// KEEP the KEPT_PTR
//		KEPT_PTR = GET_KEPT_PLUS_PTR_GLOBAL();
	// do INIT this thing, DONT keep it
	// TODO remove
	TYPE_LEX1 = get_TYPE_LEX1(); // build it
	TYPE_LEX2 = get_TYPE_LEX2(); // build it
	TYPE_DICT = get_TYPE_DICT(); // build it
	TYPE_VECT = get_TYPE_VECT(); // build it
	// a TYPE_obj is not a Tcl_Obj just a STRUCT*

	// see also TCL_PLUS_BASE.h
	// obtain addr of link loaded function
	KEPT_PTR = PLUS_MYTYPE_DupInternalRepProc;
}

// MAYBE move these to _PLUS ?

bool OBJ_module:: new_OBJ_VECT( Tcl_Interp * interp, Tcl_Obj ** RET_VAL )
{
	return OBJ_module:: new_OBJ_type(
	 interp,
	 RET_VAL,
	 TYPE_VECT
	);
}

bool OBJ_module:: new_OBJ_DICT( Tcl_Interp * interp, Tcl_Obj ** RET_VAL )
{
	return OBJ_module:: new_OBJ_type(
	 interp,
	 RET_VAL,
	 TYPE_DICT
	);
}

// COMMON PTR2 CTOR
// CXX will use PTR2 -> A_C_STRUCT not a Tcl_Obj *
// VAL1 will get used for u8_subtype u8_MORE u16_idx 

bool OBJ_module:: new_OBJ_type(
	Tcl_Interp * interp,
	Tcl_Obj ** RET_VAL,
	TCL_PLUS_BASE * TYPE_PLUS	// or make this a PLUS type .ABB
)
{
	if(!RET_VAL) {
		return FAIL("NULL RET_VAL");
	}

	const char * NAME = TYPE_PLUS -> alias_one_ABB;	// TYPE_PLUS.ABB

	int obj_idx = 0;
	int obj_idx_2 = 0;
	objs.NN( interp, & obj_idx ); // no lock upto ADD

	Tcl_Obj * VAL = Tcl_NewObj();
	if(!VAL) return FAIL("NULL VAL");

	WARN("expecting CAST");
	VAL -> typePtr = TYPE_PLUS; // does this do downcast NO!!
	VAL -> typePtr = (Tcl_ObjType*) TYPE_PLUS; // does this do downcast NO
	VAL -> typePtr = TYPE_PLUS -> plain(); // manual downcast
	TCL_set_PTR1( VAL, NULL );
//	TCL_set_PTR2( VAL, NULL );
	TCL_set_PTR2( VAL, Tcl_NewListObj( 0, NULL ) );

	// TODO rewrite so that STR is only used when needed
	// until then there is no IDX, not even TCL_LIST objs
	// except for those put into a BIND str
	// debugging might like this though
	//
	// compute the bytes
	buffer1 text;
	text.print("obj_%02X_%s", obj_idx, NAME );

	// copy text into bytes[length]
	const char * S = (STR0) text; // adds NUL byte
	int len = text.nbytes_used; // remember to add 1
	VAL->bytes = (char *)ckalloc(len + 1);
	memcpy(VAL->bytes, S, len+1);
	VAL->length = len;

	// set VAL->typePtr = TYPE_obj_2X
	// set VAL->PTR2 = TCL_LIST_over_PTR
	objs.ADD( interp, &obj_idx_2, VAL );
	if(obj_idx != obj_idx_2) {
		FAIL("obj_idx != obj_idx_2 %d != %d",
			obj_idx,
			obj_idx_2);
		// but stay
		return false; // or not
	}

	// get the new OBJ back somehow ...

	* RET_VAL = VAL;

	Tcl_SetObjResult( interp, VAL );

	print_tcl_obj( VAL, "Tcl_NewObj - PLSU ");
	return PASS("DONE == %s", VAL->bytes );

	return true;
//	return FAIL("TODO");
}


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
Store a "Literal" in a LITERAL_MATCH("GET")
ask MATCHER_get.MATCHES( objv[i] )
ask MATCHER_set.MATCHES( objv[i] )
ask MATCHER_array_set.MATCHES( objv[i] )

Use a {proc _anon_ {} { return {Literal} } to internalise it
That means it WILL have a bytes value, but test for safe zone

Match obj against LITERAL_MATCH, aiming for single PTR == PTR

Match obj against _one _two # accept possible str not SPELLING

That was inline, now call func

Now do strcmp (against match_one) to confirm match or different

KEEP first 4 competing differents, cached, false on PTR == PTR

Too many printfs, making this look a bit naff

*/

#define CRLF "\n"

/*
	an overused FAIL_cmd_argv_usage_error
*/
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

/*!
	OBJ_OBJ is the OBJ command, alis VECT alias DICT ...

	it runs to EOF, but is a bit hazy
*/
CXX_PROTO_T( OBJ_OBJ, OBJ_module * decoder )
{

	OBJ_ARGV_helper cmd(interp);

	//	CIDENT LITERAL or tcl_word_maybe_not_IDENT
	static LITERAL_MATCH match_GET( interp, "GET" );
	static LITERAL_MATCH match_SET( interp, "SET" );
	static LITERAL_MATCH match_ADD( interp, "ADD" );
	static LITERAL_MATCH match_array_set( interp, "array_set" );
	static LITERAL_MATCH match_array_get( interp, "array_get" );
	static LITERAL_MATCH match_LIST_ALL( interp, "LIST_ALL_OBJ_TYPE" );

	//	ARGV0
	static LITERAL_MATCH match_OBJ( interp, "OBJ" );
	static LITERAL_MATCH match_VECT( interp, "VECT" );
	static LITERAL_MATCH match_DICT( interp, "DICT" );

	//	PUNCT not CIDENT LITERAL
	static LITERAL_MATCH match_dash( interp, "-" ); // seems fine
	static LITERAL_MATCH match_EMPTY( interp, "" ); // 
	static LITERAL_MATCH match_NEW( interp, "NEW" ); // 

	//	problematic // because {} // at depth N
	static LITERAL_MATCH match_curly_pair( interp, "{}" );
	// no matches with {{}}


	//	if( decoder->test(interp) ) return TCL_OK;

#if 0
	INFO( "OBJ objc == %d", objc );
	for( int i=0; i< objc; i++ ) {
	CMD_NAME.print_tcl_obj( "CMD_NAME .2 of OBJ A R G S");
		INFO( "OBJ obj%d == %s", i, objv[i]->bytes ) ;
		
	}
#endif

#if 1
	buffer1 args_as_text;

	for( int i=0; i< objc; i++ ) {
		args_as_text.print("%s ", objv[i]->bytes ) ; 
	}
	INFO( "OBJ CXX running { %s }", (STR0)args_as_text );
	args_as_text.clear(); // keep for typical lines < 4K //
#endif

	// libs_apps used to love gdb insight // but red hat forgot how what why

	// gdb_invoke(false);
	//  gdb_break_point();

	// OBJ $obj GET fielname
	// OBJ $obj SET fielname val
	// OBJ $obj CALL ...
	// OBJ $obj NORMALISE
	//
	// OPTION $obj might have type specific OPCODE
	// OPTION look at OPCODE before $obj
	// OPTION [VECT] IS NEW NO ARGS // CAN CHANGE NAME NOT IDX

	if( objc < 1 ) {
		return OBJ_usage_error( interp, objc, objv );
	}

	//////////////////////////////////////////////////////////////////////
	//
	// PARSE CMD_NAME == ARGV0 == OBJ
	//
	// objv[0] == ~ "OBJ" or something
	Tcl_Obj * _CMD_NAME = objv[0]; // 
	TCL_PTR  CMD_NAME = objv[0]; // 
	CMD_NAME.print_tcl_obj( "CMD_NAME in ARGV");

	bool CMD_OBJ = false;
	bool CMD_VECT = false;
	bool CMD_DICT = false; // not written yet

	//	bool CMD_GET = false; // not written yet
	//	bool CMD_SET = false; // not written yet
	//	bool CMD_CALL = false; // not written yet

	if( match_OBJ.MATCHES(CMD_NAME) ) {
		CMD_OBJ = true;
		INFO("GOT CMD_NAME OBJ\n");
	}
	else

	if( match_VECT.MATCHES(CMD_NAME) ) {
		CMD_VECT = true;
		INFO("GOT CMD_NAME VECT\n");
	}
	else

	if( match_DICT.MATCHES(CMD_NAME) ) {
		CMD_DICT = true;
		INFO("GOT CMD_NAME DICT\n");
	}

	else {
		FAIL("not OBJ not VECT not DICT # CMD_NAME == '%s'", CMD_NAME->bytes );
		return OBJ_usage_error( interp, objc, objv );
		// stay  or ERROR
	}

	//////////////////////////////////////////////////////////////////////

	// OBJ $obj_id ...

	/*
	# tcl_obj --------->   0000_55AA_956F_1840  CMD_NAME of OBJ A R G S
	tcl_obj refCount     4
	tcl_obj bytes    3   OBJ
	tcl_obj typePtr.name cmdName    <--- so dont SPLAT over it not a LEX1
	tcl_obj PTR1         0000_55AA_9570_4E80
	tcl_obj PTR2         0000_0000_0000_0000
	*/

	Tcl_Obj * obj_id = NULL;
	bool obj_id_EMPTY = false; 
	if( objc < 2 ) {
		obj_id_EMPTY = true;
	} else {
		obj_id = objv[1];
	}
	Tcl_Obj * NEW_VAL = NULL;

	// SAME C code different CMD name
	// OBJ $id GET field
	// OBJ $id SET field VALUE
	// VECT $VECT_NAME_as_obj_X_at_idx
	// OBJ_NEW_INST
	// TCL_VECT $obj_idx
	// TCL_CODE $obj_OPCODE A R G S
	// CALL $LIBR FUNC A R G S
	// ARGV WRAP_PRE_ARGV 
	// REPRINT L I S T 
	// objv[0] == LEX1 "OBJ" but internal rep is not ours # SO NOT LEX1
	// objv[0] == CMD "OBJ" but internal rep is not ours # SO NOT LEX1

	/*
		LOOK at obj_id
		FETCH obj from OBJ sto
		TODO
	*/


	bool is_VECT = false; // a VECT is really a LIST
	bool is_DICT = false;

	// presume most common is $V is valid $obj_id
	if( !obj_id ) {
		obj_id_EMPTY = true;
	} else 
	if( obj_id -> typePtr == decoder->TYPE_VECT ) {
		is_VECT = true;
		// NB the CMD_NAME and the obj type is VECT
	} else 
	if( obj_id -> typePtr == decoder->TYPE_DICT ) {
		is_DICT = true;
	} else {
		// maybe it is "NEW" or "+" ...
		if( match_EMPTY.MATCHES(obj_id) ) { 
			obj_id_EMPTY = true;
			INFO("GOT EMPTY\n");
		}

		if( match_NEW.MATCHES(obj_id) ) {
			obj_id_EMPTY = true;
			INFO("GOT NEW\n");
		}

		if( match_dash.MATCHES(obj_id) ) {
			obj_id_EMPTY = true;
			INFO("GOT dash\n");
		}

		// dont expect this to work
		// but still need to match empty str
		if( match_curly_pair.MATCHES(obj_id) ) {
			obj_id_EMPTY = true;
			INFO("GOT curly_pair\n");
		}
	}

	// so that sets { obj_id_EMPTY = true; } requiring a NEW



	//////////////////////////////////////////////////////////////////////
	//	STILL GOT
	//	 OBJ $id ...
	//	 OBJ NEW ...
	//	 OBJ NEW VECT ...// generic use of
	//	VECT NEW ... A R G S ...
	//	DICT NEW k1 v1 k2 v2 ...

	if( obj_id_EMPTY ) {
	 switch( objc ) {
	  case 0:	// impossible
	  case 1:	// OBJ
	  case 2:	// VECT $id // id == {} or NEW

		   if( CMD_VECT) { // set V [OBJ NEW] // set X [OBJ {}] 
			if(!decoder->new_OBJ_VECT( interp, & NEW_VAL )) {
				FAIL_FAILED();
				return TCL_ERROR;
			}
			// return the new object, not its value
			// this means more later on VECT NEW A R G S
			is_VECT = true;
			PASS("DONE VECT");
			return TCL_OK;
		   }

		   if( CMD_DICT) { // set V [OBJ NEW] // set X [OBJ {}] 
			if(!decoder->new_OBJ_DICT( interp, & NEW_VAL )) {
				FAIL_FAILED();
				return TCL_ERROR;
			}
			is_DICT = true;
			PASS("DONE DICT");
			return TCL_OK;
		   }

		   return OBJ_usage_error( interp, objc, objv );
	  break;
	  case 3:	// OBJ NEW VECT // OBJ NEW DICT

		   if( CMD_VECT) { // set V [OBJ NEW] // set X [OBJ {}] 
			if(!decoder->new_OBJ_VECT( interp, & NEW_VAL )) {
				FAIL_FAILED();
				return TCL_ERROR;
			}
			PASS("DONE VECT");
			return TCL_OK;
		   }

		   if( CMD_DICT) { // set V [OBJ NEW] // set X [OBJ {}] 
			if(!decoder->new_OBJ_DICT( interp, & NEW_VAL )) {
				FAIL_FAILED();
				return TCL_ERROR;
			}
			PASS("DONE DICT");
			return TCL_OK;
		   }
	  break;

	 }
	}

	//////////////////////////////////////////////////////////////////////

	Tcl_Obj * cmd0 = objv[2];	// OBJ $id CMD ... GET SET ADD

	// maybe not yet
	Tcl_Obj * DATA = objv[3];

	/*
		LOOK at cmd0
		cmd0 REALLY should be a Literal OPCODE
		TODO
	*/


	switch( objc ) {
	 case 0:	// impossible
	 case 1:	// OBJ
	 case 2:	// OBJ $id
		return OBJ_usage_error( interp, objc, objv );
	 break;
	 case 3:	// OBJ $id OPCODE // VECT $id OPCODE // cmd0 == OPCODE

		if(obj_id_EMPTY) { // OBJ {} NEW // VECT {} NEW // OBJ NEW VECT

		 if( match_VECT.MATCHES(cmd0) ) {

			if(!decoder->new_OBJ_VECT( interp, & NEW_VAL )) {
				FAIL_FAILED();
				return TCL_ERROR;
			}
			PASS("DONE obj new VECT");
			return TCL_OK;
		 }

		 // else ... _DICT
		 // else FAIL
		 return OBJ_usage_error( interp, objc, objv );

		} // else obj_id is not "NEW" "-" "

		INFO("look for array_get");
		if( match_array_get.MATCHES(cmd0) ) {
		INFO("got array_get");
	// gdb_invoke(false);
	// gdb_break_point();

			// convert obj_id to LIST
			if( obj_id -> typePtr == decoder->TYPE_VECT ) {
				// get PTR2 into LIST without touching ref_count
				TCL_LIST LIST( (Tcl_Obj*) TCL_get_PTR2( obj_id ));
				print_tcl_obj( (Tcl_Obj*) obj_id, "obj_id" );
				print_tcl_obj( (Tcl_Obj*) LIST.list, "LIST with bad refcount" );
				int pos = 0;
				TCL_REF RET_VAL;
				if(LIST.array_get( interp, RET_VAL )) {
					Tcl_SetObjResult( interp, RET_VAL );
					return TCL_OK;
				} else
					return TCL_ERROR;
			}

			// convert obj_id to LIST
			if( obj_id -> typePtr == decoder->TYPE_DICT ) {
				// get PTR2 into DICT without touching ref_count
				TCL_DICT DICT( (Tcl_Obj*) TCL_get_PTR2( obj_id ));
				print_tcl_obj( (Tcl_Obj*) obj_id, "obj_id" );
				print_tcl_obj( (Tcl_Obj*) DICT.dict, "DICT with bad refcount" );
				int pos = 0;
		/* DIFF */		TCL_LIST RET_VAL(interp);
				if(DICT.array_get( interp, RET_VAL )) {
					Tcl_SetObjResult( interp, RET_VAL.list );
					return TCL_OK;
				} else
					return TCL_ERROR;
			}

			WARN("expected TYPE_VECT or DICT");
			return TCL_ERROR;
		}

		if( match_LIST_ALL.MATCHES(cmd0) ) {
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

		WARN("not recognised Literal %s \n",cmd0->bytes );
		return OBJ_usage_error( interp, objc, objv );

	 break;
	 case 4: // {
		// OBJ $id OPCODE fieldname
		// OBJ $id ADD value
		// OBJ $id GET fieldname
		// OBJ $id GET 2 // VECT

		// OBJ $id GET fieldname
		if( match_GET.MATCHES(cmd0 ) ) {
		 if(is_VECT) {
			int VAL_IDX = -1;
			if(TCL_OK!=Tcl_GetIntFromObj( interp, DATA, &VAL_IDX )) {
				WARN("expected VAL_IDX got %s", "XXX");
				return TCL_ERROR;
			}
			if( obj_id -> typePtr != decoder->TYPE_VECT ) {
				WARN("expected TYPE_VECT got %s", "TTT");
				return TCL_ERROR;
			}
	//  gdb_invoke(false);
	//  gdb_break_point();
				// get PTR2 into LIST without touching ref_count
			TCL_LIST VECT( (Tcl_Obj*) TCL_get_PTR2( obj_id ));
			print_tcl_obj( obj_id, "obj_id");
			print_tcl_obj( VECT.listPtr(), "VECT.listPtr()");
			TCL_REF RET_VAL;
			VECT.GET( interp, VAL_IDX, RET_VAL );
			Tcl_SetObjResult( interp, (Tcl_Obj*)RET_VAL );
			return TCL_OK;
		 }
		 if(is_DICT) {
		 }
				WARN("expected VAL_IDX got %s", "XXX");
				return TCL_ERROR;
		}

		// OBJ $id ADD value
		if( match_ADD.MATCHES(cmd0) ) {
			// convert obj_id to LIST
			if( obj_id -> typePtr == decoder->TYPE_VECT ) {
				// obj_id is id of obj AND also obj holder itself
				// 
				TCL_LIST VECT( (Tcl_Obj*) TCL_get_PTR2( obj_id ) );
				// DATA was objv[3]
				int pos = 0;
				if(!(VECT.ADD( interp, &pos, DATA ))) {
					FAIL_FAILED();
					return TCL_ERROR;
				}
				Tcl_SetObjResult( interp, DATA );
				return TCL_OK;
			}

			else
			if( obj_id -> typePtr == decoder->TYPE_DICT ) {
				WARN("DICT is not a LIST no ADD");
		//		TCL_DICT DICT( (Tcl_Obj*) TCL_get_PTR2( obj_id ));
		//		int pos = 0;
		//		Tcl_Obj * VAL = objv[3];
		//		if(DICT.ADD( interp, &pos, VAL ))
		//			return TCL_OK;
		//		else
					return TCL_ERROR;
			}

			else {
				FAIL("ADD but obj_id is not LIST");
				return OBJ_usage_error( interp, objc, objv );
				return TCL_ERROR;
			}

			if(CMD_VECT) ;
			if(CMD_DICT) // or data_is_DICT
			// VECT LIST DICT OBJ 
			INFO("GOT ADD returning nothing\n");
			return TCL_OK;
		} 

		if( match_array_set.MATCHES(cmd0) ) {

			// convert obj_id to LIST
			if( obj_id -> typePtr == decoder->TYPE_VECT ) {
				TCL_LIST LIST( interp, (Tcl_Obj **) TCL_get_EA_PTR2(obj_id));
				int pos = 0;
				Tcl_Obj * VAL = objv[3];
				if(LIST.array_set( interp, VAL )) {
					// return what ?
	Tcl_SetObjResult( interp, objv[3] );
					return TCL_OK;
				} else
					return TCL_ERROR;
			}

			// convert obj_id to DICT
			if( obj_id -> typePtr == decoder->TYPE_DICT ) {
				TCL_DICT DICT( interp, (Tcl_Obj **) TCL_get_EA_PTR2(obj_id));
				int pos = 0;
				Tcl_Obj * VAL = objv[3];
				if(DICT.array_set( interp, VAL )) {
					// return what ?
	Tcl_SetObjResult( interp, objv[3] );
//	Tcl_SetObjResult( interp, DICT.dict );
					return TCL_OK;
				} else
					return TCL_ERROR;
			}

		} else

		;

		// GET fieldname // 
		return OBJ_usage_error( interp, objc, objv );

	 // }
	 break;
	 case 5:	// OBJ $id OPCODE fieldname value

		if( match_SET.MATCHES(cmd0 ) ) {
			INFO("GOT SET\n");
			return TCL_OK;
		}

		if( match_array_set.MATCHES(cmd0 ) ) {
			INFO("GOT array_set\n");
			return TCL_OK;
		}
	 break;
	default:
		return OBJ_usage_error( interp, objc, objv );
	}

	  INFO("not GOT %s \n",cmd0->bytes );

	#if 0
	print_tcl_obj( objv[0] );
	print_tcl_obj( cmd0 );

	// look at refCount and at pointer address
	Tcl_Obj * obj_GET = mk_common_spelling(interp, "GET");
	print_tcl_obj( obj_GET );
	#endif
	#if 0
	Tcl_GetString(cmd0);
	print_tcl_obj( cmd0 );
	#endif

	return TCL_OK;
}

CXX_PROTO_T( OBJ_test1, OBJ_module * decoder )
{
	if( ! decoder -> test(interp)) {
		return TCL_ERROR;
	}
	return TCL_OK;
}


// require TCL:: // using namespace not enough // CALAS yes FUNC no
// extern 
int TCL:: declare_OBJ_functions( Tcl_Interp * interp, OBJ_module * decoder )
{
	Tcl_CmdDeleteProc * deleteProc = NULL; // for now
	Tcl_Command token;

	token=Tcl_CreateObjCommand(
		interp,
		"test1",
/* CAST */ (Tcl_ObjCmdProc*)
		OBJ_test1,
/* CAST */  (ClientData)
		decoder, // ClientData == decoder // += one_of_group
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

	token=Tcl_CreateObjCommand(
		interp,
		"VECT", // VECT new //
/* CAST */ (Tcl_ObjCmdProc*)
		OBJ_OBJ, // keep to one CXX function OBJ_OBJ decoder // etc
/* CAST */  (ClientData)
		decoder,
		deleteProc // VECT.ClientData == OBJ.CLientData == MOD_OBJ
	);

	token=Tcl_CreateObjCommand(
		interp,
		"DICT", // DICT new // an alias for "OBJ" Command
/* CAST */ (Tcl_ObjCmdProc*)
		OBJ_OBJ, // keep to one CXX function OBJ_OBJ decoder // etc
/* CAST */  (ClientData)
		decoder,
		deleteProc // VECT.ClientData == OBJ.CLientData == MOD_OBJ
	);

	return TCL_OK;
}

/*	ALIAS u8_u8 { u8_spec u8_N }
	u8_spec {
		// KNOW // PTR == u48_payload
		// u8_u8 // stmt N // GETTER("PROVIDES ARGV[N]")
		// WORDS = THIS_STO.ARGV_WORDS
		// SELF += lookup( PTR ) // spec says is u48_payload
		// N = u8_N
	}

	SPEC_t SPECS[N]
	EIGHT_t WORDS[idx]

	u8_spec = byte_A
	u8_item = byte_B
	or
	u8_spec = byte_B
	u8_item = byte_A

	HERE=CODE[u8_spec]
	SPEC=SPEC[u8_spec] // SPEC SPECS // SUBLEX



*/
/*	ALIAS

	OBJ	- stay with OBJ as it does double_step first_step
	LIST	- creates a LIST_VIEW of OBJ
	DICT	- creates a ITEM_VIEW of OBJ
	VECT	- creates a ITEM_VIEW of OBJ
	FIELDS	- byte_fields in shared REG or expanded WORD = VAR
	BITFIELDS - worth a try
	ITEM_t & ITEM = DRY[KEY] // & is dropped, but there for reminder
	ITEM_t * ITEM = DRY[KEY] // * is dropped, dot notation used
	V = %d { idx_t idx } // typedef u32 idx_t
	OBJ $V GET $idx // u64_WORD "$idx" is EXPR is local VAR "idx"
	# local VARS # nearby vars # V idx
	# typed VALS # $V is u64_VECT_V_t { TCL_VECT V // ALIAS $V }

	N_WORDS_IN_ARGV==BYTE_A
	SPEC_THIS_PICK==BYTE_B

		WORDS = EIGHT_u8 // MINI_API_MACHINE

		 // EIGHT_t * WORDS = u64_MEM_ADDR // EXPR

		OBJ $u48_V $u8_u8
		// SCRIPT makes ALIAS u8_u8 very fast
		// it KNOWS u16_lohi_of_u64_lohi // cpu_var_BYTE_FIELDS
		// {OPCODE} 
		// V = u48
		// u8_u8 // SPEC_t & SPEC = SPECS[u8_spec] // "OBJV" _t
		// u8_u8 // ITEM_t & ITEM = ITEMS[u8_item]
		// u8_u8_lohi is u16_lohi
		// u8_u8_lohi u8_u8_liho --PICK-->

		// I think that we REQUIRE ints stored lohi
		// I think that SOURCE FILTER can RPC remote data
		// 
		u8_u8_lohi
		u8_u8_hilo 

		ASM REG_RET_VAL = WORDS[BYTE_B]

	TODO

	GET
	SET
	CALL // obj is CALLABLE // GEN tracing API // LOG VAL //



*/

/*	TODO

	SCRIPT CMD 
	 ALIAS into OBJ DECODER
	  ALIAS
	   VECT

	 CMD == MINI_MACHINE // single point of API
	 CMD == ALIAS COMPONENT or LAYER of MINI_MACHINE

	 BUILD TREE of OPCODE 
	 EXPORT LIST from TREE to API // DECODER sets FLAG vars OPTIONS
	 SYNTAX does vary with ARGV0

[set V   OBJ new VECT]
	 OBJ $V GET $idx
	 OBJ $V SET $idx $value
[set idx OBJ $V ADD - $value]
	 OBJ $V array_set {
		  1 ONE
		 99 M1
		  8 EIGHT_t
		  9 BYTES_8 
		 10 BYTES_16
	 } 

	SO as part of generic DECODE A R G V
		DECODE VECT A R G V
		DECODE VECT $V A R G V
		DECODE VECT {} A R G V // NULL obj_id
		DECODE VECT + A R G V // new + NEW MK_ mk_ VECT_%d

	test is to code around keeping ARGV0 as NOT a LEX1

		want to know PTR1 PTR2 untouched by me
		want to know typePtr->name == "cmdName" ;// says Tcl
		// thats what tcl calls it "cmdName" we can use "ARGV0"

		call twice to know that first call did not LEX2 it

		also write and test the MATCHER for cmdName

DESIGN

	cmdName == "cmd0" == "ARGV0 -> typeName == "cmdName"
		EA = ARGV0
		bytes == "cmd0"

DESIGN

	PAIR of ARRAYS // only a few N

	 LAYER A
	 LAYER B

		LIST1 = list of u32_PTR_ADDR_USED // 32 BIT PTR
		LIST1 = list of u64_PTR_ADDR_USED
		LIST2 = list of u64_EIGHT_CALL_DATA

		ITEM1 == Tcl_Obj * ARGV0; // but in LIST1
		ITEM2 == PLUS_Obj * HANDLER // CODE for MINI_MACHINE

		HANDLER == { SCRIPT } {

			u16_OPCODE = from ITEM2
			u48`payload= from ITEM2

			DECODE u16_OPCODE_PAIR u16_in_CPU_VAR

			u16_payload from 32 BIT SYSTEM

				64K * 8 byte = 512K click 8

			This is the first 512K of MMAP area

			BASE = MEM_PAGE0_of_AREA
			OFFS = u16_idx * 8

			WORD_VECT = BASE // VECT must be 4K aligned?
			BASE must be u64 aligned

			MINI_MACHINE has 512K + MORE
			 makes most things work in first 512K
			 simply carries on for u16_idx u32_idx u64_idx
			 template types expanded by API GEN
			 reduce over register with home brew resolve

			ClientData = MODULE_OBJ_WORLD // 512K // MORE
			ARGV0 = 'VECT'
			ITEM2 = u48_u16 // actually u16_u8_u8
			// OPTION // u32_u32 // like PTR1 PTR2

			// OPTION we dont keep Tcl_Obj *
			// OPTION we keep u48_u16 
			// OPTION we keep u32_u32 
			// OPTION we keep u32_u16_u8_u8 

			OPTION
				EIGHT_t EIGHT = WORD_VECT[OFFS]

		EIGHT_t *
		u8 * PTR; // u16 converted to u19_pointer (512K)
		u8_aligned_to_WORD // click8 ALLOC8 ALIGN_EIGHT EIGHT_t

				EIGHT_t * WORD_PTR( "{ SCRIPT }");

				OPTION 1 # EIGHT_t & ITEM;
				OPTION 2 # EIGHT_t * WORDS_OF_ITEM_IN_MEN
					unlimited WORDS; QUOTA
		get_EA

			u16 idx = get_from_word // u48 on u64
			OFFS = idx // u16 to WORD // COPY REG2
			BASE = BASE
			ADJ = offset_of_field_in_struct

			u8 * EA = BASE + (idx * 8) + ADJ

		x86 ASM

			u8 * EA = (EA_CAST) EIGHT_t * EA8 = BASE[idx*8]

			It just so happens that X86 has [idx*8]
			we can use u32 or u64 for full file
			but we have u16 from compact_data

			x86 ASM is very good at computing that directly
			It adds an ADJ offset, for fixed field_in_struct
		}

	ASM sweeps the Tcl_Obj * obj == ARGV0 over LIST1

		It is found as match_three in LIST1

		match_three == ARGV0
		found_three == handler for match_three

		typePtr .name == "TYPE PLUS LEX3_ARGV0_cmdName"

			UDEF type SCRIPTED value types

				SUBLEX "cmd0" Tcl_Obj_typePtr_name_cmdName


*/
