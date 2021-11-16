/* spin_attr_obj objects */

#include "Python.h"
#include "attr_obj.h"

/*
	
*/


/* --------------------------------------------------------------------- */
//		SPIN	attr_obj	TYPE
/* --------------------------------------------------------------------- */

extern PyMethodDef spin_attr_obj_methods[]; // forward without problem!



static int
spin_attr_obj_print (
        SPIN_attr_obj *op,
        FILE *fp,
        int flags )
{
	// LURK this is not re-loadable
	// but at least it names the element
        fprintf(fp, "(ELEM-");
	PyObject_Print(op->eleminfo->name, fp, 0);	// LURK null
        fprintf(fp, ")");
        return 0;
}


/* --------------------------------------------------------------------- */
//		SPIN	attr_obj	new (name, is_pre)
/* --------------------------------------------------------------------- */

/* EXPORTED */
SPIN_attr_obj *
new_SPIN_attr_obj( AppObject info1, AppAttrList atts1, AppObject data1 )
{

	SPIN_attr_info * info;

#warning "CUT_AND_PASTE"
	info = as_SPIN_elem_info( info1 );
	if(NULL==info) return NULL;

	if(!PyList_Check( atts1 )) {
		PyErr_SetString(PyExc_TypeError,"list expected");
		return NULL;
	}
	if(!PyList_Check( data1 )) {
		PyErr_SetString(PyExc_TypeError,"list expected");
		return NULL;
	}

	SPIN_attr_obj *self;
	self = PyObject_NEW( SPIN_attr_obj, &SPIN_attr_obj_Type );
	if (!self) return NULL;

	self->eleminfo = info;
	self->attslist = atts1;
	self->datalist = data1;
	Py_INCREF( self->eleminfo );
	Py_INCREF( self->attslist );
	Py_INCREF( self->datalist );

	return self;
}

/*
	new is special, Python never calls it (this isn't a class, it's a
	type), and it needs to be accessed through some lookup, such as a
	module. That routine, (which as self=module) calls this new, but
	now with customised parameters.
*/
/*
	new_SPIN_attr_obj( PyObject *dict, PyObject *name, int pre )
	is special - it's the ONLY thing export from this module,
	and it registeres itself by simply existing (and holding its own
	TypeTable pointer)
	
*/
/*
	return the type of a SPIN_attr_obj
	for when you don't have to to test
	and you can't be bother to fake creating one
*/
PyObject * type_SPIN_attr_obj( void )
{
	Py_INCREF( & SPIN_attr_obj_Type );
	return (PyObject *) & SPIN_attr_obj_Type;
}


/* --------------------------------------------------------------------- */
//		SPIN	attr_obj	dealloc, get/set attrs
/* --------------------------------------------------------------------- */

static void
spin_attr_obj_dealloc( SPIN_attr_obj *self )
{
	Py_XDECREF(self->eleminfo);
	Py_XDECREF(self->attslist);
	Py_XDECREF(self->datalist);
	PyMem_DEL(self);
}

/*
	get/set attr is called for vars and also for functions
	if the result is/nt data/code it is run (one route for all)
	Is there a compilation cache? That would be nice (or not)
*/

static PyObject *
spin_attr_obj_getattr(
	SPIN_attr_obj *self,
	char *name )
{
	if(0==strcmp( name, "info" ))
	{
		Py_INCREF( self->eleminfo ); // can't be null!
		return (PyObject *) self->eleminfo ; 
	}
	if(0==strcmp( name, "atts" ))
	{
		Py_INCREF( self->attslist ); // can't be null!
		return self->attslist ; 
	}
	if(0==strcmp( name, "data" ))
	{
		Py_INCREF( self->datalist ); // can't be null!
		return self->datalist ; 
	}
	return self->get_attr_tail( spin_attr_obj_methods, name );
}

static int
spin_attr_obj_setattr(
	SPIN_attr_obj *self,
	char *name,
	PyObject *v )
{
	if(0==strcmp( name, "info" ))
	{
		SPIN_attr_info * info;
#warning "CUT_AND_PASTE"
		info = as_SPIN_elem_info( v );
		if(NULL==info) return -1;
		Py_DECREF( self->eleminfo );
		self->eleminfo = info;
		Py_INCREF( self->eleminfo );
		return 0;
	}
	if(0==strcmp( name, "atts" ))
	{
		Py_DECREF( self->attslist );
		self->attslist = v;
		Py_INCREF( self->attslist );
		return 0;
	}
	if(0==strcmp( name, "data" ))
	{
		Py_DECREF( self->datalist );
		self->datalist = v;
		Py_INCREF( self->datalist );
		return 0;
	}
	return self->set_attr_tail( name, v );
}


/* --------------------------------------------------------------------- */
//	SPIN  attr_obj  Type -- 
/* --------------------------------------------------------------------- */

// static // warning due to previous declaration 
// which HAD to be external for g++ (but not gcc)
PyTypeObject SPIN_attr_obj_Type = {
 PyObject_HEAD_INIT(&PyType_Type)
	0,				/*ob_size*/
 (char *) "spin_attr_obj",		/*tp_name*/
	sizeof(	SPIN_attr_obj ),	/*tp_basicsize*/
	0,				/*tp_itemsize*/
 /* methods */
 (destructor)	spin_attr_obj_dealloc, /*tp_dealloc*/
 (printfunc)	spin_attr_obj_print,	/*tp_print*/
 (getattrfunc)	spin_attr_obj_getattr, /*tp_getattr*/
 (setattrfunc)	spin_attr_obj_setattr, /*tp_setattr*/
	0,				/*tp_compare*/
	0,				/*tp_repr*/
	0,				/*tp_as_number*/
 	0,				/*tp_sequence*/
	0,				/*tp_as_mapping*/
	0,				/*tp_hash*/
};


/* --------------------------------------------------------------------- */
//		SPIN	attr_obj	PRIVATE get isp_pre direct
/* --------------------------------------------------------------------- */

// EXPORTED
int spin_attr_obj_is_pre( PyObject *self )
{
	if(SPIN_attr_obj_Check(self))
		return spin_elem_info_is_pre( 
			(PyObject *)(((SPIN_attr_obj *)self) -> eleminfo)
		);

	PyErr_SetString(PyExc_TypeError,"SPIN_attr_obj expected");
	// vret(0); // report it
	return 0;
}

/* --------------------------------------------------------------------- */
//		SPIN	attr_obj	UNUSED
/* --------------------------------------------------------------------- */

/*
	obj.method( self, args ) (other than the standard ones)
*/

static PyObject *
spin_attr_obj_trip(
	SPIN_attr_obj * self,
	PyObject *args )
{
	if (!PyArg_ParseTuple(args, (char *)"")) {
		PyErr_SetString(PyExc_TypeError,"no parametrs expected");
		return NULL;
	}
	PyObject * ret = PyTuple_New( 3 );
	if(NULL==ret) return NULL;
	PyTuple_SetItem( ret, 0, (PyObject *)self->eleminfo );
	PyTuple_SetItem( ret, 1, self->attslist );
	PyTuple_SetItem( ret, 2, self->datalist );
	Py_INCREF( (PyObject *)self->eleminfo );
	Py_INCREF( self->attslist );
	Py_INCREF( self->datalist );
	return ret;
}

PyMethodDef spin_attr_obj_methods[] = {
	{(char *)"trip",	(PyCFunction)spin_attr_obj_trip,	1},
	{NULL,		NULL}		/* sentinel */
};

/* --------------------------------------------------------------------- */
//		SPIN	attr_obj	MODULE functions
/* --------------------------------------------------------------------- */


/*
	NEW attr_obj has to be careful, since the element is also
	registered in a dictionary. Any scheme is applicable, here is one.
*/
