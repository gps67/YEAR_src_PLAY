
#include "enum_cls.h"
#include "enum_val.h"

extern PyMethodDef enum_val_methods[];

enum_val::enum_val(
	enum_cls * 	classspec_,
	int    		intval_,
	const char * 	strval_
)
: PY_Object_base(&enum_val_Type)
, classspec( classspec_ )
, intval( intval_)
, strval()
{
	strval.set_str( strval_ );
}

static void
enum_val_dealloc( enum_val * self )
{
        delete self;
}

static PyObject * enum_val_getattr( enum_val *self, char *name )
{
        return self->get_attr_tail( enum_val_methods, name );
}

static int enum_val_setattr( enum_val *self, char *name, PyObject *v )
{
        return self->set_attr_tail( name, v );
}


PyMethodDef enum_val_methods[] = {
        {NULL,          NULL}           /* sentinel */
};

PyTypeObject enum_val_Type = {

         PyObject_HEAD_INIT(&PyType_Type)
                0,				/*ob_size*/
         (char *) "enum_val",			/*tp_name*/
                sizeof( enum_val ),		/*tp_basicsize*/
                0,				/*tp_itemsize*/
         /* methods */
         (destructor)   enum_val_dealloc,	/*tp_dealloc*/
         (printfunc)    0,			/*tp_print*/
         (getattrfunc)  enum_val_getattr,	/*tp_getattr*/
         (setattrfunc)  enum_val_setattr,	/*tp_setattr*/
                0,                              /*tp_compare*/
                0,                              /*tp_repr*/
                0,                              /*tp_as_number*/
                0,                              /*tp_as_sequence*/
                0,                              /*tp_as_mapping*/
                0,                              /*tp_hash*/
};

