#include "M_z_N.h"
#include <assert.h>
#include <string.h> // memmove
#include "dgb.h"

	bool
	L_M_z_N_R_32::
	ALLOC_PLUS( IDX_int & V ) { 
		// assert that NBITS is plenty for R-L
		// when we alloc more, we expand NBITS early
		// at NBITS limit we leave zero room, so grow
		if( N < R ) {
			// right gap exists use gap
			V = N ++;
			return true;
		}
		int N_to_COPY = N - M; 
		int old_left_gap = M - L;
		if( old_left_gap > 0 ) {
			// left gap exists copy down by 3/4 and use gap
			// split the gap 1:3 to expand right 
		int	new_left_gap = old_left_gap  / 4; // poss 0
		int	new_right_gap = old_left_gap - new_left_gap;
			assert( 0 < new_right_gap );

			int N_bytes_to_COPY = N_to_COPY * bytes_per_item;

			char * SRC = EA_of_X( old_left_gap );
			char * DST = EA_of_X( new_left_gap );

			memmove( DST, SRC, N_bytes_to_COPY );

			L = M - new_left_gap;
			assert( L <= M );

			V = N ++;
			return true;
		}

		assert( L == M ); // no left gap
		assert( N == R ); // no right gap
		// grow array 

		if(!grow_array_right() } return FAIL_FAILED();

		int add_gap = N_to_copy / 3 + 20;


		if( L < M ) { 
			// left gap exists use some of that
		}
	}

	bool
	L_M_z_N_R_32::
	grow_array_is_right( bool is_right ){
	}

