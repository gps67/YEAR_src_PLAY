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
*/

typedef cast_fn_objv VFN;

struct opcode_usage {
	char * opcode;
	int min;
	int max;
	const char * usage;
	VFN fn;

	int check( Tcl_Interp * interp, char * word, int n ) {
		if( 0!=strcmp(opcode,word) )
			return 0;
		int max2 = max;
		if( max2 == -1 ) max2 = n;
		if( (n < min) || (n>max2) ) {
			Tcl_AppendResult(
				interp,
				"Usage: ",
				opcode,
				" ",
				usage,
				NULL );
			return -1;
		}
		return 1;
	}
};

int tokeniser_call (
	tokeniser * t,
	Tcl_Interp *interp_client,
	int objc,
	Tcl_Obj *CONST objv[]
) {
	if( t->interp != interp_client ) {
		Tcl_AppendResult(interp_client,
			"INTRNAL ERROR: interp mismatch",
			NULL );
		return TCL_ERROR;
	}
	Tcl_Obj * obj_func = objv[0];
	objc --;
	objv ++;

	if( objc == 0 )
	{
		Tcl_AppendResult(interp_client,
			"Usage: $tokeniser opcode ... args ...", NULL );
		return TCL_ERROR;
	}
	Tcl_Obj * obj_opcode = objv[0];
	objc --;
	objv ++;

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
	char * opcode = Tcl_GetString( obj_opcode );
	for( int i = 0; i < n; i++ ) {
		switch( usages[i].check( t->interp, opcode, objc )) {
		case -1:
			return TCL_ERROR;
		case 0: continue; 
		}
		switch( usages[i].max ) {
		case -1 : {
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
		char * s = "hit csr1_end_pos";
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
