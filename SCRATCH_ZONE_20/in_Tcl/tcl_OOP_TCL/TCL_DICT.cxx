#include "TCL_DICT.h"
#include "TCL_HELP.h" // needs <tcl.h>

	bool TCL_DICT:: test(  Tcl_Interp * interp )
	{
		int pos;
		TCL_PTR_t k1;
		TCL_PTR_t k2;
		TCL_PTR_t k3;
		TCL_PTR_t v1;
		TCL_PTR_t v2;
		TCL_PTR_t v3;
		TCL_PTR_t val;

		k1 = Tcl_NewStringObj( "k1", -1 ); // 
		v1 = Tcl_NewStringObj( "one", -1 ); //
		SET( interp, k1, v1 );

		k2 = Tcl_NewStringObj( "k2", -1 ); // 
		v2 = Tcl_NewStringObj( "two", -1 );
		SET( interp, k2, v2 );

		k3 = Tcl_NewStringObj( "k2", -1 ); // 
		v3 = Tcl_NewStringObj( "three", -1 );
		SET( interp, k3, v3 );

		GET( interp, k1, val );
		fprintf(stderr,"DICT[%s] == %s\n", k1.str(), val.str() );

		DEL( interp, k1 );

		if(!GET( interp, k1, val )) return false;

		GET( interp, k1, val );
		fprintf(stderr,"DICT[%s] == %s\n", k1.str(), val.str() );

		return true;
	}
