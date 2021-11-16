#ifndef P_ref_H
#define P_ref_H

#include "P_ref_base.h"
#include "PY_Static.h"
// #include "P_util.h"


class P_Int : public P_ref<PyIntObject>
{
 public:
	P_Int( int i=0 )
	{
		set_int( i );
	}
	P_Int( PyIntObject * p )
	{
		set_ptr_claim(p);
	}
	P_Int( PyObject * p )
	{
		if( PyInt_Check( p ))
			set_ptr_ok_claim(p);
		else {
			PyErr_SetString(PyExc_TypeError, "INT expected");
			set_int(0);
		}
	}
	int get_int() const
	{
		if(is_null_error()) return 0;
		return ptr()->ob_ival;
	}
	void set_int( int ival)
	{
		if(is_null() || is_shared())
			set_ptr_snatch( (PyIntObject *) PyInt_FromLong( ival ));
		else
			ptr()->ob_ival = ival;
	}
};

class P_Tuple : public P_ref<PyTupleObject>
{
 public:
	P_Tuple( int n = 0 )
	{
		set_ptr_ok_snatch( PyTuple_New( n ) );
	}
	P_Tuple( PyObject * p ) {
		set_tuple( p );
	}
	bool set_tuple( PyObject * p ) {
		if( !p || ( p == Py_None ))
		{
			set_ptr_NULL();
			return true;
		}
		if( PyTuple_Check( p ))
		{
			set_ptr_ok_claim(p);
			return true;
		} else {
			PY_Static::ReportTypeMismatch( p, & PyTuple_Type );
			return false;
		}
	}
	int N() const
	{
		if( is_null() ) return 0;
		return PyTuple_GET_SIZE(ptr());
	}
	bool set( int pos, PyObject * p )
	{
		// NOTE different INCREF policy
		if(!p) return false;
		if(is_null_error()) return false;
		// RequiresIncref: PyTuple_SetItem()
		// PRESUMABLY the old reference also needs a DECREF ?
		Py_INCREF( p );
		if(0!=PyTuple_SetItem( py_ptr(), pos, p ))
		{
			return false;
			Py_DECREF( p );
		}
		return true;
	}
	PyObject * get( int pos ) const
	{
		if(is_null_error()) return NULL;
		return PyTuple_GetItem( py_ptr(), pos );
	}

};

class P_List : public P_ref<PyListObject>
{
 public:
	P_List( int n = 0 )
	{
		if( n )
			set_ptr_ok_snatch( PyList_New( n ) );
		else
			ref = NULL;
	}
	P_List( PyObject * p ) // default constructor, then ...
	{
		set_list( p );
	}
	bool set_list( P_List & L )
	{
		/*
			L is already NULL or validated
			so propagate that safely (blindly)
		*/
		claim_this( L.ref );
		return true;
	}
	bool set_list( PyObject * p )
	{
		if( p == Py_None ) p = NULL;
		if(!p) {
			// a P_List can be NULL - but it becomes
			// or it can STAY NULL
			// p = (PyObject*) shared_empty_list;
			set_ptr_ok_claim( p );
			return true;
		}
		if(!PY_Static::Expect_List( p )) {
			return false;
		}
		set_ptr_ok_claim( p );
		return true;
	}
	int N() const
	{
		if( is_null() ) return 0;
		return PyList_GET_SIZE(ptr());
	}
	bool set( int pos, PyObject * p )
	{
		/*
			NULL items may not be placed in arrays
			(could convert Py_None - but dont loosen spec)
		*/
		if(!p) {
			PY_Static:: TypeError( "P_List.set(pos,NULL)" );
			return false;
		}
		/*
			it isnt an error for P_List to be null
			except when accessing item[pos]
		*/
		if(is_null_error()) return false;
		/*
			SetItem is DAFT - it needs INCREF
			(default for new value)
		*/
		Py_INCREF( p );
		if(0!=PyList_SetItem( py_ptr(), pos, p ))
		{
			return false;
			/*
				SetItem needs INCREF but caters for fail
				It also DECREF's the old item (the DOCS
				are ambiguous there)
				Py_DECREF( p );
			*/
		}
		return true;
	}
	PyObject * get( int pos ) const
	{
		if(is_null_error()) return NULL;
		return PyList_GetItem( py_ptr(), pos );
	}

	bool Append( PyObject * v )
	{
		if(!ref) {
			 set_ptr_ok_snatch( PyList_New(0) );
		} else if( ref == shared_empty_list ) {
			 set_ptr_ok_snatch( PyList_New(0) );
		}
		return 0==PyList_Append( py_ptr(), v );
	}

	bool Append( PY_Object_plain * v )
	{
		return Append( (PyObject*) v );
	}

	bool Append( const P_ref_base & p )
	{
		return Append( (PyObject*) p.ref );
		// OK // return 0==PyList_Append( py_ptr(),  (PyObject*) p.ref );
	}

};

class P_Dict : public P_ref<PyDictObject>
{
 public:
	P_Dict()
	{
		create_new();
	}
	P_Dict( PyObject * p ) {
		if(p) set_dict( p );
	}
	bool create_new()
	{
		set_ptr_ok_snatch( PyDict_New() );
		return not_null();
	}
	bool set_non_null()
	{
		if( is_null() ) return create_new();
		return true;
	}
	void set_null()
	{
		set_ptr_ok_snatch(NULL);
	}
	bool set_dict( PyObject * p )
	{
		if( PY_Static::Expect_Dictionary(p) )
		{
			set_ptr_ok_claim(p);
			return true;
		} else {
			return false;
		}
	}

	PyObject * GetItemString_new_ref( const char * str )
	{
		PyObject * ret = GetItemString( str );
		Py_XINCREF( ret );
		return ret;
	}

	PyObject * GetItem( PyObject * key )
	{
		return PyDict_GetItem( py_ptr(), key );
	}

	bool SetItem( PyObject * key, PyObject * val )
	{
		return 0==PyDict_SetItem( py_ptr(), key, val );
	}

	bool DelItem( PyObject * key )
	{
		return 0==PyDict_DelItem( py_ptr(), key );
	}

	PyObject * GetItemString( const char * str )
	{
		return PyDict_GetItemString( py_ptr(), (char *) str );
	}

	bool SetItemString( const char * str, PyObject * val )
	{
		return 0==PyDict_SetItemString( py_ptr(), (char *) str, val );
	}

	bool DelItemString( const char * str )
	{
		return 0==PyDict_DelItemString( py_ptr(), (char *) str );
	}

	// bool append( PyObject * p )
	// {
	// 	return false;
	// }

};

/*!
	A P_String is more of a value (whereas P_Dict is a living space)
*/
class P_String : public P_ref<PyStringObject>
{
 public:
	P_String()
	{
		ref = NULL;
	}
	P_String( const char * str )
	{
		ref = NULL;
		set_str( str );
	}
	P_String( const PyObject * str )
	{
		ref = NULL;
		set_str( str );
	}
#if 0
	void operator=( const P_String & s2 )
	{
		claim_this( py_ptr() );
	}
#endif

	const char * get_str() const
	{
		if( is_null() ) return "";
		return PyString_AsString( py_ptr() );
	}
	bool set_str( const PyObject * str )
	{
		if(!str) {
			// PyErr_SetString(PyExc_TypeError, "STRING expected got NULL");
			claim_this( (PyObject *)str ); // COPY ??
			return false;
		}
		if(!PyString_Check( str ))
		{
			// LURK - better to report type mismatch
			// expected STRING got OTHER
			PyErr_SetString(PyExc_TypeError, "STRING expected");
			return false;
		}
		claim_this( (PyObject *)str ); // COPY ??
		return ref;
	}
	bool set_str( const char * str )
	{
		snatch_this( PyString_FromString( str ) );
		return ref;
	}
	void set_null()
	{
		set_ptr_ok_snatch(NULL);
	}
	bool operator == ( const char * str ) const
	{
		return 0 == strcmp( get_str(), str );
	}

	int len() const
	{
		const char * s = get_str();
		if(!s) return 0;
		return strlen( s );
	}
};


#endif
