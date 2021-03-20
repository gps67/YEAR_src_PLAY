#include "obj_ref.h"
#include <stdio.h>
#include "misc.h"
#include <typeinfo>

#include "e_print.h"
#include "dgb.h"
#include "obj_ref0_debug_base_PRIV.h"
#include "buffer1.h" 
#include "buffer1.h" 

#define DO_PRNT false


ref_tally_t ref_tally;

/*!
	initialise the counters
*/
ref_tally_t::ref_tally_t()
{
	// fprintf(stderr, "ref_tally: INIT\n" );
	n_newed = 0;
	n_fixed = 0;
	n_static = 0;
	n_unfixed = 0;
	n_incr = 0;
	n_decr = 0;
	n_zero = 0;
	n_deleted = 0;
	
}

// local use
#define REF_FIXED_VAL 0x2000
#define REF_STATIC_VAL 0x2000

/*!
	print a report from the counters, and the debug list
*/
void ref_tally_t::print_leaked_report()
{
        obj_ref0_debug_base::list_all_objs_in_era( -1 );
	fprintf(stderr, "\n" );
	fprintf(stderr, "ref_tally_report\n" );
	fprintf(stderr, "ref_tally: n_newed   = %4d\n", n_newed );
	fprintf(stderr, "ref_tally: n_static  = %4d\n", n_static );
	fprintf(stderr, "ref_tally: n_fixed   = %4d\n", n_fixed );
	fprintf(stderr, "ref_tally: n_fixed   = %4d\n", n_fixed );
	fprintf(stderr, "ref_tally: n_unfixed = %4d\n", n_unfixed );
	fprintf(stderr, "ref_tally: n_deleted = %4d\n", n_deleted );
	fprintf(stderr, "ref_tally: n_incr    = %4d\n", n_incr );
	fprintf(stderr, "ref_tally: n_decr    = %4d\n", n_decr );
	fprintf(stderr, "ref_tally: n_zero    = %4d\n", n_zero );
	fprintf(stderr, "ref_tally: delta1    = %4d = %s\n",
		( n_newed - n_deleted ) - n_static,
		"( n_newed - n_deleted ) - n_static\n"
	);
}

#ifdef WITH_PYTHON
// relax, dont get excited, this isnt yet working
// it will require a set of cast because C++ INSISTS on VTBL being first
// that will need a FREE method which is NEVER called by Python!!
// OR: fudge C python to have a C++ virtual base class (need WIN32 port)
// OR: ...etc...
// it will require CTORS to pass down (and/or upgrade) PyType_Type
//////////////////////////////////////////////////////////////////
// some bug where <string.h> and <Python.h> both define it
#ifdef _POSIX_C_SOURCE
#undef _POSIX_C_SOURCE
#endif
#include <Python.h> // which includes "object.h" so avoid that filename ...
#define ref_count ob_refcnt // simplest way !
	/*!
		This is set in the inline contructor, where is it setup?
	*/
	struct _typeobject  VTL_PyObject::ob_type_VTL_PyObject = {
		PyObject_HEAD_INIT(&PyType_Type)
//		0,                              /*ob_size*/
		/*(char *)*/ "EXTN.VTL_PyObject",             /*tp_name*/
		sizeof( VTL_PyObject ),       /*tp_basicsize*/
		0,                              /*tp_itemsize*/
		/* methods */
//		(destructor)   spin_elem_info_dealloc, /*tp_dealloc*/
//		(printfunc)    spin_elem_info_print,   /*tp_print*/
//		(getattrfunc)  spin_elem_info_getattr, /*tp_getattr*/
//		(setattrfunc)  spin_elem_info_setattr, /*tp_setattr*/
		0,                              /*tp_compare*/
		0,                              /*tp_repr*/
		0,                              /*tp_as_number*/
		0,                              /*tp_as_sequence*/
		0,                              /*tp_as_mapping*/
		0,                              /*tp_hash*/
	};

#endif // WITH_PYTHON

/*!
	ref_decr has found ref_zero() which deletes the object

	At this stage the object is COMPLETE, and worth printing
*/
void obj_ref0::ref_zero()
{
	/*
		options here to use debug_flags
		to allow silent deletes,
		etc

		Use a private #include for the flag values
		to allow minimal recompiles, static calls, etc
	*/
	if(DO_PRNT) {
		// It is better to print it here not in DTOR
		// because DTOR gets base class, mostly deleted, this gets TOP
		//
		debug_track_pointer();
	}
	if( ref_count )
	{
		FAIL( "ref_zero() but ref is %d\n", ref_count );
	}
	debug_ref_zero(); // calls debug_event_on_delete() can change ref_count
	ref_tally.n_zero ++;
	if( !ref_count ) {
		if( DTOR_marked() ) {
			FAIL("DTOR_marked NOT CALLING: delete this;");
		} else {
			DTOR_mark();
	if(1)		delete this;
		}
	}
}

/*!
	destructor - called after all sub-class destructors
	and with VTBL (eg class_name) set to almost nothing

	It is possible that the destructor was called without
	ref_zero(). If you try not to do that, you need to
	tag and report that.

	With GDB running, the VTBL might not be useful, but the
	stack is ;-)
*/
obj_ref0::~obj_ref0()
{
	/*
		unfortunately this dctr is called AFTER the others
		so all fields have been cancelled and VTBL reset

		Objects can be statically destroyed and never ref_counted
		They are at least N_ counted 
	 */
	ref_tally.n_deleted ++;
	if( ref_count )
	{
		FAIL("ref_count == %d (type unknown)", ref_count );
		if( ref_count == REF_FIXED_VAL )
		{
			ref_count = 0;
			ref_tally.n_unfixed ++;
			return;
		}
		fprintf(stderr,"OBJ_REF bad count %d on %p\n",
			ref_count, this );
	}
}

//! mark the object as undeletable
void obj_ref0::ref_fixed()
{	// obj is a member of a stuct with dctr
	WARN("Calling this is the last resport of the desperate");
	ref_tally.n_fixed ++;
#ifdef WITH_PYTHON
	ob_refcnt+=REF_FIXED_VAL;
#else
	ref_count+=REF_FIXED_VAL;
#endif
	ref_jump_fn();
}

//! record a static object (count those which dont get deleted)
void obj_ref0::ref_static()
{	// obj is PROBABLY a static string
	WARN("Calling this is the last resport of the desperate");
	ref_tally.n_static ++;
#ifdef WITH_PYTHON
	ob_refcnt+=REF_STATIC_VAL;
#else
	ref_count+=REF_STATIC_VAL;
#endif
	ref_jump_fn();
}

// used to delete row_data held in ctree
void C_ref_decr( obj_ref * obj )
{
	if( obj ) {
		if(DO_PRNT) {
			e_print("C_ref_decr ");
			obj->debug_print_ln(); // incl nl
		}
		obj->ref_decr();
	} else {
		// NULL is registered when it doesnt have to be
		// particularly by set/prop
		if(1) WARN("OK but obj==NULL");
	}
}

/*!
	copy constructor is to be avoided, BUT logged here
*/
obj_ref0::obj_ref0( const obj_ref0 & obj2 )
{
	ref_tally.n_newed ++;
#ifdef WITH_PYTHON
	ob_refcnt = 0;
#else
	ref_count = 0;
#endif
	// GC might need to put something here // flag_bits = 0;
	// e_print("copy_constructor()\n");
	WARN("copy_constructor()");
}


/*!
	should be very similar to the inline version
*/
void obj_ref0::ref_incr_fn()
{
	debug_check_CTOR_flag();
	ref_tally.n_incr ++;
	ref_count++;
}

/*!
	should be very similar to the inline version
*/
void obj_ref0::ref_decr_fn()
{
	debug_check_CTOR_flag();
	ref_tally.n_decr ++;
	ref_count--;
	if( ref_count <= 0 ) ref_zero();
}

/*!
	after an object has been FIXED static, this is called to track things

	ie the ref_count has jumped to a new value
*/
void obj_ref0::ref_jump_fn()
{
	buffer1 tn;
	INFO("%p %s ref_count=0x%x", this, type_name(tn), ref_count );
	debug_check_CTOR_flag();
}

// VIRTUAL - hash table default functions
uint obj_ref0::ht_get_hash()
{
	// the default is plain OBJECT ADDRESS
	// convert void * to int
	union {
		obj_ref0 * that;
		uint numb;
	} u ;
	u.that = this;
	return u.numb;
//	return reinterpret_cast<void *>(this);
//	return reinterpret_cast<uint>(this);
}

bool obj_ref0::ht_equals( obj_ref0 * other )
{
	return this == other;
}

/*
const char * obj_ref0_debug_base::type_name()
{
	const char * s = typeid(*this).name();

#ifdef HAVE_CXA_DEMANGLE
	// this is rumoured to do something
	// google for it later
	const char * name = s;
	char buf[1024];
	unsigned int size=1024;
	int status;
	char* res = abi::__cxa_demangle(
		name,
		buf,
		&size,
		&status
	);
	return res;
#endif


#if 0 
// NO THIS IS NOT WORKING
//	return s;
// with N9 namespace removed
	if(*s=='N') {
		int n = 0;
		int d1 = 0;
		int d2 = 0;
		s++;
		if(( ('0'<=*s) && (*s<='9') ) {
			d1 = *s - '0';
			s++
			n = d1;
			if(( ('0'<=*s) && (*s<='9') ) {
				d2 = *s - '0';
				s++
				n = (d1 * 10) + d2;
			}
			while(( n > 0 ) && *s ) {
				n--;
				s++;
			}
		}
	}
#endif

// with leading digits skipped  

	while( ('0'<=*s) && (*s<='9') ) s++;
	return s;
}
*/

// #include "w_widget0.h"

/*!
	this is giving way to debug_track_pointer();
	... but ... 
*/
void obj_ref0::debug_print()
{
	buffer1 tn;
	int r = get_ref_count();
	const char * s = "-";
	char buf[10];
	switch(r) {
	 case 0: s = "---0"; break;
	 case 1: s = "  1-"; break;
	 case 2: s = " 2--"; break;
	 case 3: s = "3---"; break;
	 default:
		sprintf( buf, "%-4d", r );
		s = buf;
	}
	e_print(" %s*", s );
	e_print(" %-12s", type_name(tn) );
 if(1)	e_print(" %9p", this );

	if( debug_test_is_widget() ) {
	//	w_widget0 * WID = reinterpret_cast<w_widget0*>( this );
		e_print(" %p", debug_get_widget_ptr() );
		e_print(" %s", debug_widget_type_name() );
	}
	// now call a virtual function that prints some useful desc
	// eg str2 prints 'its own value'
 if(1)	e_print(" " );
	prints();
}

// virtual
str2 * obj_ref0::debug_name_get()
{
	return NULL;
}

// virtual
void obj_ref0::debug_name_set( str2 * name )
{
	throw "debug_name_set";
}

bool obj_ref0:: DTOR_marked() // was it already set ?
{
	return debug_flags.test( DEBUG_OBJ_BIT_DTOR );
}

bool obj_ref0:: DTOR_mark() // set it and return: was it already set ?
{
	bool ret = debug_flags.test( DEBUG_OBJ_BIT_DTOR );
	if( ret ) return true;
	debug_flags.set( DEBUG_OBJ_BIT_DTOR );
	return false;
}

