/* SPIN_dtd_tables objects */

#include "spin_py_mod.h"
#include "Python.h"
#include "dtd_tables.h"
#include "str_obj.h"

#include "SPIN_type_Check.h"

/*
	spin_dtd_tables holds a dictionary of elem_info items

	Each elem_info points back to its creating pool via a

		PY_weak_link * uplink;

	This enables the manual generator to create child nodes
	in the same dtd-pool, but might be confusing when mixing
	three DTD'd IN-MID-OUT (SGML-TREE-HTML).

*/


/* --------------------------------------------------------------------- */
//		SPIN	dtd_tables	TYPE
/* --------------------------------------------------------------------- */

extern PyMethodDef spin_elem_info_pool_methods[]; // forward without problem!

extern SPIN_dtd_tables *
as_SPIN_dtd_tables( PyObject * elem )
{
	/*
		for now this is a type check,
		it could cast different types,
		or name to elem_info_pool (via app_link's dictionary
		but where is app_link)
	*/
	if (elem && SPIN_dtd_tables_Check( elem )) {
		return (SPIN_dtd_tables *) elem;
	} else {
		PyErr_SetString(PyExc_TypeError,"SPIN_dtd_tables expected");
		return (SPIN_dtd_tables *) NULL;
	}
}

/*
	playing with Pythons API, to assist debug printing
*/

static int
spin_elem_info_pool_print (
        SPIN_dtd_tables *op,
        FILE *fp,
        int flags )
{
	// LURK this is not re-loadable
	// but at least it names the element
        fprintf(fp, "(SPIN_dtd_tables");
        fprintf(fp, ")");
        return 0;
}


/* --------------------------------------------------------------------- */
//		SPIN	elem_info_pool	new ()
/* --------------------------------------------------------------------- */

/* EXPORTED */
SPIN_dtd_tables * new_SPIN_dtd_tables()
{
	return new SPIN_dtd_tables();
}

/* --------------------------------------------------------------------- */
//		SPIN	elem_info_pool	dealloc, get/set attrs
/* --------------------------------------------------------------------- */

static void
spin_elem_info_pool_dealloc( SPIN_dtd_tables *self )
{
	delete self;
}

/*
*/

static PyObject *
spin_elem_info_pool_getattr(
	SPIN_dtd_tables *self,
	char *name )
{
	if(0==strcmp( name, "dict_elem_info" ))
	{
		return self->dict_elem_info.new_ref(); // Py_None if absent
	}
	if(0==strcmp( name, "dict_sdata" ))
	{
		return self->dict_sdata.new_ref(); // Py_None if absent
	}
	return self->get_attr_tail( spin_elem_info_pool_methods, name );
}

static int
spin_elem_info_pool_setattr(
	SPIN_dtd_tables *self,
	char *name,
	PyObject *v )
{
	return self->set_attr_tail( name, v );
}
/* --------------------------------------------------------------------- */
//	SPIN  elem_info_pool  Type -- 
/* --------------------------------------------------------------------- */

// static // warning due to previous declaration 
// which HAD to be external for g++ (but not gcc)

PyTypeObject SPIN_dtd_tables_Type = {

	 PyObject_HEAD_INIT(&PyType_Type)
		0,				/*ob_size*/
	 (char *) "spin_elem_info_pool",		/*tp_name*/
		sizeof(	SPIN_dtd_tables ),	/*tp_basicsize*/
		0,				/*tp_itemsize*/
	 /* methods */
	 (destructor)	spin_elem_info_pool_dealloc, /*tp_dealloc*/
	 (printfunc)	spin_elem_info_pool_print,	/*tp_print*/
	 (getattrfunc)	spin_elem_info_pool_getattr, /*tp_getattr*/
	 (setattrfunc)	spin_elem_info_pool_setattr, /*tp_setattr*/
		0,				/*tp_compare*/
		0,				/*tp_repr*/
		0,				/*tp_as_number*/
		0,				/*tp_as_sequence*/
		0,				/*tp_as_mapping*/
		0,				/*tp_hash*/
};

/* --------------------------------------------------------------------- */
//		SPIN	elem_info_pool	UNUSED
/* --------------------------------------------------------------------- */

/*
	obj.method( self, args ) (other than the standard ones)

	I couldnt think of any functions I want from elem_info_pool objects
	but having a template will help later
*/

static PyObject *
spin_elem_info_pool_hello(
	SPIN_dtd_tables *,
	PyObject *args )
{
	PyObject * string;
	printf("spin_elem_info_pool_hello()\n");
	if (!PyArg_ParseTuple(args, (char *)""))
		return NULL;
	string = PyString_FromString("hello");
	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef spin_elem_info_pool_methods[] = {
	{(char *)"hello",	(PyCFunction)spin_elem_info_pool_hello,	1},
	{NULL,		NULL}		/* sentinel */
};

/* --------------------------------------------------------------------- */
//		SPIN	elem_info_pool	MODULE functions
/* --------------------------------------------------------------------- */
