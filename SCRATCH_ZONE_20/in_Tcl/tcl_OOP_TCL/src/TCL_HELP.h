#ifndef TCL_HELP_H
#define TCL_HELP_H

namespace TCL {

extern void print_tcl_obj( Tcl_Obj * obj, const char * str = NULL );
extern Tcl_Obj * mk_str( const char * str );
extern Tcl_Obj * mk_common_spelling( Tcl_Interp * interp, const char * str );
 
 inline
void TCL_set_PTR1( Tcl_Obj * obj, void * PTR ) { 
	obj -> internalRep.twoPtrValue .ptr1 = PTR ;
}

 inline
void TCL_set_PTR2( Tcl_Obj * obj, void * PTR ) { 
	obj -> internalRep.twoPtrValue .ptr2 = PTR ;
}


 inline
void * TCL_get_PTR1( Tcl_Obj * obj ) { 
	return obj -> internalRep.twoPtrValue .ptr1 ;
}

 inline
void ** TCL_get_EA_PTR2( Tcl_Obj * obj ) { 
	return &( obj -> internalRep.twoPtrValue .ptr2) ;
}

 inline
void * TCL_get_PTR2( Tcl_Obj * obj ) { 
	return obj -> internalRep.twoPtrValue .ptr2 ;
}

#define VAL_t long

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
	return (VAL_t) TCL_get_PTR1( obj );
}

 inline
VAL_t TCL_get_VAL2( Tcl_Obj * obj ) { 
	return (VAL_t) TCL_get_PTR2( obj );
}

#undef VAL_t

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
#endif

