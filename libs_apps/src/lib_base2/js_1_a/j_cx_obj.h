#ifndef j_cx_obj_H
#define j_cx_obj_H

#include "j_confg.h"
#include "e_print.h"
#include "jsapi.h"
#include "str0.h"

/*
*/
#include "obj_ref_js.h"
#include "j_cx.h"
class  obj_ref_js;
// #include "j_class_base.h"
class j_class_base;

#include "JS1.h"

extern void gdb_break_point();

namespace JSSM {

	class j_class_base;
	class j_class_plus;

/*!
	j_cx_obj is useful for plain JSSM as well as peer_pairs
*/
struct j_cx_obj : public j_cx
{
	//! The JSObject of the call
	JSObject * js_obj;

	j_cx_obj(
		JSContext *	_cx,
		JSObject *	_js_obj
	);
	void SetPrivate( void * ClientData )
	{
		JS_SetPrivate( js_obj, ClientData );
	}
	void * GetPrivate() const
	{
		return JS_GetPrivate( js_obj );
	}
	/*!
		j_base_class does the convertion to j_base_class *
	*/
	const JSClass * GET_CLASS() const
	{
		return JS_GetClass( js_obj );   
	}

	bool SetProperty( str0 name, JS::Value * val )
	{
		return JS_SetProperty( cx, js_obj, name, val );
	}
	bool SetProperty( str0 name, JSObject * val_obj )
	{
		// I've just changed this to AMP - crash elsewhere ...
		JS::Value val = OBJECT_TO_JSVAL( val_obj) ;
		return SetProperty( name, & val ) ;
	}
	bool DeleteProperty( str0 name )
	{
		return JS_DeleteProperty( cx, js_obj, name );
	}
	bool GetProperty( str0 name, JS::Value * val )
	{
		return JS_GetProperty( cx, js_obj, name, val );
	}
	str0 as_str0( buffer1 & buf )
	{
		return j_cx::as_string( js_obj, buf ); // cx + obj
	}
	j_class_plus * get_class_plus();
};


/////////////////////////////////////////////////////////////////////
struct j_cx_obj_inst;
};

#endif

