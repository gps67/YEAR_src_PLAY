#ifndef mmap_file_zone_H
#define mmap_file_zone_H

#include "STO.h"
#include "mmap_file.h"

namespace STO {

 /*!
 	256 ZONES in a shared FILE

	ZONES can also be shared
	but when a relocation is required - SYNC REPL
	sweep over line "STOP_until_relocation_WAIT"
	exit like EINTR, no new transactions
	complete existing transactions
	(hope memory not exhausted - early sweep)
	suspend exit, leaving task-list pending

	u16 ZONES in a shared FILE
 */
 class mmap_file_zone : public obj_ref // fd_hold_1 // obj_ref_
 { public:
	 mmap_file & mapped_file; // found elsewhere

	 u8_zone_idx zone_idx;
 	
 }; // ZONE


}; // STO
#endif

/*
 DECODE u32_hilo as u32_cpu then u16 u8 u8

 	struct DECODER_u32_hilo_into_u32_cpu
 	struct DECODER_32_cpu

	DST = zero filled from SRC
	DST = sign extended from SRC

	DST_SIZE_fill_upto-EAX upto { _REAX _EAX _AX _AL } 

	u8 u8_AL = ASM_pick_AL_from_register
	u8 u8_AH = ASM_pick_AH_from_register
	u16 u16_AX = ASM_pick_AX_from_register
	u32 u32_EAX = ASM_pick_AX_from_register
	u64 u64_REAX = ASM_pick_AX_from_register

	i8 i8_AL = ASM_pick_AL_from_register
	i8 i8_AH = ASM_pick_AH_from_register
	i16 i16_AX = ASM_pick_AX_from_register
	i32 i32_EAX = ASM_pick_AX_from_register
	i64 i64_REAX = ASM_pick_AX_from_register

	I am not familiar with ARM64
	so I dont know how it manages sign-exten-integer-register
	or pick AL AX EAX REAX, which it must do, somehow
	and 386 comes with added AH_hi AL_lo of i16 from i16_hilo
	NB after SWAPB (net_to_host_byte_order) hilo is hilo in register
	but you cant call it that, as that is reserved for the opposite of lohi
	Intel-X86 is lohi, so it is only a VISUAL effect, not name of byte hilo
	IE it stores registers in memory i16_lohi, needs SWAPB for hilo
	NB SWAPB for i32 XCHNG for i16, more diags ...

*/
