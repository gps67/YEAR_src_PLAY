#include "TCL_PTR_t.h"

	TCL_PTR_t:: operator const char * ()
	{
		if(PTR) {
			int len; // dropped
			return Tcl_GetStringFromObj( PTR, &len );
		} else {
			return "(NULL)";
		}
	}

