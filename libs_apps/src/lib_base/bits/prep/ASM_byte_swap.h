#ifndef ASM_byte_swap_H
#define ASM_byte_swap_H

	// require GCC ASM "=r" malarky
	// require i486 // i386 lacks bswap
	// require typedef int u32;

inline void ASM_byte_swap_in_situ( u32 & situ ) {
	// TODO // check that IN_SITU & can go to register
        __asm__("bswapl %0" : "=r" (situ) : "0" (situ));
}

inline u32 ASM_byte_swap( u32 insitu ) {
	// C // insitu is already a copy
	ASM_byte_swap_in_situ( insitu );
	// __asm__("bswapl %0" : "=r" (insitu) : "0" (insitu));
	return insitu; // now lohi
}

inline u32 u32_FROM_u32_hilo_( u32 hilo ) {
	return ASM_byte_swap( hilo );
}

#endif
