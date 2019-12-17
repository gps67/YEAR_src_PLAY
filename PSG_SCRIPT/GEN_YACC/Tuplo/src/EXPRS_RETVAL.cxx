
#include "EXPRS_RETVAL.h"

using namespace EXPRS;

 RETVAL_builder_t &
 EXPRS:: 		// namespace not class
 get_RETVAL_builder()
 {
 	static RETVAL_builder_t TREE;
 	return TREE;
 }
 

