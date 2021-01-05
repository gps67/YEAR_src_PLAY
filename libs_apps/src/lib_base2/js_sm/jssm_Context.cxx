
#include "jssm_Context.h"
#include "jsapi.h"
#include "dgb_fail.h"
#include "buffer1.h"

namespace JSSM {

static bool JS_ReportError_gdb = false;

//	bool j_cx::ReportError( str0 msg )
//	{
//		if(1) e_print( "%s", (STR0) msg );
//		JS_ReportError( cx, "%s", (STR0) msg );
//		return false;
//	}

	bool j_cx::ReportError( str0 fmt, ... )
	{
		va_list args;
		// gcc 3 generates an warning here
		// second parameter of `va_start' not last named argument
		// va_start( args, & fmt );
		// intel icpc sees error
		va_start( args, fmt );
		buffer1 buf(200);
		buf.vprint( fmt, args );
		va_end( args );
		if(0) e_print( "%s\n", (STR0) str0(buf) );
		JS_ReportErrorASCII( cx, "%s", (STR0) str0(buf) );
		// JS_ReportError( cx, "%s", (STR0) str0(buf) );
		if( JS_ReportError_gdb ) THROW_dgb_fail( (STR0) str0(buf) );
		return false;
	}

	bool j_cx::ReportError_v( str0 fmt, ... )
	{
		va_list args;
		// gcc 3 generates an warning here
		// second parameter of `va_start' not last named argument
		// see above
		va_start( args, fmt );
		buffer1 buf(200);
		buf.vprint( fmt, args );
		va_end( args );
		if(0) e_print( "%s\n", (STR0) str0(buf) );
		JS_ReportErrorASCII( cx, "%s", (STR0) str0(buf) );
		if( JS_ReportError_gdb ) THROW_dgb_fail( (STR0) str0(buf) );
		return false;
	}

	/*!
		Append string to buf (caller must clear first)
		buf.printf( "%s", JS_GetStringBytes( js_str );
		the 'utf8' is a lie, but gets you thinking

		TODO: to make it UTF8
		https://developer.mozilla.org/en-US/docs/Mozilla/Projects/SpiderMonkey/JSAPI_reference/JS_EncodeStringToBuffer
		To encode to UTF8, use
		JS::GetDeflatedUTF8StringLength and
		JS::DeflateStringToUTF8Buffer instead.

		except that recommendation from MDN is crap
		what is a FLAT string buffer - a deflated thing ?
	*/
	bool j_cx::put_string_utf8( buffer1 & buf, JSString * js_str )
	{
		int l = JS_GetStringEncodingLength( cx, js_str );
		if(!buf.get_space( l + 1 )) {
			ReportError( "get_space() failed");
			return FAIL_FAILED();
		}
		int l2 = JS_EncodeStringToBuffer(
			cx,
			js_str,				// the string to encode
			buf.gap_addr_signed_char(),	// write string here
			buf.space_avail()		// or truncate + NUL
		);
		buf.nbytes_used_add( l2 );
		// INFO("%s", (STR0) buf );
		return true;
	}


	/*!
		convert JS::Value to JSSttring and get its char *
	
	*/
	bool j_cx::put_as_string_utf8( buffer1 & buf, JS::Value js_val )
	{
		JSString * js_str;
		// if( JSVAL_IS_STRING( js_val ) )
		if( js_val.isString() )
			js_str = js_val.toString();
		else if( js_val.isObject() )
			js_str = js_val.toString();
		else
			js_str = js_val.toString();
	//		js_str = JS_ValueToString( cx, js_val ); // GC soon
			// GC on this new value
			// did the fancy JS to string thing
		return put_as_string_utf8( buf, js_str );
	}

	/*!
	bool j_cx::put_as_string_utf8( buffer1 & buf, JSObject * js_obj )
	{
		return put_as_string_utf8( buf, js_obj->toString() );
	}
	*/

	// str0 j_cx::as_string( JS::Value js_val, buffer1 & buf )


}; // namespace JSSM
