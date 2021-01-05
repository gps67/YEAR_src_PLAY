#include "j_confg.h"
/*
	10,000 lines in /home/gps/G/mozjs/mozjs-63.0.3/js/src/shell/js.cpp

	that gives multi threaded, webasm init, etc, etc

	that just shows how awful SM is, all the way through
*/
#include "JS1.h"
using namespace JSSM;

// from shell/js.cpp

static const JSClassOps global_classOps = {
	nullptr,
	nullptr,
	nullptr,
	
	nullptr, //global_enumerate,
	nullptr, //global_resolve,
	nullptr, //global_mayResolve,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	JS_GlobalObjectTraceHook
	// nullptr, //JS_GlobalObjectTraceHook
};

JSClass my_global_class = {
	"global",
	JSCLASS_GLOBAL_FLAGS,
	&global_classOps
};

bool JS1::mk_my_global()
{

	// cx // exists

	// if (!JS::InitRealmStandardClasses(cx))


	JS::RealmOptions options;
	// SetStandardRealmOptions(options);
	// 
	// options.creationOptions().setSharedMemoryAndAtomicsEnabled(enableSharedMemory);

	options.creationOptions().setNewCompartmentAndZone();

	JSPrincipals* principals = nullptr;



/*
	OK maybe I should not be calling JS_NewGlobalObject
	NewGlobalObject is defined in js.cpp
	but that is / is-not linked into the shared library

*/
//	globalObj = JS_NewGlobalObject( cx, &my_global_class );

	static		// keep track until program exit // move to class
	 RootedObject
	  glob(
	   cx,
	   JS_NewGlobalObject(
	    cx,
	    &my_global_class,
	    // or // cx->runtime()->trustedPrincipals();
	    principals,
            JS::DontFireOnNewGlobalHook,
	    options
	   )
	  );
	return true;
}

// }
