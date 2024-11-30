#ifndef TCL_ObjType_PLUS_H
#define TCL_ObjType_PLUS_H

#include "TCL_ObjType_plain.h"
// DONE //	#include "TCL.h"
// DONE //	#include "TCL_STUBS.h" // INFO WARN FAIL
// DONE //	#include "TCL_HELP.h" // PTR1 PTR2

// #include "dgb.h" // FAIL

namespace TCL { 

// this is poly_essence (not polymorphic)
// local definition of GET_KEPT_PTR is used 
// not the one in this file

// if you change from DUP to (one of the 4 function pointers)
// you will also want to change this
// strong type makes it a sensible thing

typedef Tcl_DupInternalRepProc * KEPT_PTR_type;
// ie void (*) func( Tcl_Obj *src, Tcl_Obj *dst );


// PTR to this function is the actual KEPT_PTR
// ELF gives this VALUE as the address of the compiled C code for the function
// it cannot be undefined, without FAIL ing at module load time, or EXE load
// it is available as a PC relative ELF address, ie no MEM_BASE required, PC
// but that puts the stress on the CODE_as_const_DATA
// and a stupid back_jump, rather than jump_to // 
// it may or may not be stored in CLientData->kept_PTR
// which might be faster, than the ELF symbol WRT PC, or other
//
// the choice of this function is that it is carried by obj -> type ->.func
// also that now we know it is one of OUR TYPE we can ask_it_its_type
// 
// we do then have to do that task COPY dst = src 
// each TYPE_SPEC will need it's own COPY OBJECT 
// IDK 
// to find out what ... does RTFM Tcl_ObjType
// and also the new TCL9 stuff
// 
//  /tools/x86_64/tcl/_src/tcl9.0.0/doc/ObjectType.3 
//  /tools/x86_64/PREFIX/ bin lib share doc
//  /tools/x86_64/TOOLS // bin lib share doc
//  
//  I prefer that when Tcl is needed, to use an exact tcl/bin and it's tcl/lib
//  lots of symb links back to PREFIX means specific is 1 step away
//  Actually few LOOKUP_SO_PROVIDER
//
//

extern "C" 
void
PLUS_MYTYPE_DupInternalRepProc( Tcl_Obj *src, Tcl_Obj *dst );


// see the KEPT_PTR is that function
// this will look it up. 
inline
KEPT_PTR_type 
GET_KEPT_PLUS_PTR_GLOBAL() {
	INFO("obtaining KEPT_PTR");
	return PLUS_MYTYPE_DupInternalRepProc;
}

// MYTYPE is special but PLAIN_ cannot use _PLUS until it KNOWS
// the method of telling is used GENERALLY is_MYTYPE
// obj -> tyPtr // NULL means plain stringy type
// obj -> tyPtr -> dupIntRepProc == PLAIN_MYTYPE_DupInternalRepProc
//
// You can also take that pointer and put it in CLientData -> some_field
// that is faster because we dont have to resolve pointer to DLL code
// and because we have CLientData in a register already (Command OBJ)
//
// This function is still good, Tcl might call it, but I am not sure when
// it already KNOWS that it's typePtr is a _PLUS // because it was called
// it goes from TCL
// to PLUS_MYTYPE_DupInternalRepProc 
// that casts it's typePtr to TCL_ObjType_PLUS  // not VTBL
// that then reads a tag to see which GENRE of EXTN it is


/*
	ERRM
		giving this a VTBL
		requires convertion to and from 
			Tcl_ObjType
			TCL_ObjType_PLUS
		also requires no free
		but we never free a _PLUS

		get_typePtr_from_PLUS()
			a simple cast gets CXX to do it
		get_PLUS_from_typePtr()
			uses awful &first field
			offsetof not working (also awful)

		That is a simple addition

*/
struct TCL_ObjType_PLUS : TCL_ObjType_plain
{
	// Tcl_ObjType is the plain C Tcl declaration
	// TCL_ObjType_PLUS is our VTBL derived

	// C++ is wrong to put VTBL in front of plain STRUCT
	// it does not cost much to put VTBL in the middle
	// that would keep all pointers compat

	virtual ~TCL_ObjType_PLUS()
	{
		set_funcs_NULL(); 
		INFO("UNCALLED DTOR");
	}

	// OPTION // TCL_ObjType_BASIC += first field PTR_TO_PLUS_MGR
	// OPTION // then put _BASIC mid way through _PLUS _MGR
	// offset compute from sub to surround // less adj naff //

 // pretty printing require CTOR to set up good defaults

	const char * alias_one_ABB; // when not UDEF_25 UDEF_LEX1 or "LEX1"
	// "LEX1"

// UNUSED starts HERE
	const char * alias_one_LONG;
	// "UDEF_25_LEX1" // LEX1 is also an STR4 // with NUL


	/*
		now _PLUS attempts to describe the extra features
		eg PTR2_is_Tcl_Obj - and needs refcount
		eg PTR2_is_Tcl_Obj - but without refcount
	*/
	bool has_DICT_of_KEY_VAL; // ie user added fields { KEY VAL }

	bool bytes_never_NULL; // INIT false; // true for me // 

	/*
		DICT PTR2 -> TCL_VECT.list
		VECT PTR2 -> TCL_VECT.dict
		LEX2 PTR2 -> LEX1

		SO on setFromAny ... call CTOR ( obj ) += args 
		SO at lease DTOR call RefDecr( PTR2 )
	*/
	bool PTR2_is_Tcl_Obj; // 
	bool PTR2_is_CXX_Obj; // C obj really
	virtual bool PTR2_CTOR(Tcl_Obj * obj) { TCL_set_PTR2(obj,NULL); return true; };
	virtual bool PTR2_DTOR(Tcl_Obj * obj) { TCL_set_PTR2(obj,NULL); return true; }; 

// UNUSED UPTO HERE

	TCL_ObjType_PLUS( const char * ABB )
	{
		name = ABB;
		set_funcs_NULL(); 
		set_funcs_BASE();  
		alias_one_ABB = ABB;
		alias_one_LONG = ABB;
//		alias_two_ABB = NULL;
//		alias_two_LONG = NULL; 

		has_DICT_of_KEY_VAL = false; // LEX1 no extra KEY_VAL fields
		PTR2_is_Tcl_Obj = false;
		PTR2_is_CXX_Obj = false; // C eg openssl X509 via CXX fn
		bytes_never_NULL = false; // set true for me // default others
		INFO("CTOR '%s'", name );
	}

#if 0
	TCL_ObjType_PLUS()
	: Tcl_ObjType()
	{
		alias_one_ABB = "UNSET";
		alias_one_LONG = "UNSET";
	}
#endif

	void set_funcs_BASE();
	/* each class sets own functs */
	// caller must set the funcs
	// or not rewriting this to always use VTBL
	// or maybe not for most common // have a NULL that could do more

	bool Register_ObjType();
	// nb these return bool even though Tcl might return void
 virtual
	bool VTBL_FreeInternalRepProc( Tcl_Obj *obj );
 virtual
	bool VTBL_DupInternalRepProc( Tcl_Obj *src, Tcl_Obj * dst );
 virtual
	bool VTBL_UpdateStringProc( Tcl_Obj * obj );
 virtual
	bool VTBL_SetFromAnyProc( Tcl_Interp *interp, Tcl_Obj *obj );

 // CAST // Tcl_ObjType <--> Tcl_ObjType_PLUS <--> TCL_PLUS_BASE // 

	operator Tcl_ObjType * ()
	{
		INFO("AUTO CAST"); // never called ?
		FAIL("AUTO CAST"); // never called ?
		// it is not a FAIL but that is more visible
		return this;
		// ie unexpected behaviour from CAST (type *)
		// need to scribble some RTFM explains
	}

	Tcl_ObjType * plain()
	{
		INFO("MANUAL CALLED CAST"); // manually called !

		// I dont know why the manual is needed
		Tcl_ObjType * self_BASIC = this; // C++ did cast +8 // not above

// forwards	Tcl_ObjType * self_BASIC_another = get_typePtr_from_PLUS(this);
		Tcl_ObjType * self_BASIC_another = (Tcl_ObjType*) (this);
		// self_BASIC_another never triggered operator
		// C++ did cast +8 // but not operator cast above

		char * EA_self_PLUS = (char *) this; // PLUS as char *
		char * EA_self_BASIC = (char *) self_BASIC; // BASIC as char *

		if( EA_self_PLUS == EA_self_BASIC ) {
			FAIL("SAME");
		} else {
			INFO("DIFF %d", (int)(EA_self_BASIC - EA_self_PLUS));
			/*
				This shows as DIFF 8
				so self_BASIC had an auto cast of +8
			*/
		}

		TCL_ObjType_PLUS * THAT;
	//	int DIFF = (char *)(Tcl_ObjType *) THAT - (char *) THAT;
		int DIFF = (char *) THAT - (char *)(Tcl_ObjType *) THAT;
		INFO("UNDIFF %d", DIFF );

		return this;
	}

// PLUS // OBJ // type operates on obj // caller must know if OK

	void TCL_set_PTR2_incr( Tcl_Obj * INST, Tcl_Obj * P2 )
	{
		Tcl_Obj * old_val = TCL_get_PTR2_as_Tcl_Obj( INST );
		if( P2 ) Tcl_IncrRefCount( P2 );
		TCL_set_PTR2( INST, P2 );
		if( old_val ) {
			WARN("Expected NULL old_val PTR2 '%s'", old_val->bytes);
			Tcl_DecrRefCount( old_val );
		}
	}

	void TCL_set_PTR2_no_incr( Tcl_Obj * INST, Tcl_Obj * P2 )
	{
		// so no DECR either //
		TCL_set_PTR2( INST, P2 );
	}


}; // struct

inline // TCL_HELP
void TCL_set_PTR2_decr_NULL( Tcl_Obj * INST )
{
	Tcl_Obj * old_val = TCL_get_PTR2_as_Tcl_Obj( INST );
	TCL_set_PTR2( INST, NULL );
	if( old_val ) {
		Tcl_DecrRefCount( old_val );
	}
}

//////////////////// CAST Tcl_ObjType to TCL_ObjType_PLUS ////////////////

inline
Tcl_ObjType      * get_typePtr_from_PLUS(TCL_ObjType_PLUS * PLUS ) {
	return (Tcl_ObjType * ) PLUS;
}

inline
TCL_ObjType_PLUS * get_PLUS_from_typePtr_RAW( const Tcl_ObjType * PLAIN ) {

	// RAW means no check // caller often KNOWS

	
	// imagine a PLUS at EA_PLUS
	TCL_ObjType_PLUS * EA_PLUS;
	// EA_PLUS -> name used to be at offset ZERO
	// so adj = offsetof( name )
	// int adj = offsetof( struct TCL_ObjType_PLUS, name );
//	int adj = offsetof( struct TCL_ObjType_PLUS, name );
	int adj = (long) (u8*) &( EA_PLUS->name ) - (long) (u8*) EA_PLUS;
	// compiler should optimise out the above // adj == 8

	// so now subtract adj from PLAIN to get PLUS // EA == PTR
	u8 * PTR = (u8*) PLAIN;	
	PTR -= adj;
	INFO("CAST adj = %d", adj );
	return (TCL_ObjType_PLUS *) PTR;
}

inline
TCL_ObjType_PLUS * get_PLUS_from_typePtr( const Tcl_ObjType * PLAIN ) {

	if(!PLAIN) return NULL;
	// this dupliates logic in OBJ_module.h
	if(PLAIN -> dupIntRepProc != PLUS_MYTYPE_DupInternalRepProc )
	{
		FAIL("TYPE is '%s' not PLUS", PLAIN->name);
		return NULL;
	}

	return get_PLUS_from_typePtr_RAW( PLAIN );
}

inline
TCL_ObjType_PLUS * get_PLUS_from_obj( const Tcl_Obj * obj ) {
	INFO("UP CAST CALLED");
	// already have tested it is a _PLUS
	return get_PLUS_from_typePtr( obj->typePtr );
}

inline
TCL_ObjType_PLUS * get_PLUS_from_obj_RAW( const Tcl_Obj * obj ) {
	return get_PLUS_from_typePtr_RAW( obj->typePtr );
}

//////////////////////////////////////////////////////////////


}; // namespace
#endif
