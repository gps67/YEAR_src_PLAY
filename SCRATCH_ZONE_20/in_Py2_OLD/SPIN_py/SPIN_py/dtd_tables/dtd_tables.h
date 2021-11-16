#ifndef SPIN_dtd_tables_H
#define SPIN_dtd_tables_H

#include "app_types.h"
#include "SPIN_type_Check.h"
#include "PY_weak_base.h"
#include "P_weak_link.h"
#include "P_ref.h"

struct SPIN_elem_info;
struct SPIN_sdata;

struct SPIN_dtd_tables : public PY_weak_base
{
	P_Dict dict_elem_info;
	P_Dict dict_sdata;

	SPIN_dtd_tables()
	: PY_weak_base( & SPIN_dtd_tables_Type )
	, dict_elem_info() // PyDict_New
	, dict_sdata() // PyDict_New
	{
	}
	
	~SPIN_dtd_tables()
	{
	}

	SPIN_elem_info * find_elem_info( PyObject * name )
	{
		// no addef ref
		PyObject * info = dict_elem_info.GetItem( name );
		return (SPIN_elem_info *) info;
	}

	SPIN_elem_info * look_elem_info( PyObject * name )
	{
		// no addef ref
		PyObject * info = dict_elem_info.GetItem( name );
		return (SPIN_elem_info *) info;
	}

	SPIN_sdata * find_sdata( PyObject * name )
	{
		// no addef ref
		PyObject * info = dict_sdata.GetItem( name );
		return (SPIN_sdata *) info;
	}

	SPIN_sdata * look_sdata( PyObject * name )
	{
		// no addef ref
		PyObject * info = dict_sdata.GetItem( name );
		return (SPIN_sdata *) info;
	}

};

/* EXPORTED */

extern SPIN_dtd_tables * as_SPIN_dtd_tables( PyObject * elem );

SPIN_dtd_tables * new_SPIN_dtd_tables();


#endif
