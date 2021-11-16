#include "PY_Static.h"

// STATIC
PyObject * PY_Static:: ErrorStr( PyObject * err, const char * msg )
{
	PyErr_SetString( err, msg );
	return NULL;
}

// STATIC
PyObject * PY_Static:: TypeError( const char * msg )
{
	return ErrorStr( PyExc_TypeError, msg );
}

// STATIC
PyObject * PY_Static:: AttributeError( const char * msg )
{
	return ErrorStr( PyExc_AttributeError, msg );
}

// STATIC
PyObject * PY_Static:: ReportTypeMismatch(
	PyObject * obj,
	PyTypeObject * expected
)
{
	const char * type_name = "(NULL)";

	if(obj) {
		type_name = obj->ob_type->tp_name;
	}
	PyErr_Format(PyExc_TypeError,
		"Expected: %.400s Got: %.400s",
		expected->tp_name,
		type_name
	);
	return NULL;
}

// STATIC
bool PY_Static:: Expect_Type_fn( PyObject * obj, PyTypeObject * expected )
{

	if( obj && ( expected == obj->ob_type )) return true;
	ReportTypeMismatch( obj, expected );
	return false;
}


