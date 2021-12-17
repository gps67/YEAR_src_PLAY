#include "TCL.h" /* TCL */

#include "OBJ_module.h"

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
using namespace TCL;

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
	// maybe move this to OBJ_module
	// maybe rename that TCL_module
	// or OBJ_module is a sub assembly of module
	// add ARGV_decoder
	// 

//	bool set_progname_argv0( Tcl_Interp * interp );
	// this uses "../tests/test_tcl_oop_tcl.tcl" not "tclsh"
	// which is what you want to know which SCRIPT it was
	// but that got resolved somehow, into /usr/bin/tclsh8.6
	// maybe gdb is bright // figured absent exe file // 
	// Reading symbols from +
	// + ../obj/tcl_oop_tcl.so // dlopen remembered it
	// + /home/gps/YEAR/src/PLAY/libs_apps/build_LGX_E6320/libbase.so

	// gdb likes to know // gdb_invoke uses it as exe_name
	// progname_argv0 = // where gdb_invoke wants it
	// "/tools/CTXT/tcl/bin/tclsh";
	set_prog_name( Tcl_GetNameOfExecutable() );
//	set_prog_name( "STUPID_CTXT_NAME" );

#if 0
	const char * varval = Tcl_GetVar( interp, "argv0", TCL_GLOBAL_ONLY );
	if( varval ) {
		INFO("varval %s is argv0 is the script filename", varval );
	}
#endif


	OBJ_module * decoder = new OBJ_module( interp );

	if( TCL_OK != declare_OBJ_functions( interp, decoder ))
	 return TCL_ERROR;

	int exact = 0;
	Tcl_PkgRequire(interp, "PLUS", "0.1", exact);
	// clientData for a Package means what ??? // OBJ_module ??
	// that is the clientData for the function
	// how to get for the package - would be slow //
	// require info probably tells // seems more Pkg info

	INFO("PLUS");
	gdb_invoke(false);


	return TCL_OK;
}

