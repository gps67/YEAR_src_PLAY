#ifndef DYN_ARRAY_TEMPLATE
#define DYN_ARRAY_TEMPLATE
/*

	Whats needed here is a set of ASM arith ...
*/

#include "blk1.h"
#include "new_insitu.h"

extern void dyn_array_index_error( int i, int n );

/*!
	dyn_array - A dynamic array of C++ objects.

	element type can be ANY type (not just pointers)
	element type T must have a default constructor,
	element type T must be relocatable (no pointers to it)
	element type T MAY have non-virtual destructor (type known)
	sizeof(element type) must be as aligned as expected (OK char is 1)

	get_space(n) here means elements not bytes

	DO NOT take the address of an item, or array, without great care.
	DO NOT take the address by accident (usully safe, but ... )

	For GDB users, there is a fake array of 20 objects.
	You must not use this field, as it will be deleted.

	TODO BUG: (SYNTAX) (FORGOT)
	there was something with appending new elements with a value,
	which was avoided by creating a blank then assigning.

*/
template <class T>
struct dyn_array : public GRP_lib_base
{

	//!	declare a type for the fake array
	typedef T T_fake[20];

	//!	point to the fake array - a pointer to blk.buff
	T_fake * fake;

	//! update fake pointer whenever blk.buff changes
	void set_fake()
	{
		fake = (T_fake*) blk.buff;
	}


	//! The memory used (not a base class - avoid confusion)
	blk1 blk;	// raw memory
	
	//! The CALCULATED number of elements
	int N()
	{
		return (blk.nbytes_used / sizeof( T ) );
	}

	//! pointer to the the plain array
	T * ary_base()
	{
		return (T *) blk.buff;
	}

	//! get element[i] with mild upper-bounds checking
	T & operator[]( int i )
	{
		if(1)
		{
			int n = N();
			if( !(i<n) )
			{
				dyn_array_index_error( i, n );
			}
		}
		return ary_base()[i];
	}

	//! clear down to zero, but dont delete the blk1 memory
	void rezero()
	{	// keep alloc though
		extend_to(0);
	}

	//! clear down to zero, but dont delete the blk1 memory
	void clear()
	{
		extend_to(0);
	}

	//! extend_to( nn ) - grows or truncates array
	void resize( int nn )
	{
		extend_to( nn );
	}

	//! reduce the memory to the exact size
	void trim()
	{
		blk.trim_size();
	}

	//! constructor
	dyn_array()
	: blk()
	{
		set_fake();
	}

	//! destructor - delete each item
	~dyn_array()
	{
		int n = N();
		T * fs = ary_base();
		for( int i = 0; i < n; i++ )
		{
			DEL_ITEM( &fs[i], T );
		}
	}

	//! obtain space for # more elements, BUT DONT INITIALISE THEM
	bool get_space( int gap )
	{
		int nb = gap * sizeof( T );
		bool ok = blk.get_space( nb );
		set_fake();
		return ok;
	}

	//! obtain space for # elements total
	bool get_space_for_total( int nn )
	{
		int n = N();
		return get_space( nn - n );
	}

	//! how many items can be added without a realloc
	int space_avail()
	{
		return blk.space_avail() / sizeof( T );
	}

	//! AVOID - get pointer to position past end of real elements (LURK)
	T * gap_addr()
	{
		uchar * P1 = blk.buff + blk.nbytes_used;
		return (T*) P1;
	}

	//! AVOID - deprecated - // the gap item has not had constructor called
	T & gap_item()
	{
		return *gap_addr();
	}

	//! extend/reduce the array - calling vanilla constructor or destructor
	bool extend_by( int dn )
	{
		// reserve/obtain space, realloc early (retain excess)
		if(!get_space( dn )) return FALSE;
		// gap address is now fixed (after realloc)
		T * p1 = gap_addr();
		// directly reserve space in the blk
		blk.nbytes_used += sizeof(T) * dn;
		if( dn < 0 )
		{
			// destroy #dn elements from NEW gap_addr to top
			p1 = gap_addr();
			for( int i=-dn; i>0; i-- )
			{
				// call destructor without releasing memory
				// DEL_ITEM( p1, T );
				p1->T::~T();
				p1 ++;
			}
		} else {
			// construct #dn new elements from OLD gap to top
			for( int i=0; i<dn; i++ )
			{
				// call constructor without allocating memory
				// RAW_INIT( p1, T, () );
				new (p1) T();
				p1 ++;
			}
		}
		return TRUE;
	}

	//! construct or delete elements, leaving nn - calls extend_by()
	bool extend_to( int nn )
	{
		return extend_by( nn - N() );
	}


	//! extend and initialise with a (copied) value
	bool extend_to_fill( int n2, const T & val )
	{
		int n1 = N(); // retain n1 before it changes
		// extend array calling the VANILLA constructor
		if( !extend_to( n2 ) ) return FALSE;

		// another way of calculating addresses - fast
		T * fs = ary_base();
		for( int i=n1; i<n2; i++ ) {
			fs[ i ] = val;
		}
		return TRUE;
	}


	//! extend and initialise with a (copied) value 
	bool extend_by_fill( int dn, const T & val )
	{
		return extend_to_fill( N() + dn, val );
	}


	//! append a value (calls null constructor then =)
	bool append( const T & val )
	{
		int pos = N();
		if(!extend_by(1)) return FALSE;
		// this->operator[](pos) = val;
		// this is not copy constructor, but subsequent operator=
		// but it doesnt work with T===int ??
		// this->operator[pos] = val;
		ary_base()[pos] = val;;
		// T * addr = & this[pos];
		// *addr = val ;
		return TRUE;
	}
	

	//! insert a (NULL) constructed value at pos
	bool ins( int pos )
	{
		if(!blk.get_space( sizeof(T) )) return FALSE;
		T * fs = ary_base();
		T * p1 = &fs[ pos ];
		T * p2 = &fs[ pos + 1 ];
		T * p3 = gap_addr();
		int nb = (char *)p3 - (char *)p1; // bytes not quads or other
		if(nb) memmove( p2, p1, nb );
		blk.nbytes_used += sizeof(T);
		RAW_INIT( p1, T, () );
		return TRUE;
	}

	//! ins a new object, then assign it
	bool ins( int pos, const T & val ) // default ctr then copy=
	{
		if(!ins( pos )) return FALSE;
		*this[pos] = val ;
		return FALSE;
	}


	//! delete item at pos
	bool del( int pos )
	{
		T * fs = ary_base();
		T * dst = &fs[ pos ];		// the item being deleted
		T * src = &fs[ pos + 1 ];	// the item above
		T * end = gap_addr();		// beyond the end
		DEL_ITEM( dst, T );
		// if kept as T* array match gives the delta index
		int nb = (char *)end - (char *)src;	// number of bytes
		if(nb) memmove( dst, src, nb );
		blk.nbytes_used -= sizeof(T);
		return TRUE;
	}

/*
	Caller can maintain N (and gap), then use dary[i] (i<n2)
	This is for when you want to have the uncomitted counted
	instead use gap for uncomitted

	T & get_last()
	{
		if( !blk.nbytes_used ) {
			printf(stderr,"get_last - but empty array\n" );
			return;
		}
		uchar * P1 = blk.buff + blk.nbytes_used - sizeof( T );
		T* P = (T*) P1;
		return *P;
	}

	void set_last( const T & f ) {
		get_last() = f;
	}
*/

};

#endif
