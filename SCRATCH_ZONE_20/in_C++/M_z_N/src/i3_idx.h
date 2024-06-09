#ifndef i3_idx_H
#define i3_idx_H

#include "dgb.h" // basic use

struct i3_idx {
	// actually the 3 is a placeholder

	// SIGNED
	// SIGNED means use only CPU interpretation of nbits
	// LIMITED when i32 needs u32_N_with_added_CARRY
	// LIMITED when i32 needs u32_M_with_added_BORROW

	// M_z_N // z is 0 // ZPOS = EA_BASE() + (0 - M )
	// M_x_N // x is x // xPOS = EA_BASE() + (x - M )
	// CHECK L <= M //
	// CHECK M <= x //
	// CHECK x <  N // 
	// CHECK N <= R //

	int in_nbits;	//  3 for i3 //
	int in_MIN;	// -8 for i3 // but beware that -(-8) is still -8
	int in_MAX;	//  7 for i3

	bool avoid_MIN_VAL;

	int MAX() { return in_MAX; } ; // 7 for i3
	int MIN() { return in_MIN; } // 7 for i3
	int MIN_plus_1() { return  in_MIN + 1; }
	int MAX_plus_1() { return  in_MAX + 1; }

	/* CTOR */ 
	i3_idx( int nbits );
};

#endif

