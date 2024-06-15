#ifndef ASM_byte_swap_H
#define ASM_byte_swap_H

#include <byteswap.h> //


// INLINE // bswap edx // 0F CA // on i486

	// if in doubt use ntohl, it uses _byte_swap_
	// but this avoids including the world

	// require GCC ASM "=r" malarky
	// require i486 // i386 lacks bswap
	// require typedef int u32;

	// needs CALLER CODE to already know to SWAP_BYTES of NOT
	// needs COMPILER CODE to already know on_LOHI CPU or one that can

	// tcc cant handle "&" // it is C++ not C //
	// tcc says ASM_byte_swap.h:21: error: ',' expected (got "&")


inline void ASM_byte_swap_in_situ_32( u32 & situ ) {
	// receiving "{ u32 & situ }" works with GCC G++
	// it stays where it was, as a register

	// TODO // check that IN_SITU & can go to register
	// ok other syntax that worked
	// ok  __asm__("bswapl %0" : "=r" (situ) : "0" (situ));
	// ok  __asm__("bswapl %0" : "+r" (situ) );

 #if 1
	// ACTUAL ASM INSTRUCTION
        __asm__("bswapl %[io32]" : [io32] "+r" (situ) );
 #else
	// OPTIONAL non ASM
	situ = bswap_32( situ );
 #endif

	// return VOID - INSITU 
}

inline void ASM_byte_swap_in_situ_64( u64 & situ ) { // YES ASM //
	// OK not yet ASM, but C uses libr bswap_64 uses ASM so YES
	situ = bswap_64( situ ); // #include <byteswap.h> //
}

inline u32 ASM_byte_swap( u32 insitu ) {
	// it receives insitu as a COPY value on the stack or inline
	// it returns a copy of that copy, but C reduces that
	ASM_byte_swap_in_situ_32( insitu );
	return insitu; // now lohi
}

inline u64 ASM_byte_swap( u64 insitu ) {
	// it receives insitu as a COPY value on the stack or inline
	// it returns a copy of that copy, but C reduces that
	ASM_byte_swap_in_situ_64( insitu );
	return insitu; // now lohi
}

#endif
