#include "TCL_STUBS.h"
#include "TCL_PTR.h"
#include "TCL_HELP.h" // P64

using namespace TCL;

TCL_PTR:: operator const char * ()
{
	if(!PTR) {
		return "(NULLPTR)";
	}
	if(!PTR->bytes) {
		return "(NULL bytes)";
	}

	return PTR->bytes;
//	return Tcl_GetStringFromObj( PTR, &len );
}

/*
	print the refcount and outher fields

	remove from TCL_HELP
*/
void TCL_PTR:: print_tcl_obj( const char * str )
{
	if(!str) str = "!";
//	fprintf(stderr,"\n");
//	fprintf(stderr,"tcl_obj              0x%lx\n", (long) PTR );
	fprintf(stderr,"tcl_obj --------->   %s  %s\n", P64( PTR ), str );
	fprintf(stderr,"tcl_obj refCount     %d\n", ref_count() );
	fprintf(stderr,"tcl_obj bytes %4ld   %s\n", PTR->length, PTR->bytes );
	fprintf(stderr,"tcl_obj typePtr.name %s\n", get_type_name() );
//	fprintf(stderr,"tcl_obj PTR1         0x%lx\n", (long) PTR->internalRep.twoPtrValue.ptr1 );
//	fprintf(stderr,"tcl_obj PTR2         0x%lx\n", (long) PTR->internalRep.twoPtrValue.ptr2 );
	fprintf(stderr,"tcl_obj PTR1         %s\n", P64( PTR->internalRep.twoPtrValue.ptr1 ));
	fprintf(stderr,"tcl_obj PTR2         %s\n", P64( PTR->internalRep.twoPtrValue.ptr2 ));
}

