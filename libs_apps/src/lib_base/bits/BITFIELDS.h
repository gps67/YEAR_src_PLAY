#ifndef BITFIELDS_H
#define BITFIELDS_H

	// currently unused
	// want nasty template mixin type feature word_base
	// NB  this is the DECL_SPEC not the DATA

	struct CPU_WORD_i64_BITFIELDS_SPEC
	{
		STR_P0P2_EXPR STR; // any DECODE_64 that fits
		// eg // P0 = X0 // P2 = X5 // GEN_EA_of_AVAL

		i8_bits_lhs;
		i8_bits_mid; // in the BITFIELD
		i8_bits_rhs;
		i8_bits_tot; // 64 128 256

		i64 CPU_WORD;

		bool is_SIGNED;

		i64 GET_BITFIELD() {

		 if( bits_lhs == 0 ) { // and is that KNOW able
		 }

		 if( bits_mid == 0 ) { // checked somewhere before
		 }

		 if( bits_rhs == 0 ) { // and is that KNOW able
		 }

		 const u8 shift_left = bits_lhs;
		 const u8_shift_right = bits_lhs + bits_rhs;
		 i64_WORD2 = ( CPU_WORD << shift_left ) ;
		 if(is_SIGNED) {
		 	i64_WORD2 = ( i64_WORD2 >> shift_right );
		 } else {
		 	// ASM_INLINE a SIGNED shift right // no BORROW before
		 	u64_WORD2 = (u64) i64_WORD2;
		 	u64_WORD2 = ( u64_WORD2 >> shift_right );
		 	i64_WORD2 = (i64) u64_WORD2;
		 }
		}

		i64 SET_BITFIELD(i64 PAYLOAD) {
		 const u8 shift_left = bits_lhs;
		 const u8_shift_right = bits_lhs + bits_rhs;
		 u64 WORD_LHS = CPU_WORD >> (bits_rhs + bits_mid);
		 u64 WORD_RHS = CPU_WORD >> (bits_rhs + bits_mid);
		 i64_WORD2 = ( CPU_WORD << shift_left ) ;
		 if(is_SIGNED) {
		 	i64_WORD2 = ( i64_WORD2 >> shift_right );
		 } else {
		 	// ASM_INLINE a SIGNED shift right // no BORROW before
		 	u64_WORD2 = (u64) i64_WORD2;
		 	u64_WORD2 = ( u64_WORD2 >> shift_right );
		 	i64_WORD2 = (i64) u64_WORD2;
		 }
		}


		CPU_WORD_i64() { CPU_WORD = 0; }
		void clear_all() { CPU_WORD = 0; }
	
// i64_BITMASK

	// single bit b
	// b and any other BITS in MASK

		int bits_test(i64 mask) { return CPU_WORD & mask; }
		bool bits_set(i64 mask) { CPU_WORD |= mask; return true; }
		bool bits_clr(i64 mask) { CPU_WORD &= ~mask; return false; }
	
	// convert bit b mask // get_mask_of_bit = ( 1 << b )

		bool bit_test( int b ) { return bits_test( 1 << b ); }
		bool bit_set( int b )  { return bits_set( 1 << b ); }
		bool bit_clr( int b )  { return bits_clr( 1 << b ); }
	
	// rename // prefer bit_test( have_sent_bit ) //
	// have_sent = have_sent_bit_b // WANT type DIFF bit_b bits_mask
	// if flags.have_sent)

		bool test( int b ) { return bit_test( b ); }
		bool set( int b )  { return bit_set( b ); }
		bool clear( int b )  { return bit_clr( b ); }
		bool reset( int b )  { return bit_clr( b ); }

		// now add range of bits ....

// i64_WORD

	operator i64() { return CPU_WORD; }
	operator u64() { return (u64) CPU_WORD; }

	operator i16() { /* C without ASM */ 
		return (i16) CPU_WORD; // bounce down i16 operator
		// first time is applied to SELF (*this) a STRUCT with FIELD
		// second time is applied to CPU_WORD, not to (*this)
	};

	operator u16() { /* C without ASM */ 
		return (u16) CPU_WORD; // bounce down i16 operator
	};

	operator i8() { /* C without ASM */ 
		return (i8) CPU_WORD; // bounce down i8 operator
	};

	operator u8() { /* C without ASM */ 
		return (u8) CPU_WORD; // bounce down i8 operator
	};

	u8 u8_BYTE_B() { return (u16) CPU_WORD ) >> 8; }
	u8 u8_BYTE_A() { return (u8) CPU_WORD ); }

	i8 i8_BYTE_B() { return (i16) CPU_WORD ) >> 8; }
	i8 i8_BYTE_A() { return (i8) CPU_WORD ); }

	// ++ // set_BYTE_B ...
 
	// IIRC the Z80 only had signed bytes
	// 8086 286 386 AMD64 XEON_64 // ZEON_24_core_half

// get_i16 is NOT the basic low range bits, but an embedded BITFIELD
// Y	i16 get_i16() { return (CPU_WORD << 32 ) >> 32+16;  ) 
	i16 get_i16() { return (CPU_WORD << 32 ) >> 64-16;  ) 
	i16 get_i16_PAYLOAD() { return (CPU_WORD << 32 ) >> 64-16;  ) 
	i16 set_i16(i64); // compiled later // more rare // set_BITFIELD
	i64 get_u24() { return
	 (unsigned)( CPU_WORD << 32 ) >> 40; 
	}
	i64 get_i24() { return
		( CPU_WORD << 32 ) >> 40; 
	}
	i64 set_i24(i64 val);
	i64 set_u24(i64 val);

#endif

