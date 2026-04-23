#ifndef BYTE_ORDER_H
#define BYTE_ORDER_H

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
 // Intel LOHO
 #define CPU_LOHI 1
 #define CPU_HILO 0
 #warning "currently be testing on AMD64 - soon delete this message"
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
 // except ARM is also LOHI ...
 // ARM Motorola HILO
 #define CPU_HILO 1
 #define CPU_LOHI 0
 #warning "to be tested on ARM - then delete this message"
#else
 error "I dont support __ORDER_PDP_ENDIAN__ whatever that is"
#endif


	/*
		TODO find expr that GCC recognises CLANG for BITS_0_7 // (u8)
		TODO find expr that GCC recognises CLANG for BITS_0_15 // i16
		TODO find expr that GCC recognises CLANG for BITS_8_15

 		HERE MACRO {
		 inline
		 u8 get_u8_hi_from_u32( u32 word )
		 { return (word>>8) & 0xFF; }
		}

		DH register u8 from MACRO_EXPR
		DL register CAST u8
		DX register CAST u16
		EDX register CAST u32
		REDX register CAST u64

		ABCD_PQ_SRC_DST SP_FP_Stack_Frame _Layout

		 ABCD
		  FOUR_REGS A B C D // EACH U32_U16_u8_u8_BYTE_B
		 PQ
		  SRC DST // ITEM * PTR // ALSO u32_u16_u8 // _no_BYTE_B_no

		 MMX_FPU_REGS
		  [u4] 
		  [ N_found ] 
		  A << drop_left
		  A >> drop_right
		  A == (u4) // u6 happens a lot // u3 mini_scope_zone
		  drop_right_beyond_drop_left_starting_point = drop_right - drop_left
		  // zero_bits_above above drop_left=DONE
		  N_found_bits = 32 - drop_right;
		  N_Found = KNOW_3

		 Stack_Frame

		_Layout is external EXPR that MATCHES along with MATCH ARGS
		_POOL_ provided by _Layout_of_Module EXPR_NAME("anystr")
		// REQUIRE NOT ANYSTR // bountiful_TOKEN_POOL //  EACH -> EXPR

		HERE ASM

	*/

	/*
		u32_abcd

		when loaded into x86 u32 looks like dcba
		so AH == b AL == a 
		>>16
		so AH == d AL == c 

		swapb provides abcd
		so AX == cd AH == c AL == d
		>>16 
		so AX == ab AH == a AL == b

		when ARM maybe shift and move from R99 ro R4
	*/

   inline u32 ASM_get_u8( u32 val ) {
   	// return DL from EDX
	// OK this is C not ASM
	// to allow code to depend on it
	// even if not actually ASM
	return (u8) val;
   }

   inline u8 ASM_return_DL_from_EDX( u32 & edx ) {
   	// return DL from EDX
	u8 dst = edx & 0xFF;
	// OK this is C not ASM
	// to allow code to depend on it
	// even if not actually ASM
	return dst;
   }

   inline u8 ASM_return_DH_from_EDX( u32 & edx ) {
	u8 dst = (edx>>8) & 0xFF;
	return dst;
   }

   inline u32 ASM_byte_swap( u32 & var ) {
   	// in-situ in the register, passing through an inline function
	// a MACRO would do it in situ notice var is a register
	// PICK:
 //	__asm__("bswapl %0" : "=r" (var) : "0" (var));
	__asm__("bswapl %[io32]" : [io32] "+r" (var) );
	return var;
   }


#endif

