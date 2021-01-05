#ifndef JSSM_Context_H
#define JSSM_Context_H

#include "jssm_STUBS.h"
#include "str0.h"

// using namespace JS;

namespace JSSM {

struct j_cx : public GRP_lib_base2_js
{
	JSContext *cx;
	
	j_cx() {} ;
	j_cx( JSContext * _cx );
	JSString * mk_str( str0 s );
#if 0
	// was out before
	str0 as_string( JSString * js_str );
	str0 as_string( JSObject * js_obj );
	str0 as_string( Value js_val );
	str0 as_string( Value * js_val );
#endif

//	bool ReportError( str0 msg );
	bool ReportError( str0 msg, ... );
	bool ReportError_v( str0 msg, ... );

	bool put_string_utf8( buffer1 & buf, JSString * js_str ); 
	bool put_as_string_utf8( buffer1 & buf, JSString * js_str )
	{
		return put_string_utf8( buf, js_str );
	}
	bool put_as_string_utf8( buffer1 & buf, JS::Value js_val ); 
	// bool put_as_string_utf8( buffer1 & buf, JSObject * js_obj ); 
#if 0
	// above appends, following also call claear() 
	str0 as_string( JSString * js_obj, buffer1 & buf );
	str0 as_string( Value js_val, buffer1 & buf );
	str0 as_string( Value * js_val, buffer1 & buf );
	str0 as_string( JSObject * js_obj, buffer1 & buf );
	
	bool ok_as_bool( Value js_val, bool & var );
	bool ok_as_int( Value js_val, int & var );
	bool ok_as_double( Value js_val, double & var );

	static void set_JS_ReportError_gdb( bool val );
#endif
	
};


};
#endif

