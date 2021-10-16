#ifndef TCL_HELP_H
#define TCL_HELP_H

extern void print_tcl_obj( Tcl_Obj * obj );
extern Tcl_Obj * mk_str( const char * str );
extern Tcl_Obj * mk_common_spelling( Tcl_Interp * interp, const char * str );


#endif

