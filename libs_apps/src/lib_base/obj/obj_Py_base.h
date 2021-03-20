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

/*
	static VTL_PyObject * CAST_FROM_PyObject( PyObject * P )
	test that P is a C++ object (not a plain python type)

	There's lots of ways, but want a V quick way 

	_typeobject

		Py_Type(P) == fixed_pointer
		 P -> ob_type

		 IDEA use NULL as fixed pointer (eg using own xxx)

	P -> ob_type -> tp_flags // unsigned long // bit X

		python uses upto bit 31
		python stupidly does not then use a second word for UDEF

		a good sharable scheme would be for C++ objects with VTBL
		then we could ask the object itself,
		and have a "standard" virtual question for which module
		or require all C++ to have common bas class

		a good sharable scheme would be to a single byte extn id
		then the opcode TEST == FIXED_VALUE // == 0 //
		that can be compiled into x86 OPCODE BYTE CMP idx==0x04
		that might need a DLL to be poked a lot
		but source compiled modules would be "easy" (#define FOUR 0x04)
		#define EXTN_CPP_MODULE_ID_ours 0x04

		if( P -> ob_type -> extn_module_id == EXTN_CPP_MODULE_ID_ours)
		{
			PY_CPP_BASE * obj = static_cast<VTL_PyObject *>(P);
			// that subtracted 16 from P to get obj
			// 16 is a guess, maybe test it
			// now we know obj is derived from our base class
			// we can use C++ type casts, or our own
		} else {
			// it's not a CPP object
			// or at least not one from our scheme
			// maybe it is a PyString or Tuple or list or callable
			// ...
		}


		object.h

		pycore_object.h
		_PyType_HasFeature


	suggest

		0 = UNSET but can treat as generic PyObject as normal
		1 = standard builtin as found in tgz
		2 = common extension as found in batteries included website
		3 = embedded user group scheme THREE
		4 = c++ extensions user group scheme FOUR
		5 = C++ extensions that have their own way
		6 = temporary SOLO no standard generally unused // DEFAULT SOLO
		7 = temporary SITE specific compiled only

		8-15 = reserved as unusable, so above fits into 3 bits
		16-255 = reserved for u5-u3 // u5 == { 0, 1, AVAILABLE }
		so then you have 30 extra tags, on top of u3, all in u8
		eg c++ extn four, then which module, maybe which dll slot


		get byte
		decr
		if -ve goto was zero // if carry 
		if zero goto was one

		decr
		if zero was two

		decr
		if zero goto was three

		decr
		if zero goto was four
		goto five or more or start again with a function not inline

		compare_0x03 & unsigned byte_with 0x02
		<= goto is standard // or zero // no need to test
		< goto is unset
		> goto or jsr decode further

		compare_unsigned byte_with 0x01
		== goto is standard
		< goto is unset
		> goto or jsr decode further


*/


#endif

