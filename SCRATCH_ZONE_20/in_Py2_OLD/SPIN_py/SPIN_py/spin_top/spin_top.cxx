#include "ParserEventGeneratorKit.h"
#include "sp_peg_kit.h"
#include "SP_App.h"

#include "Python.h"
#include "app_link.h"
#include "argv_builder.h"
#include "spin_top.h"

#include "SPIN_type_Check.h"

// FORWARD
extern PyMethodDef SPIN_top_methods[];

// NAFF way of making a simple thing all complicated

#include "spin_top/spin_top_new.cx"
#include "spin_top/spin_top_dealloc.cx"
#include "spin_top/spin_top_getattr.cx"
#include "spin_top/spin_top_setattr.cx"
//#include "spin_top/spin_top_halt.cx"
#include "spin_top/spin_top_opt.cx"
#define spin_top_print 0
#include "spin_top/spin_top_tree_add.cx"
#include "spin_top/spin_top_run.cx"


PyTypeObject SPIN_top_Type = {

         PyObject_HEAD_INIT(&PyType_Type)
                0,                              /*ob_size*/
         (char *) "spin_top",             	/*tp_name*/
                sizeof( SPIN_top ),       	/*tp_basicsize*/
                0,                              /*tp_itemsize*/
         /* methods */
         (destructor)   spin_top_dealloc,	/*tp_dealloc*/
         (printfunc)    spin_top_print,  	/*tp_print*/
         (getattrfunc)  spin_top_getattr,	/*tp_getattr*/
         (setattrfunc)  spin_top_setattr, 	/*tp_setattr*/
                0,                              /*tp_compare*/
                0,                              /*tp_repr*/
                0,                              /*tp_as_number*/
                0,                              /*tp_as_sequence*/
                0,                              /*tp_as_mapping*/
                0,                              /*tp_hash*/
};
 static char doc_SPIN_halt[] =
         "(eh) -- stops the SP thread started by eh";
 static char doc_SPIN_opt[] =
         "(eh,opt,value) -- set option in running eh";
 static char doc_SPIN_tree_add[] =
         "(eh,value) -- add text or item to top of stack";
 static char doc_SPIN_run[] =
         "(argv,eh) -- send sgmlnorm.run(argv) like events to eh.fns()";

// static
PyMethodDef SPIN_top_methods[] = {

 {(char *)"app_opt",    (PyCFunction) spin_top_app_opt, 1, doc_SPIN_opt},
 {(char *)"halt",     	(PyCFunction) spin_top_halt, 1, doc_SPIN_halt},
 {(char *) "tree_add", (PyCFunction)   SPIN_tree_add, 1, doc_SPIN_tree_add },
 {(char *) "run",      (PyCFunction)   SPIN_run,       1, doc_SPIN_run },
 {         NULL,		                NULL}		/* sentinel */
};
/* --------------------------------------------------------------------- */
