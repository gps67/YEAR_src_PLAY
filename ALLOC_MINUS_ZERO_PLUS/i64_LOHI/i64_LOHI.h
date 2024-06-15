#ifndef i64_LOHI_H
#define i64_LOHI_H
// i64_LOHI/i64_LOHI.h

#if CXX_collapse_i64_WORD_t_to_i64

	typedef  i64  i64_WORD_t; // condenses to _i64_t // 

 #if 0
 	To make the above work also switch AXIOMS

		int get_BYTE_A( i64 A ) {
			ASM return A.AL
		}

		int get_i8_A( i64 A ) {
			ASM return A.AL // i64_from_i8
		}
		int get_u8_A( i64 A ) { // u64_from_u8
			ASM return A.AL
		}

		int get_i8_B( i64 A ) {
			ASM return A.AH // i64_from_i8_BYTE_B
		}
		int get_u8_B( i64 A ) { // u64_from_u8_BYTE_B
			ASM return A.AH
		}

		int get_i16_AB( i64 A ) {
			ASM return A.AX // LOHI MEMORY // PICK
		}
		int get_u16_AB( i64 A ) {
			ASM return A.AX
		}

		int get_i16_CD( i64 A ) {
			ALIAS i32_ABCD_lohi i32_WORD 
			A << 32;
			A >> 32 + 16 // SIGN SHIFT
			return A ; // A_i64
		}
		int get_u16_CD( i64 A_ ) {
			u64 A = (u64) i64 A_;

			A <<= 32;
			A >>= 32 + 16; // ZERO SHIFT // check + before >>=
			return A ; // A_i64
		}
		}

 #endif

#else
	struct i64_WORD_t { // CPU_WORD in a STRUCT of one field

		i64 i64_WORD; // CPU_WORD

		operator u64() { return (u64) i64_WORD; }
		operator i64() { return (i64) i64_WORD; }
	}

	// TODO_CODE HILO is NOT done here
	struct i64_LOHI_t : public i64_WORD_t { } ;
	struct u64_LOHI_t : public i64_WORD_t { } ;

	typedef i64_WORD_t  i64_LOHI_t; // LOHI is based on WORD // REQUIRED //
	// REQUIRE LOHI for i64_LOHI
	// DIALECT u64_LOHI // DIALECT_UNS //

	struct i8_i8_i16_i32_t : i64_LOHI_t {  // u32 often i8 i24 
	 i64 i64_WORD; // CPU_WORD
	};

	struct NOT_i8_i8_i16_i32 {  // u32 often i8 i24 
	 i8_A
	 i8_B
	 i16_CD
	 i32_EFGH
	};


 #endif // dont minimise collapse to int
#endif // include _H

