// bswap_htonl.c

// rewrite as part of libs_apps

typedef int u32;
typedef int u32_hilo;

#include "ASM_byte_swap.h"

// NAFF case statement - rewrite

#define ON_HERE_USE_ntohl		21
#define ON_HERE_USE_gcc_builtin		22
#define ON_HERE_USE_MS_byteswap_ulong	23
#define ON_HERE_USE_gcc_builtin_byteswap 24
#define ON_HERE_USE_glibc_byteswap	25
#define ON_HERE_USE_asm_byte_swap	26

#define ON_HERE \
	ON_HERE_USE_asm_byte_swap
// define ON_HERE ON_HERE_USE_ntohl
// todo // define ON_HERE ON_HERE_USE_glibc_byteswap
// define ON_HERE ON_HERE_USE_gcc_builtin_byteswap
// todo // define ON_HERE ON_HERE_USE_MS_byteswap_ulong

#if ON_HERE == ON_HERE_USE_ntohl
#include <arpa/inet.h>
#endif

#if ON_HERE == ON_HERE_USE_byteswap
#include <byteswap.h>
#endif

inline u32 u32_FROM_u32_hilo_( u32 hilo ) {

#if ON_HERE == ON_HERE_USE_gcc_builtin_byteswap
	return __builtin_bswap32( hilo );
/*
	movl %edi,%eax
	bswap %eax
	ret
*/

#elif ON_HERE == ON_HERE_USE_MS_byteswap_ulong
	return _byteswap_ulong( hilo );

#elif ON_HERE == ON_HERE_USE_ntohl
	return ntohl( hilo );
/*
	uses "/usr/include/x86_64-linux-gnu/bits/byteswap.h"
	uses __builtin_bswap32(int)

	movl %edi,%eax
	bswap %eax
	ret
*/

#elif ON_HERE == ON_HERE_USE_asm_byte_swap
#if 1
	return ASM_byte_swap( hilo );
#else

	// borrowed 
	// /usr/include/x86_64-linux-gnu/asm/swab.h
	// i486 gcc // actual i386 lacks bswap //  gcc "=r" malarky
	// -O2 put value from main into %eax // within main
	// -O2 gets rid of following temp var too
	u32 lohi = hilo;
//define lohi hilo
        __asm__("bswapl %0" : "=r" (lohi) : "0" (lohi));
	return lohi; // now lohi
#endif


#else
#error "ntohl code error"
#endif 
}

u32 test1( u32 hilo ) {
	return u32_FROM_u32_hilo_( hilo );
}




static u32 in_mem = 0x4321;

int main()
{
 u32 ret = test1( 0x01020304 );
 // gcc notices it is in situ
 ret++;
 ret = test1( ret );
 // programmers says insitu, gcc obeys & amp
 // ie & to reg not pointer to memory
 ASM_byte_swap_in_situ( in_mem );
 in_mem++;
 ASM_byte_swap_in_situ( ret );
 ASM_byte_swap_in_situ( in_mem );
 ret--;
 // gcc optimised 4321 to 4320 // so add step
 ret = u32_FROM_u32_hilo( ret );
 ASM_byte_swap_in_situ( ret );
 return ret != 0x04030201;
}
