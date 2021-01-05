#ifndef j_JSNative_args_H
#define j_JSNative_args_H

#include "j_cx.h"

/*
	absorb the (temporary) args of a JSNative call
	delete on return from function

	bool f_JSNative(
		JSContext *     _cx,
		uintN           _argc,
		JS::Value *         _vp
	) {
		j_JSNative_args args( _cx, _argc, _vp, "f_Name", 1 );
		j_cx_obj_peer C

	}

	JSNative is a function
*/

/*
	I dont want to remold me to the new JS 1.8.5
	I want to throw out SpiderMonkey

	eg my old code base used JS::Value * retval
	But that is deprecated, and now I have to use:

		JS_SET_RVAL(cx, vp, value)

	This is annoyiong pain, but what really hurts is that 
	there is no change document that sets out what I need to do
	to move from 1.8.0 to 1.8.5.

	You have to figure it out by bumping into the problems

https://developer.mozilla.org/en-US/docs/SpiderMonkey/JSAPI_Reference/JSNative

	SEE OLD CODE:

	j_args_func_Misc
	j_class_plus
*/

struct j_JSNative_args : public j_cx
{
	uintN           argc;
	JS::Value *         vp;
	const char * 	func_name;

	j_JSNative_args(
		JSContext *     _cx,
		uintN           _argc,
		JS::Value *         _vp,
		const char * 	_func_name = "function"
	}
	: j_cx( _cx )
	, argc( _argc )
	, vp( _vp )
	, func_name( _func_name )
	{
	}

	// JS_CALLEE // deprecated in strict anyway ?
	JS::Value get_this_jsval() { return JS_THIS( cx, vp ); }
	JSObject * get_this_JSObject() { return JS_THIS_OBJECT( cx, vp ); }
	JS::Value * get_argv_jsval(){ return JS_ARGV( cx, vp ); }
	// JS_RVAL // get previously set return value
	void set_ret_val( JS::Value val ) { JS_SET_RVAL( cx, vp, val ); }

};


#endif
