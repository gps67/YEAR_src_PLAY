#ifndef sto_4K_H
#define sto_4K_H
/*
	STO is currently sto_4K
	So dont include this, include that

	STO aims for PAGE_4K alignment
	but "4096" and "4095" and "AS_BITMASK(-4096)" // TODO TEST THAT

	RULE 0xFF00 + 0x00FF to get 0xFFFF == -1
	RULE MASK_0_0xFF00 + MASK_1_0x00FF == get FULL_0xFFFF == -1
	RULE 0XFF00 == 0xFFFF - 0x00FF == -1 -0x00FF == -1 -255 = -256
	== 
*/

#include "obj_ref.h" // currently ref_counted in malloc HEAP

namespace STO {

	struct EA_EXPR_ : public obj_ref // obj_hold<EA_EXPR> 
	{
	 // update SELF to cache u32_u8_page0_u24_OFFS
	 // still needs GEN
	 //
	 // NEED lots of experimental u8_u24 variations
	 // show how simple basics are
	 // add semantic bindings, u24_is_ _OFFS _IDX _EXPR_u8_u16
	 // GEN will generate the ASM required
	 // regrettably via C_API but inline maight work

	 // bool DECODE_to_OBJECT( /* this, */  BINDING & OBJECT
	 
	};

};
#endif // this.h
