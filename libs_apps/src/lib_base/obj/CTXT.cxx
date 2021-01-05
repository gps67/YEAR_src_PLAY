#include "CTXT.h"

using namespace BASE1;

static err_int_t err_int_global;

// extern
err_int_t & BASE1::get_err_int_for_this_thread()
{
//	e_print("Returning err_int_global %p\n", & err_int_global );
	return err_int_global;
}

