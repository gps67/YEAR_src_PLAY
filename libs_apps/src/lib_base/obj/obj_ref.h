#ifndef OBJ_REF_H
#define OBJ_REF_H

#include "CTXT_stubs.h"

#include "obj_ref0_debug_base.h"
// #include "obj_ref0_debug_PRIV.h"

#define makefile_decides_WITH_PYTHON 1		// base obj_ref on PyObject
#define USE_PYTHON_H		// or use stubs for speed // untyedef ?

extern obj_ref0_debug_base * debug_list_root;
extern int debug_list_era;

//! keep a tally of refcounts
/*!
	The number of objects allocated, deleted, and with debugging on,
	the number of ref_incr() calls. Used to show the extent of
	mem-leakage when different code is run. 
*/
class ref_tally_t : public GRP_lib_base
{ public:
	i32 n_newed;
	i32 n_static;
	i32 n_fixed;
	i32 n_unfixed;
	i32 n_incr;
	i32 n_decr;
	i32 n_zero;
	i32 n_deleted;

	ref_tally_t();
	void print_leaked_report();
};

extern ref_tally_t ref_tally;

////////////////////////////////////////////////////////////////////////////

//! obj_ref0 is a ref_counted base class
/*!
	As a base class, the main objective is ref_counting,
	but other common concepts will also be required,
	and it makes sense to have a single VTBL, so other
	virtual function will be added with vanilla default
	actions.
*/
class obj_ref0 : public obj_ref0_debug_base
{
	friend class w_widget0;

#ifdef WITH_PYTHON
#define ref_count ob_refcnt // NAUGHTY but OK
#else
	//! The number of refereces to this object
	int ref_count;
#endif

	//! all objects have a 32 flag bits, some reserved for (system)
	int inst_flag_bits;
	/*

		FLAG: object is immutable - readonly
		SO: create new object on set_int(), eg HT key val
		OR: all change
		(( nb new object should be equally immutable? is_shared() ? ))

		FLAG: object is BOUNCE WRAPPED
		SO: at start of function, fetch actual from CSR
		OR: bounce everything (+repeat some flags at top+)

		FLAG: object has ASSOCIATED data, extended,
		SO: on delete search out via hash-tables + delete
		OR: on delete instant over + reuse

		FLAG: gobject has floating reference
		SO:
		OR:

		FLAG: object has LRU bits xxxx
		SO: set bit if recently used, sweep into upper count
		OR: delete object from THIS cache if not used in 7

		FLAG: object has been half deleted
		SO: dont bother with refcount zero into FIRST handler
		OR:

		FLAG: object deemed readonly - eg hash key immutable
		SO:
		OR:

		FLAG: object has iterator locking it
		SO: do not realloc/delete sub-area (until?) (at least warn)
		OR:

		FLAG: object has SAFE/PRIV flag (level xxx)
		SO:
		OR:

		FLAG: object has CHANGED flag (eg spreadsheet lazy)
		SO:
		OR:

		FLAG: object has USER_LIVE flag, eg GUI is mapped/exists
		SO:
		OR:

		FLAG: object has PLUS data (group xxx)
		SO: obj.xxx -> something, can cast (obj_PLUS *)
		OR: default fallback values, or search lookup

		FLAG: object has special alloc component
		SO: do not free ptr3, ...
		OR: 

		FLAG: object has special alloc SELF
		SO: do not free this, ...
		OR: 

		FLAG: object is field of larger object
		SO: refcount is one at start ...
		OR: (stack allocated)
		++: object is snagged on sub-ref - unhook somehow when sweep

		FLAG: object is malloced by heap (xxx)
		SO: free is special handler
		OR: 

		FLAG: GC/ALLOC says dont recycle memory
		SO: buffer80 does get recycled, last block gets freed
		OR: flag says malloc contains HIDDEN field out of header (PTR_state)

	*/

 public:

	//! DESTRUCTOR
	virtual ~obj_ref0();

	//! this is a new ref_counted object
	obj_ref0()
	{
		ref_tally.n_newed ++;
#ifdef WITH_PYTHON
		// done in base class
#else
		ref_count = 0;
#endif
		inst_flag_bits = 0;
	}

	/*!
		copy constructor is to be avoided, but tracers easily added
	*/
	obj_ref0( const obj_ref0 & obj2 );

// -- REF COUNTING

	// NON-INLINE functions should be equivolent to inline ones
	// but easier to change without recompi;ing the world
	// better debug stack frames
	// but slower

	void ref_incr_fn();
	void ref_decr_fn();
	void ref_jump_fn();

	//! ref_zero is called when refcount reaches zero - can print
	virtual void ref_zero();	// ie delete, now or later

	//! increment the ref_count
	void ref_incr()
	{
		debug_check_CTOR_flag();
		ref_tally.n_incr ++;
		ref_count++;
	}

	//! decrement the ref count without any check, or counting
	void ref_decr_private()
	{
		ref_count--;
	}

	//! decrement the ref_count, possible deleting it
	void ref_decr()
	{
		debug_check_CTOR_flag();
		ref_tally.n_decr ++;
		ref_count--;
		if( ref_count <= 0 ) ref_zero();
	}

	//! unused late test
	void ref_test()
	{
#ifdef WITH_PYTHON
		// TODO - no test done
#else
		if( ref_count <= 0 ) ref_zero();
#endif
	}

	//! useful to print the reference count
	int get_ref_count() {
#ifdef WITH_PYTHON
		return ob_refcnt;
#else
		return ref_count ;
#endif
	}

	// move to .cxx
	void ref_fixed();
	void ref_static();

#if 0
src/lib_accts/table/tbl_col_spec_get.cxx:       naff_static->ref_fixed();
src/sar_data/mem/mem_line_hash.h:               temp_key.ref_fixed();
src/sar_plot/graph/axis_draw.h:         // NOT an obj_ref // text.ref_fixed();
src/sar_plot/graph/axis_draw.h:         gc.ref_fixed();

src/lib_base/strings/str2.cxx:  s2->ref_static();       // not enforced but it helps with the counting
src/lib_base2/gtk_1/w_dialog_msg.cxx:   msg->ref_static();

	a lot of strings are tagged str2_static
	but they are not retaining static ROM strings - they get copies!
	they ARE non-maleable symbols (symb_CAT = str2_static("CAT"))
	they CAN be dropped by exit() not reclaimed
	they MIGHT/NOT have any users to keep them (but ready for use)
	they could be kept in their own HASH_TABLE (most users want this)
	for now set a flag, and sink a reference ?
#endif

// -- HASH

	//! hash table key is int (u32)
	virtual uint ht_get_hash();

	//! hash table key comparison
	virtual bool ht_equals( obj_ref0 * key2 );

// -- DEBUG

/*	INHERIT
	bool debug_track_pointer();
	const char * type_name();
*/

	//! print object info
	virtual void debug_print();

	//! print object info and then a newline
//	void debug_print_ln(); // BASE

	//! get the debugging name 
	virtual str2 * debug_name_get();

	//! set the debugging name 
	virtual void debug_name_set( str2 * name );

	/*
		
	*/
	virtual void prints() {}

// -- TREE SEARCH ?

	/*!
		Generic debugging thing, (formal use needs thinking).
		thelist is a list of all reachable things.

		for each real child, call:
			thelist->list_append( child )

		avoid circular lists, etc

		USED BY: container delete all children (or was it?)
	*/
	virtual void append_children_to_list( obj_ref0 * thelist ) {}

	/*!
		generic hook for any use ;-)

		ie THIS is some sort of list,
		but if this is generally useful, 
		more thought is need.
	*/
	virtual void list_append( obj_ref0 * item ) {}

	bool DTOR_marked(); // was it already set ?
	bool DTOR_mark(); // set it and return: was it already set ?
	// there is not DTOR_clear - it is down to debug layer

};

/*!
	This causes problems with doxygen, but a simpler test case
	doesnt show any problem. 
*/

//! obj_ref is an alias for obj_ref0
typedef obj_ref0 obj_ref;

//! a C callable for obj->ref_decr();
extern void C_ref_decr( obj_ref * obj );

#ifdef WITH_PYTHON
#undef ref_count 
#undef PyObject_HEAD 
// untypedef PyObject ... how
#endif

#endif //_H
