#ifndef TCL_VAR_H
#define TCL_VAR_H

#include "TCL_STUBS.h"
#include "TCL_PTR.h"
#include "TCL.h"

namespace TCL {

 struct cached_var {

 	Tcl_Obj * var_name; // we will upgrade this to OPTIMISE_LOOKUP_LIST
	/*

		because it comes from the Tcl Parser for this script
		it is the same var_name_usage or field_name_usage or DECODE

		if used_as_a_field_name ... tree_code_stay_here
		if same_SPELLING_OBJECT ... MATCH TOKEN_xFFFF

			TOKEN = xFFFF

	*/

	/*!
		create var if was absent
		set_var_val
	*/
	Tcl_Obj * val_kept = var_set_val(
		Tcl_Interp * interp,
		Tcl_Obj * var_name,
		Tcl_Obj * ary_key, // NULL means plain VAR not ARY_KEY_VAL
		Tcl_Obj * new_val
	) {
		flags = 0 
		+ TCL_LEAVE_ERR_MSG
		;
		return Tcl_ObjSetVar2(interp, var_name, ary_key, new_val, flags)
	}

	/*!
		create var if was absent
		append_text_str
		append_list_item
	*/
	Tcl_Obj * val_kept = var_append_text_str(
		Tcl_Interp * interp,
		Tcl_Obj * var_name, // text buffer name
		Tcl_Obj * ary_key, // NULL means plain VAR not ARY_KEY_VAL
		Tcl_Obj * add_str
	) {
		flags = 0 
		+ TCL_LEAVE_ERR_MSG
		+ TCL_APPEND_VALUE
		;
		return Tcl_ObjSetVar2(interp, var_name, ary_key, new_val, flags)
	}

	/*!
		create var if was absent
		append_text_str
		append_list_item
	*/
	Tcl_Obj * val_kept = var_append_list_item( // may need to rework 
		Tcl_Interp * interp,
		Tcl_Obj * var_name, // list name
		Tcl_Obj * ary_key, // NULL means plain VAR not ARY_KEY_VAL
		Tcl_Obj * add_item
	) {
		flags = 0 
		+ TCL_LEAVE_ERR_MSG
		+ TCL_APPEND_VALUE
		+ TCL_LIST_ELEMENT
		;
		return Tcl_ObjSetVar2(interp, var_name, ary_key, new_val, flags)
	}

	/*!
		caller still has to know about ary_key_NULL_means_AVAR not ARY
		retval NULL means FAIL error message in interp 
	*/
	Tcl_Obj * val_kept = var_get( 
		Tcl_Interp * interp,
		Tcl_Obj * var_name, // list name
		Tcl_Obj * ary_key  // NULL means plain VAR not ARY_KEY_VAL
	) {
		flags = 0 
		+ TCL_LEAVE_ERR_MSG
		+ TCL_APPEND_VALUE
		+ TCL_LIST_ELEMENT
		;
		return Tcl_ObjGetVar2(interp, var_name, ary_key, flags);
	}

 }

/*
	Tcl_Interp * interp;
	const char * name;
	const char * newValue;
	Tcl_Obj * newValuePtr;
	int flags;
	Tcl_Obj *part1Ptr1; // VARNAME
	Tcl_Obj *part1Ptr2; // KEYNAME or NULL // of array_variable
	// POSSIBLY ANYSTR or STR0 //


       Tcl_Obj * Tcl_SetVar2Ex(interp, name1, name2, newValuePtr, flags)
       const char * Tcl_SetVar(interp, varName, newValue, flags)
       const char * Tcl_SetVar2(interp, name1, name2, newValue, flags)
       Tcl_Obj * Tcl_ObjSetVar2(interp, part1Ptr, part2Ptr, newValuePtr, flags)
       Tcl_Obj * Tcl_GetVar2Ex(interp, name1, name2, flags)
       const char * Tcl_GetVar(interp, varName, flags)
       const char * Tcl_GetVar2(interp, name1, name2, flags)
       Tcl_Obj * Tcl_ObjGetVar2(interp, part1Ptr, part2Ptr, flags)
       int Tcl_UnsetVar(interp, varName, flags)
       int Tcl_UnsetVar2(interp, name1, name2, flags)

       OPTION newValue is STR0 // Tcl does all the work of mk_STRING
       OPTION newValue is Tcl_Obj // We decide what is where mk_STRING(STR0)

       OPTION VAR1
       OPTION VAR1(NULL)
       OPTION VAR1(field2)

       FLAG TCL_GLOBAL_ONLY # unseen if following found first
       FLAG TCL_NAMESPACE_ONLY # or stick with default search seq

       		local to call
		current namespace
		global namespace

		# possible optimisation
		# GEN knows it is a NAMESPACE VAR
		# GEN knows it is a GLOBAL VAR

	
	TCL_LEAVE_ERR_MSG

		otherwise errors are unexplained
	
	TCL_APPEND_VALUE

		OPTION SET
		OPTION APPEND
		for printing lists to buffers out
	
	TCL_LIST_ELEMENT

		for printing lists to buffers out

		
*/


}; // namespace
#endif


