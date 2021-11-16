#include "PY_weak_link.h"

static void PY_weak_link_dealloc( PY_weak_link * self )
{
	self->item = 0;
	// instead of this, use C++ delete
	// (self)->PY_weak_link::~PY_weak_link(); // doesnt free() mem
	// PyMem_DEL( self );
	delete self;
}

PyTypeObject PY_weak_link_Type = {

         PyObject_HEAD_INIT(&PyType_Type)
                0,                              /*ob_size*/
         (char *) "PY_weak_link",             /*tp_name*/
                sizeof( PY_weak_link ),       /*tp_basicsize*/
                0,                              /*tp_itemsize*/
         /* methods */
         (destructor)   PY_weak_link_dealloc, /*tp_dealloc*/
         (printfunc)    0,   /*tp_print*/
         (getattrfunc)  0, /*tp_getattr*/
         (setattrfunc)  0, /*tp_setattr*/
                0,                              /*tp_compare*/
                0,                              /*tp_repr*/
                0,                              /*tp_as_number*/
                0,                              /*tp_as_sequence*/
                0,                              /*tp_as_mapping*/
                0,                              /*tp_hash*/
};

