#include "obj_list_sorter.h"

/*!
	Copy each pointer from the obj_list_base to array.
	Also keep another copy in keeper (ref_incr), so that
	nothing is deleted on the way back.

	An alternative to keeper would be ref_incr_all and
	ref_decr_all, and the absolute need for the array
	to not change (other threads).
*/
void obj_list_sorter::set_data( obj_list_base & data )
{
	int n = data.N();
	array = (obj_ref **) malloc( sizeof(obj_ref*) * n );
	// if not(array) throw tantrum;
	keeper.get_space( n );
	for(int i=0; i<n; i++)
	{
		obj_ref * ptr = data[i];
		array[i] = ptr;
		keeper.append( array[i] );
	}
}

/*!
	copy the sorted data back to the original obj_list_base data,
	which must have the same size as before.
*/
void obj_list_sorter::get_data( obj_list_base & data )
{
	int n = keeper.N();
	for(int i=0; i<n; i++)
	{
		data.set(i, array[i] );
	}
}

/*!
	call QuickSort() with the full range
*/
void obj_list_sorter::sort()
{
	if( ! sort_spec ) {
		e_print("sort_spec is NULL\n");
		return;
	}
	if( ! mfn_do_cmp ) {
		e_print("do_cmp is NULL\n");
		return;
	}
	if( ! array ) {
		e_print("array is NULL\n");
		return;
	}
	QuickSort( 0, keeper.N() - 1);
}

/*!
	A standard version of QuickSort, but using sort_spec
	(amd any additional parameters it holds) which is
	not a feature of libc's quicksort.

	It picks a sample value from the middle, and sweeps left
	and right inwards, so that the left are is LESS than sample,
	the right area is MORE than sample, and the middle area is empty.

	Any 'conflict' or values too-big-small, is resolved by swapping those
	two obstacles, placing them inthe other zone where they fit.

	When the middle point is reached (next to the sample value),
	both sides are recursively sorted/swept. Its good for pre-sorted,
	reverse sorted, and for completely random input data, there is
	a reasonable chance of picking a sample value which isnt close
	to the ends.

	Multiple items EQUAL to sel can be on EITHER side, but sorting
	will bring them back together at the join line.
*/
void obj_list_sorter::QuickSort( int low, int high) 
{
	if( !(low < high) ) return; // done, zero or one
	if(0) e_print("QuickSort(%2d,%2d)\n", low, high );
	int lo = low;
	int hi = high;
	int sel = ( low + high ) / 2 ; // pick a sample value
	obj_ref * val = array[sel];

	while( lo <= hi )
	{
		while( ( lo <= hi ) && ( less( lo, val ) ))
			lo++;
		// everything below lo is less than sel[]
		// subsequant swaps allow EQUAL
		// thing AT lo >= sel -- or outside
		// OR lo == high

		while( ( lo <= hi ) && ( more( hi, val ) ))
			hi--;
		// everything above hi is more than sel[]
		// thing AT hi <= sel -- or outside
		// OR equal - from below

		// it stops because of no more data - or
		// because of ==sel[] or excess
		// so put both stop values in the other region
		if( lo <= hi ) 
		{
			// both stopped due to excess value or ==
			if( lo != hi ) swap( lo, hi );
			// after swap BOTH are within the others range
			lo++;
			hi--;
		}
		// now left is <= sel
		// now right >= sel
		// loop until only left+right, no mid zone
	}
	// hi < lo // !!

	// recursively sort the left
	if( low < hi )
		QuickSort( low, hi );

	// and the right
	if( lo < high )
		QuickSort( lo, high );
}

/*!
	This does it all, and should be the only public function.

	Because of the base type of (obj_ref*), it is imperative
	that YOU ensure the sort_spec is suitable.
*/
// static
void obj_list_sorter::sort( obj_list_base * list, sort_spec_base * sort_spec )
{
	obj_list_sorter sorter( sort_spec );
	sorter.set_data( *list );
	sorter.sort();
	sorter.get_data( *list );
}

