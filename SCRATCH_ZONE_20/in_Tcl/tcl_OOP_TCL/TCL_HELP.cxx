
#include <tcl.h>

#include "TCL_HELP.h"

// this shows that "SET" in a script, appears as a NULL type string value

void print_tcl_obj( Tcl_Obj * obj )
{
	fprintf(stderr,"\n");
	fprintf(stderr,"tcl_obj              0x%lx\n", (long) obj );
	fprintf(stderr,"tcl_obj refCount     %d\n", obj->refCount );
	fprintf(stderr,"tcl_obj bytes %4d   %s\n", obj->length, obj->bytes ? obj->bytes : "(NULL)" );
	fprintf(stderr,"tcl_obj typePtr.name %s\n", obj->typePtr ? obj->typePtr->name : "(NULL)" );
	fprintf(stderr,"tcl_obj PTR1         0x%lx\n", (long) obj->internalRep.twoPtrValue.ptr1 );
	fprintf(stderr,"tcl_obj PTR2         0x%lx\n", (long) obj->internalRep.twoPtrValue.ptr2 );
}

