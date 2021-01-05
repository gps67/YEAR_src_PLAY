#include "j_confg.h"
#include "JS1.h"
using namespace JSSM;

// from shell/js.cpp

static const JSClassOps global_classOps = {
	nullptr,
	nullptr,
	nullptr,
	global_enumerate,
	global_resolve,
	global_mayResolve,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	JS_GlobalObjectTraceHook
};

JSClass my_global_class = {
	"global",
	JSCLASS_NEW_RESOLVE | JSCLASS_GLOBAL_FLAGS | JSCLASS_HAS_PRIVATE,
	JS_PropertyStub, 
	JS_PropertyStub,
	JS_PropertyStub, 
	JS_StrictPropertyStub,
JS_EnumerateStub,	// enumerate
//	global_enumerate,
JS_ResolveStub,		// resolve
//	(JSResolveOp) global_resolve,
	JS_ConvertStub, 
NULL_JS_FinalizeStub,	// finalise
//	its_finalize,
	JSCLASS_NO_OPTIONAL_MEMBERS
};

bool JS1::mk_my_global()
{
}

// }
