#ifndef P5_H
#define P5_H

#include "ints.h"
#include "dgb.h"

namespace BASE1 {
namespace Z4_P5 {

/*!
	The idea is to tokenise zone4_ids and point5_idx
	so that (variety of reasons, including remote control)
	we can select zones as u8 few bits, which then result in u8*

	This will help type computations, as the (debugged) library
	will process ZONE_C_p0 as EDGE_AT( edge_to_left_of_zone( ZONE_C )
	where int edge_to_left_of_zone( ZONE_C ) { return ZONE_C; }
	where int edge_to_right_of_zone( ZONE_C ) { return 1 + ZONE_C; }
	then get_ZONE_C_P2 = return P[ ZONE_C + 1 ];

typedef int xu2_idx_Z4;
typedef int xu14_or_so;
typedef int xu14_offs;
*/

/*!
	segment size = 64K in CPU	// code and 4x 16K
	segment size = 16K in CPU	// u14_or_so

	typedef u16 u14_or_so;		// OFFS or ITEM of 3000
	SSL uses u14_16K 		// PKT frame within STREAM
	CIRC uses u17_128K (8x16)	// 2 seconds transmit burst

	The JOY of u8* is that it is umambiguously unique
	(albeit any number of routes there)

	The problem with OFFS is that we need to have reference point
	and we need to specify WHICH referece point to use
	eg move_pointer_by( adj )
	eg move_pointer_to( vals )
*/
class u14_or_so
{
	int offs; // u14 in i32
 public:
	operator int() { return offs ; }
	u14_or_so() { offs=0; }
	void operator = (int rhs) { offs = rhs; }
	void operator += (int rhs) { offs += rhs; }
	void operator -= (int rhs) { offs -= rhs; }
	int operator - (const u14_or_so rhs) { return offs - rhs.offs; }
	bool fits_in_16K() { return offs <= 16*1024; }
};

typedef u14_or_so u14_offs;


///////////// good so far //// gets icky / lost from here ///////////////

// idx_ZONE -is- ZONE
// idx_EDGE -is- EDGE

// The only ways to produce an enumerated EDGE are controled
// then the EDGE is _KNOWN_ to be valid in range

// the only computation are E+1 with upper limit of E=E4
// any occurance of E == E5_N causes a reset of values (maybe?)
// E5_N is common exit value from loops, but exactly out of range

// STILL WANT u14_at u14_adj types
// _at is _adj but different set of rules for ADJ12 = P2-P1
// also want offs +ve for polarised steps without math hell
// ie add direction into fn_call_name or object name (width/step)

// template<const N_XS>

struct idx_BASE
{
 private:
 protected:
 public:
	int idx;
	operator int () { return idx; }

	// actually this destroys the type safety
	// CTOR for stack use of copies ... leave AUTO ?
	idx_BASE( const idx_BASE & rhs ) { idx = rhs.idx; }

	bool set_forced_value( int X ) {
		idx = X;
	}

/*
	bool set( int x, EDGE _max ) {
		return set( x, int(_max) );
	}
	bool set( int x, ZONE _max ) {
		return set( x, int(_max) );
	}
*/
	bool set( idx_BASE _idx ) {
		idx = _idx;
		return true;
	}
	bool set2( int x, int HI ) {
		if( x < 0 ) { return FAIL("Negative"); }
		if( x >= HI ) { return FAIL("Excessive"); }
		idx = x;
		return true;
	}
};

struct idx_Z4 : public idx_BASE // u2
{
	int NXS() { return Z4; }
	bool set( int x ) { return set2(x,(int)Z4); }
	bool set( ZONE z ) { return set((int)z); }

};


typedef EDGE idx_P5_EDGE;
struct idx_P5 : public idx_BASE // u3 [0[5
{
	int NXS() { return P5_N; }
	bool set( int x ) {return set2(x,(int)P5_N);}
	bool set( EDGE e ) { return set((int)e); }
};


struct idx_iterator
{
	idx_BASE idx;
	idx_BASE nxs;

	void init( int _nxs ) {
		idx.idx = -1;
		nxs.idx = _nxs;
	};

	void start_pre_step_stay( idx_BASE first)
	{
		idx.idx = first-1;
	}
	void start_pre_step_stay()
	{
		idx.idx = -1;
	}
	bool step_stay()
	{
		if( idx.idx >= nxs.idx ) return false;
		idx.idx ++;
		return true;

	}
	void start( idx_BASE pos )
	{
		idx.idx = pos;
	}
	void start()
	{
		idx.idx = 0;
	}
	bool stay()
	{
		return idx.idx < nxs.idx;
	}
	void step()
	{
		idx.idx++;
	}

	/*
		idx_iterator Z;
		for( Z.start(); Z.stay(); Z.step() ) {
			idx_BASE i = Z.idx;
		}
	*/
	
};

////////////////////////////////// arrays were always intended for own .h

/*!
*/
class array_Z4_widths
{
 public:
	u14_offs nbytes[Z4_N];
	array_Z4_widths( int total ) {
		// put all in last
		for(int i=0; i<Z4_N; i++ )
			nbytes[i] = 0;
		nbytes[ Z4_N - 1 ] = total;
	}

	void reset_0_0_0_0() {
		nbytes[0] = 0;
		nbytes[1] = 0;
		nbytes[2] = 0;
		nbytes[3] = 0;
	}

	void reset_0_0_0_all( int total ) {
		reset_0_0_0_0();
		nbytes[3] = total;
	}

	int validate_idx( int i ) {
	}

	void set_VAL( idx_Z4 p, u14_offs P ) {
		nbytes[p] = P;
	}
	void get_VAL( idx_Z4 p, u14_offs & P ) {
		P = nbytes[p];
	}

	bool move_pointer_by( idx_Z4 p, int adj )
	{
	}

	bool set_pointer( idx_Z4 p, u14_offs adj )
	{
	}

};

/*!
*/
class array_P5_offs
{
	u14_offs OFF[P5_N];

	int validate_idx( int i ) {
	}

	void set_VAL( idx_P5 p, u14_offs P ) {
		OFF[p] = P;
	}
	void get_VAL( idx_P5 p, u14_offs & P ) {
		P = OFF[p];
	}

	bool move_pointer_by( idx_P5 p, int adj )
	{
	}

	bool set_pointer( idx_P5 p, u14_offs adj )
	{
	}

};


/*!
*/
class array_P5_ptr
{
 public:
	array_P5_ptr()
	{
		setall( 0 );
	}
	array_P5_ptr( u8 * P0 )
	{
		setall( P0 );
	}

	void setall( u8 * P0 )
	{
		for(int i=0; i<P5_N; i++ ) PTR[i] = P0;
	}

	u8 * PTR[P5_N];

	void set_PTR( idx_P5_EDGE p, u8 * P ) {
		PTR[p] = P;
	}
	void get_PTR( idx_P5_EDGE p, u8 * & P ) {
		P = PTR[p];
	}
};

}; // namespace
};
#endif
