#ifndef SPIN_attr_obj_H
#define SPIN_attr_obj_H

#include "SPIN_type_Check.h"
#include "elem_info.h"

// #define ELEM_OBJ_VAR_POOL

typedef SPIN_elem_info	SPIN_attr_info;

struct SPIN_attr_obj
{
	PyObject_HEAD
	SPIN_attr_info * eleminfo;
	PyObject * attslist;
	PyObject * datalist;
};

/* EXPORTED */

SPIN_attr_obj * new_SPIN_attr_obj(
	PyObject * eleminfo,
	PyObject * attslist,
	PyObject * datalist
);
int spin_elem_obj_is_pre( PyObject *self );
/*
	you could get the type from an inst
*/
PyObject * type_SPIN_attr_obj( void ); 


#endif
