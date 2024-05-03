#ifndef TCL_OBJ_H
#define TCL_OBJ_H

	// BROKEN and wrong name

	// _OBJ
	// Tcl_Obj * OBJ // 
	// TCL_OBJ // Tcl_Obj * OBJ // = EXPR // == EXPR // 2_way_eq_eq
	// OBJ // SESS knows that TCL_OBJ is TCP_OBJ is OBJ //
	// NAME // OBJ // ALIAS in SESS in SCRIPT as SPEC_TREE
	// LITERAL_MATCH is like LITERAL_MATCHER but shorter
	// drop all LEX1 nonsense
	// go directly for strcmp
	// 


//	#include "TCL_STUBS.h"
//	#include "TCL_REF.h"
//	#include "TCL_HELP.h"
//	#include "TCL_PLUS_BASE.h"
//	#include "TCL_PLUS_LEX.h"
//	// #include "TCL_MATCHER.h"
//	
//	#include <string.h> // and hopefully its inline version of strcmp or strncmp

namespace TCL { // Module OBJ_Module // DECL OBJ_t LEX += AVAR Module //
// OBJ_Module
// Module_OBJ
// Module_
// _OBJ

 // LEXICON += MENTION
 // 

 ALIAS OBJ_t // found in DECL of OBJ // VIEW SPELLING as USED "%s" // "OBJ_t"

 typedef
 	Tcl_Obj OBJ_t;
 struct OBJ_t

 ALIAS TCL_OBJ OBJ
 ALIAS TCL_OBJ_t OBJ_t

 struct TCL_OBJ { // OBJ_t 
 // start with "{ struct TCL_OBJ : Tcl_Obj { SCRIPT }; // MODAL struct lines

 	// this struct is a single WORD holding a PTR
	// this requires compiler to do clever stuff
	// option of specific 
	// struct TCL_OBJ : caddr_t 
	// struct TCL_OBJ : u64_PTR  

 		typedef Tcl_Obj OBJ_t;

		// OBJ_t is an ALIAS for "{ Tcl_Obj * PTR }" // no semicolon
		// typedef Tcl_Obj * PTR_t }" // PTR == OBJ //
		// typedef Tcl_Obj   OBJ_t }" // PTR == OBJ //
 		OBJ_t * OBJ // STUBS OBJ_t // SUBSET TCL_OBJ.OBJ // OBJ_t //


	TCL_OBJ( Tcl_Obj * tcl_obj ) { // OBJ_t 
	// : OBJ( tcl_obj ) // WORD TYPE USES CTOR_WORD_ASIS
	// no refIncr for CPU_WORD holding OBJ_PTR
	// only used for storing item in tree 

	 	OBJ = tcl_obj; 

 	}

	Tcl_Obj * GET_OBJ() { return OBJ; }

	Tcl_Obj *
	Duplicate() { return Tcl_DuplicateObj( OBJ ); }

	ref_incr() {
	   Tcl_IncrRefCount(OBJ);
	}

	ref_decr() {
	   Tcl_DecrRefCount(OBJ);
	}

	ref_incr_then_decr() {
	   Tcl_BounceRefCount(OBJ);
	}

	CALL_Invalidate_String_Rep() {
	       Tcl_InvalidateStringRep(OBJ)
	}

	bool is_NULL() {
		return OBJ == NULL; // or PTR == NULL // NULL_MEANS_ABSENT_OBJ
		// NONE_MEANS_PICK_DEFAULT //
		// NONE_MEANS_GROW_SESSION //
		// NONE_MEANS_SAVE_CURRENT //
		// NONE_MEANS_LOAD_CURRENT //
	}

	bool is_shared() {
		return Tcl_IsShared(OBJ);
	}

	bool CTOR(/* EA_of_PTR_in_MEM */) { OBJ = NULL; }
	/*
		lifecycle of the TCL_OBJ
		it is
	*/

 };

