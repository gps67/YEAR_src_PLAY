#ifndef u64_lohi_H
#define u64_lohi_H

#include "ints.h"

struct u64_hilo_t {
	u64 word;
};

#define SWAP_BYTES_64(t) t

struct u64_lohi_t
{
	u64 word;

	operator u64() {
	// IF ON LOHI ARCH
	 return word;
	// IF ON HOLI ARCH
	 return SWAP_BYTES_64(word);
	}
	operator i64() { return (i64)(u64)this; }
};



#endif
