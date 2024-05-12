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
	increase_NBIT_WORD_to_hold( int NBITS ) {
		if( NBITS <= NBIT_WORD ) {
			WARN("this never happens");
			return true; 
		}
		if( NBITS <= 8 ) {
			NBIT_WORD = 8;
			NBIT_idx_MAX = 0xFF;
		} else
		if( NBITS <= 24 ) {
			NBIT_WORD = 24;
			NBIT_idx_MAX = 0xFFFFFF;
		} else
		if( NBITS <= 32 ) {
			NBIT_WORD = 32;
			NBIT_idx_MAX = 0xFFFFFFFF;
	#if 1
		} else {
			NBIT_WORD = 32;
			NBIT_idx_MAX = 0xFFFFFFFF;
			return FAIL("overflow u32");
		}
	#else
		if( NBITS <= 48 ) {
			NBIT_WORD = 48;
			NBIT_idx_MAX = 0xFFFFFFFFFFFF;
		} else
		if( NBITS <= 64 ) {
			NBIT_WORD = 64;
			NBIT_idx_MAX = 0xFFFFFFFFFFFFFFFF;
		} else {
			NBIT_WORD = 64;
			NBIT_idx_MAX = 0xFFFFFFFFFFFFFFFF;
			return FAIL("overflow u64");
		}
	#endif
		PASS("NBITS %d got %d MASK 0x%016X", NBITS, NBIT_WORD, NBIT_idx_MAX );

		return true;
	}

	bool
	L_M_z_N_R_32::
	increase_NBIT_to_cover( IDX_int N2 ) {

		if( N2 <= NBIT_idx_MAX )
			return true;

		// SOFTWARE LIMIT ARM32 u32_u32 HERE
		// PAUSE at 32 // fail ?
		while( ( NBIT <= 32 ) &&( N2 > NBIT_idx_MAX )) {
		INFO("NBITS %d got %d MASK 0x%016X", NBIT, NBIT_WORD, NBIT_idx_MAX );
			if(!increase_NBIT_WORD_to_hold( NBIT+1 ))
				return FAIL_FAILED();
		}

		if( N2 <= NBIT_idx_MAX )
			return true;
		INFO("NBITS %d got %d MASK  x%016X", NBIT, NBIT_WORD, NBIT_idx_MAX );
		return FAIL("CODE_ERROR");
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
			} // GAP ASIS ZERO RATIO

			// we have decided GAP, make it new_gap_left
			if(!slide_left_to( new_gap_left )) 
				return FAIL_FAILED();

		} else { // not enough GAP // realloc required
			// how much == N2 // want to pause at xFF FULL PAGE
			// 
			// gap_after from calc is MINUS // start new DIAG
			// KNOW PREV DIAG //
			//
			// PAUSE_at_CEILING_PAGE_EDGE // xFF then xFFF then xFF_u32
			// so when u5_PICK is used expect to create TRAY[32]
			// array[ u5_idx ] of WORD_PAIR_ITEM_EXPR u32_u32
			// PLUS += KNOW compiler chose u5 because u4 was FULL
			// so 16 of 32 // average +8 // N_USED = 24 //
			// NBIT required 5 // NBIT_idx_MAX = 31 //

			// we might realloc within same NBITS
			// we might realloc with extra NBITS
			// typical usage get_space_for(10) 
			// adds 20% + 20 // ROM can SHRINK shrink_wrap gap_zero

			// if we cant grow space within NBITS // +1 +8 //
			// pick a new idx_VAL_HIGH // BIND == i32_VAL // i12_VAR
			if( N2 <= NBIT_idx_MAX ) {
				// realloc but idx wont break NBIT
			} else {
				// expand NBIT computation VARS
				if(! increase_NBIT_to_cover(N2) ) 
					return FAIL_FAILED();;
				// realloc with new NBIT
				// realloc but idx wont break NBIT // after FIX
			}
			int idx_VAL_HIGH = N2;


			// N2 is exact required to have space
			int new_alloc = N2;

			if( NBIT_idx_MAX < N2 ) {
				// can realloc keeping same NBITS
			} else {
				// must expand NBITS // jump to it's 1.2
			}
			INFO( "NBIT_idx_MAX %ld", (long) NBIT_idx_MAX );

			// int dst = new_gap_left;
			// int src = M;
			// int cnt = N - M;
		}


		// Maybe space is available on the left


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

		// get_space then use space
		//	V = N ++;
	//		return true;
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
	slide_left_to(IDX_int LM_gap) { // gap left
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
		if( NBIT_idx_MAX < new_N ) { // request grow beyond xFFFF

			// maybe request can be happy WITHIN NBIT_idx_MAX

			if( old_N + 1 <= NBIT_idx_MAX ) {
				new_N = NBIT_idx_MAX +1;
				// idx_limit not N_limit
				// ie GROW UPTO NBIT CEILING // +1 // TODO //
			} else {
			}
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

