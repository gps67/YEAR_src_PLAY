#ifndef P5_EDGES_H
#define P5_EDGES_H

enum ZONE {

	Z0 = 0,
	Z1 = 1,
	Z2 = 2,
	Z3 = 3,

	ZONE_A = Z0,
	ZONE_B = Z1,
	ZONE_C = Z2,
	ZONE_D = Z3,

	ZONE0 = Z0,
	ZONE1 = Z1,
	ZONE2 = Z2,
	ZONE3 = Z3,

	Z4 = 4,
	Z4_N = Z4,
	ZONE_N = Z4,

#if 0
	R2 = ZONE_A,
	W2 = ZONE_B,
	R1 = ZONE_C,
	W1 = ZONE_D,
#endif
};

enum EDGE {
	// there are 5 points that surround 4 zones
	// edges and points are the same thing

	// the first point of a zone is P0
	// the first point of the next zone is P2 over the edge 
	// one persons P2 is the next persons P0

	// [P0[P2 is empty when P0==P2 (==somehting sensible)

	// When used in a P0P4, be careful of using P2 (use P4 instead)
	// but ...

	// ALSO moving from absolute P1 to offs_P1_wrt_P0 _wrt_PTR0

	E0 = 0,
	 E1 = 1,
	 E2 = 2,
	 E3 = 3,
	E4 = 4,

	P0 = E0,
	 P1 = E1,
	 P2 = E2,
	 P3 = E3,
	P4 = E4,

	E5_N = 5,
	P5_N = 5,

	E0_EDGE__A = E0,
	E1_EDGE_AB = E1,
	E2_EDGE_BC = E2,
	E3_EDGE_CD = E3,
	E4_EDGE_D_ = E4,

	E0_ZONE0_p0 = E0, E1_ZONE0_p2 = E1,
	E1_ZONE1_p0 = E1, E2_ZONE1_p2 = E2,
	E2_ZONE2_p0 = E2, E3_ZONE2_p2 = E3,
	E3_ZONE3_p0 = E3, E4_ZONE3_p2 = E4,

};

enum {
	idx_R2_p0 = E0_ZONE0_p0,
	idx_R2    =    ZONE0,
	idx_R2_p2 = E1_ZONE0_p2,
	idx_W2_p0 = E1_ZONE1_p0,
	idx_W2    =    ZONE1,
	idx_W2_p2 = E2_ZONE1_p2,
	idx_R1_p0 = E2_ZONE2_p0,
	idx_R1    =    ZONE2,
	idx_R1_p2 = E3_ZONE2_p2,
	idx_W1_p0 = E3_ZONE3_p0,
	idx_W1    =    ZONE3,
	idx_W1_p2 = E4_ZONE3_p2,
};

/*
	an EDGE is one of the 5 points
*/
inline bool EDGE_valid_quick( EDGE E )
{
	return (0<=E) && (E<E5_N);
}
/*
	a ZONE is one of the 4 regions
	(between points)
*/
inline bool ZONE_valid_quick( ZONE Z )
{
	return (0<=Z) && (Z<Z4_N);
}

inline EDGE EDGE_next_raw( EDGE E ) { return (EDGE) (E+1); }
inline EDGE EDGE_prev_raw( EDGE E ) { return (EDGE) (E-1); }
#define EDGE_incr_var( E ) ((E) = EDGE_next_raw( E ))
#define EDGE_decr_var( E ) ((E) = EDGE_prev_raw( E ))
#define EDGE_left_of_zone( Z ) ( (EDGE) Z )
#define EDGE_right_of_zone( Z ) ( EDGE_next_raw( (EDGE) Z ))

extern bool EDGE_valid( EDGE E );
extern bool ZONE_valid( ZONE Z );

#endif


