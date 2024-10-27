#ifndef u64_lohi_on_HILO_H
#define u64_lohi_on_HILO_H

#include "ints.h"

//	This is IF_DEF_CPU_U64_HILO on_HILO 

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
	i64 GET_as_i64() { return (i64)(u64)word; }
	operator i64() { return GET_as_i64(); }

	// UNION puts the DECODE_u64 MACROS on the INCL_LIST EXPR_LIST VARS POOL SESS CPU_REGISTER nbytes_STO API_SQL

// TODO HERE ///

	u64 GET_u64() { return word; }

};



#endif
