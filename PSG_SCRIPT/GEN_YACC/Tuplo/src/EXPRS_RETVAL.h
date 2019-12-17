#ifndef EXPR_RETVAL_H
#define EXPR_RETVAL_H

#include "EXPRS_STUBS.h"

namespace EXPRS {

 struct RETVAL_builder_t {
  EXPR * expr_tree;

  RETVAL_builder_t()
  : expr_tree( 0L )
  {
  }

 }; // struct

 extern
 RETVAL_builder_t &
 get_RETVAL_builder();

}; // namespace
#endif

