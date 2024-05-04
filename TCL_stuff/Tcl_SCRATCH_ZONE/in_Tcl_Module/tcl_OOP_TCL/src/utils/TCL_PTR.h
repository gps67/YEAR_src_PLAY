#ifndef TCL_PTR_H
#define TCL_PTR_H

#include "TCL.h"


namespace TCL {
/*!
	TCL_PTR
	a plain Tcl_Obj *

	TCP_REF is based on this, it keeps a REF

*/
struct TCL_PTR { // TCL_OBJ

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
		/*
			LOWEST BASE CLASS does not know if HOLD AREF or not
			CLEANEST is ZERO_REF_EFFECT WORD_to_WORD_CPU_move

			IE we expect GEN to do the ref_count_hold_ing
			SO we ref_count_whenspecifically told to

			Regreattably CTOR lacks spelling variation
			just parameter prototype variation

			HOLD OBJ as a DUMB register CALLER GEN does ref_incr
		*/
	}

	Tcl_Obj * GET_OBJ() { // not a NULL one //
	#if 1
		if(!PTR) {
			FAIL("WARNING NULL Tcl_Obj should not happen");
			return NULL;
		}
	#endif
		return PTR;
	}
	
	void ref_incr()
	{
		INFO("ref_count was %ld '%s'", PTR->refCount, get_str() );
		if(PTR) Tcl_IncrRefCount( PTR );
	}
	
	void ref_decr()
	{
		INFO("ref_count was %ld '%s'", PTR->refCount, get_str() );
		if(PTR) Tcl_DecrRefCount( PTR );
	}
	
	int ref_count()
	{
		return PTR->refCount; // ZERO_means_EMPTY so_DELETE__unmentioned
	}
	bool is_shared()
	{
		return Tcl_IsShared( PTR ); // errm ,,, count is 2+
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
	Tcl_Size STR0_NN() { return GET_OBJ() -> length; }
	char * STR0_BYTES() { return GET_OBJ() -> bytes; } // NULL
	Tcl_Obj * PTR1_as_Tcl_Obj() { return (Tcl_Obj*) TCL_get_PTR1(GET_OBJ()); }
	Tcl_Obj * PTR2_as_Tcl_Obj() { return (Tcl_Obj*) TCL_get_PTR2(GET_OBJ()); }

// some of this stuff has misplaced itself into OBJ_module
// more that to TCL_PTR, eg TCL_get_PTR2
	void get_from_PTR2( Tcl_Obj * obj )
	{
//	#warning this is mangled, redo
//	//	set this.PTR to obj.PTR2
		PTR = (Tcl_Obj*) TCL_get_PTR2( obj );
		// do not ref_incr
	}

	void set_into_PTR2( Tcl_Obj * obj )
	{
//warning this is mangled, redo
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

