#ifndef OBJ_list_base_H
#define OBJ_list_base_H

#include "dyn_array.h"
#include "obj_ref.h"
#include "obj_hold.h"

class sort_spec_base;

/*
	This is like a dyn_array but ALWAYS uses pointers to objects
*/

/*!
	a list of obj_ref pointers, all of same type

	The list itself is NOT a ref counted object,
	but each slot is. It uses a dynamic array of
	obj_jold<obj_ref>, which can be resized (etc),
	and those objects are reduced when this object is
	destroyed. 
*/
class obj_list_base : public GRP_lib_base
{
	typedef obj_hold<obj_ref> hold;
	dyn_array<hold> holder;
 public:

	obj_list_base();
	~obj_list_base();

	//! The number of elements in the array
	int N()
	{
		return holder.N();
	}
	

	//! extend the array by one element
	void append( obj_ref * p );

	//! extend the array by one element
	void append_if_absent( obj_ref * p );

	//! set array slot i to p (no bounds checking)
	void set( int i, obj_ref * p );

	//! get array slot i (no bounds checking)
	obj_ref * get( int i );

	//! insert a pointer at array slot i
	void ins( int _i, obj_ref * val = NULL );

	//! delete array slot i (and move following down)
	void del( int _i );

	//! search for and delete the FIRST instance of val, return was_found
	bool del_val( obj_ref * val ); // only the first instance

	//! search for and delete the FIRST instance of val, return was_found
	bool del_val_if_present( obj_ref * val ); // quieter

	//! access array slot i directly
	hold & operator[]( int i )
	{
		return holder[i];
	}

	//! ensure space exists for future expansion (usable?)
	bool get_space( int n );

	//! how much space is available within current malloc
	int space_avail();

	//! clear down array but retain allocated memory
	void rezero();

	//! resize the array, deleting pointers or adding NULL
	bool resize( int nn );

	//! search for FIRST value, set where its found, return was_found
	bool find_ptr( obj_ref * p, int & idx );

	void sort( sort_spec_base * sort_spec );

	//! append val to stack
	void stack_push( obj_ref * val )
	{
		append( val );
	}

	//! del top of stack (doesnt return it!)
	void stack_pop()
	{
		del( N() );
	}

	//! returns top of stack
	obj_ref * stack_top()
	{
		int n = N();
		if(!n) {
			// THROW_dgb_fail("stack_top() empty stack");
			return NULL;
		}
		return holder[n];
	}

};

#endif
