#ifndef spout_ctxt_H
#define spout_ctxt_H
#include "spout_STUBS.h"

// #include "spout_base_0.h"
// #include <stdlib.h>

namespace SPOUT_GEN {

	class spout_base;	// virtual thing that produces a document



/*!
	The SPOUT library has one contaext that could produce a series of document,
	but only one doc can be active at a time

	it might parse a few AFM files, and keep them

	CTXT {
	 YOUR SCRIPT
	 SOME DATA FILES
	}

	_PAGE_ = CTXT.CURR_PAGE // type case // bind var val tree //
	_PAGES_ = ps utils

		https://github.com/rrthomas/psutils
	
	_PIPES_ += UNIX_COMMAND WIN32_COMMAND

		building a pipeline

		 GEN right to left // like letting out a kite

		 pre_fork()
		  CODE
		 call_fork()
		  know on_
		  on_FAIL() // so must be on_PARENT but no data
		   CODE to DIAGNOSE_FAIL
		   REACT = return FAIL_FAILED( explain ); 
		  on_PARENT()
		   CODE
		   // BACK FORK started
		  on_CHILD()
		   CODE
		   EXEC_ARGV // ENV fds TTY the works
		   // GONE_TO TASK

	spout _base = PAGE CSR FILE DIAG_CODE
	 SPOUT_t

*/
class spout_ctxt : public spout_obj_ref
{
 public:
	SPOUT_t * spout;
	// err_info ei;

	~spout_ctxt();
	spout_ctxt();


	bool run_wrapped_report( spout_base * report );
};


}; // NAMESPACE

#endif
