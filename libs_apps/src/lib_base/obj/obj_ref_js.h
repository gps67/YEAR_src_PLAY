#ifndef obj_ref_js_H
#define obj_ref_js_H

// make deps can use this
#include "DEP_WITH_JS.h"
#include "obj_ref.h"

struct JSObject;
//#include "jsapi.h"

/*
	This defines what a C++ class needs to know to be involved with JS,
	without actually defining anything.

	If WITH_JS is not defined, these definitions disappear

	The C++ object.js_self can be upgraded BY a j_class_plus,
	which creates a JSObject which points back.
*/

struct JSObject;

namespace JSSM {
	class j_class_base;
	class j_args_newfunc;
	class j_args_delfunc;
	class j_cx_obj_ret;		// fast use of js_self
}

#ifdef WITH_JS

	/*!
		An OBJ can exist without a JSOBJ peer, or with one.

		?HOW does JS GC know when ref is kept/dropped ?

		JSOBJ can only be created by the JS_CLASS, so this object
		cannot create its own peer (no CTXT).

		See j_class_base

		see CLASS->mk_js_self( INST ) // CLASS has CTXT
	*/
	class obj_ref_js : public obj_ref0
	{
		// access 
		friend class j_class_base;
		friend class JSSM::j_class_base;
		friend class JSSM::j_args_newfunc;
		friend class JSSM::j_args_delfunc;
		friend class JSSM::j_cx_obj_ret;

		// this is the CACHED js_obj, set by js CLASS
		JSObject * js_self;
	 public:
		JSObject * get_js_self() { return js_self; }

		obj_ref_js()
		{
			js_self = NULL;
		}
		~obj_ref_js()
		{
			js_self = NULL;
		}
	};

#else // NOT WITH_JS

	class obj_ref_js : public obj_ref0
	{
	 public:
		obj_ref_js() : obj_ref0() { }
		~obj_ref_js() { }
		// virtual void mk_js_obj( JS1 * js1 ){}
	};

#endif // +- WITH_JS


#endif
