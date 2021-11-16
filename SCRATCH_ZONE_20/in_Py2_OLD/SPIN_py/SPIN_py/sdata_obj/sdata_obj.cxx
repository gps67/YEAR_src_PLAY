/* spin_sdata_obj objects */

#include "Python.h"
#include "sdata_obj.h"
#include "P_ref.h" // P_Tuple
#include "PY_weak_link.h" // 
#include "e_print.h" // 

SPIN_sdata::
SPIN_sdata(
	SPIN_dtd_tables * _sdata_pool,
	PyObject * _name,
	char _as_8859,
	PyObject * _as_utf8
)
: PY_Object_base( & SPIN_sdata_Type )
, sdata_pool_weak( _sdata_pool )
, name( _name )
, as_8859( _as_8859 )
, as_utf8( _as_utf8 )
{
}

SPIN_sdata::
~SPIN_sdata()
{
}


/*!
	get the dtd sdata pool from an existing object
*/
SPIN_dtd_tables * SPIN_sdata::get_sdata_pool()
{
	SPIN_dtd_tables * pool;
	return sdata_pool_weak.link_target();
}


extern PyMethodDef spin_sdata_obj_methods[]; // forward without problem!

static int
spin_sdata_obj_print (
        SPIN_sdata *op,
        FILE *fp,
        int flags )
{
	// LURK this is not re-loadable
        fprintf(fp, "(SDATA-");
	PyObject_Print(op->name, fp, 0);	// LURK null
        fprintf(fp, ")");
        return 0;
}


/* EXPORTED */
SPIN_sdata *
new_SPIN_sdata(
	SPIN_dtd_tables * _sdata_pool,
	PyObject * _name,
	PyObject * _as_8859,
	PyObject * _as_utf8
)
{
	/*
		check as_8859 is None or NULL or single char
	*/
	char chr_8859 = 0;
	SPIN_sdata * retval = new SPIN_sdata( _sdata_pool, _name, chr_8859, _as_utf8 );
	return retval;
}

/*
	return the type of a SPIN_sdata
	for when you don't have to to test
	and you can't be bother to fake creating one
*/
PyObject * type_SPIN_sdata( void )
{
	Py_INCREF( & SPIN_sdata_Type );
	return (PyObject *) & SPIN_sdata_Type;
}


static void
spin_sdata_obj_dealloc( SPIN_sdata *self )
{
	delete self;
}

/*
	get/set attr is called for vars and also for functions
	if the result is/nt data/code it is run (one route for all)
	Is there a compilation cache? That would be nice (or not)
*/

static PyObject *
spin_sdata_obj_getattr(
	SPIN_sdata *self,
	char *name )
{
	e_print("spin_sdata_obj_getattr(self,%s)\n", name );
	if(0==strcmp( name, "name" ))
	{
		return self->name.new_ref() ; 
	}
	if(0==strcmp( name, "as_utf8" ))
	{
		return self->as_utf8.new_ref() ; 
	}
	return self->get_attr_tail( spin_sdata_obj_methods, name );
}

#define NUL 0
static int
spin_sdata_obj_setattr(
	SPIN_sdata *self,
	char *name,
	PyObject *v )
{
	e_print("spin_sdata_obj_setattr(self,%s,v)\n", name );
	if(0==strcmp( name, "name" ))
	{
		if( self->name.set_str( v ) )
			return 0;
		else
			return -1;
	}
	if(0==strcmp( name, "as_8859" ))
	{
		self->as_8859 = NUL;
		if( v == Py_None ) return 0; // OK
		P_String s;
		if(!s.set_str( v )) return -1; // NOT A STRING reported
		if( !s ) return 0; // OK
		int l = s.len();
		if( l == 0 ) return 0; // OK
		if( l == 1 ) {
			self->as_8859 = NUL;
			return 0;
		}
		if( self->name.set_str( v ) )
			return 0;
		else
			return -1;
	}
	if(0==strcmp( name, "as_utf8" ))
	{
		if( self->as_utf8.set_str( v ) )
			return 0;
		else
			return -1;
	}
	return self->set_attr_tail( name, v );
}


/* --------------------------------------------------------------------- */
//	SPIN  sdata_obj  Type -- 
/* --------------------------------------------------------------------- */

// static // warning due to previous declaration 
// which HAD to be external for g++ (but not gcc)
PyTypeObject SPIN_sdata_Type = {
 PyObject_HEAD_INIT(&PyType_Type)
	0,				/*ob_size*/
 (char *) "spin_sdata_obj",		/*tp_name*/
	sizeof(	SPIN_sdata ),	/*tp_basicsize*/
	0,				/*tp_itemsize*/
 /* methods */
 (destructor)	spin_sdata_obj_dealloc, /*tp_dealloc*/
 (printfunc)	spin_sdata_obj_print,	/*tp_print*/
 (getattrfunc)	spin_sdata_obj_getattr, /*tp_getattr*/
 (setattrfunc)	spin_sdata_obj_setattr, /*tp_setattr*/
	0,				/*tp_compare*/
	0,				/*tp_repr*/
	0,				/*tp_as_number*/
 	0,				/*tp_sequence*/
	0,				/*tp_as_mapping*/
	0,				/*tp_hash*/
};


/* --------------------------------------------------------------------- */
//		SPIN	sdata_obj	MODULE functions
/* --------------------------------------------------------------------- */

PyMethodDef spin_sdata_obj_methods[] = {
	{NULL,		NULL}		/* sentinel */
};


/*
	NEW sdata_obj has to be careful, since the element is also
	registered in a dictionary. Any scheme is applicable, here is one.
*/
