#ifndef PY_Object_base_H
#define PY_Object_base_H

#include "PY_Object_plain.h"

#include "P_util.h"
#include "P_ref.h"

/*
	PY_Object_base is used as a base class for C++ objects.

	There is a mild overlap of when the object is constructed and
	deleted. To be 'proper' C++ does the construction, and
	uses PyObject_Init on pre-allocated memory (this)

	TODO: It also focuses on the DLL issue of which malloc() to use ...

	PROB: PyObject_NEW() is not the same as new(), ... RETHINK DETAILS
*/

struct PY_Object_base : public PY_Object_plain
{
	P_Dict var_pool;

	/*
		create a new object from a type spec (and this)
	*/
	PY_Object_base( PyTypeObject * py_type_object )
	: PY_Object_plain( py_type_object )
	{
	}

	/*!
		Python is excellent here - Py_XDECREF (when 0) calls
		the objects classes dealloc routine, which only needs
		to do a delete self;
	*/
	~PY_Object_base();

// //	These presume that this is not NULL! 

	/*
		PY_Static is too advanced to include at that low level,
		(circular headers, using basic PY types), so at this level
	*/

	bool Expect_Type( PyTypeObject * expected )
	{
		return PY_Static::Expect_Type( (PyObject *)this, expected );
	}


	PyObject * get_attr_tail( PyMethodDef * methods, const char * name );
	int set_attr_tail( const char * name, PyObject * v );

};




#endif

