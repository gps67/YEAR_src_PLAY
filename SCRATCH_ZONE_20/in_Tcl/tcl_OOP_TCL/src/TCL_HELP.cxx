
#include <tcl.h>

#include "TCL_STUBS.h" // WARN
#include "TCL_HELP.h"

// this shows that "SET" in a script, appears as a NULL type string value

char * P64( void * PTR )
{
	static char msg[32];
	long P = (long) PTR;
	int D = P & 0xFFFF; P >>= 16;
	int C = P & 0xFFFF; P >>= 16;
	int B = P & 0xFFFF; P >>= 16;
	int A = P & 0xFFFF; P >>= 16;
	sprintf(msg,"%4.4X_%4.4X_%4.4X_%4.4X", A, B, C, D );
	return msg;
}

/*
	print the refcount and outher fields
*/
void print_tcl_obj( Tcl_Obj * obj )
{
//	fprintf(stderr,"\n");
//	fprintf(stderr,"tcl_obj              0x%lx\n", (long) obj );
	fprintf(stderr,"tcl_obj --------->   %s\n", P64( obj ) );
	fprintf(stderr,"tcl_obj refCount     %d\n", obj->refCount );
	fprintf(stderr,"tcl_obj bytes %4d   %s\n", obj->length, obj->bytes ? obj->bytes : "(NULL)" );
	fprintf(stderr,"tcl_obj typePtr.name %s\n", obj->typePtr ? obj->typePtr->name : "(NULL)" );
//	fprintf(stderr,"tcl_obj PTR1         0x%lx\n", (long) obj->internalRep.twoPtrValue.ptr1 );
//	fprintf(stderr,"tcl_obj PTR2         0x%lx\n", (long) obj->internalRep.twoPtrValue.ptr2 );
	fprintf(stderr,"tcl_obj PTR1         %s\n", P64( obj->internalRep.twoPtrValue.ptr1 ));
	fprintf(stderr,"tcl_obj PTR2         %s\n", P64( obj->internalRep.twoPtrValue.ptr2 ));
}

/*
	convert a string to a Tcl_Obj
*/
Tcl_Obj * mk_text( const char * text )
{
	// -ve len means upto NUL
	// zero refCount = not shared
	// NULL type == maleable
	return Tcl_NewStringObj( text, -1 );
}

/*
	convert a SHORT string to a shared common Tcl_Obj

	this is the same as a simple fixed string in a proc,
	not guaranteed though, but yes currently definitely,
	so maybe need second _ALIAS ability
	in case someone else manages to avoid the tokenisation
	even if we got it

	not sure what happens if we run before script, should be OK
	may need same interp ?
*/
Tcl_Obj * mk_common_spelling( Tcl_Interp * interp, const char * str )
{
	// -ve len means upto NUL
	// return Tcl_NewStringObj( str, -1 );
	// NO that retuned a NEW "GET" str with zero recount etc
	// Tcl_Eval // failed did not compile
	// Tcl_EvalEx // set _ %s // failed needs a PROC
	// Tcl_EvalEx // proc _x_ {} { return {%s} } ; _x_ // WORKED A TREAT

	char line[100];
	#if 0
	snprintf(line,sizeof(line)-1,"set _junk_var_ {%s}", str );
	snprintf(line,sizeof(line)-1,"set _junk_var_ %s", str );
	snprintf(line,sizeof(line)-1,"set _junk_var_ {%s}", str ); // RC 2
	snprintf(line,sizeof(line)-1,"set _junk_var_ %s", str ); // RC2
	snprintf(line,sizeof(line)-1,"proc _junk_fn_ {} { return %s } ; _junk_fn_", str );
	#endif
	snprintf(line,sizeof(line)-1,"proc _junk_fn_ {} { return {%s} } ; _junk_fn_", str );
	int t = 0;
	// OK Tcl_Eval does not compile the script
	int flags = TCL_EVAL_GLOBAL; // _DIRECT does not compile same
	flags = TCL_EVAL_DIRECT; // _DIRECT also worked
	flags = TCL_EVAL_GLOBAL; // _DIRECT does not compile same
	flags = 0; // 0 also worked
 	t = Tcl_EvalEx( interp, line, -1, flags );
 //	t = Tcl_Eval( interp, line ); // did not compile
 //	t = Tcl_Eval( interp, "_junk_fn_" ); // did not compile 
//	t = Tcl_ExprString( interp, line ); // cant remember
//	t = Tcl_ExprString( interp, str ); // OK but RC 0
	if( t == TCL_OK ) {
		return Tcl_GetObjResult(interp);
	}
	FAIL("Tcl_EvalEx %s failed \n", str );
//	FAIL("mk_common_spelling %s failed \n", str );

	return Tcl_NewStringObj( str, -1 );
}

