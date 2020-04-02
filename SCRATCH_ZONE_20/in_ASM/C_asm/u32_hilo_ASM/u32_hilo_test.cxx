typedef short i16;
typedef unsigned short u16;
typedef int i32;
typedef unsigned int u32;
typedef long long i64;
typedef unsigned long long u64;

#include "ASM_hilo.h"

#include <stdio.h>

struct top_t {
	u32_hilo hilo_2;
	u32_hilo hilo_4;
	u32_hilo hilo_8;
};

static top_t top;

bool u32_hilo_test_1() {

	top_t top_b = top;
	top_b.hilo_2 = 0x6789;
	top = top_b;
	return true;

}

bool u32_hilo_test_2() {

	u32_hilo hilo;
	hilo = 0x55667788;
	printf( "%4.4X test2 internal\n", hilo.word_abcd );
	printf( "%4.4X test2 as u32\n", (u32) hilo );
	hilo = 0x11223344;
	printf( "%4.4X test2 internal\n", hilo.word_abcd );
	printf( "%4.4X test2 as u32\n", (u32) hilo );
	return (hilo == 66);
}

bool bool_main()
{
	if(! u32_hilo_test_1() ) return false;	
	if(! u32_hilo_test_2() ) return false;	
	return true;
}

int main()
{
	if(bool_main()) return 0;

	return 99;
}
