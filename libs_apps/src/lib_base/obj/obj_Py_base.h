#ifndef obj_Py_base_H
#define obj_Py_base_H

#define makefile_decides_WITH_PYTHON 1		// base obj_ref on PyObject
#define USE_PYTHON_H		// or use stubs for speed // untyedef ?

#ifdef WITH_PYTHON		// mixin PyObject bas to obj_ref0

#ifdef USE_PYTHON_H
/*
	You are supposed to use python.h,
	but obj_ref is the base of everything,
	and it would be good to have a STUB base definition
	so that objects start with two words
	but the entire headers dont need parsing

	Worse, I _CANT_ yet make it work without using the
	proper headers, so its a bit slow for now, but when
	I have the api  I'll have another go
 */
// some bug where <string.h> and <Python.h> both define it
#ifdef _POSIX_C_SOURCE
#undef _POSIX_C_SOURCE
#endif
// #warning INCLUDING <Python.h> 
#include <Python.h> // which includes "object.h" so avoid that name ...
#else
/*
	SIMULATE include <python.h>
	BUT all Py debugging is switched OFF
	AND object is not VAR head - single struct only!
	OK try some 
*/
        struct _typeobject;
        typedef int Py_ssize_t; // pyport.h uses ...

#define PyObject_HEAD \
	Py_ssize_t ob_refcnt; \
	_typeobject *ob_type;
//

	typedef struct _object
	{
		PyObject_HEAD
	} PyObject;


#endif
	/*!
		NO VTBL but a few helper features

		maybe Python uses a stub for PyObject - so cant derice from it
 	*/

	struct PyObject_basic : public PyObject
	{
		// PyObject_HEAD

		PyObject_basic( _typeobject * _ob_type )
		{
			ob_refcnt = 0;
			ob_type = _ob_type;
		}

		// JOB LOT of utility functions to go here
		// is_string, is_VTBL, is ....
	};

	/*
		C++ stupidly prepends the VTBL before the plain struct

		the alternative was to put it mid way inside the struct,
		which has almost zero cost in a modern CPU.
		That would have allowed the pointer to never change.

		C-Python object function s cant skip over the VTBL,
		they must be given the minged pointer to the (second) field
		When Python provides a PyObject *, we munge it back

		How can we tell that the object is castable?
		SUGGEST - put a flag in the _typeobject somewhere
		SUGGEST - limiting use in C functions found in _typeobject
		SUGGEST - leak proof get-what-was-placed
	*/
	class VTL_PyObject : public PyObject_basic
	{
	 public:
		static _typeobject ob_type_VTL_PyObject;
		virtual ~VTL_PyObject() {}

		//! CTOR sets the ob_type, and C++ sets the VTBL
		VTL_PyObject( _typeobject * _ob_type = & ob_type_VTL_PyObject )
		: PyObject_basic( & ob_type_VTL_PyObject )
		{
			// OK
			/*
				as a general theme,
				C++ builds objects in layers
				Py jumps right to last layer
				C++ splices hidden VTBL
				Py passes extra _ob_type parameter
				(here we default it)

				The paradigm shift to always pass the _ob_type
				is pervasive, but interesting,

				I also want my own VTBL-FROM-INDEX
				which would have 2-dim keys.

				See also POST (although it uses default special)
			*/
		}

		PyObject * as_PyObject()
		{
			return static_cast<PyObject *>(this);
		}

		static VTL_PyObject * CAST_FROM_PyObject( PyObject * P )
		{
			// add test on type spec
			return static_cast<VTL_PyObject *>(P);
		}

	};
#else
	// ok
#endif // WITH_PYTHON



#endif

