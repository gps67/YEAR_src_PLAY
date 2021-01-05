#ifndef j_class_base_H
#define j_class_base_H

#include "j_confg.h"
#include "e_print.h"
#include "JS1.h"
#include "jsapi.h"

#include "obj_ref_js.h"
#include "j_cx_obj.h"

namespace JSSM {

/*!
	j_class is the the C++ side of JSClass

	each subclass will setup their javascript objects differently,
	such as declaring well known properties with specific int-ids

	one idea is to allow derived classes to add JS fields as they wish
	The cookbook suggests a static list of fields

 OLD
	I have recently upgraded from JS-1.5 to JS-1.8
	and it doesnt have JSClass.spare, so ... kluded patch in place
 SO
 	mozjs-63 does have a ClientData - could use that
	Or could stay with this, save a deref
	because JS_Class belongs to calling application
	(often declared static in bss?) I can extend it 
	As usual type cast steps over VTBL prefix
	but can add extra fields 
-

	j_class_base provides JSClass, and defines how the
	JSOBJ.class.spare points to the C++ class, and how the
	JSOBJ.priv points to the C++ instance.

	You should use j_class_plus, which expects each JSObject to
	have a peer object and a C++ class defined for it. This is
	the base class which might be useful for 'plain' JS objects,
	but mostly does some admin-work to maintain the JS Class struct.

	Each class is of course an instance of the class class ;-)
*/
// class j_class_base : public JSClass // GRP_lib_base2_js
struct j_class_base : public JSClass // GRP_lib_base2_js
{
	// JSClass // base class // public
 public:

	//! JS1 holds the global context, runtime and globalObj
	JS1 * js1;

	//! all JS objects have a CTOR function with a prototype
	JSObject * prototype;

	// If its me, should it be a j_class_base * 
	//! so what is parent prototype, is it constructor object ?
	JSObject * parent_prototype;

	/*!
		Convert from SMJS's JSClass to my j_class_base

		There is some rule/assert that ensures we only handle
		JSClass which is derived, because we placed it,
		AND NO OTHER

		it steps back into that 8 byte VTBL ptr prefix
	*/
	static j_class_base * j_class_base_from_upcast( JSClass * base )
	{
		return static_cast<j_class_base *>( base );
	}

	/*!
		stepdown from C++ to SMJS's JSClass
		If we are us, this is guaranteed to work

		it steps over that 8 byte VTBL ptr prefix

		free(this) never free(that)
	*/
	JSClass * get_JSClass()
	{
		return static_cast<JSClass *>( this );
	}

	//! destructor - simple
	virtual ~j_class_base() {}

	/*!
		JS1 must be valid, with JS_Runtim JS_Context
	*/
	j_class_base(
		const char * class_name,
		JS1 * _js1
	);

	const char * class_name()
	{
		return name;
		// return my_class.name;
	}

	/*
		upgrade an inst to have a JSOB wrapper,
		this class has to be known, so does the inst
	*/
	JSObject * mk_js_self(
		obj_ref_js * inst
	);
	void set_inst(
		obj_ref_js * inst,
		JSObject * jsob
	);
	void split_cls_inst_jsob(
		obj_ref_js * inst,
		JSObject * jsob
	);

	bool ret_set_fn( JSContext *cx, JS::Value * vp, obj_ref_js * inst );
	bool ret_set_fn( JS::Value * retvar, obj_ref_js * inst );

	bool ret_set( JSContext *cx, JS::Value * vp, obj_ref_js * inst )
	{
		// no need for inline version
		return ret_set_fn( cx, vp, inst );
	}

	bool ret_set( JS::Value * retvar, obj_ref_js * inst )
	{
		if((!retvar) ||(!inst)) return ret_set_fn( retvar, inst );
		JSObject * jo = inst->js_self;
		if(!jo) jo = mk_js_self(inst);
		if(!jo) return ret_set_fn( retvar, inst );
		*retvar = OBJECT_TO_JSVAL( jo );
		return true;
	}


	/*!
		get_class_base() returns the C++ js_class_XXX of JSObject ob.

		If the JSObject isnt an extension class, return NULL.

		Check that myclass == spare (no other sub-system here
		uses spare, if you do, return NULL ?
	*/
  static
	j_class_base * get_class_base ( const j_cx_obj & cx_obj );

	JSContext * get_global_context() const
	{
		return js1->cx;
	}
/*
	a new object is associated with C++ data, which it owns.
	For this reason, the finalise method must ref_decr() it,
	AND remove the back-ref; Hence Finalising() virtual;
*/

//	JSObject * NewObject( JSContext * cx, obj_ref_js * priv );

	void init_JSClass();

	bool Def_Function(
		str0 name,
		JSNative function,
		int nargs
	);

	bool Def_Prop(
		const char * name,
		JSPropertyOp getter = NULL,
		JSStrictPropertyOp setter = NULL
	);

	bool Def_Prop(
		const char * name,
		int tinyint,
		JSPropertyOp getter = NULL,
		JSStrictPropertyOp setter = NULL
	);

//	static
//	void s_JS_Finalise( JSContext *cx, JSObject *obj );

//	virtual void finalising( JSContext * cx, JSObject * obj, obj_ref_js * priv );

};

}; // namespace JSSM

#endif
