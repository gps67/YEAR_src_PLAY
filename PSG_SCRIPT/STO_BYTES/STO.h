#ifndef STO_H
#define STO_H
	/* 
		module STO 
		data_type_t STO_t // item_type in a STO frame

		This is the STUBS of all the modules exports that we use
		Each importing module, could even create a CT_RT_Obj & obj;
		// obj = obj( ARGS ) // FN = _CTOR_ pick by fn_args
		// pick by fn args // C_ANSI_fn_signatures // OTHER
	*/

	namespace STO {
	 // typedef caddr_t PTR; // DIALECT DROPPING _t

	 class STO_MGR_; // uplink to STO.STR_manager // DIALECT T_ from T_t 
	 class SEGMENT;
	 class mmap_file;

	};

	// the difference between a 1K and 4K STO_FILE in u16 K_limit
	// user allocated 200 KB for DODO_Segment
	// u16 x u10 = u26 of u32 - u6 
	// to MMAP it all, need to step back from u32
	// the 32 bit model will hold development back a bit
	// then it will step into its own MODULE_SPACE
	// which all fits WELL UNDER 64MB x 64_MODULE_SPACES // less some memory
	// to load it all, is to MMAP it all
	//
	// ENTIRE SEGMENT is under UDEF Layout
	// but it is NK_PAGES
	// caddr_t PAGE0;
	// int NK; // MAYBE less than u16, NOT NEGATE HERE, ZERO_is_first
	// and it occurs within container STO_
	// STO_MGR_UPLINK * STO_MGR; // owner of STO HEAP alloc, resize code ...
	
#include "sto_4k.h"

#endif
