#include "PY_Object_base.h"

static int alloc_counter = 0;

/*!
	C++ constructor.

	The memory was allocated by (new) released by delete.

	~PY_Object_plain() cannot do counting,
	because it is mostly used for actual python structs
	(BUT THEN AGAIN: new + delete should never get called
	for genuinely Python objs, only for derived classes
	that call it last (before delete at top level)

PY_Object_base::PY_Object_base( PyTypeObject * py_type_object )
: PY_Object_plain( py_type_object )
{
	// _plain // PyObject_Init( (PyObject *)this, py_type_object );
	//
	// Python New returns 1 - so do we, or is it already done ??
	// Py_INCREF( this );

	if(0)
	{
		alloc_counter++;
		printf( "new %s() count = %d \n", type_name(), alloc_counter );
	}
}
*/

/*!
	C++ destructor - does nothing
*/
PY_Object_base::~PY_Object_base()
{
	if(0)
	{
		alloc_counter--;
		printf( "del %s() count = %d \n", type_name(), alloc_counter );
		fflush(0);
	}
}

//	return self->get_attr_tail( spin_write_buffer_methods, name );
//	return self->set_attr_tail( name, v );

/*!
	subclasses implementing get_attr() do their own thing then call this

	get_attr() returns a new ref, or NULL with error set.

	This is tail code that does the vanilla action (gets methods),
	with the addition of the var_pool feature that all extension
	object have (in this library).

	Notice how call method finds method through this function.
	They can be overridden with set_attr.

*/
PyObject * PY_Object_base::get_attr_tail(
	PyMethodDef * methods,
	const char * name
)
{
	/*
		direct reference to var_pool can return None
	*/
	if(0==strcmp( name, "var_pool" ))
        {
                return var_pool.new_ref() ; // NULL becomes None
        }

	/*
		although var_pool is a very attractive feature,
		when it is is used (non-null), it delays method
		lookups into two lookups. If you copy the most
		used method into var_pool, it would be ok.
	*/
	if( var_pool.not_null() )
	{
		PyObject * ret = var_pool.GetItemString( name );
		if( ret )
		{
			Py_INCREF( ret );
			return ret;
		}
	}

	/*
		Python uses this simple but slightly inefficient
		method lookup, EVERY TIME.
	*/
	if( methods )
		return Py_FindMethod( methods, (PyObject *)this, (char *)name );
	else
	{
		PyErr_SetString(PyExc_AttributeError,name);
                return NULL;
	}
}

/*!
	subclasses implementing set_attr() try their own thing then call this

	It adds support for var_pool (creating it on first use).

	If v is null, delete it (standard Python API)
*/
int
PY_Object_base::set_attr_tail(
        const char * name,
        PyObject *v
)
{
        // printf("SPIN.setattr(%s)\n",name);
        if (var_pool.is_null() )
        {
                if( !v )
		{
			// delete item from absent pool ?
			// complain here and return -1
			return 0; // not an error
		}
                if(! var_pool.create_new() ) return -1;
        }
        if (v == NULL) {
                if( !var_pool.DelItemString( name ))
                        PyErr_SetString(PyExc_AttributeError,
                                "delete non-existing attribute" );
                return 0;
        }
        else
                if( var_pool.SetItemString( name, v ) )
                        return 0;
                else
		{
			PyErr_SetString(PyExc_AttributeError,name);
                        return -1;
		}
}

