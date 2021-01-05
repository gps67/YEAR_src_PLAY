#ifndef spout_STUBS_H
#define spout_STUBS_H

// HINT you  #include "spout_obj_ref.h"
// it includes this
// only when you actually need obj_ref_js

// not mine to define
//	class obj_ref_js;	// not in namespace

namespace SPOUT_GEN {

 	class spout_obj_ref; 
	//class SPOUT_t;	// CSR_t // all in one

	class spout_PAGE_base;
	typedef spout_PAGE_base PAGE_t;
	typedef spout_PAGE_base SPOUT_t;
	class CSR_t;
	class PS_fragments_t; 

	// _Layout is PAGE has CSR
	// CSR is SPOUT
	// want some split, PS generation, PS rules, Layout

}; // namespace

/*
	spout_t == csr_t == all the same
	ctxt_t == above EXCEPT:

	 when searching for FONTS on local disk files
	  ctxt has search path
	 when preparing ZIP of out files
	  ctxt has SHELF 
	 when reading colour palette or user profile
	  ctxt has ...

	 PEN + INK
	 PAGE PAPER DOCUMENT # allow cover to be different paper source ?
	 SHELF DIR ZIPFILE
	 WEB target ../furniture/respirce files (not SPOUT per se)

ALSO

	bool broken_spout = false; // all is OK
	// FAIL ENOMEM no disk space no perm ...
	// FAIL essential FONT not available
	// FAIL database cant find essential data for report
	// FAIL anything that completely invalidates this document + this ZIP
	// caller decides  some of this
	// our code returns on FAIL or broken_spout // document_broken
*/


#endif

