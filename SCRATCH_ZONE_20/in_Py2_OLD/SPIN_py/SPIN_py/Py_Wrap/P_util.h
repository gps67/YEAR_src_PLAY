#ifndef P_util_H
#define P_util_H

#include "P_ref_base.h"
#include "SPIN_type_Check.h"
#include "PY_Static.h"


struct P_util : public PY_Static
{
	P_ref_base retval;
/*
	P_ref_base x1;
	P_ref_base x2;
	P_ref_base x3;
	P_ref_base x4;
*/

	P_util();
	~P_util();

	PyObject * return_value();

	bool call_method(
		PyObject * obj,
		const char * method
	);
	bool call_method(
		PyObject * obj,
		const char * method,
		PyObject * arg1
	);
	bool call_method(
		PyObject * obj,
		const char * method,
		PyObject * arg1,
		PyObject * arg2
	);
	bool call_method(
		PyObject * obj,
		const char * method,
		PyObject * arg1,
		PyObject * arg2,
		PyObject * arg3
	);

};


// typedef P_ref_base refhold;
class refhold : public P_ref_base
{
 public:
	refhold()
	: P_ref_base()
	{
	}

	refhold( PyObject * P )
	: P_ref_base()
	{
		snatch_this( P );
	}
};

#include "SPIN_type_Check.h"
#endif
