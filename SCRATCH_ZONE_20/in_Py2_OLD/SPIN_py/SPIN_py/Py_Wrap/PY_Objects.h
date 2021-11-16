#ifndef PY_Objects_H
#define PY_Objects_H

#include "PY_Object_base.h"

#include "P_util.h"

	///////////////////////////////////////////////////////////////
// THIS WAS ref_hold.h, which only holds a reference count - no access to it!
/*
	PY_Object_base is used as a base class for C++ objects.

	There is a mild overlap of when the object is constructed and
	deleted. To be 'proper' C++ does the construction, and some
	Python code is copied to initialise the PyObject_HEAD.

	This makes this code potentially sensitive to changes in Python releases.

	TODO: It also focuses on the DLL issue of which malloc() to use ...

	PROB: PyObject_NEW() is not the same as new(), ... RETHINK DETAILS
*/

	///////////////////////////////////////////////////////////////
#define VRET_NULL( called ) if(vret(called)) return NULL;

	///////////////////////////////////////////////////////////////


#endif

