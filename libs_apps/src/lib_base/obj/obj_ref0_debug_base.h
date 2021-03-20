#ifndef obj_ref0_debug_base_H
#define obj_ref0_debug_base_H

// need both msg and callers __FUNCTION__ but lose return ...
#define debug_track(msg) if(!debug_track_pointer()) FAIL(msg);

#define debug_check_pointer() debug_check_pointer_fn(__FILE__,__FUNCTION__)
#define debug_track_pointer() debug_track_pointer_fn(__FILE__,__FUNCTION__)
#define PRNT()		PRNT_fn(__FILE__,__FUNCTION__)
#define debug_check_CTOR_flag() debug_check_CTOR_flag_fn(__FILE__,__FUNCTION__)

#include "GLOBAL.H"

class obj_ref0;
class str2;

#include "new_insitu.h"
#include "ints.h"
#include <stdio.h>
#include "int_bitmap.h"

	struct buffer1;


// #include "obj_ref0_debug_PRIV.h"

/*
	REF_FIXED can be used where refcounting is causing a sig-segv.
	If you think something is being freed early, FIX it, or the code.

	REF_STATIC is for when an object is created on the stack or statically
	without a call to new and assignment to a pointer. Also for
	when an object is a field of a parent object (no pointer).

	This should be reworked now that flags are available.
*/
#define xREF_FIXED_VAL (1<<16)
#define xREF_STATIC_VAL (1<<17)
#define xREF_DEBUG 1

// makefile decides WITH_PYTHON // optimise frequent header
#ifdef WITH_PYTHON		//
#include "obj_Py_base.h"	// mixin PyObject base to obj_ref0
#endif				// will require recompile-everything


/*
	REF_DEBUG_LIST (item) causes each object to be added to a circular list.
	It isnt a compiler command line option (-D) because of make deps,
	ie editing this file regenerates all dependent files.

	It can be switched off at compile time (edit this file) as it
	does slow things down.

	It can also be switched on/off at runtime, but the fields are alloc'd

	New feature: flags to detect when some object gets destroyed

	Old Intention: (now proved a bad idea). You cant simply delete the
	remaining objects left in the list, as they can refer to eachother.
	You could have a clear-down-but-dont-delete, where clear-down
	might release obj_hold<X>
*/
#define REF_DEBUG_LIST 
#ifndef REF_DEBUG_LIST
#error THIS IS NEVER USED and deleted 
#else
// #warning THIS IS USED 
	/*!
		compiled with debugging -
	*/

	class obj_ref0_debug_base
#ifdef WITH_PYTHON
	: public VTL_PyObject
#endif
	{
	 public:
	 	// circular linked list of objects // debug_list_*
		obj_ref0_debug_base * debug_list_next;
		obj_ref0_debug_base * debug_list_prev;
		int debug_list_when; // debug_list_era ++ allocated in era
		int_bitmap debug_flags; // debug flags // not object_flags ?

		// debug_ref_zero should have no effect other than tracing 
		void debug_ref_zero(); // called when ref hits zero // NOOP
	 public:
		obj_ref0_debug_base(); // CTOR does nothing
		virtual ~obj_ref0_debug_base(); // DTOR does nothing

		static void list_all_objs_in_era( int era );
		bool debug_check_pointer_fn(const char * file, const char * func); // says less
		bool debug_check_pointer_one(const char * file, const char * func); // says less
		bool debug_check_pointer_is_in_list(const char * file, const char * func); // says less
		bool debug_track_pointer_fn(const char * file, const char * func); // calls PRNT
		bool debug_check_next();
		bool debug_check_prev();
		bool PRNT_fn(const char * file, const char * func);
		bool debug_check_CTOR_flag_fn(const char * file, const char * func);
		virtual void debug_event_on_delete();
		void debug_connect_event_on_delete();

		//! rtti typename with leading digits skipped
		const char * type_name(buffer1 & tn);

		//! print object info
		virtual void debug_print() {}

		//! print object info and then a newline
		void debug_print_ln();

		//! debugger told about broad categories of sub-class
		void debug_set_is_widget();
		bool debug_test_is_widget();
		virtual const char * debug_widget_type_name();
		int debug_get_ref_count();
		void *  debug_get_widget_ptr();

		const char * debug_obj_name_for_now; // recursive include loop
		const char * debug_get_obj_name();
		void debug_set_obj_name( const char * s );


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnonnull-compare"
		inline bool debug_this_is_NULL() {return !this; }
#pragma GCC diagnostic pop
	};
#endif

extern obj_ref0_debug_base * debug_list_root;
extern int debug_list_era;



#endif

