#include "TCL_LIST_t.h"

	bool TCL_LIST_t:: test(  Tcl_Interp * interp )
	{
		int pos;
		TCL_PTR_t val;

		val = Tcl_NewStringObj( "one", 3 ); // auto IncrRef
		ADD( interp, &pos, val );

		val = Tcl_NewStringObj( "two", 3 );
		ADD( interp, &pos, val );

		val = Tcl_NewStringObj( "three", 5 );
		ADD( interp, &pos, val );

		val = Tcl_NewStringObj( "ZERO one", 8 );
		if( ! SET( interp, 0, val ) ) return false;

		if( ! GET( interp, 1, val ) ) return false;

		if( ! GET( interp, 0, val ) ) return false;

		if( ! GET( interp, 2, val ) ) return false;

		if( ! GET( interp, 3, val ) ) return false;

		return true;
	}
