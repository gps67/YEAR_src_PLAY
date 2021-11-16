#ifndef SPIN_tree_print_H
#define SPIN_tree_print_H

#include "Python.h"
#include "SPIN_type_Check.h"

// no need - see getattr
// extern PyMethodDef SPIN_tree_print_methods[]; // forward
class SPIN_tree_print;

//extern SPIN_tree_print * as_SPIN_tree_print( PyObject * elem );
extern PyObject * new_SPIN_tree_print( PyObject * elem, PyObject * args );



#endif
