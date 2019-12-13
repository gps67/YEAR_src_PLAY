#ifndef STO_int16_hilo_H
#define STO_int16_hilo_H

// FILE BYTE_ORDER HILO // always ? !!
// TODO CPU_HILO is currently unattached

// STO_ WAX_ were identical, rejoin under XYWH_16 ?
// look for i16 to float32 in MMX 

// wish there was a way to avoid every user needing this
#include <byteswap.h>

typedef short i16;
#include "buffer2.h"

#include <math.h> // rounding

namespace STO {

 struct i16_hilo {

 	// SLOW used in storage, in files, hilo is private ?

  	short hilo; // on disk, in mem, backwards_in_cpu // or ARM
	
	short get_hilo() { return hilo; } // 
	short get_lohi() { return bswap_16( hilo ); } // 

#if CPU_HILO
	// define CPU_SWAPPED_false
	short get_i16_cpu() { return get_hilo(); }
	void set_from_i16_cpu(int val) { set_from_i16_hilo(val); }
# else // _LOHI
	// define CPU_SWAPPED_true
	short get_i16_cpu() { return get_lohi(); }
	void set_from_i16_cpu(int val) { set_from_i16_lohi_val(val); }
#endif

 // CTOR // COPY_CTOR // CONV_CTOR // ASSIGN

	i16_hilo() {
		hilo = 0;
	}

	i16_hilo(  const i16_hilo & rhs ) {
		hilo = rhs.hilo;
	}
	i16_hilo(  const i16 val ) {
		set_from_i16_cpu( val );
	}

	i16_hilo &
	operator=( const i16_hilo & rhs ) {
		hilo = rhs.hilo;
		return *this;
	}
	i16_hilo &
	operator=( const i16 val ) { // val is i16_cpu;
		set_from_i16_cpu( val );
		return *this; // to be nice
	}

// UTILITY 

	bool print_0xFFFF(buffer2 & out) {
		i16 val = get_i16_cpu(); // decided there is no ACTUAL type _cpu
		out.print("0x%4X", val ); // short to int
	}

 // SHIPPING as hilo network byte order 
 // COMPUTING as i16 integer (u15_idx)
 // USING as PIXEL_POS_t
 // providing named functions incase you want them

	void get_hi_lo( u8 & hi, u8 & lo ) {
		i16 pair = get_i16(); // hope XCHG involved
		lo = pair & 0xFF; // hope GCC uses AL
		hi = pair >> 8;	 // home GCC uses AH
	}

	void set_from_hi_lo( u8 hi, u8 lo ) {
		i16 pair_val = (hi<<8) + lo;
		set_from_i16_cpu( pair_val );
	}

	void set_from_i16_hilo( const i16_hilo & rhs )
	{
		// *this = rhs;
		hilo = rhs.hilo;
		*this = rhs; // should be same short circuit
	}
	void set_from_i16_hilo_val( const i16 _hilo )
	{
		// but how do you KNOW it is hilo,
		// where did it come from?
		// why not use i16_hilo in the first place ?
		INFO("free_radical claiming to be HILO 0x%4X", hilo);
		// should have used assign or something
		hilo = _hilo;
	}
	void set_from_i16_lohi_val( const i16 lohi_bw )
	{
		hilo = bswap_16( lohi_bw );
		// INFO("free_radical claiming to be LOHI 0x%4X now 0x%4X", lohi_bw, hilo);
	}
	#if 0
	void set_from_i16_lohi( const i16_lohi & rhs ) {
		set_from_i16_lohi_val( rhs.lohi );// OK we know
	}
	#endif

	void set_from_i16( i16 _val )
	{ 
		set_from_i16_cpu( _val );
	}

	i16 get_i16() { return get_i16_cpu(); }
	i32 get_i32() { return get_i16(); } // SIGN_extend 16 to 32

	float get_as_float() {
		return (float) get_i16(); // (BSWAP2) short to int to float
	}

	double get_as_double() {
		return (double) get_i16(); // (BSWAP2) short to int to double
	}

	operator double () { return get_as_double(); };
	operator float () { return get_as_float(); };

	float set_from_double( double dbl ) {
		// round() moves 0.5 to 1.0 -0.5 to -1 
		// round returnd double, sure there is an asm to i16
		i16 _val = (short) round( dbl );
		set_from_i16( _val );
	}

	float set_from_float( double dbl ) {
		return (float) get_i16(); // (BSWAP2) short to int to float
	}



	
 }; // i16_hilo

}; // namespace
#endif
