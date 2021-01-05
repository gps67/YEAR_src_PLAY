#include "dyn_array.h"
#include "e_print.h"

void dyn_array_index_error( int i, int n )
{
	if( !( i < n ) ) {
		e_print( "dyn_array[%d] when N==%d\n", i, n );
		gdb_break_point();
	}
}
