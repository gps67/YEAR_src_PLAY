#ifndef CTXT_h
#define CTXT_h

#include "obj_ref.h"
#include "CTXT_stubs.h"
#include "err_int.h"

namespace BASE1
{
 struct DIAG
 {
	err_int_t err_int;
 };

 struct CTXT
 {
	int somethings;
	DIAG * not_a_clue;
	int morethings;
 };

}; // NS BASE1
#endif
