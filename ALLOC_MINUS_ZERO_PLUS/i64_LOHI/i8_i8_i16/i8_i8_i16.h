#ifndef i8_i8_i16_H
#define i8_i8_i16_H

typedef char i8;
typedef unsigned char u8;

typedef short i16;
typedef unsigned short u16;

typedef int i32;
typedef unsigned int u32;

struct ERROR_t; // ERRRM_H

// NB // LOHI // see explain LOHI

struct i32_BITFIELDS {

	i32 WORD_i32;

	i32_BITFIELDS(i32 INIT_VAL) { // CTOR
		WORD_i32 = INIT_VAL;
	}
};


struct i8_i8_i16_BITFIELDS: public i32_BITFIELDS {

	i8_i8_i16_BITFIELDS(i32 INIT_VAL) // probably *P=VAL 
	: i32_BITFIELDS(INIT_VAL)
	{
		// MINIMAL CTOR // when SMART COMPILER 
	}

};

struct i8_i8_in_CPU_WORD {

	i8 get_i8_BYTE_A();
	u8 get_u8_BYTE_A();
	//
	i8 get_i8_BYTE_B();
	u8 get_u8_BYTE_B();
	//
	i16 get_i16_BA(); // return AX SIGNED
	u16 get_u16_BA(); // return AX UNSIGNED
	//
	i16 get_i16_clipped() { return get_i16_BA(); }
	u16 get_u16_clipped() { return get_u16_BA(); }
	//
};

struct i8_i8_i16 : i8_i8_in_CPU_WORD {

	i16 DECL_CD; // packed BITFIELD decl part;

	// rewrite these as inline ASM 
	// expect R2 = R1.BYTE_A // SIGN EXTEND BYTE to CPU_WORD ie_i64

	i16 get_i16_PAYLOAD() { return CPU_WORD_i32 >> 16 ; } 
	u16 get_u16_PAYLOAD() { return CPU_WORD_u32 >> 16 ; } 

	i16 get_i16_SHIFTED() { return i16 get_i16_PAYLOAD(); }
	u16 get_u16_SHIFTED() { return u16 get_u16_PAYLOAD(); }
	//

	i32 get_i32_PAYLOAD() { return i32_ABCD(); }
	u32 get_u32_PAYLOAD() { return u32_ABCD(); }
	//
	i32 i32_ABCD();
	u32 u32_ABCD();
	//
	// i16 DC_in_WORD; // meaninless // get_i16_PAYLOAD() //

	// ELF provides BOTH callable C_FUNC and INLINE_MACRO_ASM


	 //	NULL means OK_NULL

};

union _i32_t {

	i32 _i32;


	i8_i8_i16_t 
	_i8_i8_i16;
}
#endif
