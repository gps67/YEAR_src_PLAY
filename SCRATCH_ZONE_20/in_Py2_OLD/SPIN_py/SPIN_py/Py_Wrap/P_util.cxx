#include "P_utils.h"

////////////////

P_util::P_util()
{
	// default constructors
}

P_util::~P_util()
{
	// default desstructors
}


/*
	Pythons daft system means that MANY return values (PyObject_CallMethod)
	are new values, which MUST be decremented, is thrown away.
	Hence 'retval'
*/
/*!
	The called function returned a value with incref

	retval snatched that value, so that it will be auto deleted is unused

	here we are using the value so incref and return it

	IT can be NULL (presuming the called function set the error message)
*/
PyObject * P_util:: return_value()
{
	return retval.new_ref();
}

bool P_util:: call_method(
	PyObject * obj,
	const char * method
)
{
	if(!obj) {
		return false;
	}
	retval.snatch_this( (PY_Object*) PyObject_CallMethod(
		obj,
		(char *) method,
		(char *) ""
	));
	return retval.not_null();
}

bool P_util:: call_method(
	PyObject * obj,
	const char * method,
	PyObject * arg1
)
{
	if(!obj) {
		return false;
	}
	retval.snatch_this( PyObject_CallMethod(
		obj,
		(char *) method,
		(char *) "(O)",
		arg1
	));
	return retval.not_null();
}

bool P_util:: call_method(
	PyObject * obj,
	const char * method,
	PyObject * arg1,
	PyObject * arg2
)
{
	if(!obj) {
		return false;
	}
	retval.snatch_this( PyObject_CallMethod(
		obj,
		(char *) method,
		(char *) "(OO)",
		arg1, arg2
	));
	return retval.not_null();
}

bool P_util:: call_method(
	PyObject * obj,
	const char * method,
	PyObject * arg1,
	PyObject * arg2,
	PyObject * arg3
)
{
	if(!obj) {
		return false;
	}
	retval.snatch_this( PyObject_CallMethod(
		obj,
		(char *) method,
		(char *) "(OOO)",
		arg1, arg2, arg3
	));
	return retval.not_null();
}

////////

#define P_base P_ref_base_no_ref

bool P_base::is_null_error_fn() const
{
	if( ref ) return false; // NOT an error
	PyErr_SetString(PyExc_AttributeError, "NULL");
	return true;
}

bool P_base::expect_TYPE_fn(  PyTypeObject * expected ) const
{
	if( is_TYPE( expected )) return true;
	PY_Static::ReportTypeMismatch( (PyObject*)ref, expected );
	return false;
}
