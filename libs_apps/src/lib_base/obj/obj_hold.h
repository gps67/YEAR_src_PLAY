#ifndef OBJ_HOLD1_H
#define OBJ_HOLD1_H

#include "obj_ref.h"

#ifndef NULL
#define NULL 0
#endif

// BASE * is (obj_ref *)

/*!
	obj_hold<BASE> holder;
	
	is a reference counting holder for a pointer to BASE or
	any derived class. Beware of circular links (tree_leaf to tree_root).

	BASE does not have to be fully defined, until you use the funcs.

		NOT TRUE:

			If you try to use the default constructor on
				obj_hold<T> field;
			seems new gcc sees set(p) p->ref_incr()
			and wants the type of p to be resolved

			But if you make the surrounding constructor
			a non-inline function (ie not auto-default,
			and with a full #include) its OK.

			It might be possible to surround those ref_incr()
			calls with casts to (obj_ref*), but that would
			need the cast to be *checked* - so no incomplete type


	BIG BUG
	BIG BUG
	BIG BUG
	BIG BUG
			obj_hold<type1> var1 = val1;
	BIG BUG
	BIG BUG
	BIG BUG
	BIG BUG
			obj_hold<type1> var1; var1 = val1;

	GDB:
			there are still some issues with gdb/insight
			needing a lot of clicks to open up a smart pointer
			(particularly the private zone being folded!)
*/
template <class BASE>
class obj_hold : public GRP_lib_base
{
 public:	// makes gdb/insight a bit easier // but should be private
	BASE * ptr;
 public:

	/*!
		The destructor installs the NULL pointer early,
		which prevents any loops.
	*/
	~obj_hold()
	{
		set( NULL );
	}

	/*!
		set obj_hold to p
	*/
	void set( BASE * p )
	{
		/*
			do incr before decr - incase its the same object!
		*/
		if( p )  p->ref_incr();
		/*
			store ptr before calling ptr_old->ref_decr->destructor
			particularly useful with NULL and circular loops
		*/
		BASE * ptr_old = ptr;
		ptr = p;
		/*
			ref_decr() can call ref_zero() which deletes the object
		*/
		if( ptr_old ) ptr_old->ref_decr();
	}

	/*!
		take object from rhs, which is set to NULL

		this is slightly faster than multiple ref_incr + ref_decr

		lhs = rhs
		rhs = NULL

		take2( rhs, NULL)
	*/
	void take( obj_hold & rhs )
	{
		// take2( rhs, NULL )
		BASE * p_old = ptr;
		ptr = rhs.ptr;
		rhs.ptr = NULL;
		if( p_old ) p_old->ref_decr();
	} 

	/*!
		take object from rhs, which is set to rhs2

		rhs2 is a value not a variable

		this is slightly faster than multiple ref_incr + ref_decr

		lhs = rhs
		rhs = rhs2

		rename: var1.take_and_place( var2, val3 )
	*/
	void take2( obj_hold & rhs, BASE * rhs2 )
	{
		BASE * p_old = ptr;
		ptr = rhs.ptr;
		rhs.ptr = rhs2;
		if( rhs2 ) rhs2->ref_incr();
		if( p_old ) p_old->ref_decr();
	} 

	//! constructor - default is NULL
	obj_hold()
	{
		ptr = NULL;
	}

	//! AVOID - 
	void raw_init()
	{
		ptr = NULL;
	}

	//! constructor - assign new value
	obj_hold( BASE * p )
	{
		ptr = NULL;
		set( p );
	}

	/*!
		Copy constructor.

		I previously avoided this (checked with printf)
		now it is going to be used extensively in return types.
		The alternative is a dangerous dance with ref_count AT zero,
		or passing an obj_hold as a reference parameter.
	*/
	obj_hold( const obj_hold & hold2 )
	{
		// fprintf(stderr,"# HOLD CTOR( hold2 ) \n" );
		ptr = NULL;
		set( hold2.ptr );
	}

	//! This should not be necessary ...
	BASE * get_ptr()
	{
		return ptr;
	}

	//! assign a new value and return this variable
	obj_hold & operator=( const obj_hold & h )
	{
		// fprintf(stderr,"# HOLD op=hold \n" );
		set( h.ptr );
		return *this;
	}

	//! assign a new value and return this variable
	obj_hold & operator=( BASE * p )
	{
		// fprintf(stderr,"# HOLD op=BASE*\n" );
		set( p );
		return *this;
	}

	/*
		These make obj_hold< X > look a lot like X * 
	*/

	//! return the value - as if this was a pointer
	BASE * operator->() const
	{
		return (BASE *) ptr;
	}

	//! return the value - as if this was a pointer
	operator BASE *() const
	{
		return (BASE *) ptr;
	}

	/*!
		atomic on this, fast on other, swap values
		((TODO: find an atomic XCHG CPU opcode in C, they are all CMP ))

		Swap the two held pointers, no change of ref_counts.
		It is atomic for THIS (or should be), not for var_other.

		This fn prototype helps to avoid refcounts being lost
	*/
	void atomic_swap_other( obj_hold & var_other )
	{
		BASE * mid = (BASE *) ptr;
		ptr = (BASE *) var_other.ptr;
		var_other.ptr = mid;
	}

};

#endif
