#ifndef TCL_HELP_H
#define TCL_HELP_H

extern void print_tcl_obj( Tcl_Obj * obj );
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


#endif

