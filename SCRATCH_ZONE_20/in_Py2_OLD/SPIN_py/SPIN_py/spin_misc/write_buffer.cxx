/* spin_write_buffer object */

#include "spin_py_mod.h"
#include "Python.h"
#include "write_buffer.h"
#include "str_obj.h"

#include "buffer/misc.h"
#include "buffer/blk1.h"
#include "buffer/buffer1.h"

#include "SPIN_type_Check.h"
 

/*
	spin_write_buffer is a buffer to reduce the number of syscalls.

 Python usage:
	buff = Buffer( downstream, [lo], [hi] ) # create buffer
	buff.write( str ) # add str to buffer
	buff.flush()	  # calls downstream,write() and free() if..
	buff.flush2()	# buff.flush() downstream.flush2()

	Also need a derived class (type) which keeps line info
	such as indentation, line_is_empty, even wrapping text
	see (TBA) spin_line_buffer
*/

extern PyMethodDef spin_write_buffer_methods[]; // forward without problem!


extern SPIN_write_buffer *
as_SPIN_write_buffer( PyObject * buff )
{
	/*
		Check that buff is really a write_buffer, else error
	*/
	if (buff && SPIN_write_buffer_Check( buff )) {
		return (SPIN_write_buffer *) buff;
	} else {
		PyErr_SetString(PyExc_TypeError,"SPIN_write_buffer expected");
		return (SPIN_write_buffer *) NULL;
	}
}

/* EXPORTED */
SPIN_write_buffer * new_SPIN_write_buffer(	// create or edit an write_buffer 
	PyObject *downstream_,
	int lo_,
	int hi_
) {
	if( !downstream_ ) {
		PyErr_SetString(PyExc_TypeError,"new_SPIN_write_buffer: downstream is NULL");
		return NULL;
	}
	// create the basic object
	SPIN_write_buffer *self;
	self = new SPIN_write_buffer();
	// self->buff.raw_init(); // didnt use C++ new, used malloc
	self->down_stream.claim_this( downstream_ );
	self->lo = lo_;
	self->hi = hi_;
	self->buff.resize( lo_ );
	return self;
}

/*
	new_SPIN_write_buffer( ... ) is the only thing exprted from here,
	once the object exists, it can find its hidden static functions
*/

/* --------------------------------------------------------------------- */

static void
spin_write_buffer_dealloc( SPIN_write_buffer *self )
{
	delete( self );
}

static PyObject *
spin_write_buffer_getattr(
	SPIN_write_buffer *self,
	char *name )
{
	if(0==strcmp( name, "downstream" ))
	{
		return self->down_stream.new_ref() ; 
	}
	if(0==strcmp( name, "lo" ))
	{
		return PyInt_FromLong( self->lo );
	}
	if(0==strcmp( name, "hi" ))
	{
		return PyInt_FromLong( self->hi );
	}
	return self->get_attr_tail( spin_write_buffer_methods, name );
}

static int
spin_write_buffer_setattr(
	SPIN_write_buffer *self,
	char *name,
	PyObject *v )
{
	if(0==strcmp( name, "downstream" ))
	{
		self->down_stream.claim_this( v );
		return 0;
	}
	if(0==strcmp( name, "hi" ))
	{
		int val = PyInt_AsLong( v ); // no error detected
		self->hi = val;
		return 0;
	}
	if(0==strcmp( name, "lo" ))
	{
		int val = PyInt_AsLong( v ); // no error detected
		self->lo = val;
		return 0;
	}
	return self->set_attr_tail( name, v );
}
/* --------------------------------------------------------------------- */
//	SPIN  write_buffer  Type -- 
/* --------------------------------------------------------------------- */

// static // warning due to previous declaration 
// which HAD to be external for g++ (but not gcc)

PyTypeObject SPIN_write_buffer_Type = {

	 PyObject_HEAD_INIT(&PyType_Type)
		0,				/*ob_size*/
	 (char *) "spin_write_buffer",		/*tp_name*/
		sizeof(	SPIN_write_buffer ),	/*tp_basicsize*/
		0,				/*tp_itemsize*/
	 /* methods */
	 (destructor)	spin_write_buffer_dealloc, /*tp_dealloc*/
	 (printfunc)	0,	/*tp_print*/
	 (getattrfunc)	spin_write_buffer_getattr, /*tp_getattr*/
	 (setattrfunc)	spin_write_buffer_setattr, /*tp_setattr*/
		0,				/*tp_compare*/
		0,				/*tp_repr*/
		0,				/*tp_as_number*/
		0,				/*tp_as_sequence*/
		0,				/*tp_as_mapping*/
		0,				/*tp_hash*/
};


/* --------------------------------------------------------------------- */


static PyObject *
spin_write_buffer_write(
	SPIN_write_buffer * self,
	PyObject *args )
{
	char * str; // binary block though!
	int len;
	if (!PyArg_ParseTuple(args, (char *)"s#:spin_write_buffer_write",
		&str, &len ))
	{
		// 	"Python should gen own error");
		return NULL;
	}
	/* 
		// buffer can still exceed hi, esp single request, but OK
	*/
	if(( self->buff.nbytes_used + len ) >= self->hi )  {
		// call self.flush()
		if( ! PyObject_CallMethod(
			(PyObject *)self,
			(char *) "flush",
			(char *) "()" ) )
		{
			return NULL;
		}
	}

	self->buff.put_nn_bytes( len, str );
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *
spin_write_buffer_flush(
	SPIN_write_buffer * self,
	PyObject *args )
{
	char * str; // binary block though!
	int len;
	if (!PyArg_ParseTuple(args, (char *)":spin_write_buffer_flush"))
		return NULL;
	/* 
		// buffer can still exceed hi, esp single request, but OK
		if(( self->buff.nbytes_used + len ) > self.hi ) flush() ;
	*/
	len = self->buff.nbytes_used;
	if( ! len ) { // OK
		Py_INCREF(Py_None);
		return Py_None;
	}

	// create the String for the next (Python) device
	PyObject * string;
	string = PyString_FromStringAndSize(
	(STR0)	self->buff.buff,
		self->buff.nbytes_used);
	if(!string) {
		return NULL;
	}
	// clear the buffer, reclaim some space ?
	self->buff.nbytes_used = 0; //leave putc_prev
	self->buff.resize( self->lo );

	if( ! PyObject_CallMethod(
		self->down_stream,
		(char *) "write",
		(char *) "(O)",
		(PyObject *) string ) )
	{
		return NULL;
	}
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *
spin_write_buffer_flush2(
	SPIN_write_buffer * self,
	PyObject *args )
{
	/*
		flush2 = self.flush() and downstream.flush2() ...

		self.flush()
		try:
			self.downstream.flush2()
			return
		except:
			# noone else uses flush2, try flush, even then OK
			try:
				self.downstream.flush()
			except:
				pass
	*/
	if (!PyArg_ParseTuple(args, (char *)":spin_write_buffer_flush2"))
		return NULL;

	// call self.flush()
	if( ! PyObject_CallMethod(
		(PyObject *)self,
		(char *) "flush",
		(char *) "()" ) )
	{
		return NULL;
	}

	// call downstream.flush2()
	if( ! PyObject_CallMethod(
		self->down_stream,
		(char *) "flush2",
		(char *) "" ) )
	{
		// FAILED: ok if attribute error "flush2()", try flush()
		PyErr_Clear();
		// call downstream.flush()
		if( ! PyObject_CallMethod(
			self->down_stream,
			(char *) "flush",
			(char *) "" ) )
		{
			// ok if attribute error "flush()", pass
			PyErr_Clear();
		}
	}

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef spin_write_buffer_methods[] = {
	{(char *)"write",	(PyCFunction)spin_write_buffer_write,	1},
	{(char *)"flush",	(PyCFunction)spin_write_buffer_flush,	1},
	{(char *)"flush2",	(PyCFunction)spin_write_buffer_flush2,	1},
	{NULL,		NULL}		/* sentinel */
};
