#ifndef TCL_ObjType_PLUS_H
#define TCL_ObjType_PLUS_H
//	#include "TCL_ObjType_PLUS.h"

#include "TCL.h"
#include "TCL_STUBS.h" // INFO WARN FAIL
#include "TCL_HELP.h" // PTR1 PTR2
// #include "dgb.h" // FAIL

// this is poly_essence (not polymorphic)
// local definition of GET_KEPT_PTR is used 
// not the one in this file

#define is_my_type( obj ) \
	(obj -> typePtr == GET_KEPT_PTR()

// if you change from DUP to (one of the 4 function pointers)
// you will also want to change this
// strong type makes it a sensible thing

typedef Tcl_DupInternalRepProc * KEPT_PTR_type;
// ie void (*) func( Tcl_Obj *srcPtr, Tcl_Obj *dupPtr );


// PTR to this function is the actual KEPT_PTR
// it is available as a PC relative ELF address
// it may or may not be stored in CLientData->kept_PTR
// which might be faster, than the ELF symbol WRT PC, or other
extern "C" 
void
PLUS_MYTYPE_DupInternalRepProc( Tcl_Obj *srcPtr, Tcl_Obj *dupPtr );


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

enum PLUS_method_tag
{
	PLUS_Dup_,
	PLUS_STR_UDEF_,
	PLUS_STR_OBJ_2X,
	PLUS_STR_obj_X,
	PLUS_STR_obj_str,
};


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
struct TCL_ObjType_PLUS : Tcl_ObjType
{
	// C++ is wrong to put VTBL in front of plain STRUCT
	// it does not cost much to put VTBL in the middle
	// that would keep all pointers compat

	virtual ~TCL_ObjType_PLUS()
	{
		INFO("UNCALLED DTOR");
	}

	// OPTION // TCL_ObjType_BASIC += first field PTR_TO_PLUS_MGR
	// OPTION // then put _BASIC mid way through _PLUS _MGR
	// offset compute from sub to surround // less adj naff //

 // pretty printing require CTOR to set up good defaults

	const char * alias_one_ABB; // when not UDEF_25 UDEF_LEX1 or "LEX1"
	// "LEX1"

	const char * alias_one_LONG;
	// "UDEF_25_LEX1" // LEX1 is also an STR4 // with NUL


	/*
		you must set dupIntRepProc_PLUS

		OPTION of having Register do it,
		as that would enable the choice of DUP DEL STR CAST
		but not

		Maybe make them all "hard to see" and use SETTERS
		maybe just remember to set the _PLUS

	*/

	// Tcl_UpdateStringProc *updateStringProc;
	Tcl_DupInternalRepProc *dupIntRepProc_PLUS;

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

	TCL_ObjType_PLUS( const char * ABB )
//	: Tcl_ObjType()
	: dupIntRepProc_PLUS( NULL )
	{
		set_funcs_NULL(); // Register calls set_funcs()
		alias_one_ABB = ABB;
		alias_one_LONG = ABB;
//		alias_two_ABB = NULL;
//		alias_two_LONG = NULL; 
		name = ABB;

		has_DICT_of_KEY_VAL = false; // LEX1 no extra KEY_VAL fields
		PTR2_is_Tcl_Obj = false;
		PTR2_is_CXX_Obj = false; // C eg openssl X509 via CXX fn
		bytes_never_NULL = false; // set true for me // default others
		INFO("CTOR '%s'", name );
	}

	TCL_ObjType_PLUS()
	: Tcl_ObjType()
	{
		alias_one_ABB = "UNSET";
		alias_one_LONG = "UNSET";
	}

	void set_funcs_NULL()
	{
	 #if 0
		Tcl_FreeInternalRepProc *freeIntRepProc;
		Tcl_DupInternalRepProc *dupIntRepProc;
		Tcl_UpdateStringProc *updateStringProc;
		Tcl_SetFromAnyProc *setFromAnyProc;
	 #endif
		freeIntRepProc = NULL;
		dupIntRepProc = NULL;
		updateStringProc = NULL;
		setFromAnyProc = NULL;

		dupIntRepProc_PLUS = NULL;
	}

	void set_funcs_BASE();
	virtual void set_funcs();
	/* each class sets own functs */

	// caller must set the funcs

	bool check_funcs_not_NULL();

	bool Register_ObjType();

	void TCL_set_PTR2_incr( Tcl_Obj * INST, Tcl_Obj * P2 )
	{
		Tcl_Obj * old_val = (Tcl_Obj *) TCL_get_PTR2( INST );
		if( P2 ) Tcl_IncrRefCount( P2 );
		TCL_set_PTR2( INST, P2 );
		if( old_val ) {
			WARN("Expected NULL old_val PTR2 '%s'", old_val->bytes);
			Tcl_DecrRefCount( old_val );
		}
	}

	operator Tcl_ObjType * ()
	{
		INFO("AUTO CAST"); // never called ?
		FAIL("AUTO CAST"); // never called ?
		return this;
	}


}; // struct

inline // TCL_HELP
void TCL_set_PTR2_decr_NULL( Tcl_Obj * INST )
{
	Tcl_Obj * old_val = (Tcl_Obj *) TCL_get_PTR2( INST );
	TCL_set_PTR2( INST, NULL );
	if( old_val ) {
		Tcl_DecrRefCount( old_val );
	}
}

inline
Tcl_ObjType      * get_typePtr_from_PLUS(TCL_ObjType_PLUS * PLUS ) {
	return (Tcl_ObjType * ) PLUS;
}

inline
TCL_ObjType_PLUS * get_PLUS_from_typePtr( const Tcl_ObjType * PLAIN ) {
	
	TCL_ObjType_PLUS * P_PLUS;
//	int adj = offsetof( struct TCL_ObjType_PLUS, name );
//	u8 * P1 = (u8*) P_PLUS;
//	u8 * P2 = 
	int adj = (long) (u8*) &( P_PLUS->name ) - (long) (u8*) P_PLUS;
	u8 * PTR = (u8*) PLAIN;
	PTR -= adj;
	INFO("adj = %d", adj );
	return (TCL_ObjType_PLUS *) PTR;
}

inline
TCL_ObjType_PLUS * get_PLUS_from_obj( const Tcl_Obj * obj ) {
	INFO("CALLED");
	// already have tested it is a _PLUS
	return get_PLUS_from_typePtr( obj->typePtr );
}

struct TCL_ObjType_LEX1 : TCL_ObjType_PLUS
{
	TCL_ObjType_LEX1()
	: TCL_ObjType_PLUS("LEX1")
	{
		// see get_TYPE_LEX1
//		bytes_never_NULL = true; // LEX1 is LEX1 -> bytes
//		PTR2_is_Tcl_Obj = false; // LEX1 is just a typePtr tag
	}

	virtual void set_funcs();

};


struct TCL_ObjType_LEX2 : TCL_ObjType_PLUS
{
	TCL_ObjType_LEX2()
	: TCL_ObjType_PLUS("LEX2")
	{
		// see get_TYPE_LEX2() for details
		bytes_never_NULL = true; // LEX2 is LEX1 -> bytes
	}
	virtual void set_funcs();
};

struct TCL_ObjType_DICT : TCL_ObjType_PLUS
{
	TCL_ObjType_DICT()
	: TCL_ObjType_PLUS("DICT")
	{
		PTR2_is_Tcl_Obj = true; // dict
	}
	virtual void set_funcs();
};

struct TCL_ObjType_VECT : TCL_ObjType_PLUS
{
	TCL_ObjType_VECT()
	: TCL_ObjType_PLUS("VECT")
	{
		PTR2_is_Tcl_Obj = true; // list is vect
	}
	virtual void set_funcs();
};


// NB this is where GLOBAL meets parametised LAYERS
// mk_LEX1("lookup") needs global TYPE_LEX1 and global LEX1_SPELLING_POOL

extern
TCL_ObjType_LEX1 * get_TYPE_LEX1(); // does not need interp
extern
TCL_ObjType_LEX2 * get_TYPE_LEX2(); // does not need interp
extern
TCL_ObjType_DICT * get_TYPE_DICT(); // does not need interp
extern
TCL_ObjType_VECT * get_TYPE_VECT(); // does not need interp

Tcl_Obj * mk_LEX1( Tcl_Interp * interp, const char * str );
bool upgrade_to_LEX2( Tcl_Obj * obj,  Tcl_Obj * LEX1 );


#endif
