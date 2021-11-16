#ifndef SPIN_TOP_H
#define SPIN_TOP_H

/*
	You load the spin_py module and create a SPIN_top.
	(simple model - single thread - etc)
	Then you configure the kit (OutputComments etc)
	Then you Run (EH,files)
	EH (a plain py class) receives the events

	SPIN_top holds the various bits in one place,
	the kit is also a "standalone" PyObject (reused code)
	the eventGen is kept so that we can call halt ()
	the SP_App is kept so that you can set its options
	(switch wrap on/off, access its stack, etc)
	It probably has a pointer to this - for convenience

	This allows the app to hold a pointer to the C objects,
	VIA this interface. (deleting code for find_SP_App).

	It also holds the elem_info dictionary (which has to be somewhere).

*/

#include "app_types.h"
#include "dtd_tables.h"
// #include "SP_App.h"
class SP_App;
#include "sp_peg_kit.h"

#include "PY_Object_base.h"

struct SPIN_top : public PY_Object_base
{

	SPIN_top()
	: PY_Object_base( & SPIN_top_Type )
	{
	}

	P_ref<SP_peg_kit> kit;
	P_ref<SPIN_dtd_tables> dtd_tables; // More than PyDict * PyObject * 
	P_ref_base EH;
	// PyObject * EH;  // plain EventHandler

	// SP types - NOT PyObject based (but do use things)
	EventGenerator * eg;
	SP_App * app;
};

/* EXPORTED */

PyObject * new_SPIN_top( PyObject *, PyObject * args );


#endif
