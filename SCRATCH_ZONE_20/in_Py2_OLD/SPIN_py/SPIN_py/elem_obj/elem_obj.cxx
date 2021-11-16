/* spin_elem_obj objects */

#include "Python.h"
#include "elem_obj.h"
#include "P_ref.h" // P_Tuple
#include "PY_weak_link.h" // 
#include "dtd_tables.h" //
#include "sdata_obj.h" //

SPIN_elem_obj::
SPIN_elem_obj()
: PY_Object_base( & SPIN_elem_obj_Type )
{
}

SPIN_elem_obj::
~SPIN_elem_obj()
{
}


/*!
	get the dtd elem_info pool from an existing object
*/
SPIN_dtd_tables * SPIN_elem_obj::get_dtd_tables()
{
	SPIN_dtd_tables * pool;
	return eleminfo.self() -> get_dtd_tables();
}


extern PyMethodDef spin_elem_obj_methods[]; // forward without problem!



static int
spin_elem_obj_print (
        SPIN_elem_obj *op,
        FILE *fp,
        int flags )
{
	// LURK this is not re-loadable
	// but at least it names the element
        fprintf(fp, "(ELEM-");
	PyObject_Print(op->eleminfo.self()->name, fp, 0);	// LURK null
        fprintf(fp, ")");
        return 0;
}


/* EXPORTED */
SPIN_elem_obj *
new_SPIN_elem_obj( AppObject info1, AppAttrList atts1, AppObject data1 )
{
	// e_print("new_SPIN_elem_obj {\n");

	SPIN_elem_info * info2;
	SPIN_elem_obj * self;

	info2 = as_SPIN_elem_info( info1 );
	if( !info2 ) return NULL;

	P_List atts2;
	P_List data2;

	if(!atts2.set_list( atts1 )) {
		e_print("atts2 fails\n");
		return NULL;
	}
	if(!data2.set_list( data1 )) {
		e_print("data2 fails\n");
		return NULL;
	}

	self = new SPIN_elem_obj();
	if (!self) return NULL;

	self->eleminfo.claim_this( info2 );
	if(!self->attslist.set_list( atts2 )) {
		e_print("attslist fail\n");
		return NULL;
	}
	if(!self->datalist.set_list( data2 )) {
		e_print("datalist fail\n");
		return NULL;
	}

	// e_print("new_SPIN_elem_obj OK\n");

	return self;
}

/*
	new is special, Python never calls it (this isn't a class, it's a
	type), and it needs to be accessed through some lookup, such as a
	module. That routine, (which as self=module) calls this new, but
	now with customised parameters.
*/
/*
	new_SPIN_elem_obj( PyObject *dict, PyObject *name, int pre )
	is special - it's the ONLY thing export from this module,
	and it registeres itself by simply existing (and holding its own
	TypeTable pointer)
	
*/
/*
	return the type of a SPIN_elem_obj
	for when you don't have to to test
	and you can't be bother to fake creating one
*/
PyObject * type_SPIN_elem_obj( void )
{
	Py_INCREF( & SPIN_elem_obj_Type );
	return (PyObject *) & SPIN_elem_obj_Type;
}


static void
spin_elem_obj_dealloc( SPIN_elem_obj *self )
{
	delete self;
}

/*
	get/set attr is called for vars and also for functions
	if the result is/nt data/code it is run (one route for all)
	Is there a compilation cache? That would be nice (or not)
*/

static PyObject *
spin_elem_obj_getattr(
	SPIN_elem_obj *self,
	char *name )
{
	if(0==strcmp( name, "info" ))
	{
		return self->eleminfo.new_ref() ; 
	}
	if(0==strcmp( name, "atts" ))
	{
		return self->attslist.new_ref() ; 
	}
	if(0==strcmp( name, "data" ))
	{
		return self->datalist.new_ref() ; 
	}
	return self->get_attr_tail( spin_elem_obj_methods, name );
}

static int
spin_elem_obj_setattr(
	SPIN_elem_obj *self,
	char *name,
	PyObject *v )
{
	if(0==strcmp( name, "info" ))
	{
		SPIN_elem_info * info;
		info = as_SPIN_elem_info( v );
		if(NULL==info) return -1;

		self->eleminfo.claim_this( info );
		return 0;
	}
	if(0==strcmp( name, "atts" ))
	{
		self->attslist.claim_this( v );
		return 0;
	}
	if(0==strcmp( name, "data" ))
	{
		self->datalist.claim_this( v );
		return 0;
	}
	return self->set_attr_tail( name, v );
}


// static // warning due to previous declaration 
// which HAD to be external for g++ (but not gcc)
PyTypeObject SPIN_elem_obj_Type = {
 PyObject_HEAD_INIT(&PyType_Type)
	0,				/*ob_size*/
 (char *) "spin_elem_obj",		/*tp_name*/
	sizeof(	SPIN_elem_obj ),	/*tp_basicsize*/
	0,				/*tp_itemsize*/
 /* methods */
 (destructor)	spin_elem_obj_dealloc, /*tp_dealloc*/
 (printfunc)	spin_elem_obj_print,	/*tp_print*/
 (getattrfunc)	spin_elem_obj_getattr, /*tp_getattr*/
 (setattrfunc)	spin_elem_obj_setattr, /*tp_setattr*/
	0,				/*tp_compare*/
	0,				/*tp_repr*/
	0,				/*tp_as_number*/
 	0,				/*tp_sequence*/
	0,				/*tp_as_mapping*/
	0,				/*tp_hash*/
};


// EXPORTED
int spin_elem_obj_is_pre( PyObject *self )
{
	if(SPIN_elem_obj_Check(self))
		return spin_elem_info_is_pre( 
			(PyObject *)(((SPIN_elem_obj *)self) -> eleminfo)
		);

	PyErr_SetString(PyExc_TypeError,"SPIN_elem_obj expected");
	// vret(0); // report it
	return 0;
}

/*
	obj.method( self, args ) (other than the standard ones)
*/

static PyObject *
spin_elem_obj_trip(
	SPIN_elem_obj * self,
	PyObject *args )
{
	if (!PyArg_ParseTuple(args, (char *)"")) {
		PyErr_SetString(PyExc_TypeError,"no parameters expected");
		return NULL;
	}
	P_Tuple ret( 3 );
	ret.set( 0, self->eleminfo );
	ret.set( 1, self->attslist );
	ret.set( 2, self->datalist );
	return ret.new_ref();
}

static PyObject *
spin_elem_obj_add_elem(
	SPIN_elem_obj * self,
	PyObject *args_ )
{
	PyObject * name = NULL;
	SPIN_elem_info * info = NULL;
	AppAttrList atts = NULL;
	AppObject data = NULL;
	/*
		add_elem( info|name )
		add_elem( info|name, atts, data )
	*/
	P_Tuple args;
	args.claim_this( args_ ); // caller owns reference, must increment
	switch( args.N() )
	{
	 case 3:
		data = args.get(2);
		// continue next case
	 case 2:
		atts = args.get(1);
		// continue next case
	 case 1:
	 {
		// name must be a string - not an elem_info !
		name = args.get(0);
		
		// get the pool where the elem_info will be found/created
		SPIN_dtd_tables * pool = self -> get_dtd_tables();
		if(!pool)
		{
			return NULL;
		}

		// lookup / add (which is why pool needs top.EH)
		info = pool -> find_elem_info( name );
		if( !info )
		{
			PyErr_SetString(PyExc_TypeError,"add_elem(name) -- name not in pool -- info is NULL");
			return NULL;
		}
	 }
	 break;
	 case 0:
		PyErr_SetString(PyExc_TypeError,"no parameters provided");
		return NULL;
	 default:
		PyErr_SetString(PyExc_TypeError,"too many parameters provided");
		return NULL;
	}

	PyObject* elem = (PyObject*) new_SPIN_elem_obj( (PyObject*) info, atts, data );

	self->datalist.Append( elem );

	PyObject * ret = elem;
	Py_INCREF( ret );
	return ret;
}

static PyObject *
spin_elem_obj_add(
	SPIN_elem_obj * self,
	PyObject *args )
{
	PyObject * text = NULL;
	if (!PyArg_ParseTuple(args, (char *)"O", &text )) {
		PyErr_SetString(PyExc_TypeError,"string parameter expected");
		return NULL;
	}
	/*
		TODO: check top mode 8859 convert ?
	*/
	self->datalist.Append( text );

	Py_INCREF( Py_None );
	return Py_None;
}

static PyObject *
spin_elem_obj_add_text(
	SPIN_elem_obj * self,
	PyObject *args )
{
	PyObject * text = NULL;
	if (!PyArg_ParseTuple(args, (char *)"S", &text )) {
		PyErr_SetString(PyExc_TypeError,"string parameter expected");
		return NULL;
	}
	/*
		TODO: check top mode 8859 convert ?
	*/
	self->datalist.Append( text );

	Py_INCREF( Py_None );
	return Py_None;
}

static PyObject *
spin_elem_obj_add_attr(
	SPIN_elem_obj * self,
	PyObject *args )
{
	PyObject * name = NULL;
	PyObject * value = NULL;
	if (!PyArg_ParseTuple(args, (char *)"SS", &name, &value )) {
		PyErr_SetString(PyExc_TypeError,"string parameters expected");
		return NULL;
	}
	P_Tuple attr( 2 );
	attr.set( 0, name );
	attr.set( 1, value );
	self->attslist.Append( attr );

	return attr.new_ref();
}

#define NUL 0

static PyObject *
spin_elem_obj_add_sdata(
	SPIN_elem_obj * self,
	PyObject *args )
{

	/*
		NB this is not calling EH.sdata( name, text ) - refactor time?

		ACTUALLY THERE IS NOTHING TO CALL!

		so need a Python object to hold sdata, and also
		a mechanism to resolve KNOWN ones (DTD load)
	*/
	PyObject * name = NULL;
	if (!PyArg_ParseTuple(args, (char *)"S", &name )) {
		PyErr_SetString(PyExc_TypeError,"string parameter expected");
		return NULL;
	}

	SPIN_sdata * sdata = new_SPIN_sdata(
		self->get_dtd_tables(),
		name,
		NUL,
		NULL
	);

// #warning "sdata should be added to the elem/info/pool.dict_sdata

	self->datalist.Append( sdata );
	PyObject * sdata_ = (PyObject *) sdata;

	return sdata_;
}

PyMethodDef spin_elem_obj_methods[] = {
	{(char *)"trip",	(PyCFunction)spin_elem_obj_trip,	1},
	{(char *)"add",		(PyCFunction)spin_elem_obj_add, 	1},
	{(char *)"add_elem",	(PyCFunction)spin_elem_obj_add_elem,	1},
	{(char *)"add_text",	(PyCFunction)spin_elem_obj_add_text,	1},
	{(char *)"add_attr",	(PyCFunction)spin_elem_obj_add_attr,	1},
	{(char *)"add_sdata",	(PyCFunction)spin_elem_obj_add_sdata,	1},
	{NULL,		NULL}		/* sentinel */
};


/*
	NEW elem_obj has to be careful, since the element is also
	registered in a dictionary. Any scheme is applicable, here is one.
*/
