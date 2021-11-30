#include "dgb.h"
#include <stdio.h>
typedef unsigned int uns;

// x86 linux gcc allocates bits low to high
// first thing is most natural
// next shifted above it
// pause at 32 bits for now

struct WORD1 {
 uns A:8;
 uns B:8;
 uns DC:16;
};

int main() {

 INFO("running");

 WORD1 word1;

 word1.A = 0xAA;
 word1.B = 0xBB;
 word1.DC = 0xDDCC;

 uns W = * (uns*) &word1;
 fprintf(stdout,"0x%4X \n", W );
 // 0xDDCCBBAA 

 return 0;
}
