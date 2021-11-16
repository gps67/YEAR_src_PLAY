#ifndef PY_weak_base_H
#define PY_weak_base_H

#include "PY_Object_base.h"
#include "PY_weak_link.h"

extern PyTypeObject PY_weak_link_Type;

/*!
	A real object, which expects uplinks to be point to it.

	It creates, but doesnt delete the PY_weak_link (to this).
	Anyone that expects a link to this should use link = this->weak,
	and the INCREF/DECREF it as usual.

	To convert that weak link back to real link use link.item;

	If an object has children with weak uplinks, this is
	the real object, PY_weak_link is the stepping stone.

	This controls the weak_step, and keeps it alive with
	a ref_count.

	The many chidren, add normal ref_counts to the step,
	but nothing to this main objects ref-count.
*/
struct PY_weak_base : public PY_Object_base
{
	PY_weak_link * sole_link;

	PY_weak_base(
		PyTypeObject * py_type_object
	)
	: PY_Object_base( py_type_object )
	, sole_link( 0 )
	{
		sole_link = new PY_weak_link( this );
		Py_INCREF( sole_link );
	}

	PY_weak_link * get_weak_link()
	{
		return sole_link;
		// caller should INCREF if kept
	}

	~PY_weak_base()
	{
		sole_link -> declare_base_item_has_gone();
		Py_XDECREF( sole_link );
	}
};


#endif
