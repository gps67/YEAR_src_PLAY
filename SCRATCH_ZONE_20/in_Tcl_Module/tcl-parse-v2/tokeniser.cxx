#include <tcl.h>
#include "tokeniser.h"

// ----------------------------------------

void tokeniser_del( tokeniser * t )
{
  /*
	when Tcl destroys the command/object, it calls this
	so dispose of the object. Note that the function
	prototype is actually CAST into ClientData.

	If you want to delete the object, you can,
	but check again if you get a core. Basically after
	delete t, you must not do anything with t (not even check
	t->interp), but you can flag it to null and recursively
	call Tcl Delete Command, (Tcl known about already deleted)
	THEN delete t. However its easier to simply call:

	Tcl_DeleteCommandFromToken( t->interp, t->tcl_command );
  */

	if( t->tcl_command == NULL ) return;	// t is itself still valid

	Tcl_Command command = t->tcl_command;
	t->tcl_command = NULL;
	Tcl_DeleteCommandFromToken( t->interp, command );
	// in t destructor, t->tcl_command is NULL, BUT INTERP IS AVAIL
	delete t;
}

int tokeniser_call (
	tokeniser * t,
	Tcl_Interp *interp_client,
	int objc,
	Tcl_Obj *CONST objv[]
);

int tokeniser_new (
	ClientData loader_clientData,
	Tcl_Interp *interp_client,
	int objc,
	Tcl_Obj *CONST objv[]
) {
  /*
	loader_clientData  comes from when this function was defined 
	by the module-loader, possibly from before if it was buried safe
	here it is ignored - a new ClientData for a new fn IS setup
	You might want the t object to hold that ClientData (module)
  */
  /*
	interp_client  is the (possibly safe) interp that is this script,
	where this function/object will be available. It will ALWAYS
	be passed, as exactly the same pointer, every time our command
	is called, but keep a copy to save passing parameters everywhere.
  */
  /*
	objc/objv  is this commands name and parameters as Tcl Objects
	There should be one parameter - the name of the function to create
	ie much like canvas .c, but tokeniser::new parser1
  */
	if( objc != 2 ) {
		Tcl_AppendResult( interp_client,
			"Usage: ",
			Tcl_GetString( objv[0]),
			"new_object_proc_name\n",
			0
		);
		return TCL_ERROR;
	}

  /*
	cmd_name  is user defined
	your safe env might not allow that directly
	though arguably thats not dangerous in a safe env
	you could enforce a namespace where user can do such things
	and set that when the library loads the module, or other
  */
	char * cmd_name = Tcl_GetString( objv[1] );

  /*
	cmd_fn  is the objects interface
	it's initial parameter is the function to call
	that is checked in the usage table and the
	object member function gets called, eg

		parser1 parse_script $text

	results in:

		 t->parse_script( text );
  */
	Tcl_ObjCmdProc * cmd_fn = (Tcl_ObjCmdProc*) &tokeniser_call;

  /*
	cmd_obj  is the object itself
	It is a C++ object, which Tcl passes around as ClientData
	It is a Tcl aware object, because that what its used for
	It holds an interp for convenience (tokeniser_call
	actually checks that it has the correct value).
	It also holds the token (name) for the command tht it is.
	Here is is created from the provided parameters
  */
	tokeniser * t = new tokeniser( interp_client );
	ClientData cmd_obj = (ClientData) t;
	// eg malloc error
	if( t==NULL ) {
		Tcl_AppendResult( interp_client,
			" ",
			Tcl_GetString( objv[0] ),
			" new tokeniser( interp ) failed\n",
			0 
		);
		return TCL_ERROR;
	}

  /*
	cmd_del  is called by Tcl to delete the object
  */
	Tcl_CmdDeleteProc * cmd_del = (Tcl_CmdDeleteProc*) tokeniser_del;
  /*
	create the users function
  */
	t->tcl_command = Tcl_CreateObjCommand(
		interp_client,
		cmd_name,
		cmd_fn,
		cmd_obj,
		cmd_del
        );
	return TCL_OK;
}

/*
	here is some dreadful C++ syntax for
	pointers to member functions (simple struct class)
	see tokeniser_call for the using side

	The idea is have functions with a specific number of Obj * args
	or (if usage[i].max == -1) keep the objc/objv parameters.

	This code auto checks the NUMBER of parameters, with a standard
	Tcl usage message. The receiver has to check the type/value
	of the parameter
	
*/

	typedef int (tokeniser::* cast_fn_0)( void );
	typedef int (tokeniser::* cast_fn_1)(
		Tcl_Obj * CONST obj1
	);
	typedef int (tokeniser::* cast_fn_2)(
		Tcl_Obj * CONST obj1,
		Tcl_Obj * CONST obj2
	);
	// add more here ...
	typedef int (tokeniser::* cast_fn_objv)(
		int objc,
		Tcl_Obj *CONST objv[]
	);

/*
	VFN is a short-circuit cast,
	so be careful to ensure that min/max correctly
	specifies the number of args expected.

	min is only really usful for objv functions's which really
	take N + more args, or allow extra -flags. Fixed args
	have min==max

	VFN is a cast_fn_objv = { argc, argv } of STR0 || TOKEN || OBJECT || API
	VFN is ARGV of Tcl_Obj *
	The other forms, are {
	 += CALLABLE
	 += TOKEN
	 += STO
	 += SCRIPT
	 += UDEF_PLUS
	}
	UDEF_MODULE { SCRIPT } SCRIPT is a MODULE += DIALECT += SESSION 


	// ASIDE
	DECODE streaming_ASM_CPU_64_lohi
	 EA_ITEM
	 u64_lohi_WORD_ADDR_EXPR_IDX_OFFS_CSR_TOKEN_SYMBOL_LABEL has_u16_OPCODE
	 u16_lohi
	 u48_DATA
	 u64_lohi
	  _WORD
	  _ADDR
	  _EXPR
	  _IDX
	  _OFFS
	  _CSR
	  _TOKEN
	  _SYMBOL
	  _LABEL 

	   _BITFIELD_ ...
	   _u16_OPCODE
	   _u48_PAYLOAD

	   	u16_OPCODE
		u16_LHS
		u16_RHS
		u16_PLUS

		That fits FOUR u16 items per cpu_var_WORD
		But limits world to u128 _ITEMS[u16]
		or next level DECODE of u64_WORD_u64_PLUS // _lohi // WORD_PAIR
		{
			u64_as_EA_EXPR // in Module
			u64_as_EA_PLUS // DATA for EA_EXPR

			u64_as_u64_lohi // and NOT as PAIR
			// ( PAIR is for PARSED_STREAM_OPCODE_ARGV )
			// ARGV // u64_argc; ITEM_t * item
			// CLASS // PTR = WORD >> 16 // u64_CAST_USES_PTR

			ask_OPCODE_nbytes_for_csr() { return DECODED_VALUE }
			// LAZY EVAL CACHES DECODED_VALUE
			// Bench DECODED += DECODER
			// P0P2 over u64_WORD and u64_WORDS_PLUS
		}
		SESSION = array [ u64 ] of u256 // FOUR x u64_WORD

	   	 u64_OPCODE
		 u64_LHS
		 u64_RHS
		 u64_PLUS

		TUPLO5 OPCODE LHS RHS PLUS
		 u64_lohi WORD; // EXPR
		 // OPTION CodePoint == PC == IP _POS_ _CSR_
		 // ie CTXT_SCRIPT_CODE_POINT sublexes to
		 // ie cpu_var_CTXT -> CTXT_t 
		 // #define CTXT cpu_var_CTXT // _t
		 // _t is an api elf _in_chroot_api_t

		 TUPLOX OPCODE UNARY // UNARY_API_CACHE
		  u8_u8 OPCODE
		  u48_u16 PAYLOAD_u64_lohi // ALIAS THIS // 

	//

	// TODO // -1,0,+1 TOKENS used
	// -1 looks like FAIL
	// +1 looks like PASS
	// 0 looks line MATCH_HALF opcode == check( CTXT, 

	// is there a spare WORD in Rcl_Interp *

*/

typedef cast_fn_objv VFN; // VFN = ARGV = argc, argv, u64_WORD, u256_FOUR_WORD

struct opcode_usage {
	const char * opcode; 		// REPLACE CACHE WITH Tcl_Obj * MY_STR_t
	int min;		// tcl_obj_type = Module_2_type_4
	int max;		// which is SYMBOL is TOKEN is STR0
	const char * usage;	// WHICH IS ALSO pre_looked_up
	VFN fn;

	int check( Tcl_Interp * interp, char * word, int n ) {
		// we know caller is sweeping other opcodes
		// quickly return 0 for NO_MATCH
		if( 0!=strcmp(opcode,word) )
			return 0; // NO_MATCH

		int max2 = max; // locally varied copy
		if( max2 == -1 ) max2 = n; // -1 axxepts ANY as EXACT
		if( (n < min) || (n>max2) ) {
			Tcl_AppendResult(
				interp,
				"Usage: ",
				opcode,
				" ",
				usage,
				NULL );
			// BAD USAGE
			return -1;
		}
		// MATCHED opcode_str // ALL GOOD 
		return 1;
	}
};

int tokeniser_call (
	tokeniser * t,
	Tcl_Interp *interp_client,
	int objc,
	Tcl_Obj *CONST objv[]
) {
	// Tcl has evolved some new clutch of interpreter CTXT_t 
	// RTFM for safe_mode_usages
	// for now say what is seen // if not same
	if( t->interp != interp_client ) {
		Tcl_AppendResult(interp_client,
			"INTRNAL ERROR: interp mismatch",
			NULL );
		return TCL_ERROR;
	}

	// ARGV  _CSR_ THIS_FUNC opcode ...
	// slide ARGV past THIS_FUNC // ? //
	Tcl_Obj * obj_func = objv[0];
	objc --;
	objv ++;

	// ARGV  THIS_FUNC _CSR_ opcode ...
	if( objc == 0 )
	{
		Tcl_AppendResult(interp_client,
			"Usage: $tokeniser opcode ... args ...", NULL );
		return TCL_ERROR;
	}

	// ARGV  THIS_FUNC _CSR_ opcode ...
	Tcl_Obj * obj_opcode = objv[0];
	objc --;
	objv ++;

	// ARGV  THIS_FUNC opcode _CSR_ ...

/* only parse_script has any code - at present */

	opcode_usage usages[] = {
{ "parse_script",  1, 1, "script",  (VFN)(& tokeniser::parse_script) },
{ "parse_command", 1, 1, "command", (VFN)(& tokeniser::parse_command) },
{ "parse_expr",    1, 1, "expr",    (VFN)(& tokeniser::parse_expr) },
{ "parse_var",     1, 1, "var",     (VFN)(& tokeniser::parse_var) },
{ "parse_braces",  1, 1, "braces",  (VFN)(& tokeniser::parse_braces) },
{ "parse_quotes",  1, 1, "quotes",  (VFN)(& tokeniser::parse_quotes) }
	};
	int n = sizeof( usages ) / sizeof( usages[0] );

	// look for opcode in above table
	char * opcode = Tcl_GetString( obj_opcode );
	for( int i = 0; i < n; i++ ) {
		switch( usages[i].check( t->interp, opcode, objc )) {
		case -1:
			// check() is so upset, that we FAIL
			// eg you passed wrong # of ARGS
			// there is no clecer PLUS args (yet)
			// RTFM setting ERROR as AppendResult
			return TCL_ERROR;
		case 0:
			// opcode == obj_opcode -> STR_PTR_IS_SET || func
			// A = PTR ;
			// if(!A) CALL Tcl_GetString 
			// we keep a ref, and steal it's internal STR0
			// we could UPGRADE Tcl's item
			// we could BOUNCE to Tcl's SPELLING from OURS
			// we could UPGRADE our item to share Tcl's (keep ref)
			// opcode == possibly borrowed temp
			continue; 
		case 1:
			
			// LOG SOMETHING
			printf("# check( interp, OPCODE=%s, objc=%d )"
				" returned 1 \n",
				opcode, objc ) ; 
			fflush(0);
			// WARN not FAIL

			// of options { -1 0 1 }
			// all the others continued to next i
		}

		// CASE 1 == MATCH FOUND

		// check( argc )
		// and prepare fast FFI call
		switch( usages[i].max ) {
		case -1 : {
			// ARGV passed straight through
			cast_fn_objv fn = (cast_fn_objv) usages[i].fn;
			return (t->*fn)( objc, objv );
		} case 0: {
			cast_fn_0 f0 = (cast_fn_0) usages[i].fn;
			return (t->*f0)();
		} case 1: {
			cast_fn_1 f1 = (cast_fn_1) usages[i].fn;
			return (t->*f1)( objv[0] );
		} case 2: {
			cast_fn_2 f2 = (cast_fn_2) usages[i].fn;
			return (t->*f2)( objv[0], objv[1] );
		} }
		Tcl_AppendResult( interp_client,
			"usages[i].max exceeds 2", NULL );
		return TCL_ERROR;
	}
	// POSS // opcode was never found // i is outside table
	Tcl_AppendResult(interp_client,
		"bad opcode: ",
		opcode,
		NULL );
	return TCL_ERROR;
}
// ----------------------------------------
Tcl_Obj * tokeniser::bug_scan_variable_tokens( int n )
{
	Tcl_Obj * varname = conv_next_token();
	Tcl_Obj * index = NULL;
	if(n == 1)
		index = mk_string( "-NO-INDEX-" );
	else
		index = conv_next_token();
	return mk_list2( varname, index );	
}

Tcl_Obj * tokeniser::conv_next_token()
{
	Tcl_Obj * type = NULL;
	Tcl_Obj * text = NULL;
	Tcl_Obj * subs = NULL;
	if( csr1 >= csr1_end_pos )
	{
		const char * s = "hit csr1_end_pos";
		printf("hit csr1_end_pos" ); fflush(0);
		retval( mk_string( s ));
		return NULL;
	}
	Tcl_Token * token = csr1++;
	int n = token->numComponents;
	// csr is updated by recursive calls
	// printf("got TCL_TOKEN %d\n", token->type); fflush(0);
	switch( token->type ) {
	case TCL_TOKEN_WORD :
		type = mk_string( "TCL_TOKEN_WORD" );
		break;
	case TCL_TOKEN_SIMPLE_WORD :
		type = mk_string( "TCL_TOKEN_SIMPLE_WORD" );
		break;
	case TCL_TOKEN_TEXT :
		type = mk_string( "TCL_TOKEN_TEXT" );
		break;
	case TCL_TOKEN_BS :
		type = mk_string( "TCL_TOKEN_BS" );
		break;
	case TCL_TOKEN_COMMAND :
		type = mk_string( "TCL_TOKEN_COMMAND" );
		break;
	case TCL_TOKEN_VARIABLE :
		// definitely a TCL design bug
		// sub-components are counted here!
		// by now its too late
		// TCL_TOKEN_WORD has the wrong count
		// hence switching to new counting (while csr)
		// n--;
		type = mk_string( "TCL_TOKEN_VARIABLE" );
		break;
	case TCL_TOKEN_SUB_EXPR :
		type = mk_string( "TCL_TOKEN_SUB_EXPR" );
		break;
	case TCL_TOKEN_OPERATOR :
		type = mk_string( "TCL_TOKEN_OPERATOR" );
		break;
	default :
		printf("Unrecognised TCL_TOKEN %d\n", token->type);
		fflush(0);
		printf("with text: %s\n", token->start);
		fflush(0);
		type = mk_string( "TCL_TOKEN_not_regognised" );
		break;
	}
	text = mk_string( token->start, token->size );
	subs = Tcl_NewListObj( 0, NULL );

if( 0 )
printf("+ %d %d %s - %s \n", n, token->type, Tcl_GetString(type), Tcl_GetString( text )); fflush(0);

	Tcl_Token * csr2_end_pos = csr1 + n; // + 1 already in csr1

	while( csr1 < csr2_end_pos )
	{
		mk_list_append(
			subs,
			conv_next_token()
		);
	}

	return mk_list3(
		type,
		text,
		subs
	);
}

Tcl_Obj * tokeniser::conv_next_word()
{
	return conv_next_token();
}

Tcl_Obj * tokeniser::conv_word_list()
{
	Tcl_Obj * list = Tcl_NewListObj( 0, NULL );

	int i = parsed.numTokens;
	int w = parsed.numWords;
	// csr1 climbs down words
	csr1 = parsed.tokenPtr;
	csr1_end_pos = csr1 + i;

	while( w > 0 )
	{
		w--;
		if( csr1 >= csr1_end_pos ) {
			printf("# ERROR csr past end of list\n");
			fflush(0);
			break;
		}
		Tcl_Obj * item = conv_next_word();
		if(NULL==item) return NULL;
		mk_list_append( list, item );
	}
	return list;
}

Tcl_Obj * tokeniser::parse_one_command(int with_gaps )
{
	with_gaps = 0; /* not working */
	do_free();
/*
	Tcl_Obj * gap1 = NULL;;
	Tcl_Obj * cmnt = NULL;;
	Tcl_Obj * gap2 = NULL;;
	Tcl_Obj * cmd_str = NULL;;
	Tcl_Obj * cmd_argv = NULL;;
	Tcl_Obj * gap3 = NULL;;

	Tcl_Obj * list_list_token = NULL;;
*/
	must_free = 1;
	int nested = 0;
	int t = Tcl_ParseCommand( interp,
		remain_str,
		remain_len,
		nested,
		&parsed
	);
	if( TCL_ERROR == t ) { return NULL; }
	
	Tcl_Obj * cmnt = Tcl_NewStringObj(
		parsed.commentStart,
		parsed.commentSize
	);
	Tcl_Obj * cmd_str = Tcl_NewStringObj(
		parsed.commandStart,
		parsed.commandSize
	);
	Tcl_Obj * cmd_argv = conv_word_list();
	if(NULL==cmd_argv) return NULL;
	remain_str = parsed.commandStart + parsed.commandSize;
	remain_len = script_str + script_len - remain_str;
	Tcl_Obj * list [] = {
		cmnt,
		cmd_str,
		cmd_argv
	};
	int n = sizeof( list ) / sizeof( list[0] );
	Tcl_Obj * ret_val = Tcl_NewListObj( n, list );
	return ret_val;
//	retval( ret_val );
//	return t;
}

int tokeniser::parse_script( Tcl_Obj * str )
{
	int t = TCL_OK;
	do_free();
	set_script( str );
	// to initialise from first try passong one as pointer to it in list
	Tcl_Obj * cmd_list = Tcl_NewListObj( 0, NULL );
	
	while( remain_len )
	{
		Tcl_Obj * cmd = parse_one_command();
		if( NULL == cmd ) {
			// most Tcl_Objs auto_free themselves
			// by checking list just before next VM command tick
			return TCL_ERROR;
		}
		t = mk_list_append( cmd_list, cmd );
	}
	retval( cmd_list );
	return TCL_OK;
}
int tokeniser::parse_command( Tcl_Obj * str )
{
	return TCL_OK;
}
int tokeniser::parse_expr( Tcl_Obj * str )
{
	return TCL_OK;
}
int tokeniser::parse_var( Tcl_Obj * str )
{
	return TCL_OK;
}
int tokeniser::parse_quotes( Tcl_Obj * str )
{
	return TCL_OK;
}
int tokeniser::parse_braces( Tcl_Obj * str )
{
	return TCL_OK;
}
 
// ----------------------------------------

void tokeniser::set_script( Tcl_Obj * str)
{
	int len=0;
	char * s = Tcl_GetStringFromObj( str, &len);
	set_script( s, len );
}

void tokeniser::set_script( const char * str, int len ) {
	do_free();
	script_str = str;
	script_len = len;
	remain_str = str;
	remain_len = len;
};

tokeniser::tokeniser( Tcl_Interp *interp1 ) { 
	interp = interp1;
	tcl_command = NULL;
	must_free = 0;
	set_script( "!", 0 );
}
void tokeniser::do_free( void ) {
	if( must_free ) { // thread LURK atomic
		must_free = 0;
		Tcl_FreeParse( &parsed );
	}
};

tokeniser::~tokeniser( void ) {
	do_free();
}

/*
	generic Tcl hooks to drive ANY_SCRIPT
*/
Tcl_Obj * tokeniser::mk_string( const char * s, int len )
{
	return Tcl_NewStringObj( s, len );
}

Tcl_Obj * tokeniser::mk_string( const char * s )
{
	return Tcl_NewStringObj( s, strlen(s) );
}
Tcl_Obj * tokeniser::mk_list0()
{
	return Tcl_NewListObj( 0, NULL );
}
Tcl_Obj * tokeniser::mk_list1( Tcl_Obj * a )
{
	return Tcl_NewListObj( 1, &a );
}
Tcl_Obj * tokeniser::mk_list2( Tcl_Obj * a, Tcl_Obj * b )
{
	Tcl_Obj * list2[] = { a, b };
	Tcl_Obj * list = Tcl_NewListObj( 2, list2 );
	return list;
}
Tcl_Obj * tokeniser::mk_list3( Tcl_Obj * a, Tcl_Obj * b, Tcl_Obj * c )
{
	Tcl_Obj * list3[] = { a, b, c };
	Tcl_Obj * list = Tcl_NewListObj( 3, list3 );
	return list;
}
int tokeniser::mk_list_append( Tcl_Obj * list, Tcl_Obj * item )
{
	return Tcl_ListObjAppendElement( interp, list, item );
}

 

int tokeniser::retval( Tcl_Obj * list )
{
	Tcl_SetObjResult( interp, list );
	return TCL_OK;
}

int tokeniser::retval_error( const char * msg )
{
	Tcl_SetResult( interp, (char *) msg, NULL );
	return TCL_ERROR;
}

int tokeniser::retval_error( Tcl_Obj * msg )
{
	Tcl_SetObjResult( interp, msg );
	return TCL_ERROR;
}
