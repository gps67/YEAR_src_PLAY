#ifndef j_cx_H
#define j_cx_H

#include "e_print.h"
#include "str0.h"
// stubs ?
#include "buffer1.h"

// STUBS
// j_config.h // #include "js_confg.h"
#include "JS1.h" 
#include "jsapi.h"
// #include "js_confg.h"
// #warning new STUBS here
namespace js {
	// struct JSContext;
	// struct JSString;
};
using namespace js;
namespace JS {
	// 
};
using namespace JS;


namespace JSSM {

/*!
	j_cx wraps JSContext which is the thread/session context.
*/
struct j_cx : public GRP_lib_base2_js
{
	JSContext *cx;
	
	j_cx() {} ;
	j_cx( JSContext * _cx );
	JSString * mk_str( str0 s );
#if 0
	str0 as_string( JSString * js_str );
	str0 as_string( JSObject * js_obj );
	str0 as_string( Value js_val );
	str0 as_string( Value * js_val );
#endif
	bool put_string_utf8( buffer1 & buf, JSString * js_str ); 
	// above appends, following also call claear() 
	str0 as_string( JSString * js_obj, buffer1 & buf );
	str0 as_string( Value js_val, buffer1 & buf );
	str0 as_string( Value * js_val, buffer1 & buf );
	str0 as_string( JSObject * js_obj, buffer1 & buf );
	
	bool ok_as_bool( Value js_val, bool & var );
	bool ok_as_int( Value js_val, int & var );
	bool ok_as_double( Value js_val, double & var );
	
//	bool ReportError( str0 msg );
	bool ReportError( str0 msg, ... );
	bool ReportError_v( str0 msg, ... );

	static void set_JS_ReportError_gdb( bool val );
	
};

}; // NAMESPACE
#endif

