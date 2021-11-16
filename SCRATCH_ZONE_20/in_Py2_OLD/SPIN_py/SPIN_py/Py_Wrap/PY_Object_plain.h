#ifndef PY_Object_plain_H
#define PY_Object_plain_H

struct PY_Object_plain;

#include "Python.h"
// #include "PY_Static.h" // circular include - excludes basic types

/*!
	PY_Object_plain is also for use when dealing with (PyObject*)
	from standard Python sources (ie not C++ objects).

	subclass PY_Object_base is for extension classes.

	By casting function prototypes, any received PyObject can be
	seen as a PY_Object_plain

	Several functions have been grafted on, for convenience.

	P_ref is for pointers to PY_Object_plain (it does refcounting,
	and a bit more work for standard objects like Int). It does
	more with NULL, whereas this is for the struct. The distinction
	is a bit obscure, and confusing. If you like think of both as
	parallel worlds, with only CAPitalisation to separate the calls.
*/
struct PY_Object_plain
{
 private:
	// prevent these from existing - detect their AUTO use
	PY_Object_plain( const PY_Object_plain & );
	PY_Object_plain & operator=( const PY_Object_plain & );
 public:
	PyObject_HEAD
	;

	/*!
		constructor - NOOP

		Real NEW objects will need initialise, which is done by
		CTOR: PY_Object_base( PyTypeObject * py_type_object );
		which also adds var_pool (most USED things need this,
		all get it).
	*/
	PY_Object_plain()
	{
	}

	/*!
		destructor - NOOP
	*/
	~PY_Object_plain()
	{
	}

	/*!
		PY_Object_plain can acyually be constructed
		(was sep to clarify)
	*/
	PY_Object_plain( PyTypeObject * py_type_object )
	{
		PyObject_Init( (PyObject *)this, py_type_object ); 
	}

	/*!
		return this cast as a PyObject * (pointless really)
	*/
	operator PyObject *() const
	{
		return (PyObject *)this;
	}

	/*!
		return this cast as a PyObject * (pointless really)
	*/
	PyObject * py_ref()
	{
		return (PyObject *)this;
	}

	/*!
		The Python type name as
	*/
	const char * type_name() const
	{
		return ob_type->tp_name;
	}

	/*!
		PY: ret = self.method()
	*/
	PY_Object_plain * call_method(
		const char * method
	);

	/*!
		PY: ret = self.method( arg1 )
	*/
	PY_Object_plain * call_method(
		const char * method,
		PY_Object_plain * arg1
	);

	/*!
		PY: ret = self.method( arg1, arg2 )
	*/
	PY_Object_plain * call_method(
		const char * method,
		PY_Object_plain * arg1,
		PY_Object_plain * arg2
	);

	/*!
		PY: ret = self.method( arg1, arg2, arg3 )
	*/
	PY_Object_plain * call_method(
		const char * method,
		PY_Object_plain * arg1,
		PY_Object_plain * arg2,
		PY_Object_plain * arg3
	);

	bool Is_Type( PyTypeObject * expected )
	{
		// not necessarily an error
		return expected == ob_type;
	}

	PyObject * ReportTypeMismatch( PyTypeObject * expected );

};

typedef PY_Object_plain PY_Object;
extern PY_Object_plain * shared_empty_list; // = 

#endif

