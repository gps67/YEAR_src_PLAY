#ifndef enum_cls_H
#define enum_cls_H

struct enum_cls;
struct enum_val;

// #include "PY_Object_base.h"
#include "PY_weak_base.h"
#include "P_ref.h"
#include "str0.h"

extern PyTypeObject enum_cls_Type;;                                   
extern PyTypeObject enum_val_Type;;                                   

#define enum_cls_Check(v)    ((v)->ob_type == &enum_cls_Type)
#define enum_val_Check(v)    ((v)->ob_type == &enum_val_Type)

struct enum_cls : public PY_weak_base // PY_Object_base
{
	P_String	cls_name;
	P_ref_base	val_unset;
	P_List		val_list;

	enum_cls( const char * name );
	enum_val * add_val_name(
		int val,
		const char * str
	);
	enum_val * find_by_name( const char * str );
	enum_val * find_by_name( PyObject * str );
	enum_val * find_by_val( int v );
	enum_val * find_by_any( PyObject * str );

	enum_val * get_val_unset() { return (enum_val*) val_unset.ref; }
};

#endif
