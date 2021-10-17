#include "TCL_PTR.h"

	TCL_PTR:: operator const char * ()
	{
		if(PTR) {
			int len; // dropped
			return Tcl_GetStringFromObj( PTR, &len );
		} else {
			return "(NULL)";
		}
	}

