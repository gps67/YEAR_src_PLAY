#ifndef M_z_N_H
#define M_z_N_H

// need NULL defined in
#include "stdlib.h"


// for simplicity we limit ourselves to i32_idx inside u32_array
// we still hope for u8_idx

typedef int IDX_int; // POS

struct L_M_z_N_R_32 {

	char * PTR_to_ARRAY;

	IDX_int L;
	IDX_int M;
	IDX_int N;
	IDX_int R;

	/*!
		 NBIT increases by 1 
	*/
	int NBIT; // 2 3 4 5 6 7 8 9 10 ..
	int N_MAX;
	int M_MIN;
	int NBIT_WORD; // 2 6 8 12 16 24 32 48 64
	int bytes_per_item; // 1 2 4 8 

static const	IDX_int Z = 0;

	L_M_z_N_R_32()
	: PTR_to_ARRAY(NULL)
	, L(0), M(0), N(0), R(0)
	, NBIT(2)
	, NBIT_WORD(2)
	, bytes_per_item(4)
	{
	}

	IDX_int X_of_V( IDX_int V ) { return V - L; }
	IDX_int V_of_X( IDX_int X ) { return X + L; }

	char * EA_of_X( IDX_int X ) {
		return PTR_to_ARRAY + X * bytes_per_item;
	}

	char * EA_of_V( IDX_int V ) {
		return EA_of_X( X_of_V( V ) );
	}

	IDX_int MAX_of_NBIT( int NBIT ) {
		return 2 ^ NBIT - 1;
	}

	bool ALLOC_PLUS( IDX_int & V );
	bool ALLOC_MINUS( IDX_int & V );

	bool grow_array_left() { return grow_array_is_right( false ); }
	bool grow_array_right() { return grow_array_is_right( true ); }
	bool grow_array_is_right( bool is_right );


};

#endif
