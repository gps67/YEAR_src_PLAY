#ifndef STO_H
#define STO_H
	/* 
		module STO 
		data_type_t STO_t // item_type in a STO frame

		This is the STUBS of all the modules exports that we use
		Each importing module, could even create a CT_RT_Obj & obj;
		// pick by fn args // C_ANSI_fn_signatures // OTHER
		// obj = new obj( ARGS ) // DIALECT_SHIFT drop the new keyword
		// obj = obj( ARGS ) // api CTOR rules apply // morph_transport
		// obj = obj( ARGS ) // FN = _CTOR_ pick by fn_args
		// ITEM_t & item = lookup( pool, item ) ; // FRAME_var "item"
		// some RULES includes use of NAME_t & name = lookup("anystr");
		// some RULES include use of GCC/ASM/SCRIPT, CT_machine = DIAG
		// some RULES include use of DIAG + ANIM + LEXICON.script["%s"]
		// EG2
		// Parse stuff like { LEXICON.script["%s"] }
		// Match // LEXICON.script["%s"] } BLOB_QUOTED_anystr
		// Use of Name <--> IDX <--> USE // _of_SCRIPT
		// COMPILE_TOKENISE_EXPR ( CTXT, NAME, OPCODE )
		// OPCODE BIND_EA_EXPR // it doesn't have to run, just use id
		// BIND LINK, link = //EA// lookup( name )
		// BIND LINK link = some_other_EA_expr

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
