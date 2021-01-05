#ifndef P0P4_H
#define P0P4_H

#include "ints.h" // does the GRP_lib_base
#include "misc.h"
#include "dgb.h"
#include "P5.h"
#include "p0p2.h"

namespace BASE1 {
namespace Z4_P5 {

 /*!
	This is a highly simplified version

	It uses:  u8 * PTR[5]; // 5 points [01234][5

	It does not use _offs

	It does not store nbytes

	P0P2 are from P0P4

	It does use array of pointers.

	It could build p0p2 from WITHIN its vector
	and then use pointers to changable values directly

	There is still the same broad locking issues,
	but with a single threaded 2-sided queue ...
 */
 class P0P4_basic : public GRP_lib_base // as pointers
 {
 protected:
	u8 * PTR[P5_N]; // 5 points [01234[5
 public:

	P0P4_basic()
	{
		// actually init 0 helps
		// set_P0P4_empty_at( 0 )
		// set_P0P4( 0,0 );
	}
	P0P4_basic( u8 * _P0 )
	{
		set_P0P4_empty_at( _P0 );
	}

	P0P4_basic( const p0p2 & _FULL )
	{
		set_P0P4( _FULL );
	}

	bool set_P0P4_empty_at( u8 * _P0 )
	{
		set_P0P4( p0p2( _P0, _P0 ));
	}

	bool set_P0P4( const p0p2 & _FULL )
	{
		PTR[0] = _FULL.p0;
		PTR[1] = _FULL.p0;
		PTR[2] = _FULL.p0;
		PTR[3] = _FULL.p0;
	// its all here in W1
		PTR[4] = _FULL.p2;
		return true;

	// # validate code goes somewhere
	// # check PTR[0] <= PTR[1]
	// # check PTR[1] <= PTR[2]
	// # check PTR[2] <= PTR[3]
	// # check PTR[3] <= PTR[4]

	// # check PTR[0] <= PTR[4]

	// # if EXTRA_TESTS ... # endif
		if( PTR[4] < PTR[0] ) {
			PTR[4] = PTR[0];
			FAIL( "set_P0P4: p2 < p0: CODING ERROR: Empty area is ==");
			return false;
		}
		return true;
	}

	int nbytes_P0P4() {
		return PTR[P4] - PTR[P0];
	}

	bool CHECK_offs( int offs );

	u8 * get_PTR( EDGE E ) {
		return PTR[E];
	}

	bool CHECK_ptr( u8 * P );
	bool CHECK_ptr_quick( u8 * P ) { return CHECK_ptr(P); }

	bool set_PTR( EDGE E, u8 * P );
	bool move_edge_by( EDGE E, int nbytes );

	int nbytes_in( ZONE Z ) {
		if(ZONE_valid_quick(Z))
			return PTR[Z+1] - PTR[Z];
		// we know ZONE is valid, really we do, dont we
		if(!ZONE_valid(Z)) return 0;
		EDGE edge_left = (EDGE) (int) Z;
		EDGE edge_right = (EDGE) (1+edge_left);
		return PTR[edge_right] - PTR[edge_left];
	}

	bool move_P1_forward( int adj ) { return move_edge_by( P1, adj ); }
	bool move_P2_forward( int adj ) { return move_edge_by( P2, adj ); }
	bool move_P3_forward( int adj ) { return move_edge_by( P3, adj ); }
	// this only moves the left edge inwards, and needs a new name
	bool move_ZONE_forward( ZONE Z, int adj ) { return move_edge_by( EDGE_left_of_zone(Z), adj ); }

	int nbytes_Z0() { return nbytes_in( Z0 ); }
	int nbytes_Z1() { return nbytes_in( Z1 ); }
	int nbytes_Z2() { return nbytes_in( Z2 ); }
	int nbytes_Z3() { return nbytes_in( Z3 ); }

	void set_0_0_0_rest() { PTR[3] = PTR[2] = PTR[1] = PTR[0]; }

 };

 class P0P4 : public P0P4_basic 
 {
	P0P4()
	: P0P4_basic()
	{
	}
	P0P4( u8 * _P0 )
	: P0P4_basic( _P0 )
	{
	}
	P0P4( const p0p2 & _FULL )
	: P0P4_basic( _FULL )
	{
	}

 };



/*
	The code aliases for names for zones.
	The idea is to make it as error free as possible.
	Unfortunatle this syntax would create a new class
	(which is good because it excludes other names)
	and that then needs bouncing through ZONE_FROM_CIRC_ZONE()
	and move_XXX_forwards( XXX_from_YYY )

	If the compiler does full inlining, it all disappears,
	but if the CPP does it, it also disappears,
	but still allows the original to shine through

	enum CIRC_ZONE {
		R2 = Z0,
		W2 = Z1,
		R1 = Z2,
		W2 = Z3,
	};
*/
// lots not exclude ALIAS just because it uses CPP not C++

#define	 R2  Z0
#define	 W2  Z1
#define	 R1  Z2
#define	 W1  Z3

 class P0P4_R2_W2_R1_W1 : public P0P4
 {
  public:

#if 0
	bool move_ZONE_forward( CIRC_ZONE Z, int adj ) { return move_edge_by( EDGE_left_of_zone(Z), adj ); }
#endif

	bool move_R1_R2( int _n1, int _n2 ) {
		if(!move_R1( _n1 )) { return false; }
		if(!move_R2( _n2 )) { return false; }
	}
	bool move_W1_W2( int _n1, int _n2 ) {
		if(!move_W1( _n1 )) { return false; }
		if(!move_W2( _n2 )) { return false; }
	}
	bool move_z1_z2( ZONE z1, ZONE z2, int _n1, int _n2 ) {
		if(!move_ZONE_forward( z1, _n1 )) { return false; }
		if(!_n2) return true;
		if(!move_ZONE_forward( z2, _n2 )) { return false; }
		// maybe reset if that failed
	}
	bool done_z1_z2( ZONE z1, ZONE z2, int _n1, int _n2 ) {
		return move_z1_z2( z1, z2, _n1, _n2 );
	}

	bool move_R1( int _n ) { return move_ZONE_forward( R1, _n ); }
	bool move_R2( int _n ) { return move_ZONE_forward( R2, _n ); }
	bool move_W1( int _n ) { return move_ZONE_forward( W1, _n ); }
	bool move_W2( int _n ) { return move_ZONE_forward( W2, _n ); }

	int nbytes_R1() { return nbytes_in( R1 ); }
	int nbytes_W1() { return nbytes_in( W1 ); }
	int nbytes_R2() { return nbytes_in( R2 ); }
	int nbytes_W2() { return nbytes_in( W2 ); }

	int nbytes_R1_R2() { return nbytes_R1() + nbytes_R2(); }
	int nbytes_W1_W2() { return nbytes_W1() + nbytes_W2(); }

	bool plan_to_use( ZONE z1, int n ) {
	// so simple it doesnt need writing down
		int n1 = nbytes_in( z1 );
		return n1 <= n;
	}

	bool plan_to_use( ZONE z1, ZONE z2, int n, int & _n1, int & _n2 ) {
		int n1 = nbytes_in( z1 );
		if( n1 >= n ) {
			_n1 = n;
			_n2 = 0;
			return true; // single write
		}
		int n2 = nbytes_in( z2 );
		int n1_n2 = n1 + n2;
		if( (n1_n2) >= n ) {
			_n1 = n1;
			n1_n2 -= n1;
			_n2 = n1_n2;
			return true;
		}
		return false;
		// NB this includes a common case where _n1==0 _n2!=0
		// so dont rely on _n1 to tell you much
		// this function api encourages safer useage
	}
	bool plan_read( int n, int & _n1, int & _n2 ) {
		return plan_to_use( R1, R2, n, _n1, _n2 );
	}
	bool plan_write( int n, int & _n1, int & _n2 ) {
		return plan_to_use( W1, W2, n, _n1, _n2 );
	}
	bool done_read( int _n1, int _n2 ) { move_z1_z2( R1, R2, _n1, _n2 ); }
	bool done_write( int _n1, int _n2 ) { move_z1_z2( W1, W2, _n1, _n2 ); }
 };

/*
	now need a strategy

		LOCK
		PLAN
		COPY
		DONE

	and additional vectors, for COPY_WITH_FRAG_OK, XOFF
	It will need things like hi_tide markers
*/

};
};

#endif
