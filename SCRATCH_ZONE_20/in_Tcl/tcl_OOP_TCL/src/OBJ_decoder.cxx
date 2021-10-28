
#include "OBJ_decoder.h"
#include "TCL_HELP.h"
#include <string.h>
#include "TCL_MATCHER.h"
#include "buffer1.h" // buffer1 print

	// MAYBE move these to _PLUS ?

	bool OBJ_decoder:: new_OBJ_DICT( Tcl_Interp * interp, Tcl_Obj ** RET_VAL )
	{
		if(!RET_VAL) {
			return FAIL("NULL RET_VAL");
		}
		int pos = 0;
	//	Tcl_Obj * VAL = Tcl_NewObj();
		buffer1 text;
		int obj_idx = 0;
		int obj_idx_2 = 0;
		list.NN( interp, & obj_idx ); // no lock upto ADD
		text.print("obj_%2X", obj_idx );
		Tcl_Obj * VAL = Tcl_NewStringObj((STR0) text,-1);
//	// there is no TYPE_DICT
//	maybe reuse set from any
		VAL -> typePtr = TYPE_DICT;

		// set VAL->typePtr = TYPE_obj_2X
		// set VAL->PTR2 = TCL_LIST_over_PTR
		if(!VAL) return FAIL("NULL VAL");
		list.ADD( interp, &obj_idx_2, VAL );
		if(obj_idx != obj_idx_2) {
			FAIL("obj_idx != obj_idx_2 %d != %d",
				obj_idx,
				obj_idx_2);
			// but stay
			return false; // or not
		}

		// get the new OBJ back somehow ...

		* RET_VAL = VAL;

		return PASS("DONE == %s", VAL->bytes );

		return true;
	//	return FAIL("TODO");
	}


	bool OBJ_decoder:: new_OBJ_VECT( Tcl_Interp * interp, Tcl_Obj ** RET_VAL )
	{
		if(!RET_VAL) {
			return FAIL("NULL RET_VAL");
		}
		int pos = 0;
	//	Tcl_Obj * VAL = Tcl_NewObj();
		buffer1 text;
		int obj_idx = 0;
		int obj_idx_2 = 0;
		list.NN( interp, & obj_idx ); // no lock upto ADD
		text.print("obj_%2X", obj_idx );
		Tcl_Obj * VAL = Tcl_NewStringObj((STR0) text,-1);
		// set VAL->typePtr = TYPE_obj_2X
		VAL -> typePtr = TYPE_VECT;
		// set VAL->PTR2 = TCL_LIST_over_PTR
		if(!VAL) return FAIL("NULL VAL");
		list.ADD( interp, &obj_idx_2, VAL );
		if(obj_idx != obj_idx_2) {
			FAIL("obj_idx != obj_idx_2 %d != %d",
				obj_idx,
				obj_idx_2);
			// but stay
			return false; // or not
		}

		// get the new OBJ back somehow ...

		* RET_VAL = VAL;

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
	Store a "Literal" in a LITERAL_MATCHER("GET")
	ask MATCHER_get.MATCHES( objv[i] )
	ask MATCHER_set.MATCHES( objv[i] )
	ask MATCHER_array_set.MATCHES( objv[i] )

	Use a {proc _anon_ {} { return {Literal} } to internalise it
	That means it WILL have a bytes value, but test for safe zone

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

//	CIDENT LITERAL or tcl_word_maybe_not_IDENT
	static LITERAL_MATCHER match_GET( interp, "GET" );
	static LITERAL_MATCHER match_SET( interp, "SET" );
	static LITERAL_MATCHER match_array_set( interp, "array_set" );
	static LITERAL_MATCHER match_array_get( interp, "array_get" );
	static LITERAL_MATCHER match_LIST_ALL( interp, "LIST_ALL_OBJ_TYPE" );

//	ARGV0
	static LITERAL_MATCHER match_OBJ( interp, "OBJ" );
	static LITERAL_MATCHER match_VECT( interp, "VECT" );

//	PUNCT not CIDENT LITERAL
	static LITERAL_MATCHER match_dash( interp, "-" ); // seems fine
	static LITERAL_MATCHER match_EMPTY( interp, "" ); // 
	static LITERAL_MATCHER match_NEW( interp, "NEW" ); // 

//	problematic // because {} // at depth N
	static LITERAL_MATCHER match_curly_pair( interp, "{}" );
	// no matches with {{}}


  //	if( decoder->test(interp) ) return TCL_OK;

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
	// OPTION [VECT] IS NEW NO ARGS // CAN CHANGE NAME NOT IDX

	if( objc < 1 ) {
		return OBJ_usage_error( interp, objc, objv );
	}

	// objv[0] == ~ "OBJ" or something
	Tcl_Obj * _CMD_NAME = objv[0]; // 
	TCL_PTR  CMD_NAME = objv[0]; // 
	CMD_NAME.print_tcl_obj( "CMD_NAME .1 of OBJ A R G S");

	bool CMD_OBJ = false;
	bool CMD_VECT = false;

	if( match_OBJ.MATCHES(CMD_NAME) ) {
		CMD_OBJ = true;
		INFO("GOT OBJ\n");
	}
	else

	if( match_VECT.MATCHES(CMD_NAME) ) {
		CMD_VECT = true;
		INFO("GOT VECT\n");
	}

	else {
		FAIL("not OBJ not VECT stays '%s'", CMD_NAME->bytes );
	}

	CMD_NAME.print_tcl_obj( "CMD_NAME .2 of OBJ A R G S");

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
	if(objc < 2 ) {
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

	Tcl_Obj * cmd = objv[2];

	/*
		LOOK at obj_id
		FETCH obj from OBJ sto
		TODO
	*/


	if( !obj_id ) {
		obj_id_EMPTY = true;

	} else {
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
	
	if( obj_id_EMPTY ) {
	 switch( objc ) {
	  case 0:	// impossible
	  case 1:	// OBJ
	  case 2:	// OBJ $id // id == {} or NEW

		   if( CMD_OBJ) { // set V [OBJ NEW] // set X [OBJ {}] 


			if(!decoder->new_OBJ_DICT( interp, & NEW_VAL ))
				return FAIL_FAILED();

			Tcl_SetObjResult( interp, NEW_VAL );
			PASS("DONE");
			return TCL_OK;
		   }
		   if( CMD_VECT) { // set V [VECT NEW] // set V [VECT {}] 

			if(!decoder->new_OBJ_VECT( interp, & NEW_VAL ))
				return FAIL_FAILED();

			Tcl_SetObjResult( interp, NEW_VAL );
			PASS("DONE");
			return TCL_OK;
		   }
		   return OBJ_usage_error( interp, objc, objv );
	  break;
	  case 3:	// OBJ $id OPCODE // VECT $id OPCODE
	  break;

	 }
	}

	/*
		LOOK at cmd
		cmd REALLY should be a Literal OPCODE
		TODO
	*/


	switch( objc ) {
	 case 0:	// impossible
	 case 1:	// OBJ
	 case 2:	// OBJ $id
		return OBJ_usage_error( interp, objc, objv );
	 break;
	 case 3:	// OBJ $id OPCODE // VECT $id OPCODE

	 	if(obj_id_EMPTY) { // OBJ {} NEW // VECT {} NEW // OBJ NEW VECT


			if( match_VECT.MATCHES(cmd) ) {
				INFO("GOT VECT\n"); // OBJ NEW VECT // VECT {} VECT

				return TCL_OK;
			}
		}

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

	token=Tcl_CreateObjCommand(
		interp,
		"VECT", // VECT new //
/* CAST */ (Tcl_ObjCmdProc*)
		OBJ_OBJ, // keep to one CXX function OBJ_OBJ decoder // etc
/* CAST */  (ClientData)
		decoder,
		deleteProc // VECT.ClientData == OBJ.CLientData == MOD_OBJ
	);

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

		cmdName == "cmd" == "ARGV0 -> typeName == "cmdName"
			EA = ARGV0
			bytes == "cmd"
	
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

					SUBLEX "cmd" Tcl_Obj_typePtr_name_cmdName


	*/

	return TCL_OK;
}

