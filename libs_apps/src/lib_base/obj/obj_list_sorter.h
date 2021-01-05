#ifndef obj_list_sorter_H
#define obj_list_sorter_H

#include "obj_list_base.h"
#include "sort_spec_base.h"

/*!
	obj_list_sorter sorts an obj_list_base using a sort_spec
	This class is only used by obj_list_base.cxx, you
	define your sort_spec and call list.sort(spec).
*/
class obj_list_sorter  : public GRP_lib_base
{
 public:
	//! This is the only exposed function, used by obj_list_base
	static void sort( obj_list_base * list, sort_spec_base * sort_spec1 );
 private:
	//! The obj_list is ref_counted for correctness
	obj_list_base keeper;	// keeps obj_incr() +=1 else const

	//! The array of pointers with less inline access
	/*!
		Elements are individually copied into/from array,
		but when there, the compiler can use internal
		pointer arithmetic, which should be faster.
	*/
	obj_ref ** array;

	//! The pointer to the compare function
	/*!
		sort_spec has a virtual function that returns this value
		and it is stored here for fast access. It is still a
		member function (pointer plus offs instead of one),
		which allows the sort_spec to use its own fields.
	*/
	sort_spec_base::MFN_do_cmp mfn_do_cmp;

	//! The object that defines how two elements compare
	/*!
		mfn_do_cmp is a member function of sort_spec, which
		can have other data fields, such as a reverse flag,
		and most importantly the SUB-FIELD of the row
		which is used to compare objects.
	*/
	sort_spec_base * sort_spec;

	obj_list_sorter( sort_spec_base * sort_spec1 )
	{
		array = NULL;
		sort_spec = sort_spec1;
		mfn_do_cmp = sort_spec -> get_do_cmp();
	}

	~obj_list_sorter()
	{
		if( array ) free(array);
		array = NULL;
		// keeper also gets deleted
	}

	void swap( int i, int j )
	{
		if( i == j ) return;
		obj_ref * T = array[i];
		array[i] = array[j];
		array[j] = T;
	}

	//! call the member function in the context of the sort_spec
	/*!
		A member function uses 8 bytes instead of 4 (offset ofX)
		so it must be slower somewhere, but you get the advantage
		of (sort_spec*that) being passed and available.

		It isnt a virtual function, so thats a slight saving.

		This inline saves repeating the syntax.
	*/
	IS_DIFF_t call_cmp( obj_ref * a, obj_ref * b )
	{
		return (sort_spec->* mfn_do_cmp)( a, b );
	}

	bool more( int i, obj_ref * v )
	{
		return IS_MORE==call_cmp( array[i], v );
	}

	bool less( int i, obj_ref * v )
	{
		return IS_LESS==call_cmp( array[i], v );
	}

	bool more( int i, int j )
	{
		return IS_MORE==call_cmp( array[i], array[j] );
	}

	bool less( int i, int j )
	{
		return IS_LESS==call_cmp( array[i], array[j] );
	}

	//! copy the array of pointers from data
	void set_data( obj_list_base & data );
	//! copy the array of pointers back to data
	void get_data( obj_list_base & data );
	//! sort calls QuickSort with the initial bounds
	void sort();
	//! a recursive implementation of quick-sort over a sub-zone
	void QuickSort( int low, int high);
};
#endif
