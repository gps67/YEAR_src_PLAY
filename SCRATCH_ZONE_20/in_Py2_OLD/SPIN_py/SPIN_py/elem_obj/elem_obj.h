#ifndef SPIN_ELEM_OBJ_H
#define SPIN_ELEM_OBJ_H

#include "SPIN_type_Check.h"
#include "elem_info.h"
#include "PY_Object_base.h"
#include "P_ref.h"

struct SPIN_elem_obj : public PY_Object_base
{
	P_ref<SPIN_elem_info> eleminfo;
	P_List attslist;
	P_List datalist;

	SPIN_elem_obj();
	~SPIN_elem_obj();

	
	SPIN_dtd_tables * get_dtd_tables();
};

/* EXPORTED */

SPIN_elem_obj * new_SPIN_elem_obj(
	PyObject * eleminfo,
	PyObject * attslist,
	PyObject * datalist
);
int spin_elem_obj_is_pre( PyObject *self );
/*
	you could get the type from an inst
*/
PyObject * type_SPIN_elem_obj( void ); 


#endif
