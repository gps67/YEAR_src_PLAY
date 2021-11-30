#ifndef TCL_PTR_H
#define TCL_PTR_H

#include "TCL.h"


namespace TCL {
/*!
	a plain Tcl_Obj *

	TCP_REF is based on this, it keeps a REF

*/
struct TCL_PTR {

private:
protected: // avoid using *this = ptr // use PTR = ptr
	Tcl_Obj * PTR;
public:

	TCL_PTR ()
	: PTR( NULL )
	{
	}

	TCL_PTR ( Tcl_Obj * ptr )
	: PTR( ptr )
	{
	}
	
	void ref_incr()
	{
		INFO("ref_count was %d '%s'", PTR->refCount, get_str() );
		if(PTR) Tcl_IncrRefCount( PTR );
	}
	
	void ref_decr()
	{
		INFO("ref_count was %d '%s'", PTR->refCount, get_str() );
		if(PTR) Tcl_DecrRefCount( PTR );
	}
	
	int ref_count()
	{
		return PTR->refCount;
	}
	bool is_shared()
	{
		return Tcl_IsShared( PTR );
	}

	~TCL_PTR ()
	{
		// if we have a pointer to someone elses obj
		// it does not go out of scope when this does
		// so OK for debugging clear out registers
		PTR = NULL; // debugging option
	}

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

	TCL_PTR & operator= ( Tcl_Obj * ptr )
	{
		INFO("maybe this should not be here");
		#if 0
		// see TCL_REF operator=
		if(ptr) 
			Tcl_IncrRefCount( ptr );
		Tcl_Obj * old = PTR;
		PTR = ptr;
		if(old) 
			Tcl_DecrRefCount( old );
		#endif
		PTR = ptr;
		return *this;
	}

	void get_from_PTR2( Tcl_Obj * obj )
	{
		PTR = (Tcl_Obj*) obj -> internalRep.twoPtrValue.ptr2 ;
//		PTR = (Tcl_Obj*) TCL_get_PTR2( obj );
		// do not ref_incr
	}

	void set_into_PTR2( Tcl_Obj * obj )
	{
		obj -> internalRep.twoPtrValue.ptr2  = PTR;
//		TCL_set_PTR2( obj, PTR );
		// do not ref_incr
	}

	operator const char * ();

	const char * str() { return * this; } // cast
	const char * get_str() { return * this; } // cast // str is a busy ident

	const char * get_type_name()
	{
		if(!PTR) return "(NULL PTR)";
		if(!PTR->typePtr) return "(NULL TYPE)";
		if(!PTR->typePtr->name) return "(NULL TYPE NAME)";
		return PTR->typePtr->name; // never NULL // ATM
	}

	void print_tcl_obj( const char * str );

};

}; // namespace
#endif

