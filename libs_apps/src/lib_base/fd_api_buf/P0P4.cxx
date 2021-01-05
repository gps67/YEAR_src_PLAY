
#include "P0P4.h"

namespace BASE1 {
namespace Z4_P5 {

	bool P0P4_basic::CHECK_offs( int offs ) {
		if(offs<0) return FAIL("Negative");
		// as you add each project use, test the soft borders
	//	if(offs>=1024*16) WARN("Exceeds 16 K");
	//	if(offs>=1024*128) WARN("Exceeds 128 K");
	//	if(offs>=1024*128) WARN("Exceeds 128 K");
		//
	//	if(offs>=1024*16) return FAIL("Excessive CODING ERROR");
		int max_offs = PTR[P4] - PTR[P0];
		if(offs>=1024*16) return FAIL("Excessive CODING ERROR");
		return true;
	}

	bool P0P4_basic::CHECK_ptr( u8 * P ) {
		if( P < PTR[0] ) {
			FAIL("set_PTR() beyond P4");
			return false;
		}
		if( P > PTR[4] ) {
			FAIL("set_PTR() before P0");
			return false;
		}
		return true;
		
	}

	bool P0P4_basic::set_PTR( EDGE E, u8 * P ) {
		if(!CHECK_ptr(P)) return false;
		PTR[E] = P;
		/*
			Now ripple right, pushing following pointers
		*/
		for(
			EDGE r = EDGE_next_raw(E);
			r<E5_N;
			EDGE_incr_var(r)
		) {
			if( PTR[r] < P ) PTR[r] = P; else break;
			// actually P[4] _should_ not be reached
		}
		/*
			Now ripple left, pushing preceding pointers
		*/
		for(
			EDGE l = EDGE_prev_raw(E);
			l>=0;
			EDGE_decr_var(l)
		) {
			if( PTR[l] > P ) PTR[l] = P; else break;
			// actually P[0] _should_ not be reached
			// because CHECK_ptr limits us to exact == match
			// so replace with same value, do not even replace
		}
		/*
			The caller is supposed to get it right with
			move_edge_by_nbytes, so no pushing happens,
			however, force P0<=P1<=P2<=P3<=P4,
			and preserve P0P4 (by code examination)
			and let strange behaviour replace crashes
		*/
	}

	bool P0P4_basic::move_edge_by( EDGE E, int nbytes ) {
		if(nbytes<0) {
			// stay to see what happens
			// WARN("move_edge_by( E, negative )");
			// or not
			return WARN("move_edge_by( E, negative )");
		}
		// if((0<=Z)&&(Z<Z4_N)) 
		if(EDGE_valid_quick(E)) {
			u8 * P = PTR[E] + nbytes;
			return set_PTR( E, P );
		} else {
			if( EDGE_valid(E) ) {
				FAIL("CODING ERROR in line above");
			}
			FAIL("move_edge_by( BAD_EDGE adj)");
			return false;
		}
	}


};};
