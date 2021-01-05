#ifndef j_args_H
#define j_args_H

// https://bugzilla.mozilla.org/show_bug.cgi?id=673556
// UINT32_MAX
// #define __STDC_LIMIT_MACROS 
// #include <stdint.h>
// // #define UINT32_MAX 0xffffffff 

#include "j_confg.h"
#include "j_cx_obj.h"
#include "j_class_base.h"
#include "jsc_prop_list.h"

#include "str2.h"
#include "dgb_fail.h"

// #include "jsinterp.h"
// #include "jsprvtd.h"
// #include "jspubtd.h"
// #include "jsstdint.h"


namespace JSSM {

	class j_class_base;
	class j_class_plus;

/*!
	j_cx_obj_inst is almost a _peer (with a class_plus)
	but it might be a plain int/string/array, ie an arg to a func
*/
struct j_cx_obj_inst : j_cx_obj
{
	//! the private data if available (assuming its obj_ref_js)
	obj_ref_js * inst;

	//! the peer class, NULL means its a plain thing
	j_class_plus * cls;

	~j_cx_obj_inst(){}

	j_cx_obj_inst(
		JSContext *	_cx,
		JSObject * 	_js_obj
	)
	: j_cx_obj( _cx, _js_obj )
	, cls( NULL )
	{
		// the C++ instance of a JS class is the ClientData
		if(!get_class_plus())
		{
			// not a problem
		}
		inst = (obj_ref_js *) GetPrivate();
	}

	/*!
		cached fetch of the peer class, or NULL if int/str/etc
	*/
	j_class_plus * get_class_plus()
	{
		if( cls ) return cls;
		cls = j_cx_obj::get_class_plus();
		return cls;
	}

	bool full_sanity_check()
	{
		if(!get_js1())
		{
			ReportError("insane call - js1 is NULL");
			return false;
		}
		if(!inst)
		{
			ReportError("insane call - peer inst is NULL");
			return false;
		}
		return true;
	}

	JS1 * get_js1()
	{
		if(!cls)
		{
			ReportError("get_js1() NULL cls\n");
			return NULL;
		}
		// j_class_plus is not included here ! // return cls->js1;
		return ((j_class_base*)cls)->js1;
	}

};

/*
	j_class_plus * get_class_plus( obj_ref_js * inst )
	{
		if(!inst) return NULL;
		JSObject * js_inst = inst->get_js_self();
		if(!js_inst) return NULL;
		j_class_plus * jsc_inst = get_class_plus( js_inst );
		return jsc_inst;
	}
*/

/*!
*/
struct j_cx_obj_peer : j_cx_obj_inst
{

	~j_cx_obj_peer(){}

	j_cx_obj_peer(
		JSContext *	_cx,
		uns		_argc,
		JS::Value *		_vp,
		bool		expect_peer = true
	)
	: j_cx_obj_inst( _cx, JS_THIS_OBJECT( _cx, _vp) )
	{
		/*
			inst is null during construction - see j_class_plus.cxx
		*/
		// the C++ instance of a JS class is the ClientData
		if(expect_peer && (!get_class_plus())) //  || !inst) )
		{
			throw dgb_fail("not a PEER object");
		}
	}

	j_cx_obj_peer(
		JSContext *	_cx,
		JSObject * 	_js_obj,
		bool		expect_peer = true
	)
	: j_cx_obj_inst( _cx, _js_obj )
	{
		/*
			inst is null during construction - see j_class_plus.cxx
		*/
		// the C++ instance of a JS class is the ClientData
		if(expect_peer && (!get_class_plus())) //  || !inst) )
		{
			throw dgb_fail("not a PEER object");
		}
	}

};

// #warning "DEPRECATED JS_RVAL(cx,vp) (*(vp)) // retvar = vp "
// #define RETVAR_FROM_VP(vp) (vp)

/*!
	functions that have a primary 'THIS' peer obj and a return type
	use this struct
*/
struct j_cx_obj_ret: public j_cx_obj_peer
{
	JS::Value *	retvar;
	j_cx_obj_ret (
		JSContext *	_cx,
		JSObject * 	_js_obj,
		JS::Value	*	_retvar
	)
	: j_cx_obj_peer( _cx, _js_obj )
	{
		retvar = _retvar;
	}

	void set_ret( str2 * s2 )
	{
		// extract plain STR0 from str2
		set_ret( str0(s2->str) );
	}

	void set_ret( str0 s )
	{
		// mk_str build a JS string
		INFO("'%s'", (STR0) s );
		set_ret( mk_str( (STR0) s ) );
	}

	void set_ret( JSString * retval )
	{
		// convert JSString pointer to JSVAL
		if(retval)
			*retvar = STRING_TO_JSVAL(retval);
		else {
			throw dgb_fail("set_ret NULL");
		}
	}

	void set_ret( double retval )
	{
		/*
			a double is bigger than a JSVAL register
			so it is allocated and its POINTER cast
			to a JSVAL
		*/
		// e_print("set_ret( (double) %f )\n", retval );

#if 0
		jsdouble * dp = JS_NewDouble(cx, retval );
		*retvar = DOUBLE_TO_JSVAL(dp);
#endif

#if 0
		JS_NewNumberValue( cx, retval, retvar );
#endif
		JS::Value jsv = JS_NumberValue( retval );
		set_ret( jsv );
	}

	void set_ret( int retval )
	{
		/*
			ints are compressed into the JSVAL itself

			so there is no alloc of starage,
			as happens for strings and doubles
		*/
		// e_print("set_ret( (int) %d )\n", retval );

		*retvar = INT_TO_JSVAL(retval);
	}

	void set_ret_bool( bool retval )
	{
		*retvar = BOOLEAN_TO_JSVAL(retval);
	}

	void set_ret( JSObject * retval )
	{
		/*
			the objects is already alloc'd as a JSObject
			so wrap its pointer into a JSVAL (low 2 bits == xx )
		*/
		if(retval)
			*retvar = OBJECT_TO_JSVAL(retval);
		else {
			throw dgb_fail("set_ret NULL");
		}
	}

	/*
		JSC is ALREADY SPECIFIC to the cx
		JSC can create JSVAL's from its allocated objects
	*/
	void set_ret( JS::Value retval )
	{
		*retvar = retval;
	}

//	void set_ret( j_class_base * jsc, obj_ref_js * inst );

	void set_ret( j_class_base * jsc, obj_ref_js * inst )
#if 1
	// THIS HAS circular includes - so loss of inline until adjusted
	{
		JSObject * jo = inst->js_self;
		if(!jo) jo = jsc->mk_js_self( inst );
		*retvar = OBJECT_TO_JSVAL( jo );
	}
#endif

};

/*!
*/
struct j_args_prop_id: public j_cx_obj_ret
{
	jsid	id;
	int	id_int;
	JSString * id_js_str;
	jsc_prop_list_item * jsc_prop; // my info about the prop // by_id only

	j_args_prop_id(
		JSContext *	_cx,
		JSObject * 	_js_obj,
		jsid		_id,
		JS::Value	*	_retvar
	);













	str0 prop_name( buffer1 & buf );
/*
	str0 prop_name() {
		// MAYBE id_js_str is NULL
		return j_cx::as_string( id_js_str );
		//	(STR0) args->as_string( args->id ) );
	}
*/
};

/*!
*/
struct j_args_prop_get: public j_args_prop_id
{
	j_args_prop_get(
		JSContext *	_cx,
		JSObject * 	_js_obj,
		jsid		_id,
		JS::Value	*	_retvar
	)
	: j_args_prop_id( _cx, _js_obj, _id, _retvar )
	{
	}

};

/*!
	prop_set is very much like prop_get, but with a typed name and in-val
*/
struct j_args_prop_set: public j_args_prop_id
{
	JS::Value	* val;

// #warning "TODO rework with new ENUM TYPE and all callers too"
/*
// https://developer.mozilla.org/en-US/docs/SpiderMonkey/JSAPI_Reference/JS_TypeOfValue
JS_TypeOfValue examines a specified JS value, v,
and returns its JS data type.
The return value is always one of
	JSTYPE_VOID,
	JSTYPE_OBJECT,
	JSTYPE_FUNCTION,
	JSTYPE_STRING,
	JSTYPE_NUMBER,
	JSTYPE_BOOLEAN.

??	_RegularExpression
??	_Date
??	UNDEFINED is VOID ??
??	INT -vs- FLOAT -vs- NUMBER (probably doesnt matter)
??	JSVAL_INT
??	JSVAL_DOUBLE
??	JSVAL_NULL
??	JSVAL_VOID

*/
	JSType val_type_tag()
	{
		// prefer something closer, without checks for cx, val, etc
		if(!val) {
			FAIL("val should not be null");
			return JSTYPE_VOID;
		}
		return JS_TypeOfValue( cx, *val );

//		1.8.0 did it an old way, 1.8.5 is different
//		if(!val) return 0;
//		if((*val)&1) return JSVAL_INT;
//		return( JSVAL_TAG( *val ) );
	}

	j_args_prop_set(
		JSContext *	_cx,
		JSObject * 	_js_obj,
		// there is no retvar - 
		jsid		_id,
		JS::Value	*	_val
	)
	: j_args_prop_id( _cx, _js_obj, _id, 0 )
	{
		val = _val;
	}
};

/*!
*/
struct j_args_func: public j_cx_obj_ret
{
	uns argc;
	JS::Value *argv;

	j_args_func(
		JSContext *	_cx,
		uns 		_argc, 
		JS::Value *		_vp 
	)
	: j_cx_obj_ret( _cx, JS_THIS_OBJECT(_cx,_vp), _vp )
	{
		argc = _argc;
		argv = JS_ARGV(_cx,_vp);;
	}

	j_args_func(
		const char * absent_parameter_dead_code,
		JSContext *	_cx,
		JSObject * 	_js_obj,
		uns 		_argc, 
		JS::Value *		_argv, 
		JS::Value *		_retvar 
	)
	: j_cx_obj_ret( _cx, _js_obj, _retvar )
	{
		argc = _argc;
		argv = _argv;
	}
};

/*!
*/
struct j_args_delfunc: public j_cx_obj_peer
{
	j_args_delfunc(
		JSContext *	_cx,
		JSObject * 	_js_obj
	)
	: j_cx_obj_peer( _cx, _js_obj )
	{
	}
	void del_obj_inst()
	{
		split_pair();
	}
	bool split_pair()
	{
		if( !inst ) {
			JS_ReportError(cx,"j_args_delfunc - inst not set\n");
			return false;
		}
		inst->js_self = NULL;
		JS_SetPrivate( js_obj, NULL );
		inst->ref_decr();
		if(0) e_print("del_Thing: js_obj %p -> inst %p\n", js_obj, inst );
		return true;
	}
};

/*!
	script.js: obj = new ClssName( args )
	jsapi.c: calls (j_class_plus::static)
		bool
		s_new_Thing(
			JSContext *cx,
			JSObject *obj, // pre-made VANILLA JSObject
			uns argc,
			JS::Value *argv,
			JS::Value *rval
		) ;

	js1> mod = new Stats_Module()
		CompileScript,
		ExecuteScript,
		InvokeConstructor( cx, CallArgs ) )
		CallJSNative( cx, argc, vp )
		s_new_Thing( cx, argc, vp ):


	see JS_InitClass(...)
		the CTOR must not use JS_THIS nut create object

	

*/

struct j_args_newfunc: public j_cx_obj
{
	uns argc;
	JS::Value *argv;

	j_args_newfunc(
		JSContext *	_cx,
		JSObject * 	_js_obj, // VANILLA JSObject
		uns 		_argc, 
		JS::Value *		_argv 
	)
	: j_cx_obj( _cx, _js_obj )
	{
		argc = _argc;
		argv = _argv;
	}

};

}; // namespace JSSM
#endif
