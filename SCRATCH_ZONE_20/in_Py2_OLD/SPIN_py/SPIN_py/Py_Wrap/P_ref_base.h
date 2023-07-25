#ifndef P_ref_base_H
#define P_ref_base_H

#include "PY_Object_plain.h"

/*
	P_ref_base_no_ref does the POINTER stuff - but not ref counting

	P_ref_base adds ref counting

	P_ref<TYPE> adds type casting and specific (Int) functions

	P_String, P_List, P_Dict,  are all P_ref, they are Python Pointers.
	PY_Object is the struct, (often) used as a base class.

	P_ref_base_no_ref (and P_SomeType) is a pointer to a PyObject of SomeType.
	The pointer might be NULL.
	The pointer always adds one reference count.
*/

class P_ref_base_no_ref
{
 public:
	// this is the ONLY field
	PY_Object_plain * ref;
 private:
	// these dont exist
	P_ref_base_no_ref( const P_ref_base_no_ref & );
	P_ref_base_no_ref & operator=( const P_ref_base_no_ref & );

 public:
 	P_ref_base_no_ref()
	{
		ref = 0;
	}

#if 0
	//
	// these are defined here but cancelled by subclass
	// the point being - should there be a ref_count or not
	//
 	P_ref_base_no_ref( PY_Object_plain * _ref )
	{
		ref = _ref;
	}

 	P_ref_base_no_ref( PyObject * _ref )
	{
		ref = (PY_Object_plain *) _ref;
	}
#endif

	operator PyObject * () const
	{
		return (PyObject*) ref;
	}

	operator bool() const
	{
		return ref;
	}

	int ref_count() const
	{
		if(!ref) return 0;
		return ref->ob_refcnt;
	}

	bool not_null() const { return 0!=ref; }
	bool is_null() const { return 0==ref; }
	bool is_shared() const { return ref_count()>1; }

	bool is_null_error_fn() const ;

	bool is_null_error() const
	{
		if( ref ) return false; // NOT an error
		return is_null_error_fn();
	}

	bool is_None() const
	{
		return (PyObject*) ref == Py_None;
	}

	bool is_SUBTYPE( PyTypeObject * expected ) const
	{
		if( !ref ) return false;
		if(  ref->ob_type == expected ) return true;
		return is_SUBTYPE_fn( expected );
        }

	bool is_SUBTYPE_fn( PyTypeObject * expected ) const
	{
		// move this to compiled
		if( !ref ) return false;
		if(  ref->ob_type == expected ) return true;
		return PyType_IsSubtype( ref->ob_type, expected );
        }


	bool is_TYPE( PyTypeObject * expected ) const
	{
		if( !ref ) return false;
		return ref->ob_type == expected ;
	}

	bool expect_TYPE(  PyTypeObject * expected ) const
        {
		if( is_TYPE( expected )) return true;
		return expect_TYPE_fn(  expected );
	}

	bool expect_TYPE_fn(  PyTypeObject * expected ) const;

	bool is_Buffer()	{ return is_TYPE( &PyBuffer_Type	); }
	bool is_CFunction()	{ return is_TYPE( &PyCFunction_Type	); }
	bool is_CObject()	{ return is_TYPE( &PyCObject_Type	); }
	bool is_CallIter()	{ return is_TYPE( &PyCallIter_Type	); }
	bool is_Cell()		{ return is_TYPE( &PyCell_Type		); }
	bool is_Class()		{ return is_TYPE( &PyClass_Type	); }
	bool is_Complex()	{ return is_TYPE( &PyComplex_Type	); }
	bool is_Dict()		{ return is_TYPE( &PyDict_Type		); }
	bool is_File()		{ return is_TYPE( &PyFile_Type		); }
	bool is_Float()		{ return is_TYPE( &PyFloat_Type	); }
	bool is_Function()	{ return is_TYPE( &PyFunction_Type	); }
	bool is_Int()		{ return is_TYPE( &PyInt_Type		); }
	bool is_List()		{ return is_TYPE( &PyList_Type		); }
	bool is_Long()		{ return is_TYPE( &PyLong_Type		); }
	bool is_Module()	{ return is_TYPE( &PyModule_Type	); }
	bool is_Range()		{ return is_TYPE( &PyRange_Type	); }
	bool is_SeqIter()	{ return is_TYPE( &PySeqIter_Type	); }
	bool is_Slice()		{ return is_TYPE( &PySlice_Type	); }
	bool is_String()	{ return is_TYPE( &PyString_Type	); }
	bool is_TraceBack()	{ return is_TYPE( &PyTraceBack_Type	); }
	bool is_Tuple()		{ return is_TYPE( &PyTuple_Type	); }
	bool is_Type()		{ return is_TYPE( &PyType_Type		); }
	bool is_Unicode()	{ return is_TYPE( &PyUnicode_Type	); }

//	bool is_Code()		{ is_TYPE( &PyCode_Type		); }
//	bool is_CursesWindow()	{ is_TYPE( &PyCursesWindow_Type	); }
//	bool is_Frame()		{ is_TYPE( &PyFrame_Type	); }
//	bool is_SymtableEntry()	{ is_TYPE( &PySymtableEntry_Type	); }

//	PyBaseObject_Type
//	PySuper_Type

	/*!
		NULL is the error indicator.
		
		The error indicator (string) should be set
	*/
	PyObject * new_ref_or_NULL()
	{
		if( !ref )
		{
			return NULL;
		} else {
			Py_INCREF( ref );
			return (PyObject *)ref;
		}
	}
	
	/*!
		NULL is being used as a None not an error
	*/
	PyObject * new_ref_or_None()
	{
		if( !ref )
		{
			Py_INCREF( Py_None );
			return Py_None;
		} else {
			Py_INCREF( ref );
			return (PyObject *)ref;
		}
	}
	
	PyObject * new_ref()
	{
		return new_ref_or_None();
	}
	
	PyObject * py_ptr() const
	{
		return (PyObject*) ref;
	}
	
	PyObject * x_py_ptr_plus()
	{
		Py_XINCREF( ref );
		return (PyObject*) ref;
	}
	
	const char * type_name() const
	{
		if(!ref) return NULL;
		return ref->type_name();
	}

 	void forget()
	{
		ref = 0;
	}

 	void release ()
	{
		Py_XDECREF( ref );
		ref = 0;
	}

 	void claim_this( PY_Object_plain * obj ) //
	{
		Py_XINCREF( obj ); // do first in-case same obj
		release();
		ref = obj;
	}

 	void snatch_this( PY_Object_plain * obj ) // I was already holding it
	{
		release();
		ref = obj;
	}

	/*!
		some P_List types allow NULL as a valid value
		Most use NULL instead
	*/
	void set_ptr_None()
	{
		claim_this( Py_None );
	}

	/*!
		some P_List types allow NULL as a valid value
	*/
	void set_ptr_NULL()
	{
		release();
	}

// upgrade to _plain

 	void snatch_this( PyObject * obj )
	{
		snatch_this( (PY_Object_plain *) obj );
	}
 	void claim_this( PyObject * obj )
	{
		claim_this( (PY_Object_plain *) obj );
	}

/*
	include/pyerrors.h:

extern DL_IMPORT(PyObject *) PyExc_Exception;                                   
extern DL_IMPORT(PyObject *) PyExc_StopIteration;                               
extern DL_IMPORT(PyObject *) PyExc_StandardError;                               
extern DL_IMPORT(PyObject *) PyExc_ArithmeticError;                             
extern DL_IMPORT(PyObject *) PyExc_LookupError;                                 
                                                                                
extern DL_IMPORT(PyObject *) PyExc_AssertionError;                              
extern DL_IMPORT(PyObject *) PyExc_AttributeError;                              
extern DL_IMPORT(PyObject *) PyExc_EOFError;                                    
extern DL_IMPORT(PyObject *) PyExc_FloatingPointError;                          
extern DL_IMPORT(PyObject *) PyExc_EnvironmentError;                            
extern DL_IMPORT(PyObject *) PyExc_IOError;                                     
extern DL_IMPORT(PyObject *) PyExc_OSError;                                     
extern DL_IMPORT(PyObject *) PyExc_ImportError;                                 
extern DL_IMPORT(PyObject *) PyExc_IndexError;                                  
extern DL_IMPORT(PyObject *) PyExc_KeyError;                                    
extern DL_IMPORT(PyObject *) PyExc_KeyboardInterrupt;                           
extern DL_IMPORT(PyObject *) PyExc_MemoryError;                                 
extern DL_IMPORT(PyObject *) PyExc_NameError;                                   
extern DL_IMPORT(PyObject *) PyExc_OverflowError;                               
extern DL_IMPORT(PyObject *) PyExc_RuntimeError;                                
extern DL_IMPORT(PyObject *) PyExc_NotImplementedError;                         
extern DL_IMPORT(PyObject *) PyExc_SyntaxError;                                 
extern DL_IMPORT(PyObject *) PyExc_IndentationError;                            
extern DL_IMPORT(PyObject *) PyExc_TabError;                                    
extern DL_IMPORT(PyObject *) PyExc_ReferenceError;                              
extern DL_IMPORT(PyObject *) PyExc_SystemError;                                 
extern DL_IMPORT(PyObject *) PyExc_SystemExit;                                  
extern DL_IMPORT(PyObject *) PyExc_TypeError;                                   
extern DL_IMPORT(PyObject *) PyExc_UnboundLocalError;                           
extern DL_IMPORT(PyObject *) PyExc_UnicodeError;                                
extern DL_IMPORT(PyObject *) PyExc_ValueError;                                  
extern DL_IMPORT(PyObject *) PyExc_ZeroDivisionError;                           
#ifdef MS_WINDOWS                                                               
extern DL_IMPORT(PyObject *) PyExc_WindowsError;                                
#endif                                                                          
                                                                                
extern DL_IMPORT(PyObject *) PyExc_MemoryErrorInst;                             

*/

};

/*!
	P_ref_base is like P_ref_base_no_ref, but it always
	carries a ref count (and releases it at end).

	There is no copy constructor, so you set a value with claim()

	P_ref<type> is the intended subclass - it DOES have initialisers
*/
class P_ref_base : public P_ref_base_no_ref
{
 private:
	// these dont exist
	P_ref_base( const P_ref_base & );
 	P_ref_base( PY_Object_plain * _ref );
	P_ref_base & operator=( const P_ref_base & );
 public:
 	P_ref_base()
 	: P_ref_base_no_ref()
	{
	}
 	~P_ref_base()
	{
		release();
	}
};

	///////////////////////////////////////////////////////////////
//	P_ref is type cast to P_TYPE
	///////////////////////////////////////////////////////////////
// PyTypeObject 
template <class P_TYPE>
class P_ref : public P_ref_base
{
 public:

	P_TYPE * ptr() const
	{
		return (P_TYPE *) ref;
	}
	P_TYPE * self() const
	{
		return (P_TYPE *) ref;
	}
	P_TYPE * self_plus()
	{
		Py_XINCREF( ref );
		return (P_TYPE *) ref;
	}
	operator P_TYPE * () const
	{
		return (P_TYPE *) ref;
	}

	void set_ptr_claim(P_TYPE * p)
	{
		claim_this( ( PY_Object_plain * ) p );
	}
	void set_ptr_snatch(P_TYPE * p)
	{
		snatch_this( ( PY_Object_plain * ) p );
	}
	// NOTE args uses PyObject not P_TYPE (PyThorn)
	void set_ptr_ok_claim(PyObject * p)
	{
		claim_this( ( PY_Object_plain * ) p );
	}
	void set_ptr_ok_snatch(PyObject * p)
	{
		snatch_this( ( PY_Object_plain * ) p );
	}


};

#endif

