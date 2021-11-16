
#include "PY_Object_plain.h"

PY_Object_plain * PY_Object_plain::call_method(
	const char * method
)
{
	return (PY_Object_plain *) PyObject_CallMethod(
		py_ref(),
		(char *) method,
		(char *) ""
	);
}

PY_Object_plain * PY_Object_plain::call_method(
	const char * method,
	PY_Object_plain * arg1
)
{
	return (PY_Object_plain *) PyObject_CallMethod(
		py_ref(),
		(char *) method,
		(char *) "(O)",
		(PyObject*) arg1
	);
}

PY_Object_plain * PY_Object_plain::call_method(
	const char * method,
	PY_Object_plain * arg1,
	PY_Object_plain * arg2
)
{
	return (PY_Object_plain *) PyObject_CallMethod(
		py_ref(),
		(char *) method,
		(char *) "(OO)",
		(PyObject*) arg1,
		(PyObject*) arg2
	);
}

PY_Object_plain * PY_Object_plain::call_method(
	const char * method,
	PY_Object_plain * arg1,
	PY_Object_plain * arg2,
	PY_Object_plain * arg3
)
{
	return (PY_Object_plain *) PyObject_CallMethod(
		py_ref(),
		(char *) method,
		(char *) "(OOO)",
		(PyObject*) arg1,
		(PyObject*) arg2,
		(PyObject*) arg3
	);
}


// STATIC
PY_Object_plain *
shared_empty_list = (PY_Object_plain*) PyList_New(0);
