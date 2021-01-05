#ifndef JSSM_STUBS_H
#define JSSM_STUBS_H

// struct GRP_lib_base2_js  : public GRP_lib_base2 {}; // ints.h
#include "ints.h"


// DEBUG
#include "j_confg.h"	

namespace js {
};
// using namespace js;

namespace JS {
	union Value;
	// 
};
// using namespace JS;

namespace JS_SM {
};
// using namespace JS_SM;

// NOT in namespace
// NB gcc said /(INSTALLED)/mozjs-67/js/TypeDecls.h:28:8: note: candidates are: struct JSContext
// but did not say what the other fully resolved namespace was
// when not in a namespace ? or that is OK nested definition ? 

	struct JSContext;
	class JSString;
	class JSObject;


#endif

