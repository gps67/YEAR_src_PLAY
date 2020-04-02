#ifndef ASM_byte_swap_H
#define ASM_byte_swap_H

// INLINE // bswap edx // 0F CA // on i486

	// if in doubt use ntohl, it uses _byte_swap_
	// but this avoids including the world

	// require GCC ASM "=r" malarky
	// require i486 // i386 lacks bswap
	// require typedef int u32;

inline void ASM_byte_swap_in_situ( u32 & situ ) {
	// TODO // check that IN_SITU & can go to register
 // ok  __asm__("bswapl %0" : "=r" (situ) : "0" (situ));
 // ok  __asm__("bswapl %0" : "+r" (situ) );
        __asm__("bswapl %[io32]" : [io32] "+r" (situ) );
	// TODO // "bswapl %[insitu]" : [insitu]"+r" (situ) //)
	// 0 means like %0 // % is for at+t asm ??
	// http://www.ethernut.de/en/documents/arm-inline-asm.html
	// http://ericw.ca/notes/a-tiny-guide-to-gcc-inline-assembly.html
	// REV is ARM reverse swap bytes
}

inline u32 ASM_byte_swap( u32 insitu ) {
	// C // insitu is already a copy
	ASM_byte_swap_in_situ( insitu );
	// __asm__("bswapl %0" : "=r" (insitu) : "0" (insitu));
	return insitu; // now lohi
}

inline u32 u32_FROM_u32_hilo( u32 hilo ) {
	return ASM_byte_swap( hilo );
}

#endif
