#include "M_z_N.h"
#include <assert.h>
#include <string.h> // memmove
#include "dgb.h"
	
//	using namespace XYZ;

	bool
	L_M_z_N_R_32::
	ALLOC_PLUS( IDX_int & V ) { 

		if(!get_plus_space_for( 1 )) return FAIL_FAILED();
		V = N ++;
		INFO("ALLOC idx = N ++ // idx(%d)", V ); // VALUE V == idx
		return true;
	}

	bool
	L_M_z_N_R_32::
	get_plus_space_for( int add ) {  

		// usually called with 1
		// load_array might already know N // HINT when ROM == GEN
		// some extra utilities for grow sequence presets or calc_now
		// currently grow by NBITS_LIMIT .51 .6 .7 .8 .99 1.00 1.2

		// new_N is N2
		int N2 = N + add; // new_N
		if( N2 < R ) {
			// space is already there just take it
			return true; // job done // space // gap_right //
		}

		// we are definitely going to do a memmove // maybe realloc
		// if there is enough gap_left + gap_right //

		int gap_left = M - L;
		int gap_right = R - N;
		int gap_after = gap_left + gap_right - add;

		if( gap_after >= 0 ) { // no realloc required
			int new_gap_left = gap_after / 4; // possible 0 // 
			if( gap_after == 0 ) {
				// no gap to split // spare after add
				// keep new_gap_left == 0
				assert( new_gap_left == 0 );
			} else 
			if( M == 0 ) { // ALLOC_MINUS not yet called
				INFO("gap_after %d # 0 from %d", gap_after, new_gap_left );
				new_gap_left = 0;
			} else
			{
			/*
				ratio split gap
				according to M N2 use for far
				Remember N is old_N N2 is new_N but L_M reduced

				MZ : ZN		data used
				LM : NR		gaps left

				(Z - M) : (N - Z)	data used
				(M - L) : (R - N )	gaps left
			*/
			// we are alloc_PLUS so we know ZN > 0
			int Z = 0;
			float MZ = -M; // Z - M // nolonger poss 0 
			float ZN = N2; // N2 - Z;
			assert( ZN > 0.0 );
			double LM = gap_after * MZ / ZN;
			new_gap_left = (int) LM;
			INFO("gap_after %d # 0 from %d - ratio split ", gap_after, new_gap_left );

			if(!slide_left_to( new_gap_left )) 
				return FAIL_FAILED();

		} else { // not enough GAP // realloc required

			// we might realloc within same NBITS
			// we might realloc with extra NBITS
			// typical usage get_space_for(10) 
			// adds 20% + 20 // ROM can SHRINK shrink_wrap gap_zero

			// N2 is exact required to have space
			int new_alloc = N2;

			if( N2 < limit

			// int dst = new_gap_left;
			// int src = M;
			// int cnt = N - M;
			}

		}

		// Maybe space is available on the left


		else if( L < M ) {
			// gap on left can be shifted 3/4 to the right
			if(!slide_MN_left_some())
				return FAIL_FAILED();
			assert( N2 < R );
			V = N ++;
			return true;
		} else {
			// that is an exact fit
			// this is particularly fun when moving u8 to u16
			// 
			// grow_array
			//
			// option of at_least_ten // ie copy cost too much
			// but slide_MN_left_some() could do a late grow_array
			// so opt to call GROW_rather_than_COPY
			if(!grow_array()) return FAIL_FAILED();
		}

		return FAIL("CONTINUE CODING HERE");

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

			// now zero fill the adj gap
			// TODO //

			V = N ++;
			return true;
		}

		assert( L == M ); // no left gap
		assert( N == R ); // no right gap
		// grow array 

		if(!grow_array_right() ) return FAIL_FAILED();

		int add_gap = N_to_COPY / 3 + 20;


		if( L < M ) { 
			// left gap exists use some of that
		}

		return FAIL("TODO");
	}

	bool
	L_M_z_N_R_32::
	grow_array() {

		// other funcs might have opther args // eg get_space_for( nn )

		int old_N = N - M; 
		int new_N =( old_N * 3 / 2 ) + 32; // grow step 
		
		// old_CURR_NBITS_limit was OK for old_N
		// new_N might be above it
		if( NBITS_idx_MAX < new_N ) { // request grow beyond xFFFF

			// maybe request can be happy WITHIN NBIT_idx_MAX

			if( old_N + 1 <= NBITS_idx_MAX ) {
				new_N = NBITS_idx_MAX +1;
				// idx_limit not N_limit
				// ie GROW UPTO NBIT CEILING // +1 // TODO //
			} else
		}
		return FAIL("TODO");
		return FAIL_FAILED();
	}

	bool
	L_M_z_N_R_32::
	slide_MN_left_some() {
		return FAIL("TODO");
		return FAIL_FAILED();
	}

	bool
	L_M_z_N_R_32::
	grow_array_on_right( bool is_right ){
		return FAIL("TODO");
	}

