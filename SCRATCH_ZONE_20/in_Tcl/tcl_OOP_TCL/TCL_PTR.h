#ifndef PTR_keeper_H
#define PTR_keeper_H

#include <tcl.h>

struct TCL_PTR {

	Tcl_Obj * PTR;

	TCL_PTR ()
	: PTR( NULL )
	{
	}

	TCL_PTR ( Tcl_Obj * ptr )
	: PTR( ptr )
	{
		if(PTR) 
			Tcl_IncrRefCount( PTR );
	}

	~TCL_PTR ()
	{
		if(PTR) 
			Tcl_DecrRefCount( PTR );
		PTR = NULL; // debugging option
	}

	operator Tcl_Obj * ()
	{
		return PTR;
	}

	operator Tcl_Obj ** ()
	{
		return & PTR;
	}

	operator bool()
	{
		return PTR; // (PTR != NULL)
	}

	TCL_PTR & operator= ( Tcl_Obj * ptr )
	{
		if(ptr) 
			Tcl_IncrRefCount( ptr );
		Tcl_Obj * old = PTR;
		PTR = ptr;
		if(old) 
			Tcl_DecrRefCount( old );
		return *this;
	}

	operator const char * ();

	const char * str() { return * this; } // cast

};

#endif

