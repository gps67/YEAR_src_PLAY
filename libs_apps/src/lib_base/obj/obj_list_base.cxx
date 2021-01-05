#include "dyn_array.h"
#include "obj_ref.h"
#include "obj_list_base.h"
#include "obj_list_sorter.h"

//! a list of obj_ref pointers, PROBABLY all of same type
/*!
	The list itself is NOT a ref counted object,
	but each slot is. It uses a dynamic array of
	obj_jold<obj_ref>, which can be resized (etc),
	and those objects are reduced when this object is
	destroyed. 
*/

obj_list_base::obj_list_base()
: holder()
{
}

obj_list_base::~obj_list_base()
{
 if(0) // this is NOT necessary
	for( int i=0; i<N(); i++) 
		holder[i].set( NULL );
}

#if 0
//! The number of elements in the array
int obj_list_base::N()
{
	return holder.N();
	
}
#endif

bool obj_list_base::resize( int nn )
{
	holder.resize( nn );
	return true; // LURK void in base class
}

//! extend the array by one element
void obj_list_base::append_if_absent( obj_ref * p )
{
	int pos = 0;
	if( !find_ptr( p, pos ) ) append( p );
}

//! extend the array by one element
void obj_list_base::append( obj_ref * p )
{
	int n = N();
	holder.extend_by( 1 );
	holder[n].set( p );
}

//! set array slot i to p (no bounds checking)
void obj_list_base::set( int i, obj_ref * p )
{
	holder[i].set( p );
}

//! get array slot i (no bounds checking)
obj_ref * obj_list_base::get( int i )
{
	return (obj_ref *) holder[i]; // .ptr;
}

//! insert a NULL at array slot i
void obj_list_base::ins( int i, obj_ref * p )
{
	holder.ins( i );
	holder[i].set( p );
}

//! delete array slot i (and move following down)
void obj_list_base::del( int _i )
{
	holder.del( _i );
}

//! search for and delete the FIRST instance of val, return was_found
bool obj_list_base::del_val_if_present( obj_ref * val ) // only the first instance
{
	int idx = 0;
	if(! find_ptr( val, idx ))
	{
		return FALSE;
	}
	holder.del( idx );
	return TRUE;
}

//! search for and delete the FIRST instance of val, return was_found
bool obj_list_base::del_val( obj_ref * val ) // only the first instance
{
	int idx = 0;
	if(! find_ptr( val, idx ))
	{
		if(1) e_print("obj_list::del_val - didnt find val\n");
		// no callable print() in these templates :-(
		return FALSE;
	}
	holder.del( idx );
	return TRUE;
}

#if 0
//! access array slot i directly
obj_hold<obj_ref> & obj_list_base::operator[]( int i )
{
	return holder[i];
}
#endif

//! ensure space exists for future expansion (usable?)
bool obj_list_base::get_space( int n )
{
	/*
		Does this prepare space for allocation
		or fill it in - not sure?
	*/
	return holder.get_space(n);
}

int obj_list_base::space_avail()
{
	return holder.space_avail();
}

//! clear down array but retain allocated memory
void obj_list_base::rezero()
{
	holder.rezero(); // retain alloc though
}

//! search for FIRST value, set where its found, return was_found
bool obj_list_base::find_ptr( obj_ref * p, int & idx )
{
	int n = N();
	for( int i = 0; i<n; i++ )
	{
		if( holder[i] == p ) 
		{
			idx = i;
			return TRUE;
		}
	}
	idx = 0;
	return FALSE;
}

/*!
	Sort this list by sort_spec, which MUST be suitable.
*/
void obj_list_base::sort( sort_spec_base * sort_spec )
{
	obj_list_sorter::sort( this, sort_spec );
}


