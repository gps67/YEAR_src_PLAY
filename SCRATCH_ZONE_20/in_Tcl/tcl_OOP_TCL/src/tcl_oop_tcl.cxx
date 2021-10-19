#include <tcl.h> /* TCL */

#include "OBJ_decoder.h"

// some struggling attempt to parametise the module anme
// CAP1 makes it tricky
// ## makes it very tricky // failed // just write it 3 times
// the namespace of the same name is not expected, except by me
// the package of .. what name ... ABSENT would permit
// package require Optical // just put the load in there ?

#define MODULE_NAME_CAP1 Optical
#define MODULE_NAME_low1 optical
#define MODULE_NAME_low1_str "optical"

#define MODULE_INIT( mod_name ) int mod_name ##  _Init( Tcl_Interp *interp )
#define SPACE_1_NAME( name ) MODULE_NAME_low1_str "::" name

/*
	The module _Init function that Tcl finds and calls 

	        load ./tcl_oop_tcl.so Optical	;# Optical_Init

	It must be named %s_Init MODULE_NAME_CAP1

	        load ./tcl_oop_tcl.so 		;# Tcl_oop_tcl_Init

	In all cases the loader forces a CAP1

	        load ./tcl_oop_tcl.so optical	;# Optical_Init
	
	It must be declared extern "C" // if CXX
	It gets the parameter (Tcl_interp * interp)
	It return TCL_OK

	extern "C" int Optical_Init( Tcl_interp * interp ) ...

SAFE_MODULE == TBS
	
TRUSTED MODULE

	It defines all the things that it wants defined, etc

	You could also add the pkg_provide bits, but 'load' does
	a similar effect itself.

	By using "optical::OBJ" as the function name
	the namespace is auto_created.
*/

extern "C"
// int Optical_Init( Tcl_Interp *interp )
// NO // int MODULE_NAME_CAP1 ##_Init( Tcl_Interp *interp )
// OK // MODULE_INIT( Optical ) // Optical_Init
// NO // MODULE_INIT( MODULE_NAME_CAP1 ) // MODULE_NAME_CAP1_Init
MODULE_INIT( Optical )
{

#if 0
	static OBJ_MODULE MODULE( interp );
	static OBJ_MODULE * MOD = & MODULE;
//	static OBJ_MODULE * MOD = new OBJ_MODULE( interp );

	if(!MOD) {
		Tcl_AppendResult(
			interp,
                        "CXX_new_OBJ_MODULE",
                        ": CTOR returned NULL",
                        (char *) NULL);
                return TCL_ERROR;

	}
#endif


	OBJ_decoder * decoder = new OBJ_decoder( interp );

	if( TCL_OK != declare_OBJ_functions( interp, decoder ))
	 return TCL_ERROR;

	return TCL_OK;
}

