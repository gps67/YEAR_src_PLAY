#ifndef STO_int32_hilo_H
#define STO_int32_hilo_H

// FILE BYTE_ORDER HILO // always ? !!
// TODO CPU_HILO is currently unattached

// STO_ WAX_ were identical, rejoin under XYWH_32 ?
// look for i32 to float32 in MMX 

// wish there was a way to avoid every user needing this
#include <byteswap.h>

#if 0
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
 // Little Endian (albeit at start)
 // Intel LOHO
 #define CPU_LOHI 1
 #define CPU_HILO 0
 // #warning "currently be testing on AMD64 - soon delete this message"
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
 // big endian (albeit at the start end)
 // ARM Motorola SPARC NetworkByteOrder HILO
 #define CPU_HILO 1
 #define CPU_LOHI 0
 #warning "to be tested on ARM - then delete this message"
#else
 error "I dont support __ORDER_PDP_ENDIAN__ whatever that is"
#endif
#endif

#include "buffer2.h"

#include <math.h> // rounding

namespace STO {

 struct i32_hilo {

 	// SLOW used in storage, in files, word is private ?

  	i32 word; // on disk, in mem, backwards_in_cpu // or ARM
	
	i32 get_word() { return word; } // 
	i32 get_swap() { return bswap_32( word ); } // 

	void set_word( const i32 _word ) { word = _word; }
	void set_swap( const i32 _word ) { word = bswap_32( _word ); }

#if CPU_HILO
	i32 get_i32_cpu() { return get_word(); }
	void set_from_i32_cpu(int val) { set_word(val); }
	// define CPU_SWAPPED_false
# else // _LOHI
	i32 get_i32_cpu() { return get_swap(); }
	void set_from_i32_cpu(int val) { set_swap(val); }
	// define CPU_SWAPPED_true
#endif


 // CTOR // COPY_CTOR // CONV_CTOR // ASSIGN

	i32_hilo() {
		word = 0;
	}

	i32_hilo(  const i32_hilo & rhs ) {
		word = rhs.word;
	}

	i32_hilo(  const i32 val ) {
		set_from_i32_cpu( val );
	}

	i32_hilo &
	operator=( const i32_hilo & rhs ) {
		word = rhs.word;
		return *this;
	}

	i32_hilo &
	operator=( const i32 val ) { // val is i32_cpu;
		set_from_i32_cpu( val );
		return *this; // to be nice
	}

	i32_hilo &
	operator=( const u32 val ) { // val is i32_cpu;
		set_from_i32_cpu( (i32) val );
		return *this; // to be nice
	}


// EXTRA low byte and second_lo byte
// in case used into CPU
// direct WHEN AMD64 to beware of i32 extns
// offs = u32_sto >> 8;
// offs = i32_sto >> 8;
// WHEN SIGN >> 8
// 00 00 00 00    // BASE u32 loaded from u32_hilo // but as i32
// ++ 7F FF FF    // LAST +ve
// 00 80 00 00    // FIRST signed addr i24 // 
// 80 00 00 00    // FIRST signed addr
// FF 80 00 00    // FIRST signed addr i24
// ++ 7F FF FF    // SPACE u23 i24
// FF 80 00 00    // 
// FF FF FF FF    // FULL
// FF FF 80 00 -  // 

// NOT YET //	u8 get_u8() {
// NOT YET //	}


// EXTRA ODD S

	void set_from_i32_hilo_val( const i32 _word ) // ALREADY is hilo
	{
		// but how do you KNOW it is HILO,
		// where did it come from?
		// why not use i32_word in the first place ?
		INFO("free_radical claiming to be HILO 0x%4X", word);
		// should have used assign or something
		word = _word;
	}

// UTILITY 

	bool print_0xFFFF(buffer2 & out) {
		i32 val = get_i32_cpu(); // decided there is no ACTUAL type _cpu
		out.print("0x%4X", val ); // i32 to int
		return true;
	}

 // SHIPPING as word network byte order 
 // COMPUTING as i32 integer (u15_idx)
 // USING as PIXEL_POS_t
 // providing named functions incase you want them

	void get_hi_lo( u8 & hi, u8 & lo ) {
		i32 pair = get_i32(); // hope XCHG involved
		lo = pair & 0xFF; // hope GCC uses AL
		hi = pair >> 8;	 // home GCC uses AH
	}

	void set_from_hi_lo( u8 hi, u8 lo ) {
		i32 pair_val = (hi<<8) + lo;
		set_from_i32_cpu( pair_val );
	}

	#if 0
	void set_from_i32_lohi( const i32_lohi & rhs ) {
		set_swap( rhs.lohi );// OK we know
	}
	#endif

	void set_from_i32( i32 _val )
	{ 
		set_from_i32_cpu( _val );
	}

	i32 get_i32() { return get_i32_cpu(); }
	u32 get_u32() { return get_i32(); } // SIGN_extend 32 to 32

	float get_as_float() {
		return (float) get_i32(); // (BSWAP2) i32 to int to float
	}

	double get_as_double() {
		return (double) get_i32(); // (BSWAP2) i32 to int to double
	}

	operator u32 () { return get_u32(); };
	operator i32 () { return get_i32(); };
	operator double () { return get_as_double(); };
	operator float () { return get_as_float(); };

	float set_from_double( double dbl ) {
		// round() moves 0.5 to 1.0 -0.5 to -1 
		// round returnd double, sure there is an asm to i32
		i32 _val = (i32) round( dbl );
		set_from_i32( _val );
		return dbl;
	}

	float set_from_float( double dbl ) {
		return (float) get_i32(); // (BSWAP2) i32 to int to float
	}


 }; // i32_hilo

 struct u32_hilo : public i32_hilo
 {
	 u32_hilo( u32 val )
	 : i32_hilo( (i32) val )
	 {
	 }
	 
	 u32_hilo( i32 val )
	 : i32_hilo( (i32) val )
	 {
	 }
 };

}; // namespace
#endif
