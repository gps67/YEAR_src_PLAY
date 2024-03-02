// bswap_htonl.c

// rewrite as part of libs_apps

// typedef int i32;
// typedef int i32_hilo;

#include "ints.h"
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

#define CPU_is_LIHO 1
#ifdef CPU_is_LIHO
#define i32 i32_cpu_FROM_lohi_( i32 lohi ) { return lohi; }
#define i32 i32_cpu_FROM_hilo_( i32 lohi ) { return hilo_FROM_i32_lohi(lohi); }

#define i64 i64_cpu_FROM_lohi_( i64 lohi ) { return lohi; }
#define i64 i64_cpu_FROM_hilo_( i64 lohi ) { return hilo_FROM_i64_lohi(lohi); }
#else
#define i32 i32_cpu_FROM_lohi_( i32 lohi ) { return hilo_FROM_i32_lohi(lohi); }
#define i32 i32_cpu_FROM_hilo_( i32 hilo ) { return hilo; }

#define i64 i64_cpu_FROM_lohi_( i64 lohi ) { return hilo_FROM_i64_lohi(lohi); }
#define i64 i64_cpu_FROM_hilo_( i64 hilo ) { return hilo; }
#fi


#if ON_HERE == ON_HERE_USE_ntohl
#include <arpa/inet.h>
#endif

#if ON_HERE == ON_HERE_USE_byteswap
#include <byteswap.h>
#endif

inline i32 hilo_FROM_i32_lohi_( i32 lohi ) {
	return i32 lohi_FROM_i32_hilo_( i32 lohi ); // AS GOOD AS // :-)
}

inline i32 lohi_FROM_i32_hilo_( i32 hilo ) {

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
	i32 lohi = hilo;
//define lohi hilo
        __asm__("bswapl %0" : "=r" (lohi) : "0" (lohi));
	return lohi; // now lohi
#endif


#else
#error "ntohl code error"
#endif 
}

i32 test1( i32 hilo ) {
	return i32_FROM_i32_hilo_( hilo );
}




static i32 in_mem = 0x4321;

int main()
{
 i32 ret = test1( 0x01020304 );
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
 ret = i32_FROM_i32_hilo( ret );
 ASM_byte_swap_in_situ( ret );
 return ret != 0x04030201;
}
