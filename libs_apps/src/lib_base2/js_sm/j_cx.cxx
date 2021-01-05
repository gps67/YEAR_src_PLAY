#include "j_cx.h"
#include "dgb_fail.h"
#include "buffer1.h"

using namespace JSSM;

static bool JS_ReportError_gdb = false;

	void j_cx::set_JS_ReportError_gdb( bool val )
	{
		JS_ReportError_gdb = val;
	}

	j_cx::j_cx(
		JSContext *	_cx
	)
	{
		cx = _cx;
		if(!cx)
		{
			throw dgb_fail( "new j_cx(cx) NULL cx" );
		}
	}

	JSString * j_cx::mk_str( str0 s )
	{
		if(!s) s = "";
	if(1)	INFO("'%s'", (STR0) s );
		int l = s.str_len();
		return JS_NewStringCopyN( cx, s, l );
	}
#if 1
	/*
		yet another example why returning a string is nonsense
		and a STRETCH buffer should be provided to print to
		in JS 1.8.0 the string of the JSObject was borrowed
		which is OK for a short while, but only microseconds!
		return JS_GetStringBytes( js_str ); // deprecated

		Note: JS_GetStringBytes() and JS_GetStringBytesZ()
		have both been removed as of JavaScript 1.8.5 (Firefox 4).
		Instead, you should use
		JS_EncodeString(),
		JS_GetStringEncodingLength(), and
		JS_EncodeStringToBuffer().

	*/



	/*!
		get (char *) of JSString.
	
		Caller must provide a buffer,
		Caller must convert to JSString
	*/
	str0 j_cx::as_string( JSString * js_str, buffer1 & buf )
	{
		buf.clear();
		if(! put_string_utf8( buf, js_str )) {
			FAIL_FAILED();
			return "(as_string FAILED)";
		}
		return buf; // pointer to memory held within buf
	}

	/*!
		convert JSObject to JS::Value and call again
	*/
	str0 j_cx::as_string( JSObject * js_obj, buffer1 & buf )
	{
		JS::Value jsv = OBJECT_TO_JSVAL( js_obj) ;
		return as_string( jsv, buf );
	}

	/*! 
		follow pointer and call as_string() 
	*/
	str0 j_cx::as_string( JS::Value * js_val, buffer1 & buf )
	{
		if( !js_val ) return "(NULL)";
		return as_string( *js_val, buf );
	}

/*
	jsobj.cpp
	if (const jschar *chars = str->getChars(NULL))
		dumpChars(chars, str->length());

*/
#endif

	bool j_cx::ok_as_bool( JS::Value js_val, bool & var )
	{
		if( JSVAL_IS_BOOLEAN( js_val ) )
		{
			var = JSVAL_TO_BOOLEAN( js_val );
			return true;
		} else {
			return false;
			// js_str = JS_ValueToString( cx, js_val ); // GC soon
		}
		// return JS_GetStringBytes( js_str );
	}

	bool j_cx::ok_as_int( JS::Value js_val, int & var )
	{
		if( JSVAL_IS_INT( js_val ) )
		{
			var = JSVAL_TO_INT( js_val );
			return true;
		} else {
			return false;
		}
	}

	bool j_cx::ok_as_double( JS::Value js_val, double & var )
	{
		if( JSVAL_IS_DOUBLE( js_val ) )
		{
			// 1.8.0 returned NULL if it wasnt
			// 1.8.5 retunrs value not pointer
			var =  JSVAL_TO_DOUBLE( js_val );
			return true;
		} else {
			return false;
		}
	}

