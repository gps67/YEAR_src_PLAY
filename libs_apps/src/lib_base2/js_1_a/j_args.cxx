#include "j_args.h"

#include "j_class_plus.h" // get_class_plus()
#include "buffer1.h" // vprint to str
#include "dgb_fail.h" // vprint to str
#include "dgb.h" // 

extern void gdb_break_point();

namespace JSSM {



/*!
*/
	j_cx_obj::j_cx_obj(
		JSContext *	_cx,
		JSObject *	_js_obj
	)
	: j_cx( _cx )
	{
		js_obj = _js_obj;
		if(!_js_obj)
		{
			throw dgb_fail( "new j_cx_obj(cx,obj) NULL js_OBJ");
		}
	}

	j_class_plus * j_cx_obj::get_class_plus()
	{
		return j_class_plus::get_class_plus( *this );
	}

///////////

#warning "MISTAKE - maybe - is id really not an int nor string but an idval?"

	j_args_prop_id::j_args_prop_id(
		JSContext *	_cx,
		JSObject * 	_js_obj,
		jsid		_id,
		JS::Value	*	_retvar
	)
	: j_cx_obj_ret( _cx, _js_obj, _retvar )
	, jsc_prop( NULL )
	{
		id = _id;
		id_int = -1;
		id_js_str = NULL;
		JS::Value idval;
		if(! JS_IdToValue( cx, id, & idval ))
		{
			FAIL("JS_IdToValue(id)");
			return;
		}
		if( JSVAL_IS_INT( idval ) ) {
		{
			id_int = (JSVAL_TO_INT(idval));                                           
			// needs header, hence in .cxx not in .h
			if( cls ) {
				// check that i<n // ie was allocated
				jsc_prop = cls->prop_list.get( id_int );
				if(!jsc_prop)
				{
				}
			}
		}
		} else if( JSVAL_IS_STRING( idval ) ) {
			id_js_str = JSVAL_TO_STRING(idval);
		} else {
			INFO("not INT not STRING cast convert to string");
			// gdb_invoke(1);
			// gdb_break_point();
			id_js_str = JS_ValueToString( cx, idval );
		}
	}

	str0 j_args_prop_id:: prop_name( buffer1 & buf )
	{
		bool b = false;
		if(b) INFO( "id_int = %d", id_int );
		if(b) INFO( "id = 0x%x", id );
		b = true;
		if( id_js_str ) {
			const char * s = j_cx::as_string( id_js_str, buf );
			if(b) INFO("id_js_str as_string() %s",  s );
			if(1) {
				char c = buf.buff[0];
				if(( '0' <= c ) && (c<='9')) {
					WARN("number is not name %s", (STR0) buf);
					// gdb_invoke(1);
				}
			}
			return s;
		}
		WARN( "NOT id_js_str" );
		gdb_invoke(1);
		return "DUNNO";

	}


#if 0
	void j_cx_obj_ret::set_ret( j_class_base * jsc, obj_ref_js * inst )
	{
		JSObject * jo = inst->js_self;
		if(!jo) jo = jsc->mk_js_self( inst );
		*retvar = OBJECT_TO_JSVAL( jo );
	}
#endif


}; // namespace JSSM
