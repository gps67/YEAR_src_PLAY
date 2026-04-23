#ifndef E32_H
#define E32_H

typedef int i32;
typedef unsigned int u32;
typedef short i16;
typedef unsigned short u16;
typedef char i8;
typedef unsigned char u8;

namespace STO {

 /*
 	HERE It is all u32 all the way, often stored to u32_hilo _t

	You can tell the difference,
	u32_hilo uses a struct . method( ARGS )
	E32 uses INLINE_FUNC( e32 )

	E32 goes to u8_u24
 	E32 cant get to u16_u8_u8 // get DH of EDX
 */

 // struct E32 { ... } // NOT THIS 

 typedef u32 E32_t; // WORD // unused though

 // union CT tagged
 // u32 -> u8_u24
 // u32 -> u8_u8_u16_or_u8_u24
 // u32 -> u8_u8_u16
 // u32 -> u8_u24
 // u32 -> u16_u16
 // u32 -> i16_i16

 // YES it is written idx_OFFS hi_LO
 // but implemented OFFS_idx for ASM // C = (u8) D
 // subsequent >>8 loses idx, but can drop idx // DONE

   inline u32 mk_u8_u24( u8 _u8, u32 _u24 ) { return (_u24 << 8) + _u8; }
// inline u8 get_u8_from_u8_u24( u32 u8_u24 ) { return u8_u24 & 0xFF; }
   inline u8 get_u8_from_u8_u24( u32 u8_u24 ) { return (u8) u8_u24 ; }
   inline u32 get_u24_from_u8_u24( u32 u8_u24 ) { return u8_u24 >> 8 ; }

// TEST u8 get_u8_from_u8_u24( u32 u8_u24 ) { return (u8) u8_u24 ; }
// PEER u8 get_u8_from_u8_u24( u32 u8_u24 ) { return u8_u24 & 0xFF; }
   inline u8 get_u8_hi_from_u32( u32 _word ) { return (_word>>8) & 0xFF; }

 // support i16 by providing u16, caller can specifically cast (i16) _u16;
// PICK
// inline u16 get_u16_HIGH( u32 _word ) { return _word >> 16; }
   inline i16 get_u16_HIGH( u32 _word ) { return ((i32)_word) >> 16; }
// inline u16 get_u16_LOW( u32 _word ) { return (u16) _word;  } // CPU chops // 
   inline i16 get_u16_LOW( u32 _word ) { return (i16) (i32) _word;  } // 
   inline u32 E32_from_u16( u16 _u16 ) { return (u32) _u16;  } // 
   inline u32 E32_from_i16( i16 _i16 ) { return (i32) _i16;  } // SIGN-EX 1st
   inline u16 u16_from_E32( u32 _word ) { return       _word; } // use uns
   inline i16 i16_from_E32( u32 _word ) { return (i32) _word; } // SIGN-EX 1st

   inline u32 get_u32( u32 _word ) { return _word;  } // call as if an INST
   inline u32 get_i32( u32 _word ) { return (i32) _word;  } // opaque yet int

   /*
   	CAN we reduce this to

		typedef u32 OFFS_idx;
	
	KNOW to save as u32_hilo
   */

   typedef u32 u8_u24;
   typedef u8_u24 OFFS_idx; // E32{ u8_idx u24_OFFS }


   /*
   	OK that cheats twice - we dont know this is ASM_INTEL
   	OK that cheats twice - we dont know this is CPU_LOHI

	Here is the E32 component - it only uses GCC CPP vars
   */

}; // namespace

#if 0
 typedef u8_u24 OFFS_idx; // E32{ u8_idx u24_OFFS }
 E32{ u8_idx u24_OFFS }
 TUPLO {
  SPEC = E32;
  SPEC = u8_u8_POOL_ITEM _in_E32
 }

#endif
#endif // _H
