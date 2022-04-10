#include <tcl.h>
#include "tokeniser.h"
// include "module_tokeniser.h"

/*
	Parse_Library is several independently loadable modules
	With added control for WHEN they are loaded into interp
	MAY evolve to take two interps: controller client
	So for sake of ecperimentation you have to do: (tcl)

		load tcl_parser.so Parser_Library
		load {} tcl_parse
	
	Hence the doodling use of module class, which is there to
	register the loadability of tcl_parse. Your early scripts
	might as well have "safe" zones.

	tokeniser is ... see tokeniser .h

	module initialisation is here:
	Controlling Interpreter can pre-load and set special metrics
*/

extern "C" int
Parser_library_Init(
    Tcl_Interp *interp
);

// static module_tokeniser_loader & libr;

int
Parser_library_Init(
    Tcl_Interp *interp
)
{
	const char * name = "Tokeniser"; // load {} tokeniser
	printf( "Parser_library advertising: %s \n", name );
	fflush(0);
	Tcl_StaticPackage(
		interp,
		name,
		tokeniser_init_trusted,
		tokeniser_init_restricted
	);
	/*
		FIXABLE BUG -
		For some unknown reason the above never gets used properly
		hence always load the sub-module here
	*/
  if( 1 ) {
	/* this is necessary - due to my lack of understanding */
	tokeniser_init_trusted( interp );
	Tcl_AppendResult( interp, name, " initialised", NULL );
	printf( "# module force loaded by Parser_library_Init \n" );
	fflush(0);
  } else {
/* thinks - static load failed because no pkg-provide done */
	Tcl_PkgProvide( interp, "Tokeniser", "1.2" );
  }
	return TCL_OK;
}

int tokeniser_init_trusted( Tcl_Interp * interp_client ) {
	printf( "tokeniser_init_trusted() - which calls ... \n" );
	fflush(0);
	/* there is noting unsafe there ? */
	return tokeniser_init_restricted( interp_client );
}

int tokeniser_init_restricted( Tcl_Interp * interp_client )
{
	int t = TCL_OK;
	printf( "tokeniser_init_restricted - %d\n", t );
	fflush(0);
	// When module is loaded, it gets an instance
	// that is passed to all functions defined here (_new)
	// the new function itself, creates functions
	// they have another object as the client data

	// module_tokeniser * mod = new( object );
	int mod = 9876;
	ClientData cmd_obj = (ClientData) (long long) mod; // cast from 32 to 64
	// C is stupid, sizoeof(word) is 64 not 32 bit
	// for structs larger than 2G
	printf("sizeof int %ld sizeof ClientData %ld \n", sizeof(int), sizeof(ClientData) );
	Tcl_ObjCmdProc * cmd_fn = tokeniser_new;
	Tcl_CmdDeleteProc * cmd_del = NULL;
	
	// char * str = "tokeniser::";
	// Tcl_Obj * str1 = Tcl_NewStringObj( str, strlen(str) );
	// str = "new";
	// Tcl_AppendToObj( str1, str, strlen(str));
	// char * cmd_name = Tcl_GetString( str1 );

	const char * cmd_name = "::tokeniser::new";
	// const char * cmd_name = "tokeniser_new";

	Tcl_Command tcl_cmd = Tcl_CreateObjCommand(
		interp_client,
		cmd_name,
		cmd_fn,
		cmd_obj,
		cmd_del
	);
	return TCL_OK;
}
 


