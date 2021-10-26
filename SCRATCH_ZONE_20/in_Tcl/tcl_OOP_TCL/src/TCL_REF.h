#ifndef TCL_REF_H
#define TCL_REF_H

#include "TCL_STUBS.h"
#include "TCL_PTR.h"
#include <tcl.h>

/*!
	a smart pointer to a Tcl_Obj

	nb this stops you using return (Tcl_Obj *)
	because the DTOR will delete the item before it arrives
	so you have to pass a { TCL_REF & RET_VAR } in
*/
struct TCL_REF : public TCL_PTR {

	TCL_REF ()
	: TCL_PTR( NULL )
	{
	}

	TCL_REF ( Tcl_Obj * ptr )
	: TCL_PTR( ptr )
	{
		if(ptr) ref_incr();
	}

	~TCL_REF ()
	{
		if(PTR) ref_decr();
		PTR = NULL; // debugging option
	}

#if 0
	operator Tcl_Obj * () const
	{
		return PTR;
	}

	Tcl_Obj * operator -> () const	// return pointer even though -> // ?
	{
		return PTR;
	}

	operator Tcl_Obj ** ()  // cant be const
	{
		return & PTR;
	}

	operator bool() // cant be const
	{
		return PTR; // (PTR != NULL)
	}
#endif

	TCL_REF & operator= ( Tcl_Obj * ptr )
	{
		if(ptr) 
			Tcl_IncrRefCount( ptr );
		Tcl_Obj * old = PTR;
		PTR = ptr;
		if(old) 
			Tcl_DecrRefCount( old );
		return *this;
	}

};

#endif

