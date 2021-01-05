#ifndef OBJ_list_H
#define OBJ_list_H

#include "dyn_array.h"
#include "obj_ref.h"
#include "obj_hold.h"
#include "obj_list_base.h"

// class sort_spec_base;

//! a dynamic array of obj_hold pointers to BASE 
/*!
	The code has been moved to obj_list_base, which
	requires BASE to be derived from obj_ref, and this
	class is a TEMPLATED inline wrapper which does type casts.

	That means that some array access is no-longer inline,
	but I distrust templates, when there is no need.

	Commented out functions are inherited without type cast.

	NB: This is not itself an obj_ref. Previously the idea
	was to get better inlining access to the array access,
	that could be re-exposed. You are supposed to place
	several of these in your object derived from obj_ref.
*/

template <class BASE>
class obj_list : public obj_list_base
{
	typedef obj_hold<BASE> hold;
 public:

	obj_list()
	: obj_list_base()
	{
	}

	~obj_list()
	{
	}

	//! extend the array by one element
	void append( BASE * p )
	{
		obj_list_base::append((obj_ref *) p);
	}

	void clear()
	{
		obj_list_base::rezero();
	}
	void rezero()
	{
		obj_list_base::rezero();
	}

	//! set array slot i to p (no bounds checking)
	void set( int i, BASE * p )
	{
		obj_list_base::set( i, (obj_ref *) p);
	}

	//! get array slot i (no bounds checking)
	BASE * get( int i )
	{
		return (BASE *) obj_list_base::get( i );
	}

	//! search for and delete the FIRST instance of val, return was_found
	bool del_val( BASE * val ) // only the first instance
	{
		return obj_list_base::del_val( (obj_ref *) val );
	}

	//! search for and delete the FIRST instance of val, return was_found
	bool del_val_if_present( BASE * val ) // only the first instance
	{
		return obj_list_base::del_val_if_present( (obj_ref *) val );
	}

	//! access array slot i directly
	hold & operator[]( int i )
	{
		return (hold &) obj_list_base::operator[]( i );
	}

	//! search for FIRST value, set where its found, return was_found
	bool find_ptr( BASE * p, int & idx )
	{
		return obj_list_base::find_ptr( (obj_ref *) p, idx );
	}

	// INHERITED // void sort( sort_spec_base * sort_spec );

	BASE * stack_pop( hold & top )
	{
		top = stack_top();
		stack_pop();
		return (BASE *) top;
	}

	//! append val to stack
	void stack_push( BASE * val )
	{
		obj_list_base::stack_push( (obj_ref*) val );
	}

	//! del top of stack (doesnt return it!)
	void stack_pop()
	{
		obj_list_base::stack_pop();
	}

	//! returns top of stack
	BASE * stack_top()
	{
		return (BASE *) obj_list_base::stack_top();
	}

};

#endif
