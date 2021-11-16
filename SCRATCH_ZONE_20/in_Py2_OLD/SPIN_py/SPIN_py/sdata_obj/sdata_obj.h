#ifndef SPIN_sdata_H
#define SPIN_sdata_H

#include "SPIN_type_Check.h"
#include "elem_info.h"
#include "PY_Object_base.h"
#include "P_ref.h"
#include "dtd_tables.h"


/*!
	AMP lt ;

	There should be a POOL dictionary of named sdata objects
	Each object has
		name	"lt"	&lt;
		as_ascii '<'
		as_8859	'<'	NUL means not
		as_utf8 "<"	STRING in current interpretation!

	For now its enough to have a holder for &nbsp; in a elem_obj's list
*/
struct SPIN_sdata : public PY_Object_base
{
	P_weak_link<SPIN_dtd_tables> sdata_pool_weak;

	P_String	name;
	char		as_8859;	// NULL if not == a single char
	P_String	as_utf8;	// might be an alternate repr
	
	SPIN_sdata(
		SPIN_dtd_tables * _sdata_pool,
		PyObject * _name,
		char _as_8859,
		PyObject * _as_utf8
	);
	~SPIN_sdata();

	
	SPIN_dtd_tables * get_sdata_pool();
};

/* EXPORTED */

SPIN_sdata * new_SPIN_sdata(
	SPIN_dtd_tables * _sdata_pool,
	PyObject * _name,
	PyObject * _as_8859,
	PyObject * _as_utf8
);
/*
	you could get the type from an inst
*/
PyObject * type_SPIN_sdata( void ); 


#endif
