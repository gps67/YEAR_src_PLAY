/* spin_string objects */

#include "Python.h"
#include "str_obj.h"
#include "SPIN_type_Check.h"

#define STRING_TRACERS_off

/*

	currently much the same as a Python 8859 string
	in future ...
	
*/


/* --------------------------------------------------------------------- */
//		SPIN	string	TYPE
/* --------------------------------------------------------------------- */


static PyMethodDef spin_string_methods[]; // forward without problem!

#define SPIN_string_Check(v)        ((v)->ob_type == &SPIN_string_Type)

// exported
// want static Type obj for speed
// allow func for occasional use
int is_SPIN_string( PyObject * obj )
{
	return SPIN_string_Check(obj);
}

static int
spin_string_print (
        SPIN_string *op,
        FILE *fp,
        int flags )
{
#ifdef STRING_TRACERS
        fprintf(fp, "(print-");
#endif
	int l=op->len; 
	char * s = op->str;
	while( l > 0 ) {
		fputc( *s++, fp );
		l--;
	}
#ifdef STRING_TRACERS
        fprintf(fp, ")");
#endif
        return 0;
}

PyObject *
spin_string_repr ( SPIN_string *self )
{
#ifdef STRING_TRACERS
	// as used in cast OBJ -> string (not print)
	PyObject * L = PyString_FromString("REPR(");
	PyObject * R = PyString_FromString("):");
#else
	PyObject * L = PyString_FromString("");
	PyObject * R = PyString_FromString("");
#endif
	PyString_ConcatAndDel( & L, PyString_FromStringAndSize(
		self->str,
		self->len
	));
	PyString_ConcatAndDel( & L, R );
	return L;
}


/* --------------------------------------------------------------------- */
//		SPIN	string	new (name, is_pre)
/* --------------------------------------------------------------------- */

/* EXPORTED */

SPIN_string *
new_SPIN_string( int flags, int sz, char * str );

SPIN_string *
new_SPIN_string( PyObject *string1 )
{
	if(!PyString_Check( string1 )) {
		PyErr_SetString(PyExc_TypeError,"String expected");
		return NULL;
	}
	int sz = PyString_GET_SIZE(string1);
	char * str = PyString_AS_STRING( string1 );
	int flags = SPIN_string_flags_8859;
	return new_SPIN_string( flags, sz, str );
}

// #define ADDNULL_dont
#define ADDNULL
SPIN_string *
new_SPIN_string( int flags, int sz, char * str )
{
#ifdef ADDNULL
	sz++;
#endif
	char * s = (char *)Py_Malloc( sz );
#ifdef ADDNULL
	sz--;
#endif
	if(NULL==s) {
		PyErr_SetString(PyExc_TypeError,"malloc failed");
		return NULL;
	}
	SPIN_string * self = PyObject_NEW( SPIN_string, &SPIN_string_Type );
	if(NULL==self) {
		Py_Free( s );
		PyErr_SetString(PyExc_TypeError,"malloc failed");
		return NULL;
	}
	memcpy( s, str, sz );
#ifdef ADDNULL
	s[sz] = 0;
#endif
	self->flags = flags;
	self->len = sz;
	self->str = s;
	self->flags = 0;
	
	return self;
}

/*
	new is special, Python never calls it (this isn't a class, it's a
	type), and it needs to be accessed through some lookup, such as a
	module. That routine, (which as self=module) calls this new, but
	now with customised parameters.
*/
/*
	new_SPIN_string( PyObject *dict, PyObject *name, int pre )
	is special - it's the ONLY thing export from this module,
	and it registeres itself by simply existing (and holding its own
	TypeTable pointer)
	
*/
/*
	return the type of a SPIN_string
	for when you don't have to to test
	and you can't be bother to fake creating one
*/
PyObject * type_SPIN_string( void )
{
	Py_INCREF( & SPIN_string_Type );
	return (PyObject *) & SPIN_string_Type;
}


long spin_string_hash(SPIN_string * str)
{
	int h = 0xa5a5a5a5;
	char * s = str->str;
	int l = str->len;
	const unsigned factor = (1 + 32);
	while( l-- > 0 ) {
		char c = *s++;
		h += c;
		h *= factor;
	}
	return h;
}



/* --------------------------------------------------------------------- */
//		SPIN	string	dealloc, get/set attrs
/* --------------------------------------------------------------------- */

static void
spin_string_dealloc( SPIN_string *self )
{
	if(self->str) Py_Free(self->str);
	PyMem_DEL(self);
}

/*
	get/set attr is called for vars and also for functions
	if the result is/nt data/code it is run (one route for all)
	Is there a compilation cache? That would be nice (or not)
*/

static PyObject *
spin_string_getattr(
	SPIN_string *self,
	char *name )
{
	if(0==strcmp( name, "byte_len" ))
	{
		return PyInt_FromLong( self->len );
	}
	if(0==strcmp( name, "flags" ))
	{
		return PyInt_FromLong( self->flags );
	}
	if(0==strcmp( name, "data_string" ))
	{
		// write(1, self->str, self->len );
		return PyString_FromStringAndSize(
			self->str,
			self->len
		);
	}
	return Py_FindMethod(spin_string_methods, (PyObject *)self, name);
}

static int
spin_string_setattr(
	SPIN_string *self,
	char *name,
	PyObject *v )
{
        if(0==strcmp( name, "flags" )) {
		if(!PyInt_Check( v )) {
			return -1;
		}
		self->flags = (int) PyInt_AsLong( v );
	}

	return -1;
}


/* --------------------------------------------------------------------- */
//	SPIN  string  Type -- 
/* --------------------------------------------------------------------- */

// static // warning due to previous declaration 
// which HAD to be external for g++ (but not gcc)
PyTypeObject SPIN_string_Type = {
 PyObject_HEAD_INIT(&PyType_Type)
	0,				/*ob_size*/
 (char *) "spin_string",		/*tp_name*/
	sizeof(	SPIN_string ),	/*tp_basicsize*/
	0,				/*tp_itemsize*/
 /* methods */
 (destructor)	spin_string_dealloc, /*tp_dealloc*/
// 	0,	/*tp_print*/
 (printfunc)	spin_string_print,	/*tp_print*/
 (getattrfunc)	spin_string_getattr, /*tp_getattr*/
 (setattrfunc)	spin_string_setattr, /*tp_setattr*/
	0,				/*tp_compare*/
//	0,				/*tp_repr*/
 (reprfunc)	spin_string_repr,	/*tp_repr */
	0,				/*tp_as_number*/
 	0,				/*tp_sequence*/
	0,				/*tp_as_mapping*/
  (hashfunc)	spin_string_hash,	/*tp_hash */
//	0,				/*tp_hash*/
/* 
        ternaryfunc tp_call;
        reprfunc tp_str;
        getattrofunc tp_getattro;
        setattrofunc tp_setattro;


        // Flags to define presence of optional/expanded features 
        long tp_flags;

        char *tp_doc; // Documentation string 

        long tp_xxx5;
        long tp_xxx6;
        long tp_xxx7;
        long tp_xxx8;


        int tp_alloc;
        int tp_free;
        int tp_maxalloc;
        struct _typeobject *tp_next;
*/
};



static PyMethodDef spin_string_methods[] = {
	{NULL,		NULL}		/* sentinel */
};

/* --------------------------------------------------------------------- */
//		SPIN	string	MODULE functions
/* --------------------------------------------------------------------- */


/*
	NEW string has to be careful, since the element is also
	registered in a dictionary. Any scheme is applicable, here is one.
*/
