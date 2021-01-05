#ifndef jsc_Stats_Module_H
#define jsc_Stats_Module_H

/*
	This was js_accts, cut+paste'd, without proper design.
	It calls some_main() from JS, not properly integrated.
	That itself may be a good idea to reduce the number of main()'s

	For make deps to be used, WITH_JS should really be defined
	IN THIS FILE, and all dependent users would get recompiled.
*/

#include "JS1.h"
#include "j_args.h"
#include "j_class_plus.h"
#include "obj_ref_js.h"

////////////////////////////////////////////////////////////

using namespace JSSM;

#include "dset.h"
#include "j_args.h"

////////////////////////////////////////////////////////////

/*
class no_priv : public obj_ref_js
{
 public:
	no_priv() {}
	~no_priv() {}
};
*/

////////////////////////////////////////////////////////////

/*!
	jsc_Stats_Module is a module

	Its just a stub to enable stats_sysinfo() to be called
	from a JS script (which saves a main but costs this)
*/
class jsc_Stats_Module : j_class_plus
{
 public:
	jsc_Stats_Module( JS1 * js1 );

	/*
		Initialise a new JS Stats_Module JSObject 
	*/
  virtual
	JSBool new_Thing( j_args_newfunc * args );

	static PROTO_JS_FUNC( f_sysinfo );
	static PROTO_JS_FUNC( f_partitions );
};

////////////////////////////////////////////////////////////

#endif

