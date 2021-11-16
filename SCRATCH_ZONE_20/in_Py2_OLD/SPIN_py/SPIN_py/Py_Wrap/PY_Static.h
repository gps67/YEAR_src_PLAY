#ifndef PY_Static_H
#define PY_Static_H

#include "P_ref_base.h"                                                         
#include "SPIN_type_Check.h"                                                    

/*!
	This "class" holds static functions
*/
struct PY_Static
{

	static bool IS_Type( PyObject * obj, PyTypeObject * expected )
	{
		// not necessarily an error
		return obj && ( expected == obj->ob_type );
	}

	static bool Expect_Type( PyObject * obj, PyTypeObject * expected )
	{
		if( obj && ( expected == obj->ob_type )) return true;
		return Expect_Type_fn( obj, expected );
	}

	static bool Expect_Type_fn( PyObject * obj, PyTypeObject * expected );

	static PyObject * ReportTypeMismatch( PyObject * obj, PyTypeObject * expected );

	static PyObject * ErrorStr( PyObject * err, const char * msg );

	static PyObject * TypeError( const char * msg );

	static PyObject * AttributeError( const char * msg );

	static bool IS_String( PyObject * obj )
	{
		return obj && PyString_Check( obj );
	}

	static bool IS_Int( PyObject * obj )
	{
		return obj && PyInt_Check( obj );
	}

	static bool IS_Tuple( PyObject * obj )
	{
		return obj && PyTuple_Check( obj );
	}

	static bool IS_List( PyObject * obj )
	{
		return obj && PyList_Check( obj );
	}

	static bool IS_Dictionary( PyObject * obj )
	{
		return obj && PyDict_Check( obj );
	}

	static bool IS_Instance( PyObject * obj )
	{
		return obj && PyInstance_Check( obj );
	}

	static bool IS_elem_obj( PyObject * obj )
	{
		return obj && SPIN_elem_obj_Check( obj );
	}

	static bool IS_elem_info( PyObject * obj )
	{
		return obj && SPIN_elem_info_Check( obj );
	}

	static bool IS_sdata( PyObject * obj )
	{
		return obj && SPIN_sdata_Check( obj );
	}


	static bool Expect_String( PyObject * obj )
	{
		return Expect_Type( obj, & PyString_Type );
	}

	static bool Expect_Tuple( PyObject * obj )
	{
		return Expect_Type( obj, & PyTuple_Type );
	}

	static bool Expect_List( PyObject * obj )
	{
		return Expect_Type( obj, & PyList_Type );
	}

	static bool Expect_Dictionary( PyObject * obj )
	{
		return Expect_Type( obj, & PyDict_Type );
	}

	static bool Expect_Instance( PyObject * obj )
	{
		return Expect_Type( obj, & PyInstance_Type );
	}

	static bool Expect_elem_obj( PyObject * obj )
	{
		return Expect_Type( obj, & SPIN_elem_obj_Type );
	}

	static bool Expect_elem_info( PyObject * obj )
	{
		return Expect_Type( obj, & SPIN_elem_info_Type );
	}

};



struct PY_Static_must_have_zero_prefix : public PY_Static
{
	int anint;
};

// SYNTAX // #if sizeof( PY_Static_must_have_zero_prefix ) != sizeof( int )
// SYNTAX // #error "PY_Static_must_have_zero_prefix - but it doesnt"
// SYNTAX // #endif


#endif

