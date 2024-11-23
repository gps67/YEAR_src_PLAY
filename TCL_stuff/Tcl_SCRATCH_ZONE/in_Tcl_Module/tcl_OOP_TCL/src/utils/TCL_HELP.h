#ifndef TCL_HELP_H
#define TCL_HELP_H

// #include "str0.h"
typedef const char * STR0;

namespace TCL {

/*
	TCL does not define a pair of VAL1 VAL2
	BUT we can use PTR1 and PTR2 and typecast them as int
	actual word_size must match PTR
*/
// #define VAL_t long
// typedef unsigned long VAL_t; 
typedef long VAL_t; 
// ALIAS PTR_as_UNSIGNED_t
// ALIAS PTR_as_LONG_t
// ALIAS PTR_as_LONG_t

extern void print_tcl_obj( Tcl_Obj * obj, const char * str = NULL );
extern Tcl_Obj * mk_str( const char * str );
extern Tcl_Obj * mk_common_spelling( Tcl_Interp * interp, const char * str );

 inline
STR0 TCL_get_STR0( Tcl_Obj * obj ) { 
	return obj -> bytes; // could easily be NULL
}

 inline
void * TCL_get_PTR1_as_void( Tcl_Obj * obj ) { 
	return obj -> internalRep.twoPtrValue .ptr1 ;
}

 inline
void * TCL_get_PTR2_as_void( Tcl_Obj * obj ) { 
	return obj -> internalRep.twoPtrValue .ptr2 ;
}

 inline
Tcl_Obj * TCL_get_PTR1_as_Tcl_Obj( Tcl_Obj * obj ) { 
	return (Tcl_Obj *) obj -> internalRep.twoPtrValue .ptr1 ;
}

 inline
Tcl_Obj * TCL_get_PTR2_as_Tcl_Obj( Tcl_Obj * obj ) { 
	return (Tcl_Obj *) obj -> internalRep.twoPtrValue .ptr2 ;
}

// - as VAL_t

 inline
VAL_t TCL_get_VAL1_as_VAL_t( Tcl_Obj * obj ) { 
	return (VAL_t) obj -> internalRep.twoPtrValue .ptr1 ;
}

 inline
VAL_t TCL_get_VAL2_as_VAL_t( Tcl_Obj * obj ) { 
	return (VAL_t) obj -> internalRep.twoPtrValue .ptr2 ;
}

// USED or NOT // ??
 inline
void ** TCL_get_EA_PTR2( Tcl_Obj * obj ) { 
	return &( obj -> internalRep.twoPtrValue .ptr2) ;
}

 inline
void TCL_set_PTR1( Tcl_Obj * obj, void * PTR ) { 
	obj -> internalRep.twoPtrValue .ptr1 = PTR ;
}

 inline
void TCL_set_PTR2( Tcl_Obj * obj, void * PTR ) { 
	obj -> internalRep.twoPtrValue .ptr2 = PTR ;
}

 inline
void TCL_set_VAL1( Tcl_Obj * obj, VAL_t VAL ) { 
	TCL_set_PTR1( obj, (void *) VAL );
}

 inline
void TCL_set_VAL2( Tcl_Obj * obj, VAL_t VAL ) { 
	TCL_set_PTR2( obj, (void *) VAL );
}


 inline
VAL_t TCL_get_VAL1( Tcl_Obj * obj ) { 
	return (VAL_t) TCL_get_PTR1_as_void( obj );
}

 inline
VAL_t TCL_get_VAL2( Tcl_Obj * obj ) { 
	return (VAL_t) TCL_get_PTR2_as_void( obj );
}


// this was private, seems useful
extern
char * P64( void * PTR );

inline
const char * str_not_NULL( const char * str )
{
	return str ? str : "(null)";
}

inline
const char * TCL_get_type_name( Tcl_Obj * obj ) {
	if(!obj) {
		FAIL("EXPECT NEVER NULL");
		return "(NULL OBJ)";
	}
	if(!obj->typePtr) {
		return "(NULL TYPE)";
	}
	if(!obj->typePtr->name) {
		FAIL("EXPECT name NEVER NULL");
		return "(NULL TYPE NAME)";
	}
	return obj->typePtr->name;
}

}; // namespace
#undef VAL_t
#endif

