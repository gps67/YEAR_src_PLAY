#include "obj_ref.h"
#include "e_print.h" // e_print - NOT

#include "obj_ref0_debug_base_PRIV.h"
#include "dgb.h"
#include "dgb_fail.h"
// #include <string.h> // debug_obj_name_for_now strdup
#include "missing_strndup.h"
#include "buffer1.h"

#include <typeinfo>

#define DO_PRNT false

/*
	thread sensitive updates to shared global lists
*/

	/*!
		a circular list of every object
	*/
	obj_ref0_debug_base * debug_list_root = NULL;

	/*!
		each object has an 'era'
	*/
	int debug_list_era = 0;

#ifdef REF_DEBUG_LIST


static void * debug_P_lo = NULL;
static void * debug_P_hi = NULL;

bool check_addr_range_28_fn( void * PTR ); // return true when OK

/*!
	check that PTR is within the first 256 MB - most things are

	NOTE that this is local to this file.cxx
*/
inline bool check_addr_range_28( void * PTR )
{
	if((long)PTR>>28) return check_addr_range_28_fn( PTR );
	else
		return true; // OK
}

/*!
	CTOR - remember this is all removable, debug helpers
*/
obj_ref0_debug_base::obj_ref0_debug_base()
{
	debug_obj_name_for_now = NULL;
	// calling STEP() or e_print invokes a circular wotsit
if(0)	fprintf(stderr,"ctor = %p\n", this );
//	fprintf(stderr,"CTOR = %p %s\n", this, type_name(tn) ); // all obj_ref0_debug_base
	debug_list_when = debug_list_era;
//	debug_item_flags = 0;
	debug_flags.set( DEBUG_OBJ_BIT_CTOR );
	if(!debug_list_root)
	{
		debug_list_root =
		debug_list_prev =
		debug_list_next =
			this;

		debug_P_lo =
		debug_P_hi =
			this;
		return;
	};
	obj_ref0_debug_base * C =    debug_list_root;
	obj_ref0_debug_base * A = C->debug_list_prev; // add to tail
	
	debug_list_prev = A;
	debug_list_next = C;

	A-> debug_list_next = this;
	C-> debug_list_prev = this;

	if(!check_addr_range_28( this )) {
	//	FAIL("%p ADDR >> 24 bits", this);
		// moving to debug_P_lo _hi
		// other values come from STATIC segment .bss init
		// which is what I am trying to avoid
		// but only gdb_invoke when actively tracking
		//
		// I am moving to forcing obj_hold on all obj_ref
		// ie NO STATICS (and also when in fields on struct)
		// this only catches the first (SP pointer is negative top)
		// it is then hard to find, maybe use a flag-bit-track
#warning make this a runtime option - invoke gdb for static obj
		if(0) // 28 not relevent
		 gdb_invoke(true);
		//
		// not adding the pointers to P_lo P_hi might call gdb_invoke
		// so simply pint the nagging lines, and add
		// that might include invalid ranges inbetween
		// think about a better segment layout test
	}
	if( this < debug_P_lo ) debug_P_lo = this;
	if( this > debug_P_hi ) debug_P_hi = this;

}

const char * obj_ref0_debug_base:: debug_get_obj_name()
{
// gcc says:
// nonnull argument ‘this’ compared to NULL [-Wnonnull-compare]
// but there is always the possibility ?
	// if(this == NULL ) return "((NULL))" ;
	if(debug_this_is_NULL()) THROW_dgb_fail("NULL this->debug_get_obj_name()");
	if(debug_this_is_NULL()) return("name_of_NULLL");
	if( debug_obj_name_for_now )
		return debug_obj_name_for_now;
	return "((OBJANON))";
}

void obj_ref0_debug_base:: debug_set_obj_name( const char * s )
{
#warning debug names leak like crazy, use virtual storer, into str1
	debug_obj_name_for_now = strndup( s, 100 );
if(0)	debug_track_pointer(); // it appears soon enough
}

/*!
*/
obj_ref0_debug_base::~obj_ref0_debug_base()
{
if(0)	fprintf(stderr,"DTOR = %p\n", this );
//	INFO("DTOR %p", this );

	/*
		FUTURE - allow apps to set the DTOR flag
		for when classes are reduced down to here
		or when DTOR is real imminent (added to a list)
		That would revent recursive DTORs
	*/
	if(debug_flags.test( DEBUG_OBJ_BIT_DTOR )) {
	 // ref_zero() places the DTOR flag - so if it is absent ...
	 if(debug_flags.test( DEBUG_OBJ_BIT_CTOR )) {
		// +DTOR +CTOR
		// OK app used DTOR flag on a valid object
	 } else {
		// +DTOR -ctor
		// BAD app used DTOR flag on a bad object
		// or this object is being deleted twice
		debug_check_CTOR_flag_fn(__FILE__,__FUNCTION__);
		return; // dont cause more corruption
	 }
	} else {
	 // ref_zero() places the DTOR flag - so if it is absent ...
	 bool CTOR = debug_flags.test( DEBUG_OBJ_BIT_CTOR );
	 FAIL("DTOR flag is missing CTOR=%s", str_true_false( CTOR ) );
 if(0)	 debug_track_pointer();
	 if(CTOR) {
	 	debug_check_CTOR_flag_fn(__FILE__,__FUNCTION__);
	 	return; // dont cause more corruption
	 }
	}

	if(!debug_flags.test( DEBUG_OBJ_BIT_CTOR ))
	{
		debug_check_CTOR_flag_fn(__FILE__,__FUNCTION__);
		return; // dont cause more corruption
	}

	debug_flags.set( DEBUG_OBJ_BIT_DTOR);
	debug_flags.clear( DEBUG_OBJ_BIT_CTOR);

	if( debug_list_root == this )
	{
 #define QUIETER 0
 if(QUIETER)
		INFO("%p  debug_list_root == this (good or unused)", this );
		if(0) list_all_objs_in_era( 0 );
		// gdb_invoke(true);
		// only one is necessary ||
		if( (debug_list_next == this ) &&
		    (debug_list_prev == this ) )
		{
 if(QUIETER)
			INFO("setting debug_list_root to NULL (too good to be true)");
			debug_list_root = NULL;
		} else {
			// this might be common for push pop
			INFO("setting debug_list_root to next");
			debug_list_root = debug_list_next;
		}
	}

	debug_list_prev->
	debug_list_next =
	debug_list_next;

	debug_list_next->
	debug_list_prev =
	debug_list_prev;

	debug_list_next =
	debug_list_prev =
	NULL ;

	if( debug_obj_name_for_now ) {
		* (char *) debug_obj_name_for_now = 'X'; 
		free( (void*) debug_obj_name_for_now );
		debug_obj_name_for_now = NULL;
	}
}

/*!
*/
// static 
void obj_ref0_debug_base::list_all_objs_in_era( int era )
{
	INFO("not printing a useless list");
	return;
	gdb_break_point(); // but its not easy to decode ref_obj *
	if( !debug_list_root )
	{
		e_print("debug_list_root is NULL\n");	
		return; // thats either amazing or incredible
	}
	obj_ref0_debug_base * R = debug_list_root;
	obj_ref0_debug_base * N = R->debug_list_next;
	obj_ref0_debug_base * O1 = NULL;
	while( O1 != R )
	{
		if(!N) { FAIL("NULL IN LIST"); gdb_invoke(true); }
		O1 = N;
		// some cast required, otherwise C++ deduces it MUST be this base
		// probably because there is no virtual function here
		N = N->debug_list_next;

		e_print(" when=%d", O1->debug_list_when );
		obj_ref0 * O = (obj_ref0*) O1;
		O->debug_print_ln();
	}

}

/*
	Print a 1 line summary of a VALID object
*/
bool obj_ref0_debug_base:: PRNT_fn(const char * file, const char * func)
{
	// stepwise to catch segfault in gdb
	// but since it is called by debug_check_pointer ...
	int rc = debug_get_ref_count();
	void * wp = debug_get_widget_ptr();
	buffer1 tn;
	const char * s_type_name = type_name(tn);
	const char * s_gtk_name = "-";
// #warning I need a way of detecting the following crash ...
// NOW I think it is a NULL g_class pointer
	if(wp) s_gtk_name = debug_widget_type_name();
	const char * s_obj_name = debug_get_obj_name();
	const char *  Q1 = "\'";
	if(!s_obj_name) Q1 = "";
	if(!s_obj_name) s_obj_name = "";
	if(1) {

		buffer1 buf;	
	 	buf.print("%9p %-11s *%d %-9p %s %s%s%s",
			this,		// 0x1a8d330
			s_type_name,	// str2
			rc,		// *0 = ref_count
			wp,		// 0x1a8d380 = ADDR of w
			s_gtk_name,	// NOT_A_WIDGET
			Q1,
			s_obj_name,	// ((OBJANON))
			Q1
		);

		fn_WARN(
			"PRNT",
			3,
			file,
			func,
			"%s",
			(STR0) buf
		);

/*
		INFO( "%s", (STR0) (str0) buf );
*/
	}
#if 0
	if(0) {
	// YUK
	if(s_obj_name) INFO( "%p = %s", this, s_obj_name );
#ifdef WIN32
	// %p not available ??
	// and maybe causes errno=9
	 INFO("0x%7.7x %-11s *%d 0x%7.7x %s",
#else
	 INFO("%9p %-11s *%d %-9p %s",
#endif
		this,
		s_type_name,
		rc,
		wp,
		s_gtk_name
	 );
	} else {
	 fprintf(stderr, "# TTY # %s this = %p %s rc=%d %p %s\n",
		__FUNCTION__,
		this,
		s_type_name,
		rc,
		wp,
		s_gtk_name
	 );
	}
#endif
	return true;
}

/*
	use by check range, soon to be moved
*/
bool check_addr_range_28_fn( void * PTR )
{
	if((long)PTR>>28) {
#ifdef __LP64__
		return true;
		WARN("%p exceeds 28 bits BUT THIS IS 64 bit?", PTR);
		return true;
#endif
		FAIL("%p exceeds 28 bits", PTR);
		return false;
	} else return true;
}



#ifdef __LP64__
/*
	This function does not even exist on non linux 64 bit ports
	gcc (and maybe others) defines	__LP64__
	x86_64
*/

int top_bit( void * PTR )
{
	int pos = 0;
	long P = (long) PTR;
	// ignore possible infinite -ve loop
	while( P ) {
		pos ++;
		P >>= 1;
	}
	return pos;
}

bool in_bit_range( int lo, int hi, void * PTR )
{
	if( !((long)PTR >> lo ) ) return false;
	if(  ((long)PTR >> hi ) ) return false;
	return true;
}

/*!
	STATIC to this file, 
	SPECIFIC to linux AMD64

	http://en.wikipedia.org/wiki/X86-64
	48 bits connected, top 16 must be 00 or FF (and match bit 47)

*/
bool check_addr_range_64( void * PTR )
{
	if(!PTR) return FAIL("NULL");
	// pointers are never signed in C, but obviously are in the CPU
	long P_signed = (long) PTR;

	// check top 16 bits
	int top16 = P_signed >> 48;
	if( top16 == 0 ) {
		// OK // 
		// PASS("top16 PTR = 0x%p", PTR );
	} else if( top16 < 0 ) {
		if( top16 == -1 ) {
			INFO("PTR %p", PTR );
			INFO("P is below zero");
			// check not in top page
			// such as offset within struct at zero
		} else {
			INFO("PTR %p", PTR );
			return FAIL("Illegal top 16 bits not FF (nor 0)");
		}
	} else {
		INFO("PTR %p", PTR );
		return FAIL("Illegal top 16 bits not 00 nor FF (is+)");
	}

	// check bottom 3 bits
	if( (long) PTR & 0x07 ) {
		return FAIL("PTR low 3 bits must be 0 = 0x%p", PTR );
	}

	if( !((long)PTR >> 23 ) ) {
		// too low
		INFO( "top bit is %d of %p", top_bit( PTR ), PTR );
		return FAIL("PTR too low 0x%p", PTR );
		
	}

	if( !((long)PTR >> 26 ) ) {
		// just right
		return true;
		return PASS("PTR topbit 23-%d-26 bits 0x%p", top_bit( PTR ), PTR );
		
	}

	if( in_bit_range( 45, 47, PTR )) {
		return true;
		return PASS("PTR topbit 45-%d-47 bits 0x%p", top_bit( PTR ), PTR );
	}

	gdb_break_point();

	INFO( "top bit is %d of %p", top_bit( PTR ), PTR ); // 24 is common

	/*
		-12 0xFFFFFc is definitely corrupted (taken from a NULL-12)

		The first page IS reserved by Linux, the last is unlikely.
	*/
	char * P_0 = 0L;
	char * P_M1 = P_0 - 4096;
	char * P_P1 = P_0 + 4096;
	char * P = (char *) PTR;
	
#if 1
	// this is correct as pointers are unsigned
	if((P_P1<P)&&(P<P_M1)) {
		// OK above P1 and below M2
	} else {
		if(0) {
			FAIL("corrupted pointer %p", PTR );
			gdb_invoke(true);
		}
		return FAIL("corrupted pointer %p", PTR );
		// actually want a throw
	}
#else
	// this is wrong as pointers are _SUPPOSED_TO_BE_ unsigned
	if((P_M1<P)&&(P<P_P1)) {
		if(0) {
			FAIL("corrupted pointer %p", PTR );
			gdb_invoke(true);
		}
		return FAIL("corrupted pointer %p", PTR );
		// actually want a throw
	}
#endif
	if( PTR < debug_P_lo ) {
		FAIL("corrupted or STACK pointer %p", PTR );
		gdb_invoke(true);
		return false;
	}
	if( PTR > debug_P_hi ) {
		FAIL("corrupted or STACK pointer %p", PTR );
		gdb_invoke(true);
		return false;
	}

#if 0
	// this is an approximate test, architecture dependent
	// and could break overnight, P_lo P_hi is better
	if(!check_addr_range_28( PTR )) {
	 if(0) {
		WARN("corrupted or STACK pointer %p", PTR );
		gdb_invoke(true);
		return false;
	 } else {
		return FAIL("corrupted or STACK pointer %p", PTR );
	 }
	}
#endif
	return true;
}
#endif // __LP64__

/*!
	OLD 32 version - fewer bit to check for bad patterns
*/
bool check_addr_range( void * PTR )
{
	if(!PTR) return FAIL("NULL");

	/*
		-12 0xFFFFFc is definitely corrupted (taken from a NULL-12)

		The first page IS reserved by Linux, the last is unlikely.
	*/
	char * P_0 = 0L;
	char * P_M1 = P_0 - 4096;
	char * P_P1 = P_0 + 4096;
	char * P = (char *) PTR;
	
#if 1
	// this is correct as pointers are unsigned
	if((P_P1<P)&&(P<P_M1)) {
		// OK above P1 and below M2
	} else {
		if(0) {
			FAIL("corrupted pointer %p", PTR );
			gdb_invoke(true);
		}
		return FAIL("corrupted pointer %p", PTR );
		// actually want a throw
	}
#else
	// this is wrong as pointers are _SUPPOSED_TO_BE_ unsigned
	if((P_M1<P)&&(P<P_P1)) {
		if(0) {
			FAIL("corrupted pointer %p", PTR );
			gdb_invoke(true);
		}
		return FAIL("corrupted pointer %p", PTR );
		// actually want a throw
	}
#endif
	if( PTR < debug_P_lo ) {
		FAIL("corrupted or STACK pointer %p", PTR );
		gdb_invoke(true);
		return false;
	}
	if( PTR > debug_P_hi ) {
		FAIL("corrupted or STACK pointer %p", PTR );
		gdb_invoke(true);
		return false;
	}

#if 1
	// this is an approximate test, architecture dependent
	// and could break overnight, P_lo P_hi is better
	if(!check_addr_range_28( PTR )) {
	 if(0) {
		WARN("corrupted or STACK pointer %p", PTR );
		gdb_invoke(true);
		return false;
	 } else {
		return FAIL("corrupted or STACK pointer %p", PTR );
	 }
	}
#endif
	return true;
}

#if 0
bool obj_ref0_debug_base::debug_validate_pointer()
{
#if 0
	// desperate?  print every call to tty not e_print subsystem
	fprintf(stderr, "# TTY # %s this = %p\n", __FUNCTION__, this );
	fflush(stderr);
#endif
	if( !debug_check_pointer() ) {
		// above does call FAIL but not PRNT
		return false;
	}
	// PRNT();
	return true;
}
#endif

bool obj_ref0_debug_base::debug_track_pointer_fn(const char * file, const char * func)
{
#if 0
	// desperate?  print every call to tty not e_print subsystem
	fprintf(stderr, "# TTY # %s this = %p\n", __FUNCTION__, this );
	fflush(stderr);
#endif
	if( !debug_check_pointer_fn(file,func) ) {
		// above check might call FAIL but not PRNT
		return false;
	}
	PRNT_fn(file,func);
	return true;
}

bool obj_ref0_debug_base::debug_check_prev()
{
	if(!debug_list_prev) {
		return FAIL("%p null debug_list_prev", this);
	}
	if(this == debug_list_prev) {
		bool last_ok = true;
		if( this != debug_list_root ) {
		 last_ok = FAIL("%p ( this != debug_list_root )", this );
		}
		if( this != debug_list_prev ) {
		 last_ok = FAIL("%p ( this != debug_list_prev )", this );
		}

		INFO("this/root/prev/next == %p/%p/%p/%p (SOLO ON LIST?)",
			this,
			debug_list_root,
			debug_list_prev,
			debug_list_next
		);
		return last_ok;
	}
	if(this != debug_list_prev->debug_list_next) {
		return FAIL("%p debug_list_prev->debug_list_next != this", this);
	}

	if(!debug_list_prev->debug_check_CTOR_flag() ) {
		return FAIL_FAILED();
	}
	return true;
}

bool obj_ref0_debug_base::debug_check_next()
{
	if(!debug_list_next) {
		return FAIL("%p null debug_list_next", this);
	}
	if(this == debug_list_next) {
		bool last_ok = true;
		if( this != debug_list_root ) {
		 last_ok = FAIL("%p ( this != debug_list_root )", this );
		}
		if( this != debug_list_next ) {
		 last_ok = FAIL("%p ( this != debug_list_prev )", this );
		}

		INFO("this/root/prev/next == %p/%p/%p/%p (SOLO ON LIST?)",
			this,
			debug_list_root,
			debug_list_prev,
			debug_list_next
		);
		return last_ok;
	}
	if(this != debug_list_next->debug_list_prev) {
		return FAIL("%p debug_list_next->debug_list_prev != this", this);
	}

	if(!debug_list_prev->debug_check_CTOR_flag() ) {
		return FAIL_FAILED();
	}
	return true;
}

/*!
*/
bool obj_ref0_debug_base::debug_check_pointer_fn(const char * file, const char * func)
{
	if(!debug_check_pointer_one( file, func )) return false;
	if(!debug_check_pointer_is_in_list( file, func )) return false;
	return true;
}

bool obj_ref0_debug_base::debug_check_pointer_one(const char * file, const char * func)
{
	bool ok = true;

// gcc complains
// nonnull argument ‘this’ compared to NULL [-Wnonnull-compare]
	/*
		NULL is not a valid pointer, but it might be

		FAIL will print a stack trace (not on WIN32)
	*/
	if(debug_this_is_NULL()) return FAIL("NULL");

	if(!check_addr_range_64(this)) 
		return FAIL("corrupted pointer %p", this );

//	if(!check_addr_range(this)) 
//		return FAIL("corrupted pointer %p", this );

	/*
		check that the CTOR flag is set (or magic value in a words)
		it is set and reset by the CTOR DTOR

		Plus for luck, check that the neighbours also have it!
	*/
	if(!debug_check_CTOR_flag() ) {
		ok = false; // return false;
		return debug_check_CTOR_flag_fn(file,func);
	}

	if(!debug_check_prev()) return FAIL_FAILED();
	if(!debug_check_next()) return FAIL_FAILED();

	/*
		problem (1) is that debug_check_pointer is called by
		w_widget0::C_event_resize (etc) within e_print_log
		which causes a line to be printed
		which resises the widget
		which soon after causes another C_event_resize
		so use fprintf(stderr) instead of INFO_
		...

		problem(2) ref_count is obj_ref0 NOT obj_ref0_debug_base
		so cast it, check that, knowing it MUST be obj_ref0
		(or get debug_get_ref_count() to do that for us)
		...
		problem(0) bad pointer - is why we are here, and no VTABLE
		Because of a double delete, or whatever, the VTBL is naff
		so cannot use it to print type_name() without crashing
		Something similar for widget(w) being non null but corrupt
		(eg it has been deleted already, AND finalised)
		If we are going to crash, print something
		Maybe have a catch signal that prints a buffer ?
		Maybe ask libgcc for a has_valid_vtbl_ptr(obj)
	*/

	/*
		Getting these values one at a time,
		means a crash will show the other values ;-)
	*/
	// if(ok)	PRNT();
	if(!ok) return FAIL_FAILED();

	return true;  // all OK
}

bool obj_ref0_debug_base::debug_check_pointer_is_in_list(const char * file, const char * func)
{

	/*
		now search the list for the item
	*/
	obj_ref0_debug_base * R = debug_list_root;
	if(R) {
		obj_ref0_debug_base * N = R->debug_list_next;
		obj_ref0_debug_base * O1 = NULL;
		while( O1 != R )
		{
			if(!N) {
				FAIL("NULL IN LIST");
				gdb_invoke(true);
			}
			if(!N->debug_check_pointer_one(file,func)) {
				FAIL("WHILST SCANNING LIST");
				gdb_invoke(true);
			}
			O1 = N;
			N = N->debug_list_next;

			if(O1 == this ) return true; // poss CTOR flag
		}
	}
	FAIL("ITEM %p not found in list", this);

	((obj_ref0*) this) -> debug_print_ln();
 if(0)	gdb_break_point(); // but its not easy to decode ref_obj *
 else	gdb_invoke(true); // but its not easy to decode ref_obj *
	return false;
}

/*!
	CTOR and DTOR set and clear a flag for debugging
*/
bool obj_ref0_debug_base::debug_check_CTOR_flag_fn(const char * file, const char * func)
{

	if(!debug_flags.test( DEBUG_OBJ_BIT_CTOR ))
	{
		FAIL("%p CTOR flag not set - already DTOR'd or random pointer?", this);
		gdb_invoke(true);
		list_all_objs_in_era(0);
		return false;
	}
#if 1
	if(!debug_obj_name_for_now) {
		buffer1 tn1;
		debug_set_obj_name( type_name( tn1 )); 
		
	}
#endif
	return true;
}

/*!
*/
// VIRTUAL
void obj_ref0_debug_base:: debug_event_on_delete()
{
	WARN("connected but no method provided - default is NOOP");
	fprintf(stderr, "# NOOP # # obj_ref # debug_event_on_delete() ");
	fflush(0);
	debug_print_ln();
	fflush(0);
}

/*!
*/
void obj_ref0_debug_base:: debug_connect_event_on_delete()
{
	debug_flags.set( DEBUG_OBJ_BIT_on_delete );
}
/*!
*/
void obj_ref0_debug_base:: debug_ref_zero()
{
	if(debug_flags.test( DEBUG_OBJ_BIT_on_delete ))
	{
		STEP("calling debug_event_on_delete()");
		debug_event_on_delete();
	}
}
/*!
	The C++ type name of this, slightly cleaned up for readability
*/
const char * obj_ref0_debug_base::type_name(buffer1 & tn)
{
        const char * s = typeid(*this).name();
	int pos = tn.gap_offs(); // pos where s will be placed
	demangle_cpp_symbol( tn, s );
	return (char *) tn.addr_of_offset(pos);
// with leading digits skipped
        while( ('0'<=*s) && (*s<='9') ) s++;
        return s;
}

void obj_ref0_debug_base::debug_print_ln()
{
	debug_print();
	const char * n = debug_get_obj_name();
	if(!n) n = "(anon)";
	e_print(" '%s'\n", n );
//	e_print("\n");
}

void obj_ref0_debug_base:: debug_set_is_widget()
{
	debug_flags.set( DEBUG_OBJ_BIT_is_widget );
}

bool obj_ref0_debug_base:: debug_test_is_widget()
{
	return debug_flags.test( DEBUG_OBJ_BIT_is_widget );
}

//virtual
const char *  obj_ref0_debug_base:: debug_widget_type_name()
{
	return "NOT_A_WIDGET"; // or use NULL // or other groups
}

#include "w_widget0.h"
void *  obj_ref0_debug_base:: debug_get_widget_ptr()
{
	/*
		libbase1 can be compiled without any GTK around
		so why is the .h available to this function ?

		... dunno ...

		It is only really needed to print w from validate_
		which will fail when VTBL is corrupt :-(

		... then .. make this a virtual function
	*/
//#ifdef WITH_GTK2
	w_widget0 * WID = reinterpret_cast<w_widget0*>(this);
	return WID->w;
//#else
//	return NULL;
//#endif
}

int obj_ref0_debug_base:: debug_get_ref_count()
{
	obj_ref0 * o = reinterpret_cast<obj_ref*>(this);
	return o->get_ref_count();
}

#endif

